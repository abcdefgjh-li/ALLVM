//===- PtraceDetect.cpp - Ptrace调试检测注入Pass ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 本文件实现Ptrace调试检测注入Pass，在程序入口点注入检测代码
// 检测ptrace附加调试
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/PtraceDetect.h"
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

#define DEBUG_TYPE "ptracedetect"

using namespace llvm;

namespace {

struct PtraceDetect : public ModulePass {
    static char ID;

    PtraceDetect() : ModulePass(ID) {
        initializePtraceDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"PtraceDetect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createPtraceCheckFunc(Module &M, Function *reportFunc);
};

}

char PtraceDetect::ID = 0;

Function* PtraceDetect::createPtraceCheckFunc(Module &M, Function *reportFunc) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "check_ptrace",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *PtraceOkBB = BasicBlock::Create(Ctx, "ptrace_ok", Func);
    BasicBlock *DebuggedBB = BasicBlock::Create(Ctx, "debugged", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    // 尝试ptrace自己
    FunctionCallee PtraceFunc = M.getOrInsertFunction(
        "ptrace",
        FunctionType::get(Int64Ty, {Int64Ty, Int64Ty, Int64Ty, Int64Ty}, false)
    );
    
    // PTRACE_TRACEME = 0
    Value *PtraceRet = Builder.CreateCall(PtraceFunc, {
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0)
    });
    
    // ptrace返回-1表示失败（已经被调试）
    Value *PtraceFailed = Builder.CreateICmpEQ(PtraceRet, ConstantInt::get(Int64Ty, -1));
    Builder.CreateCondBr(PtraceFailed, DebuggedBB, PtraceOkBB);
    
    Builder.SetInsertPoint(PtraceOkBB);
    
    // 再次ptrace自己（双重检测）
    Value *PtraceRet2 = Builder.CreateCall(PtraceFunc, {
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0)
    });
    
    Value *PtraceFailed2 = Builder.CreateICmpEQ(PtraceRet2, ConstantInt::get(Int64Ty, -1));
    Builder.CreateCondBr(PtraceFailed2, DebuggedBB, ExitBB);
    
    Builder.SetInsertPoint(DebuggedBB);
    Builder.CreateCall(reportFunc);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool PtraceDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] PtraceDetect: Injecting ptrace detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 使用公共模块创建报告函数
    Function *ReportFunc = DetectUtils::createReportAndKillFunc(M);
    
    // 创建Ptrace检测函数
    Function *CheckFunc = createPtraceCheckFunc(M, ReportFunc);
    
    // 配置选项
    DetectOptions opts = DetectOptions::create(false);
    
    // 注入到main函数
    return DetectUtils::injectToMain(M, CheckFunc, opts);
}

ModulePass *llvm::createPtraceDetectPass() {
    return new PtraceDetect();
}

INITIALIZE_PASS_BEGIN(PtraceDetect, "ptracedetect", "Inject ptrace debugger detection at program start", false, false)
INITIALIZE_PASS_END(PtraceDetect, "ptracedetect", "Inject ptrace debugger detection at program start", false, false)
