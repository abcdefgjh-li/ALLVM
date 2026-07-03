#include "llvm/Transforms/Obfuscation/aVMP/aVMPInterpreter.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/Transforms/Obfuscation/aVMP/vm.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Transforms/Utils/Cloning.h"

#include <assert.h>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace llvm;

namespace {

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
    "chacha20_byte_at",
    "vm_trace_push",
    "vm_dump_fault_context",
    "get_aggregate_addr",
    "vmp_resume_unwind",
    "vm_interpreter",
    "vm_interpreter_callinst_dispatch"};

bool is_interpreter_function(Function *targetFunction) {
    if (!targetFunction->isDeclaration() && targetFunction->hasName()) {
        std::string func_name = targetFunction->getName().str();
        for (const std::string &curr_func : interpreter_function_names) {
            if (func_name.find(curr_func.c_str()) != std::string::npos) {
                if (func_name.find("vm_interpreter_callinst_dispatch") != std::string::npos) {
                    return false;
                }
                return true;
            }
        }
    }
    return false;
}

GlobalVariable *getOrCreateSharedGV(Module *M, Type *Ty, Constant *Init,
                                    StringRef Name, StringRef Section) {
    if (GlobalVariable *GV = M->getGlobalVariable(Name, true)) {
        if (!Section.empty()) {
            GV->setSection(Section);
        }
        return GV;
    }
    GlobalVariable *GV = new GlobalVariable(*M, Ty, false,
                                            GlobalValue::InternalLinkage,
                                            Init, Name);
    if (!Section.empty()) {
        GV->setSection(Section);
    }
    return GV;
}

Function *createVmpDebugId(Module *M, bool debug_enabled,
                           std::string funcName = "vmp_debug_id") {
    LLVMContext &Ctx = M->getContext();
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);

    FunctionType *FuncTy = FunctionType::get(VoidTy, {Int32Ty, Int64Ty}, false);
    Function *Func = Function::Create(FuncTy, GlobalValue::InternalLinkage,
                                      M->getDataLayout().getProgramAddressSpace(),
                                      funcName, M);

    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(EntryBB);

    if (!debug_enabled) {
        Builder.CreateRetVoid();
        return Func;
    }

    BasicBlock *NewBBBlock = BasicBlock::Create(Ctx, "new_bb", Func);
    BasicBlock *OpcodeBlock = BasicBlock::Create(Ctx, "opcode", Func);
    BasicBlock *CmpBlock = BasicBlock::Create(Ctx, "cmp", Func);
    BasicBlock *DefaultBlock = BasicBlock::Create(Ctx, "default", Func);

    Value *Id = Func->arg_begin();
    Value *Val = Func->arg_begin() + 1;

    SwitchInst *Switch = Builder.CreateSwitch(Id, DefaultBlock, 10);
    Switch->addCase(cast<ConstantInt>(ConstantInt::get(Int32Ty, 1)), NewBBBlock);
    Switch->addCase(cast<ConstantInt>(ConstantInt::get(Int32Ty, 2)), OpcodeBlock);
    Switch->addCase(cast<ConstantInt>(ConstantInt::get(Int32Ty, 3)), CmpBlock);

    FunctionCallee PrintfFunc = M->getOrInsertFunction(
        "printf", FunctionType::get(Int32Ty, {CharPtrTy}, true));

    IRBuilder<> NewBBBuilder(NewBBBlock);
    Constant *NewBBStr = ConstantDataArray::getString(Ctx, "[BB] IP=%ld\n");
    GlobalVariable *NewBBGV = new GlobalVariable(
        *M, NewBBStr->getType(), true, GlobalValue::PrivateLinkage,
        NewBBStr, ".vmp.dbg.bb");
    NewBBBuilder.CreateCall(PrintfFunc,
                            {ConstantExpr::getBitCast(NewBBGV, CharPtrTy), Val});
    NewBBBuilder.CreateBr(DefaultBlock);

    IRBuilder<> OpcodeBuilder(OpcodeBlock);
    Constant *OpcodeStr = ConstantDataArray::getString(Ctx, "[OP] %ld\n");
    GlobalVariable *OpcodeGV = new GlobalVariable(
        *M, OpcodeStr->getType(), true, GlobalValue::PrivateLinkage,
        OpcodeStr, ".vmp.dbg.op");
    OpcodeBuilder.CreateCall(PrintfFunc,
                             {ConstantExpr::getBitCast(OpcodeGV, CharPtrTy), Val});
    OpcodeBuilder.CreateBr(DefaultBlock);

    IRBuilder<> CmpBuilder(CmpBlock);
    Constant *CmpStr = ConstantDataArray::getString(Ctx, "[CMP] pred=%ld\n");
    GlobalVariable *CmpGV = new GlobalVariable(
        *M, CmpStr->getType(), true, GlobalValue::PrivateLinkage,
        CmpStr, ".vmp.dbg.cmp");
    CmpBuilder.CreateCall(PrintfFunc,
                          {ConstantExpr::getBitCast(CmpGV, CharPtrTy), Val});
    CmpBuilder.CreateBr(DefaultBlock);

    IRBuilder<> DefaultBuilder(DefaultBlock);
    DefaultBuilder.CreateRetVoid();

    return Func;
}

} // namespace

