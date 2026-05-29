//===- aVMP.cpp - 虚拟机保护混淆Pass ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/InlineCost.h"
#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/ValueHandle.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/ModuleUtils.h" 
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/Transforms/Obfuscation/aVMP.h"
#include "llvm/Transforms/Obfuscation/vm.h"
#include <assert.h>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#define ENABLE_VMP
#ifdef ENABLE_VMP

using namespace llvm;
using namespace std;
// code and data segment
// extern GlobalVariable * gv_code_seg;
// extern GlobalVariable * gv_data_seg;
// extern GlobalVariable * ip;
// extern GlobalVariable * data_seg_addr;
GlobalVariable * gv_code_seg;
GlobalVariable * gv_data_seg;
GlobalVariable * ip;
GlobalVariable * data_seg_addr;
GlobalVariable * code_seg_addr;
GlobalVariable *exception_thrown;
GlobalVariable *exception_ptr_global;
GlobalVariable *exception_selector_global;
GlobalVariable *last_br_from_bb_id;
GlobalVariable *current_bb_id;

// data and code seg size
#define VM_CODE_SEG_SIZE 5000
#define VM_DATA_SEG_SIZE 5000

// Interpreter
// extern Function * govm_interpreter;
Function * govm_interpreter;


// Opcode 
#define NOP_OP              0x00
#define ALLOCA_OP           0x01
#define LOAD_OP             0x02
#define STORE_OP            0x03
#define BinaryOperator_OP   0x04
#define GEP_OP              0x05
#define CMP_OP              0x06
#define CAST_OP             0x07
#define BR_OP               0x08
#define Call_OP             0x09
#define Ret_OP              0x0A
#define SWITCH_OP           0x0B
#define INSERTVALUE_OP      0x0C
#define EXTRACTVALUE_OP     0x0D
#define PHI_OP              0x0E
#define SELECT_OP           0x0F
#define LANDINGPAD_OP       0x10
#define RESUME_OP           0x11
#define INDIRECTBR_OP       0x12
#define EXTRACTELEMENT_OP   0x13
#define INSERTELEMENT_OP    0x14
#define SHUFFLEVECTOR_OP    0x15
#define FREEZE_OP           0x16

#define OP_TOTAL            0x16





/* ********************************************************************
*   PROJECT_LOGUTILS_H
***********************************************************************
*/

class LogUtils {
    public:
        static std::string *log_title(const char *);
};

std::string *LogUtils::log_title(const char *title) {
    int width = 64-4;
    int title_len = strlen(title);
    int pos = width/2-title_len/2;
    if (title_len%2) {
        pos -= 1;
    }

    assert(strlen(title) <= 60);

    std::string *res = new std::string("");
    *res += "\n\n################################################################\n";
    *res += "##                                                            ##\n";
    *res += "##                                                            ##\n";
    *res += "##";
    for (int i = 0; i < pos; i++) {
        *res += " ";
    }
    *res += title;
    for (int i = 0; i < width-pos-title_len; i++) {
        *res += " ";
    }
    *res += "##\n";

    *res += "##                                                            ##\n";
    *res += "##                                                            ##\n";
    *res += "################################################################\n\n";

    return res;
}


/* ********************************************************************
*   GOVMTRANSLATOR_H
***********************************************************************
*/


#define GOVTRANSLATOR_DEBUG

/***
 * The main class that modify the callsite of vm-function.
 */
class GOVMTranslator {
    
    public:
        GOVMTranslator(Function * F) {
            this->Mod = F->getParent();
            this->F = F;
            this->modDataLayout = const_cast<DataLayout *>(&this->Mod->getDataLayout());
            this->pointer_size = modDataLayout->getPointerSize();  // 动态获取指针大小

            // construct function and global variables
            init();
        }

        Module * Mod;
        Function * F;
        DataLayout * modDataLayout;
        unsigned pointer_size;  // 动态获取的指针大小,支持不同架构

        // Global variables for VM interpreter
        GlobalVariable *gv_code_seg;
        GlobalVariable *gv_data_seg;
        GlobalVariable *ip;
        GlobalVariable *data_seg_addr;
        GlobalVariable *code_seg_addr;
        GlobalVariable *exception_thrown;
        GlobalVariable *exception_ptr_global;
        GlobalVariable *exception_selector_global;
        GlobalVariable *last_br_from_bb_id;
        GlobalVariable *current_bb_id;

        // construct callinst_handler to interprete callinst 
        Function * callinst_handler;
        BasicBlock * callinst_handler_conBBL;
        BasicBlock * callinst_handler_entryBB;  // 保存entryBB用于添加if-else
        Value * targetfunc_id;
        unsigned callinst_handler_curr_idx;
        std::map<Function *, unsigned> function_id_map;

        // hex code
        std::vector<uint8_t> vm_code;

        // map
        std::map<Value *, int> value_map;
        std::map<BasicBlock *, int> basicblock_map;
        std::vector<pair<int, BasicBlock *>> br_map;
        std::map<CallBase *, long long> callinst_map;
        
        // LLVM 21: SwitchInst支持
        std::vector<std::tuple<int, BasicBlock *, int>> switch_map;  // (code_pos, target_bb, case_value)

        // collect global variable
        std::map<GlobalVariable *, int> gv_value_map;

        // current offset in data_seg
        int curr_data_offset = 0;


        virtual bool run ();
        virtual void handle_inst(Instruction *);
        virtual void construct_gv();

        // create callinst_handler function, add instructions to callinst_handler and create ret basicblock to callinst_handler 
        virtual void setup_callinst_handler();
        virtual void handle_callinst(CallBase * inst, long long curr_func_id);
        virtual void finish_callinst_handler();

        // get a NULL value
        std::vector<uint8_t> get_null_value() {
            return std::vector<uint8_t>(2 + pointer_size);
        }

        // pack a value
        #define GET_PACK_VALUE(value) (packValue(value, &value_map))


        // construct function and global variables
        void init() {
            // construct_gv();
            setup_callinst_handler();  // 创建call_handler

            // encrypt opcode
            init_xorshift32();
        }

        Function *get_callinst_handler() {
            return this->callinst_handler;
        }

        GlobalVariable *get_gv_data_seg() { return this->gv_data_seg; }
        GlobalVariable *get_gv_code_seg() { return this->gv_code_seg; }
        GlobalVariable *get_ip() { return this->ip; }
        GlobalVariable *get_data_seg_addr() { return this->data_seg_addr; }
        GlobalVariable *get_code_seg_addr() { return this->code_seg_addr; }
        int get_data_seg_size() { return this->curr_data_offset; }

        // insert arg into res.end
        template <typename T, typename Arg>
        void vector_appender(T &res, Arg arg)
        {
            res.insert(res.end(), arg.begin(), arg.end());
        }

        // combine multiple vector, result in res
        template <typename T, typename... Args>
        void ins_to_hex(T &res, Args ... arg)
        {
            (void)std::initializer_list<int>{ (vector_appender(res, arg), 0)... };
        }

        std::map<GlobalVariable *, int> *get_gv_value_map() {
            return &gv_value_map;
        }

        std::map<Value *, int> *get_value_map() {
            return &value_map;
        }


        // insert a value to value_map
        void insert_to_value_map(std::map<Value *, int> * value_map, Value * value, int offset){
            value_map->insert(pair<Value *, int>(value, offset));
        }


        void dump_vector(std::vector<uint8_t> v){
            if (isIRObfuscationDebugEnabled()) {
                for(auto i : v){
                    errs() << int(i) << " ";
                }
                errs() << "\n";
            }
        }


        // pack a int to vector<uint8_t>(4)
        std::vector<uint8_t> p32(int int32){
            std::vector<uint8_t> tmp;
            tmp.push_back(int32 & 0xFF);
            tmp.push_back((int32 >> 8) & 0xFF);
            tmp.push_back((int32 >> 16) & 0xFF);
            tmp.push_back((int32 >> 24) & 0xFF);
            return tmp;
        }

        // pack a int to vector<uint8_t>(1-8)
        std::vector<uint8_t> pack(long long int int_n, int size){

            if (size > 8){
                // long long is 64bit
                assert(0);
            }

            std::vector<uint8_t> tmp;
            while(size > 0){
                tmp.push_back((int_n) & 0xFF);
                size --;
                int_n = int_n >> 8;
            }
            return tmp;
        }

        // encrypt opcode, use xorshift
        uint32_t xorshift32_state = 0;
        uint32_t xorshift32_seed = 0;

        /* encrypt vm_code */
        // mark seed for each basicblock
        std::map<uint32_t, pair<uint32_t, uint32_t>> vm_code_seed_map;

        void init_xorshift32() {
            srand(time(0));
        }

        uint32_t gen_xorshift32_seed() {
            for (int _ = 0; _ < 10; _++) {
                xorshift32_seed ^= rand();
            }
            return xorshift32_seed;
        }

        /* The state word must be initialized to non-zero */
        uint32_t xorshift32(uint32_t *state)
        {
            /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
            uint32_t x = *state;
            x ^= x << 13;
            x ^= x >> 17;
            x ^= x << 5;
            return *state = x;
        }

        uint32_t vm_code_seed_setup() {
            uint32_t res = gen_xorshift32_seed();

            std::vector<uint8_t> hex_code;
            ins_to_hex(hex_code, pack(res, sizeof(uint32_t)));
            vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

            return res;
        }

        uint32_t opcode_seed_setup() {
            xorshift32_seed = gen_xorshift32_seed();
            xorshift32_state = xorshift32_seed;

            errs() << "[opcode_seed_setup] xorshift32_seed=" << xorshift32_seed << " xorshift32_state=" << xorshift32_state << "\n";

            std::vector<uint8_t> hex_code;
            ins_to_hex(hex_code, pack(xorshift32_seed, sizeof(uint32_t)));
            vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
            
            return xorshift32_seed;
        }

        void encrypt_vm_code() {
            for (auto p: vm_code_seed_map) {
                uint32_t vm_code_seed = p.first;
                for (uint32_t addr = p.second.first; addr < p.second.second; addr++) {
                    vm_code[addr] ^= (xorshift32(&vm_code_seed) & 0xFF);
                }
            }
        }

        // pack one byte opcode
        std::vector<uint8_t> pack_op(uint8_t op){
            uint8_t res = 0;
            std::vector<uint8_t> his;
            const int MAX_RETRIES = 1000;
            int total_retries = 0;
            
            for (int i = 0; i < op; i++) {
                if (total_retries >= MAX_RETRIES) {
                    res = xorshift32(&xorshift32_state) & 0xFF;
                    break;
                }
                
                uint8_t tmp = xorshift32(&xorshift32_state) & 0xFF;
                if (find(his.begin(), his.end(), tmp) == his.end()) {
                    his.push_back(tmp);
                    res = tmp;
                }
                else {
                    i--;
                    total_retries++;
                }
            }
            errs() << "[pack_op] op=" << (int)op << " res=" << (int)res << " xorshift32_state=" << xorshift32_state << "\n";
            return pack(res, 1);
        }

        // pack a Constant to a vector
        std::vector<uint8_t> pack_const_value(Value * const_value){
            std::vector<uint8_t> res;
            Type * type = const_value->getType();
            int type_size = modDataLayout->getTypeAllocSize(type);
            int64_t value = 0;
            bool handled = false;

            if (type->isIntegerTy()){
                if (ConstantInt* CI = dyn_cast<ConstantInt>(const_value)) {
                    if (CI->getBitWidth() <= 64) {
                        value = CI->getSExtValue();
                        handled = true;
                    }
                } else if (isa<UndefValue>(const_value) || isa<PoisonValue>(const_value)) {
                    value = 0;
                    handled = true;
                }
            }

            if (!handled) {
                if (ConstantFP *CFP = dyn_cast<ConstantFP>(const_value)) {
                    APInt api = CFP->getValueAPF().bitcastToAPInt();
                    value = api.getZExtValue();
                    handled = true;
                }
            }

            if (!handled && isa<ConstantPointerNull>(const_value)) {
                value = 0;
                handled = true;
            }

            if (!handled && (isa<UndefValue>(const_value) || isa<PoisonValue>(const_value))) {
                value = 0;
                handled = true;
            }

            if (!handled) {
                if (isIRObfuscationDebugEnabled()) {
                    errs() << "Unsport const value: " << *const_value << "\n";
                }
                value = 0;
            }

            res = pack(value, type_size);

            return res;
        }


        // pack type to a vector(2)
        // {size, TypeID}
        std::vector<uint8_t> type_to_hex(Type * type){
            std::vector<uint8_t> res;
            res.push_back(modDataLayout->getTypeAllocSize(type));
            res.push_back(type->getTypeID());
            return res;
        }


