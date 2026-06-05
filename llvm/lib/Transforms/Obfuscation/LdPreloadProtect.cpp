//===- LdPreloadProtect.cpp - LD_PRELOAD检测注入Pass --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现LD_PRELOAD检测注入Pass，在程序入口点注入检测代码
// 一键注入，无需额外源文件
// 使用直接读取 /proc/self/environ 方式，绕过 getenv hook
// 使用 ARM64 汇编强制终止进程
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/LdPreloadProtect.h"
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

#define DEBUG_TYPE "ldpreloadprotect"

using namespace llvm;

namespace {

struct LdPreloadProtect : public ModulePass {
    static char ID;

    LdPreloadProtect() : ModulePass(ID) {
        initializeLdPreloadProtectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"LdPreloadProtect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createReportAndKillFunc(Module &M);
    Function* createCheckFunc(Module &M, Function *ReportAndKillFunc);
};

}

char LdPreloadProtect::ID = 0;

Function* LdPreloadProtect::createReportAndKillFunc(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "ldpreload_report_and_kill",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);
    
    FunctionCallee PrintfFunc = M.getOrInsertFunction(
        "printf",
        FunctionType::get(Int32Ty, {CharPtrTy}, true)
    );
    
    FunctionCallee FflushFunc = M.getOrInsertFunction(
        "fflush",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    
    Constant *MsgStr = ConstantDataArray::getString(Ctx, "你就滚你妈的就完事了!!!\n");
    GlobalVariable *MsgGV = new GlobalVariable(
        M,
        MsgStr->getType(),
        true,
        GlobalValue::PrivateLinkage,
        MsgStr,
        ".ldpreload.msg"
    );
    MsgGV->setSection(".AProtect.rodata");
    Constant *MsgPtr = ConstantExpr::getBitCast(MsgGV, CharPtrTy);
    
    Builder.CreateCall(PrintfFunc, {MsgPtr});
    
    Constant *NullPtr = ConstantPointerNull::get(CharPtrTy);
    Builder.CreateCall(FflushFunc, {NullPtr});
    
    FunctionCallee GetpidFunc = M.getOrInsertFunction(
        "getpid",
        FunctionType::get(Int32Ty, {}, false)
    );
    
    FunctionCallee KillFunc = M.getOrInsertFunction(
        "kill",
        FunctionType::get(Int32Ty, {Int32Ty, Int32Ty}, false)
    );
    
    Value *Pid = Builder.CreateCall(GetpidFunc);
    Value *Sigkill = ConstantInt::get(Int32Ty, 9);
    Builder.CreateCall(KillFunc, {Pid, Sigkill});
    
    FunctionCallee AsmExitFunc = M.getOrInsertFunction("exit",
        FunctionType::get(Type::getVoidTy(M.getContext()), {Int32Ty}, false));
    Builder.CreateCall(AsmExitFunc, {ConstantInt::get(Int32Ty, 0)});
    
    Builder.CreateUnreachable();
    
    return Func;
}

Function* LdPreloadProtect::createCheckFunc(Module &M, Function *ReportAndKillFunc) {
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
        "ldpreload_check",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *ReadBB = BasicBlock::Create(Ctx, "read", Func);
    BasicBlock *SearchBB = BasicBlock::Create(Ctx, "search", Func);
    BasicBlock *FoundBB = BasicBlock::Create(Ctx, "found", Func);
    BasicBlock *NotFoundBB = BasicBlock::Create(Ctx, "notfound", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    FunctionCallee OpenFunc = M.getOrInsertFunction(
        "open",
        FunctionType::get(Int32Ty, {CharPtrTy, Int32Ty}, false)
    );
    
    Constant *EnvironPath = ConstantDataArray::getString(Ctx, "/proc/self/environ");
    GlobalVariable *EnvironPathGV = new GlobalVariable(
        M,
        EnvironPath->getType(),
        true,
        GlobalValue::PrivateLinkage,
        EnvironPath,
        ".environ.path"
    );
    EnvironPathGV->setSection(".AProtect.rodata");
    Constant *EnvironPathPtr = ConstantExpr::getBitCast(EnvironPathGV, CharPtrTy);
    
    Value *O_RDONLY = ConstantInt::get(Int32Ty, 0);
    Value *Fd = Builder.CreateCall(OpenFunc, {EnvironPathPtr, O_RDONLY});
    
    Value *FdNeg = Builder.CreateICmpSLT(Fd, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(FdNeg, NotFoundBB, ReadBB);
    
    Builder.SetInsertPoint(ReadBB);
    
    FunctionCallee ReadFunc = M.getOrInsertFunction(
        "read",
        FunctionType::get(Int64Ty, {Int32Ty, CharPtrTy, Int64Ty}, false)
    );
    
    FunctionCallee CloseFunc = M.getOrInsertFunction(
        "close",
        FunctionType::get(Int32Ty, {Int32Ty}, false)
    );
    
    Constant *LdPreloadNeedle = ConstantDataArray::getString(Ctx, "LD_PRELOAD=");
    GlobalVariable *LdPreloadNeedleGV = new GlobalVariable(
        M,
        LdPreloadNeedle->getType(),
        true,
        GlobalValue::PrivateLinkage,
        LdPreloadNeedle,
        ".ldpreload.needle"
    );
    LdPreloadNeedleGV->setSection(".AProtect.rodata");
    Constant *NeedlePtr = ConstantExpr::getBitCast(LdPreloadNeedleGV, CharPtrTy);

    Type *ByteArrayTy = ArrayType::get(Type::getInt8Ty(Ctx), 8192);
    Constant *ZeroInit = Constant::getNullValue(ByteArrayTy);
    GlobalVariable *BufferGV = new GlobalVariable(
        M,
        ByteArrayTy,
        false,
        GlobalValue::PrivateLinkage,
        ZeroInit,
        ".environ.buf"
    );
    BufferGV->setSection(".AProtect.bss");
    Constant *BufferPtr = ConstantExpr::getBitCast(BufferGV, CharPtrTy);
    
    Value *BufSize = ConstantInt::get(Int64Ty, 8192);
    Value *BytesRead = Builder.CreateCall(ReadFunc, {Fd, BufferPtr, BufSize});
    
    Builder.CreateCall(CloseFunc, {Fd});
    
    Value *ReadFailed = Builder.CreateICmpSLE(BytesRead, ConstantInt::get(Int64Ty, 0));
    Builder.CreateCondBr(ReadFailed, NotFoundBB, SearchBB);
    
    Builder.SetInsertPoint(SearchBB);
    
    FunctionCallee MemmemFunc = M.getOrInsertFunction(
        "memmem",
        FunctionType::get(CharPtrTy, {CharPtrTy, Int64Ty, CharPtrTy, Int64Ty}, false)
    );
    
    Value *NeedleLen = ConstantInt::get(Int64Ty, 10);
    Value *Found = Builder.CreateCall(MemmemFunc, {BufferPtr, BytesRead, NeedlePtr, NeedleLen});
    
    Value *FoundNotNull = Builder.CreateICmpNE(Found, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(FoundNotNull, FoundBB, NotFoundBB);
    
    Builder.SetInsertPoint(FoundBB);
    Builder.CreateCall(ReportAndKillFunc);
    Builder.CreateBr(NotFoundBB);
    
    Builder.SetInsertPoint(NotFoundBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool LdPreloadProtect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] LdPreloadProtect: Injecting LD_PRELOAD detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration()) {
        return false;
    }

    if (MainFunc->empty()) {
        return false;
    }

    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] LdPreloadProtect: Injecting LD_PRELOAD detection\n";
    }

    BasicBlock &EntryBB = MainFunc->getEntryBlock();
    
    LLVMContext &Ctx = M.getContext();

    Function *ReportAndKillFunc = createReportAndKillFunc(M);
    Function *CheckFunc = createCheckFunc(M, ReportAndKillFunc);

    IRBuilder<> Builder(&EntryBB, EntryBB.getFirstInsertionPt());
    
    if (isIRObfuscationDebugEnabled()) {
        FunctionCallee PrintfFunc = M.getOrInsertFunction(
            "printf",
            FunctionType::get(Type::getInt32Ty(Ctx), {PointerType::get(Ctx, 0)}, true)
        );
        Constant *DebugStr = ConstantDataArray::getString(Ctx, "[DEBUG] LdPreloadProtect: Checking LD_PRELOAD...\n");
        GlobalVariable *DebugGV = new GlobalVariable(
            M, DebugStr->getType(), true,
            GlobalValue::PrivateLinkage, DebugStr,
            ".ldpreload.debug"
        );
        DebugGV->setSection(".AProtect.rodata");
        Builder.CreateCall(PrintfFunc, {ConstantExpr::getBitCast(DebugGV, PointerType::get(Ctx, 0))});
    }
    
    Builder.CreateCall(CheckFunc);

    return true;
}

ModulePass *llvm::createLdPreloadProtectPass() {
    return new LdPreloadProtect();
}

INITIALIZE_PASS_BEGIN(LdPreloadProtect, "ldpreloadprotect", "Inject LD_PRELOAD detection at program start", false, false)
INITIALIZE_PASS_END(LdPreloadProtect, "ldpreloadprotect", "Inject LD_PRELOAD detection at program start", false, false)
