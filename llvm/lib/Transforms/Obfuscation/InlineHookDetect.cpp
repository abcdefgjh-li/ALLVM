//===- InlineHookDetect.cpp - Inline Hook检测注入Pass --------===//
//
//                     The LLVM Compiler Infrastructure
//
// 本文件实现Inline Hook检测注入Pass，在程序入口点注入检测代码
// 检测函数开头的跳转指令（Hook特征）
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/InlineHookDetect.h"
#include "llvm/Transforms/Obfuscation/DetectUtils.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "inlinehookdetect"

using namespace llvm;

namespace {

struct InlineHookDetect : public ModulePass {
    static char ID;

    InlineHookDetect() : ModulePass(ID) {
        initializeInlineHookDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"InlineHookDetect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createInlineHookCheckFunc(Module &M, Function *reportFunc);
};

}

char InlineHookDetect::ID = 0;

Function* InlineHookDetect::createInlineHookCheckFunc(Module &M, Function *reportFunc) {
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
        "inline_hook_check",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *CheckLibcBB = BasicBlock::Create(Ctx, "check_libc", Func);
    BasicBlock *FoundHookBB = BasicBlock::Create(Ctx, "found_hook", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    FunctionCallee DlopenFunc = M.getOrInsertFunction(
        "dlopen",
        FunctionType::get(CharPtrTy, {CharPtrTy, Int32Ty}, false)
    );
    
    FunctionCallee DlsymFunc = M.getOrInsertFunction(
        "dlsym",
        FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy}, false)
    );
    
    auto makeString = [&](const char *str) -> Constant* {
        return DetectUtils::createGlobalString(M, str, ".hook.str");
    };
    
    Constant *LibcPath = makeString("libc.so");
    Constant *OpenName = makeString("open");
    
    Value *RtldNoLoad = ConstantInt::get(Int32Ty, 4);
    Value *LibcHandle = Builder.CreateCall(DlopenFunc, {LibcPath, RtldNoLoad});
    
    Value *HandleNull = Builder.CreateICmpEQ(LibcHandle, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(HandleNull, ExitBB, CheckLibcBB);
    
    Builder.SetInsertPoint(CheckLibcBB);
    
    Value *OpenAddr = Builder.CreateCall(DlsymFunc, {LibcHandle, OpenName});
    
    Value *AddrNull = Builder.CreateICmpEQ(OpenAddr, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(AddrNull, ExitBB, FoundHookBB);
    
    // 读取函数开头的字节
    Builder.SetInsertPoint(FoundHookBB);
    
    Type *BytesTy = ArrayType::get(Int8Ty, 16);
    Value *Bytes = Builder.CreateAlloca(BytesTy, nullptr, "bytes");
    
    // 使用内联汇编读取内存（ARM64）
    // 检测特征：
    // B指令：0x17xxxxxx 或 0x14xxxxxx
    // LDR + BR：组合跳转
    
    FunctionCallee MemcpyFunc = M.getOrInsertFunction(
        "memcpy",
        FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy, Int64Ty}, false)
    );
    
    Value *BytesPtr = Builder.CreateBitCast(Bytes, CharPtrTy);
    Builder.CreateCall(MemcpyFunc, {BytesPtr, OpenAddr, ConstantInt::get(Int64Ty, 16)});
    
    // 读取前4个字节
    Value *Byte0 = Builder.CreateLoad(Int8Ty, BytesPtr);
    Value *Byte1 = Builder.CreateGEP(Int8Ty, BytesPtr, ConstantInt::get(Int64Ty, 1));
    Value *Byte1Val = Builder.CreateLoad(Int8Ty, Byte1);
    Value *Byte2 = Builder.CreateGEP(Int8Ty, BytesPtr, ConstantInt::get(Int64Ty, 2));
    Value *Byte2Val = Builder.CreateLoad(Int8Ty, Byte2);
    Value *Byte3 = Builder.CreateGEP(Int8Ty, BytesPtr, ConstantInt::get(Int64Ty, 3));
    Value *Byte3Val = Builder.CreateLoad(Int8Ty, Byte3);
    
    // 组合成32位指令
    Value *Instr = Builder.CreateZExt(Byte0, Int32Ty);
    Value *Tmp1 = Builder.CreateShl(Builder.CreateZExt(Byte1Val, Int32Ty), ConstantInt::get(Int32Ty, 8));
    Value *Tmp2 = Builder.CreateShl(Builder.CreateZExt(Byte2Val, Int32Ty), ConstantInt::get(Int32Ty, 16));
    Value *Tmp3 = Builder.CreateShl(Builder.CreateZExt(Byte3Val, Int32Ty), ConstantInt::get(Int32Ty, 24));
    Instr = Builder.CreateOr(Instr, Tmp1);
    Instr = Builder.CreateOr(Instr, Tmp2);
    Instr = Builder.CreateOr(Instr, Tmp3);
    
    // 检测B指令 (0x14xxxxxx 或 0x17xxxxxx)
    Value *Mask = ConstantInt::get(Int32Ty, 0xFC000000);
    Value *BInstr = ConstantInt::get(Int32Ty, 0x14000000);
    Value *BInstrAlt = ConstantInt::get(Int32Ty, 0x17000000);
    
    Value *Masked = Builder.CreateAnd(Instr, Mask);
    Value *IsB = Builder.CreateICmpEQ(Masked, BInstr);
    Value *IsBAlt = Builder.CreateICmpEQ(Masked, BInstrAlt);
    Value *IsHook = Builder.CreateOr(IsB, IsBAlt);
    
    BasicBlock *ReportBB = BasicBlock::Create(Ctx, "report", Func);
    Builder.CreateCondBr(IsHook, ReportBB, ExitBB);
    
    Builder.SetInsertPoint(ReportBB);
    Builder.CreateCall(reportFunc);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool InlineHookDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] InlineHookDetect: Injecting inline hook detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 使用公共模块创建报告函数
    Function *ReportFunc = DetectUtils::createReportAndKillFunc(M);
    
    // 创建检测函数
    Function *CheckFunc = createInlineHookCheckFunc(M, ReportFunc);
    
    // 配置选项
    DetectOptions opts = DetectOptions::create(false);
    
    // 注入到main函数
    return DetectUtils::injectToMain(M, CheckFunc, opts);
}

ModulePass *llvm::createInlineHookDetectPass() {
    return new InlineHookDetect();
}

INITIALIZE_PASS_BEGIN(InlineHookDetect, "inlinehookdetect", "Inject inline hook detection at program start", false, false)
INITIALIZE_PASS_END(InlineHookDetect, "inlinehookdetect", "Inject inline hook detection at program start", false, false)