        // pack a value
        std::vector<uint8_t> packValue(Value * value, std::map<Value *, int> * value_map) {
            std::vector<uint8_t> res;
            std::vector<uint8_t> packed;
            std::vector<uint8_t> packType = type_to_hex(value->getType());
            if(ConstantData* CD = dyn_cast<ConstantData>(value)){
                packed = pack_const_value(value);
            }
            else if(ConstantExpr* CE = dyn_cast<ConstantExpr>(value)){
                if (CE->getOpcode() == Instruction::GetElementPtr) {
                    if (GlobalVariable *GV = dyn_cast<GlobalVariable>(CE->getOperand(0))) {
                        if (gv_value_map.find(GV) == gv_value_map.end()) {
                            gv_value_map.insert({GV, curr_data_offset});
                            insert_to_value_map(value_map, GV, curr_data_offset);
                            int res_size = modDataLayout->getTypeAllocSize(GV->getValueType());
                            curr_data_offset += res_size;
                        }
                        packed = pack((*value_map)[GV], pointer_size);
                    } else {
                        packed = pack(0, pointer_size);
                    }
                } else if (CE->getOpcode() == Instruction::BitCast) {
                    return packValue(CE->getOperand(0), value_map);
                } else {
                    packed = pack(0, pointer_size);
                }
                packType[1] = 0;
            }
            else{
                // if value not in map
                if (value_map->find(value) == value_map->end()) {
                    // check value is not a GlobalVariable
                    if (GlobalVariable *gv = dyn_cast<GlobalVariable>(value)) {
                        // is a GlobalVariable and not in value_map
                        // put it into value_map
                        insert_to_value_map(value_map, value, curr_data_offset);

                        // also put it into gv_value_map
                        gv_value_map.insert(pair<GlobalVariable *, int>(gv, curr_data_offset));

                        int res_size = modDataLayout->getTypeAllocSize(gv->getValueType());
                        curr_data_offset += res_size;
                    }
                    else {
                        assert(value_map->find(value) != value_map->end());
                    }
                }

                packed = pack((*value_map)[value], pointer_size);
                // variableï¼Œpacktype->TypeID=0
                packType[1] = 0;
            }

            res.insert(res.end(), packType.begin(), packType.end());
            res.insert(res.end(), packed.begin(), packed.end());

            return res;
        }

};




/* ********************************************************************
*   GOVMTRANSLATOR_CPP
***********************************************************************
*/

// GlobalVariable * gv_code_seg;
// GlobalVariable * gv_data_seg;
// GlobalVariable * ip;
// GlobalVariable * data_seg_addr;
// GlobalVariable * code_seg_addr;

void GOVMTranslator::construct_gv() {
    // construct code global array from vm_code
    
    // set Initializer for gv_code_seg
    ArrayRef<uint8_t> code_seg_arrayref(vm_code);
    Constant * code_seg_init = ConstantDataArray::get(Mod->getContext(), code_seg_arrayref);

    ArrayType * code_seg_type = ArrayType::get(IntegerType::get(Mod->getContext(), 8), vm_code.size());
    // ArrayType * code_seg_type = ArrayType::get(IntegerType::get(Mod->getContext(), 8), VM_CODE_SEG_SIZE);
    // global
    gv_code_seg = new GlobalVariable(
                                                    /*Module=*/*Mod,
                                                    /*Type=*/code_seg_type,
                                                    /*isConstant=*/true,
                                                    /*Linkage=*/GlobalValue::InternalLinkage,
                                                    /*Initializer=*/code_seg_init, // has initializer, specified
                                                                                    // below
                                                    /*Name=*/"gv_code_seg_"+F->getName());
    


    // construct data global array
    std::vector<uint8_t> data_seg_vector(curr_data_offset);
    // std::vector<uint8_t> data_seg_vector(VM_DATA_SEG_SIZE);
    ArrayRef<uint8_t> data_seg_arrayref(data_seg_vector);
    Constant * data_seg_init = ConstantDataArray::get(Mod->getContext(), data_seg_arrayref);
    ArrayType * data_seg_type = ArrayType::get(IntegerType::get(Mod->getContext(), 8), curr_data_offset);
    // ArrayType * data_seg_type = ArrayType::get(IntegerType::get(Mod->getContext(), 8), VM_DATA_SEG_SIZE);
    // global
    gv_data_seg = new GlobalVariable(
                                                    /*Module=*/*Mod,
                                                    /*Type=*/data_seg_type,
                                                    /*isConstant=*/false,
                                                    /*Linkage=*/GlobalValue::InternalLinkage,
                                                    /*Initializer=*/data_seg_init, // has initializer, specified
                                                                                    // below
                                                    /*Name=*/"gv_data_seg_"+F->getName());


    // ip
    Constant *ip_initGV = ConstantInt::get(Type::getInt32Ty(Mod->getContext()), 0);
    ip = new GlobalVariable(*Mod, Type::getInt32Ty(Mod->getContext()), 
                false,  GlobalValue::InternalLinkage, 
                ip_initGV, "ip_"+F->getName());

    // data_seg_addr
    Constant *data_seg_addr_initGV = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
    data_seg_addr = new GlobalVariable(*Mod, Type::getInt64Ty(Mod->getContext()), 
                false,  GlobalValue::InternalLinkage, 
                data_seg_addr_initGV, "data_seg_addr_"+F->getName());

    // code_seg_addr
    Constant *code_seg_addr_initGV = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
    code_seg_addr = new GlobalVariable(*Mod, Type::getInt64Ty(Mod->getContext()), 
                false,  GlobalValue::InternalLinkage, 
                code_seg_addr_initGV, "code_seg_addr_"+F->getName());

    // exception_thrown
    Constant *exc_init = ConstantInt::get(Type::getInt8Ty(Mod->getContext()), 0);
    exception_thrown = new GlobalVariable(*Mod, Type::getInt8Ty(Mod->getContext()),
        false, GlobalValue::InternalLinkage, exc_init, "exception_thrown_"+F->getName());

    // exception_ptr_global (void*)
    Constant *exc_ptr_init = Constant::getNullValue(Type::getInt64Ty(Mod->getContext()));
    exception_ptr_global = new GlobalVariable(*Mod, Type::getInt64Ty(Mod->getContext()),
        false, GlobalValue::InternalLinkage, exc_ptr_init, "exception_ptr_"+F->getName());

    // exception_selector_global
    Constant *exc_sel_init = ConstantInt::get(Type::getInt32Ty(Mod->getContext()), 0);
    exception_selector_global = new GlobalVariable(*Mod, Type::getInt32Ty(Mod->getContext()),
        false, GlobalValue::InternalLinkage, exc_sel_init, "exception_selector_"+F->getName());

    // last_br_from_bb_id
    Constant *last_bb_init = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
    last_br_from_bb_id = new GlobalVariable(*Mod, Type::getInt64Ty(Mod->getContext()),
        false, GlobalValue::InternalLinkage, last_bb_init, "last_br_from_bb_id_"+F->getName());

    // current_bb_id
    Constant *curr_bb_init = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
    current_bb_id = new GlobalVariable(*Mod, Type::getInt64Ty(Mod->getContext()),
        false, GlobalValue::InternalLinkage, curr_bb_init, "current_bb_id_"+F->getName());
}

void GOVMTranslator::setup_callinst_handler() {
    // collect dispatch function args type 
    std::vector<Type*> FuncTy_args;
    // param: targetfunc_id_value
    FuncTy_args.push_back(Type::getInt64Ty(Mod->getContext()));

    // get dispatch function type
    FunctionType* FuncTy = FunctionType::get(
        /*Result=*/Type::getVoidTy(this->Mod->getContext()),  // returning void
        /*Params=*/FuncTy_args,  
        /*isVarArg=*/false);
    // Constant *tmp = Mod->getOrInsertFunction("",FuncTy);
    Constant *tmp = Function::Create(FuncTy, llvm::GlobalValue::LinkageTypes::InternalLinkage, "vm_interpreter_callinst_dispatch_"+F->getName(), Mod);
    Function *func =  cast<Function>(tmp);
    // func->setLinkage(llvm::GlobalValue::LinkageTypes::InternalLinkage);

    // create entry BasicBlock
    BasicBlock *entryBB = BasicBlock::Create(func->getContext(), "entryBB", func);
    IRBuilder<> IRBentryBB(entryBB);

    // Store params
    Value *target_id_value;
    for (auto arg = func->arg_begin(); arg != func->arg_end(); arg++) {
        Value *tmparg = &*arg;
        if (arg == func->arg_begin()) {
            // targetfunc_id_value
            Value *paramPtr = IRBentryBB.CreateAlloca(Type::getInt64Ty(Mod->getContext()));
            IRBentryBB.CreateStore(tmparg, paramPtr);
            target_id_value = IRBentryBB.CreateLoad(Type::getInt64Ty(Mod->getContext()), paramPtr);
        } 
    }

    // 不在这里创建返回指令，会在handle_callinst中添加条件跳转
    // 最后一个基本块会在finish_callinst_handler中添加返回指令

    this->callinst_handler_curr_idx = 0;

    this->callinst_handler = func;
    this->callinst_handler_conBBL = entryBB;  // 使用entryBB作为conBBL

    this->targetfunc_id = target_id_value;
    
    this->callinst_handler_entryBB = entryBB;
}

void GOVMTranslator::finish_callinst_handler() {
    // 为最后一个基本块添加返回指令
    IRBuilder<> IRB(this->callinst_handler_conBBL);
    IRB.CreateRetVoid();
}

