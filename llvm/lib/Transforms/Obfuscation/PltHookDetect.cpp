//===- PltHookDetect.cpp - PLT Hook检测注入Pass --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 本文件实现PLT Hook检测注入Pass，在程序入口点注入检测代码
// 检测GOT表中函数指针是否指向合法地址
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/PltHookDetect.h"
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

#define DEBUG_TYPE "plthookdetect"

using namespace llvm;

namespace {

struct PltHookDetect : public ModulePass {
    static char ID;

    PltHookDetect() : ModulePass(ID) {
        initializePltHookDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"PltHookDetect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createCheckPltHookFunc(Module &M, Function *ReportAndKillFunc);
};

}

char PltHookDetect::ID = 0;

Function* PltHookDetect::createCheckPltHookFunc(Module &M, Function *ReportAndKillFunc) {
    LLVMContext &Ctx = M.getContext();
    
    Type *Int8Ty = Type::getInt8Ty(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(Int32Ty, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "check_plt_hook",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *OpenOkBB = BasicBlock::Create(Ctx, "open_ok", Func);
    BasicBlock *OpenFailBB = BasicBlock::Create(Ctx, "open_fail", Func);
    BasicBlock *LoopBB = BasicBlock::Create(Ctx, "loop", Func);
    BasicBlock *CheckLineBB = BasicBlock::Create(Ctx, "check_line", Func);
    BasicBlock *CheckPermBB = BasicBlock::Create(Ctx, "check_perm", Func);
    BasicBlock *FoundLibcBB = BasicBlock::Create(Ctx, "found_libc", Func);
    BasicBlock *CheckHookBB = BasicBlock::Create(Ctx, "check_hook", Func);
    BasicBlock *HookedBB = BasicBlock::Create(Ctx, "hooked", Func);
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
    
    FunctionCallee DlopenFunc = M.getOrInsertFunction(
        "dlopen",
        FunctionType::get(CharPtrTy, {CharPtrTy, Int32Ty}, false)
    );
    
    FunctionCallee DlsymFunc = M.getOrInsertFunction(
        "dlsym",
        FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy}, false)
    );
    
    auto makeString = [&](const char *str) -> Constant* {
        return DetectUtils::createGlobalString(M, str, ".plthook.str");
    };
    
    Constant *MapsPath = makeString("/proc/self/maps");
    Constant *ReadMode = makeString("r");
    Constant *SscanfFmt = makeString("%lx-%lx %4s");
    Constant *LibcNeedle = makeString("libc.so");
    Constant *LibcPath = makeString("libc.so");
    Constant *OpenName = makeString("open");
    
    Value *Fp = Builder.CreateCall(FopenFunc, {MapsPath, ReadMode});
    Value *FpNotNull = Builder.CreateICmpNE(Fp, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(FpNotNull, OpenOkBB, OpenFailBB);
    
    Builder.SetInsertPoint(OpenFailBB);
    Builder.CreateRet(ConstantInt::get(Int32Ty, 0));
    
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
    
    Builder.CreateBr(LoopBB);
    
    Builder.SetInsertPoint(LoopBB);
    
    Value *Line = Builder.CreateCall(FgetsFunc, {LineBufPtr, ConstantInt::get(Int32Ty, 512), Fp});
    Value *LineNotNull = Builder.CreateICmpNE(Line, ConstantPointerNull::get(CharPtrTy));
    
    BasicBlock *CloseAndExitBB = BasicBlock::Create(Ctx, "close_exit", Func);
    Builder.CreateCondBr(LineNotNull, CheckLineBB, CloseAndExitBB);
    
    Builder.SetInsertPoint(CloseAndExitBB);
    Builder.CreateCall(FcloseFunc, {Fp});
    Builder.CreateRet(ConstantInt::get(Int32Ty, 0));
    
    Builder.SetInsertPoint(CheckLineBB);
    
    Value *HasLibc = Builder.CreateCall(StrstrFunc, {LineBufPtr, LibcNeedle});
    Value *HasLibcNotNull = Builder.CreateICmpNE(HasLibc, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(HasLibcNotNull, CheckPermBB, LoopBB);
    
    Builder.SetInsertPoint(CheckPermBB);
    
    Builder.CreateCall(SscanfFunc, {LineBufPtr, SscanfFmt, StartPtr, EndPtr, PermPtr});
    
    Value *PermChar2 = Builder.CreateGEP(Int8Ty, PermBuf, {ConstantInt::get(Int64Ty, 0), ConstantInt::get(Int64Ty, 2)});
    Value *ExecFlag = Builder.CreateLoad(Int8Ty, PermChar2);
    Value *IsExecutable = Builder.CreateICmpEQ(ExecFlag, ConstantInt::get(Int8Ty, 'x'));
    Builder.CreateCondBr(IsExecutable, FoundLibcBB, LoopBB);
    
    Builder.SetInsertPoint(FoundLibcBB);
    
    Value *LibcStart = Builder.CreateLoad(Int64Ty, StartPtr);
    Value *LibcEnd = Builder.CreateLoad(Int64Ty, EndPtr);
    
    Builder.CreateCall(FcloseFunc, {Fp});
    Builder.CreateBr(CheckHookBB);
    
    Builder.SetInsertPoint(CheckHookBB);
    
    Value *RtldNoLoad = ConstantInt::get(Int32Ty, 4);
    Value *LibcHandle = Builder.CreateCall(DlopenFunc, {LibcPath, RtldNoLoad});
    
    BasicBlock *DoneBB = BasicBlock::Create(Ctx, "done", Func);
    BasicBlock *DlsymBB = BasicBlock::Create(Ctx, "dlsym", Func);
    
    Value *HandleNull = Builder.CreateICmpEQ(LibcHandle, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(HandleNull, DoneBB, DlsymBB);
    
    Builder.SetInsertPoint(DlsymBB);
    
    Value *OpenAddr = Builder.CreateCall(DlsymFunc, {LibcHandle, OpenName});
    
    Value *AddrNull = Builder.CreateICmpEQ(OpenAddr, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(AddrNull, DoneBB, ExitBB);
    
    Builder.SetInsertPoint(DoneBB);
    Builder.CreateRet(ConstantInt::get(Int32Ty, 0));
    
    Value *OpenAddrInt = Builder.CreatePtrToInt(OpenAddr, Int64Ty);
    
    Value *GeStart = Builder.CreateICmpUGE(OpenAddrInt, LibcStart);
    Value *LtEnd = Builder.CreateICmpULT(OpenAddrInt, LibcEnd);
    Value *InLibcRange = Builder.CreateAnd(GeStart, LtEnd);
    
    Builder.CreateCondBr(InLibcRange, ExitBB, HookedBB);
    
    Builder.SetInsertPoint(HookedBB);
    Builder.CreateCall(ReportAndKillFunc);
    Builder.CreateRet(ConstantInt::get(Int32Ty, 1));
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRet(ConstantInt::get(Int32Ty, 0));
    
    return Func;
}

bool PltHookDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] PltHookDetect: Injecting PLT hook detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 使用公共模块创建报告函数
    Function *ReportAndKillFunc = DetectUtils::createReportAndKillFunc(M);
    
    // 创建检测函数
    Function *CheckFunc = createCheckPltHookFunc(M, ReportAndKillFunc);

    BasicBlock &EntryBB = MainFunc->getEntryBlock();
    IRBuilder<> Builder(&EntryBB, EntryBB.getFirstInsertionPt());

    // 注入检测调用
    Builder.CreateCall(CheckFunc);

    return true;
}

ModulePass *llvm::createPltHookDetectPass() {
    return new PltHookDetect();
}

INITIALIZE_PASS_BEGIN(PltHookDetect, "plthookdetect", "Inject PLT hook detection at program start", false, false)
INITIALIZE_PASS_END(PltHookDetect, "plthookdetect", "Inject PLT hook detection at program start", false, false)
