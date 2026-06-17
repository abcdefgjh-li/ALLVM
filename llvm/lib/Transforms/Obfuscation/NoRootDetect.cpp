//===- NoRootDetect.cpp - 无Root检测注入Pass -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 检测到没有root权限时退出，打印"请以root运行!!!"
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/NoRootDetect.h"
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

#define DEBUG_TYPE "norootdetect"

using namespace llvm;

namespace {

struct NoRootDetect : public ModulePass {
    static char ID;

    NoRootDetect() : ModulePass(ID) {
        initializeNoRootDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"NoRootDetect"};
    }

    bool runOnModule(Module &M) override;
};

}

char NoRootDetect::ID = 0;

bool NoRootDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] NoRootDetect: Entering runOnModule\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc) {
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[DEBUG] NoRootDetect: No main function found, exiting\n";
        }
        return false;
    }

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] NoRootDetect: Found main function: " << MainFunc->getName() << "\n";
    }

    if (MainFunc->isDeclaration()) {
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[DEBUG] NoRootDetect: main is declaration only, exiting\n";
        }
        return false;
    }

    if (MainFunc->empty()) {
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[DEBUG] NoRootDetect: main has no body, exiting\n";
        }
        return false;
    }

    LLVMContext &Ctx = M.getContext();
    BasicBlock &EntryBB = MainFunc->getEntryBlock();

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] NoRootDetect: Splitting entry block...\n";
    }

    BasicBlock *ContinueBB = EntryBB.splitBasicBlock(EntryBB.getFirstInsertionPt(), "noroot_continue");

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] NoRootDetect: EntryBB=" << EntryBB.getName()
               << " ContinueBB=" << ContinueBB->getName() << "\n";
    }

    IRBuilder<> Builder(&EntryBB, EntryBB.getFirstInsertionPt());

    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] NoRootDetect: Declaring external functions...\n";
    }

    FunctionCallee GetuidFunc = M.getOrInsertFunction(
        "getuid",
        FunctionType::get(Int32Ty, {}, false)
    );

    FunctionCallee PrintfFunc = M.getOrInsertFunction(
        "printf",
        FunctionType::get(Int32Ty, {CharPtrTy}, true)
    );

    FunctionCallee FflushFunc = M.getOrInsertFunction(
        "fflush",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );

    FunctionCallee ExitFunc = M.getOrInsertFunction(
        "_exit",
        FunctionType::get(VoidTy, {Int32Ty}, false)
    );

    if (isIRObfuscationDebugEnabled()) {
        Constant *DebugStr = ConstantDataArray::getString(Ctx, "[DEBUG] NoRootDetect: Checking no-root...\n");
        GlobalVariable *DebugGV = new GlobalVariable(
            M, DebugStr->getType(), true,
            GlobalValue::PrivateLinkage, DebugStr,
            ".noroot.debug"
        );
        Builder.CreateCall(PrintfFunc, {ConstantExpr::getBitCast(DebugGV, CharPtrTy)});
    }

    BasicBlock *CheckBB = BasicBlock::Create(Ctx, "noroot_check", MainFunc);
    BasicBlock *NoRootFoundBB = BasicBlock::Create(Ctx, "noroot_found", MainFunc);

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] NoRootDetect: Creating BBs: CheckBB=" << CheckBB->getName()
               << " NoRootFoundBB=" << NoRootFoundBB->getName() << "\n";
    }

    Builder.CreateBr(CheckBB);
    EntryBB.getTerminator()->eraseFromParent();

    Builder.SetInsertPoint(CheckBB);
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] NoRootDetect: Building getuid() check in CheckBB\n";
    }
    Value *Uid = Builder.CreateCall(GetuidFunc);
    Value *IsNotRoot = Builder.CreateICmpNE(Uid, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(IsNotRoot, NoRootFoundBB, ContinueBB);

    Builder.SetInsertPoint(NoRootFoundBB);
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] NoRootDetect: Building no-root exit in NoRootFoundBB\n";
    }
    // 使用DetectUtils的报告函数
    Function *ReportFunc = DetectUtils::createReportAndKillFunc(M, "No Root Access");
    Builder.CreateCall(ReportFunc);
    Builder.CreateUnreachable();

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] NoRootDetect: Insertion complete, returning true\n";
    }

    return true;
}

ModulePass *llvm::createNoRootDetectPass() {
    return new NoRootDetect();
}

INITIALIZE_PASS_BEGIN(NoRootDetect, "norootdetect", "Inject no-root detection at main", false, false)
INITIALIZE_PASS_END(NoRootDetect, "norootdetect", "Inject no-root detection at main", false, false)