void GOVMTranslator::handle_callinst(CallBase *inst, long long curr_func_id) {

    // errs() << "[handle_callinst] Processing callinst #" << curr_func_id << "\n";
    
    if (!this->callinst_handler_conBBL) {
        // errs() << "[handle_callinst] ERROR: callinst_handler_conBBL is null!\n";
        return;
    }
    
    IRBuilder<> IRBcon(this->callinst_handler_conBBL);

    // firstly,  we need to unpack function args
    // errs() << "[handle_callinst] Unpacking args, arg_size=" << inst->arg_size() << "\n";
    std::vector<Value *> target_func_args;
    for (unsigned idx = 0; idx < inst->arg_size(); idx++){
        // errs() << "[handle_callinst] Processing arg " << idx << "\n";
        Value * currarg = inst->getArgOperand(idx);

        // if value is a constant, use it directly
        if(isa<Constant>(currarg)){
            // errs() << "[handle_callinst] Arg " << idx << " is constant\n";
            target_func_args.push_back(currarg);
            continue;
        }

        if (value_map.find(currarg) == value_map.end()) {
            // errs() << "[VMP] ERROR: arg not found in value_map for call: " << *inst << "\n";
            target_func_args.push_back(UndefValue::get(currarg->getType()));
            continue;
        }
        // errs() << "[handle_callinst] Arg " << idx << " found in value_map\n";
        unsigned curroffset = value_map[currarg];

        // construct load
        ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
        Value * offset_value = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), curroffset);
        Value * gepinst = IRBcon.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, offset_value}, "");

        // 对于指针类型，gepinst就是指向data_seg中存储指针值的位置
        // 需要加载这个指针值（实际地址）
        if (currarg->getType()->isPointerTy()) {
            // 从data_seg加载指针值（实际地址）
            Value * ptr_addr = IRBcon.CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));
            Value * actual_ptr = IRBcon.CreateLoad(PointerType::get(Mod->getContext(), 0), ptr_addr);
            target_func_args.push_back(actual_ptr);
        } else {
            // 非指针类型，从data_seg加载值
            Value * ptr = IRBcon.CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));
            Value * arg = IRBcon.CreateLoad(currarg->getType(), ptr);
            target_func_args.push_back(arg);
        }
    }


    // errs() << "[handle_callinst] Args done, creating callFunction BB\n";
    // secondly, we create a new basic block to construct callinst
    BasicBlock *callFunction = BasicBlock::Create(Mod->getContext(), "callFunction_" + to_string(this->callinst_handler_curr_idx), this->callinst_handler);
    IRBuilder<> IRBcallFunction(callFunction);

    // errs() << "[handle_callinst] Checking call type\n";
    Value *resultValue;

    // errs() << "[handle_callinst] isIndirectCall=" << inst->isIndirectCall() << "\n";
    
    bool treatAsIndirect = inst->isIndirectCall();
    Function *callee = nullptr;
    Value *calledValue = nullptr;
    
    if (!inst->isIndirectCall()) {
        // errs() << "[handle_callinst] Direct call\n";
        callee = inst->getCalledFunction();
        
        if (!callee) {
            // errs() << "[handle_callinst] WARNING: callee is null for direct call, treating as indirect: " << *inst << "\n";
            treatAsIndirect = true;
            calledValue = inst->getCalledOperand();
        } else {
            // errs() << "[handle_callinst] Callee: " << callee->getName() << "\n";
        }
    }

    if (!treatAsIndirect && callee) {
        // 对于普通函数调用，需要判断是标准库函数还是用户函数
        // GOVMModifier会将原函数重命名为"原函数名_original"并创建wrapper
        
        // 检查是否是标准库函数
        bool is_stdlib = false;
            if (callee->hasName()) {
                std::string calleeName = callee->getName().str();
                
                // 跳过以__开头（编译器内部函数）
                if(calleeName.length() >= 2 && calleeName[0] == '_' && calleeName[1] == '_') {
                    is_stdlib = true;
                }
                // 跳过C标准库函数
                else if(calleeName.find("printf") != std::string::npos ||
                   calleeName.find("sprintf") != std::string::npos ||
                   calleeName.find("fprintf") != std::string::npos ||
                   calleeName.find("vsprintf") != std::string::npos ||
                   calleeName.find("vfprintf") != std::string::npos ||
                   calleeName.find("vsnprintf") != std::string::npos ||
                   calleeName.find("local_stdio") != std::string::npos ||
                   calleeName.find("frexp") != std::string::npos ||
                   calleeName.find("ldexp") != std::string::npos ||
                   calleeName.find("modf") != std::string::npos ||
                   calleeName.find("scalbn") != std::string::npos ||
                   calleeName.find("ilogb") != std::string::npos ||
                   calleeName.find("logb") != std::string::npos ||
                   calleeName.find("copysign") != std::string::npos ||
                   calleeName.find("nan") != std::string::npos ||
                   calleeName.find("nextafter") != std::string::npos ||
                   calleeName.find("fdim") != std::string::npos ||
                   calleeName.find("fmax") != std::string::npos ||
                   calleeName.find("fmin") != std::string::npos ||
                   calleeName.find("fma") != std::string::npos ||
                   calleeName.find("isnan") != std::string::npos ||
                   calleeName.find("isinf") != std::string::npos ||
                   calleeName.find("isfinite") != std::string::npos ||
                   calleeName.find("fabs") != std::string::npos ||
                   calleeName.find("ceil") != std::string::npos ||
                   calleeName.find("floor") != std::string::npos ||
                   calleeName.find("round") != std::string::npos ||
                   calleeName.find("trunc") != std::string::npos ||
                   calleeName.find("sqrt") != std::string::npos ||
                   calleeName.find("pow") != std::string::npos ||
                   calleeName.find("exp") != std::string::npos ||
                   calleeName.find("log") != std::string::npos ||
                   calleeName.find("sin") != std::string::npos ||
                   calleeName.find("cos") != std::string::npos ||
                   calleeName.find("tan") != std::string::npos ||
                   calleeName.find("asin") != std::string::npos ||
                   calleeName.find("acos") != std::string::npos ||
                   calleeName.find("atan") != std::string::npos ||
                   calleeName.find("atan2") != std::string::npos ||
                   calleeName.find("sinh") != std::string::npos ||
                   calleeName.find("cosh") != std::string::npos ||
                   calleeName.find("tanh") != std::string::npos ||
                   calleeName.find("malloc") != std::string::npos ||
                   calleeName.find("free") != std::string::npos ||
                   calleeName.find("calloc") != std::string::npos ||
                   calleeName.find("realloc") != std::string::npos ||
                   calleeName.find("memcpy") != std::string::npos ||
                   calleeName.find("memset") != std::string::npos ||
                   calleeName.find("memmove") != std::string::npos ||
                   calleeName.find("strcmp") != std::string::npos ||
                   calleeName.find("strlen") != std::string::npos ||
                   calleeName.find("strcpy") != std::string::npos) {
                    is_stdlib = true;
                }
                // 跳过C++标准库函数
                else if(calleeName.find("std::") != std::string::npos ||
                   calleeName.find("basic_ostream") != std::string::npos ||
                   calleeName.find("basic_ios") != std::string::npos ||
                   calleeName.find("basic_istream") != std::string::npos ||
                   calleeName.find("basic_string") != std::string::npos ||
                   calleeName.find("basic_iostream") != std::string::npos ||
                   calleeName.find("basic_fstream") != std::string::npos ||
                   calleeName.find("basic_ifstream") != std::string::npos ||
                   calleeName.find("basic_ofstream") != std::string::npos ||
                   calleeName.find("basic_stringbuf") != std::string::npos ||
                   calleeName.find("basic_istringstream") != std::string::npos ||
                   calleeName.find("basic_ostringstream") != std::string::npos ||
                   calleeName.find("basic_stringstream") != std::string::npos ||
                   calleeName.find("ctype") != std::string::npos ||
                   calleeName.find("locale") != std::string::npos ||
                   calleeName.find("char_traits") != std::string::npos ||
                   calleeName.find("numpunct") != std::string::npos ||
                   calleeName.find("num_put") != std::string::npos ||
                   calleeName.find("allocator") != std::string::npos ||
                   calleeName.find("ios_base") != std::string::npos ||
                   calleeName.find("ostreambuf") != std::string::npos ||
                   calleeName.find("istreambuf") != std::string::npos) {
                    is_stdlib = true;
                }
                // 检查是否是声明（外部函数）
                else if (callee->isDeclaration()) {
                    is_stdlib = true;
                }
            }
            
            if (is_stdlib) {
                // 标准库函数，直接调用
                resultValue = IRBcallFunction.CreateCall(callee->getFunctionType(), callee,
                            ArrayRef<Value *>(target_func_args));
            } else {
                // 用户函数，调用wrapper函数（原函数名）
                // wrapper函数会设置VM环境并调用vm_interpreter
                resultValue = IRBcallFunction.CreateCall(callee->getFunctionType(), callee,
                            ArrayRef<Value *>(target_func_args));
            }
    }
    else {
        // indirect call (or direct call with null callee treated as indirect)
        if (!calledValue) {
            calledValue = inst->getCalledOperand();
        }
        if (value_map.find(calledValue) == value_map.end()) {
            // errs() << "[VMP] WARNING: called_value not found in value_map for indirect call: " << *inst << "\n";
            // fallback: use a direct call approach by calling the function directly
            FunctionType *funcType = inst->getFunctionType();
            Value *funcPtr = IRBcallFunction.CreatePointerCast(calledValue, PointerType::get(Mod->getContext(), 0));
            resultValue = IRBcallFunction.CreateCall(funcType, funcPtr, ArrayRef<Value *>(target_func_args));
        } else {
            unsigned called_value_offset = value_map[calledValue];

            // load value from gv_data_seg
            ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
            Value * offset_value = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), called_value_offset);
            Value * gepinst = IRBcallFunction.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, offset_value}, "");

            // convert gep from i8* to value->getType() *
            Value * ptr = IRBcallFunction.CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));
            
            // load from gv_data_seg
            Value * value = IRBcallFunction.CreateLoad(calledValue->getType(), ptr);


            // indirect call - need to cast the function pointer
            FunctionType *funcType = inst->getFunctionType();
            Value *funcPtr = IRBcallFunction.CreatePointerCast(value, PointerType::get(Mod->getContext(), 0));
            resultValue = IRBcallFunction.CreateCall(funcType, funcPtr, ArrayRef<Value *>(target_func_args));
        }
    }

    // if return not void, store it to gv_data_seg
    if (inst->getType() != Type::getVoidTy(this->Mod->getContext())) {
        if (value_map.find(inst) == value_map.end()) {
            // errs() << "[VMP] ERROR: call result not found in value_map: " << *inst << "\n";
        } else {
            unsigned result_value_offset = value_map[inst];

            // load value from gv_data_seg
            ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
            Value * offset_value = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), result_value_offset);
            Value * gepinst = IRBcallFunction.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, offset_value}, "");

            // convert gep from i8* to value->getType() *
            Value * ptr = IRBcallFunction.CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));

            // store
            IRBcallFunction.CreateStore(resultValue, ptr);
        }
    }

    
    // Create Return
    IRBcallFunction.CreateRetVoid();
    

    // compare and jmp
    BasicBlock *falseconBBL = BasicBlock::Create(Mod->getContext(), "falseconBBL", this->callinst_handler);

    Value *currfunc_id = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), curr_func_id);
    Value *condition = IRBcon.CreateICmpEQ(this->targetfunc_id, currfunc_id);
    IRBcon.CreateCondBr(condition, callFunction, falseconBBL);
    this->callinst_handler_conBBL = falseconBBL;

}


