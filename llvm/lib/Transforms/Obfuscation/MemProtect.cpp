//===- MemProtect.cpp - 内存保护注入Pass ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 本文件实现内存保护注入Pass，在程序入口点注入保护代码
// 使用mprotect保护代码段
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/MemProtect.h"
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

#define DEBUG_TYPE "memprotect"

using namespace llvm;

namespace {

struct MemProtect : public ModulePass {
    static char ID;

    MemProtect() : ModulePass(ID) {
        initializeMemProtectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"MemProtect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createMemProtectFunc(Module &M);
};

}

char MemProtect::ID = 0;

Function* MemProtect::createMemProtectFunc(Module &M) {
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
        "mem_protect",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *OpenOkBB = BasicBlock::Create(Ctx, "open_ok", Func);
    BasicBlock *OpenFailBB = BasicBlock::Create(Ctx, "open_fail", Func);
    BasicBlock *LoopBB = BasicBlock::Create(Ctx, "loop", Func);
    BasicBlock *CheckLineBB = BasicBlock::Create(Ctx, "check_line", Func);
    BasicBlock *ProtectBB = BasicBlock::Create(Ctx, "protect", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
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
    
    FunctionCallee StrstrFunc = M.getOrInsertFunction(
        "strstr",
        FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy}, false)
    );
    
    FunctionCallee SscanfFunc = M.getOrInsertFunction(
        "sscanf",
        FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy}, true)
    );
    
    FunctionCallee MprotectFunc = M.getOrInsertFunction(
        "mprotect",
        FunctionType::get(Int32Ty, {CharPtrTy, Int64Ty, Int32Ty}, false)
    );
    
    auto makeString = [&](const char *str) -> Constant* {
        return DetectUtils::createGlobalString(M, str, ".mem.str");
    };
    
    Constant *MapsPath = makeString("/proc/self/maps");
    Constant *ReadMode = makeString("r");
    Constant *SscanfFmt = makeString("%lx-%lx %4s %lx");

    Type *Int8Ty = Type::getInt8Ty(Ctx);
    
    Value *Fp = Builder.CreateCall(FopenFunc, {MapsPath, ReadMode});
    Value *FpNotNull = Builder.CreateICmpNE(Fp, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(FpNotNull, OpenOkBB, OpenFailBB);
    
    Builder.SetInsertPoint(OpenFailBB);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(OpenOkBB);
    
    Type *LineBufTy = ArrayType::get(Int8Ty, 512);
    Value *LineBuf = Builder.CreateAlloca(LineBufTy, nullptr, "linebuf");
    Value *LineBufPtr = Builder.CreateBitCast(LineBuf, CharPtrTy);
    
    Type *StartBufTy = ArrayType::get(Int64Ty, 1);
    Value *StartBuf = Builder.CreateAlloca(StartBufTy, nullptr, "start");
    Value *StartPtr = Builder.CreateBitCast(StartBuf, CharPtrTy);
    
    Type *EndBufTy = ArrayType::get(Int64Ty, 1);
    Value *EndBuf = Builder.CreateAlloca(EndBufTy, nullptr, "end");
    Value *EndPtr = Builder.CreateBitCast(EndBuf, CharPtrTy);
    
    Type *PermBufTy = ArrayType::get(Int8Ty, 8);
    Value *PermBuf = Builder.CreateAlloca(PermBufTy, nullptr, "perm");
    Value *PermPtr = Builder.CreateBitCast(PermBuf, CharPtrTy);
    
    Type *OffsetBufTy = ArrayType::get(Int64Ty, 1);
    Value *OffsetBuf = Builder.CreateAlloca(OffsetBufTy, nullptr, "offset");
    Value *OffsetPtr = Builder.CreateBitCast(OffsetBuf, CharPtrTy);
    
    Builder.CreateBr(LoopBB);
    
    Builder.SetInsertPoint(LoopBB);
    
    Value *Line = Builder.CreateCall(FgetsFunc, {LineBufPtr, ConstantInt::get(Int32Ty, 512), Fp});
    Value *LineNotNull = Builder.CreateICmpNE(Line, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(LineNotNull, CheckLineBB, ExitBB);
    
    Builder.SetInsertPoint(CheckLineBB);
    
    Builder.CreateCall(SscanfFunc, {LineBufPtr, SscanfFmt, StartPtr, EndPtr, PermPtr, OffsetPtr});
    
    Value *Start = Builder.CreateLoad(Int64Ty, StartPtr);
    Value *End = Builder.CreateLoad(Int64Ty, EndPtr);
    
    Value *PermChar2 = Builder.CreateGEP(Int8Ty, PermBuf, {ConstantInt::get(Int64Ty, 0), ConstantInt::get(Int64Ty, 2)});
    Value *ExecFlag = Builder.CreateLoad(Int8Ty, PermChar2);
    Value *IsExecutable = Builder.CreateICmpEQ(ExecFlag, ConstantInt::get(Int8Ty, 'x'));
    
    Value *Offset = Builder.CreateLoad(Int64Ty, OffsetPtr);
    Value *IsText = Builder.CreateICmpEQ(Offset, ConstantInt::get(Int64Ty, 0));
    
    Value *ShouldProtect = Builder.CreateAnd(IsExecutable, IsText);
    Builder.CreateCondBr(ShouldProtect, ProtectBB, LoopBB);
    
    Builder.SetInsertPoint(ProtectBB);
    
    Value *Size = Builder.CreateSub(End, Start);
    Value *Addr = Builder.CreateIntToPtr(Start, CharPtrTy);
    
    // PROT_READ = 1
    Builder.CreateCall(MprotectFunc, {Addr, Size, ConstantInt::get(Int32Ty, 1)});
    
    Builder.CreateBr(LoopBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateCall(FcloseFunc, {Fp});
    Builder.CreateRetVoid();
    
    return Func;
}

bool MemProtect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] MemProtect: Injecting memory protection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 创建保护函数
    Function *ProtectFunc = createMemProtectFunc(M);
    
    // 配置选项
    DetectOptions opts = DetectOptions::create(false);
    
    // 注入到main函数
    return DetectUtils::injectToMain(M, ProtectFunc, opts);
}

ModulePass *llvm::createMemProtectPass() {
    return new MemProtect();
}

INITIALIZE_PASS_BEGIN(MemProtect, "memprotect", "Inject memory protection at program start", false, false)
INITIALIZE_PASS_END(MemProtect, "memprotect", "Inject memory protection at program start", false, false)
