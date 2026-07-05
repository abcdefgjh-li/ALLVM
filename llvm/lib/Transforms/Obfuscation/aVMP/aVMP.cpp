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
#include "llvm/IR/GlobalAlias.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
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
#include "llvm/Transforms/Obfuscation/aVMP/aVMP.h"
#include "llvm/Transforms/Obfuscation/aVMP/vm.h"
#include "llvm/Transforms/Obfuscation/aVMP/aVMPCrypto.h"
#include "llvm/Transforms/Obfuscation/aVMP/aVMPDispatcher.h"
#include <assert.h>

static uint64_t vmpSplitmix64NextLocal(uint64_t &State) {
    State += 0x9e3779b97f4a7c15ULL;
    uint64_t Value = State;
    Value ^= Value >> 30;
    Value *= 0xbf58476d1ce4e5b9ULL;
    Value ^= Value >> 27;
    Value *= 0x94d049bb133111ebULL;
    Value ^= Value >> 31;
    return Value;
}

uint64_t deriveVMFunctionKey(const llvm::Function &F);
uint64_t deriveBBToken(uint64_t FunctionKey, uint32_t BBOffset);
uint32_t deriveBBTag(uint64_t FunctionKey, uint32_t BBOffset);
uint64_t deriveOpcodeSeed(uint64_t BBToken);
uint64_t deriveVMSeed(uint64_t FunctionKey, uint64_t BBToken);
uint64_t deriveChainSeed(uint64_t FunctionKey, uint64_t BBToken,
                         uint32_t BBOffset);
void deriveChaCha20Material(uint64_t FunctionKey, uint64_t PayloadSeed,
                            uint64_t ChainSeed, uint32_t BBOffset,
                            uint32_t KeyWords[8], uint32_t NonceWords[3]);
void chacha20Block(const uint32_t KeyWords[8], uint32_t Counter,
                   const uint32_t NonceWords[3], uint8_t Out[64]);
uint8_t chacha20ByteAt(uint64_t FunctionKey, uint64_t PayloadSeed,
                       uint64_t ChainSeed, uint32_t BBOffset,
                       uint32_t PayloadIndex);

#include <cstdint>
#include <functional>
#include <map>
#include <set>

using namespace llvm;
using namespace std;

// 异常处理转换 Pass
namespace {

struct ExceptionToErrorHandlingPass : public FunctionPass {
  static char ID;
  ExceptionToErrorHandlingPass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    // 检查函数是否使用了异常处理
    bool hasExceptions = false;
    SmallVector<Instruction*, 16> toRemove;

    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
      Instruction *Inst = &*I;

      if (isa<InvokeInst>(Inst) || isa<LandingPadInst>(Inst) ||
          isa<ResumeInst>(Inst) || isa<CatchSwitchInst>(Inst) ||
          isa<CatchReturnInst>(Inst) || isa<CleanupReturnInst>(Inst)) {
        hasExceptions = true;
        break;
      }
    }

    if (!hasExceptions) {
      return false;
    }

    // Legacy pass warning: the main VMProtect pipeline has partial EH support now,
    // but this compatibility pass still declines EH-heavy functions when invoked.
    errs() << "[VMP Legacy Warning] Function '" << F.getName()
           << "' uses C++ exception handling.\n";
    errs() << "[VMP Legacy Warning] This compatibility pass is outdated relative to the main VMP EH support.\n";
    errs() << "[VMP Legacy Warning] If this pass is enabled, VMP protection will be skipped for this function.\n";

    // 移除 VMP 保护属性
    F.removeFnAttr("vmp");

    return false;
  }
};

}

char ExceptionToErrorHandlingPass::ID = 0;
static RegisterPass<ExceptionToErrorHandlingPass>
    ExceptionToErrorHandlingPassReg("exception-to-error", "Convert C++ exceptions to error handling", false, false);
#include <memory>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#define ENABLE_VMP
#ifdef ENABLE_VMP

using namespace llvm;
using namespace std;

static void normalizeLocalDefaultVisibility(GlobalValue *GV) {
    if (!GV || !GV->hasLocalLinkage()) {
        return;
    }
    if (GV->getVisibility() != GlobalValue::DefaultVisibility) {
        GV->setVisibility(GlobalValue::DefaultVisibility);
    }
}

static void normalizeModuleLocalDefaultVisibility(Module &M) {
    for (Function &F : M) {
        normalizeLocalDefaultVisibility(&F);
    }
    for (GlobalVariable &GV : M.globals()) {
        normalizeLocalDefaultVisibility(&GV);
    }
    for (GlobalAlias &GA : M.aliases()) {
        normalizeLocalDefaultVisibility(&GA);
    }
}
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

static bool isKnownStdlibLikeFunctionName(const std::string &funcName) {
    if (funcName.length() >= 2 && funcName[0] == '_' && funcName[1] == '_') {
        return true;
    }
    if (funcName.find("__syscall_") == 0 || funcName.find("__real_") == 0) {
        return true;
    }
    if (funcName.find("St6__ndk1") != std::string::npos ||
        funcName.find("St3__1") != std::string::npos ||
        funcName.find("std::") != std::string::npos ||
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
        funcName.find("exception") != std::string::npos ||
        funcName.find("random_device") != std::string::npos ||
        funcName.find("mt19937") != std::string::npos ||
        funcName.find("mt19937_64") != std::string::npos ||
        funcName.find("minstd_rand") != std::string::npos ||
        funcName.find("minstd_rand0") != std::string::npos ||
        funcName.find("ranlux24") != std::string::npos ||
        funcName.find("ranlux48") != std::string::npos ||
        funcName.find("knuth_b") != std::string::npos ||
        funcName.find("linear_congruential_engine") != std::string::npos ||
        funcName.find("mersenne_twister_engine") != std::string::npos ||
        funcName.find("subtract_with_carry_engine") != std::string::npos ||
        funcName.find("discard_block_engine") != std::string::npos ||
        funcName.find("independent_bits_engine") != std::string::npos ||
        funcName.find("shuffle_order_engine") != std::string::npos ||
        funcName.find("uniform_int_distribution") != std::string::npos ||
        funcName.find("uniform_real_distribution") != std::string::npos ||
        funcName.find("bernoulli_distribution") != std::string::npos ||
        funcName.find("binomial_distribution") != std::string::npos ||
        funcName.find("geometric_distribution") != std::string::npos ||
        funcName.find("negative_binomial_distribution") != std::string::npos ||
        funcName.find("poisson_distribution") != std::string::npos ||
        funcName.find("exponential_distribution") != std::string::npos ||
        funcName.find("gamma_distribution") != std::string::npos ||
        funcName.find("weibull_distribution") != std::string::npos ||
        funcName.find("extreme_value_distribution") != std::string::npos ||
        funcName.find("normal_distribution") != std::string::npos ||
        funcName.find("lognormal_distribution") != std::string::npos ||
        funcName.find("chi_squared_distribution") != std::string::npos ||
        funcName.find("cauchy_distribution") != std::string::npos ||
        funcName.find("fisher_f_distribution") != std::string::npos ||
        funcName.find("student_t_distribution") != std::string::npos ||
        funcName.find("discrete_distribution") != std::string::npos ||
        funcName.find("piecewise_constant_distribution") != std::string::npos ||
        funcName.find("piecewise_linear_distribution") != std::string::npos ||
        funcName.find("seed_seq") != std::string::npos) {
        return true;
    }
    return false;
}

static bool hasFunctionAnnotationKeyword(Function *F, StringRef keyword) {
    if (!F || !F->getParent()) {
        return false;
    }

    GlobalVariable *glob = F->getParent()->getGlobalVariable("llvm.global.annotations");
    if (!glob || !glob->hasInitializer()) {
        return false;
    }

    auto *ca = dyn_cast<ConstantArray>(glob->getInitializer());
    if (!ca) {
        return false;
    }

    for (unsigned i = 0; i < ca->getNumOperands(); ++i) {
        auto *cs = dyn_cast<ConstantStruct>(ca->getOperand(i));
        if (!cs || cs->getNumOperands() < 2) {
            continue;
        }

        Value *annotated = cs->getOperand(0)->stripPointerCasts();
        if (annotated != F) {
            continue;
        }

        Value *annotationValue = cs->getOperand(1)->stripPointerCasts();
        auto *annotationGV = dyn_cast<GlobalVariable>(annotationValue);
        if (!annotationGV || !annotationGV->hasInitializer()) {
            continue;
        }

        auto *annotationData = dyn_cast<ConstantDataSequential>(annotationGV->getInitializer());
        if (!annotationData || !annotationData->isCString()) {
            continue;
        }

        if (annotationData->getAsCString().contains(keyword)) {
            return true;
        }
    }

    return false;
}

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
#define CATCHSWITCH_OP      0x17  // 新增：异常分发
#define CALLBR_OP           0x18  // 新增：CallBr 指令
#define FENCE_OP            0x19  // 新增：内存屏障指令
#define ATOMIC_CMPXCHG_OP   0x1A  // 新增：原子比较交换指令
#define ATOMIC_RMW_OP       0x1B  // 新增：原子读-修改-写指令
#define VAARG_OP            0x1C  // 新增：可变参数指令

#define OP_TOTAL            0x1C





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


// GEP 信息结构体（全局定义，供 GOVMTranslator 和 GOVMModifier 共享）
struct GEPInfo {
    GlobalVariable *GV;
    int gep_offset;  // GEP 计算出的偏移量
};

