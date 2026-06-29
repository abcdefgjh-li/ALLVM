//===- RootDetect.cpp - Root检测注入Pass -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 检测到有root权限时退出
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/RootDetect.h"
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

#define DEBUG_TYPE "rootdetect"

using namespace llvm;

namespace {

struct RootDetect : public ModulePass {
    static char ID;

    RootDetect() : ModulePass(ID) {
        initializeRootDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"RootDetect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createRootCheckFunc(Module &M, Function *reportFunc);
};

}

char RootDetect::ID = 0;

Function* RootDetect::createRootCheckFunc(Module &M, Function *reportFunc) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "check_root",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *RootFoundBB = BasicBlock::Create(Ctx, "root_found", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    // 调用getuid()检测root
    FunctionCallee GetuidFunc = M.getOrInsertFunction(
        "getuid",
        FunctionType::get(Int32Ty, {}, false)
    );
    
    Value *Uid = Builder.CreateCall(GetuidFunc);
    Value *IsRoot = Builder.CreateICmpEQ(Uid, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(IsRoot, RootFoundBB, ExitBB);
    
    Builder.SetInsertPoint(RootFoundBB);
    Builder.CreateCall(reportFunc);
    Builder.CreateUnreachable();
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool RootDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] RootDetect: Injecting root detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 使用公共模块创建报告函数
    Function *ReportFunc = DetectUtils::createReportAndKillFunc(M, "Root Access");
    
    // 创建Root检测函数
    Function *CheckFunc = createRootCheckFunc(M, ReportFunc);
    
    // 配置选项
    DetectOptions opts = DetectOptions::create(false);
    
    // 注入到main函数
    return DetectUtils::injectToMain(M, CheckFunc, opts);
}

ModulePass *llvm::createRootDetectPass() {
    return new RootDetect();
}

INITIALIZE_PASS_BEGIN(RootDetect, "rootdetect", "Inject root detection at main", false, false)
INITIALIZE_PASS_END(RootDetect, "rootdetect", "Inject root detection at main", false, false)
