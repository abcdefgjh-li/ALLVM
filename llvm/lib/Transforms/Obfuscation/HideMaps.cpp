//===- HideMaps.cpp - 隐藏内存映射注入Pass --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 本文件实现隐藏内存映射注入Pass，在程序入口点注入保护代码
// 使用mount bind隐藏/proc/self/maps
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/HideMaps.h"
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

#define DEBUG_TYPE "hidemaps"

using namespace llvm;

namespace {

struct HideMaps : public ModulePass {
    static char ID;

    HideMaps() : ModulePass(ID) {
        initializeHideMapsPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"HideMaps"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createHideMapsFunc(Module &M);
};

}

char HideMaps::ID = 0;

Function* HideMaps::createHideMapsFunc(Module &M) {
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
        "hide_maps",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *CreateFakeBB = BasicBlock::Create(Ctx, "create_fake", Func);
    BasicBlock *MountBB = BasicBlock::Create(Ctx, "mount", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    FunctionCallee GetuidFunc = M.getOrInsertFunction(
        "getuid",
        FunctionType::get(Int32Ty, {}, false)
    );
    
    FunctionCallee MountFunc = M.getOrInsertFunction(
        "mount",
        FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy, CharPtrTy, Int64Ty, CharPtrTy}, false)
    );
    
    FunctionCallee FopenFunc = M.getOrInsertFunction(
        "fopen",
        FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy}, false)
    );
    
    FunctionCallee FprintfFunc = M.getOrInsertFunction(
        "fprintf",
        FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy}, true)
    );
    
    FunctionCallee FcloseFunc = M.getOrInsertFunction(
        "fclose",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    
    auto makeString = [&](const char *str) -> Constant* {
        return DetectUtils::createGlobalString(M, str, ".hidemaps.str");
    };
    
    // 检查是否有root权限
    Value *Uid = Builder.CreateCall(GetuidFunc);
    Value *IsRoot = Builder.CreateICmpEQ(Uid, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(IsRoot, CreateFakeBB, ExitBB);
    
    Builder.SetInsertPoint(CreateFakeBB);
    
    Constant *FakePath = makeString("/data/local/tmp/fake_maps");
    Constant *WriteMode = makeString("w");
    Constant *FakeContent = makeString("00000000-00000000 ---p 00000000 00:00 0\n");
    
    Value *FakeFp = Builder.CreateCall(FopenFunc, {FakePath, WriteMode});
    Value *FakeFpNotNull = Builder.CreateICmpNE(FakeFp, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(FakeFpNotNull, MountBB, ExitBB);
    
    Builder.SetInsertPoint(MountBB);
    
    Builder.CreateCall(FprintfFunc, {FakeFp, FakeContent});
    Builder.CreateCall(FcloseFunc, {FakeFp});
    
    Constant *MapsPath = makeString("/proc/self/maps");
    Constant *FsType = makeString("none");
    
    // MS_BIND = 4096
    Value *MountFlags = ConstantInt::get(Type::getInt64Ty(Ctx), 4096);
    
    Builder.CreateCall(MountFunc, {
        FakePath, MapsPath, FsType, MountFlags, ConstantPointerNull::get(CharPtrTy)
    });
    
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool HideMaps::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] HideMaps: Injecting hide maps protection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 创建保护函数
    Function *ProtectFunc = createHideMapsFunc(M);
    
    // 配置选项
    DetectOptions opts = DetectOptions::create(false);
    
    // 注入到main函数
    return DetectUtils::injectToMain(M, ProtectFunc, opts);
}

ModulePass *llvm::createHideMapsPass() {
    return new HideMaps();
}

INITIALIZE_PASS_BEGIN(HideMaps, "hidemaps", "Inject hide maps protection at program start", false, false)
INITIALIZE_PASS_END(HideMaps, "hidemaps", "Inject hide maps protection at program start", false, false)