struct BlockAddressInfo {
    BlockAddress *BA;
};

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
            this->has_unsupported_instruction = false;  // 初始化标志
            this->vm_function_key = deriveVMFunctionKey(*F);
            this->gv_code_seg = nullptr;
            this->gv_data_seg = nullptr;
            this->ip = nullptr;
            this->data_seg_addr = nullptr;
            this->code_seg_addr = nullptr;
            this->exception_thrown = nullptr;
            this->exception_ptr_global = nullptr;
            this->exception_selector_global = nullptr;
            this->last_br_from_bb_id = nullptr;
            this->current_bb_id = nullptr;
            this->vmp_debug_enabled_gv = nullptr;
            this->callinst_handler = nullptr;
            this->callinst_handler_conBBL = nullptr;
            this->callinst_handler_entryBB = nullptr;
            this->targetfunc_id = nullptr;
            this->callinst_handler_curr_idx = 0;
            this->init_opcode_variant();
        }

        Module * Mod;
        Function * F;
        DataLayout * modDataLayout;
        unsigned pointer_size;  // 动态获取的指针大小,支持不同架构
        bool has_unsupported_instruction;  // 标记是否遇到不支持的指令
        uint64_t vm_function_key;

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
        GlobalVariable *vmp_debug_enabled_gv;  // Debug mode control

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
        std::map<Value *, int> alloca_area_map;
        std::map<BasicBlock *, int> basicblock_map;
        std::vector<pair<int, BasicBlock *>> br_map;
        std::map<CallBase *, long long> callinst_map;

        // LLVM 21: SwitchInst支持
        std::vector<std::tuple<int, BasicBlock *, int>> switch_map;  // (code_pos, target_bb, case_value)

        // collect global variable
        std::map<GlobalVariable *, int> gv_value_map;

        // collect GEP results (保存 GEP 信息：全局变量和偏移量)
        std::map<int, GEPInfo> gep_info_map;  // key: data_seg 中的偏移量

        // collect BlockAddress constants used by indirectbr/callbr dispatch
        std::map<int, BlockAddressInfo> blockaddress_info_map;  // key: data_seg 中的偏移量
        std::vector<std::tuple<Instruction *, unsigned, ConstantExpr *, Instruction *>> lowered_constexprs;

        // current offset in data_seg
        int curr_data_offset = 0;


        virtual bool run ();
        virtual bool prescan_supported_ir();
        virtual bool is_supported_instruction(Instruction *ins);
        virtual void rollback_created_ir();
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
        Function *get_function() { return this->F; }

        GlobalVariable *get_gv_data_seg() { return this->gv_data_seg; }
        GlobalVariable *get_gv_code_seg() { return this->gv_code_seg; }
        GlobalVariable *get_ip() { return this->ip; }
        GlobalVariable *get_data_seg_addr() { return this->data_seg_addr; }
        GlobalVariable *get_code_seg_addr() { return this->code_seg_addr; }
        uint64_t get_vm_function_key() const { return this->vm_function_key; }
        int get_data_seg_size() { return this->curr_data_offset; }
        std::map<GlobalVariable *, int> *get_gv_value_map() { return &gv_value_map; }
        std::map<int, GEPInfo> *get_gep_info_map() { return &gep_info_map; }
        std::map<int, BlockAddressInfo> *get_blockaddress_info_map() { return &blockaddress_info_map; }
        std::map<Value *, int> *get_value_map() { return &value_map; }

    private:
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
        uint64_t xorshift32_state = 0;
        uint64_t xorshift32_seed = 0;
        uint8_t opcode_encode_map[OP_TOTAL + 1] = {};
        uint8_t opcode_decode_map[OP_TOTAL + 1] = {};

        /* encrypt vm_code */
        // mark seed for each basicblock
        std::vector<std::tuple<uint64_t, uint64_t, uint32_t, uint32_t>> vm_code_seed_map;

        void init_xorshift32() {
            xorshift32_seed = vm_function_key;
        }

        /* The state word must be initialized to non-zero */
        uint64_t xorshift32(uint64_t *state)
        {
            return vmpSplitmix64NextLocal(*state);
        }

        void init_opcode_variant() {
            for (unsigned i = 0; i <= OP_TOTAL; ++i) {
                opcode_encode_map[i] = 0;
                opcode_decode_map[i] = 0;
            }

            for (unsigned i = 1; i <= OP_TOTAL; ++i) {
                opcode_decode_map[i] = static_cast<uint8_t>(i);
            }

            uint64_t state = vm_function_key ^ 0x6d2b79f5aa17c3e9ULL;
            if (state == 0) {
                state = 0x9e3779b97f4a7c15ULL;
            }

            for (int i = OP_TOTAL; i > 1; --i) {
                uint64_t r = vmpSplitmix64NextLocal(state);
                int j = static_cast<int>(r % static_cast<uint64_t>(i)) + 1;
                uint8_t tmp = opcode_decode_map[i];
                opcode_decode_map[i] = opcode_decode_map[j];
                opcode_decode_map[j] = tmp;
            }

            for (unsigned ordinal = 1; ordinal <= OP_TOTAL; ++ordinal) {
                uint8_t semantic = opcode_decode_map[ordinal];
                if (semantic <= OP_TOTAL) {
                    opcode_encode_map[semantic] = static_cast<uint8_t>(ordinal);
                }
            }

            if (isIRObfuscationDebugEnabled()) {
                errs() << "[VMP_VARIANT] function=" << F->getName()
                       << " key=0x" << Twine::utohexstr(vm_function_key)
                       << " semantic_to_ordinal=";
                for (unsigned semantic = 1; semantic <= OP_TOTAL; ++semantic) {
                    errs() << semantic << ":" << (unsigned)opcode_encode_map[semantic];
                    if (semantic != OP_TOTAL) {
                        errs() << ",";
                    }
                }
                errs() << "\n";
            }
        }

        void encrypt_vm_code() {
            const bool debugEnabled = isIRObfuscationDebugEnabled();
            for (auto &p : vm_code_seed_map) {
                uint64_t vm_code_seed = std::get<0>(p);
                uint64_t chain_seed = std::get<1>(p);
                uint32_t begin = std::get<2>(p);
                uint32_t end = std::get<3>(p);
                if (begin > end || end > vm_code.size()) {
                    if (debugEnabled) {
                        errs() << "[VM_ENCRYPT_WARN] invalid_range begin=" << begin
                               << " end=" << end
                               << " vm_code_size=" << vm_code.size()
                               << " seed=0x" << Twine::utohexstr(vm_code_seed) << "\n";
                    }
                    continue;
                }
                if (begin == end) {
                    if (debugEnabled) {
                        errs() << "[VM_ENCRYPT_WARN] empty_range begin=" << begin
                               << " seed=0x" << Twine::utohexstr(vm_code_seed) << "\n";
                    }
                    continue;
                }

                uint8_t first_before = vm_code[begin];
                uint32_t bb_offset = begin >= 4 ? begin - 4 : 0;
                uint8_t first_key =
                    chacha20ByteAt(vm_function_key, vm_code_seed, chain_seed,
                                   bb_offset, 0);
                if (debugEnabled && bb_offset == 0) {
                    uint32_t key_words[8];
                    uint32_t nonce_words[3];
                    uint8_t block[64];
                    deriveChaCha20Material(vm_function_key, vm_code_seed, chain_seed,
                                           bb_offset, key_words, nonce_words);
                    chacha20Block(key_words, 0, nonce_words, block);
                    errs() << "[VM_BB0_ENCRYPT] function_key=0x"
                           << Twine::utohexstr(vm_function_key)
                           << " vm_seed=0x" << Twine::utohexstr(vm_code_seed)
                           << " chain_seed=0x" << Twine::utohexstr(chain_seed)
                           << " first_plain=0x" << Twine::utohexstr(first_before)
                           << " first_key=0x" << Twine::utohexstr(first_key)
                           << " first_cipher=0x"
                           << Twine::utohexstr((uint8_t)(first_before ^ first_key))
                           << "\n";
                    errs() << "[VM_BB0_MATERIAL] key="
                           << format_hex_no_prefix(key_words[0], 8) << " "
                           << format_hex_no_prefix(key_words[1], 8) << " "
                           << format_hex_no_prefix(key_words[2], 8) << " "
                           << format_hex_no_prefix(key_words[3], 8) << " "
                           << format_hex_no_prefix(key_words[4], 8) << " "
                           << format_hex_no_prefix(key_words[5], 8) << " "
                           << format_hex_no_prefix(key_words[6], 8) << " "
                           << format_hex_no_prefix(key_words[7], 8)
                           << " nonce="
                           << format_hex_no_prefix(nonce_words[0], 8) << " "
                           << format_hex_no_prefix(nonce_words[1], 8) << " "
                           << format_hex_no_prefix(nonce_words[2], 8)
                           << " block="
                           << format_hex_no_prefix(block[0], 2) << " "
                           << format_hex_no_prefix(block[1], 2) << " "
                           << format_hex_no_prefix(block[2], 2) << " "
                           << format_hex_no_prefix(block[3], 2) << "\n";
                }
                for (uint32_t addr = begin; addr < end; addr++) {
                    vm_code[addr] ^= chacha20ByteAt(vm_function_key, vm_code_seed,
                                                    chain_seed, bb_offset,
                                                    addr - begin);
                }
                if (debugEnabled && first_key != 0 && vm_code[begin] == first_before) {
                    errs() << "[VM_ENCRYPT_WARN] unchanged_first_byte begin=" << begin
                           << " end=" << end
                           << " first=0x" << Twine::utohexstr(first_before)
                           << " key=0x" << Twine::utohexstr(first_key)
                           << " seed=0x" << Twine::utohexstr(std::get<0>(p)) << "\n";
                }
            }
        }

        // pack one byte opcode
        std::vector<uint8_t> pack_op(uint8_t op){
            uint8_t ordinal = op;
            if (op > 0 && op <= OP_TOTAL) {
                ordinal = opcode_encode_map[op] ? opcode_encode_map[op] : op;
            }

            uint8_t res = 0;
            std::vector<uint8_t> his;
            const int MAX_RETRIES = 1000;
            int total_retries = 0;
            
            for (int i = 0; i < ordinal; i++) {
                if (total_retries >= MAX_RETRIES) {
                    res = (uint8_t)xorshift32(&xorshift32_state);
                    break;
                }
                
                uint8_t tmp = (uint8_t)xorshift32(&xorshift32_state);
                if (find(his.begin(), his.end(), tmp) == his.end()) {
                    his.push_back(tmp);
                    res = tmp;
                }
                else {
                    i--;
                    total_retries++;
                }
            }
            if (isIRObfuscationDebugEnabled()) {
                errs() << "[pack_op] op=" << (int)op
                       << " ordinal=" << (int)ordinal
                       << " res=" << (int)res
                       << " xorshift32_state=" << xorshift32_state << "\n";
            }
            return pack(res, 1);
        }

        // pack a Constant to a vector
        std::vector<uint8_t> pack_const_value(Value * const_value){
            std::vector<uint8_t> res;
            Type * type = const_value->getType();
            int type_size = modDataLayout->getTypeAllocSize(type);
            int64_t value = 0;
            bool handled = false;

            if (type_size > 8 &&
                (cast<Constant>(const_value)->isNullValue() ||
                 isa<UndefValue>(const_value) ||
                 isa<PoisonValue>(const_value))) {
                return std::vector<uint8_t>(type_size, 0);
            }

            if (type->isIntegerTy()){
                if (ConstantInt* CI = dyn_cast<ConstantInt>(const_value)) {
                    if (CI->getBitWidth() <= 64) {
                        // VM stores integer constants as raw low-order bytes.  Using
                        // sign-extension turns i1 true into 0xff, so `xor i1 %v, true`
                        // becomes `%v ^ 0xff` and any non-zero byte is treated as true
                        // by the branch handler.
                        value = static_cast<int64_t>(CI->getZExtValue());
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

            // Handle function pointer constants (like std::endl)
            // Function pointers are stored as addresses, we pack 0 as placeholder
            // The actual address will be resolved at runtime
            if (!handled && isa<Function>(const_value)) {
                // Function pointer - pack as 0, actual address resolved at runtime
                value = 0;
                handled = true;
            }

            // Handle GlobalValue pointers (functions, global variables)
            if (!handled && isa<GlobalValue>(const_value)) {
                // Global value pointer - pack as 0, actual address resolved at runtime
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
                // 对于常量值，直接打包常量值，设置 type_id = 1 表示常量
                packed = pack_const_value(value);
                packType[1] = 1;  // 标记为常量
            }
            else if(ConstantExpr* CE = dyn_cast<ConstantExpr>(value)){
                if (CE->getOpcode() == Instruction::GetElementPtr) {
                    if (GlobalVariable *GV = dyn_cast<GlobalVariable>(CE->getOperand(0))) {
                        // 将全局变量添加到 gv_value_map
                        if (gv_value_map.find(GV) == gv_value_map.end()) {
                            gv_value_map.insert({GV, curr_data_offset});
                            insert_to_value_map(value_map, GV, curr_data_offset);
                            int res_size = modDataLayout->getTypeAllocSize(GV->getValueType());
                            curr_data_offset += res_size;
                        }

                        // 计算 GEP 的偏移量
                        APInt offset(64, 0);
                        if (GEPOperator *GEP = dyn_cast<GEPOperator>(CE)) {
                            if (GEP->accumulateConstantOffset(*modDataLayout, offset)) {
                                // 成功计算偏移量
                                int gep_offset_val = offset.getSExtValue();

                                // 将 GEP 信息添加到 gep_info_map
                                int data_offset = curr_data_offset;
                                insert_to_value_map(value_map, value, curr_data_offset);
                                curr_data_offset += pointer_size;

                                GEPInfo info;
                                info.GV = GV;
                                info.gep_offset = gep_offset_val;
                                gep_info_map.insert({data_offset, info});

                                packed = pack(data_offset, pointer_size);
                            } else {
                                // 无法计算偏移量，使用全局变量的偏移量
                                packed = pack((*value_map)[GV], pointer_size);
                            }
                        } else {
                            packed = pack((*value_map)[GV], pointer_size);
                        }
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
            else if (BlockAddress *BA = dyn_cast<BlockAddress>(value)) {
                auto existing = value_map->find(BA);
                if (existing == value_map->end()) {
                    int data_offset = curr_data_offset;
                    insert_to_value_map(value_map, BA, data_offset);
                    curr_data_offset += pointer_size;
                    blockaddress_info_map.insert({data_offset, {BA}});
                    existing = value_map->find(BA);
                }

                packed = pack(existing->second, pointer_size);
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
                    // Handle Function* (like std::endl) - function pointer constants
                    else if (Function *func = dyn_cast<Function>(value)) {
                        // Function pointer - pack as 0 (placeholder)
                        // The actual function address will be resolved at runtime
                        packed = pack(0, pointer_size);
                        packType[1] = 0;
                        res.insert(res.end(), packType.begin(), packType.end());
                        res.insert(res.end(), packed.begin(), packed.end());
                        return res;
                    }
                    // Handle other GlobalValue types (like GlobalAlias)
                    else if (GlobalValue *gv = dyn_cast<GlobalValue>(value)) {
                        // Other global value - pack as 0 (placeholder)
                        packed = pack(0, pointer_size);
                        packType[1] = 0;
                        res.insert(res.end(), packType.begin(), packType.end());
                        res.insert(res.end(), packed.begin(), packed.end());
                        return res;
                    }
                    else {
                        if (isIRObfuscationDebugEnabled()) {
                            errs() << "[packValue] ERROR: Value not in map and not a GlobalVariable/Function/GlobalValue!\n";
                            errs() << "[packValue] Value: " << *value << "\n";
                            errs() << "[packValue] Type: " << *value->getType() << "\n";
                        }
                        // Instead of asserting, add it to value_map
                        insert_to_value_map(value_map, value, curr_data_offset);
                        int res_size = modDataLayout->getTypeAllocSize(value->getType());
                        curr_data_offset += res_size;
                    }
                }

                packed = pack((*value_map)[value], pointer_size);
                // variable，packtype->TypeID=0
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

static GlobalVariable *getOrCreateSharedGV(Module *M, Type *Ty, Constant *Init,
                                            StringRef Name,
                                            StringRef Section = ".AProtect.data");

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
    // 设置段名为 .AProtect.data
    gv_code_seg->setSection(".AProtect.data");
    


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
    // 设置段名为 .AProtect.bss
    gv_data_seg->setSection(".AProtect.bss");
    gv_data_seg->setAlignment(Align(16));


    ip = getOrCreateSharedGV(Mod, Type::getInt32Ty(Mod->getContext()),
                             ConstantInt::get(Type::getInt32Ty(Mod->getContext()), 0),
                             "vmp_shared_ip", ".AProtect.data");

    data_seg_addr = getOrCreateSharedGV(Mod, Type::getInt64Ty(Mod->getContext()),
                                        ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
                                        "vmp_shared_data_seg_addr", ".AProtect.data");

    code_seg_addr = getOrCreateSharedGV(Mod, Type::getInt64Ty(Mod->getContext()),
                                        ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
                                        "vmp_shared_code_seg_addr", ".AProtect.data");

    exception_thrown = getOrCreateSharedGV(Mod, Type::getInt8Ty(Mod->getContext()),
                                           ConstantInt::get(Type::getInt8Ty(Mod->getContext()), 0),
                                           "vmp_shared_exception_thrown", ".AProtect.data");

    exception_ptr_global = getOrCreateSharedGV(Mod, PointerType::get(Mod->getContext(), 0),
                                               ConstantPointerNull::get(PointerType::get(Mod->getContext(), 0)),
                                               "vmp_shared_exception_ptr", ".AProtect.data");

    exception_selector_global = getOrCreateSharedGV(Mod, Type::getInt32Ty(Mod->getContext()),
                                                    ConstantInt::get(Type::getInt32Ty(Mod->getContext()), 0),
                                                    "vmp_shared_exception_selector", ".AProtect.data");

    last_br_from_bb_id = getOrCreateSharedGV(Mod, Type::getInt64Ty(Mod->getContext()),
                                             ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
                                             "vmp_shared_last_br_from_bb_id", ".AProtect.data");

    current_bb_id = getOrCreateSharedGV(Mod, Type::getInt64Ty(Mod->getContext()),
                                        ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
                                        "vmp_shared_current_bb_id", ".AProtect.data");

    uint8_t debug_enabled = isIRObfuscationDebugEnabled() ? 1 : 0;
    vmp_debug_enabled_gv = getOrCreateSharedGV(Mod, Type::getInt8Ty(Mod->getContext()),
                                               ConstantInt::get(Type::getInt8Ty(Mod->getContext()), debug_enabled),
                                               "vmp_shared_debug_enabled", ".AProtect.data");
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

    // 设置段名为 .AProtect.text
    func->setSection(".AProtect.text");

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

    // Check if this is an InvokeInst
    // 完整支持异常处理
    bool isInvoke = isa<InvokeInst>(inst);
    Function *debugCallee = inst->getCalledFunction();

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[CALL_MAP] funcid=" << curr_func_id
               << " invoke=" << (isInvoke ? 1 : 0)
               << " indirect=" << (inst->isIndirectCall() ? 1 : 0);
        if (debugCallee) {
            errs() << " callee=" << debugCallee->getName();
        } else {
            errs() << " callee=<indirect>";
        }
        errs() << " ret_type=" << *inst->getType() << "\n";

        errs() << "[handle_callinst] Processing callinst #" << curr_func_id << "\n";
        if (isInvoke) {
            errs() << "[handle_callinst]   This is an InvokeInst (exception handling enabled)\n";
        }
        Function *callee = inst->getCalledFunction();
        if (callee) {
            errs() << "[handle_callinst]   Callee: " << callee->getName() << "\n";
        } else {
            errs() << "[handle_callinst]   Indirect call\n";
        }
    }
    
    if (!this->callinst_handler_conBBL) {
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[handle_callinst] ERROR: callinst_handler_conBBL is null!\n";
        }
        return;
    }
    
    IRBuilder<> IRBcon(this->callinst_handler_conBBL);
    Function *directCallee = inst->isIndirectCall() ? nullptr : inst->getCalledFunction();
    std::string tracedCallName;
    if (directCallee && directCallee->hasName()) {
        StringRef calleeName = directCallee->getName();
        if (calleeName == "_Z8httppostRKNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEES7_S7_") {
            tracedCallName = "httppost";
        } else if (calleeName == "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC2B8ne210000ILi0EEEPKc") {
            tracedCallName = "string_ctor_cstr";
        } else if (calleeName == "_Z33b263fe5d59982e095638f763b0155ae8eRKNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEES7_") {
            tracedCallName = "b263";
        } else if (calleeName == "_Z33h3489af11618a58be3cd5128cf0e932d0RKNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEES7_") {
            tracedCallName = "h3489";
        } else if (calleeName == "_Z33uaf2c82b988c77fae683d2c5c8331faeeRKNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE") {
            tracedCallName = "uaf2";
        } else if (calleeName == "_Z33rf89634bf35a0f3833c8c765bfa7a6e2eRKNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE") {
            tracedCallName = "rf";
        }
    }
    auto isAggregateLikeAddress = [&](Type *T) -> bool {
        if (!T) return false;
        if (T->isAggregateType()) return true;
        return T->isSized() && modDataLayout->getTypeAllocSize(T) > pointer_size;
    };
    auto shouldPassPointerArgByAddress = [&](Value *V) -> bool {
        if (!V) return false;
        V = V->stripPointerCasts();

        if (AllocaInst *AI = dyn_cast<AllocaInst>(V)) {
            return isAggregateLikeAddress(AI->getAllocatedType());
        }

        if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(V)) {
            Value *base = GEP->getPointerOperand()->stripPointerCasts();
            if (AllocaInst *AI = dyn_cast<AllocaInst>(base)) {
                return isAggregateLikeAddress(AI->getAllocatedType());
            }
            if (GlobalVariable *GV = dyn_cast<GlobalVariable>(base)) {
                return isAggregateLikeAddress(GV->getValueType());
            }
        }

        if (GlobalVariable *GV = dyn_cast<GlobalVariable>(V)) {
            return isAggregateLikeAddress(GV->getValueType());
        }

        return false;
    };
    auto resolveAllocaAreaOffset = [&](Value *V) -> int {
        if (!V) return -1;
        V = V->stripPointerCasts();

        if (AllocaInst *AI = dyn_cast<AllocaInst>(V)) {
            auto it = alloca_area_map.find(AI);
            if (it != alloca_area_map.end()) {
                return it->second;
            }
        }

        if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(V)) {
            Value *base = GEP->getPointerOperand()->stripPointerCasts();
            if (AllocaInst *AI = dyn_cast<AllocaInst>(base)) {
                auto it = alloca_area_map.find(AI);
                if (it != alloca_area_map.end()) {
                    APInt gepOffset(64, 0);
                    if (GEP->accumulateConstantOffset(*modDataLayout, gepOffset)) {
                        return it->second + static_cast<int>(gepOffset.getSExtValue());
                    }
                    return it->second;
                }
            }
        }

        return -1;
    };

    // firstly,  we need to unpack function args
    // errs() << "[handle_callinst] Unpacking args, arg_size=" << inst->arg_size() << "\n";
    std::vector<Value *> target_func_args;
    unsigned sret_result_offset = 0;
    uint64_t sret_result_size = 0;
    
    for (unsigned idx = 0; idx < inst->arg_size(); idx++){
        // errs() << "[handle_callinst] Processing arg " << idx << "\n";
        Value * currarg = inst->getArgOperand(idx);

        // if value is a constant, use it directly
        if(isa<Constant>(currarg)){
            // Handle ConstantExpr GEP (like stdout)
            if (ConstantExpr *CE = dyn_cast<ConstantExpr>(currarg)) {
                if (CE->getOpcode() == Instruction::GetElementPtr) {
                    target_func_args.push_back(currarg);
                    if (isIRObfuscationDebugEnabled()) {
                        errs() << "[handle_callinst][" << tracedCallName << "] const-gep-direct arg " << idx
                               << " value=" << *currarg << "\n";
                        errs() << "[handle_callinst] Arg " << idx << " is ConstantExpr GEP, passing directly\n";
                    }
                    continue;
                } else if (CE->getOpcode() == Instruction::BitCast) {
                    // For BitCast, get the underlying value
                    Value *underlyingValue = CE->getOperand(0);
                    if (isa<Function>(underlyingValue) || isa<GlobalValue>(underlyingValue)) {
                        // It's a function pointer or global value being bitcast
                        // Use the BitCast result directly - LLVM will handle the address
                        target_func_args.push_back(currarg);
                        if (isIRObfuscationDebugEnabled()) {
                            errs() << "[handle_callinst][" << tracedCallName << "] const-bitcast arg " << idx
                                   << " value=" << *currarg << "\n";
                        }
                        continue;
                    }
                }
            }
            // Handle Function* directly (like std::endl)
            if (isa<Function>(currarg)) {
                // It's a direct function pointer (like std::endl)
                target_func_args.push_back(currarg);
                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[handle_callinst][" << tracedCallName << "] function arg " << idx
                           << " value=" << *currarg << "\n";
                }
                continue;
            }
            if (GlobalVariable *GV = dyn_cast<GlobalVariable>(currarg)) {
                if (GV->getValueType()->isArrayTy()) {
                    Constant *Zero32 = ConstantInt::get(Type::getInt32Ty(Mod->getContext()), 0);
                    SmallVector<Constant *, 2> DecayIndices = {Zero32, Zero32};
                    Constant *DecayPtr = ConstantExpr::getGetElementPtr(
                        GV->getValueType(), GV, DecayIndices);
                    target_func_args.push_back(DecayPtr);
                    if (isIRObfuscationDebugEnabled()) {
                        errs() << "[handle_callinst][" << tracedCallName << "] global-array arg " << idx
                               << " decayed_from=" << *currarg << "\n";
                    }
                    continue;
                }
            }
            if (isIRObfuscationDebugEnabled()) {
                errs() << "[handle_callinst][" << tracedCallName << "] const arg " << idx
                       << " value=" << *currarg << "\n";
            }
            target_func_args.push_back(currarg);
            continue;
        }

        if (value_map.find(currarg) == value_map.end()) {
            if (isIRObfuscationDebugEnabled()) {
                errs() << "[handle_callinst][" << tracedCallName << "] arg " << idx
                       << " missing in value_map: " << *currarg << "\n";
            }
            // 检查是否是 GetElementPtrInst
            if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(currarg)) {
                // GetElementPtrInst：检查其操作数是否是全局变量
                Value *ptrOperand = GEP->getPointerOperand();
                if (GlobalVariable *GV = dyn_cast<GlobalVariable>(ptrOperand)) {
                    // 全局变量的 GEP：从 gv_value_map 中查找偏移量
                    if (gv_value_map.find(GV) != gv_value_map.end()) {
                        unsigned curroffset = gv_value_map[GV];
                        ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
                        Value * offset_value = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), curroffset);
                        Value * gepinst = IRBcon.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, offset_value}, "");
                        Value * ptr_addr = IRBcon.CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));
                        Value * actual_ptr = IRBcon.CreateLoad(PointerType::get(Mod->getContext(), 0), ptr_addr);
                        target_func_args.push_back(actual_ptr);
                        if (isIRObfuscationDebugEnabled()) {
                            errs() << "[handle_callinst] Arg " << idx << " is GetElementPtrInst of GlobalVariable, loading from data_seg[" << curroffset << "]\n";
                        }
                        continue;
                    }
                }
            }
            // 检查是否是全局变量
            if (GlobalVariable *GV = dyn_cast<GlobalVariable>(currarg)) {
                // 全局变量：从 gv_value_map 中查找偏移量
                if (gv_value_map.find(GV) != gv_value_map.end()) {
                    unsigned curroffset = gv_value_map[GV];
                    ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
                    Value * offset_value = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), curroffset);
                    Value * gepinst = IRBcon.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, offset_value}, "");
                    Value * ptr_addr = IRBcon.CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));
                    Value * actual_ptr = IRBcon.CreateLoad(PointerType::get(Mod->getContext(), 0), ptr_addr);
                    target_func_args.push_back(actual_ptr);
                    if (isIRObfuscationDebugEnabled()) {
                        errs() << "[handle_callinst] Arg " << idx << " is GlobalVariable, loading from data_seg[" << curroffset << "]\n";
                    }
                    continue;
                }
            }
            // errs() << "[VMP] ERROR: arg not found in value_map for call: " << *inst << "\n";
            target_func_args.push_back(UndefValue::get(currarg->getType()));
            continue;
        }
        // errs() << "[handle_callinst] Arg " << idx << " found in value_map\n";
        unsigned curroffset = value_map[currarg];
        if (inst->paramHasAttr(idx, Attribute::StructRet)) {
            if (Type *sretType = inst->getParamStructRetType(idx)) {
                if (sretType->isSized()) {
                    int sretAreaOffset = resolveAllocaAreaOffset(currarg);
                    sret_result_offset = sretAreaOffset >= 0 ? static_cast<unsigned>(sretAreaOffset) : curroffset;
                    sret_result_size = modDataLayout->getTypeAllocSize(sretType);
                    if (isIRObfuscationDebugEnabled()) {
                        errs() << "[handle_callinst] Detected sret arg " << idx
                               << " at data_seg[" << sret_result_offset
                               << "] size=" << sret_result_size << "\n";
                        errs() << "[handle_callinst][" << tracedCallName << "] sret arg " << idx
                               << " result_offset=" << sret_result_offset
                               << " result_size=" << sret_result_size
                               << " currarg=" << *currarg << "\n";
                    }
                }
            }
        }

        // construct load
        ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
        Value * offset_value = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), curroffset);
        Value * gepinst = IRBcon.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, offset_value}, "");

        // LLVM 中的引用参数最终也表现为指针实参，而 data_seg 为指针 SSA 值保存的是
        // “指针值本身”，不是对象内容。这里统一从 data_seg 槽位中取出实际指针，
        // 避免把槽位地址误当成聚合对象地址传给被调函数。
        if (currarg->getType()->isPointerTy()) {
            // 查找这个参数在 value_map 中对应的原始值
            // 通过遍历 value_map 找到对应的值
            Value *originalValue = nullptr;
            for (auto &pair : value_map) {
                if (pair.second == (int)curroffset) {
                    originalValue = pair.first;
                    break;
                }
            }

            bool aggregateLikePointer =
                shouldPassPointerArgByAddress(currarg) ||
                shouldPassPointerArgByAddress(originalValue);
            int aggregateAreaOffset = aggregateLikePointer ? resolveAllocaAreaOffset(currarg) : -1;
            if (aggregateAreaOffset < 0 && aggregateLikePointer) {
                aggregateAreaOffset = resolveAllocaAreaOffset(originalValue);
            }

            if (isIRObfuscationDebugEnabled()) {
                errs() << "[handle_callinst][" << tracedCallName << "] arg " << idx
                       << " offset=" << curroffset
                       << " currarg=" << *currarg;
                if (originalValue) {
                    errs() << " original=" << *originalValue;
                } else {
                    errs() << " original=<null>";
                }
                errs() << " aggregate_like=" << (aggregateLikePointer ? 1 : 0);
                if (aggregateAreaOffset >= 0) {
                    errs() << " aggregate_area_offset=" << aggregateAreaOffset;
                }
                errs() << "\n";
            }

            if (aggregateAreaOffset >= 0) {
                Value *areaOffsetValue = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), aggregateAreaOffset);
                Value *areaGep = IRBcon.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, areaOffsetValue}, "");
                Value *direct_obj_addr = IRBcon.CreatePointerCast(areaGep, PointerType::get(Mod->getContext(), 0));
                target_func_args.push_back(direct_obj_addr);
            } else {
                Value * ptr_addr = IRBcon.CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));
                Value * actual_ptr = IRBcon.CreateLoad(PointerType::get(Mod->getContext(), 0), ptr_addr);
                target_func_args.push_back(actual_ptr);
            }

            // #region debug-point callinst-pointer-arg
            if (isIRObfuscationDebugEnabled()) {
                errs() << "[handle_callinst] Saving pointer arg " << idx
                       << " from data_seg[" << curroffset << "]"
                       << " arg_type=" << *currarg->getType();
                if (aggregateLikePointer) {
                    errs() << " aggregate_like=1";
                }
                if (originalValue) {
                    errs() << " original_type=" << *originalValue->getType();
                }
                errs() << "\n";
            }
            // #endregion
        } else {
            if (isIRObfuscationDebugEnabled()) {
                errs() << "[handle_callinst][" << tracedCallName << "] non-pointer arg " << idx
                       << " offset=" << curroffset
                       << " currarg=" << *currarg << "\n";
            }
            // 非指针类型，从data_seg加载值
            Value * ptr = IRBcon.CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));
            Value * arg = IRBcon.CreateLoad(currarg->getType(), ptr);
            target_func_args.push_back(arg);

            if (isIRObfuscationDebugEnabled()) {
                errs() << "[handle_callinst] Saving arg " << idx << " from data_seg[" << curroffset << "]\n";
            }
        }
    }


    // errs() << "[handle_callinst] Args done, creating callFunction BB\n";
    // secondly, we create a new basic block to construct callinst
    BasicBlock *callFunction = BasicBlock::Create(Mod->getContext(), "callFunction_" + to_string(this->callinst_handler_curr_idx), this->callinst_handler);
    IRBuilder<> IRBcallFunction(callFunction);
    
    // DEBUG: 添加调试输出
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[handle_callinst] Created callFunction BB: " << callFunction->getName() << "\n";
        errs() << "[handle_callinst] callFunction parent: " << callFunction->getParent()->getName() << "\n";
        errs() << "[handle_callinst] IRBcallFunction.GetInsertBlock(): " << IRBcallFunction.GetInsertBlock()->getName() << "\n";
        errs() << "[handle_callinst] IRBcallFunction.GetInsertPoint(): ";
        if (IRBcallFunction.GetInsertPoint() == callFunction->end()) {
            errs() << "end() of BB\n";
        } else {
            errs() << &*IRBcallFunction.GetInsertPoint() << "\n";
        }
    }

    Function *memcpy_func = Intrinsic::getOrInsertDeclaration(Mod, Intrinsic::memcpy, {PointerType::get(Mod->getContext(), 0), PointerType::get(Mod->getContext(), 0), Type::getInt64Ty(Mod->getContext())});
    Function *malloc_func = Mod->getFunction("malloc");
    if (!malloc_func) {
        FunctionType *malloc_type = FunctionType::get(PointerType::get(Mod->getContext(), 0), {Type::getInt64Ty(Mod->getContext())}, false);
        malloc_func = Function::Create(malloc_type, Function::ExternalLinkage, "malloc", Mod);
    }
    Function *free_func = Mod->getFunction("free");
    if (!free_func) {
        FunctionType *free_type = FunctionType::get(Type::getVoidTy(Mod->getContext()), {PointerType::get(Mod->getContext(), 0)}, false);
        free_func = Function::Create(free_type, Function::ExternalLinkage, "free", Mod);
    }
    Function *printf_func = Mod->getFunction("printf");
    if (!printf_func) {
        FunctionType *printf_type = FunctionType::get(
            Type::getInt32Ty(Mod->getContext()),
            {PointerType::get(Mod->getContext(), 0)},
            true);
        printf_func = Function::Create(printf_type, Function::ExternalLinkage, "printf", Mod);
    }
    
    // 获取 data_seg 实际大小（从 gv_data_seg 的类型中获取）
    // gv_data_seg 是 [N x i8] 类型的数组
    Type *data_seg_type = gv_data_seg->getValueType();
    uint64_t data_seg_size = 5000;  // 默认值
    if (ArrayType *arr_type = dyn_cast<ArrayType>(data_seg_type)) {
        data_seg_size = arr_type->getNumElements();
    }
    
    // errs() << "[handle_callinst] Checking call type\n";
    Value *resultValue = nullptr;
    Value *saved_data_seg = nullptr;
    Value *saved_sret_result = nullptr;
    Value *alloc_size = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), data_seg_size);
    bool should_restore_caller_data_seg = false;
    bool isInvokeCall = isa<InvokeInst>(inst);
    IRBuilder<> *resultBuilder = &IRBcallFunction;
    std::unique_ptr<IRBuilder<>> invokeNormalBuilder;

    auto clearExceptionState = [&](IRBuilder<> &B) {
        B.CreateStore(ConstantInt::get(Type::getInt8Ty(Mod->getContext()), 0), exception_thrown);
        B.CreateStore(ConstantPointerNull::get(PointerType::get(Mod->getContext(), 0)), exception_ptr_global);
        B.CreateStore(ConstantInt::get(Type::getInt32Ty(Mod->getContext()), 0), exception_selector_global);
    };

    auto restoreCallerSnapshot = [&](IRBuilder<> &B) {
        if (!should_restore_caller_data_seg || !saved_data_seg) {
            return;
        }
        Value *src_restore = B.CreatePointerCast(saved_data_seg, PointerType::get(Mod->getContext(), 0));
        Value *dest_restore = B.CreatePointerCast(gv_data_seg, PointerType::get(Mod->getContext(), 0));
        B.CreateCall(memcpy_func, {dest_restore, src_restore, alloc_size,
                                   ConstantInt::get(Type::getInt1Ty(Mod->getContext()), 0)});
    };

    auto freeCallerSnapshot = [&](IRBuilder<> &B) {
        if (should_restore_caller_data_seg && saved_data_seg) {
            B.CreateCall(free_func, {saved_data_seg});
        }
    };

    auto copyOriginalCallABI = [&](CallBase *newCall) {
        if (!newCall) {
            return;
        }
        newCall->setCallingConv(inst->getCallingConv());
        newCall->setAttributes(inst->getAttributes());
        newCall->setDebugLoc(inst->getDebugLoc());
        if (auto *newCI = dyn_cast<CallInst>(newCall)) {
            if (auto *oldCI = dyn_cast<CallInst>(inst)) {
                newCI->setTailCallKind(oldCI->getTailCallKind());
            }
        }
    };

    auto emitCallLike = [&](FunctionType *funcType, Value *calleeValue) -> Value * {
        if (!isInvokeCall) {
            CallInst *newCall = IRBcallFunction.CreateCall(funcType, calleeValue, ArrayRef<Value *>(target_func_args));
            copyOriginalCallABI(newCall);
            return newCall;
        }

        InvokeInst *originalInvoke = cast<InvokeInst>(inst);
        if (!this->callinst_handler->hasPersonalityFn() && F->hasPersonalityFn()) {
            this->callinst_handler->setPersonalityFn(cast<Constant>(F->getPersonalityFn()));
        }

        BasicBlock *normalBB = BasicBlock::Create(
            Mod->getContext(),
            "invokeNormal_" + to_string(this->callinst_handler_curr_idx),
            this->callinst_handler);
        BasicBlock *unwindBB = BasicBlock::Create(
            Mod->getContext(),
            "invokeUnwind_" + to_string(this->callinst_handler_curr_idx),
            this->callinst_handler);

        InvokeInst *newInvoke = IRBcallFunction.CreateInvoke(
            funcType,
            calleeValue,
            normalBB,
            unwindBB,
            ArrayRef<Value *>(target_func_args));
        copyOriginalCallABI(newInvoke);

        invokeNormalBuilder = std::make_unique<IRBuilder<>>(normalBB);
        resultBuilder = invokeNormalBuilder.get();
        clearExceptionState(*resultBuilder);

        IRBuilder<> IRBunwind(unwindBB);
        LandingPadInst *sourceLandingPad = nullptr;
        for (Instruction &UI : *originalInvoke->getUnwindDest()) {
            if (auto *LP = dyn_cast<LandingPadInst>(&UI)) {
                sourceLandingPad = LP;
                break;
            }
        }

        StructType *landingPadTy = StructType::get(
            PointerType::get(Mod->getContext(), 0),
            Type::getInt32Ty(Mod->getContext()));
        unsigned clauseCount = sourceLandingPad ? sourceLandingPad->getNumClauses() : 0;
        LandingPadInst *newLandingPad = IRBunwind.CreateLandingPad(landingPadTy, clauseCount);
        if (sourceLandingPad) {
            newLandingPad->setCleanup(sourceLandingPad->isCleanup());
            for (unsigned i = 0; i < sourceLandingPad->getNumClauses(); ++i) {
                newLandingPad->addClause(sourceLandingPad->getClause(i));
            }
        } else {
            newLandingPad->setCleanup(true);
        }

        Value *excObj = IRBunwind.CreateExtractValue(newLandingPad, 0);
        Value *excSel = IRBunwind.CreateExtractValue(newLandingPad, 1);

        restoreCallerSnapshot(IRBunwind);
        freeCallerSnapshot(IRBunwind);

        IRBunwind.CreateStore(ConstantInt::get(Type::getInt8Ty(Mod->getContext()), 1), exception_thrown);
        IRBunwind.CreateStore(excObj, exception_ptr_global);
        IRBunwind.CreateStore(excSel, exception_selector_global);
        IRBunwind.CreateRetVoid();
        return newInvoke;
    };

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
            // Force disable inlining for the callee if -irobf-vmp-noinline is enabled
            if (isForceNoInlineEnabled() && callee->hasName()) {
                callee->addFnAttr(Attribute::NoInline);
                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[handle_callinst] Added NoInline attribute to: " << callee->getName() << "\n";
                }
            }
        }
    }

    if (!treatAsIndirect && callee) {
        // 对于普通函数调用，需要判断是标准库函数还是用户函数
        // GOVMModifier会将原函数重命名为"原函数名_original"并创建wrapper
        
        // 只把真正的外部/运行库调用当成“当前 VM 内存写入”的调用。
        // 之前使用 substring 匹配会把用户函数 `power` 错误识别成 `pow`。
        bool is_stdlib = callee->isDeclaration() || callee->isIntrinsic();
        if (callee->hasName()) {
            std::string calleeName = callee->getName().str();
            if (isKnownStdlibLikeFunctionName(calleeName)) {
                is_stdlib = true;
            }
        }

            // 只有真正会进入另一套 VM 上下文的 VMP callee，才需要恢复 caller data_seg。
            // 普通本地 helper（如析构/构造/通过指针修改 caller 内存的函数）必须保留写入结果。
            bool calleeUsesVmWrapper = !is_stdlib &&
                hasFunctionAnnotationKeyword(callee, "vmp") &&
                !hasFunctionAnnotationKeyword(callee, "novmp");
            should_restore_caller_data_seg = calleeUsesVmWrapper;

            if (should_restore_caller_data_seg) {
                saved_data_seg = IRBcallFunction.CreateCall(malloc_func, {alloc_size});
                Value *dest_save = IRBcallFunction.CreatePointerCast(saved_data_seg, PointerType::get(Mod->getContext(), 0));
                Value *src_save = IRBcallFunction.CreatePointerCast(gv_data_seg, PointerType::get(Mod->getContext(), 0));
                IRBcallFunction.CreateCall(memcpy_func, {dest_save, src_save, alloc_size, ConstantInt::get(Type::getInt1Ty(Mod->getContext()), 0)});
            } else if (isIRObfuscationDebugEnabled()) {
                errs() << "[handle_callinst] Keeping current data_seg writes for callee: "
                       << callee->getName() << "\n";
            }
            
            if (is_stdlib) {
                // 标准库函数，直接调用；对 InvokeInst 需要保留 unwind 语义
                if (isIRObfuscationDebugEnabled() &&
                    tracedCallName == "string_ctor_cstr" && target_func_args.size() >= 2) {
                    Value *objPtr = IRBcallFunction.CreatePointerCast(target_func_args[0], PointerType::get(Mod->getContext(), 0));
                    Value *cstrPtr = IRBcallFunction.CreatePointerCast(target_func_args[1], PointerType::get(Mod->getContext(), 0));
                    Value *firstChar = IRBcallFunction.CreateLoad(Type::getInt8Ty(Mod->getContext()), cstrPtr);
                    Value *firstCharInt = IRBcallFunction.CreateSExt(firstChar, Type::getInt32Ty(Mod->getContext()));
                    Value *preFmt = IRBcallFunction.CreateGlobalString("[VMP_CTOR_PRE] this=%p cstr=%p first=%d\n");
                    IRBcallFunction.CreateCall(printf_func, {preFmt, objPtr, cstrPtr, firstCharInt});
                }
                resultValue = emitCallLike(callee->getFunctionType(), callee);
                if (isIRObfuscationDebugEnabled() &&
                    tracedCallName == "string_ctor_cstr" && target_func_args.size() >= 1) {
                    Value *objPtr = resultBuilder->CreatePointerCast(target_func_args[0], PointerType::get(Mod->getContext(), 0));
                    Value *word0 = resultBuilder->CreateLoad(Type::getInt64Ty(Mod->getContext()), objPtr);
                    Value *objPtr8 = resultBuilder->CreateConstGEP1_64(Type::getInt8Ty(Mod->getContext()), objPtr, 8);
                    Value *word1 = resultBuilder->CreateLoad(Type::getInt64Ty(Mod->getContext()), objPtr8);
                    Value *objPtr16 = resultBuilder->CreateConstGEP1_64(Type::getInt8Ty(Mod->getContext()), objPtr, 16);
                    Value *word2 = resultBuilder->CreateLoad(Type::getInt64Ty(Mod->getContext()), objPtr16);
                    Value *postFmt = resultBuilder->CreateGlobalString("[VMP_CTOR_POST] this=%p w0=%llx w1=%llx w2=%llx\n");
                    resultBuilder->CreateCall(printf_func, {postFmt, objPtr, word0, word1, word2});
                }
            } else {
                // 用户函数，调用wrapper函数（原函数名）
                // wrapper函数会设置VM环境并调用vm_interpreter
                resultValue = emitCallLike(callee->getFunctionType(), callee);
            }
    }
    else {
        // indirect call (or direct call with null callee treated as indirect)
        should_restore_caller_data_seg = true;
        saved_data_seg = IRBcallFunction.CreateCall(malloc_func, {alloc_size});
        Value *dest_save = IRBcallFunction.CreatePointerCast(saved_data_seg, PointerType::get(Mod->getContext(), 0));
        Value *src_save = IRBcallFunction.CreatePointerCast(gv_data_seg, PointerType::get(Mod->getContext(), 0));
        IRBcallFunction.CreateCall(memcpy_func, {dest_save, src_save, alloc_size, ConstantInt::get(Type::getInt1Ty(Mod->getContext()), 0)});

        if (!calledValue) {
            calledValue = inst->getCalledOperand();
        }
        if (value_map.find(calledValue) == value_map.end()) {
            // errs() << "[VMP] WARNING: called_value not found in value_map for indirect call: " << *inst << "\n";
            // fallback: use a direct call approach by calling the function directly
            FunctionType *funcType = inst->getFunctionType();
            Value *funcPtr = IRBcallFunction.CreatePointerCast(calledValue, PointerType::get(Mod->getContext(), 0));
            resultValue = emitCallLike(funcType, funcPtr);
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
            resultValue = emitCallLike(funcType, funcPtr);
        }
    }

    // Store result and create return
    // 问题：data_seg[0..callee_return_size-1] 存储被调用函数的返回值
    // 恢复 data_seg 时，应该跳过这个空间，保留被调用函数的返回值

    // 1. 保存返回值到临时变量
    // 重要：返回值在 resultValue 中（寄存器），不在 data_seg[0] 中
    Value *saved_result = nullptr;
    if (inst->getType() != Type::getVoidTy(this->Mod->getContext())) {
        // 使用 wrapper 函数的返回值（resultValue），而不是从 data_seg[0] 读取
        saved_result = resultValue;

        // DEBUG: 添加调试输出
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[handle_callinst] saved_result from function return (not from data_seg[0])\n";
            if (saved_result) {
                errs() << "[handle_callinst] saved_result type: " << *saved_result->getType() << "\n";
            }
        }
    }

    // 2. 对于 sret（如按值返回 std::string）先保存返回对象，再恢复调用者 data_seg。
    if (should_restore_caller_data_seg && sret_result_size > 0) {
        Value *sret_alloc_size = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), sret_result_size);
        saved_sret_result = resultBuilder->CreateCall(malloc_func, {sret_alloc_size});

        ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
        Value *sret_offset_value = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), sret_result_offset);
        Value *sret_gep = resultBuilder->CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, sret_offset_value}, "");
        Value *sret_src = resultBuilder->CreatePointerCast(sret_gep, PointerType::get(Mod->getContext(), 0));
        Value *sret_dst = resultBuilder->CreatePointerCast(saved_sret_result, PointerType::get(Mod->getContext(), 0));
        resultBuilder->CreateCall(memcpy_func, {sret_dst, sret_src, sret_alloc_size, ConstantInt::get(Type::getInt1Ty(Mod->getContext()), 0)});
    }

    // 3. 完整恢复调用者 data_seg。
    // 返回值已经保存在 saved_result 中，恢复整个快照不会丢失返回值，
    // 但可以避免前缀区域（通常正好是参数/局部变量）被嵌套调用污染。
    if (should_restore_caller_data_seg) {
        Value *src_restore = resultBuilder->CreatePointerCast(saved_data_seg, PointerType::get(Mod->getContext(), 0));
        Value *dest_restore = resultBuilder->CreatePointerCast(gv_data_seg, PointerType::get(Mod->getContext(), 0));
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[handle_callinst] Restoring full data_seg[0.." << data_seg_size << ")\n";
        }
        resultBuilder->CreateCall(memcpy_func, {dest_restore, src_restore, alloc_size, ConstantInt::get(Type::getInt1Ty(Mod->getContext()), 0)});
    }
    
    // 4. 把 sret 返回对象写回恢复后的 caller data_seg。
    if (should_restore_caller_data_seg && saved_sret_result != nullptr && sret_result_size > 0) {
        ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
        Value *sret_offset_value = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), sret_result_offset);
        Value *sret_gep = resultBuilder->CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, sret_offset_value}, "");
        Value *sret_dst = resultBuilder->CreatePointerCast(sret_gep, PointerType::get(Mod->getContext(), 0));
        Value *sret_src = resultBuilder->CreatePointerCast(saved_sret_result, PointerType::get(Mod->getContext(), 0));
        Value *sret_alloc_size = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), sret_result_size);
        resultBuilder->CreateCall(memcpy_func, {sret_dst, sret_src, sret_alloc_size, ConstantInt::get(Type::getInt1Ty(Mod->getContext()), 0)});
    }

    // 5. 释放堆内存
    if (should_restore_caller_data_seg) {
        resultBuilder->CreateCall(free_func, {saved_data_seg});
    }
    if (saved_sret_result != nullptr) {
        resultBuilder->CreateCall(free_func, {saved_sret_result});
    }
    
    // 6. 存储返回值到正确位置
    if (saved_result != nullptr) {
        if (value_map.find(inst) == value_map.end()) {
            // errs() << "[VMP] ERROR: call result not found in value_map: " << *inst << "\n";
        } else {
            unsigned result_value_offset = value_map[inst];

            // DEBUG: 添加调试输出
            if (isIRObfuscationDebugEnabled()) {
                errs() << "[handle_callinst] Storing result to data_seg[" << result_value_offset << "]\n";
            }

            // 存储到 data_seg[result_offset]
            ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
            Value * offset_value = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), result_value_offset);
            Value * gepinst = resultBuilder->CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, offset_value}, "");
            Value * ptr = resultBuilder->CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));
            resultBuilder->CreateStore(saved_result, ptr);
        }
    }
    
    // Create Return
    resultBuilder->CreateRetVoid();
    

    // compare and jmp
    BasicBlock *falseconBBL = BasicBlock::Create(Mod->getContext(), "falseconBBL", this->callinst_handler);

    Value *currfunc_id = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), curr_func_id);
    Value *condition = IRBcon.CreateICmpEQ(this->targetfunc_id, currfunc_id);
    IRBcon.CreateCondBr(condition, callFunction, falseconBBL);
    this->callinst_handler_conBBL = falseconBBL;

}