void GOVMTranslator::handle_inst(Instruction *ins) {

    // switch inst type
    if(AllocaInst * inst = dyn_cast<AllocaInst>(ins)){
        // alloca memory for AllocaInst_Res and AllocaInst_alloca_area

        // AllocaInst_Res
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);

        // AllocaInst_alloca_area
        int area_offset = curr_data_offset;
        int alloca_size = modDataLayout->getTypeAllocSize(inst->getAllocatedType());
        curr_data_offset += alloca_size;
        
        // align to pointer_size for next allocation
        if (curr_data_offset % pointer_size != 0) {
            curr_data_offset += pointer_size - (curr_data_offset % pointer_size);
        }

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(ALLOCA_OP), packed_res, pack(area_offset, pointer_size));
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        #ifdef GOVTRANSLATOR_DEBUG
        // errs() << "[*] AllocaInst: " << *inst << "\n";
        // errs() << "\t area_offset: " << area_offset << "\tarea_size: " << alloca_size << "\n";
        // errs() << "\t current code_pos: " << vm_code.size() - hex_code.size() << "\n";
        // errs() << "\t Hex Code: "; dump_vector(hex_code); errs() << "\n";
        #endif
    }

    else if(LoadInst * inst = dyn_cast<LoadInst>(ins)){
        
        // return
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);

        // PointerOperand
        std::vector<uint8_t> packed_pointer_operand = GET_PACK_VALUE(inst->getPointerOperand());


        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(LOAD_OP), packed_res, packed_pointer_operand);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    else if(StoreInst * inst = dyn_cast<StoreInst>(ins)){
        
        // ValueOperand
        std::vector<uint8_t> packed_value_operand = GET_PACK_VALUE(inst->getValueOperand());

        // PointerOperand
        std::vector<uint8_t> packed_pointer_operand = GET_PACK_VALUE(inst->getPointerOperand());

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(STORE_OP), packed_value_operand, packed_pointer_operand);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
        
        #ifdef GOVTRANSLATOR_DEBUG
        // errs() << "[*] StoreInst: " << *inst << "\n";
        // errs() << "\t current code_pos: " << vm_code.size() - hex_code.size() << "\n";
        // errs() << "\t Hex Code: "; dump_vector(hex_code); errs() << "\n";
        #endif
    }

    else if(UnaryOperator * inst = dyn_cast<UnaryOperator>(ins)) {
        if (inst->getOpcode() == Instruction::FNeg) {
            insert_to_value_map(&value_map, inst, curr_data_offset);
            int res_size = modDataLayout->getTypeAllocSize(inst->getType());
            curr_data_offset += res_size;

            std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
            std::vector<uint8_t> packed_fneg = {static_cast<uint8_t>(31)};
            std::vector<uint8_t> packed_op0 = GET_PACK_VALUE(inst->getOperand(0));

            std::vector<uint8_t> hex_code;
            ins_to_hex(hex_code, pack_op(BinaryOperator_OP), packed_fneg, packed_res, packed_op0);
            vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
        }
    }

    else if(ins->isBinaryOp()){
        BinaryOperator * inst = dyn_cast<BinaryOperator>(ins);

        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;
        
        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);

        std::vector<uint8_t> packed_binaryOpcode = {static_cast<uint8_t>(inst->getOpcode())};

        std::vector<uint8_t> packed_op0 = GET_PACK_VALUE(inst->getOperand(0));
        std::vector<uint8_t> packed_op1 = GET_PACK_VALUE(inst->getOperand(1));


        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(BinaryOperator_OP), packed_binaryOpcode, packed_res, packed_op0, packed_op1);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    else if(CmpInst * inst = dyn_cast<CmpInst>(ins)){

        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);

        std::vector<uint8_t> packed_op0 = GET_PACK_VALUE(inst->getOperand(0));
        std::vector<uint8_t> packed_op1 = GET_PACK_VALUE(inst->getOperand(1));

        unsigned predicate_val = inst->getPredicate();
        if (inst->isFPPredicate()) {
            predicate_val += 42;
        }
        std::vector<uint8_t> packed_predicate = {static_cast<uint8_t>(predicate_val)};

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(CMP_OP), packed_predicate, packed_res, packed_op0, packed_op1);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        #ifdef GOVTRANSLATOR_DEBUG
        // errs() << "[*] CmpInst: " << *inst << "\n";
        // errs() << "\t Predicate: " << inst->getPredicate() << "\n";
        // errs() << "\t res_size: " << res_size << "\n";
        // errs() << "\t Op 0 Type: " << *inst->getOperand(0)->getType() << "\t Op 0 Size: " << modDataLayout->getTypeAllocSize(inst->getOperand(0)->getType()) << "\n";
        // errs() << "\t Op 1 Type: " << *inst->getOperand(1)->getType() << "\t Op 1 Size: " << modDataLayout->getTypeAllocSize(inst->getOperand(1)->getType()) << "\n";
        // errs() << "\t current code_pos: " << vm_code.size() - hex_code.size() << "\n";
        // errs() << "\n";
        #endif
    }

    else if(GetElementPtrInst * inst = dyn_cast<GetElementPtrInst>(ins)){
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);

        std::vector<uint8_t> packed_ptr = GET_PACK_VALUE(inst->getPointerOperand());

        // get indices
        // but only consider last indice
        std::vector<Value *> indices;
        for (auto curr_idx=inst->idx_begin(); curr_idx != inst->idx_end(); curr_idx++){
            indices.push_back(*curr_idx);
        }

        // GEP type
        // {0, 0}: structure value is offset
        // {x, x}: array, value is offset
        Type * srcType = inst->getSourceElementType();
        std::vector<uint8_t> gep_type;
        std::vector<uint8_t> packed_value;
        if (dyn_cast<StructType>(srcType)) {
            // is struct type
            StructType * st = dyn_cast<StructType>(srcType);
            gep_type = {0, 0};
            if (indices.empty()) {
                // zero-index GEP on struct: result is same as base pointer
                packed_value = {0, 0};
            } else {
                Value* last_idx = indices.back();
                if (ConstantInt* CI = dyn_cast<ConstantInt>(last_idx)) {
                    int element_idx = CI->getSExtValue();
                    int curr_element_offset = 0;
                    for (int i = 0; i < element_idx; i++) {
                        curr_element_offset += modDataLayout->getTypeAllocSize(st->getElementType(i));
                    }
                    packed_value = pack(curr_element_offset, pointer_size);
                    packed_value.insert(packed_value.begin(), 1);
                    packed_value.insert(packed_value.begin(), pointer_size);
                } else {
                    // last index is not a constant, use it as a variable
                    packed_value = GET_PACK_VALUE(last_idx);
                }
            }
        } else {
            // is array type
            gep_type = type_to_hex(inst->getResultElementType());
            if (indices.empty()) {
                packed_value = {0, 0};
            } else {
                packed_value = GET_PACK_VALUE(indices.back());
            }
        }

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(GEP_OP), gep_type, packed_res, packed_ptr, packed_value);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());


        #ifdef GOVTRANSLATOR_DEBUG
        // errs() << "[*] GetElementPtrInst: " << *inst << "\n";
        // errs() << "\t res_size: " << res_size << "\n";
        // errs() << "\t is struct gep: " << (dyn_cast<StructType>(srcType) != 0) << "\n";
        // errs() << "\t current code_pos: " << vm_code.size() - hex_code.size() << "\n";
        // errs() << "\n";
        #endif
    }

    else if(CastInst * inst = dyn_cast<CastInst>(ins)){
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        uint8_t cast_op = (uint8_t)inst->getOpcode();
        int op_size = modDataLayout->getTypeAllocSize(inst->getOperand(0)->getType());
        std::vector<uint8_t> packed_cast_op = pack(cast_op, 1);
        std::vector<uint8_t> packed_op_size = pack(op_size, 1);
        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        std::vector<uint8_t> packed_value = GET_PACK_VALUE(inst->getOperand(0));

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(CAST_OP), packed_cast_op, packed_op_size, packed_res, packed_value);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        #ifdef GOVTRANSLATOR_DEBUG
        // errs() << "[*] CastInst: " << *inst << "\n";
        // errs() << "\t res_size: " << res_size << "\n";
        // errs() << "\t Op 0 Type: " << *inst->getOperand(0)->getType() << "\t Op 0 Size: " << modDataLayout->getTypeAllocSize(inst->getOperand(0)->getType()) << "\n";
        // errs() << "\t current code_pos: " << vm_code.size() - hex_code.size() << "\n";
        // errs() << "\n";
        #endif
    }

    else if(BranchInst * inst = dyn_cast<BranchInst>(ins)){
        std::vector<uint8_t> hex_code;
        hex_code = pack_op(BR_OP);
        std::vector<uint8_t> padding = pack(0, pointer_size);
        int source_bb_offset = basicblock_map[inst->getParent()];
        std::vector<uint8_t> packed_source_bb = pack(source_bb_offset, pointer_size);

        errs() << "[BR] source_bb_offset=" << source_bb_offset 
               << " for BB " << inst->getParent()->getName().str()
               << " current code_pos=" << vm_code.size() << "\n";

        if (inst->isUnconditional()) {
            hex_code.push_back(0);
            hex_code.insert(hex_code.end(), packed_source_bb.begin(), packed_source_bb.end());
            br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+hex_code.size(), inst->getSuccessor(0)));
            errs() << "[BR] Unconditional to BB " << inst->getSuccessor(0)->getName().str() << "\n";
            errs() << "[BR] packed_source_bb bytes: ";
            for (auto b : packed_source_bb) errs() << (int)b << " ";
            errs() << "\n";
            hex_code.insert(hex_code.end(), padding.begin(), padding.end());
        } else {
            hex_code.push_back(1);
            hex_code.insert(hex_code.end(), packed_source_bb.begin(), packed_source_bb.end());
            std::vector<uint8_t> pack_condition = packValue(inst->getCondition(), &value_map);
            hex_code.insert(hex_code.end(), pack_condition.begin(), pack_condition.end());

            errs() << "[BR] Conditional: true->BB " << inst->getSuccessor(0)->getName().str()
                   << " false->BB " << inst->getSuccessor(1)->getName().str() << "\n";
            errs() << "[BR] packed_source_bb bytes: ";
            for (auto b : packed_source_bb) errs() << (int)b << " ";
            errs() << "\n";
            br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+hex_code.size(), inst->getSuccessor(0)));
            hex_code.insert(hex_code.end(), padding.begin(), padding.end());
            br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+hex_code.size(), inst->getSuccessor(1)));
            hex_code.insert(hex_code.end(), padding.begin(), padding.end());
        }
        

        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        // errs() << "[*] BranchInst: " << *inst << "\n";
        // if (inst->isConditional())
        //     errs() << "\t Condition: " << *inst->getCondition() << "\n";
        // for (unsigned i=0; i<inst->getNumSuccessors(); i++) {
        //     errs() << "\t Successors: " << inst->getSuccessor(i)->getName().str() << "\n";
        // }
        // errs() << "\t current code_pos: " << vm_code.size() - hex_code.size() << "\n";
        // errs() << "\n";
    }
    
    else if(ReturnInst * inst = dyn_cast<ReturnInst>(ins)) {
        std::vector<uint8_t> value;

        if (inst->getNumOperands() == 0){           // return void
            value = get_null_value();
        }
        else{                                       // return something
            value = GET_PACK_VALUE(inst->getReturnValue());
        }

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(Ret_OP), value);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    else if(InvokeInst * inst = dyn_cast<InvokeInst>(ins)) {
        long long curr_func_id = this->callinst_handler_curr_idx++;
        std::vector<uint8_t> packed_funcid = pack(curr_func_id, pointer_size);

        std::vector<uint8_t> packed_res;
        if (inst->getType() != Type::getVoidTy(this->Mod->getContext())) {
            insert_to_value_map(&value_map, inst, curr_data_offset);
            int res_size = modDataLayout->getTypeAllocSize(inst->getType());
            curr_data_offset += res_size;
            packed_res = GET_PACK_VALUE(inst);
        }

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(Call_OP), packed_funcid);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        callinst_map.insert(std::pair<CallBase *, long long>(cast<CallBase>(inst), curr_func_id));

        std::vector<uint8_t> br_hex;
        br_hex = pack_op(BR_OP);
        br_hex.push_back(1); // conditional

        int current_bb_offset = vm_code.size();
        std::vector<uint8_t> packed_src_bb = pack(current_bb_offset, pointer_size);
        br_hex.insert(br_hex.end(), packed_src_bb.begin(), packed_src_bb.end());

        std::vector<uint8_t> packed_cond;
        if (exception_thrown) {
            auto it = value_map.find(exception_thrown);
            if (it != value_map.end()) {
                packed_cond.push_back(1);
                packed_cond.push_back(0);
                std::vector<uint8_t> off = pack(it->second, pointer_size);
                packed_cond.insert(packed_cond.end(), off.begin(), off.end());
            }
        }

        br_hex.insert(br_hex.end(), packed_cond.begin(), packed_cond.end());

        std::vector<uint8_t> padding = pack(0, pointer_size);
        br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+br_hex.size(), inst->getNormalDest()));
        br_hex.insert(br_hex.end(), padding.begin(), padding.end());
        br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+br_hex.size(), inst->getUnwindDest()));
        br_hex.insert(br_hex.end(), padding.begin(), padding.end());

        vm_code.insert(vm_code.end(), br_hex.begin(), br_hex.end());
    }

    else if(CallBase * inst = dyn_cast<CallBase>(ins)) {
        
        // current function id
        long long curr_func_id = this->callinst_handler_curr_idx ++;
        
        std::vector<uint8_t> packed_funcid = pack(curr_func_id, pointer_size);
        
        // check if this callsite return a void
        std::vector<uint8_t> packed_res;
        if (inst->getType() != Type::getVoidTy(this->Mod->getContext())) {
            // return a value
            insert_to_value_map(&value_map, inst, curr_data_offset);
            int res_size = modDataLayout->getTypeAllocSize(inst->getType());
            curr_data_offset += res_size;

            packed_res = GET_PACK_VALUE(inst);
        }

        // construct hex code
        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(Call_OP), packed_funcid);

        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        callinst_map.insert(std::pair<CallBase *, long long>(cast<CallBase>(ins), curr_func_id));
    }

    else if(PHINode * inst = dyn_cast<PHINode>(ins)) {
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        unsigned num_incoming = inst->getNumIncomingValues();
        std::vector<uint8_t> packed_num = pack(num_incoming, 4);

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(PHI_OP), packed_res, packed_num);

        errs() << "[PHI] num_incoming=" << num_incoming << " at code_pos=" << vm_code.size() << "\n";

        for (unsigned i = 0; i < num_incoming; i++) {
            BasicBlock *incoming_bb = inst->getIncomingBlock(i);
            Value *incoming_val = inst->getIncomingValue(i);
            int bb_id = basicblock_map[incoming_bb];
            std::vector<uint8_t> packed_bb_id = pack(bb_id, pointer_size);
            
            errs() << "[PHI] incoming[" << i << "] bb_id=" << bb_id 
                   << " incoming_bb=" << incoming_bb->getName().str()
                   << " basicblock_map lookup result=" << bb_id << "\n";
            
            std::vector<uint8_t> packed_incoming = GET_PACK_VALUE(incoming_val);
            ins_to_hex(hex_code, packed_bb_id, packed_incoming);
        }

        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    else if(SelectInst * inst = dyn_cast<SelectInst>(ins)) {
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        std::vector<uint8_t> packed_cond = GET_PACK_VALUE(inst->getCondition());
        std::vector<uint8_t> packed_true = GET_PACK_VALUE(inst->getTrueValue());
        std::vector<uint8_t> packed_false = GET_PACK_VALUE(inst->getFalseValue());

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(SELECT_OP), packed_res, packed_cond, packed_true, packed_false);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    // LLVM 21: SwitchInst支持
    else if(SwitchInst * inst = dyn_cast<SwitchInst>(ins)) {
        std::vector<uint8_t> hex_code;
        hex_code = pack_op(SWITCH_OP);
        
        // pack condition value
        std::vector<uint8_t> packed_condition = GET_PACK_VALUE(inst->getCondition());
        hex_code.insert(hex_code.end(), packed_condition.begin(), packed_condition.end());
        
        // number of cases (excluding default)
        uint32_t num_cases = inst->getNumCases();
        std::vector<uint8_t> packed_num_cases = pack(num_cases, 4);
        hex_code.insert(hex_code.end(), packed_num_cases.begin(), packed_num_cases.end());
        
        // case value size (needed by interpreter to parse each case)
        int case_val_size = modDataLayout->getTypeAllocSize(inst->getCondition()->getType());
        std::vector<uint8_t> packed_case_size = pack(case_val_size, 4);
        hex_code.insert(hex_code.end(), packed_case_size.begin(), packed_case_size.end());
        
        // default case target (will be filled later)
        std::vector<uint8_t> padding = pack(0, pointer_size);
        int default_code_pos = vm_code.size() + hex_code.size();
        switch_map.push_back(std::make_tuple(default_code_pos, inst->getDefaultDest(), -1));
        hex_code.insert(hex_code.end(), padding.begin(), padding.end());
        
        // pack each case
        for (auto it = inst->case_begin(); it != inst->case_end(); ++it) {
            ConstantInt *case_value = it->getCaseValue();
            BasicBlock *case_bb = it->getCaseSuccessor();
            
            // pack case value
            int64_t case_val = case_value->getSExtValue();
            std::vector<uint8_t> packed_case_val = pack(case_val, case_val_size);
            hex_code.insert(hex_code.end(), packed_case_val.begin(), packed_case_val.end());
            
            // case target (will be filled later)
            int case_code_pos = vm_code.size() + hex_code.size();
            switch_map.push_back(std::make_tuple(case_code_pos, case_bb, (int)case_val));
            hex_code.insert(hex_code.end(), padding.begin(), padding.end());
        }
        
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    // ExtractValueInst支持 - 从结构体/数组中提取值
    else if(ExtractValueInst * inst = dyn_cast<ExtractValueInst>(ins)) {
        Value *aggregate = inst->getAggregateOperand();
        Type *aggType = aggregate->getType();
        
        // 计算偏移量
        unsigned offset = 0;
        Type *currentType = aggType;
        for (unsigned idx : inst->indices()) {
            if (StructType *ST = dyn_cast<StructType>(currentType)) {
                for (unsigned i = 0; i < idx; i++) {
                    offset += modDataLayout->getTypeAllocSize(ST->getElementType(i));
                }
                currentType = ST->getElementType(idx);
            } else if (ArrayType *AT = dyn_cast<ArrayType>(currentType)) {
                offset += idx * modDataLayout->getTypeAllocSize(AT->getElementType());
                currentType = AT->getElementType();
            }
        }
        
        // 结果值
        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        
        // 聚合操作数
        std::vector<uint8_t> packed_agg = GET_PACK_VALUE(aggregate);
        
        // 偏移量作为常量
        std::vector<uint8_t> packed_offset = pack(offset, pointer_size);
        packed_offset.insert(packed_offset.begin(), 1);  // type = 1 (常量)
        packed_offset.insert(packed_offset.begin(), pointer_size);  // size
        
        // 结果类型大小
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        std::vector<uint8_t> packed_size = pack(res_size, 4);
        
        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(EXTRACTVALUE_OP), packed_res, packed_agg, packed_offset, packed_size);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    // UnreachableInst支持 - 生成无条件跳转到错误处理
    else if(isa<UnreachableInst>(ins)) {
        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(BR_OP));
        // 目标地址填0（表示退出VM）
        std::vector<uint8_t> packed_target = pack(0, pointer_size);
        hex_code.insert(hex_code.end(), packed_target.begin(), packed_target.end());
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    // C++ 异常处理指令
    // landingpad: 异常处理入口，返回异常对象和类型
    else if(LandingPadInst * inst = dyn_cast<LandingPadInst>(ins)) {
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(LANDINGPAD_OP), packed_res);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    // resume: 恢复异常传播
    else if(ResumeInst * inst = dyn_cast<ResumeInst>(ins)) {
        Value *exc_value = inst->getValue();
        std::vector<uint8_t> packed_exc = GET_PACK_VALUE(exc_value);

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(RESUME_OP), packed_exc);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    // insertvalue: 插入值到聚合类型，简化为直接存储
    else if(InsertValueInst *inst = dyn_cast<InsertValueInst>(ins)) {
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;
        
        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        std::vector<uint8_t> packed_agg = GET_PACK_VALUE(inst->getAggregateOperand());
        std::vector<uint8_t> packed_val = GET_PACK_VALUE(inst->getInsertedValueOperand());
        
        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(INSERTVALUE_OP), packed_res, packed_agg, packed_val);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    // extractvalue: 从聚合类型提取值
    else if(ExtractValueInst *inst = dyn_cast<ExtractValueInst>(ins)) {
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;
        
        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        std::vector<uint8_t> packed_agg = GET_PACK_VALUE(inst->getAggregateOperand());
        
        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(EXTRACTVALUE_OP), packed_res, packed_agg);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    else if(IndirectBrInst * inst = dyn_cast<IndirectBrInst>(ins)) {
        std::vector<uint8_t> hex_code;
        hex_code = pack_op(INDIRECTBR_OP);

        std::vector<uint8_t> packed_addr = GET_PACK_VALUE(inst->getAddress());
        hex_code.insert(hex_code.end(), packed_addr.begin(), packed_addr.end());

        std::vector<uint8_t> padding = pack(0, pointer_size);
        br_map.push_back(pair<int, BasicBlock *>(vm_code.size() + hex_code.size(), inst->getSuccessor(0)));
        hex_code.insert(hex_code.end(), padding.begin(), padding.end());

        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    else if(ExtractElementInst * inst = dyn_cast<ExtractElementInst>(ins)) {
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        Value *vec = inst->getVectorOperand();
        Value *idx = inst->getIndexOperand();
        Type *vecType = vec->getType();
        Type *elemType = nullptr;
        if (FixedVectorType *FVT = dyn_cast<FixedVectorType>(vecType))
            elemType = FVT->getElementType();
        else if (ScalableVectorType *SVT = dyn_cast<ScalableVectorType>(vecType))
            elemType = SVT->getElementType();

        int elem_size = elemType ? modDataLayout->getTypeAllocSize(elemType) : 4;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        std::vector<uint8_t> packed_vec = GET_PACK_VALUE(vec);
        std::vector<uint8_t> packed_idx = GET_PACK_VALUE(idx);
        std::vector<uint8_t> packed_elem_size = pack(elem_size, 4);

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(EXTRACTELEMENT_OP), packed_res, packed_vec, packed_idx, packed_elem_size);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    else if(InsertElementInst * inst = dyn_cast<InsertElementInst>(ins)) {
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        Value *vec = inst->getOperand(0);
        Value *elem = inst->getOperand(1);
        Value *idx = inst->getOperand(2);
        Type *elemType = elem->getType();
        int elem_size = modDataLayout->getTypeAllocSize(elemType);
        int vec_total_size = res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        std::vector<uint8_t> packed_vec = GET_PACK_VALUE(vec);
        std::vector<uint8_t> packed_elem = GET_PACK_VALUE(elem);
        std::vector<uint8_t> packed_idx = GET_PACK_VALUE(idx);
        std::vector<uint8_t> packed_esize = pack(elem_size, 4);
        std::vector<uint8_t> packed_vsize = pack(vec_total_size, 4);

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(INSERTELEMENT_OP), packed_res, packed_vec, packed_elem, packed_idx, packed_esize, packed_vsize);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    else if(ShuffleVectorInst * inst = dyn_cast<ShuffleVectorInst>(ins)) {
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        Value *v1 = inst->getOperand(0);
        Value *v2 = inst->getOperand(1);
        Value *mask = inst->getOperand(2);

        Type *elemType = nullptr;
        if (FixedVectorType *FVT = dyn_cast<FixedVectorType>(v1->getType()))
            elemType = FVT->getElementType();
        int elem_size = elemType ? modDataLayout->getTypeAllocSize(elemType) : 8;

        int v1_num_elements = 0;
        if (FixedVectorType *FVT = dyn_cast<FixedVectorType>(v1->getType()))
            v1_num_elements = FVT->getNumElements();

        int mask_num_elements = 0;
        if (FixedVectorType *FVT = dyn_cast<FixedVectorType>(inst->getType()))
            mask_num_elements = FVT->getNumElements();

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        std::vector<uint8_t> packed_v1 = GET_PACK_VALUE(v1);
        std::vector<uint8_t> packed_v2 = GET_PACK_VALUE(v2);
        std::vector<uint8_t> packed_esize = pack(elem_size, 4);
        std::vector<uint8_t> packed_v1num = pack(v1_num_elements, 4);
        std::vector<uint8_t> packed_masknum = pack(mask_num_elements, 4);

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(SHUFFLEVECTOR_OP), packed_res, packed_v1, packed_v2, packed_esize, packed_v1num, packed_masknum);

        if (Constant *maskConst = dyn_cast<Constant>(mask)) {
            if (ConstantDataVector *CDV = dyn_cast<ConstantDataVector>(maskConst)) {
                for (unsigned i = 0; i < CDV->getNumElements(); i++) {
                    int64_t mask_val = -1;
                    if (ConstantInt *CI = dyn_cast<ConstantInt>(CDV->getElementAsConstant(i)))
                        mask_val = CI->getSExtValue();
                    std::vector<uint8_t> packed_mask_val = pack((int)(mask_val), 4);
                    hex_code.insert(hex_code.end(), packed_mask_val.begin(), packed_mask_val.end());
                }
            } else if (ConstantVector *CV = dyn_cast<ConstantVector>(maskConst)) {
                for (unsigned i = 0; i < CV->getNumOperands(); i++) {
                    int64_t mask_val = -1;
                    if (ConstantInt *CI = dyn_cast<ConstantInt>(CV->getOperand(i)))
                        mask_val = CI->getSExtValue();
                    std::vector<uint8_t> packed_mask_val = pack((int)(mask_val), 4);
                    hex_code.insert(hex_code.end(), packed_mask_val.begin(), packed_mask_val.end());
                }
            }
        } else {
            for (unsigned i = 0; i < mask_num_elements; i++) {
                std::vector<uint8_t> packed_mask_val = pack(-1, 4);
                hex_code.insert(hex_code.end(), packed_mask_val.begin(), packed_mask_val.end());
            }
        }

        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    else if(FreezeInst * inst = dyn_cast<FreezeInst>(ins)) {
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        std::vector<uint8_t> packed_val = GET_PACK_VALUE(inst->getOperand(0));

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(FREEZE_OP), packed_res, packed_val);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    // CatchSwitchInst: 异常处理分发，生成 NOP
    else if(isa<CatchSwitchInst>(ins)) {
        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(NOP_OP));
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    // CatchReturnInst: 异常处理返回，生成 NOP
    else if(isa<CatchReturnInst>(ins)) {
        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(NOP_OP));
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    // CleanupReturnInst: 清理返回，生成 NOP
    else if(isa<CleanupReturnInst>(ins)) {
        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(NOP_OP));
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    else{
    }
}


// Translator
bool GOVMTranslator::run(){
    // errs() << "[Translator] Starting for function: " << F->getName() << "\n";
    
    bool has_exception_handling = false;
    for(auto &BB : *F) {
        for(auto &I : BB) {
            if(isa<LandingPadInst>(&I)) {
                has_exception_handling = true;
                break;
            }
        }
        if(has_exception_handling) break;
    }
    
    if(has_exception_handling) {
    }
    
    curr_data_offset = 0;
    
    // if return not void, alloca a memory
    if (!F->getReturnType()->isVoidTy()) {
        curr_data_offset += modDataLayout->getTypeAllocSize(F->getReturnType());
    }
    
    // parameter allocation
    if(!F->isVarArg()){
        for(auto arg = F->arg_begin(); arg != F->arg_end(); ++arg) {
            
            Value * tmparg = &*arg;
            insert_to_value_map(&value_map, tmparg, curr_data_offset);
            
            curr_data_offset += modDataLayout->getTypeAllocSize(tmparg->getType());
        }
    }
    
    // 确保数据段至少有最小大小（16字节），避免空数据段导致的内存访问问题
    if (curr_data_offset < 16) {
        curr_data_offset = 16;
    }

    // Align curr_data_offset to pointer alignment for subsequent allocations
    if (curr_data_offset % pointer_size != 0) {
        curr_data_offset += pointer_size - (curr_data_offset % pointer_size);
    }

    // traverse whole function
    int bb_count = 0;
    const int MAX_BASIC_BLOCKS = 10000;
    const int MAX_INSTRUCTIONS = 100000;
    int total_instructions = 0;
    
    // errs() << "[Translator] Data offset: " << curr_data_offset << ", starting traversal\n";
    
    for(auto bbl = F->begin(); bbl != F->end(); bbl++){
        
        if (bb_count >= MAX_BASIC_BLOCKS) {
            break;
        }

        BasicBlock * bb = &*bbl;
        basicblock_map.insert(pair<BasicBlock *, int>(bb, vm_code.size()));

        // 每个基本块开头需要两个种子：
        // 1. opcode_xorshift32_state - 用于解密操作码
        // 2. vm_code_state - 用于解密VM代码
        opcode_seed_setup();
        uint32_t vm_code_seed = vm_code_seed_setup();
        uint32_t currbb_begin = vm_code.size();

        std::vector<Instruction *> instructions_to_process;
        for(auto ins = bbl->begin(); ins != bbl->end(); ins++){
            instructions_to_process.push_back(&*ins);
        }
        
        // errs() << "[Translator] BB " << bb_count << " has " << instructions_to_process.size() << " instructions\n";
        
        for(Instruction *inst : instructions_to_process){

            if (total_instructions >= MAX_INSTRUCTIONS) {
                break;
            }
            
            if (!inst || inst->getParent() != bb) {
                continue;
            }

            std::vector<std::pair<unsigned, ConstantExpr *>> const_exprs;
            for (unsigned idx = 0; idx < inst->getNumOperands(); idx++) {
                if (ConstantExpr * Op = dyn_cast<ConstantExpr>(inst->getOperand(idx))) {
                    const_exprs.push_back(std::make_pair(idx, Op));
                }
            }
            
            for (auto &pair : const_exprs) {
                unsigned idx = pair.first;
                ConstantExpr *Op = pair.second;
                
                if (!inst || inst->getParent() != bb) {
                    break;
                }
                
                Instruction * const_inst = Op->getAsInstruction();
                
                if (isa<PHINode>(inst) && !isa<PHINode>(const_inst)) {
                    BasicBlock::iterator insertPos = bb->getFirstInsertionPt();
                    const_inst->insertBefore(insertPos);
                } else {
                    const_inst->insertBefore(inst->getIterator());
                }

                inst->setOperand(idx, const_inst);

                handle_inst(const_inst);
            }
            
            if (inst && inst->getParent() == bb) {
                // errs() << "[Translator] Handling inst #" << total_instructions << ": " << inst->getOpcodeName() << "\n";
                handle_inst(inst);
            }
            total_instructions++;
        }
        
        if (total_instructions >= MAX_INSTRUCTIONS) {
            break;
        }

        uint32_t currbb_end = vm_code.size();
        vm_code_seed_map.insert(pair<uint32_t, pair<uint32_t, uint32_t>>(vm_code_seed, pair<uint32_t, uint32_t>(currbb_begin, currbb_end)));
        bb_count++;
    }

    // fill br map
    for(auto it=br_map.rbegin(); it!=br_map.rend(); it++) {
        int code_pos = it->first;
        BasicBlock * target_bb = it->second;
        auto bb_it = basicblock_map.find(target_bb);
        if (bb_it != basicblock_map.end()) {
            uint32_t target_offset = bb_it->second;
            if (target_offset >= vm_code.size()) {
                target_offset = 0;
            }
            std::vector<uint8_t> bb_addr = pack(target_offset, pointer_size);
            if (code_pos + pointer_size <= (int)vm_code.size()) {
                std::copy(bb_addr.begin(), bb_addr.end(), vm_code.begin()+code_pos);
            }
        }
    }
    
    // LLVM 21: Fill switch_map
    for(auto it=switch_map.rbegin(); it!=switch_map.rend(); it++) {
        int code_pos = std::get<0>(*it);
        BasicBlock * target_bb = std::get<1>(*it);
        auto bb_it = basicblock_map.find(target_bb);
        if (bb_it != basicblock_map.end()) {
            uint32_t target_offset = bb_it->second;
            if (target_offset >= vm_code.size()) {
                target_offset = 0;
            }
            std::vector<uint8_t> bb_addr = pack(target_offset, pointer_size);
            if (code_pos + pointer_size <= (int)vm_code.size()) {
                std::copy(bb_addr.begin(), bb_addr.end(), vm_code.begin()+code_pos);
            }
        }
    }

    /* vm_code finish */

    // errs() << "[Translator] Encrypting vm_code...\n";
    // encrypt vm_code with basicblock seed
    encrypt_vm_code();

    // errs() << "[Translator] Constructing gv...\n";
    construct_gv();

    // errs() << "[Translator] Handling callinst...\n";
    // handle callinst
    for (auto p: callinst_map) {
        handle_callinst(p.first, p.second);
    }

    // errs() << "[Translator] Finishing callinst_handler...\n";
    // callinst_handler fini
    finish_callinst_handler();
    
    // errs() << "[Translator] Done!\n";
    return true;  // 返回 true 表示成功处理
}



/* ********************************************************************
*   GOVMMODIFIER_H
***********************************************************************
*/

#define GOVMMODIFIER_DEBUG

/***
 * The main class that modify the callsite of vm-function.
 */
class GOVMModifier {
    
    public:
        GOVMModifier(Function * F, std::map<GlobalVariable *, int> *gv_value_map, std::map<Value *, int> *value_map,
                     GlobalVariable *gv_data_seg, GlobalVariable *gv_code_seg,
                     GlobalVariable *ip, GlobalVariable *data_seg_addr, GlobalVariable *code_seg_addr,
                     Function *govm_interpreter, int data_seg_size) {
            this->Mod = F->getParent();
            this->F = F;
            this->modDataLayout = const_cast<DataLayout *>(&this->Mod->getDataLayout());
            this->gv_value_map = gv_value_map;
            this->value_map = value_map;
            this->gv_data_seg = gv_data_seg;
            this->gv_code_seg = gv_code_seg;
            this->ip = ip;
            this->data_seg_addr = data_seg_addr;
            this->code_seg_addr = code_seg_addr;
            this->govm_interpreter = govm_interpreter;
            this->data_seg_size = data_seg_size;
        }

        Module * Mod;
        Function * F;
        DataLayout * modDataLayout;

        std::map<GlobalVariable *, int> *gv_value_map;
        std::map<Value *, int> *value_map;

        GlobalVariable *gv_data_seg;
        GlobalVariable *gv_code_seg;
        GlobalVariable *ip;
        GlobalVariable *data_seg_addr;
        GlobalVariable *code_seg_addr;
        Function *govm_interpreter;
        int data_seg_size;


        virtual void run ();


};



/* ********************************************************************
*   GOVMMODIFIER_CPP
***********************************************************************
*/

void GOVMModifier::run() {

    errs() << "[GOVMModifier] Starting run() for function: " << F->getName() << "\n";

    std::string orig_name = F->getName().str();
    
    errs() << "[GOVMModifier]   Deleting function body...\n";
    F->deleteBody();
    
    errs() << "[GOVMModifier]   Creating new basic block...\n";
    BasicBlock* body_bbl = BasicBlock::Create(this->Mod->getContext(), "entry", F);
    IRBuilder<> irbuilder(body_bbl);

    assert(!F->isVarArg());

    errs() << "[GOVMModifier]   Processing arguments...\n";
    std::vector<pair<Value*, int>> args_map;
    int arg_offset = 0;
    if (!F->getReturnType()->isVoidTy()) {
        arg_offset += modDataLayout->getTypeAllocSize(F->getReturnType());
    }

    std::map<int, std::pair<Value*, int>> wrapper_arg_to_orig;
    int wrapper_arg_idx = 0;
    for (auto arg = F->arg_begin(); arg != F->arg_end(); arg++, wrapper_arg_idx++) {
        Value *tmparg = &*arg;
        
        if (value_map->find(tmparg) != value_map->end()) {
            int orig_offset = (*value_map)[tmparg];
            wrapper_arg_to_orig[wrapper_arg_idx] = std::make_pair(tmparg, orig_offset);
        }
        
        arg_offset += modDataLayout->getTypeAllocSize(tmparg->getType());
    }

    errs() << "[GOVMModifier]   Storing global variable addresses (gv_value_map size=" << gv_value_map->size() << ")...\n";
    for (auto p: *gv_value_map) {
        GlobalVariable *gv = p.first;
        int offset = p.second;

        Value * gv_addr_int = irbuilder.CreatePtrToInt(gv,Type::getInt64Ty(Mod->getContext()));

        ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
        Value * offset_value = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), offset);
        Value * gepinst = irbuilder.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, offset_value}, "");

        Value * ptr = irbuilder.CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));

        irbuilder.CreateStore(gv_addr_int, ptr);
    }

    errs() << "[GOVMModifier]   Processing argument mappings...\n";
    int temp_arg_idx = 0;
    for (auto arg = F->arg_begin(); arg != F->arg_end(); arg++, temp_arg_idx++) {
        Value *tmparg = &*arg;
        
        auto it = wrapper_arg_to_orig.find(temp_arg_idx);
        if (it == wrapper_arg_to_orig.end()) {
            continue;
        }
        
        Value *paramPtr = irbuilder.CreateAlloca(tmparg->getType());
        irbuilder.CreateStore(tmparg, paramPtr);
        Value *currvalue = irbuilder.CreateLoad(tmparg->getType(), paramPtr);
        
        int offset = it->second.second;
        
        ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(F->getContext()), 0);
        Value * const_curr_value_offset = ConstantInt::get(Type::getInt64Ty(F->getContext()), offset);
        Value * gepinst = irbuilder.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, const_curr_value_offset}, "");

        Value * ptr = irbuilder.CreatePointerCast(gepinst, PointerType::get(F->getContext(), 0));
        
        irbuilder.CreateStore(currvalue, ptr);
    }

    errs() << "[GOVMModifier]   Setting data_seg_addr and code_seg_addr...\n";
    
    Value * data_seg_ptr2int = irbuilder.CreatePtrToInt(gv_data_seg, Type::getInt64Ty(Mod->getContext()));
    irbuilder.CreateStore(data_seg_ptr2int, data_seg_addr);
    Value * code_seg_ptr2int = irbuilder.CreatePtrToInt(gv_code_seg, Type::getInt64Ty(Mod->getContext()));
    irbuilder.CreateStore(code_seg_ptr2int, code_seg_addr);

    errs() << "[GOVMModifier]   Creating call to vm_interpreter...\n";
    irbuilder.CreateCall(govm_interpreter);

    errs() << "[GOVMModifier]   Creating return...\n";

    if (!F->getReturnType()->isVoidTy()) {
        ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(F->getContext()), 0);
        Value * gepinst = irbuilder.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, Zero}, "");
        Value * ptr = irbuilder.CreatePointerCast(gepinst, PointerType::get(F->getContext(), 0));
        Value * retval = irbuilder.CreateLoad(F->getReturnType(), ptr);
        irbuilder.CreateRet(retval);
    }
    else {
        irbuilder.CreateRetVoid();
    }
}



