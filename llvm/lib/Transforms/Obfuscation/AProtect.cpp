//===- AProtect.cpp - A-protector注入Pass ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// A-protector 输出注入 Pass
// 注入到全局构造函数，避免与VMProtect冲突
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/AProtect.h"
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

#define DEBUG_TYPE "aprotect"

using namespace llvm;

namespace {

struct AProtect : public ModulePass {
    static char ID;

    AProtect() : ModulePass(ID) {
        initializeAProtectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"AProtect"};
    }

    bool runOnModule(Module &M) override;
};

}

char AProtect::ID = 0;

bool AProtect::runOnModule(Module &M) {
    LLVMContext &Ctx = M.getContext();
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *CharPtrTy = PointerType::get(Ctx, 0);
    Type *VoidTy = Type::getVoidTy(Ctx);

    // 创建AProtect初始化函数
    FunctionType *InitFuncTy = FunctionType::get(VoidTy, false);
    Function *InitFunc = Function::Create(
        InitFuncTy, GlobalValue::PrivateLinkage, ".ap.init", &M);
    
    BasicBlock *InitBB = BasicBlock::Create(Ctx, "entry", InitFunc);
    IRBuilder<> Builder(InitBB);

    // 声明外部函数
    FunctionCallee PrintfFunc = M.getOrInsertFunction(
        "printf", FunctionType::get(Int32Ty, {CharPtrTy}, true));

    // 打印 A-protector（白色）
    Constant *AProtectStr = ConstantDataArray::getString(Ctx, "A-protector\n");
    GlobalVariable *AProtectGV = new GlobalVariable(
        M, AProtectStr->getType(), true, GlobalValue::PrivateLinkage,
        AProtectStr, ".ap.str");
    Constant *AProtectPtr = ConstantExpr::getBitCast(AProtectGV, CharPtrTy);
    Builder.CreateCall(PrintfFunc, {AProtectPtr});

    // 打印版本号
    Constant *VersionStr = ConstantDataArray::getString(Ctx, "Protection v1.0.0\n");
    GlobalVariable *VersionGV = new GlobalVariable(
        M, VersionStr->getType(), true, GlobalValue::PrivateLinkage,
        VersionStr, ".ap.version");
    Constant *VersionPtr = ConstantExpr::getBitCast(VersionGV, CharPtrTy);
    Builder.CreateCall(PrintfFunc, {VersionPtr});
    
    Builder.CreateRetVoid();

    // 将初始化函数添加到全局构造函数列表
    // 使用优先级65535确保在其他构造函数之后执行
    StructType *CtorTy = StructType::get(Ctx, {Int32Ty, CharPtrTy, CharPtrTy});
    
    Constant *CtorFuncPtr = ConstantExpr::getBitCast(InitFunc, CharPtrTy);
    Constant *Priority = ConstantInt::get(Int32Ty, 65535);
    Constant *CtorElem = ConstantStruct::get(CtorTy, {Priority, CtorFuncPtr, 
        ConstantPointerNull::get(PointerType::get(Ctx, 0))});
    
    // 查找或创建llvm.global_ctors
    GlobalVariable *GlobalCtors = M.getGlobalVariable("llvm.global_ctors");
    if (!GlobalCtors) {
        GlobalCtors = new GlobalVariable(
            M, ArrayType::get(CtorTy, 1), false,
            GlobalValue::AppendingLinkage,
            ConstantArray::get(ArrayType::get(CtorTy, 1), {CtorElem}),
            "llvm.global_ctors");
    } else {
        // 追加到现有的构造函数列表
        Constant *OldInit = GlobalCtors->getInitializer();
        ArrayType *OldArrTy = cast<ArrayType>(OldInit->getType());
        unsigned OldSize = OldArrTy->getNumElements();
        
        ArrayType *NewArrTy = ArrayType::get(CtorTy, OldSize + 1);
        std::vector<Constant *> NewCtors;
        for (unsigned i = 0; i < OldSize; ++i) {
            NewCtors.push_back(OldInit->getAggregateElement(i));
        }
        NewCtors.push_back(CtorElem);
        
        GlobalCtors->setInitializer(ConstantArray::get(NewArrTy, NewCtors));
    }

    return true;
}

ModulePass *llvm::createAProtectPass() {
    return new AProtect();
}

INITIALIZE_PASS_BEGIN(AProtect, "aprotect", "Inject A-protector output at program start", false, false)
INITIALIZE_PASS_END(AProtect, "aprotect", "Inject A-protector output at program start", false, false)