bool GOVMTranslator::is_supported_instruction(Instruction *ins) {
    if (CallBase *CB = dyn_cast<CallBase>(ins)) {
        if (Function *Callee = CB->getCalledFunction()) {
            if (Callee->isIntrinsic()) {
                Intrinsic::ID IID = Callee->getIntrinsicID();
                if (IID == Intrinsic::lifetime_start ||
                    IID == Intrinsic::lifetime_end ||
                    IID == Intrinsic::dbg_declare ||
                    IID == Intrinsic::dbg_value ||
                    IID == Intrinsic::dbg_assign) {
                    return true;
                }
            }
        }
    }

    if (UnaryOperator *UO = dyn_cast<UnaryOperator>(ins)) {
        return UO->getOpcode() == Instruction::FNeg;
    }

    return isa<AllocaInst>(ins) || isa<LoadInst>(ins) || isa<StoreInst>(ins) ||
           ins->isBinaryOp() || isa<CmpInst>(ins) || isa<GetElementPtrInst>(ins) ||
           isa<CastInst>(ins) || isa<BranchInst>(ins) || isa<ReturnInst>(ins) ||
           isa<InvokeInst>(ins) || isa<CallBase>(ins) || isa<PHINode>(ins) ||
           isa<SelectInst>(ins) || isa<SwitchInst>(ins) || isa<ExtractValueInst>(ins) ||
           isa<UnreachableInst>(ins) || isa<LandingPadInst>(ins) || isa<ResumeInst>(ins) ||
           isa<InsertValueInst>(ins) || isa<IndirectBrInst>(ins) ||
           isa<ExtractElementInst>(ins) || isa<InsertElementInst>(ins) ||
           isa<ShuffleVectorInst>(ins) || isa<FreezeInst>(ins) ||
           isa<CatchSwitchInst>(ins) || isa<CatchReturnInst>(ins) ||
           isa<CleanupReturnInst>(ins) || isa<CallBrInst>(ins) || isa<FenceInst>(ins) ||
           isa<AtomicCmpXchgInst>(ins) || isa<AtomicRMWInst>(ins) || isa<VAArgInst>(ins);
}

