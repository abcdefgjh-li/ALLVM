//===- HideMaps.cpp - 隐藏maps文件注入Pass -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现隐藏/proc/self/maps文件的Pass，通过mount bind用假maps覆盖真实maps
// 需要root权限才能生效
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/HideMaps.h"
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
    Function* createGenerateFakeMapsFunc(Module &M);
    Function* createHideOneFunc(Module &M);
    Function* createHideAllMapsFunc(Module &M);
    Function* createStartThreadFunc(Module &M, Function *HideAllMapsFunc);
};

}

char HideMaps::ID = 0;

Function* HideMaps::createGenerateFakeMapsFunc(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "generate_fake_maps",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *WriteBB = BasicBlock::Create(Ctx, "write", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    FunctionCallee FopenFunc = M.getOrInsertFunction(
        "fopen",
        FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy}, false)
    );
    
    Value *TmpPath = Builder.CreateGlobalStringPtr("/data/local/tmp/.fake_maps", "tmp_path");
    Value *Mode = Builder.CreateGlobalStringPtr("w", "mode");
    
    CallInst *Fp = Builder.CreateCall(FopenFunc, {TmpPath, Mode});
    
    Value *NullPtr = ConstantPointerNull::get(CharPtrTy);
    Value *IsNull = Builder.CreateICmpEQ(Fp, NullPtr);
    Builder.CreateCondBr(IsNull, ExitBB, WriteBB);
    
    Builder.SetInsertPoint(WriteBB);
    
    FunctionCallee FprintfFunc = M.getOrInsertFunction(
        "fprintf",
        FunctionType::get(Type::getInt32Ty(Ctx), {CharPtrTy, CharPtrTy}, true)
    );
    
    Value *Fmt1 = Builder.CreateGlobalStringPtr("%08lx-%08lx %c%c%c%c %08lx %02x:%02x %lu %s\n", "fmt1");
    Value *Zero = ConstantInt::get(Type::getInt64Ty(Ctx), 0);
    Value *One = ConstantInt::get(Type::getInt64Ty(Ctx), 0x1000);
    Value *Two = ConstantInt::get(Type::getInt64Ty(Ctx), 0x2000);
    Value *Three = ConstantInt::get(Type::getInt64Ty(Ctx), 0x3000);
    Value *R = ConstantInt::get(Type::getInt32Ty(Ctx), 'r');
    Value *X = ConstantInt::get(Type::getInt32Ty(Ctx), 'x');
    Value *W = ConstantInt::get(Type::getInt32Ty(Ctx), 'w');
    Value *Dash = ConstantInt::get(Type::getInt32Ty(Ctx), '-');
    Value *Fake = Builder.CreateGlobalStringPtr("[fake]", "fake");
    
    Builder.CreateCall(FprintfFunc, {Fp, Fmt1, Zero, One, R, Dash, Dash, Dash, Zero, Zero, Zero, Zero, Fake});
    Builder.CreateCall(FprintfFunc, {Fp, Fmt1, One, Two, R, X, Dash, Dash, Zero, Zero, Zero, Zero, Fake});
    Builder.CreateCall(FprintfFunc, {Fp, Fmt1, Two, Three, R, W, Dash, Dash, Zero, Zero, Zero, Zero, Fake});
    
    FunctionCallee FcloseFunc = M.getOrInsertFunction(
        "fclose",
        FunctionType::get(Type::getInt32Ty(Ctx), {CharPtrTy}, false)
    );
    Builder.CreateCall(FcloseFunc, {Fp});
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

