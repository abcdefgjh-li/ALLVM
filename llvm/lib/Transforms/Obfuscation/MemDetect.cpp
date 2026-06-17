//===- MemDetect.cpp - 内存检测注入Pass -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现内存检测注入Pass，在程序入口点注入检测代码
// 使用mincore系统调用检测内存页驻留状态，检测调试器
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/MemDetect.h"
#include "llvm/Transforms/Obfuscation/DetectUtils.h"
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
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"

#define DEBUG_TYPE "memdetect"

using namespace llvm;

namespace {

struct MemDetect : public ModulePass {
    static char ID;

    MemDetect() : ModulePass(ID) {
        initializeMemDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"MemDetect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createMemCheckFunc(Module &M, Function *ReportAndKillFunc);
};

}

char MemDetect::ID = 0;

Function* MemDetect::createMemCheckFunc(Module &M, Function *ReportAndKillFunc) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int8Ty = Type::getInt8Ty(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "mem_check",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *CheckBB = BasicBlock::Create(Ctx, "check", Func);
    BasicBlock *LoopBB = BasicBlock::Create(Ctx, "loop", Func);
    BasicBlock *FoundBB = BasicBlock::Create(Ctx, "found", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    FunctionCallee SysconfFunc = M.getOrInsertFunction(
        "sysconf",
        FunctionType::get(Int64Ty, {Int32Ty}, false)
    );
    
    Value *PageSize = Builder.CreateCall(SysconfFunc, {ConstantInt::get(Int32Ty, 30)});
    
    Value *PageSizeValid = Builder.CreateICmpNE(PageSize, ConstantInt::get(Int64Ty, -1));
    BasicBlock *PageSizeOkBB = BasicBlock::Create(Ctx, "pagesize_ok", Func);
    BasicBlock *PageSizeFailBB = BasicBlock::Create(Ctx, "pagesize_fail", Func);
    Builder.CreateCondBr(PageSizeValid, PageSizeOkBB, PageSizeFailBB);
    
    Builder.SetInsertPoint(PageSizeFailBB);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(PageSizeOkBB);
    
    Type *VecTy = ArrayType::get(Int8Ty, 16);
    Value *Vec = Builder.CreateAlloca(VecTy, nullptr, "vec");
    Value *VecPtr = Builder.CreateBitCast(Vec, CharPtrTy);
    
    Value *TestAddr = Builder.CreateAlloca(Int8Ty, nullptr, "test_addr");
    Value *TestAddrPtr = Builder.CreateBitCast(TestAddr, CharPtrTy);
    
    FunctionCallee MincoreFunc = M.getOrInsertFunction(
        "mincore",
        FunctionType::get(Int32Ty, {CharPtrTy, Int64Ty, CharPtrTy}, false)
    );
    
    Value *MincoreRet = Builder.CreateCall(MincoreFunc, {TestAddrPtr, PageSize, VecPtr});
    
    Value *MincoreOk = Builder.CreateICmpEQ(MincoreRet, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(MincoreOk, CheckBB, ExitBB);
    
    Builder.SetInsertPoint(CheckBB);
    
    Builder.CreateBr(LoopBB);
    
    Builder.SetInsertPoint(LoopBB);
    PHINode *Idx = Builder.CreatePHI(Int32Ty, 2, "idx");
    Idx->addIncoming(ConstantInt::get(Int32Ty, 0), CheckBB);
    
    Value *VecElemPtr = Builder.CreateGEP(VecTy, Vec, {ConstantInt::get(Int32Ty, 0), Idx});
    Value *VecElem = Builder.CreateLoad(Int8Ty, VecElemPtr);
    Value *Resident = Builder.CreateAnd(VecElem, ConstantInt::get(Int8Ty, 1));
    Value *NotResident = Builder.CreateICmpEQ(Resident, ConstantInt::get(Int8Ty, 0));
    
    Value *NextIdx = Builder.CreateAdd(Idx, ConstantInt::get(Int32Ty, 1));
    Idx->addIncoming(NextIdx, LoopBB);
    
    Builder.CreateCondBr(NotResident, FoundBB, LoopBB);
    
    Builder.SetInsertPoint(FoundBB);
    Builder.CreateCall(ReportAndKillFunc);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool MemDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] MemDetect: Injecting memory detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration()) {
        return false;
    }

    if (MainFunc->empty()) {
        return false;
    }

    BasicBlock &EntryBB = MainFunc->getEntryBlock();
    
    Function *ReportAndKillFunc = DetectUtils::createReportAndKillFunc(M, "Memory Resident");
    Function *CheckFunc = createMemCheckFunc(M, ReportAndKillFunc);

    IRBuilder<> Builder(&EntryBB, EntryBB.getFirstInsertionPt());

    if (isIRObfuscationDebugEnabled()) {
        LLVMContext &Ctx = M.getContext();
        FunctionCallee PrintfFunc = M.getOrInsertFunction(
            "printf",
            FunctionType::get(Type::getInt32Ty(Ctx), {PointerType::get(Ctx, 0)}, true)
        );
        Constant *DebugStr = ConstantDataArray::getString(Ctx, "[DEBUG] MemDetect: Checking memory resident...\n");
        GlobalVariable *DebugGV = new GlobalVariable(
            M, DebugStr->getType(), true,
            GlobalValue::PrivateLinkage, DebugStr,
            ".mem.debug"
        );
        Builder.CreateCall(PrintfFunc, {ConstantExpr::getBitCast(DebugGV, PointerType::get(Ctx, 0))});
    }

    Builder.CreateCall(CheckFunc);

    return true;
}

ModulePass *llvm::createMemDetectPass() {
    return new MemDetect();
}

INITIALIZE_PASS_BEGIN(MemDetect, "memdetect", "Inject memory detection at program start", false, false)
INITIALIZE_PASS_END(MemDetect, "memdetect", "Inject memory detection at program start", false, false)