bool GOVMTranslator::prescan_supported_ir() {
    const int MAX_BASIC_BLOCKS = 10000;
    const int MAX_INSTRUCTIONS = 100000;
    int bb_count = 0;
    int total_instructions = 0;

    for (BasicBlock &BB : *F) {
        if (bb_count >= MAX_BASIC_BLOCKS) {
            errs() << "[VMP Warning] Function '" << F->getName()
                   << "' exceeds VMP basic block limit.\n";
            return false;
        }
        bb_count++;

        for (Instruction &I : BB) {
            if (total_instructions >= MAX_INSTRUCTIONS) {
                errs() << "[VMP Warning] Function '" << F->getName()
                       << "' exceeds VMP instruction limit.\n";
                return false;
            }
            total_instructions++;

            if (!is_supported_instruction(&I)) {
                errs() << "[VMP Warning] Unsupported instruction in function '" << F->getName() << "':\n";
                errs() << "[VMP Warning]   " << I << "\n";
                errs() << "[VMP Warning]   Instruction type: " << I.getOpcodeName() << "\n";
                errs() << "[VMP Warning] Skipping VMP protection for this function.\n";
                return false;
            }

            for (unsigned idx = 0; idx < I.getNumOperands(); idx++) {
                if (ConstantExpr *Op = dyn_cast<ConstantExpr>(I.getOperand(idx))) {
                    Instruction *ConstInst = Op->getAsInstruction();
                    bool Supported = is_supported_instruction(ConstInst);
                    if (!Supported) {
                        errs() << "[VMP Warning] Unsupported constant expression lowering in function '"
                               << F->getName() << "':\n";
                        errs() << "[VMP Warning]   " << *ConstInst << "\n";
                        errs() << "[VMP Warning]   Instruction type: " << ConstInst->getOpcodeName() << "\n";
                        errs() << "[VMP Warning] Skipping VMP protection for this function.\n";
                    }
                    ConstInst->deleteValue();
                    if (!Supported) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void GOVMTranslator::rollback_created_ir() {
    for (auto it = lowered_constexprs.rbegin(); it != lowered_constexprs.rend(); ++it) {
        Instruction *UserInst = std::get<0>(*it);
        unsigned OperandIndex = std::get<1>(*it);
        ConstantExpr *OriginalExpr = std::get<2>(*it);
        Instruction *LoweredInst = std::get<3>(*it);
        if (UserInst && UserInst->getParent() && OperandIndex < UserInst->getNumOperands()) {
            UserInst->setOperand(OperandIndex, OriginalExpr);
        }
        if (LoweredInst && LoweredInst->getParent()) {
            LoweredInst->eraseFromParent();
        }
    }
    lowered_constexprs.clear();
    if (callinst_handler) {
        callinst_handler->eraseFromParent();
        callinst_handler = nullptr;
    }
    if (gv_code_seg) {
        gv_code_seg->eraseFromParent();
        gv_code_seg = nullptr;
    }
    if (gv_data_seg) {
        gv_data_seg->eraseFromParent();
        gv_data_seg = nullptr;
    }
}

void GOVMTranslator::handle_inst(Instruction *ins) {
    if (CallBase *CB = dyn_cast<CallBase>(ins)) {
        if (Function *Callee = CB->getCalledFunction()) {
            if (Callee->isIntrinsic()) {
                Intrinsic::ID IID = Callee->getIntrinsicID();
                if (IID == Intrinsic::lifetime_start ||
                    IID == Intrinsic::lifetime_end ||
                    IID == Intrinsic::dbg_declare ||
                    IID == Intrinsic::dbg_value ||
                    IID == Intrinsic::dbg_assign) {
                    return;
                }
            }
        }
    }

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
        alloca_area_map[inst] = area_offset;
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

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[CMP] pred=" << predicate_val 
                   << " op0_type=" << *inst->getOperand(0)->getType()
                   << " op1_type=" << *inst->getOperand(1)->getType()
                   << " res_offset=" << (curr_data_offset - res_size)
                   << "\n";
        }
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
                    // Use proper struct layout to get correct offset with alignment
                    const StructLayout *SL = modDataLayout->getStructLayout(st);
                    int curr_element_offset = SL->getElementOffset(element_idx);
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

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[BR] source_bb_offset=" << source_bb_offset 
                   << " for BB " << inst->getParent()->getName().str()
                   << " current code_pos=" << vm_code.size() << "\n";
        }

        if (inst->isUnconditional()) {
            hex_code.push_back(0);
            hex_code.insert(hex_code.end(), packed_source_bb.begin(), packed_source_bb.end());
            br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+hex_code.size(), inst->getSuccessor(0)));
            if (isIRObfuscationDebugEnabled()) {
                errs() << "[BR] Unconditional to BB " << inst->getSuccessor(0)->getName().str() << "\n";
                errs() << "[BR] packed_source_bb bytes: ";
                for (auto b : packed_source_bb) errs() << (int)b << " ";
                errs() << "\n";
            }
            hex_code.insert(hex_code.end(), padding.begin(), padding.end());
        } else {
            hex_code.push_back(1);
            hex_code.insert(hex_code.end(), packed_source_bb.begin(), packed_source_bb.end());
            std::vector<uint8_t> pack_condition = packValue(inst->getCondition(), &value_map);
            hex_code.insert(hex_code.end(), pack_condition.begin(), pack_condition.end());

            if (isIRObfuscationDebugEnabled()) {
                errs() << "[BR] Conditional: true->BB " << inst->getSuccessor(0)->getName().str()
                       << " false->BB " << inst->getSuccessor(1)->getName().str() << "\n";
                errs() << "[BR] packed_source_bb bytes: ";
                for (auto b : packed_source_bb) errs() << (int)b << " ";
                errs() << "\n";
            }
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
        // 完整支持异常处理：生成Call_OP + 条件分支
        long long curr_func_id = this->callinst_handler_curr_idx++;
        std::vector<uint8_t> packed_funcid = pack(curr_func_id, pointer_size);

        std::vector<uint8_t> packed_res;
        if (inst->getType() != Type::getVoidTy(this->Mod->getContext())) {
            insert_to_value_map(&value_map, inst, curr_data_offset);
            int res_size = modDataLayout->getTypeAllocSize(inst->getType());
            curr_data_offset += res_size;
            packed_res = GET_PACK_VALUE(inst);
        } else {
            // void return - generate a placeholder packed_res (type_size=0, type_id=0, offset=0)
            packed_res = {0, 0}; // type_size=0, type_id=VoidTyID
            std::vector<uint8_t> zero_offset = pack(0, pointer_size);
            packed_res.insert(packed_res.end(), zero_offset.begin(), zero_offset.end());
        }

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(Call_OP), packed_funcid, packed_res);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        callinst_map.insert(std::pair<CallBase *, long long>(cast<CallBase>(inst), curr_func_id));

        // 检查异常标志，决定跳转到normal还是unwind目标
        std::vector<uint8_t> br_hex;
        br_hex = pack_op(BR_OP);
        br_hex.push_back(2); // invoke-exception branch

        int current_bb_offset = vm_code.size();
        std::vector<uint8_t> packed_src_bb = pack(current_bb_offset, pointer_size);
        br_hex.insert(br_hex.end(), packed_src_bb.begin(), packed_src_bb.end());

        // true_target: unwind目标（异常处理）
        std::vector<uint8_t> padding = pack(0, pointer_size);
        br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+br_hex.size(), inst->getUnwindDest()));
        br_hex.insert(br_hex.end(), padding.begin(), padding.end());

        // false_target: normal目标（正常返回）
        br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+br_hex.size(), inst->getNormalDest()));
        br_hex.insert(br_hex.end(), padding.begin(), padding.end());

        vm_code.insert(vm_code.end(), br_hex.begin(), br_hex.end());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[INVOKE] Generated Call_OP + conditional BR for " << inst->getName() << "\n";
            errs() << "[INVOKE]   normal dest: " << inst->getNormalDest()->getName() << "\n";
            errs() << "[INVOKE]   unwind dest: " << inst->getUnwindDest()->getName() << "\n";
        }
    }

    else if(CallBase * inst = dyn_cast<CallBase>(ins)) {

        if (isIRObfuscationDebugEnabled()) {
            Function *callee = inst->getCalledFunction();
            if (callee) {
                errs() << "[Translator] Handling CallBase to: " << callee->getName() << "\n";
            } else {
                errs() << "[Translator] Handling CallBase (indirect call)\n";
            }
        }

        // 遍历参数，处理 ConstantExpr GEP 和 GetElementPtrInst (like stdout)
        for (unsigned idx = 0; idx < inst->arg_size(); idx++) {
            Value *currarg = inst->getArgOperand(idx);
            if (isIRObfuscationDebugEnabled()) {
                errs() << "[Translator]   Arg " << idx << ": " << *currarg << "\n";
                errs() << "[Translator]     Type: " << *currarg->getType() << "\n";
                errs() << "[Translator]     IsConstant: " << isa<Constant>(currarg) << "\n";
                errs() << "[Translator]     IsConstantExpr: " << isa<ConstantExpr>(currarg) << "\n";
                errs() << "[Translator]     IsGetElementPtrInst: " << isa<GetElementPtrInst>(currarg) << "\n";
            }

            // 处理 ConstantExpr GEP
            if (ConstantExpr *CE = dyn_cast<ConstantExpr>(currarg)) {
                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[Translator]     Opcode: " << CE->getOpcodeName() << "\n";
                }
                if (CE->getOpcode() == Instruction::GetElementPtr) {
                    if (isIRObfuscationDebugEnabled()) {
                        errs() << "[Translator]   Found ConstantExpr GEP in arg " << idx << ", calling packValue...\n";
                    }
                    // 调用 packValue 来填充 gep_value_map
                    std::vector<uint8_t> packed = packValue(currarg, &value_map);
                }
            }
            // 处理 GetElementPtrInst (like stdout)
            else if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(currarg)) {
                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[Translator]     Found GetElementPtrInst in arg " << idx << "\n";
                }
                Value *ptrOperand = GEP->getPointerOperand();
                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[Translator]       Pointer operand: " << *ptrOperand << "\n";
                }
                if (GlobalVariable *GV = dyn_cast<GlobalVariable>(ptrOperand)) {
                    if (isIRObfuscationDebugEnabled()) {
                        errs() << "[Translator]       Pointer is GlobalVariable: " << GV->getName() << "\n";
                    }
                    // 将全局变量添加到 gv_value_map
                    if (gv_value_map.find(GV) == gv_value_map.end()) {
                        gv_value_map.insert({GV, curr_data_offset});
                        insert_to_value_map(&value_map, GV, curr_data_offset);
                        int res_size = modDataLayout->getTypeAllocSize(GV->getValueType());
                        curr_data_offset += res_size;
                        if (isIRObfuscationDebugEnabled()) {
                            errs() << "[Translator]       Added GV to gv_value_map: offset=" << curr_data_offset - res_size << "\n";
                        }
                    }

                    // 计算 GEP 的偏移量
                    APInt gep_offset(64, 0);
                    if (GEP->accumulateConstantOffset(*modDataLayout, gep_offset)) {
                        int offset_val = gep_offset.getSExtValue();
                        if (isIRObfuscationDebugEnabled()) {
                            errs() << "[Translator]       GEP offset: " << offset_val << "\n";
                        }

                        // 将 GEP 信息添加到 gep_info_map
                        int data_offset = curr_data_offset;
                        insert_to_value_map(&value_map, currarg, curr_data_offset);
                        curr_data_offset += pointer_size;

                        GEPInfo info;
                        info.GV = GV;
                        info.gep_offset = offset_val;
                        gep_info_map.insert({data_offset, info});

                        if (isIRObfuscationDebugEnabled()) {
                            errs() << "[Translator]       Added GEP to gep_info_map: data_offset=" << data_offset
                                   << ", gep_offset=" << offset_val << "\n";
                        }
                    }
                }
            }
        }

        // current function id
        long long curr_func_id = this->callinst_handler_curr_idx ++;

        std::vector<uint8_t> packed_funcid = pack(curr_func_id, pointer_size);

        // check if this callsite return a void
        std::vector<uint8_t> packed_res;
        if (inst->getType() != Type::getVoidTy(this->Mod->getContext())) {
            // return a value
            Type *retType = inst->getType();
            int res_size = modDataLayout->getTypeAllocSize(retType);

            if (isIRObfuscationDebugEnabled()) {
                errs() << "[Translator]   Return type: " << *retType << "\n";
                errs() << "[Translator]   Return type size: " << res_size << " bytes\n";
                errs() << "[Translator]   Is struct: " << retType->isStructTy() << "\n";
                if (retType->isStructTy()) {
                    StructType *ST = cast<StructType>(retType);
                    errs() << "[Translator]   Struct name: " << (ST->hasName() ? ST->getName() : "<anonymous>") << "\n";
                    errs() << "[Translator]   Num elements: " << ST->getNumElements() << "\n";
                    for (unsigned i = 0; i < ST->getNumElements(); i++) {
                        errs() << "[Translator]     Element " << i << ": " << *ST->getElementType(i)
                               << " (size=" << modDataLayout->getTypeAllocSize(ST->getElementType(i)) << ")\n";
                    }
                }
            }

            insert_to_value_map(&value_map, inst, curr_data_offset);
            curr_data_offset += res_size;

            packed_res = GET_PACK_VALUE(inst);
        } else {
            // void return - generate a placeholder packed_res (type_size=0, type_id=0, offset=0)
            packed_res = {0, 0}; // type_size=0, type_id=VoidTyID
            std::vector<uint8_t> zero_offset = pack(0, pointer_size);
            packed_res.insert(packed_res.end(), zero_offset.begin(), zero_offset.end());
        }

        // construct hex code
        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(Call_OP), packed_funcid, packed_res);

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

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[PHI] num_incoming=" << num_incoming << " at code_pos=" << vm_code.size() << "\n";
        }

        for (unsigned i = 0; i < num_incoming; i++) {
            BasicBlock *incoming_bb = inst->getIncomingBlock(i);
            Value *incoming_val = inst->getIncomingValue(i);
            int bb_id = basicblock_map[incoming_bb];
            std::vector<uint8_t> packed_bb_id = pack(bb_id, pointer_size);
            
            if (isIRObfuscationDebugEnabled()) {
                errs() << "[PHI] incoming[" << i << "] bb_id=" << bb_id 
                       << " incoming_bb=" << incoming_bb->getName().str()
                       << " basicblock_map lookup result=" << bb_id << "\n";
            }
            
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
                const StructLayout *SL = modDataLayout->getStructLayout(ST);
                offset += SL->getElementOffset(idx);
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

    // C++ 异常处理指令 - 完整实现RTTI类型匹配
    // landingpad: 异常处理入口，返回异常对象和类型
    else if(LandingPadInst * inst = dyn_cast<LandingPadInst>(ins)) {
        // 分配结果空间（异常对象和类型选择器）
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);

        // 提取所有catch和filter clauses
        unsigned num_clauses = inst->getNumClauses();
        std::vector<uint8_t> packed_num_clauses = pack(num_clauses, 4);

        std::vector<uint8_t> hex_code;
        hex_code = pack_op(LANDINGPAD_OP);
        hex_code.insert(hex_code.end(), packed_res.begin(), packed_res.end());
        hex_code.insert(hex_code.end(), packed_num_clauses.begin(), packed_num_clauses.end());

        // 存储每个clause的类型信息
        for (unsigned i = 0; i < num_clauses; i++) {
            Constant *clause = inst->getClause(i);
            
            // 判断是catch还是filter
            bool is_catch = inst->isCatch(i);
            hex_code.push_back(is_catch ? 1 : 0);  // 1=catch, 0=filter

            if (is_catch) {
                // Catch clause: 存储类型信息
                if (GlobalVariable *GV = dyn_cast<GlobalVariable>(clause)) {
                    // 存储类型信息的地址
                    uint64_t type_info_addr = (uint64_t)(uintptr_t)GV;
                    std::vector<uint8_t> packed_type_info = pack(type_info_addr, pointer_size);
                    hex_code.insert(hex_code.end(), packed_type_info.begin(), packed_type_info.end());
                } else if (ConstantPointerNull *CPN = dyn_cast<ConstantPointerNull>(clause)) {
                    // catch (...) - null表示捕获所有异常
                    std::vector<uint8_t> zero_type = pack(0, pointer_size);
                    hex_code.insert(hex_code.end(), zero_type.begin(), zero_type.end());
                } else {
                    // 其他常量，存储其值
                    uint64_t type_info_val = 0;
                    if (ConstantInt *CI = dyn_cast<ConstantInt>(clause)) {
                        type_info_val = CI->getZExtValue();
                    }
                    std::vector<uint8_t> packed_type_info = pack(type_info_val, pointer_size);
                    hex_code.insert(hex_code.end(), packed_type_info.begin(), packed_type_info.end());
                }
            } else {
                // Filter clause: 存储类型数组
                if (ConstantArray *CA = dyn_cast<ConstantArray>(clause)) {
                    // Filter包含多个类型
                    unsigned num_types = CA->getNumOperands();
                    std::vector<uint8_t> packed_num_types = pack(num_types, 4);
                    hex_code.insert(hex_code.end(), packed_num_types.begin(), packed_num_types.end());

                    for (unsigned j = 0; j < num_types; j++) {
                        Constant *type_const = CA->getOperand(j);
                        uint64_t type_info_addr = 0;
                        if (GlobalVariable *GV = dyn_cast<GlobalVariable>(type_const)) {
                            type_info_addr = (uint64_t)(uintptr_t)GV;
                        }
                        std::vector<uint8_t> packed_type = pack(type_info_addr, pointer_size);
                        hex_code.insert(hex_code.end(), packed_type.begin(), packed_type.end());
                    }
                } else {
                    // 单个类型filter
                    std::vector<uint8_t> packed_num_types = pack(1, 4);
                    hex_code.insert(hex_code.end(), packed_num_types.begin(), packed_num_types.end());

                    uint64_t type_info_addr = 0;
                    if (GlobalVariable *GV = dyn_cast<GlobalVariable>(clause)) {
                        type_info_addr = (uint64_t)(uintptr_t)GV;
                    }
                    std::vector<uint8_t> packed_type = pack(type_info_addr, pointer_size);
                    hex_code.insert(hex_code.end(), packed_type.begin(), packed_type.end());
                }
            }
        }

        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[LANDINGPAD] Generated LANDINGPAD_OP with " << num_clauses << " clauses at code_pos=" 
                   << (vm_code.size() - hex_code.size()) << "\n";
            for (unsigned i = 0; i < num_clauses; i++) {
                errs() << "[LANDINGPAD]   Clause " << i << ": " 
                       << (inst->isCatch(i) ? "catch" : "filter") << "\n";
            }
        }
    }

    // resume: 恢复异常传播
    else if(ResumeInst * inst = dyn_cast<ResumeInst>(ins)) {
        // 获取异常对象
        std::vector<uint8_t> packed_exc = GET_PACK_VALUE(inst->getValue());

        // 生成RESUME_OP指令
        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(RESUME_OP), packed_exc);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[RESUME] Generated RESUME_OP at code_pos=" << (vm_code.size() - hex_code.size()) << "\n";
        }
    }

    // insertvalue: 插入值到聚合类型
    else if(InsertValueInst *inst = dyn_cast<InsertValueInst>(ins)) {
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        unsigned offset = 0;
        Type *currentType = inst->getAggregateOperand()->getType();
        for (unsigned idx : inst->indices()) {
            if (StructType *ST = dyn_cast<StructType>(currentType)) {
                const StructLayout *SL = modDataLayout->getStructLayout(ST);
                offset += SL->getElementOffset(idx);
                currentType = ST->getElementType(idx);
            } else if (ArrayType *AT = dyn_cast<ArrayType>(currentType)) {
                offset += idx * modDataLayout->getTypeAllocSize(AT->getElementType());
                currentType = AT->getElementType();
            }
        }

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        std::vector<uint8_t> packed_agg = GET_PACK_VALUE(inst->getAggregateOperand());
        std::vector<uint8_t> packed_val = GET_PACK_VALUE(inst->getInsertedValueOperand());
        std::vector<uint8_t> packed_offset = pack(offset, pointer_size);
        packed_offset.insert(packed_offset.begin(), 1);
        packed_offset.insert(packed_offset.begin(), pointer_size);
        std::vector<uint8_t> packed_size =
            pack((uint32_t)modDataLayout->getTypeAllocSize(inst->getInsertedValueOperand()->getType()), 4);

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(INSERTVALUE_OP), packed_res, packed_agg, packed_val, packed_offset, packed_size);
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[INSERTVALUE_TRANSLATOR] " << *inst << "\n";
            errs() << "[INSERTVALUE_DETAIL] res_size=" << res_size
                   << " indices=" << inst->getNumIndices()
                   << " offset=" << offset
                   << " agg_type=" << *inst->getAggregateOperand()->getType()
                   << " inserted_type=" << *inst->getInsertedValueOperand()->getType()
                   << " packed_res=" << packed_res.size()
                   << " packed_agg=" << packed_agg.size()
                   << " packed_val=" << packed_val.size()
                   << " packed_offset=" << packed_offset.size()
                   << " packed_size=" << packed_size.size()
                   << " total_bytes=" << hex_code.size() << "\n";
            errs() << "[INSERTVALUE_DETAIL] agg_is_const=" << isa<Constant>(inst->getAggregateOperand())
                   << " agg_is_constexpr=" << isa<ConstantExpr>(inst->getAggregateOperand())
                   << " agg_is_aggregate_zero=" << isa<ConstantAggregateZero>(inst->getAggregateOperand())
                   << " agg_is_poison=" << isa<PoisonValue>(inst->getAggregateOperand())
                   << " agg_is_undef=" << isa<UndefValue>(inst->getAggregateOperand())
                   << " inserted_is_const=" << isa<Constant>(inst->getInsertedValueOperand())
                   << " inserted_is_constexpr=" << isa<ConstantExpr>(inst->getInsertedValueOperand())
                   << "\n";
            errs() << "[INSERTVALUE_DETAIL] agg_operand=" << *inst->getAggregateOperand() << "\n";
            errs() << "[INSERTVALUE_DETAIL] inserted_operand=" << *inst->getInsertedValueOperand() << "\n";
        }
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    // extractvalue: 从聚合类型提取值
    else if(ExtractValueInst *inst = dyn_cast<ExtractValueInst>(ins)) {
        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        unsigned offset = 0;
        Type *currentType = inst->getAggregateOperand()->getType();
        for (unsigned idx : inst->indices()) {
            if (StructType *ST = dyn_cast<StructType>(currentType)) {
                const StructLayout *SL = modDataLayout->getStructLayout(ST);
                offset += SL->getElementOffset(idx);
                currentType = ST->getElementType(idx);
            } else if (ArrayType *AT = dyn_cast<ArrayType>(currentType)) {
                offset += idx * modDataLayout->getTypeAllocSize(AT->getElementType());
                currentType = AT->getElementType();
            }
        }
        
        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);
        std::vector<uint8_t> packed_agg = GET_PACK_VALUE(inst->getAggregateOperand());
        std::vector<uint8_t> packed_offset = pack(offset, pointer_size);
        packed_offset.insert(packed_offset.begin(), 1);
        packed_offset.insert(packed_offset.begin(), pointer_size);
        std::vector<uint8_t> packed_size = pack((uint32_t)res_size, 4);
        
        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(EXTRACTVALUE_OP), packed_res, packed_agg, packed_offset, packed_size);
        // #region debug-point translator-extractvalue
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[EXTRACTVALUE_TRANSLATOR] res_size=" << res_size
                   << " indices=" << inst->getNumIndices()
                   << " offset=" << offset
                   << " agg_type=" << *inst->getAggregateOperand()->getType()
                   << " packed_res=" << packed_res.size()
                   << " packed_agg=" << packed_agg.size()
                   << " packed_offset=" << packed_offset.size()
                   << " packed_size=" << packed_size.size()
                   << " total_bytes=" << hex_code.size() << "\n";
        }
        // #endregion
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());
    }

    else if(IndirectBrInst * inst = dyn_cast<IndirectBrInst>(ins)) {
        std::vector<uint8_t> hex_code;
        hex_code = pack_op(INDIRECTBR_OP);

        std::vector<uint8_t> packed_addr = GET_PACK_VALUE(inst->getAddress());
        hex_code.insert(hex_code.end(), packed_addr.begin(), packed_addr.end());

        std::vector<uint8_t> packed_num_targets = pack((uint32_t)inst->getNumSuccessors(), 4);
        hex_code.insert(hex_code.end(), packed_num_targets.begin(), packed_num_targets.end());

        std::vector<uint8_t> padding = pack(0, pointer_size);
        for (unsigned idx = 0; idx < inst->getNumSuccessors(); ++idx) {
            BasicBlock *targetBB = inst->getSuccessor(idx);
            std::vector<uint8_t> packed_expected_addr =
                packValue(BlockAddress::get(F, targetBB), &value_map);
            hex_code.insert(hex_code.end(), packed_expected_addr.begin(), packed_expected_addr.end());
            br_map.push_back(pair<int, BasicBlock *>(vm_code.size() + hex_code.size(), targetBB));
            hex_code.insert(hex_code.end(), padding.begin(), padding.end());
        }

        // #region debug-point translator-indirectbr
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[INDIRECTBR_TRANSLATOR] addr_type=" << *inst->getAddress()->getType()
                   << " packed_addr=" << packed_addr.size()
                   << " num_successors=" << inst->getNumSuccessors()
                   << "\n";
        }
        // #endregion

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

        unsigned mask_num_elements = 0;
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

    // CatchSwitchInst: 异常处理分发 - 完整实现异常类型匹配
    else if(CatchSwitchInst * inst = dyn_cast<CatchSwitchInst>(ins)) {
        std::vector<uint8_t> hex_code;
        hex_code = pack_op(CATCHSWITCH_OP);

        // 获取handler数量
        unsigned num_handlers = inst->getNumHandlers();
        std::vector<uint8_t> packed_num_handlers = pack(num_handlers, 4);
        hex_code.insert(hex_code.end(), packed_num_handlers.begin(), packed_num_handlers.end());

        // unwind目标（如果没有，填0表示退出）
        std::vector<uint8_t> padding = pack(0, pointer_size);
        if (inst->hasUnwindDest()) {
            br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+hex_code.size(), inst->getUnwindDest()));
        }
        hex_code.insert(hex_code.end(), padding.begin(), padding.end());

        // 遍历每个handler，提取类型信息
        for (auto it = inst->handler_begin(); it != inst->handler_end(); ++it) {
            BasicBlock *handler_bb = *it;
            
            // 查找handler块中的CatchPadInst
            CatchPadInst *catchPad = nullptr;
            for (Instruction &I : *handler_bb) {
                if (CatchPadInst *CPI = dyn_cast<CatchPadInst>(&I)) {
                    catchPad = CPI;
                    break;
                }
            }

            // 存储handler目标地址（稍后回填）
            br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+hex_code.size(), handler_bb));
            hex_code.insert(hex_code.end(), padding.begin(), padding.end());

            // 存储异常类型信息
            if (catchPad && catchPad->arg_size() > 0) {
                // CatchPadInst的参数：第一个参数通常是异常类型信息
                Value *typeInfo = catchPad->getArgOperand(0);
                
                if (Constant *constTypeInfo = dyn_cast<Constant>(typeInfo)) {
                    // 如果是常量（类型信息），存储其地址
                    if (GlobalVariable *GV = dyn_cast<GlobalVariable>(constTypeInfo)) {
                        // 存储全局变量的地址（作为类型ID）
                        uint64_t type_id = (uint64_t)(uintptr_t)GV;
                        std::vector<uint8_t> packed_type_id = pack(type_id, pointer_size);
                        hex_code.insert(hex_code.end(), packed_type_id.begin(), packed_type_id.end());
                    } else {
                        // 其他常量，存储0表示catch-all
                        std::vector<uint8_t> zero_type = pack(0, pointer_size);
                        hex_code.insert(hex_code.end(), zero_type.begin(), zero_type.end());
                    }
                } else {
                    // 非常量类型信息，存储0表示catch-all
                    std::vector<uint8_t> zero_type = pack(0, pointer_size);
                    hex_code.insert(hex_code.end(), zero_type.begin(), zero_type.end());
                }
            } else {
                // 没有类型信息，表示catch-all (...)
                std::vector<uint8_t> zero_type = pack(0, pointer_size);
                hex_code.insert(hex_code.end(), zero_type.begin(), zero_type.end());
            }
        }

        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[CATCHSWITCH] Generated CATCHSWITCH_OP with " << num_handlers << " handlers\n";
            if (inst->hasUnwindDest()) {
                errs() << "[CATCHSWITCH]   Unwind dest: " << inst->getUnwindDest()->getName() << "\n";
            }
            for (auto it = inst->handler_begin(); it != inst->handler_end(); ++it) {
                errs() << "[CATCHSWITCH]   Handler: " << (*it)->getName() << "\n";
            }
        }
    }

    // CatchReturnInst: 异常处理返回
    else if(CatchReturnInst * inst = dyn_cast<CatchReturnInst>(ins)) {
        // 跳转到catchret的目标
        std::vector<uint8_t> hex_code;
        hex_code = pack_op(BR_OP);
        hex_code.push_back(0); // unconditional

        int current_bb_offset = vm_code.size();
        std::vector<uint8_t> packed_src_bb = pack(current_bb_offset, pointer_size);
        hex_code.insert(hex_code.end(), packed_src_bb.begin(), packed_src_bb.end());

        std::vector<uint8_t> padding = pack(0, pointer_size);
        br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+hex_code.size(), inst->getSuccessor()));
        hex_code.insert(hex_code.end(), padding.begin(), padding.end());

        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[CATCHRETURN] Generated unconditional BR to " << inst->getSuccessor()->getName() << "\n";
        }
    }

    // CleanupReturnInst: 清理返回
    else if(CleanupReturnInst * inst = dyn_cast<CleanupReturnInst>(ins)) {
        // 跳转到cleanupret的目标或unwind目标
        std::vector<uint8_t> hex_code;
        hex_code = pack_op(BR_OP);
        hex_code.push_back(0); // unconditional

        int current_bb_offset = vm_code.size();
        std::vector<uint8_t> packed_src_bb = pack(current_bb_offset, pointer_size);
        hex_code.insert(hex_code.end(), packed_src_bb.begin(), packed_src_bb.end());

        std::vector<uint8_t> padding = pack(0, pointer_size);
        if (inst->hasUnwindDest()) {
            br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+hex_code.size(), inst->getUnwindDest()));
        } else {
            // 没有unwind目标，跳转到退出
            std::vector<uint8_t> zero_target = pack(0, pointer_size);
            hex_code.insert(hex_code.end(), zero_target.begin(), zero_target.end());
        }
        hex_code.insert(hex_code.end(), padding.begin(), padding.end());

        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[CLEANUPRETURN] Generated unconditional BR\n";
        }
    }

    // Android/Itanium EH 中，catchpad/cleanuppad 主要承载 funclet 元数据。
    // 当前 VM 的异常分发实际依赖 landingpad/catchswitch/catchret/cleanupret，
    // 因此这里将 pad 指令视为已消费的结构化元数据，避免整函数因 unsupported 回退。
    else if (isa<CatchPadInst>(ins)) {
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[CATCHPAD] Treat as metadata-only funclet pad in VM for: "
                   << F->getName() << "\n";
        }
    }
    else if (isa<CleanupPadInst>(ins)) {
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[CLEANUPPAD] Treat as metadata-only funclet pad in VM for: "
                   << F->getName() << "\n";
        }
    }

    // ==================== 新增指令支持 ====================

    // CallBrInst: CallBr 指令 - 类似 Invoke，但用于间接分支
    else if(CallBrInst * inst = dyn_cast<CallBrInst>(ins)) {
        // CallBr 类似于 Invoke，但有间接分支目标
        // 处理方式：生成 Call_OP + 间接跳转

        long long curr_func_id = this->callinst_handler_curr_idx++;
        std::vector<uint8_t> packed_funcid = pack(curr_func_id, pointer_size);

        std::vector<uint8_t> packed_res;
        if (inst->getType() != Type::getVoidTy(this->Mod->getContext())) {
            insert_to_value_map(&value_map, inst, curr_data_offset);
            int res_size = modDataLayout->getTypeAllocSize(inst->getType());
            curr_data_offset += res_size;
            packed_res = GET_PACK_VALUE(inst);
        } else {
            packed_res = {0, 0};
            std::vector<uint8_t> zero_offset = pack(0, pointer_size);
            packed_res.insert(packed_res.end(), zero_offset.begin(), zero_offset.end());
        }

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(CALLBR_OP), packed_funcid, packed_res);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        callinst_map.insert(std::pair<CallBase *, long long>(cast<CallBase>(inst), curr_func_id));

        // 生成间接跳转到 default 目标
        std::vector<uint8_t> br_hex;
        br_hex = pack_op(BR_OP);
        br_hex.push_back(0); // unconditional

        int current_bb_offset = vm_code.size();
        std::vector<uint8_t> packed_src_bb = pack(current_bb_offset, pointer_size);
        br_hex.insert(br_hex.end(), packed_src_bb.begin(), packed_src_bb.end());

        std::vector<uint8_t> padding = pack(0, pointer_size);
        br_map.push_back(pair<int, BasicBlock *>(vm_code.size()+br_hex.size(), inst->getDefaultDest()));
        br_hex.insert(br_hex.end(), padding.begin(), padding.end());

        vm_code.insert(vm_code.end(), br_hex.begin(), br_hex.end());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[CALLBR] Generated CALLBR_OP for " << inst->getName() << "\n";
            errs() << "[CALLBR]   Default dest: " << inst->getDefaultDest()->getName() << "\n";
            errs() << "[CALLBR]   Num indirect dests: " << inst->getNumIndirectDests() << "\n";
        }
    }

    // FenceInst: 内存屏障指令
    else if(FenceInst * inst = dyn_cast<FenceInst>(ins)) {
        // Fence 指令确保内存操作的顺序性
        // 序列化操作，生成 FENCE_OP

        // 获取内存序
        AtomicOrdering ordering = inst->getOrdering();
        uint8_t ordering_val = 0;
        switch (ordering) {
            case AtomicOrdering::NotAtomic: ordering_val = 0; break;
            case AtomicOrdering::Unordered: ordering_val = 1; break;
            case AtomicOrdering::Monotonic: ordering_val = 2; break;
            case AtomicOrdering::Acquire: ordering_val = 3; break;
            case AtomicOrdering::Release: ordering_val = 4; break;
            case AtomicOrdering::AcquireRelease: ordering_val = 5; break;
            case AtomicOrdering::SequentiallyConsistent: ordering_val = 6; break;
        }

        std::vector<uint8_t> packed_ordering = {ordering_val};

        // 同步作用域
        SyncScope::ID scope = inst->getSyncScopeID();
        std::vector<uint8_t> packed_scope = pack(static_cast<uint32_t>(scope), 4);

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(FENCE_OP), packed_ordering, packed_scope);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[FENCE] Generated FENCE_OP with ordering=" << (int)ordering_val
                   << " scope=" << scope << "\n";
        }
    }

    // AtomicCmpXchgInst: 原子比较交换指令
    else if(AtomicCmpXchgInst * inst = dyn_cast<AtomicCmpXchgInst>(ins)) {
        // 原子比较并交换操作
        // 返回值是一个结构体：{原始值, 比较结果}

        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);

        // 指针操作数
        std::vector<uint8_t> packed_ptr = GET_PACK_VALUE(inst->getPointerOperand());

        // 比较值
        std::vector<uint8_t> packed_cmp = GET_PACK_VALUE(inst->getCompareOperand());

        // 新值
        std::vector<uint8_t> packed_new = GET_PACK_VALUE(inst->getNewValOperand());

        // 内存序
        AtomicOrdering success_ordering = inst->getSuccessOrdering();
        AtomicOrdering failure_ordering = inst->getFailureOrdering();
        uint8_t success_ordering_val = 0;
        uint8_t failure_ordering_val = 0;

        switch (success_ordering) {
            case AtomicOrdering::Monotonic: success_ordering_val = 2; break;
            case AtomicOrdering::Acquire: success_ordering_val = 3; break;
            case AtomicOrdering::Release: success_ordering_val = 4; break;
            case AtomicOrdering::AcquireRelease: success_ordering_val = 5; break;
            case AtomicOrdering::SequentiallyConsistent: success_ordering_val = 6; break;
            default: success_ordering_val = 6;
        }

        switch (failure_ordering) {
            case AtomicOrdering::Monotonic: failure_ordering_val = 2; break;
            case AtomicOrdering::Acquire: failure_ordering_val = 3; break;
            case AtomicOrdering::SequentiallyConsistent: failure_ordering_val = 6; break;
            default: failure_ordering_val = 6;
        }

        std::vector<uint8_t> packed_ordering = {success_ordering_val, failure_ordering_val};

        // 值类型大小
        int val_size = modDataLayout->getTypeAllocSize(inst->getCompareOperand()->getType());
        std::vector<uint8_t> packed_val_size = pack(val_size, 4);

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(ATOMIC_CMPXCHG_OP), packed_res, packed_ptr, packed_cmp, packed_new, packed_ordering, packed_val_size);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[ATOMIC_CMPXCHG] Generated ATOMIC_CMPXCHG_OP at code_pos=" << (vm_code.size() - hex_code.size())
                   << " val_size=" << val_size << "\n";
        }
    }

    // AtomicRMWInst: 原子读-修改-写指令
    else if(AtomicRMWInst * inst = dyn_cast<AtomicRMWInst>(ins)) {
        // 原子读-修改-写操作

        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);

        // 操作类型
        AtomicRMWInst::BinOp op = inst->getOperation();
        uint8_t op_val = 0;
        switch (op) {
            case AtomicRMWInst::Xchg: op_val = 0; break;
            case AtomicRMWInst::Add: op_val = 1; break;
            case AtomicRMWInst::Sub: op_val = 2; break;
            case AtomicRMWInst::And: op_val = 3; break;
            case AtomicRMWInst::Nand: op_val = 4; break;
            case AtomicRMWInst::Or: op_val = 5; break;
            case AtomicRMWInst::Xor: op_val = 6; break;
            case AtomicRMWInst::Max: op_val = 7; break;
            case AtomicRMWInst::Min: op_val = 8; break;
            case AtomicRMWInst::UMax: op_val = 9; break;
            case AtomicRMWInst::UMin: op_val = 10; break;
            case AtomicRMWInst::FAdd: op_val = 11; break;
            case AtomicRMWInst::FSub: op_val = 12; break;
            case AtomicRMWInst::FMax: op_val = 13; break;
            case AtomicRMWInst::FMin: op_val = 14; break;
            case AtomicRMWInst::FMaximum: op_val = 15; break;
            case AtomicRMWInst::FMinimum: op_val = 16; break;
            case AtomicRMWInst::UIncWrap: op_val = 17; break;
            case AtomicRMWInst::UDecWrap: op_val = 18; break;
            case AtomicRMWInst::USubCond: op_val = 19; break;
            case AtomicRMWInst::USubSat: op_val = 20; break;
            default:
                // Handle any future operations
                op_val = 0; break;
        }

        std::vector<uint8_t> packed_op = {op_val};

        // 指针操作数
        std::vector<uint8_t> packed_ptr = GET_PACK_VALUE(inst->getPointerOperand());

        // 值操作数
        std::vector<uint8_t> packed_val = GET_PACK_VALUE(inst->getValOperand());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[ATOMIC_RMW] packed_val size=" << packed_val.size() << "\n";
            errs() << "[ATOMIC_RMW] val operand: " << *inst->getValOperand() << "\n";
        }

        // 内存序
        AtomicOrdering ordering = inst->getOrdering();
        uint8_t ordering_val = 0;
        switch (ordering) {
            case AtomicOrdering::Monotonic: ordering_val = 2; break;
            case AtomicOrdering::Acquire: ordering_val = 3; break;
            case AtomicOrdering::Release: ordering_val = 4; break;
            case AtomicOrdering::AcquireRelease: ordering_val = 5; break;
            case AtomicOrdering::SequentiallyConsistent: ordering_val = 6; break;
            default: ordering_val = 6;
        }

        std::vector<uint8_t> packed_ordering = {ordering_val};

        // 值类型大小
        int val_size = modDataLayout->getTypeAllocSize(inst->getValOperand()->getType());
        std::vector<uint8_t> packed_val_size = pack(val_size, 4);

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(ATOMIC_RMW_OP), packed_op, packed_res, packed_ptr, packed_val, packed_ordering, packed_val_size);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[ATOMIC_RMW] Generated ATOMIC_RMW_OP with op=" << (int)op_val
                   << " val_size=" << val_size << "\n";
        }
    }

    // VAArgInst: 可变参数指令
    else if(VAArgInst * inst = dyn_cast<VAArgInst>(ins)) {
        // 访问可变参数函数的参数

        insert_to_value_map(&value_map, inst, curr_data_offset);
        int res_size = modDataLayout->getTypeAllocSize(inst->getType());
        curr_data_offset += res_size;

        std::vector<uint8_t> packed_res = GET_PACK_VALUE(inst);

        // va_list 指针
        std::vector<uint8_t> packed_va_list = GET_PACK_VALUE(inst->getPointerOperand());

        // 参数类型大小
        int arg_size = modDataLayout->getTypeAllocSize(inst->getType());
        std::vector<uint8_t> packed_arg_size = pack(arg_size, 4);

        // 参数类型信息
        Type *argType = inst->getType();
        std::vector<uint8_t> packed_type = type_to_hex(argType);

        std::vector<uint8_t> hex_code;
        ins_to_hex(hex_code, pack_op(VAARG_OP), packed_res, packed_va_list, packed_arg_size, packed_type);
        vm_code.insert(vm_code.end(), hex_code.begin(), hex_code.end());

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[VAARG] Generated VAARG_OP at code_pos=" << (vm_code.size() - hex_code.size())
                   << " arg_size=" << arg_size << "\n";
        }
    }

    else{
        // 遇到不支持的指令类型
        has_unsupported_instruction = true;

        // 打印警告信息
        errs() << "[VMP Warning] Unsupported instruction in function '" << F->getName() << "':\n";
        errs() << "[VMP Warning]   " << *ins << "\n";
        errs() << "[VMP Warning]   Instruction type: " << ins->getOpcodeName() << "\n";
        errs() << "[VMP Warning] Skipping VMP protection for this function.\n";
    }
}


