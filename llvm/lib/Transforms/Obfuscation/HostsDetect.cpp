//===- HostsDetect.cpp - Hosts文件检测注入Pass ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 本文件实现Hosts文件检测注入Pass，在程序入口点注入检测代码
// 检测/etc/hosts中的可疑内容（代理/抓包工具特征）
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/HostsDetect.h"
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

#define DEBUG_TYPE "hostsdetect"

using namespace llvm;

namespace {

struct HostsDetect : public ModulePass {
    static char ID;

    HostsDetect() : ModulePass(ID) {
        initializeHostsDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"HostsDetect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createHostsCheckFunc(Module &M, Function *reportFunc);
};

}

char HostsDetect::ID = 0;

Function* HostsDetect::createHostsCheckFunc(Module &M, Function *reportFunc) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int8Ty = Type::getInt8Ty(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "hosts_check",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *OpenOkBB = BasicBlock::Create(Ctx, "open_ok", Func);
    BasicBlock *OpenFailBB = BasicBlock::Create(Ctx, "open_fail", Func);
    BasicBlock *LoopBB = BasicBlock::Create(Ctx, "loop", Func);
    BasicBlock *CheckLineBB = BasicBlock::Create(Ctx, "check_line", Func);
    BasicBlock *CheckPatternsBB = BasicBlock::Create(Ctx, "check_patterns", Func);
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
    
    auto makeString = [&](const char *str) -> Constant* {
        return DetectUtils::createGlobalString(M, str, ".hosts.str");
    };
    
    Constant *HostsPath = makeString("/etc/hosts");
    Constant *ReadMode = makeString("r");
    
    Value *Fp = Builder.CreateCall(FopenFunc, {HostsPath, ReadMode});
    Value *FpNotNull = Builder.CreateICmpNE(Fp, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(FpNotNull, OpenOkBB, OpenFailBB);
    
    Builder.SetInsertPoint(OpenFailBB);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(OpenOkBB);
    
    Type *LineBufTy = ArrayType::get(Int8Ty, 1024);
    Value *LineBuf = Builder.CreateAlloca(LineBufTy, nullptr, "linebuf");
    Value *LineBufPtr = Builder.CreateBitCast(LineBuf, CharPtrTy);
    
    Builder.CreateBr(LoopBB);
    
    Builder.SetInsertPoint(LoopBB);
    
    Value *Line = Builder.CreateCall(FgetsFunc, {LineBufPtr, ConstantInt::get(Int32Ty, 1024), Fp});
    Value *LineNotNull = Builder.CreateICmpNE(Line, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(LineNotNull, CheckLineBB, ExitBB);
    
    Builder.SetInsertPoint(CheckLineBB);
    
    Value *FirstChar = Builder.CreateLoad(Int8Ty, LineBufPtr);
    
    Value *IsComment = Builder.CreateICmpEQ(FirstChar, ConstantInt::get(Int8Ty, '#'));
    Value *IsNewline = Builder.CreateICmpEQ(FirstChar, ConstantInt::get(Int8Ty, '\n'));
    Value *IsNull = Builder.CreateICmpEQ(FirstChar, ConstantInt::get(Int8Ty, 0));
    
    Value *Skip = Builder.CreateOr(IsComment, IsNewline);
    Skip = Builder.CreateOr(Skip, IsNull);
    
    Builder.CreateCondBr(Skip, LoopBB, CheckPatternsBB);
    
    Builder.SetInsertPoint(CheckPatternsBB);
    
    Constant *Patterns[] = {
        makeString("js"),
        makeString("wy"),
        makeString("t3"),
        makeString("wig")
    };
    
    Value *AnyFound = nullptr;
    for (Constant *Pattern : Patterns) {
        Value *Found = Builder.CreateCall(StrstrFunc, {LineBufPtr, Pattern});
        Value *FoundNotNull = Builder.CreateICmpNE(Found, ConstantPointerNull::get(CharPtrTy));
        if (AnyFound == nullptr) {
            AnyFound = FoundNotNull;
        } else {
            AnyFound = Builder.CreateOr(AnyFound, FoundNotNull);
        }
    }
    
    BasicBlock *FoundBB = BasicBlock::Create(Ctx, "found", Func);
    Builder.CreateCondBr(AnyFound, FoundBB, LoopBB);
    
    Builder.SetInsertPoint(FoundBB);
    Builder.CreateCall(FcloseFunc, {Fp});
    Builder.CreateCall(reportFunc);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool HostsDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] HostsDetect: Injecting hosts detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 使用公共模块创建报告函数
    Function *ReportFunc = DetectUtils::createReportAndKillFunc(M, "Hosts File");
    
    // 创建检测函数
    Function *CheckFunc = createHostsCheckFunc(M, ReportFunc);
    
    // 配置选项
    DetectOptions opts = DetectOptions::create(false);
    
    // 注入到main函数
    return DetectUtils::injectToMain(M, CheckFunc, opts);
}

ModulePass *llvm::createHostsDetectPass() {
    return new HostsDetect();
}

INITIALIZE_PASS_BEGIN(HostsDetect, "hostsdetect", "Inject hosts file detection at program start", false, false)
INITIALIZE_PASS_END(HostsDetect, "hostsdetect", "Inject hosts file detection at program start", false, false)
