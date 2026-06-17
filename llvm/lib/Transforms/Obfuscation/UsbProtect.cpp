//===- UsbProtect.cpp - USB调试禁用注入Pass --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现USB调试保护注入Pass，在程序入口点注入保护代码
// 尽力关闭USB调试；如果设备环境不支持，则静默跳过，避免误报误杀
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/UsbProtect.h"
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

#define DEBUG_TYPE "usbprotect"

using namespace llvm;

namespace {

struct UsbProtect : public ModulePass {
    static char ID;

    UsbProtect() : ModulePass(ID) {
        initializeUsbProtectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"UsbProtect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createUsbCheckFunc(Module &M);
};

}

char UsbProtect::ID = 0;

Function* UsbProtect::createUsbCheckFunc(Module &M) {
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
        "usb_check",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *CheckEnableBB = BasicBlock::Create(Ctx, "check_enable", Func);
    BasicBlock *CheckEnableOkBB = BasicBlock::Create(Ctx, "check_enable_ok", Func);
    BasicBlock *CheckFuncsBB = BasicBlock::Create(Ctx, "check_funcs", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    FunctionCallee FopenFunc = M.getOrInsertFunction(
        "fopen",
        FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy}, false)
    );
    
    FunctionCallee FwriteFunc = M.getOrInsertFunction(
        "fwrite",
        FunctionType::get(Int64Ty, {CharPtrTy, Int64Ty, Int64Ty, CharPtrTy}, false)
    );
    
    FunctionCallee FcloseFunc = M.getOrInsertFunction(
        "fclose",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    
    FunctionCallee SystemFunc = M.getOrInsertFunction(
        "system",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    
    FunctionCallee UsleepFunc = M.getOrInsertFunction(
        "usleep",
        FunctionType::get(Int32Ty, {Int32Ty}, false)
    );
    
    auto makeString = [&](const char *str) -> Constant* {
        Constant *StrConst = ConstantDataArray::getString(Ctx, str);
        GlobalVariable *StrGV = new GlobalVariable(
            M, StrConst->getType(), true,
            GlobalValue::PrivateLinkage, StrConst,
            ".usb.str." + Twine(str)
        );
        return ConstantExpr::getBitCast(StrGV, CharPtrTy);
    };
    
    Constant *WriteMode = makeString("w");
    Constant *ReadMode = makeString("r");
    Constant *EnablePath = makeString("/sys/class/android_usb/android0/enable");
    Constant *FuncsPath = makeString("/sys/class/android_usb/android0/functions");
    Constant *Str0 = makeString("0");
    Constant *StrNone = makeString("none");
    Constant *SetpropCmd1 = makeString("setprop sys.usb.config none 2>/dev/null");
    Constant *SetpropCmd2 = makeString("setprop sys.usb.state none 2>/dev/null");
    Constant *SetpropCmd3 = makeString("setprop persist.sys.usb.config none 2>/dev/null");
    
    Value *Fp = Builder.CreateCall(FopenFunc, {EnablePath, WriteMode});
    Value *FpNotNull = Builder.CreateICmpNE(Fp, ConstantPointerNull::get(CharPtrTy));
    BasicBlock *WriteEnableBB = BasicBlock::Create(Ctx, "write_enable", Func);
    BasicBlock *SkipEnableBB = BasicBlock::Create(Ctx, "skip_enable", Func);
    Builder.CreateCondBr(FpNotNull, WriteEnableBB, SkipEnableBB);
    
    Builder.SetInsertPoint(WriteEnableBB);
    Builder.CreateCall(FwriteFunc, {Str0, ConstantInt::get(Int64Ty, 1), ConstantInt::get(Int64Ty, 1), Fp});
    Builder.CreateCall(FcloseFunc, {Fp});
    Builder.CreateBr(SkipEnableBB);
    
    Builder.SetInsertPoint(SkipEnableBB);
    Fp = Builder.CreateCall(FopenFunc, {FuncsPath, WriteMode});
    FpNotNull = Builder.CreateICmpNE(Fp, ConstantPointerNull::get(CharPtrTy));
    BasicBlock *WriteFuncsBB = BasicBlock::Create(Ctx, "write_funcs", Func);
    BasicBlock *SkipFuncsBB = BasicBlock::Create(Ctx, "skip_funcs", Func);
    Builder.CreateCondBr(FpNotNull, WriteFuncsBB, SkipFuncsBB);
    
    Builder.SetInsertPoint(WriteFuncsBB);
    Builder.CreateCall(FwriteFunc, {StrNone, ConstantInt::get(Int64Ty, 1), ConstantInt::get(Int64Ty, 4), Fp});
    Builder.CreateCall(FcloseFunc, {Fp});
    Builder.CreateBr(SkipFuncsBB);
    
    Builder.SetInsertPoint(SkipFuncsBB);
    Builder.CreateCall(SystemFunc, {SetpropCmd1});
    Builder.CreateCall(SystemFunc, {SetpropCmd2});
    Builder.CreateCall(SystemFunc, {SetpropCmd3});
    Builder.CreateCall(UsleepFunc, {ConstantInt::get(Int32Ty, 500000)});
    Builder.CreateBr(CheckEnableBB);
    
    Builder.SetInsertPoint(CheckEnableBB);
    Fp = Builder.CreateCall(FopenFunc, {EnablePath, ReadMode});
    FpNotNull = Builder.CreateICmpNE(Fp, ConstantPointerNull::get(CharPtrTy));
    // 如果文件不存在，跳过检测（模拟器可能没有这些文件）
    Builder.CreateCondBr(FpNotNull, CheckEnableOkBB, CheckFuncsBB);
    
    Builder.SetInsertPoint(CheckEnableOkBB);
    Type *Buf16Ty = ArrayType::get(Type::getInt8Ty(Ctx), 16);
    Value *Buf16 = Builder.CreateAlloca(Buf16Ty, nullptr, "buf16");
    Value *Buf16Ptr = Builder.CreateBitCast(Buf16, CharPtrTy);
    
    FunctionCallee FreadFunc = M.getOrInsertFunction(
        "fread",
        FunctionType::get(Int64Ty, {CharPtrTy, Int64Ty, Int64Ty, CharPtrTy}, false)
    );
    
    Builder.CreateCall(FreadFunc, {Buf16Ptr, ConstantInt::get(Int64Ty, 1), ConstantInt::get(Int64Ty, 15), Fp});
    Builder.CreateCall(FcloseFunc, {Fp});
    
    FunctionCallee StrstrFunc = M.getOrInsertFunction(
        "strstr",
        FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy}, false)
    );
    