GOVMInterpreter::GOVMInterpreter(Function *F, Function *callinst_handler,
                                 GlobalVariable *gv_data_seg,
                                 GlobalVariable *gv_code_seg,
                                 GlobalVariable *ip,
                                 GlobalVariable *data_seg_addr,
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

Module *GOVMInterpreter::llvm_parse_bitcode_from_string() {
    std::vector<char> binary_ir = get_binary_ir();
    StringRef str_ref(binary_ir.data(), binary_ir.size());
    MemoryBufferRef buf_ref = MemoryBufferRef(str_ref, "aVMPInterpreter.bc");
    Expected<std::unique_ptr<Module>> ModuleOrErr = parseBitcodeFile(buf_ref, Mod->getContext());
    if (!ModuleOrErr) {
        return nullptr;
    }
    return ModuleOrErr.get().release();
}

Module *GOVMInterpreter::llvm_parse_bitcode() {
    SMDiagnostic Err;
    LLVMContext *LLVMCtx = &Mod->getContext();
    std::unique_ptr<Module> M = parseIRFile("../c-implement/govm.bc", Err, *LLVMCtx);
    return M.release();
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
    for (auto Func = interpreter_module->begin(); Func != interpreter_module->end(); ++Func) {
        Function *fun = &*Func;
        if (fun->isDeclaration()) {
            if (fun->getName() == "call_handler") {
                continue;
            }
            if (!Mod->getFunction(fun->getName())) {
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

    std::map<Function*, Function*> interpreter_func_map;
    for (auto Func = interpreter_module->begin(); Func != interpreter_module->end(); ++Func) {
        Function *fun = &*Func;
        if (is_interpreter_function(fun)) {
            std::string funcName = fun->getName().str();
            std::string newFuncName = funcName + "_shared";

            Function *NewF = Mod->getFunction(newFuncName);
            if (!NewF) {
                NewF = Function::Create(fun->getFunctionType(),
                                        llvm::GlobalValue::LinkageTypes::InternalLinkage,
                                        newFuncName, Mod);
            }
            interpreter_func_map[fun] = NewF;
        }
    }

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[GOVMInterpreter] Step 8: Cloning interpreter functions...\n";
    }
    int func_idx = 0;
    for (auto Func = interpreter_module->begin(); Func != interpreter_module->end(); ++Func) {
        Function *fun = &*Func;

        if (is_interpreter_function(fun)) {
            std::string funcName = fun->getName().str();
            std::string newFuncName = funcName + "_shared";

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

            if (isIRObfuscationDebugEnabled()) {
                errs() << "[GOVMInterpreter]     Processing global variables...\n";
            }
            for (auto it = interpreter_module->global_begin(); it != interpreter_module->global_end(); ++it) {
                GlobalVariable &GV = *it;
                if (VMap.find(&GV) == VMap.end()) {
                    Constant *Init = nullptr;
                    if (GV.hasInitializer()) {
                        Init = GV.getInitializer();
                    } else {
                        Init = Constant::getNullValue(GV.getValueType());
                    }

                    GlobalVariable *NewGV = new GlobalVariable(
                        *Mod, GV.getValueType(), GV.isConstant(), GV.getLinkage(),
                        Init, GV.getName().str() + "_shared");
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
                        auto it = interpreter_func_map.find(Callee);
                        if (it != interpreter_func_map.end()) {
                            VMap[Callee] = it->second;
                        } else if (is_interpreter_function(Callee)) {
                            std::string calleeFuncName = Callee->getName().str();
                            std::string mappedName = calleeFuncName + "_shared";
                            Function *TargetCallee = Mod->getFunction(mappedName);
                            if (TargetCallee) {
                                VMap[Callee] = TargetCallee;
                            }
                        } else if (!Callee->isDeclaration()) {
                            if (Callee->getName().find("vm_interpreter_callinst_dispatch") != std::string::npos) {
                                continue;
                            }
                            if (Callee->getName().find("vmp_debug_id