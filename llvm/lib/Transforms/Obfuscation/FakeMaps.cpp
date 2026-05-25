//===- FakeMaps.cpp - 生成假maps文件注入Pass -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现生成假/proc/self/maps文件内容的Pass
// 在程序启动时生成假的内存映射信息，欺骗调试工具
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/FakeMaps.h"
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

#define DEBUG_TYPE "fakemaps"

using namespace llvm;

namespace {

struct FakeMaps : public ModulePass {
    static char ID;

    FakeMaps() : ModulePass(ID) {
        initializeFakeMapsPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"FakeMaps"};
    }

    bool runOnModule(Module &M) override;
    Function* createGenerateFakeMapsFunc(Module &M);
};

}

char FakeMaps::ID = 0;

Function* FakeMaps::createGenerateFakeMapsFunc(Module &M) {
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
        "generate_fake_maps_content",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *OpenBB = BasicBlock::Create(Ctx, "open", Func);
    BasicBlock *WriteBB = BasicBlock::Create(Ctx, "write", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    FunctionCallee GetpidFunc = M.getOrInsertFunction(
        "getpid",
        FunctionType::get(Int32Ty, {}, false)
    );
    
    CallInst *Pid = Builder.CreateCall(GetpidFunc);
    
    Value *PathBuf = Builder.CreateAlloca(CharPtrTy, ConstantInt::get(Int64Ty, 128));
    Value *MapsFmt = Builder.CreateGlobalStringPtr("/proc/%d/maps", "maps_fmt");
    
    FunctionCallee SnprintfFunc = M.getOrInsertFunction(
        "snprintf",
        FunctionType::get(Int32Ty, {CharPtrTy, Int64Ty, CharPtrTy}, true)
    );
    
    Builder.CreateCall(SnprintfFunc, {PathBuf, ConstantInt::get(Int64Ty, 128), MapsFmt, Pid});
    
    FunctionCallee FopenFunc = M.getOrInsertFunction(
        "fopen",
        FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy}, false)
    );
    
    Value *Mode = Builder.CreateGlobalStringPtr("w", "mode");
    Builder.CreateBr(OpenBB);
    
    Builder.SetInsertPoint(OpenBB);
    CallInst *Fp = Builder.CreateCall(FopenFunc, {PathBuf, Mode});
    
    Value *NullPtr = ConstantPointerNull::get(CharPtrTy);
    Value *IsNull = Builder.CreateICmpEQ(Fp, NullPtr);
    Builder.CreateCondBr(IsNull, ExitBB, WriteBB);
    
    Builder.SetInsertPoint(WriteBB);
    
    FunctionCallee FprintfFunc = M.getOrInsertFunction(
        "fprintf",
        FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy}, true)
    );
    
    Value *Fmt = Builder.CreateGlobalStringPtr("%08lx-%08lx %c%c%c%c %08lx %02x:%02x %lu %s\n", "fmt");
    
    auto writeEntry = [&](uint64_t start, uint64_t end, char r, char w, char x, char p, const char *name) {
        Value *Start = ConstantInt::get(Int64Ty, start);
        Value *End = ConstantInt::get(Int64Ty, end);
        Value *R = ConstantInt::get(Int32Ty, r);
        Value *W = ConstantInt::get(Int32Ty, w);
        Value *X = ConstantInt::get(Int32Ty, x);
        Value *P = ConstantInt::get(Int32Ty, p);
        Value *Offset = ConstantInt::get(Int64Ty, 0);
        Value *DevMaj = ConstantInt::get(Int32Ty, 0);
        Value *DevMin = ConstantInt::get(Int32Ty, 0);
        Value *Inode = ConstantInt::get(Int64Ty, 0);
        Value *Name = Builder.CreateGlobalStringPtr(name, "name");
        
        Builder.CreateCall(FprintfFunc, {Fp, Fmt, Start, End, R, W, X, P, Offset, DevMaj, DevMin, Inode, Name});
    };
    
    writeEntry(0x00000000, 0x00010000, 'r', '-', '-', 'p', "[vdso]");
    writeEntry(0x00010000, 0x00020000, 'r', 'x', '-', 'p', "[vvar]");
    writeEntry(0x00020000, 0x00030000, 'r', '-', '-', 'p', "[stack]");
    writeEntry(0x00030000, 0x00040000, 'r', '-', '-', 'p', "[heap]");
    writeEntry(0x00040000, 0x00050000, 'r', 'x', '-', 'p', "/system/lib64/libc.so");
    writeEntry(0x00050000, 0x00060000, 'r', '-', '-', 'p', "/system/lib64/libc.so");
    writeEntry(0x00060000, 0x00070000, 'r', 'w', '-', 'p', "/system/lib64/libc.so");
    writeEntry(0x00070000, 0x00080000, 'r', 'x', '-', 'p', "/system/lib64/libm.so");
    writeEntry(0x00080000, 0x00090000, 'r', '-', '-', 'p', "/system/lib64/libdl.so");
    writeEntry(0x00090000, 0x000a0000, 'r', 'w', '-', 'p', "[anon]");
    
    FunctionCallee FcloseFunc = M.getOrInsertFunction(
        "fclose",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    Builder.CreateCall(FcloseFunc, {Fp});
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool FakeMaps::runOnModule(Module &M) {
    Function *Main = M.getFunction("main");
    if (!Main || Main->empty()) {
        return false;
    }
    
    Function *GenerateFakeMaps = createGenerateFakeMapsFunc(M);
    
    IRBuilder<> Builder(&Main->getEntryBlock());
    Builder.SetInsertPoint(&*Main->getEntryBlock().getFirstInsertionPt());
    Builder.CreateCall(GenerateFakeMaps);
    
    return true;
}

ModulePass *llvm::createFakeMapsPass() {
    return new FakeMaps();
}

INITIALIZE_PASS_BEGIN(FakeMaps, "fakemaps", "Generate fake /proc/self/maps", false, false)
INITIALIZE_PASS_END(FakeMaps, "fakemaps", "Generate fake /proc/self/maps", false, false)