// Translator
bool GOVMTranslator::run(){
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[Translator] Starting for function: " << F->getName() << "\n";
    }

    if (!prescan_supported_ir()) {
        return false;
    }
    init();
    
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
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[Translator] Function has exception handling\n";
        }
    }
    
    curr_data_offset = 0;

    // if return not void, alloca a memory at offset 0
    if (!F->getReturnType()->isVoidTy()) {
        // 返回值存储在偏移量 0
        // 注意：不需要插入到 value_map，因为返回值没有对应的 Value*
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
            has_unsupported_instruction = true;
            rollback_created_ir();
            return false;
        }

        BasicBlock * bb = &*bbl;
        basicblock_map.insert(pair<BasicBlock *, int>(bb, vm_code.size()));

        uint32_t bb_offset = (uint32_t)vm_code.size();
        uint64_t bb_token = deriveBBToken(vm_function_key, bb_offset);
        uint32_t bb_tag = deriveBBTag(vm_function_key, bb_offset);
        uint64_t opcode_seed = deriveOpcodeSeed(bb_token);
        uint64_t vm_code_seed = deriveVMSeed(vm_function_key, bb_token);
        uint64_t chain_seed = deriveChainSeed(vm_function_key, bb_token, bb_offset);
        xorshift32_seed = opcode_seed;
        xorshift32_state = opcode_seed;
        std::vector<uint8_t> bb_header = pack(bb_tag, sizeof(uint32_t));
        vm_code.insert(vm_code.end(), bb_header.begin(), bb_header.end());
        uint32_t currbb_begin = (uint32_t)vm_code.size();

        std::vector<Instruction *> instructions_to_process;
        for(auto ins = bbl->begin(); ins != bbl->end(); ins++){
            instructions_to_process.push_back(&*ins);
        }
        
        // errs() << "[Translator] BB " << bb_count << " has " << instructions_to_process.size() << " instructions\n";
        
        for(Instruction *inst : instructions_to_process){

            if (total_instructions >= MAX_INSTRUCTIONS) {
                has_unsupported_instruction = true;
                rollback_created_ir();
                return false;
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
                lowered_constexprs.emplace_back(inst, idx, Op, const_inst);

                handle_inst(const_inst);
            }
            
            if (inst && inst->getParent() == bb) {
                // errs() << "[Translator] Handling inst #" << total_instructions << ": " << inst->getOpcodeName() << "\n";
                handle_inst(inst);
            }
            total_instructions++;
        }
        
        if (total_instructions >= MAX_INSTRUCTIONS) {
            has_unsupported_instruction = true;
            rollback_created_ir();
            return false;
        }

        uint32_t currbb_end = vm_code.size();
        if (isIRObfuscationDebugEnabled() && bb_offset == 0) {
            errs() << "[VM_BB0_PLAINTEXT] begin=" << currbb_begin
                   << " end=" << currbb_end << " bytes=";
            uint32_t dump_end = std::min<uint32_t>(currbb_begin + 16, currbb_end);
            for (uint32_t idx = currbb_begin; idx < dump_end; ++idx) {
                errs() << format_hex_no_prefix(vm_code[idx], 2);
                if (idx + 1 != dump_end) errs() << " ";
            }
            errs() << "\n";
        }
        vm_code_seed_map.emplace_back(vm_code_seed, chain_seed, currbb_begin, currbb_end);
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

    // 检查是否遇到不支持的指令
    if (has_unsupported_instruction) {
        errs() << "[VMP Warning] Function '" << F->getName() << "' contains unsupported instructions.\n";
        errs() << "[VMP Warning] VMP protection has been skipped for this function.\n";
        rollback_created_ir();
        return false;  // 返回 false 表示跳过虚拟化
    }

    lowered_constexprs.clear();
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
        GOVMModifier(Function * F, std::map<GlobalVariable *, int> *gv_value_map,
                     std::map<int, GEPInfo> *gep_info_map, std::map<int, BlockAddressInfo> *blockaddress_info_map,
                     std::map<Value *, int> *value_map,
                     GlobalVariable *gv_data_seg, GlobalVariable *gv_code_seg,
                     GlobalVariable *ip, GlobalVariable *data_seg_addr,
                     GlobalVariable *code_seg_addr,
                     GlobalVariable *dispatch_code_seg_addr,
                     GlobalVariable *exception_thrown_gv,
                     GlobalVariable *exception_ptr_gv,
                     GlobalVariable *exception_selector_gv,
                     GlobalVariable *vm_block_chain_state_gv,
                     GlobalVariable *expected_bb_token_gv,
                     GlobalVariable *vm_function_key_gv,
                     GlobalVariable *opcode_xorshift32_state_gv,
                     GlobalVariable *vm_code_state_gv,
                     uint64_t vm_function_key,
                     Function *govm_interpreter,
                     Function *resume_unwind_helper,
                     int data_seg_size) {
            this->Mod = F->getParent();
            this->F = F;
            this->modDataLayout = const_cast<DataLayout *>(&this->Mod->getDataLayout());
            this->gv_value_map = gv_value_map;
            this->gep_info_map = gep_info_map;
            this->blockaddress_info_map = blockaddress_info_map;
            this->value_map = value_map;
            this->gv_data_seg = gv_data_seg;
            this->gv_code_seg = gv_code_seg;
            this->ip = ip;
            this->data_seg_addr = data_seg_addr;
            this->code_seg_addr = code_seg_addr;
            this->dispatch_code_seg_addr = dispatch_code_seg_addr;
            this->exception_thrown_gv = exception_thrown_gv;
            this->exception_ptr_gv = exception_ptr_gv;
            this->exception_selector_gv = exception_selector_gv;
            this->vm_block_chain_state_gv = vm_block_chain_state_gv;
            this->expected_bb_token_gv = expected_bb_token_gv;
            this->vm_function_key_gv = vm_function_key_gv;
            this->opcode_xorshift32_state_gv = opcode_xorshift32_state_gv;
            this->vm_code_state_gv = vm_code_state_gv;
            this->vm_function_key = vm_function_key;
            this->govm_interpreter = govm_interpreter;
            this->resume_unwind_helper = resume_unwind_helper;
            this->data_seg_size = data_seg_size;
        }

        Module * Mod;
        Function * F;
        DataLayout * modDataLayout;

        std::map<GlobalVariable *, int> *gv_value_map;
        std::map<int, GEPInfo> *gep_info_map;
        std::map<int, BlockAddressInfo> *blockaddress_info_map;
        std::map<Value *, int> *value_map;

        GlobalVariable *gv_data_seg;
        GlobalVariable *gv_code_seg;
        GlobalVariable *ip;
        GlobalVariable *data_seg_addr;
        GlobalVariable *code_seg_addr;
        GlobalVariable *dispatch_code_seg_addr;
        GlobalVariable *exception_thrown_gv;
        GlobalVariable *exception_ptr_gv;
        GlobalVariable *exception_selector_gv;
        GlobalVariable *vm_block_chain_state_gv;
        GlobalVariable *expected_bb_token_gv;
        GlobalVariable *vm_function_key_gv;
        GlobalVariable *opcode_xorshift32_state_gv;
        GlobalVariable *vm_code_state_gv;
        uint64_t vm_function_key;
        Function *govm_interpreter;
        Function *resume_unwind_helper;
        int data_seg_size;


        virtual void run ();


};