/* ********************************************************************
*   GOVMINTERPRETER_H
***********************************************************************
*/

#define GOVMINTERPRETER_DEBUG


/***
 * The main class that construct the interpreter of vm-function.
 */
class GOVMInterpreter {
    
    public:
        GOVMInterpreter(Function * F, Function * callinst_handler, 
                        GlobalVariable *gv_data_seg, GlobalVariable *gv_code_seg,
                        GlobalVariable *ip, GlobalVariable *data_seg_addr, 
                        GlobalVariable *code_seg_addr) {
            this->Mod = F->getParent();
            this->F = F;
            this->modDataLayout = const_cast<DataLayout *>(&this->Mod->getDataLayout());
            this->callinst_handler = callinst_handler;
            this->gv_data_seg = gv_data_seg;
            this->gv_code_seg = gv_code_seg;
            this->ip = ip;
            this->data_seg_addr = data_seg_addr;
            this->code_seg_addr = code_seg_addr;

            construct_gv();
        }

        Module * Mod;
        Function * F;
        DataLayout * modDataLayout;

        Function *callinst_handler;

        GlobalVariable *gv_data_seg;
        GlobalVariable *gv_code_seg;
        GlobalVariable *ip;
        GlobalVariable *data_seg_addr;
        GlobalVariable *code_seg_addr;
        GlobalVariable *pointer_size_gv;
        GlobalVariable *opcode_xorshift32_state;
        GlobalVariable *vm_code_state;
        GlobalVariable *exc_thrown_gv;
        GlobalVariable *exc_ptr_gv;
        GlobalVariable *exc_sel_gv;
        GlobalVariable *last_bb_gv;
        GlobalVariable *curr_bb_gv;