Function* HideMaps::createHideOneFunc(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(Int32Ty, {CharPtrTy}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "hide_one_maps",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    Argument *PathArg = &*Func->arg_begin();
    PathArg->setName("path");
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(EntryBB);
    
    FunctionCallee SystemFunc = M.getOrInsertFunction(
        "system",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    
    Value *CmdFormat = Builder.CreateGlobalStringPtr("mount -o bind /data/local/tmp/.fake_maps %s 2>/dev/null", "cmd_fmt");
    
    Value *CmdBuf = Builder.CreateAlloca(CharPtrTy, ConstantInt::get(Type::getInt64Ty(Ctx), 256));
    
    FunctionCallee SnprintfFunc = M.getOrInsertFunction(
        "snprintf",
        FunctionType::get(Int32Ty, {CharPtrTy, Type::getInt64Ty(Ctx), CharPtrTy}, true)
    );
    
    Builder.CreateCall(SnprintfFunc, {CmdBuf, ConstantInt::get(Type::getInt64Ty(Ctx), 256), CmdFormat, PathArg});
    
    CallInst *Ret = Builder.CreateCall(SystemFunc, {CmdBuf});
    Builder.CreateRet(Ret);
    
    return Func;
}

Function* HideMaps::createHideAllMapsFunc(Module &M) {
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
        "hide_all_maps",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *CheckRootBB = BasicBlock::Create(Ctx, "check_root", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    FunctionCallee GetuidFunc = M.getOrInsertFunction(
        "getuid",
        FunctionType::get(Int32Ty, {}, false)
    );
    
    CallInst *Uid = Builder.CreateCall(GetuidFunc);
    Value *IsRoot = Builder.CreateICmpEQ(Uid, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(IsRoot, CheckRootBB, ExitBB);
    
    Builder.SetInsertPoint(CheckRootBB);
    
    Function *GenerateFake = createGenerateFakeMapsFunc(M);
    Builder.CreateCall(GenerateFake);
    
    FunctionCallee GetpidFunc = M.getOrInsertFunction(
        "getpid",
        FunctionType::get(Int32Ty, {}, false)
    );
    
    CallInst *Pid = Builder.CreateCall(GetpidFunc);
    
    Value *PathBuf = Builder.CreateAlloca(CharPtrTy, ConstantInt::get(Int64Ty, 128));
    Value *TaskBuf = Builder.CreateAlloca(CharPtrTy, ConstantInt::get(Int64Ty, 64));
    
    Value *MapsFmt = Builder.CreateGlobalStringPtr("/proc/%d/maps", "maps_fmt");
    Value *TaskFmt = Builder.CreateGlobalStringPtr("/proc/%d/task", "task_fmt");
    
    FunctionCallee SnprintfFunc = M.getOrInsertFunction(
        "snprintf",
        FunctionType::get(Int32Ty, {CharPtrTy, Int64Ty, CharPtrTy}, true)
    );
    
    Builder.CreateCall(SnprintfFunc, {PathBuf, ConstantInt::get(Int64Ty, 128), MapsFmt, Pid});
    Builder.CreateCall(SnprintfFunc, {TaskBuf, ConstantInt::get(Int64Ty, 64), TaskFmt, Pid});
    
    Function *HideOne = createHideOneFunc(M);
    Builder.CreateCall(HideOne, {PathBuf});
    
    FunctionCallee OpendirFunc = M.getOrInsertFunction(
        "opendir",
        FunctionType::get(CharPtrTy, {CharPtrTy}, false)
    );
    
    CallInst *Dir = Builder.CreateCall(OpendirFunc, {TaskBuf});
    Value *DirNull = Builder.CreateICmpEQ(Dir, ConstantPointerNull::get(CharPtrTy));
    
    BasicBlock *ReadDirBB = BasicBlock::Create(Ctx, "readdir", Func);
    BasicBlock *CloseDirBB = BasicBlock::Create(Ctx, "closedir", Func);
    
    Builder.CreateCondBr(DirNull, ExitBB, ReadDirBB);
    
    Builder.SetInsertPoint(ReadDirBB);
    
    FunctionCallee ReaddirFunc = M.getOrInsertFunction(
        "readdir",
        FunctionType::get(CharPtrTy, {CharPtrTy}, false)
    );
    
    PHINode *DirPhi = Builder.CreatePHI(CharPtrTy, 2, "dir_ptr");
    DirPhi->addIncoming(Dir, CheckRootBB);
    
    CallInst *Entry = Builder.CreateCall(ReaddirFunc, {DirPhi});
    Value *EntryNull = Builder.CreateICmpEQ(Entry, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(EntryNull, CloseDirBB, ReadDirBB);
    
    Builder.SetInsertPoint(CloseDirBB);
    
    FunctionCallee ClosedirFunc = M.getOrInsertFunction(
        "closedir",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    Builder.CreateCall(ClosedirFunc, {Dir});
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

Function* HideMaps::createStartThreadFunc(Module &M, Function *HideAllMapsFunc) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *ThreadFuncTy = FunctionType::get(VoidTy, {CharPtrTy}, false);
    Function *ThreadFunc = Function::Create(
        ThreadFuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "hide_maps_thread",
        &M
    );
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", ThreadFunc);
    IRBuilder<> Builder(BB);
    
    Builder.CreateCall(HideAllMapsFunc);
    Builder.CreateRetVoid();
    
    FunctionType *FuncTy = FunctionType::get(Int32Ty, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "start_hide_maps_thread",
        &M
    );
    
    BB = BasicBlock::Create(Ctx, "entry", Func);
    Builder.SetInsertPoint(BB);
    
    FunctionCallee PthreadCreateFunc = M.getOrInsertFunction(
        "pthread_create",
        FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy, CharPtrTy, CharPtrTy}, false)
    );
    
    Value *NullPtr = ConstantPointerNull::get(CharPtrTy);
    Value *ThreadFuncPtr = Builder.CreateBitCast(ThreadFunc, CharPtrTy);
    
    Builder.CreateCall(PthreadCreateFunc, {NullPtr, NullPtr, ThreadFuncPtr, NullPtr});
    Builder.CreateRet(ConstantInt::get(Int32Ty, 0));
    
    return Func;
}

bool HideMaps::runOnModule(Module &M) {
    Function *Main = M.getFunction("main");
    if (!Main || Main->empty()) {
        return false;
    }
    
    Function *HideAllMaps = createHideAllMapsFunc(M);
    Function *StartThread = createStartThreadFunc(M, HideAllMaps);
    
    IRBuilder<> Builder(&Main->getEntryBlock());
    Builder.SetInsertPoint(&*Main->getEntryBlock().getFirstInsertionPt());
    Builder.CreateCall(StartThread);
    
    return true;
}

ModulePass *llvm::createHideMapsPass() {
    return new HideMaps();
}

INITIALIZE_PASS_BEGIN(HideMaps, "hidemaps", "Hide /proc/self/maps", false, false)
INITIALIZE_PASS_END(HideMaps, "hidemaps", "Hide /proc/self/maps", false, false)