/* ********************************************************************
*   GOVMMODIFIER_CPP
***********************************************************************
*/

void GOVMModifier::run() {

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier] Starting run() for function: " << F->getName() << "\n";
    }

    // 第二步改为强制内联，让 wrapper 尽量并回调用点。
    F->removeFnAttr(Attribute::NoInline);
    F->addFnAttr(Attribute::AlwaysInline);

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier]   Added alwaysinline attribute for wrapper folding\n";
    }

    std::string orig_name = F->getName().str();

    if (!blockaddress_info_map->empty()) {
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[GOVMModifier]   Cloning blockaddress anchor function for " << F->getName() << "\n";
        }

        Function *BlockAddrAnchor = Function::Create(
            F->getFunctionType(),
            GlobalValue::InternalLinkage,
            orig_name + "_blockaddr_anchor",
            Mod);
        BlockAddrAnchor->setCallingConv(F->getCallingConv());
        BlockAddrAnchor->setSection(".AProtect.text");
        BlockAddrAnchor->copyAttributesFrom(F);
        BlockAddrAnchor->removeFnAttr(Attribute::AlwaysInline);

        ValueToValueMapTy BlockAddrVMap;
        Function::arg_iterator DestI = BlockAddrAnchor->arg_begin();
        for (const Argument &I : F->args()) {
            DestI->setName(I.getName());
            BlockAddrVMap[&I] = &*DestI++;
        }

        SmallVector<ReturnInst *, 8> returns;
        CloneFunctionInto(BlockAddrAnchor, F, BlockAddrVMap, CloneFunctionChangeType::DifferentModule, returns);

        for (auto &entry : *blockaddress_info_map) {
            BlockAddress *OldBA = entry.second.BA;
            auto mappedBB = BlockAddrVMap.find(OldBA->getBasicBlock());
            if (mappedBB == BlockAddrVMap.end()) {
                continue;
            }

            BasicBlock *NewBB = cast<BasicBlock>(mappedBB->second);
            BlockAddress *NewBA = BlockAddress::get(BlockAddrAnchor, NewBB);
            OldBA->replaceAllUsesWith(NewBA);
            entry.second.BA = NewBA;
        }
    }
    
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier]   Deleting function body...\n";
    }
    F->deleteBody();
    
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier]   Creating new basic block...\n";
    }
    BasicBlock* body_bbl = BasicBlock::Create(this->Mod->getContext(), "entry", F);
    IRBuilder<> irbuilder(body_bbl);

    assert(!F->isVarArg());

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier]   Processing arguments...\n";
    }
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

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier]   Storing global variable addresses (gv_value_map size=" << gv_value_map->size() << ")...\n";
    }
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

    // 存储 GEP 的结果地址
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier]   Storing GEP result addresses (gep_info_map size=" << gep_info_map->size() << ")...\n";
    }
    for (auto p: *gep_info_map) {
        int data_offset = p.first;
        GEPInfo info = p.second;

        errs() << "[GOVMModifier]   Processing GEP: data_offset=" << data_offset
               << ", GV=" << info.GV->getName() << ", gep_offset=" << info.gep_offset << "\n";

        // 获取全局变量的地址
        Value * gv_addr_int = irbuilder.CreatePtrToInt(info.GV, Type::getInt64Ty(Mod->getContext()));

        // 计算 GEP 结果地址
        Value * offset_int = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), info.gep_offset);
        Value * result_addr = irbuilder.CreateAdd(gv_addr_int, offset_int);

        // 存储结果地址到 data_seg
        ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
        Value * data_offset_val = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), data_offset);
        Value * gepinst = irbuilder.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, data_offset_val}, "");
        Value * ptr = irbuilder.CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));
        irbuilder.CreateStore(result_addr, ptr);

        if (isIRObfuscationDebugEnabled()) {
            errs() << "[GOVMModifier]     Stored GEP result at data_seg[" << data_offset << "]\n";
        }
    }

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier]   Storing BlockAddress values (blockaddress_info_map size="
               << blockaddress_info_map->size() << ")...\n";
    }
    for (auto p : *blockaddress_info_map) {
        int data_offset = p.first;
        BlockAddress *BA = p.second.BA;

        Value *ba_addr_int = irbuilder.CreatePtrToInt(BA, Type::getInt64Ty(Mod->getContext()));
        ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0);
        Value *offset_value = ConstantInt::get(Type::getInt64Ty(Mod->getContext()), data_offset);
        Value *gepinst = irbuilder.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, offset_value}, "");
        Value *ptr = irbuilder.CreatePointerCast(gepinst, PointerType::get(Mod->getContext(), 0));
        irbuilder.CreateStore(ba_addr_int, ptr);
    }

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier]   Processing argument mappings...\n";
    }
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

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier]   Setting data_seg_addr and code_seg_addr...\n";
    }


    Value * data_seg_ptr2int = irbuilder.CreatePtrToInt(gv_data_seg, Type::getInt64Ty(Mod->getContext()));
    irbuilder.CreateStore(data_seg_ptr2int, data_seg_addr);
    Value * code_seg_ptr2int = irbuilder.CreatePtrToInt(gv_code_seg, Type::getInt64Ty(Mod->getContext()));
    irbuilder.CreateStore(code_seg_ptr2int, code_seg_addr);
    irbuilder.CreateStore(code_seg_ptr2int, dispatch_code_seg_addr);

    irbuilder.CreateStore(ConstantInt::get(Type::getInt64Ty(Mod->getContext()), vm_function_key), vm_function_key_gv);

    // 重置 ip 为 0，确保每次调用都从函数开头执行
    irbuilder.CreateStore(ConstantInt::get(Type::getInt32Ty(Mod->getContext()), 0), ip);
    irbuilder.CreateStore(ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0), vm_block_chain_state_gv);
    irbuilder.CreateStore(ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0), expected_bb_token_gv);
    irbuilder.CreateStore(ConstantInt::get(Type::getInt8Ty(Mod->getContext()), 0), exception_thrown_gv);
    irbuilder.CreateStore(ConstantPointerNull::get(PointerType::get(Mod->getContext(), 0)), exception_ptr_gv);
    irbuilder.CreateStore(ConstantInt::get(Type::getInt32Ty(Mod->getContext()), 0), exception_selector_gv);

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier]   Creating call to vm_interpreter...\n";
    }
    irbuilder.CreateCall(govm_interpreter);

    BasicBlock *normalReturnBB = BasicBlock::Create(this->Mod->getContext(), "vm_normal_return", F);
    BasicBlock *resumeExceptionBB = BasicBlock::Create(this->Mod->getContext(), "vm_resume_exception", F);
    Value *exceptionThrownFlag = irbuilder.CreateLoad(Type::getInt8Ty(Mod->getContext()), exception_thrown_gv);
    Value *hasUnhandledException = irbuilder.CreateICmpNE(
        exceptionThrownFlag,
        ConstantInt::get(Type::getInt8Ty(Mod->getContext()), 0));
    irbuilder.CreateCondBr(hasUnhandledException, resumeExceptionBB, normalReturnBB);

    IRBuilder<> normalBuilder(normalReturnBB);

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMModifier]   Creating return...\n";
    }

    if (!F->getReturnType()->isVoidTy()) {
        ConstantInt *Zero = ConstantInt::get(Type::getInt64Ty(F->getContext()), 0);
        Value * gepinst = normalBuilder.CreateGEP(gv_data_seg->getValueType(), gv_data_seg, {Zero, Zero}, "");
        Value * ptr = normalBuilder.CreatePointerCast(gepinst, PointerType::get(F->getContext(), 0));
        Value * retval = normalBuilder.CreateLoad(F->getReturnType(), ptr);
        normalBuilder.CreateRet(retval);
    }
    else {
        normalBuilder.CreateRetVoid();
    }

    IRBuilder<> resumeBuilder(resumeExceptionBB);
    Value *resumeExceptionPtr = resumeBuilder.CreateLoad(PointerType::get(Mod->getContext(), 0), exception_ptr_gv);
    resumeBuilder.CreateCall(resume_unwind_helper, {resumeExceptionPtr});
    resumeBuilder.CreateUnreachable();
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
                        GlobalVariable *code_seg_addr,
                        GlobalVariable *exc_thrown_gv,
                        GlobalVariable *exc_ptr_gv,
                        GlobalVariable *exc_sel_gv) {
            this->Mod = F->getParent();
            this->F = F;
            this->modDataLayout = const_cast<DataLayout *>(&this->Mod->getDataLayout());
            this->callinst_handler = callinst_handler;
            this->gv_data_seg = gv_data_seg;
            this->gv_code_seg = gv_code_seg;
            this->ip = ip;
            this->data_seg_addr = data_seg_addr;
            this->code_seg_addr = code_seg_addr;
            this->exc_thrown_gv = exc_thrown_gv;
            this->exc_ptr_gv = exc_ptr_gv;
            this->exc_sel_gv = exc_sel_gv;

            construct_gv();
        }

        Module * Mod;
        Function * F;
        DataLayout * modDataLayout;

        Function *callinst_handler;

        GlobalVariable *dispatch_code_seg_addr;
        GlobalVariable *gv_data_seg;
        GlobalVariable *gv_code_seg;
        GlobalVariable *ip;
        GlobalVariable *data_seg_addr;
        GlobalVariable *code_seg_addr;
        GlobalVariable *pointer_size_gv;
        GlobalVariable *opcode_xorshift32_state;
        GlobalVariable *vm_code_state;
        GlobalVariable *vm_function_key_gv;
        GlobalVariable *vm_block_chain_state_gv;
        GlobalVariable *expected_bb_token_gv;
        GlobalVariable *exc_thrown_gv;
        GlobalVariable *exc_ptr_gv;
        GlobalVariable *exc_sel_gv;
        GlobalVariable *last_bb_gv;
        GlobalVariable *curr_bb_gv;
        GlobalVariable *vmp_debug_enabled_gv;  // Debug mode control

        virtual void run ();
        virtual void construct_gv ();
        GlobalVariable *get_vm_block_chain_state_gv() { return vm_block_chain_state_gv; }
        GlobalVariable *get_expected_bb_token_gv() { return expected_bb_token_gv; }
        GlobalVariable *get_vm_function_key_gv() { return vm_function_key_gv; }
        GlobalVariable *get_dispatch_code_seg_addr_gv() { return dispatch_code_seg_addr; }
        GlobalVariable *get_opcode_xorshift32_state_gv() { return opcode_xorshift32_state; }
        GlobalVariable *get_vm_code_state_gv() { return vm_code_state; }


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
                                                        "splitmix64Next",
                                                        "load32_le",
                                                        "store32_le",
                                                        "chacha_rotl32",
                                                        "chacha20_block",
                                                        "derive_chacha_material",
                                                        "vmp_schedule_bitrev6",
                                                        "vmp_schedule_rotl6",
                                                        "vmp_schedule_seed",
                                                        "vmp_schedule_index",
                                                        "vmp_schedule_mask",
                                                        "chacha20_byte_at",
                                                        "vm_layout_mix64",
                                                        "vm_prepare_layout_variant",
                                                        "vm_trace_encode_payload",
                                                        "vm_trace_decode_payload",
                                                        "vm_debug_layout_value",
                                                        "vm_debug_log_stdio_entry",
                                                        "vm_debug_log_ip_stage",
                                                        "vm_debug_log_u32_stage",
                                                        "vm_trace_push",
                                                        "vm_dump_fault_context",
                                                        "call_handler_with_exception_handling",
                                                        "vmp_report_and_kill",
                                                        "__clang_call_terminate",
                                                        "get_aggregate_addr",
                                                        "vmp_resume_unwind",
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
                        // 排除callinst_dispatch函数，因为它是在当前模块中创建的，不是解释器模块中的
                        if (func_name.find("vm_interpreter_callinst_dispatch") != std::string::npos) {
                            return false;
                        }
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

