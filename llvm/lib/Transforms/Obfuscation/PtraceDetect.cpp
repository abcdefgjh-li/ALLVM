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
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
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
    BasicBlock *CheckStatusBB = BasicBlock::Create(Ctx, "check_status", Func);
    BasicBlock *DebuggedBB = BasicBlock::Create(Ctx, "debugged", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    // 方法1: 检查 /proc/self/status 中的 TracerPid
    // 这是更可靠的检测方式
    FunctionCallee FopenFunc = M.getOrInsertFunction(
        "fopen",
        FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy}, false)
    );
    
    FunctionCallee FgetsFunc = M.getOrInsertFunction(
        "fgets",
        FunctionType::get(CharPtrTy, {CharPtrTy, Int32Ty, CharPtrTy}, false)
    );
    
    FunctionCallee FcloseFunc = M.getOrInsertFunction(
        "fclose",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    
    FunctionCallee StrncmpFunc = M.getOrInsertFunction(
        "strncmp",
        FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy, Int64Ty}, false)
    );
    
    FunctionCallee AtoiFunc = M.getOrInsertFunction(
        "atoi",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    
    auto makeString = [&](const char *str) -> Constant* {
        Constant *StrConst = ConstantDataArray::getString(Ctx, str);
        GlobalVariable *StrGV = new GlobalVariable(
            M, StrConst->getType(), true,
            GlobalValue::PrivateLinkage, StrConst,
            ".ptrace.str"
        );
        return ConstantExpr::getBitCast(StrGV, CharPtrTy);
    };
    
    Constant *StatusPath = makeString("/proc/self/status");
    Constant *ReadMode = makeString("r");
    Constant *TracerPidStr = makeString("TracerPid:");
    
    // 打开 /proc/self/status
    Value *Fp = Builder.CreateCall(FopenFunc, {StatusPath, ReadMode});
    Value *FpNotNull = Builder.CreateICmpNE(Fp, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(FpNotNull, CheckStatusBB, ExitBB);
    
    Builder.SetInsertPoint(CheckStatusBB);
    
    // 读取每一行
    Type *BufTy = ArrayType::get(Type::getInt8Ty(Ctx), 256);
    Value *Buf = Builder.CreateAlloca(BufTy, nullptr, "buf");
    Value *BufPtr = Builder.CreateBitCast(Buf, CharPtrTy);
    
    // 循环读取
    BasicBlock *LoopBB = BasicBlock::Create(Ctx, "loop", Func);
    BasicBlock *FoundBB = BasicBlock::Create(Ctx, "found", Func);
    BasicBlock *CheckValueBB = BasicBlock::Create(Ctx, "check_value", Func);
    
    Builder.CreateBr(LoopBB);
    
    Builder.SetInsertPoint(LoopBB);
    Value *Line = Builder.CreateCall(FgetsFunc, {BufPtr, ConstantInt::get(Int32Ty, 256), Fp});
    Value *LineNotNull = Builder.CreateICmpNE(Line, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(LineNotNull, FoundBB, ExitBB);
    
    Builder.SetInsertPoint(FoundBB);
    // 检查是否是 TracerPid 行
    Value *CmpResult = Builder.CreateCall(StrncmpFunc, {BufPtr, TracerPidStr, ConstantInt::get(Int64Ty, 10)});
    Value *IsTracerPidLine = Builder.CreateICmpEQ(CmpResult, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(IsTracerPidLine, CheckValueBB, LoopBB);
    
    Builder.SetInsertPoint(CheckValueBB);
    // 获取 TracerPid 的值（跳过 "TracerPid:" 前缀）
    Value *ValuePtr = Builder.CreateConstGEP1_64(BufTy, Buf, 10);
    Value *ValuePtrChar = Builder.CreateBitCast(ValuePtr, CharPtrTy);
    Value *TracerPid = Builder.CreateCall(AtoiFunc, {ValuePtrChar});
    
    // 如果 TracerPid != 0，说明被调试
    Value *IsDebugged = Builder.CreateICmpNE(TracerPid, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(IsDebugged, DebuggedBB, ExitBB);
    
    Builder.SetInsertPoint(DebuggedBB);
    Builder.CreateCall(reportFunc);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    // 关闭文件
    Builder.CreateCall(FcloseFunc, {Fp});
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
