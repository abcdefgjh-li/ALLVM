//===- TimeDetect.cpp - 时间差调试检测注入Pass -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 本文件实现时间差调试检测注入Pass，在程序入口点注入检测代码
// 通过测量代码执行时间检测调试器（调试时执行会变慢）
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/TimeDetect.h"
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

#define DEBUG_TYPE "timedetect"

using namespace llvm;

namespace {

struct TimeDetect : public ModulePass {
    static char ID;

    TimeDetect() : ModulePass(ID) {
        initializeTimeDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"TimeDetect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createTimeCheckFunc(Module &M, Function *ReportAndKillFunc);
};

}

char TimeDetect::ID = 0;

Function* TimeDetect::createTimeCheckFunc(Module &M, Function *ReportAndKillFunc) {
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
        "time_check",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *LoopBB = BasicBlock::Create(Ctx, "loop", Func);
    BasicBlock *CheckBB = BasicBlock::Create(Ctx, "check", Func);
    BasicBlock *FoundBB = BasicBlock::Create(Ctx, "found", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    Type *TimevalTy = StructType::create(Ctx, "timeval");
    StructType *TimevalStruct = cast<StructType>(TimevalTy);
    TimevalStruct->setBody({Int64Ty, Int64Ty});
    
    FunctionCallee GettimeofdayFunc = M.getOrInsertFunction(
        "gettimeofday",
        FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy}, false)
    );
    
    Value *StartTv = Builder.CreateAlloca(TimevalStruct, nullptr, "start_tv");
    Value *StartTvPtr = Builder.CreateBitCast(StartTv, CharPtrTy);
    Builder.CreateCall(GettimeofdayFunc, {StartTvPtr, ConstantPointerNull::get(CharPtrTy)});
    
    Builder.CreateBr(LoopBB);
    
    Builder.SetInsertPoint(LoopBB);
    PHINode *Counter = Builder.CreatePHI(Int32Ty, 2, "i");
    Counter->addIncoming(ConstantInt::get(Int32Ty, 0), EntryBB);
    
    Value *NextCounter = Builder.CreateAdd(Counter, ConstantInt::get(Int32Ty, 1));
    Value *Continue = Builder.CreateICmpSLT(NextCounter, ConstantInt::get(Int32Ty, 1000000));
    Counter->addIncoming(NextCounter, LoopBB);
    Builder.CreateCondBr(Continue, LoopBB, CheckBB);
    
    Builder.SetInsertPoint(CheckBB);
    
    Value *EndTv = Builder.CreateAlloca(TimevalStruct, nullptr, "end_tv");
    Value *EndTvPtr = Builder.CreateBitCast(EndTv, CharPtrTy);
    Builder.CreateCall(GettimeofdayFunc, {EndTvPtr, ConstantPointerNull::get(CharPtrTy)});
    
    Value *StartTvSecPtr = Builder.CreateGEP(TimevalStruct, StartTv, {ConstantInt::get(Int32Ty, 0), ConstantInt::get(Int32Ty, 0)});
    Value *StartTvUsecPtr = Builder.CreateGEP(TimevalStruct, StartTv, {ConstantInt::get(Int32Ty, 0), ConstantInt::get(Int32Ty, 1)});
    Value *EndTvSecPtr = Builder.CreateGEP(TimevalStruct, EndTv, {ConstantInt::get(Int32Ty, 0), ConstantInt::get(Int32Ty, 0)});
    Value *EndTvUsecPtr = Builder.CreateGEP(TimevalStruct, EndTv, {ConstantInt::get(Int32Ty, 0), ConstantInt::get(Int32Ty, 1)});
    
    Value *StartSec = Builder.CreateLoad(Int64Ty, StartTvSecPtr);
    Value *StartUsec = Builder.CreateLoad(Int64Ty, StartTvUsecPtr);
    Value *EndSec = Builder.CreateLoad(Int64Ty, EndTvSecPtr);
    Value *EndUsec = Builder.CreateLoad(Int64Ty, EndTvUsecPtr);
    
    Value *SecDiff = Builder.CreateSub(EndSec, StartSec);
    Value *UsecDiff = Builder.CreateSub(EndUsec, StartUsec);
    Value *SecToUsec = Builder.CreateMul(SecDiff, ConstantInt::get(Int64Ty, 1000000));
    Value *Elapsed = Builder.CreateAdd(SecToUsec, UsecDiff);
    
    // 阈值100000微秒（100ms），超过则认为被调试
    Value *TooSlow = Builder.CreateICmpSGT(Elapsed, ConstantInt::get(Int64Ty, 100000));
    Builder.CreateCondBr(TooSlow, FoundBB, ExitBB);
    
    Builder.SetInsertPoint(FoundBB);
    Builder.CreateCall(ReportAndKillFunc);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool TimeDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] TimeDetect: Injecting time detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 使用公共模块创建报告函数
    Function *ReportAndKillFunc = DetectUtils::createReportAndKillFunc(M, "Time-based Debugger");
    
    // 创建时间检测函数
    Function *CheckFunc = createTimeCheckFunc(M, ReportAndKillFunc);
    
    // 配置选项
    DetectOptions opts = DetectOptions::create(false);
    
    // 注入到main函数
    return DetectUtils::injectToMain(M, CheckFunc, opts);
}

ModulePass *llvm::createTimeDetectPass() {
    return new TimeDetect();
}

INITIALIZE_PASS_BEGIN(TimeDetect, "timedetect", "Inject time-based debugger detection at program start", false, false)
INITIALIZE_PASS_END(TimeDetect, "timedetect", "Inject time-based debugger detection at program start", false, false)