        virtual void run ();
        virtual void construct_gv ();


        Module *llvm_parse_bitcode_from_string()
        {
            // Use the new binary_ir_data array
            std::vector<char> binary_ir = get_binary_ir();
            
            StringRef str_ref(binary_ir.data(), binary_ir.size());
            MemoryBufferRef buf_ref = MemoryBufferRef(str_ref, "aVMPInterpreter.bc");
            
            // Try parseBitcodeFile first (more reliable for embedded bitcode)
            Expected<std::unique_ptr<Module>> ModuleOrErr = parseBitcodeFile(buf_ref, Mod->getContext());
            if (!ModuleOrErr) {
                return nullptr;
            }
            
            return ModuleOrErr.get().release();
        }

        Module *llvm_parse_bitcode()
        {
            SMDiagnostic Err;
            // LLVMContext *LLVMCtx = new LLVMContext();
            LLVMContext *LLVMCtx = &Mod->getContext();          // match Mod context
            unique_ptr<Module> M = parseIRFile("../c-implement/govm.bc", Err, *LLVMCtx);
            return M.release();
        }

};


#define IS_INLINE_FUNC

// memcpy functions: for points to and taint propagation.
const std::set<std::string> interpreter_function_names{
#ifndef IS_INLINE_FUNC
                                                        "xorshift32",
                                                        "get_byte_code",
                                                        "get_xorshift_seed",
                                                        "unpack_code",
                                                        "unpack_data",
                                                        "unpack_addr",
                                                        "pack_store_addr", 
                                                        "get_value_with_size", 
                                                        "get_value",
                                                        "alloca_handler",
                                                        "load_handler",
                                                        "store_handler",
                                                        "binaryOperator_handler",
                                                        "gep_handler",
                                                        "cmp_handler",
                                                        "cast_handler",
                                                        "br_handler",
                                                        "return_handler",
                                                        "get_opcode",
#endif
                                                        "vm_interpreter",
                                                        "vm_interpreter_callinst_dispatch"      // only for check annotation

                                                        };



// check targetfunction is c-implement functions
        bool is_interpreter_function(Function *targetFunction) {
            if(!targetFunction->isDeclaration() && targetFunction->hasName()) {
                std::string func_name = targetFunction->getName().str();
                // errs() << "is_interpreter_function: " << func_name << "\n";
                for (const std::string &curr_func: interpreter_function_names) {
                    if (func_name.find(curr_func.c_str()) != std::string::npos) {
                    // if (targetFunction->getName().str() == curr_func.c_str()) {
                        return true;
                    }
                }
            }
            return false;
        }
        std::string get_vm_function_name(Function *targetFunction) {
            if(!targetFunction->isDeclaration() && targetFunction->hasName()) {
                std::string func_name = targetFunction->getName().str();
                // errs() << "is_interpreter_function: " << func_name << "\n";
                for (const std::string &curr_func: interpreter_function_names) {
                    size_t pos = func_name.find(curr_func.c_str());
                    if (pos != std::string::npos) {
                        if (func_name.length() <= pos+curr_func.length()) {
                            // just vm function self
                            return func_name;
                        } else {
                            return func_name.substr(pos+curr_func.length()+1);
                        }
                    }
                }
            }
            return "";
        }


/* ********************************************************************
*   GOVMINTERPRETER_CPP
***********************************************************************
*/

void GOVMInterpreter::construct_gv() {
    // pointer_size - 根据目标架构动态设置（必须是可修改的，因为解释器会设置它）
    unsigned ptr_size = modDataLayout->getPointerSize();
    Constant *pointer_size_initGV = ConstantInt::get(Type::getInt32Ty(Mod->getContext()), ptr_size);
    pointer_size_gv = new GlobalVariable(*Mod, Type::getInt32Ty(Mod->getContext()), 
                false,  GlobalValue::InternalLinkage, 
                pointer_size_initGV, "pointer_size_"+F->getName());

    // opcode_xorshift32_state      32bit
    Constant *opcode_xorshift32_state_initGV = ConstantInt::get(Type::getInt32Ty(Mod->getContext()), 0);
    opcode_xorshift32_state = new GlobalVariable(*Mod, Type::getInt32Ty(Mod->getContext()), 
                false,  GlobalValue::InternalLinkage, 
                opcode_xorshift32_state_initGV, "opcode_xorshift32_state_"+F->getName());

    // vm_code_state                32bit
    Constant *vm_code_state_initGV = ConstantInt::get(Type::getInt32Ty(Mod->getContext()), 0);
    vm_code_state = new GlobalVariable(*Mod, Type::getInt32Ty(Mod->getContext()), 
                false,  GlobalValue::InternalLinkage, 
                vm_code_state_initGV, "vm_code_state_"+F->getName());

    // exception_thrown
    Constant *exc_thrown_init = ConstantInt::get(Type::getInt8Ty(Mod->getContext()), 0);
    exc_thrown_gv = new GlobalVariable(*Mod, Type::getInt8Ty(Mod->getContext()),
        false, GlobalValue::InternalLinkage, exc_thrown_init, "exception_thrown");

    // exception_ptr
    Constant *exc_ptr_init = Constant::getNullValue(PointerType::get(Mod->getContext(), 0));
    exc_ptr_gv = new GlobalVariable(*Mod, PointerType::get(Mod->getContext(), 0),
        false, GlobalValue::InternalLinkage, exc_ptr_init, "exception_ptr");

    // exception_selector
    Constant *exc_sel_init = ConstantInt::get(Type::getInt32Ty(Mod->getContext()), 0);
    exc_sel_gv = new GlobalVariable(*Mod, Type::getInt32Ty(Mod->getContext()),
        false, GlobalValue::InternalLinkage, exc_sel_init, "exception_selector");

    // last_br_from_bb_id
    Constant *last_bb_init = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
    last_bb_gv = new GlobalVariable(*Mod, Type::getInt64Ty(Mod->getContext()),
        false, GlobalValue::InternalLinkage, last_bb_init, "last_br_from_bb_id");

    // current_bb_id
    Constant *curr_bb_init = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
    curr_bb_gv = new GlobalVariable(*Mod, Type::getInt64Ty(Mod->getContext()),
        false, GlobalValue::InternalLinkage, curr_bb_init, "current_bb_id");
}