    Constant *Needle1 = makeString("1");
    Value *Found1 = Builder.CreateCall(StrstrFunc, {Buf16Ptr, Needle1});
    Value *Found1NotNull = Builder.CreateICmpNE(Found1, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(Found1NotNull, ExitBB, CheckFuncsBB);
    
    Builder.SetInsertPoint(CheckFuncsBB);
    Fp = Builder.CreateCall(FopenFunc, {FuncsPath, ReadMode});
    FpNotNull = Builder.CreateICmpNE(Fp, ConstantPointerNull::get(CharPtrTy));
    BasicBlock *CheckFuncsOkBB = BasicBlock::Create(Ctx, "check_funcs_ok", Func);
    Builder.CreateCondBr(FpNotNull, CheckFuncsOkBB, ExitBB);
    
    Builder.SetInsertPoint(CheckFuncsOkBB);
    Type *Buf256Ty = ArrayType::get(Type::getInt8Ty(Ctx), 256);
    Value *Buf256 = Builder.CreateAlloca(Buf256Ty, nullptr, "buf256");
    Value *Buf256Ptr = Builder.CreateBitCast(Buf256, CharPtrTy);
    
    Builder.CreateCall(FreadFunc, {Buf256Ptr, ConstantInt::get(Int64Ty, 1), ConstantInt::get(Int64Ty, 255), Fp});
    Builder.CreateCall(FcloseFunc, {Fp});
    
    Constant *NeedleMtp = makeString("mtp");
    Constant *NeedlePtp = makeString("ptp");
    Constant *NeedleMass = makeString("mass_storage");
    Constant *NeedleFile = makeString("file");
    
    Value *FoundMtp = Builder.CreateCall(StrstrFunc, {Buf256Ptr, NeedleMtp});
    Value *FoundMtpNotNull = Builder.CreateICmpNE(FoundMtp, ConstantPointerNull::get(CharPtrTy));
    
    Value *FoundPtp = Builder.CreateCall(StrstrFunc, {Buf256Ptr, NeedlePtp});
    Value *FoundPtpNotNull = Builder.CreateICmpNE(FoundPtp, ConstantPointerNull::get(CharPtrTy));
    
    Value *FoundMass = Builder.CreateCall(StrstrFunc, {Buf256Ptr, NeedleMass});
    Value *FoundMassNotNull = Builder.CreateICmpNE(FoundMass, ConstantPointerNull::get(CharPtrTy));
    
    Value *FoundFile = Builder.CreateCall(StrstrFunc, {Buf256Ptr, NeedleFile});
    Value *FoundFileNotNull = Builder.CreateICmpNE(FoundFile, ConstantPointerNull::get(CharPtrTy));
    
    Value *AnyFound = Builder.CreateOr(FoundMtpNotNull, FoundPtpNotNull);
    AnyFound = Builder.CreateOr(AnyFound, FoundMassNotNull);
    AnyFound = Builder.CreateOr(AnyFound, FoundFileNotNull);
    
    Builder.CreateCondBr(AnyFound, ExitBB, ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool UsbProtect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] UsbProtect: Injecting USB protection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration()) {
        return false;
    }

    if (MainFunc->empty()) {
        return false;
    }

    BasicBlock &EntryBB = MainFunc->getEntryBlock();
    
    Function *CheckFunc = createUsbCheckFunc(M);

    IRBuilder<> Builder(&EntryBB, EntryBB.getFirstInsertionPt());

    if (isIRObfuscationDebugEnabled()) {
        LLVMContext &Ctx = M.getContext();
        FunctionCallee PrintfFunc = M.getOrInsertFunction(
            "printf",
            FunctionType::get(Type::getInt32Ty(Ctx), {PointerType::get(Ctx, 0)}, true)
        );
        Constant *DebugStr = ConstantDataArray::getString(Ctx, "[DEBUG] UsbProtect: Checking USB debug...\n");
        GlobalVariable *DebugGV = new GlobalVariable(
            M, DebugStr->getType(), true,
            GlobalValue::PrivateLinkage, DebugStr,
            ".usb.debug"
        );
        Builder.CreateCall(PrintfFunc, {ConstantExpr::getBitCast(DebugGV, PointerType::get(Ctx, 0))});
    }

    Builder.CreateCall(CheckFunc);

    return true;
}

ModulePass *llvm::createUsbProtectPass() {
    return new UsbProtect();
}

INITIALIZE_PASS_BEGIN(UsbProtect, "usbprotect", "Inject USB debug protection at program start", false, false)
INITIALIZE_PASS_END(UsbProtect, "usbprotect", "Inject USB debug protection at program start", false, false)
