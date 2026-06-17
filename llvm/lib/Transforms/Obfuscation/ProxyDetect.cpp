//===- ProxyDetect.cpp - 代理检测注入Pass ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 本文件实现代理检测注入Pass，在程序入口点注入检测代码
// 检测HTTP代理环境变量
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/ProxyDetect.h"
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

#define DEBUG_TYPE "proxydetect"

using namespace llvm;

namespace {

struct ProxyDetect : public ModulePass {
    static char ID;

    ProxyDetect() : ModulePass(ID) {
        initializeProxyDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"ProxyDetect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createProxyCheckFunc(Module &M, Function *reportFunc);
};

}

char ProxyDetect::ID = 0;

Function* ProxyDetect::createProxyCheckFunc(Module &M, Function *reportFunc) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "proxy_check",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *CheckHttpProxyBB = BasicBlock::Create(Ctx, "check_http_proxy", Func);
    BasicBlock *CheckHttpsProxyBB = BasicBlock::Create(Ctx, "check_https_proxy", Func);
    BasicBlock *FoundProxyBB = BasicBlock::Create(Ctx, "found_proxy", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    FunctionCallee GetenvFunc = M.getOrInsertFunction(
        "getenv",
        FunctionType::get(CharPtrTy, {CharPtrTy}, false)
    );
    
    auto makeString = [&](const char *str) -> Constant* {
        return DetectUtils::createGlobalString(M, str, ".proxy.str");
    };
    
    Constant *HttpProxyEnv = makeString("http_proxy");
    Constant *HttpsProxyEnv = makeString("https_proxy");
    
    // 检查http_proxy
    Value *HttpProxy = Builder.CreateCall(GetenvFunc, {HttpProxyEnv});
    Value *HttpProxyNotNull = Builder.CreateICmpNE(HttpProxy, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(HttpProxyNotNull, FoundProxyBB, CheckHttpProxyBB);
    
    Builder.SetInsertPoint(CheckHttpProxyBB);
    
    // 检查HTTP_PROXY（大写）
    Constant *HttpProxyEnvUpper = makeString("HTTP_PROXY");
    Value *HttpProxyUpper = Builder.CreateCall(GetenvFunc, {HttpProxyEnvUpper});
    Value *HttpProxyUpperNotNull = Builder.CreateICmpNE(HttpProxyUpper, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(HttpProxyUpperNotNull, FoundProxyBB, CheckHttpsProxyBB);
    
    Builder.SetInsertPoint(CheckHttpsProxyBB);
    
    // 检查https_proxy
    Value *HttpsProxy = Builder.CreateCall(GetenvFunc, {HttpsProxyEnv});
    Value *HttpsProxyNotNull = Builder.CreateICmpNE(HttpsProxy, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(HttpsProxyNotNull, FoundProxyBB, ExitBB);
    
    Builder.SetInsertPoint(FoundProxyBB);
    Builder.CreateCall(reportFunc);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool ProxyDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] ProxyDetect: Injecting proxy detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 使用公共模块创建报告函数
    Function *ReportFunc = DetectUtils::createReportAndKillFunc(M, "Proxy/iptables");
    
    // 创建检测函数
    Function *CheckFunc = createProxyCheckFunc(M, ReportFunc);
    
    // 配置选项
    DetectOptions opts = DetectOptions::create(false);
    
    // 注入到main函数
    return DetectUtils::injectToMain(M, CheckFunc, opts);
}

ModulePass *llvm::createProxyDetectPass() {
    return new ProxyDetect();
}

INITIALIZE_PASS_BEGIN(ProxyDetect, "proxydetect", "Inject proxy detection at program start", false, false)
INITIALIZE_PASS_END(ProxyDetect, "proxydetect", "Inject proxy detection at program start", false, false)