static GlobalVariable *getOrCreateSharedGV(Module *M, Type *Ty, Constant *Init,
                                            StringRef Name,
                                            StringRef Section) {
    if (GlobalVariable *GV = M->getGlobalVariable(Name, true)) {
        if (!Section.empty()) {
            GV->setSection(Section);
        }
        return GV;
    }
    GlobalVariable *GV = new GlobalVariable(*M, Ty, false, GlobalValue::InternalLinkage,
                                            Init, Name);
    if (!Section.empty()) {
        GV->setSection(Section);
    }
    return GV;
}

void GOVMInterpreter::construct_gv() {
    unsigned ptr_size = modDataLayout->getPointerSize();
    dispatch_code_seg_addr = getOrCreateSharedGV(
        Mod, Type::getInt64Ty(Mod->getContext()),
        ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
        "vmp_shared_dispatch_code_seg_addr", ".AProtect.data");

    pointer_size_gv = getOrCreateSharedGV(
        Mod, Type::getInt32Ty(Mod->getContext()),
        ConstantInt::get(Type::getInt32Ty(Mod->getContext()), ptr_size),
        "vmp_shared_pointer_size", ".AProtect.data");

    opcode_xorshift32_state = getOrCreateSharedGV(
        Mod, Type::getInt64Ty(Mod->getContext()),
        ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
        "vmp_shared_opcode_xorshift32_state", ".AProtect.data");

    vm_code_state = getOrCreateSharedGV(
        Mod, Type::getInt64Ty(Mod->getContext()),
        ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
        "vmp_shared_vm_code_state", ".AProtect.data");

    vm_function_key_gv = getOrCreateSharedGV(
        Mod, Type::getInt64Ty(Mod->getContext()),
        ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
        "vmp_shared_vm_function_key", ".AProtect.data");

    vm_block_chain_state_gv = getOrCreateSharedGV(
        Mod, Type::getInt64Ty(Mod->getContext()),
        ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
        "vmp_shared_vm_block_chain_state", ".AProtect.data");

    expected_bb_token_gv = getOrCreateSharedGV(
        Mod, Type::getInt64Ty(Mod->getContext()),
        ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
        "vmp_shared_expected_bb_token", ".AProtect.data");

    assert(exc_thrown_gv && "missing shared exception_thrown global");
    assert(exc_ptr_gv && "missing shared exception_ptr global");
    assert(exc_sel_gv && "missing shared exception_selector global");

    last_bb_gv = getOrCreateSharedGV(
        Mod, Type::getInt64Ty(Mod->getContext()),
        ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
        "vmp_shared_last_br_from_bb_id", ".AProtect.data");

    curr_bb_gv = getOrCreateSharedGV(
        Mod, Type::getInt64Ty(Mod->getContext()),
        ConstantInt::get(Type::getInt64Ty(Mod->getContext()), 0),
        "vmp_shared_current_bb_id", ".AProtect.data");

    uint8_t debug_enabled = isIRObfuscationDebugEnabled() ? 1 : 0;
    vmp_debug_enabled_gv = getOrCreateSharedGV(
        Mod, Type::getInt8Ty(Mod->getContext()),
        ConstantInt::get(Type::getInt8Ty(Mod->getContext()), debug_enabled),
        "vmp_shared_debug_enabled", ".AProtect.data");
}

// Function *govm_interpreter;

// Create debug print function using IRBuilder (like IdaDetect does)
// Uses integer ID instead of string to avoid string constant cloning issues
// If debug is disabled, creates a no-op function
static Function* createVmpDebugId(Module *M, bool debug_enabled, std::string funcName = "vmp_debug_id") {
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
        funcName,
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

static std::string vmpHex64(uint64_t Value) {
    static const char Hex[] = "0123456789abcdef";
    std::string Out;
    Out.reserve(16);
    for (int Shift = 60; Shift >= 0; Shift -= 4) {
        Out.push_back(Hex[(Value >> Shift) & 0xFULL]);
    }
    return Out;
}

static std::string vmpMakeSafeSymbolSuffix(StringRef Name, uint64_t Key) {
    std::string Suffix;
    Suffix.reserve(Name.size() + 24);
    for (char C : Name) {
        bool Keep = (C >= 'a' && C <= 'z') ||
                    (C >= 'A' && C <= 'Z') ||
                    (C >= '0' && C <= '9') ||
                    C == '_' || C == '$' || C == '.';
        Suffix.push_back(Keep ? C : '_');
    }
    if (Suffix.empty()) {
        Suffix = "anon";
    }
    Suffix += "_k";
    Suffix += vmpHex64(Key);
    return Suffix;
}

struct VMPDispatchCase {
    ConstantInt *Value;
    BasicBlock *Dest;
};

static SwitchInst *findVmOpcodeDispatchSwitch(Function *F) {
    if (!F) {
        return nullptr;
    }

    for (Instruction &I : instructions(F)) {
        SwitchInst *SI = dyn_cast<SwitchInst>(&I);
        if (!SI) {
            continue;
        }
        if (!SI->getCondition()->getType()->isIntegerTy(8)) {
            continue;
        }

        bool Seen[OP_TOTAL + 1] = {};
        unsigned Matched = 0;
        for (auto CaseIt = SI->case_begin(); CaseIt != SI->case_end(); ++CaseIt) {
            ConstantInt *CaseValue = CaseIt->getCaseValue();
            if (!CaseValue) {
                continue;
            }
            uint64_t Opcode = CaseValue->getZExtValue();
            if (Opcode <= OP_TOTAL && !Seen[Opcode]) {
                Seen[Opcode] = true;
                ++Matched;
            }
        }

        if (Matched >= OP_TOTAL) {
            return SI;
        }
    }

    return nullptr;
}

static void addPhiIncomingForNewDispatchPred(BasicBlock *Dest,
                                             BasicBlock *OldPred,
                                             BasicBlock *NewPred) {
    if (!Dest || !OldPred || !NewPred) {
        return;
    }

    for (PHINode &PN : Dest->phis()) {
        Value *Incoming = nullptr;
        for (unsigned I = 0, E = PN.getNumIncomingValues(); I != E; ++I) {
            if (PN.getIncomingBlock(I) == OldPred) {
                Incoming = PN.getIncomingValue(I);
                break;
            }
        }
        if (!Incoming) {
            Incoming = UndefValue::get(PN.getType());
        }
        PN.addIncoming(Incoming, NewPred);
    }
}

static void removePhiIncomingForOldDispatchPred(BasicBlock *Dest,
                                                BasicBlock *OldPred) {
    if (!Dest || !OldPred) {
        return;
    }

    for (PHINode &PN : Dest->phis()) {
        while (true) {
            int Index = PN.getBasicBlockIndex(OldPred);
            if (Index < 0) {
                break;
            }
            PN.removeIncomingValue((unsigned)Index, false);
        }
    }
}

static bool randomizeVmOpcodeDispatch(Function *Clone,
                                      uint64_t FunctionKey,
                                      GlobalVariable *SaltGV) {
    SwitchInst *SI = findVmOpcodeDispatchSwitch(Clone);
    if (!SI) {
        if (isIRObfuscationDebugEnabled() && Clone) {
            errs() << "[VMP_DISPATCH_RANDOMIZE] no opcode switch in "
                   << Clone->getName() << "\n";
        }
        return false;
    }

    BasicBlock *SwitchBB = SI->getParent();
    BasicBlock *DefaultDest = SI->getDefaultDest();
    Value *OpcodeValue = SI->getCondition();
    IntegerType *OpcodeTy = dyn_cast<IntegerType>(OpcodeValue->getType());
    if (!SwitchBB || !DefaultDest || !OpcodeTy) {
        return false;
    }

    std::vector<VMPDispatchCase> Cases;
    Cases.reserve(SI->getNumCases());
    for (auto CaseIt = SI->case_begin(); CaseIt != SI->case_end(); ++CaseIt) {
        Cases.push_back({CaseIt->getCaseValue(), CaseIt->getCaseSuccessor()});
    }
    if (Cases.empty()) {
        return false;
    }

    uint64_t State = FunctionKey ^ 0x94d049bb133111ebULL;
    if (State == 0) {
        State = 0xd1b54a32d192ed03ULL;
    }
    for (size_t I = Cases.size(); I > 1; --I) {
        uint64_t R = vmpSplitmix64NextLocal(State);
        size_t J = static_cast<size_t>(R % I);
        std::swap(Cases[I - 1], Cases[J]);
    }

    Function *F = SwitchBB->getParent();
    LLVMContext &Ctx = F->getContext();
    std::string BaseName = "vmp.dispatch." + vmpHex64(FunctionKey);
    std::vector<BasicBlock *> TestBlocks;
    TestBlocks.reserve(Cases.size());
    for (size_t I = 0; I < Cases.size(); ++I) {
        TestBlocks.push_back(BasicBlock::Create(
            Ctx, BaseName + "." + std::to_string(I), F));
    }

    std::set<BasicBlock *> Destinations;
    for (size_t I = 0; I < Cases.size(); ++I) {
        BasicBlock *TestBB = TestBlocks[I];
        BasicBlock *CaseDest = Cases[I].Dest;
        BasicBlock *FalseDest = (I + 1 < Cases.size()) ? TestBlocks[I + 1] : DefaultDest;

        addPhiIncomingForNewDispatchPred(CaseDest, SwitchBB, TestBB);
        Destinations.insert(CaseDest);

        IRBuilder<> Builder(TestBB);
        Value *LHS = OpcodeValue;
        Value *RHS = Cases[I].Value;

        if (SaltGV && SaltGV->getValueType()->isIntegerTy(64)) {
            LoadInst *SaltA = Builder.CreateLoad(SaltGV->getValueType(), SaltGV,
                                                 "vmp.dispatch.salt.a");
            LoadInst *SaltB = Builder.CreateLoad(SaltGV->getValueType(), SaltGV,
                                                 "vmp.dispatch.salt.b");
            SaltA->setVolatile(true);
            SaltB->setVolatile(true);
            Value *SaltAOp = Builder.CreateTrunc(SaltA, OpcodeTy);
            Value *SaltBOp = Builder.CreateTrunc(SaltB, OpcodeTy);
            LHS = Builder.CreateXor(OpcodeValue, SaltAOp);
            RHS = Builder.CreateXor(Cases[I].Value, SaltBOp);
        }

        Value *Cond = Builder.CreateICmpEQ(LHS, RHS);
        Builder.CreateCondBr(Cond, CaseDest, FalseDest);
    }

    addPhiIncomingForNewDispatchPred(DefaultDest, SwitchBB, TestBlocks.back());
    Destinations.insert(DefaultDest);

    IRBuilder<> EntryBuilder(SI);
    EntryBuilder.CreateBr(TestBlocks.front());
    SI->eraseFromParent();

    for (BasicBlock *Dest : Destinations) {
        removePhiIncomingForOldDispatchPred(Dest, SwitchBB);
    }

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[VMP_DISPATCH_RANDOMIZE] clone=" << F->getName()
               << " cases=" << Cases.size()
               << " style=volatile-xor-chain"
               << " key=0x" << Twine::utohexstr(FunctionKey)
               << "\n";
    }

    return true;
}

static Function *cloneVmInterpreterForFunction(Module *M,
                                               Function *SharedInterpreter,
                                               Function *TargetFunction,
                                               uint64_t FunctionKey) {
    if (!M || !SharedInterpreter || !TargetFunction || SharedInterpreter->empty()) {
        return SharedInterpreter;
    }

    std::string Suffix = vmpMakeSafeSymbolSuffix(TargetFunction->getName(), FunctionKey);
    std::string CloneName = "vm_interpreter_" + Suffix;

    Function *Clone = Function::Create(
        SharedInterpreter->getFunctionType(),
        GlobalValue::InternalLinkage,
        CloneName,
        M);
    Clone->copyAttributesFrom(SharedInterpreter);
    Clone->setCallingConv(SharedInterpreter->getCallingConv());
    Clone->setDSOLocal(true);
    normalizeLocalDefaultVisibility(Clone);

    ValueToValueMapTy VMap;
    Function::arg_iterator DestI = Clone->arg_begin();
    for (const Argument &Arg : SharedInterpreter->args()) {
        DestI->setName(Arg.getName());
        VMap[&Arg] = &*DestI++;
    }

    SmallVector<ReturnInst *, 8> Returns;
    CloneFunctionInto(Clone, SharedInterpreter, VMap,
                      CloneFunctionChangeType::LocalChangesOnly, Returns);
    Clone->setSection(".AProtect.text");
    normalizeLocalDefaultVisibility(Clone);

    IntegerType *I64Ty = Type::getInt64Ty(M->getContext());
    uint64_t Salt = FunctionKey ^ 0xa0761d6478bd642fULL;
    GlobalVariable *SaltGV = new GlobalVariable(
        *M,
        I64Ty,
        false,
        GlobalValue::InternalLinkage,
        ConstantInt::get(I64Ty, Salt),
        "vmp_interpreter_clone_salt_" + Suffix);
    SaltGV->setSection(".AProtect.data");
    SaltGV->setAlignment(Align(8));

    BasicBlock &Entry = Clone->getEntryBlock();
    IRBuilder<> Builder(&*Entry.getFirstInsertionPt());
    LoadInst *SaltLoad = Builder.CreateLoad(I64Ty, SaltGV, "vmp.clone.salt");
    SaltLoad->setVolatile(true);

    randomizeVmOpcodeDispatch(Clone, FunctionKey, SaltGV);

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[VMP_INTERPRETER_CLONE] function=" << TargetFunction->getName()
               << " key=0x" << Twine::utohexstr(FunctionKey)
               << " clone=" << Clone->getName() << "\n";
    }

    return Clone;
}

