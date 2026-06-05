//===- BanDump.cpp - 防止内存Dump注入Pass ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 本文件实现防止内存Dump注入Pass，在程序入口点注入保护代码
// 使用mprotect保护内存区域
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/BanDump.h"
#include "llvm/Transforms/Obfuscation/DetectUtils.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "bandump"

using namespace llvm;

namespace {

struct BanDump : public ModulePass {
    static char ID;

    BanDump() : ModulePass(ID) {
        initializeBanDumpPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"BanDump"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createBanDumpFunc(Module &M);
};

}

char BanDump::ID = 0;

Function* BanDump::createBanDumpFunc(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "ban_dump",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    // 使用mlockall锁定内存，防止被swap出去
    FunctionCallee MlockallFunc = M.getOrInsertFunction(
        "mlockall",
        FunctionType::get(Int32Ty, {Int32Ty}, false)
    );
    
    // MCL_CURRENT | MCL_FUTURE = 1 | 2 = 3
    Builder.CreateCall(MlockallFunc, {ConstantInt::get(Int32Ty, 3)});
    
    // 使用setrlimit限制core dump
    FunctionCallee SetrlimitFunc = M.getOrInsertFunction(
        "setrlimit",
        FunctionType::get(Int32Ty, {Int32Ty, CharPtrTy}, false)
    );
    
    // RLIMIT_CORE = 4 (Linux ARM64)
    Constant *RlimitCore = ConstantInt::get(Int32Ty, 4);
    
    // struct rlimit { rlim_t rlim_cur; rlim_t rlim_max; }
    Type *RlimitTy = StructType::create(Ctx, "rlimit");
    StructType *RlimitStruct = cast<StructType>(RlimitTy);
    RlimitStruct->setBody({Int64Ty, Int64Ty});
    
    Value *Rlimit = Builder.CreateAlloca(RlimitStruct, nullptr, "rlimit");
    
    // 设置rlim_cur = 0, rlim_max = 0
    Value *CurPtr = Builder.CreateGEP(RlimitStruct, Rlimit, {
        ConstantInt::get(Int32Ty, 0),
        ConstantInt::get(Int32Ty, 0)
    });
    Builder.CreateStore(ConstantInt::get(Int64Ty, 0), CurPtr);
    
    Value *MaxPtr = Builder.CreateGEP(RlimitStruct, Rlimit, {
        ConstantInt::get(Int32Ty, 0),
        ConstantInt::get(Int32Ty, 1)
    });
    Builder.CreateStore(ConstantInt::get(Int64Ty, 0), MaxPtr);
    
    Value *RlimitPtr = Builder.CreateBitCast(Rlimit, CharPtrTy);
    Builder.CreateCall(SetrlimitFunc, {RlimitCore, RlimitPtr});
    
    // 使用prctl禁止ptrace
    FunctionCallee PrctlFunc = M.getOrInsertFunction(
        "prctl",
        FunctionType::get(Int32Ty, {Int32Ty, Int64Ty, Int64Ty, Int64Ty, Int64Ty}, false)
    );
    
    // PR_SET_DUMPABLE = 4, 设置为0禁止dump
    Builder.CreateCall(PrctlFunc, {
        ConstantInt::get(Int32Ty, 4),
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0)
    });
    
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool BanDump::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] BanDump: Injecting anti-dump protection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 创建保护函数
    Function *ProtectFunc = createBanDumpFunc(M);
    
    // 配置选项
    DetectOptions opts = DetectOptions::create(false);
    
    // 注入到main函数
    return DetectUtils::injectToMain(M, ProtectFunc, opts);
}

ModulePass *llvm::createBanDumpPass() {
    return new BanDump();
}

INITIALIZE_PASS_BEGIN(BanDump, "bandump", "Inject anti-dump protection at program start", false, false)
INITIALIZE_PASS_END(BanDump, "bandump", "Inject anti-dump protection at program start", false, false)