// Function *govm_interpreter;

// Create debug print function using IRBuilder (like IdaDetect does)
// Uses integer ID instead of string to avoid string constant cloning issues
// If debug is disabled, creates a no-op function
static Function* createVmpDebugId(Module *M, bool debug_enabled) {
    LLVMContext &Ctx = M->getContext();
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {Int32Ty, Int64Ty}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M->getDataLayout().getProgramAddressSpace(),
        "vmp_debug_id",
        M
    );
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(EntryBB);
    
    if (!debug_enabled) {
        // Debug disabled: create no-op function
        Builder.CreateRetVoid();
        return Func;
    }
    
    // Debug enabled: create switch-based debug output
    BasicBlock *NewBBBlock = BasicBlock::Create(Ctx, "new_bb", Func);
    BasicBlock *OpcodeBlock = BasicBlock::Create(Ctx, "opcode", Func);
    BasicBlock *CmpBlock = BasicBlock::Create(Ctx, "cmp", Func);
    BasicBlock *DefaultBlock = BasicBlock::Create(Ctx, "default", Func);
    
    Value *Id = Func->arg_begin();
    Value *Val = Func->arg_begin() + 1;
    
    // Switch on ID
    SwitchInst *Switch = Builder.CreateSwitch(Id, DefaultBlock, 10);
    Switch->addCase(cast<ConstantInt>(ConstantInt::get(Int32Ty, 1)), NewBBBlock);
    Switch->addCase(cast<ConstantInt>(ConstantInt::get(Int32Ty, 2)), OpcodeBlock);
    Switch->addCase(cast<ConstantInt>(ConstantInt::get(Int32Ty, 3)), CmpBlock);
    
    // Get printf
    FunctionCallee PrintfFunc = M->getOrInsertFunction(
        "printf",
        FunctionType::get(Int32Ty, {CharPtrTy}, true)
    );
    
    // New BB block: printf("[BB] IP=%ld\n", val)
    IRBuilder<> NewBBBuilder(NewBBBlock);
    Constant *NewBBStr = ConstantDataArray::getString(Ctx, "[BB] IP=%ld\n");
    GlobalVariable *NewBBGV = new GlobalVariable(
        *M, NewBBStr->getType(), true,
        GlobalValue::PrivateLinkage, NewBBStr, ".vmp.dbg.bb"
    );
    NewBBBuilder.CreateCall(PrintfFunc, {
        ConstantExpr::getBitCast(NewBBGV, CharPtrTy), Val
    });
    NewBBBuilder.CreateBr(DefaultBlock);
    
    // Opcode block: printf("[OP] %ld\n", val)
    IRBuilder<> OpcodeBuilder(OpcodeBlock);
    Constant *OpcodeStr = ConstantDataArray::getString(Ctx, "[OP] %ld\n");
    GlobalVariable *OpcodeGV = new GlobalVariable(
        *M, OpcodeStr->getType(), true,
        GlobalValue::PrivateLinkage, OpcodeStr, ".vmp.dbg.op"
    );
    OpcodeBuilder.CreateCall(PrintfFunc, {
        ConstantExpr::getBitCast(OpcodeGV, CharPtrTy), Val
    });
    OpcodeBuilder.CreateBr(DefaultBlock);
    
    // CMP block: printf("[CMP] pred=%ld\n", val)
    IRBuilder<> CmpBuilder(CmpBlock);
    Constant *CmpStr = ConstantDataArray::getString(Ctx, "[CMP] pred=%ld\n");
    GlobalVariable *CmpGV = new GlobalVariable(
        *M, CmpStr->getType(), true,
        GlobalValue::PrivateLinkage, CmpStr, ".vmp.dbg.cmp"
    );
    CmpBuilder.CreateCall(PrintfFunc, {
        ConstantExpr::getBitCast(CmpGV, CharPtrTy), Val
    });
    CmpBuilder.CreateBr(DefaultBlock);
    
    // Default block: just return
    IRBuilder<> DefaultBuilder(DefaultBlock);
    DefaultBuilder.CreateRetVoid();
    
    return Func;
}

void GOVMInterpreter::run() {

    errs() << "[GOVMInterpreter] Starting run() for function: " << F->getName() << "\n";

    errs() << "[GOVMInterpreter] Step 1: Parsing bitcode...\n";
    Module *interpreter_module = llvm_parse_bitcode_from_string();
    if (!interpreter_module) {
        errs() << "[GOVMInterpreter] ERROR: Failed to parse bitcode\n";
        return;
    }
    errs() << "[GOVMInterpreter] Step 1: Bitcode parsed successfully\n";

    // Create debug function in target module (like IdaDetect does)
    // Controlled by -irobf-debug flag
    errs() << "[GOVMInterpreter] Step 2: Creating debug function...\n";
    Function *DebugIdFunc = createVmpDebugId(Mod, isIRObfuscationDebugEnabled());
    errs() << "[GOVMInterpreter] Step 2: Debug function created\n";

    // Replace debug function declaration in interpreter module
    errs() << "[GOVMInterpreter] Step 3: Replacing debug function...\n";
    if (Function *OldDebugId = interpreter_module->getFunction("vmp_debug_id")) {
        OldDebugId->replaceAllUsesWith(DebugIdFunc);
    }
    errs() << "[GOVMInterpreter] Step 3: Debug function replaced\n";

    // replace GlobalVariable 
    errs() << "[GOVMInterpreter] Step 4: Replacing global variables...\n";
    errs() << "[GOVMInterpreter]   gv_data_seg = " << (void*)gv_data_seg << "\n";
    errs() << "[GOVMInterpreter]   gv_code_seg = " << (void*)gv_code_seg << "\n";
    errs() << "[GOVMInterpreter]   ip = " << (void*)ip << "\n";
    errs() << "[GOVMInterpreter]   data_seg_addr = " << (void*)data_seg_addr << "\n";
    errs() << "[GOVMInterpreter]   code_seg_addr = " << (void*)code_seg_addr << "\n";
    
    std::vector<std::string> gv_list = {"gv_data_seg", "gv_code_seg", "ip", "data_seg_addr", "code_seg_addr", "pointer_size", "opcode_xorshift32_state", "vm_code_state", "exception_thrown", "exception_ptr", "exception_selector", "last_br_from_bb_id", "current_bb_id"};
    std::vector<GlobalVariable *> new_gv_list = {gv_data_seg, gv_code_seg, ip, data_seg_addr, code_seg_addr, pointer_size_gv, opcode_xorshift32_state, vm_code_state, exc_thrown_gv, exc_ptr_gv, exc_sel_gv, last_bb_gv, curr_bb_gv};
    
    std::map<GlobalVariable*, GlobalVariable*> gv_remap;
    for (unsigned i = 0; i < gv_list.size(); i++) {
        GlobalVariable *old_gv = interpreter_module->getGlobalVariable(gv_list[i]);
        if (!old_gv) {
            errs() << "[GOVMInterpreter]   WARNING: old_gv '" << gv_list[i] << "' not found in interpreter module\n";
            continue;
        }
        GlobalVariable *new_gv = new_gv_list[i];
        if (!new_gv) {
            errs() << "[GOVMInterpreter]   WARNING: new_gv '" << gv_list[i] << "' is null\n";
            continue;
        }
        
        errs() << "[GOVMInterpreter]   Replacing " << gv_list[i] << ": old type=" << *old_gv->getValueType() << ", new type=" << *new_gv->getValueType() << "\n";
        gv_remap[old_gv] = new_gv;
    }
    errs() << "[GOVMInterpreter] Step 4: Global variables mapped, gv_remap size=" << gv_remap.size() << "\n";

    // replace call_handler
    errs() << "[GOVMInterpreter] Step 5: Replacing call_handler...\n";
    Function *old_func = interpreter_module->getFunction("call_handler");
    if (old_func && callinst_handler) {
        old_func->replaceAllUsesWith(callinst_handler);
    }
    errs() << "[GOVMInterpreter] Step 5: call_handler replaced\n";

    // clone functions
    // First, collect all function declarations from interpreter module
    // and add them to target module (including intrinsics)
    errs() << "[GOVMInterpreter] Step 6: Collecting function declarations...\n";
    for(auto Func = interpreter_module->begin(); Func != interpreter_module->end(); ++Func) {
        Function *fun = &*Func;
        if(fun->isDeclaration()) {
            // Check if function already exists in target module
            if(!Mod->getFunction(fun->getName())) {
                FunctionCallee FC = Mod->getOrInsertFunction(fun->getName().str(), fun->getFunctionType());
                Function *NewF = cast<Function>(FC.getCallee());
                NewF->setLinkage(fun->getLinkage());
            }
        }
    }
    errs() << "[GOVMInterpreter] Step 6: Function declarations collected\n";
    
    errs() << "[GOVMInterpreter] Step 7: Cloning interpreter functions...\n";
    int func_idx = 0;
    for(auto Func = interpreter_module->begin();Func!=interpreter_module->end();++Func)
        {
            
            Function *fun = &*Func;

            if(is_interpreter_function(fun)) {
                errs() << "[GOVMInterpreter]   Cloning function: " << fun->getName() << " (idx=" << func_idx++ << ")\n";
                FunctionCallee FC = Mod->getOrInsertFunction(fun->getName().str(), fun->getFunctionType());
                Function *NewF = cast<Function>(FC.getCallee());
                NewF->setLinkage(llvm::GlobalValue::LinkageTypes::InternalLinkage);


                ValueToValueMapTy VMap;
                SmallVector<ReturnInst*, 8> returns;

                if (DebugIdFunc) {
                    VMap[interpreter_module->getFunction("vmp_debug_id")] = DebugIdFunc;
                }

                for (auto &gv_pair : gv_remap) {
                    VMap[gv_pair.first] = gv_pair.second;
                }

                errs() << "[GOVMInterpreter]     Processing instructions...\n";
                for (Instruction &I : instructions(fun)) {
                    if (CallBase *CB = dyn_cast<CallBase>(&I)) {
                        Function *Callee = CB->getCalledFunction();
                        if (Callee && Callee != fun) {
                            Function *TargetCallee = Mod->getFunction(Callee->getName());
                            if (!TargetCallee && Callee->isDeclaration()) {
                                FunctionCallee FC2 = Mod->getOrInsertFunction(Callee->getName().str(), Callee->getFunctionType());
                                TargetCallee = cast<Function>(FC2.getCallee());
                                TargetCallee->setLinkage(Callee->getLinkage());
                            }
                            if (TargetCallee) {
                                VMap[Callee] = TargetCallee;
                            }
                        }
                    }
                }

                errs() << "[GOVMInterpreter]     Setting up arguments...\n";
                Function::arg_iterator DestI = NewF->arg_begin();

                for (const Argument & I : fun->args())
                    if (VMap.count(&I) == 0) {
                        DestI->setName(I.getName());
                        VMap[&I] = &*DestI++;
                    }

                errs() << "[GOVMInterpreter]     Calling CloneFunctionInto...\n";
                CloneFunctionInto(NewF, fun, VMap, CloneFunctionChangeType::DifferentModule, returns);
                errs() << "[GOVMInterpreter]     CloneFunctionInto completed\n";

                NewF->setName(fun->getName()+"_"+F->getName());

                std::vector<CallInst *> F_users;
                for (User *U : fun->users()) {
                    if (CallInst *CI = dyn_cast<CallInst>(U)) {
                        F_users.push_back(CI);
                    }
                }

                // replace references
                for (CallInst *CI: F_users) {
                    // errs() << "[*] Replacing references: " << *CI << "\n";
                    CI->setCalledFunction(NewF);
                }
                
            }

            
        }
    
    
    // remove all function of interpreter_module
    while(true) {
        bool flag = true;
        for(auto Func = interpreter_module->begin(), Funcend = interpreter_module->end();Func!=Funcend;++Func) {

            Function *fun = dyn_cast<Function>(&*Func);
            
            if(fun->use_empty()) {
                // errs() << "[*] Removing function: " << fun->getName().str() << "\n";
                flag = false;
                fun->eraseFromParent();
                break;
            }
        }
        if (flag)
            break;
    }

    errs() << "[GOVMInterpreter] Step 7: All interpreter functions cloned\n";
}
namespace {