void GOVMInterpreter::run() {

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Starting run() for function: " << F->getName() << "\n";
        errs() << "[GOVMInterpreter] Step 1: Parsing bitcode...\n";
    }
    Module *interpreter_module = llvm_parse_bitcode_from_string();
    if (!interpreter_module) {
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[GOVMInterpreter] ERROR: Failed to parse bitcode\n";
        }
        return;
    }
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 1: Bitcode parsed successfully\n";
    }

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 2: Creating debug function...\n";
    }
    // 为每个VMP函数创建一个唯一的debug函数名
    std::string debugFuncName = "vmp_debug_id_" + F->getName().str();
    Function *DebugIdFunc = createVmpDebugId(Mod, isIRObfuscationDebugEnabled(), debugFuncName);
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 2: Debug function created: " << debugFuncName << "\n";
    }

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 3: Replacing debug function...\n";
    }
    if (Function *OldDebugId = interpreter_module->getFunction("vmp_debug_id")) {
        OldDebugId->replaceAllUsesWith(DebugIdFunc);
    }
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 3: Debug function replaced\n";
    }

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 4: Replacing global variables...\n";
        errs() << "[GOVMInterpreter]   gv_data_seg = " << (void*)gv_data_seg << "\n";
        errs() << "[GOVMInterpreter]   gv_code_seg = " << (void*)gv_code_seg << "\n";
        errs() << "[GOVMInterpreter]   ip = " << (void*)ip << "\n";
        errs() << "[GOVMInterpreter]   data_seg_addr = " << (void*)data_seg_addr << "\n";
        errs() << "[GOVMInterpreter]   code_seg_addr = " << (void*)code_seg_addr << "\n";
    }
    
    std::vector<std::string> gv_list = {"gv_data_seg", "gv_code_seg", "ip", "data_seg_addr", "code_seg_addr", "dispatch_code_seg_addr", "pointer_size", "opcode_xorshift32_state", "vm_code_state", "vm_function_key", "vm_block_chain_state", "expected_bb_token", "exception_thrown", "exception_ptr", "exception_selector", "last_br_from_bb_id", "current_bb_id", "vmp_debug_enabled"};
    std::vector<GlobalVariable *> new_gv_list = {gv_data_seg, gv_code_seg, ip, data_seg_addr, code_seg_addr, dispatch_code_seg_addr, pointer_size_gv, opcode_xorshift32_state, vm_code_state, vm_function_key_gv, vm_block_chain_state_gv, expected_bb_token_gv, exc_thrown_gv, exc_ptr_gv, exc_sel_gv, last_bb_gv, curr_bb_gv, vmp_debug_enabled_gv};
    
    std::map<GlobalVariable*, GlobalVariable*> gv_remap;
    for (unsigned i = 0; i < gv_list.size(); i++) {
        GlobalVariable *old_gv = interpreter_module->getGlobalVariable(gv_list[i]);
        if (!old_gv) {
            if (isIRObfuscationDebugEnabled()) {
                errs() << "[GOVMInterpreter]   WARNING: old_gv '" << gv_list[i] << "' not found in interpreter module\n";
            }
            continue;
        }
        GlobalVariable *new_gv = new_gv_list[i];
        if (!new_gv) {
            if (isIRObfuscationDebugEnabled()) {
                errs() << "[GOVMInterpreter]   WARNING: new_gv '" << gv_list[i] << "' is null\n";
            }
            continue;
        }
        
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[GOVMInterpreter]   Replacing " << gv_list[i] << ": old type=" << *old_gv->getValueType() << ", new type=" << *new_gv->getValueType() << "\n";
        }
        gv_remap[old_gv] = new_gv;
    }
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 4: Global variables mapped, gv_remap size=" << gv_remap.size() << "\n";
    }

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 5: Replacing call_handler...\n";
    }
    Function *old_func = interpreter_module->getFunction("call_handler");
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 5: call_handler replaced\n";
    }

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 6: Collecting function declarations...\n";
    }
    for(auto Func = interpreter_module->begin(); Func != interpreter_module->end(); ++Func) {
        Function *fun = &*Func;
        if(fun->isDeclaration()) {
            if (fun->getName() == "call_handler") {
                continue;
            }
            if(!Mod->getFunction(fun->getName())) {
                FunctionCallee FC = Mod->getOrInsertFunction(fun->getName().str(), fun->getFunctionType());
                Function *NewF = cast<Function>(FC.getCallee());
                NewF->setLinkage(fun->getLinkage());
            }
        }
    }
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 6: Function declarations collected\n";
    }
    
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 7: Creating interpreter function declarations...\n";
    }
    
    // 步骤7a: 先创建所有解释器函数的声明，避免克隆时找不到目标函数
    std::map<Function*, Function*> interpreter_func_map;  // 原函数 -> 新函数
    for(auto Func = interpreter_module->begin(); Func != interpreter_module->end(); ++Func) {
        Function *fun = &*Func;
        if(is_interpreter_function(fun)) {
            std::string funcName = fun->getName().str();
            std::string newFuncName;
            
            newFuncName = funcName + "_shared";
            
            Function *NewF = Mod->getFunction(newFuncName);
            if (!NewF) {
                NewF = Function::Create(fun->getFunctionType(),
                    llvm::GlobalValue::LinkageTypes::InternalLinkage,
                    newFuncName, Mod);
            }
            normalizeLocalDefaultVisibility(NewF);
            interpreter_func_map[fun] = NewF;
        }
    }
    
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 8: Cloning interpreter functions...\n";
    }
    int func_idx = 0;
    for(auto Func = interpreter_module->begin();Func!=interpreter_module->end();++Func)
        {
            
            Function *fun = &*Func;

            if(is_interpreter_function(fun)) {
                // 使用带函数名后缀的名称，避免多个VMP函数之间的冲突
                // 但是如果函数名已经包含了当前VMP函数名，就不要再添加后缀
                std::string funcName = fun->getName().str();
                std::string newFuncName;
                
                newFuncName = funcName + "_shared";
                
                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[GOVMInterpreter]   Cloning function: " << fun->getName() << " -> " << newFuncName << " (idx=" << func_idx++ << ")\n";
                }
                
                Function *NewF = interpreter_func_map[fun];
                if (!NewF->empty()) {
                    continue;
                }

                ValueToValueMapTy VMap;
                SmallVector<ReturnInst*, 8> returns;

                if (DebugIdFunc) {
                    VMap[interpreter_module->getFunction("vmp_debug_id")] = DebugIdFunc;
                }
                if (old_func && callinst_handler) {
                    VMap[old_func] = callinst_handler;
                }

                for (auto &gv_pair : gv_remap) {
                    VMap[gv_pair.first] = gv_pair.second;
                }

                // 处理所有全局变量（包括字符串常量）
                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[GOVMInterpreter]     Processing global variables...\n";
                }
                for (auto it = interpreter_module->global_begin(); it != interpreter_module->global_end(); ++it) {
                    GlobalVariable &GV = *it;
                    if (GV.hasAppendingLinkage() || GV.getName().starts_with("llvm.")) {
                        continue;
                    }
                    if (VMap.find(&GV) == VMap.end()) {
                        // 如果这个全局变量还没有被映射，创建一个新的
                        Constant *Init = nullptr;
                        if (GV.hasInitializer()) {
                            Init = GV.getInitializer();
                        } else {
                            Init = Constant::getNullValue(GV.getValueType());
                        }

                        GlobalVariable *NewGV = new GlobalVariable(
                            *Mod,
                            GV.getValueType(),
                            GV.isConstant(),
                            GV.getLinkage(),
                            Init,
                            GV.getName().str() + "_shared"
                        );
                        normalizeLocalDefaultVisibility(NewGV);
                        if (GV.hasInitializer()) {
                            NewGV->setInitializer(Init);
                        }
                        if (GV.hasGlobalUnnamedAddr()) {
                            NewGV->setUnnamedAddr(GlobalValue::UnnamedAddr::Global);
                        } else if (GV.hasAtLeastLocalUnnamedAddr()) {
                            NewGV->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
                        }
                        if (GV.hasComdat()) {
                            NewGV->setComdat(Mod->getOrInsertComdat(NewGV->getName()));
                        }
                        NewGV->setThreadLocalMode(GV.getThreadLocalMode());
                        NewGV->setDSOLocal(GV.isDSOLocal());
                        if (GV.getAlign()) {
                            NewGV->setAlignment(GV.getAlign());
                        }
                        NewGV->setSection(GV.getSection());
                        VMap[&GV] = NewGV;

                        if (isIRObfuscationDebugEnabled()) {
                            errs() << "[GOVMInterpreter]       Cloned global: " << GV.getName() << " -> " << NewGV->getName() << "\n";
                        }
                    }
                }

                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[GOVMInterpreter]     Processing instructions...\n";
                }
                for (Instruction &I : instructions(fun)) {
                    if (CallBase *CB = dyn_cast<CallBase>(&I)) {
                        Function *Callee = CB->getCalledFunction();
                        if (Callee && Callee != fun) {
                            // 首先检查是否是解释器函数
                            auto it = interpreter_func_map.find(Callee);
                            if (it != interpreter_func_map.end()) {
                                // 是解释器函数，使用映射的新函数
                                VMap[Callee] = it->second;
                            } else if (is_interpreter_function(Callee)) {
                                std::string calleeFuncName = Callee->getName().str();
                                std::string mappedName = calleeFuncName + "_shared";
                                Function *TargetCallee = Mod->getFunction(mappedName);
                                if (TargetCallee) {
                                    VMap[Callee] = TargetCallee;
                                }
                            } else if (!Callee->isDeclaration()) {
                                // 不是解释器函数，也不是声明
                                // 检查是否是callinst_dispatch函数，如果是，不要重命名
                                if (Callee->getName().find("vm_interpreter_callinst_dispatch") != std::string::npos) {
                                    // callinst_dispatch函数，不添加到VMap，保持原样
                                    continue;
                                }
                                
                                // 检查是否是vmp_debug_id函数，如果是，不要重命名
                                if (Callee->getName().find("vmp_debug_id") != std::string::npos) {
                                    // vmp_debug_id函数，不添加到VMap，保持原样
                                    continue;
                                }
                                
                                // 其他非声明函数，查找或创建声明
                                std::string calleeFuncName = Callee->getName().str();
                                std::string calleeNewName = calleeFuncName + "_shared";
                                Function *TargetCallee = Mod->getFunction(calleeNewName);
                                
                                if (!TargetCallee) {
                                    TargetCallee = Function::Create(Callee->getFunctionType(), 
                                        Callee->getLinkage(), calleeNewName, Mod);
                                }
                                
                                if (TargetCallee) {
                                    VMap[Callee] = TargetCallee;
                                }
                            }
                            // 如果是声明（外部函数），不添加到VMap，保持原样
                        }
                    }
                }

                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[GOVMInterpreter]     Setting up arguments...\n";
                }
                Function::arg_iterator DestI = NewF->arg_begin();

                for (const Argument & I : fun->args())
                    if (VMap.count(&I) == 0) {
                        DestI->setName(I.getName());
                        VMap[&I] = &*DestI++;
                    }

                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[GOVMInterpreter]     Calling CloneFunctionInto...\n";
                }
                CloneFunctionInto(NewF, fun, VMap, CloneFunctionChangeType::DifferentModule, returns);
                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[GOVMInterpreter]     CloneFunctionInto completed\n";
                }
                normalizeLocalDefaultVisibility(NewF);

                // 设置段名为 .AProtect.text
                NewF->setSection(".AProtect.text");
                normalizeLocalDefaultVisibility(NewF);

                // 函数名已经在创建时设置，这里不需要再设置

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

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 7: All interpreter functions cloned\n";
    }
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
        
        std::string annotation = readAnnotate(f);
        
        if (annotation.find(attrNo) != std::string::npos) {
            return false;
        }

        if (annotation.find(attr) != std::string::npos) {
            used_passes.insert(attribute);
            return true;
        }
        
        std::string vmFunctionsList = getVMFunctionsList();
        if (!vmFunctionsList.empty()) {
            std::string funcName = f->getName().str();
            std::istringstream ss(vmFunctionsList);
            std::string token;
            while (std::getline(ss, token, ';')) {
                if (!token.empty() && funcName == token) {
                    used_passes.insert(attribute);
                    return true;
                }
            }
        }
        
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
            errs() << "[VMP Warning] Function '" << F->getName()
                   << "' is variadic, which is not supported by VMP.\n";
            errs() << "[VMP Warning] Skipping VMP protection for this function.\n";
            continue;
          }
          if (F->isDeclaration()) {
            errs() << "[VMP Warning] Function '" << F->getName()
                   << "' is declaration-only, which is not supported by VMP.\n";
            errs() << "[VMP Warning] Skipping VMP protection for this function.\n";
            continue;
          }
          functions_to_process.push_back(F);
        }
      }
      
      // 如果没有函数需要保护，直接返回
      if (functions_to_process.empty()) {
        return false;
      }
      
      std::vector<GOVMTranslator *> translators;
      for(Function *F : functions_to_process)
      {
        if (isIRObfuscationDebugEnabled()) {
          errs() << "[VMP] Translating function: " << F->getName() << "\n";
        }
        GOVMTranslator *translator = new GOVMTranslator(F);
        if (translator->run()) {
          translators.push_back(translator);
        }
      }

      if (translators.empty()) {
        return false;
      }

      GlobalVariable *dispatch_code_seg_addr = getOrCreateSharedGV(
          &M, Type::getInt64Ty(M.getContext()),
          ConstantInt::get(Type::getInt64Ty(M.getContext()), 0),
          "vmp_shared_dispatch_code_seg_addr", ".AProtect.data");
      std::vector<VMPDispatchTarget> dispatch_targets;
      dispatch_targets.reserve(translators.size());
      for (GOVMTranslator *translator : translators) {
        dispatch_targets.push_back({translator->get_gv_code_seg(),
                                    translator->get_callinst_handler()});
      }
      Function *shared_dispatcher = buildSharedCallDispatcher(
          &M, dispatch_targets, dispatch_code_seg_addr);
      GOVMInterpreter *interpreter = new GOVMInterpreter(
          translators.front()->get_function(), shared_dispatcher,
          translators.front()->get_gv_data_seg(),
          translators.front()->get_gv_code_seg(),
          translators.front()->get_ip(),
          translators.front()->get_data_seg_addr(),
          translators.front()->get_code_seg_addr(),
          translators.front()->exception_thrown,
          translators.front()->exception_ptr_global,
          translators.front()->exception_selector_global);
      interpreter->run();

      Function *shared_vm_interpreter_func = M.getFunction("vm_interpreter_shared");
      Function *resume_unwind_helper_func = M.getFunction("vmp_resume_unwind_shared");
      for (GOVMTranslator *translator : translators) {
        Function *F = translator->get_function();
        Function *vm_interpreter_func = cloneVmInterpreterForFunction(
            &M, shared_vm_interpreter_func, F, translator->get_vm_function_key());
        GOVMModifier *modifier = new GOVMModifier(F, translator->get_gv_value_map(), translator->get_gep_info_map(),
                                                   translator->get_blockaddress_info_map(),
                                                   translator->get_value_map(),
                                                   translator->get_gv_data_seg(),
                                                   translator->get_gv_code_seg(),
                                                   translator->get_ip(),
                                                   translator->get_data_seg_addr(),
                                                   translator->get_code_seg_addr(),
                                                   dispatch_code_seg_addr,
                                                   translator->exception_thrown,
                                                   translator->exception_ptr_global,
                                                   translator->exception_selector_global,
                                                   interpreter->get_vm_block_chain_state_gv(),
                                                   interpreter->get_expected_bb_token_gv(),
                                                   interpreter->get_vm_function_key_gv(),
                                                   interpreter->get_opcode_xorshift32_state_gv(),
                                                   interpreter->get_vm_code_state_gv(),
                                                   translator->get_vm_function_key(),
                                                   vm_interpreter_func,
                                                   resume_unwind_helper_func,
                                                   translator->get_data_seg_size());
        modifier->run();
        if (isIRObfuscationDebugEnabled()) {
          errs() << "[VMP] Function done: " << F->getName() << "\n";
        }
      }

      normalizeModuleLocalDefaultVisibility(M);
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
        errs() << "[VMP Warning] Function '" << F->getName()
               << "' is variadic, which is not supported by VMP.\n";
        errs() << "[VMP Warning] Skipping VMP protection for this function.\n";
        continue;
      }
      if (F->isDeclaration()) {
        errs() << "[VMP Warning] Function '" << F->getName()
               << "' is declaration-only, which is not supported by VMP.\n";
        errs() << "[VMP Warning] Skipping VMP protection for this function.\n";
        continue;
      }
      
      functions_to_process.push_back(F);
    }
  }
  
  // 如果没有函数需要保护，直接返回
  if (functions_to_process.empty()) {
    return PreservedAnalyses::all();
  }
  
  std::vector<GOVMTranslator *> translators;
  for(Function *F : functions_to_process) {
    GOVMTranslator *translator = new GOVMTranslator(F);
    if (translator->run()) {
      translators.push_back(translator);
    }
  }

  if (translators.empty()) {
    return PreservedAnalyses::all();
  }

  GlobalVariable *dispatch_code_seg_addr = getOrCreateSharedGV(
      &M, Type::getInt64Ty(M.getContext()),
      ConstantInt::get(Type::getInt64Ty(M.getContext()), 0),
      "vmp_shared_dispatch_code_seg_addr", ".AProtect.data");
  std::vector<VMPDispatchTarget> dispatch_targets;
  dispatch_targets.reserve(translators.size());
  for (GOVMTranslator *translator : translators) {
    dispatch_targets.push_back({translator->get_gv_code_seg(),
                                translator->get_callinst_handler()});
  }
  Function *shared_dispatcher = buildSharedCallDispatcher(
      &M, dispatch_targets, dispatch_code_seg_addr);
  GOVMInterpreter *interpreter = new GOVMInterpreter(
      translators.front()->get_function(), shared_dispatcher,
      translators.front()->get_gv_data_seg(),
      translators.front()->get_gv_code_seg(),
      translators.front()->get_ip(),
      translators.front()->get_data_seg_addr(),
      translators.front()->get_code_seg_addr(),
      translators.front()->exception_thrown,
      translators.front()->exception_ptr_global,
      translators.front()->exception_selector_global);
  interpreter->run();

  Function *shared_vm_interpreter_func = M.getFunction("vm_interpreter_shared");
  Function *resume_unwind_helper_func = M.getFunction("vmp_resume_unwind_shared");
  for (GOVMTranslator *translator : translators) {
    Function *F = translator->get_function();
    Function *vm_interpreter_func = cloneVmInterpreterForFunction(
        &M, shared_vm_interpreter_func, F, translator->get_vm_function_key());
    GOVMModifier *modifier = new GOVMModifier(F, translator->get_gv_value_map(), translator->get_gep_info_map(),
                                               translator->get_blockaddress_info_map(),
                                               translator->get_value_map(),
                                               translator->get_gv_data_seg(),
                                               translator->get_gv_code_seg(),
                                               translator->get_ip(),
                                               translator->get_data_seg_addr(),
                                               translator->get_code_seg_addr(),
                                               dispatch_code_seg_addr,
                                               translator->exception_thrown,
                                               translator->exception_ptr_global,
                                               translator->exception_selector_global,
                                               interpreter->get_vm_block_chain_state_gv(),
                                               interpreter->get_expected_bb_token_gv(),
                                               interpreter->get_vm_function_key_gv(),
                                               interpreter->get_opcode_xorshift32_state_gv(),
                                               interpreter->get_vm_code_state_gv(),
                                               translator->get_vm_function_key(),
                                               vm_interpreter_func,
                                               resume_unwind_helper_func,
                                               translator->get_data_seg_size());
    modifier->run();
    changed = true;
  }

  normalizeModuleLocalDefaultVisibility(M);
  
  return changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}
#endif
