//===- VpnDetect.cpp - VPN检测注入Pass ------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 本文件实现VPN检测注入Pass，在程序入口点注入检测代码
// 检测VPN连接状态
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/VpnDetect.h"
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

#define DEBUG_TYPE "vpndetect"

using namespace llvm;

namespace {

struct VpnDetect : public ModulePass {
    static char ID;

    VpnDetect() : ModulePass(ID) {
        initializeVpnDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"VpnDetect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createVpnCheckFunc(Module &M, Function *reportFunc);
};

}

char VpnDetect::ID = 0;

Function* VpnDetect::createVpnCheckFunc(Module &M, Function *reportFunc) {
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
        "vpn_check",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *OpenOkBB = BasicBlock::Create(Ctx, "open_ok", Func);
    BasicBlock *OpenFailBB = BasicBlock::Create(Ctx, "open_fail", Func);
    BasicBlock *LoopBB = BasicBlock::Create(Ctx, "loop", Func);
    BasicBlock *CheckLineBB = BasicBlock::Create(Ctx, "check_line", Func);
    BasicBlock *FoundVpnBB = BasicBlock::Create(Ctx, "found_vpn", Func);
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
        return DetectUtils::createGlobalString(M, str, ".vpn.str");
    };
    
    Constant *NetDevPath = makeString("/proc/net/dev");
    Constant *ReadMode = makeString("r");
    Constant *TunNeedle = makeString("tun");
    
    Value *Fp = Builder.CreateCall(FopenFunc, {NetDevPath, ReadMode});
    Value *FpNotNull = Builder.CreateICmpNE(Fp, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(FpNotNull, OpenOkBB, OpenFailBB);
    
    Builder.SetInsertPoint(OpenFailBB);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(OpenOkBB);
    
    Type *LineBufTy = ArrayType::get(Int8Ty, 512);
    Value *LineBuf = Builder.CreateAlloca(LineBufTy, nullptr, "linebuf");
    Value *LineBufPtr = Builder.CreateBitCast(LineBuf, CharPtrTy);
    
    Builder.CreateBr(LoopBB);
    
    Builder.SetInsertPoint(LoopBB);
    
    Value *Line = Builder.CreateCall(FgetsFunc, {LineBufPtr, ConstantInt::get(Int32Ty, 512), Fp});
    Value *LineNotNull = Builder.CreateICmpNE(Line, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(LineNotNull, CheckLineBB, ExitBB);
    
    Builder.SetInsertPoint(CheckLineBB);
    
    Value *FoundTun = Builder.CreateCall(StrstrFunc, {LineBufPtr, TunNeedle});
    Value *FoundTunNotNull = Builder.CreateICmpNE(FoundTun, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(FoundTunNotNull, FoundVpnBB, LoopBB);
    
    Builder.SetInsertPoint(FoundVpnBB);
    Builder.CreateCall(FcloseFunc, {Fp});
    Builder.CreateCall(reportFunc);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool VpnDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] VpnDetect: Injecting VPN detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 使用公共模块创建报告函数
    Function *ReportFunc = DetectUtils::createReportAndKillFunc(M, "VPN Connection");
    
    // 创建检测函数
    Function *CheckFunc = createVpnCheckFunc(M, ReportFunc);
    
    // 配置选项
    DetectOptions opts = DetectOptions::create(false);
    
    // 注入到main函数
    return DetectUtils::injectToMain(M, CheckFunc, opts);
}

ModulePass *llvm::createVpnDetectPass() {
    return new VpnDetect();
}

INITIALIZE_PASS_BEGIN(VpnDetect, "vpndetect", "Inject VPN detection at program start", false, false)
INITIALIZE_PASS_END(VpnDetect, "vpndetect", "Inject VPN detection at program start", false, false)