    std::string readAnnotate(Function *f) {
        std::string annotation = "";

        // Get annotation variable
        GlobalVariable *glob =
            f->getParent()->getGlobalVariable("llvm.global.annotations");

        if (glob != NULL && glob->hasInitializer()) {
            Constant *init = glob->getInitializer();
            if (!init) return annotation;
            
            // Get the array
            if (ConstantArray *ca = dyn_cast<ConstantArray>(init)) {
            for (unsigned i = 0; i < ca->getNumOperands(); ++i) {
                // Get the struct
                if (ConstantStruct *structAn =
                        dyn_cast<ConstantStruct>(ca->getOperand(i))) {
                    
                // Get the annotated function
                // Can be a ConstantExpr (BitCast) or a direct Function pointer
                Function *annotatedFunc = nullptr;
                Value *op0 = structAn->getOperand(0);
                
                if (ConstantExpr *expr = dyn_cast<ConstantExpr>(op0)) {
                    // It's a ConstantExpr (e.g., BitCast)
                    if (expr->getOpcode() == Instruction::BitCast) {
                        annotatedFunc = dyn_cast<Function>(expr->getOperand(0));
                    }
                } else if (Function *directFunc = dyn_cast<Function>(op0)) {
                    // It's a direct Function pointer
                    annotatedFunc = directFunc;
                }
                
                if (annotatedFunc == f) {
                    // Found annotation for this function
                    // Get the annotation string
                    // Can be a ConstantExpr (GetElementPtr) or a direct GlobalVariable
                    Value *op1 = structAn->getOperand(1);
                    GlobalVariable *annoteStr = nullptr;
                    
                    if (ConstantExpr *note = dyn_cast<ConstantExpr>(op1)) {
                        // It's a ConstantExpr (e.g., GetElementPtr)
                        if (note->getOpcode() == Instruction::GetElementPtr) {
                            annoteStr = dyn_cast<GlobalVariable>(note->getOperand(0));
                        }
                    } else if (GlobalVariable *directGV = dyn_cast<GlobalVariable>(op1)) {
                        // It's a direct GlobalVariable pointer
                        annoteStr = directGV;
                    }
                    
                    if (annoteStr) {
                        if (ConstantDataSequential *data =
                                dyn_cast<ConstantDataSequential>(annoteStr->getInitializer())) {
                            if (data->isString()) {
                                annotation += data->getAsString().lower() + " ";
                            }
                        }
                    }
                }
                }
            }
            }
        }
        return annotation;
        }
    bool toObfuscateModule(bool global_flag,Module* M,std::string attribute)
        {
        std::string attr = "cpp_" + attribute;
        std::string noattr = "cpp_no" + attribute;
        for(auto Func = M->begin();Func!=M->end();++Func)
        {
            if(Function * f = dyn_cast<Function>(Func))
            {
            if (readAnnotate(f).find(attr) != std::string::npos)
            {
                return true;
            }
            if (readAnnotate(f).find(noattr) != std::string::npos)
            {
                return false;
            }
            }
        }
        return global_flag;
            
        }

    std::set<string> used_passes;
    bool toObfuscateFunction(bool global_flag, Function *f, std::string attribute) {
        std::string attr = attribute;
        std::string attrNo = "no" + attr;
        
        // 读取注解
        std::string annotation = readAnnotate(f);
        
        // 检查是否有novmp注解
        if (annotation.find(attrNo) != std::string::npos) {
            return false;
        }

        // 检查是否有vmp注解
        if (annotation.find(attr) != std::string::npos) {
            used_passes.insert(attribute);
            return true;
        }
        
        // VMP只处理有明确vmp注解的函数，不处理没有注解的函数
        // 这与FLA等其他混淆不同，因为VMP会显著改变函数结构
        return false;
    }

struct VMProtect : public ModulePass {
  static char ID;
  bool flag;
  VMProtect() : ModulePass(ID) {}
  VMProtect(bool flag): ModulePass(ID)
  {
    this->flag = flag;
  }
  virtual bool runOnModule(Module &M)
  {

      if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] VMProtect: Starting runOnModule\n";
      }
      
      // 先收集所有需要处理的函数，避免在遍历时修改模块
      std::vector<Function *> functions_to_process;
      for(auto Func = M.begin(); Func != M.end(); ++Func)
      {
        Function *F = &*Func;
        // errs() << "[VMProtect] Checking function: " << F->getName() << "\n";
        
        if(toObfuscateFunction(this->flag,F,"vmp"))
        {
          if(F->isVarArg()) {
            continue;
          }
          // VMP 函数使用 O0 优化，添加 OptimizeNone 防止优化
          // 添加 AlwaysInline 强制内联
          F->addFnAttr(Attribute::AlwaysInline);
          F->addFnAttr(Attribute::OptimizeNone);
          // 跳过标准库函数（只跳过明确的标准库函数，不跳过用户函数）
          // 带vmp注解的函数不进行标准库名称过滤
          std::string funcName = F->getName().str();
          
          bool is_stdlib = false;
          
          if(F->isDeclaration()) {
            // 跳过以__开头（编译器内部函数）
            if(funcName.length() >= 2 && funcName[0] == '_' && funcName[1] == '_') {
              is_stdlib = true;
            }
            // 跳过C标准库函数
            else if(funcName.find("printf") != std::string::npos ||
               funcName.find("sprintf") != std::string::npos ||
               funcName.find("fprintf") != std::string::npos ||
               funcName.find("vsprintf") != std::string::npos ||
               funcName.find("vfprintf") != std::string::npos ||
               funcName.find("vsnprintf") != std::string::npos ||
               funcName.find("local_stdio") != std::string::npos ||
               funcName.find("frexp") != std::string::npos ||
               funcName.find("ldexp") != std::string::npos ||
               funcName.find("modf") != std::string::npos ||
               funcName.find("scalbn") != std::string::npos ||
               funcName.find("ilogb") != std::string::npos ||
               funcName.find("logb") != std::string::npos ||
               funcName.find("copysign") != std::string::npos ||
               funcName.find("nan") != std::string::npos ||
               funcName.find("nextafter") != std::string::npos ||
               funcName.find("fdim") != std::string::npos ||
               funcName.find("fmax") != std::string::npos ||
               funcName.find("fmin") != std::string::npos ||
               funcName.find("fma") != std::string::npos ||
               funcName.find("isnan") != std::string::npos ||
               funcName.find("isinf") != std::string::npos ||
               funcName.find("isfinite") != std::string::npos ||
               funcName.find("fabs") != std::string::npos ||
               funcName.find("ceil") != std::string::npos ||
               funcName.find("floor") != std::string::npos ||
               funcName.find("round") != std::string::npos ||
               funcName.find("trunc") != std::string::npos ||
               funcName.find("sqrt") != std::string::npos ||
               funcName.find("pow") != std::string::npos ||
               funcName.find("exp") != std::string::npos ||
               funcName.find("log") != std::string::npos ||
               funcName.find("sin") != std::string::npos ||
               funcName.find("cos") != std::string::npos ||
               funcName.find("tan") != std::string::npos ||
               funcName.find("asin") != std::string::npos ||
               funcName.find("acos") != std::string::npos ||
               funcName.find("atan") != std::string::npos ||
               funcName.find("atan2") != std::string::npos ||
               funcName.find("sinh") != std::string::npos ||
               funcName.find("cosh") != std::string::npos ||
               funcName.find("tanh") != std::string::npos) {
              is_stdlib = true;
            }
            // 跳过C++标准库函数（检查std::命名空间）
            else if(funcName.find("std::") != std::string::npos ||
               funcName.find("basic_ostream") != std::string::npos ||
               funcName.find("basic_ios") != std::string::npos ||
               funcName.find("basic_istream") != std::string::npos ||
               funcName.find("basic_string") != std::string::npos ||
               funcName.find("basic_iostream") != std::string::npos ||
               funcName.find("basic_fstream") != std::string::npos ||
               funcName.find("basic_ifstream") != std::string::npos ||
               funcName.find("basic_ofstream") != std::string::npos ||
               funcName.find("basic_stringbuf") != std::string::npos ||
               funcName.find("basic_istringstream") != std::string::npos ||
               funcName.find("basic_ostringstream") != std::string::npos ||
               funcName.find("basic_stringstream") != std::string::npos ||
               funcName.find("ctype") != std::string::npos ||
               funcName.find("locale") != std::string::npos ||
               funcName.find("char_traits") != std::string::npos ||
               funcName.find("numpunct") != std::string::npos ||
               funcName.find("num_put") != std::string::npos ||
               funcName.find("allocator") != std::string::npos ||
               funcName.find("ios_base") != std::string::npos ||
               funcName.find("ostreambuf") != std::string::npos ||
               funcName.find("istreambuf") != std::string::npos ||
               funcName.find("bad_cast") != std::string::npos ||
               funcName.find("bad_alloc") != std::string::npos ||
               funcName.find("exception") != std::string::npos ||
               funcName.find("bad_exception") != std::string::npos ||
               funcName.find("runtime_error") != std::string::npos ||
               funcName.find("logic_error") != std::string::npos ||
               funcName.find("out_of_range") != std::string::npos ||
               funcName.find("length_error") != std::string::npos ||
               funcName.find("domain_error") != std::string::npos ||
               funcName.find("invalid_argument") != std::string::npos ||
               funcName.find("range_error") != std::string::npos ||
               funcName.find("overflow_error") != std::string::npos ||
               funcName.find("underflow_error") != std::string::npos) {
              is_stdlib = true;
            }
          }
          
          if(is_stdlib) {
            continue;
          }
          functions_to_process.push_back(F);
        }
      }
      
      // 如果没有函数需要保护，直接返回
      if (functions_to_process.empty()) {
        return false;
      }
      
      // 处理收集到的函数
      int func_count = 0;
      for(Function *F : functions_to_process)
      {
        if (isIRObfuscationDebugEnabled()) {
          errs() << "[VMP] Processing function: " << F->getName() << "\n";
        }
        
        GOVMTranslator * translator = new GOVMTranslator(F);
        
        if (!translator->run()) {
          continue;
        }
        
        GOVMInterpreter * interpreter = new GOVMInterpreter(F, translator->get_callinst_handler(),
                                                              translator->get_gv_data_seg(),
                                                              translator->get_gv_code_seg(),
                                                              translator->get_ip(),
                                                              translator->get_data_seg_addr(),
                                                              translator->get_code_seg_addr());
        interpreter->run();
        
        Function *vm_interpreter_func = F->getParent()->getFunction("vm_interpreter_"+F->getName().str());
        
        GOVMModifier * modifier = new GOVMModifier(F, translator->get_gv_value_map(), translator->get_value_map(),
                                                    translator->get_gv_data_seg(),
                                                    translator->get_gv_code_seg(),
                                                    translator->get_ip(),
                                                    translator->get_data_seg_addr(),
                                                    translator->get_code_seg_addr(),
                                                    vm_interpreter_func,
                                                    translator->get_data_seg_size());
        modifier->run();
        
        if (isIRObfuscationDebugEnabled()) {
          errs() << "[VMP] Function done: " << F->getName() << "\n";
        }
        func_count++;
      }
      
      return true;

  }


}; // end of struct InlineFunction
}  // end of anonymous namespace

char VMProtect::ID = 0;
static RegisterPass<VMProtect> X("aVMP", "aVMP",false,true);
Pass *llvm::createVMProtectPass(bool flag) {
  return new VMProtect(flag);
}

// New PassManager support
PreservedAnalyses llvm::VMProtectPass::run(Module &M, ModuleAnalysisManager &AM) {
  bool changed = false;
  
  std::vector<Function *> functions_to_process;
  for(auto Func = M.begin(); Func != M.end(); ++Func)
  {
    Function *F = &*Func;
    if(toObfuscateFunction(this->flag,F,"vmp"))
    {
      if(F->isVarArg()) {
        continue;
      }
      // 为 VMP 函数添加 inline 属性，允许内联展开
      F->addFnAttr(Attribute::AlwaysInline);
      std::string funcName = F->getName().str();
      
      bool is_stdlib = false;
      
      // 只对声明函数（外部函数）进行标准库过滤
      if(F->isDeclaration()) {
        if(funcName.find("printf") != std::string::npos ||
           funcName.find("sprintf") != std::string::npos ||
           funcName.find("fprintf") != std::string::npos ||
           funcName.find("malloc") != std::string::npos ||
           funcName.find("free") != std::string::npos ||
           funcName.find("std::") != std::string::npos ||
           funcName.find("basic_ostream") != std::string::npos ||
           funcName.find("basic_string") != std::string::npos) {
          is_stdlib = true;
        }
      }
      
      if(is_stdlib) {
        continue;
      }
      
      functions_to_process.push_back(F);
    }
  }
  
  // 如果没有函数需要保护，直接返回
  if (functions_to_process.empty()) {
    return PreservedAnalyses::all();
  }
  
  int func_count = 0;
  for(Function *F : functions_to_process) {
    GOVMTranslator * translator = new GOVMTranslator(F);
    
    if (!translator->run()) {
      continue;
    }
    
    GOVMInterpreter * interpreter = new GOVMInterpreter(F, translator->get_callinst_handler(),
                                                          translator->get_gv_data_seg(),
                                                          translator->get_gv_code_seg(),
                                                          translator->get_ip(),
                                                          translator->get_data_seg_addr(),
                                                          translator->get_code_seg_addr());
    
    interpreter->run();
    
    Function *vm_interpreter_func = F->getParent()->getFunction("vm_interpreter_"+F->getName().str());
    
    GOVMModifier * modifier = new GOVMModifier(F, translator->get_gv_value_map(), translator->get_value_map(),
                                                translator->get_gv_data_seg(),
                                                translator->get_gv_code_seg(),
                                                translator->get_ip(),
                                                translator->get_data_seg_addr(),
                                                translator->get_code_seg_addr(),
                                                vm_interpreter_func,
                                                translator->get_data_seg_size());
    
    modifier->run();
    
    func_count++;
    changed = true;
  }
  
  return changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}
#endif

