//===- SyscallProtect.cpp - 系统调用保护Pass ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现系统调用保护Pass，将libc函数替换为直接syscall实现，绕过libc防止Hook注入
//
// 支持替换的函数:
//   - 网络相关: recv, recvfrom, send, sendto, connect, getaddrinfo
//   - 文件相关: read, write, open, openat, fopen, remove, unlink, unlinkat, rmdir, truncate, ftruncate
//   - 进程相关: system, popen, execve, execvp, execvpe, exit, _exit, ptrace
//   - 其他: clock_gettime, getenv, memcmp
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/SyscallProtect.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/TargetParser/Triple.h"

#define DEBUG_TYPE "syscallprotect"

using namespace llvm;

namespace {

struct SyscallProtect : public ModulePass {
    static char ID;

    SyscallProtect() : ModulePass(ID) {
        initializeSyscallProtectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"SyscallProtect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createSyscallWrapper(Module &M, StringRef Name, int SyscallNum, int NumArgs, 
                                    ArrayRef<Type*> CustomArgTypes = {});
    Function* createMemcmpWrapper(Module &M);
    Function* createGetenvWrapper(Module &M);
    Function* createGetaddrinfoWrapper(Module &M);
    Function* createPopenWrapper(Module &M);
    Function* createSystemWrapper(Module &M);
    Function* createExecvpWrapper(Module &M);
    Function* createExecvpeWrapper(Module &M);
    Function* createRemoveWrapper(Module &M);
    Function* createFopenWrapper(Module &M);
    void injectDebugPrintf(IRBuilder<> &Builder, Module &M, const char *Msg);
    
    bool isSyscallFunction(const StringRef &Name);
    bool isManualImplFunction(const StringRef &Name);
    bool shouldReplace(const StringRef &Name);
    int getSyscallNum(const StringRef &Name);
    int getNumArgs(const StringRef &Name);
    SmallVector<Type*, 6> getArgTypes(LLVMContext &Ctx, const StringRef &Name);
};

}

char SyscallProtect::ID = 0;

bool SyscallProtect::isSyscallFunction(const StringRef &Name) {
    static const StringSet<> SyscallFuncs = {
        "recv", "recvfrom", "send", "sendto",
        "read", "write", "connect", "clock_gettime",
        "open", "openat", "unlink", "unlinkat", "rmdir",
        "truncate", "ftruncate", "exit", "_exit",
        "ptrace", "execve",
        "__orig_recv", "__orig_recvfrom", "__orig_send", "__orig_sendto",
        "__orig_read", "__orig_write", "__orig_connect", "__orig_clock_gettime",
        "__orig_open", "__orig_openat", "__orig_unlink", "__orig_unlinkat", "__orig_rmdir",
        "__orig_truncate", "__orig_ftruncate", "__orig_exit", "__orig__exit",
        "__orig_ptrace", "__orig_execve"
    };
    return SyscallFuncs.contains(Name);
}

bool SyscallProtect::isManualImplFunction(const StringRef &Name) {
    static const StringSet<> ManualFuncs = {
        "memcmp", "__orig_memcmp",
        "getenv", "__orig_getenv",
        "getaddrinfo", "__orig_getaddrinfo",
        "popen", "__orig_popen",
        "system", "__orig_system",
        "execvp", "__orig_execvp",
        "execvpe", "__orig_execvpe",
        "remove", "__orig_remove",
        "fopen", "__orig_fopen"
    };
    return ManualFuncs.contains(Name);
}

bool SyscallProtect::shouldReplace(const StringRef &Name) {
    return isSyscallFunction(Name) || isManualImplFunction(Name);
}

int SyscallProtect::getSyscallNum(const StringRef &Name) {
    static const StringMap<int> SyscallNums = {
        {"sendto", 206}, {"recvfrom", 207},
        {"send", 206}, {"recv", 207},
        {"read", 63}, {"write", 64},
        {"connect", 203}, {"clock_gettime", 223},
        {"open", 56}, {"openat", 56},
        {"unlink", 87}, {"unlinkat", 35},
        {"rmdir", 84}, {"truncate", 45},
        {"ftruncate", 46}, {"exit", 93},
        {"_exit", 93}, {"ptrace", 117},
        {"execve", 221},
        {"__orig_sendto", 206}, {"__orig_recvfrom", 207},
        {"__orig_send", 206}, {"__orig_recv", 207},
        {"__orig_read", 63}, {"__orig_write", 64},
        {"__orig_connect", 203}, {"__orig_clock_gettime", 223},
        {"__orig_open", 56}, {"__orig_openat", 56},
        {"__orig_unlink", 87}, {"__orig_unlinkat", 35},
        {"__orig_rmdir", 84}, {"__orig_truncate", 45},
        {"__orig_ftruncate", 46}, {"__orig_exit", 93},
        {"__orig__exit", 93}, {"__orig_ptrace", 117},
        {"__orig_execve", 221}
    };
    auto It = SyscallNums.find(Name);
    return It != SyscallNums.end() ? It->second : -1;
}

int SyscallProtect::getNumArgs(const StringRef &Name) {
    static const StringMap<int> NumArgsMap = {
        {"sendto", 6}, {"recvfrom", 6},
        {"send", 4}, {"recv", 4},
        {"read", 3}, {"write", 3},
        {"connect", 3}, {"clock_gettime", 2},
        {"open", 3}, {"openat", 4},
        {"unlink", 1}, {"unlinkat", 3},
        {"rmdir", 1}, {"truncate", 2},
        {"ftruncate", 2}, {"exit", 1},
        {"_exit", 1}, {"ptrace", 4},
        {"execve", 3},
        {"__orig_sendto", 6}, {"__orig_recvfrom", 6},
        {"__orig_send", 4}, {"__orig_recv", 4},
        {"__orig_read", 3}, {"__orig_write", 3},
        {"__orig_connect", 3}, {"__orig_clock_gettime", 2},
        {"__orig_open", 3}, {"__orig_openat", 4},
        {"__orig_unlink", 1}, {"__orig_unlinkat", 3},
        {"__orig_rmdir", 1}, {"__orig_truncate", 2},
        {"__orig_ftruncate", 2}, {"__orig_exit", 1},
        {"__orig__exit", 1}, {"__orig_ptrace", 4},
        {"__orig_execve", 3}
    };
    auto It = NumArgsMap.find(Name);
    return It != NumArgsMap.end() ? It->second : 0;
}

SmallVector<Type*, 6> SyscallProtect::getArgTypes(LLVMContext &Ctx, const StringRef &Name) {
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    
    StringRef BaseName = Name.starts_with("__orig_") ? Name.substr(7) : Name;
    
    if (BaseName == "open") {
        return SmallVector<Type*, 6>{PtrTy, Int32Ty, Int32Ty};
    }
    if (BaseName == "openat") {
        return SmallVector<Type*, 6>{Int32Ty, PtrTy, Int32Ty, Int32Ty};
    }
    if (BaseName == "unlink") {
        return SmallVector<Type*, 6>{PtrTy};
    }
    if (BaseName == "unlinkat") {
        return SmallVector<Type*, 6>{Int32Ty, PtrTy, Int32Ty};
    }
    if (BaseName == "rmdir") {
        return SmallVector<Type*, 6>{PtrTy};
    }
    if (BaseName == "truncate") {
        return SmallVector<Type*, 6>{PtrTy, Int64Ty};
    }
    if (BaseName == "ftruncate") {
        return SmallVector<Type*, 6>{Int32Ty, Int64Ty};
    }
    if (BaseName == "exit" || BaseName == "_exit") {
        return SmallVector<Type*, 6>{Int32Ty};
    }
    if (BaseName == "ptrace") {
        return SmallVector<Type*, 6>{Int64Ty, Int64Ty, PtrTy, PtrTy};
    }
    if (BaseName == "execve") {
        return SmallVector<Type*, 6>{PtrTy, PtrTy, PtrTy};
    }
    
    int NumArgs = getNumArgs(Name);
    SmallVector<Type*, 6> ArgTypes;
    for (int i = 0; i < NumArgs; i++) {
        if (i == 0) ArgTypes.push_back(Int32Ty);
        else if (i == 2) ArgTypes.push_back(Int32Ty);
        else if (i == 3) ArgTypes.push_back(Int32Ty);
        else ArgTypes.push_back(PtrTy);
    }
    return ArgTypes;
}

void SyscallProtect::injectDebugPrintf(IRBuilder<> &Builder, Module &M, const char *Msg) {
    if (!isIRObfuscationDebugEnabled()) return;
    
    LLVMContext &Ctx = M.getContext();
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionCallee PrintfFunc = M.getOrInsertFunction(
        "printf",
        FunctionType::get(Int32Ty, {CharPtrTy}, true)
    );
    
    Constant *MsgStr = ConstantDataArray::getString(Ctx, Msg);
    GlobalVariable *MsgGV = new GlobalVariable(
        M, MsgStr->getType(), true,
        GlobalValue::PrivateLinkage, MsgStr,
        ".syscall.debug"
    );
    MsgGV->setSection(".AProtect.rodata");
    Builder.CreateCall(PrintfFunc, {ConstantExpr::getBitCast(MsgGV, CharPtrTy)});
}

Function* SyscallProtect::createSyscallWrapper(Module &M, StringRef OrigName, int SyscallNum, 
                                                int NumArgs, ArrayRef<Type*> CustomArgTypes) {
    LLVMContext &Ctx = M.getContext();
    
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    
    SmallVector<Type*, 6> ArgTypes = CustomArgTypes.empty() ? getArgTypes(Ctx, OrigName) : SmallVector<Type*, 6>(CustomArgTypes.begin(), CustomArgTypes.end());
    
    FunctionType *FuncTy = FunctionType::get(Int64Ty, ArgTypes, false);
    
    std::string WrapperName = "__syscall_" + OrigName.str();
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        WrapperName,
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);
    
    SmallVector<Value*, 6> Args;
    for (Argument &Arg : Func->args()) {
        Args.push_back(&Arg);
    }
    
    while (Args.size() < 6) {
        Args.push_back(Constant::getNullValue(PtrTy));
    }
    
    Value *A0 = Args[0]->getType()->isPointerTy() 
        ? Builder.CreatePtrToInt(Args[0], Int64Ty)
        : Builder.CreateZExtOrTrunc(Args[0], Int64Ty);
    Value *A1 = Args[1]->getType()->isPointerTy() 
        ? Builder.CreatePtrToInt(Args[1], Int64Ty)
        : Builder.CreateZExtOrTrunc(Args[1], Int64Ty);
    Value *A2 = Args.size() > 2 
        ? (Args[2]->getType()->isPointerTy() 
            ? Builder.CreatePtrToInt(Args[2], Int64Ty)
            : Builder.CreateZExtOrTrunc(Args[2], Int64Ty))
        : ConstantInt::get(Int64Ty, 0);
    Value *A3 = Args.size() > 3 
        ? (Args[3]->getType()->isPointerTy() 
            ? Builder.CreatePtrToInt(Args[3], Int64Ty)
            : Builder.CreateZExtOrTrunc(Args[3], Int64Ty))
        : ConstantInt::get(Int64Ty, 0);
    Value *A4 = Args.size() > 4 
        ? (Args[4]->getType()->isPointerTy() 
            ? Builder.CreatePtrToInt(Args[4], Int64Ty)
            : Builder.CreateZExtOrTrunc(Args[4], Int64Ty))
        : ConstantInt::get(Int64Ty, 0);
    Value *A5 = Args.size() > 5 
        ? (Args[5]->getType()->isPointerTy() 
            ? Builder.CreatePtrToInt(Args[5], Int64Ty)
            : Builder.CreateZExtOrTrunc(Args[5], Int64Ty))
        : ConstantInt::get(Int64Ty, 0);
    
    FunctionType *AsmTy = FunctionType::get(Int64Ty, 
        {Int64Ty, Int64Ty, Int64Ty, Int64Ty, Int64Ty, Int64Ty, Int64Ty}, false);
    
    InlineAsm *Asm = InlineAsm::get(AsmTy,
        "svc #0",
        "={x0},{x0},{x1},{x2},{x3},{x4},{x5},{x8},~{memory},~{cc}",
        true, false);
    
    Value *SysNum = ConstantInt::get(Int64Ty, SyscallNum);
    Value *Result = Builder.CreateCall(Asm, {A0, A1, A2, A3, A4, A5, SysNum});
    Builder.CreateRet(Result);
    
    return Func;
}

Function* SyscallProtect::createMemcmpWrapper(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    Type *Int8Ty = Type::getInt8Ty(Ctx);
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(Int32Ty, {PtrTy, PtrTy, Int64Ty}, false);
    
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "__syscall_memcmp",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    
    Argument *S1Arg = &*Func->arg_begin();
    Argument *S2Arg = &*(Func->arg_begin() + 1);
    Argument *NArg = &*(Func->arg_begin() + 2);
    
    S1Arg->setName("s1");
    S2Arg->setName("s2");
    NArg->setName("n");
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *LoopCondBB = BasicBlock::Create(Ctx, "loop.cond", Func);
    BasicBlock *LoopBodyBB = BasicBlock::Create(Ctx, "loop.body", Func);
    BasicBlock *ReturnDiffBB = BasicBlock::Create(Ctx, "return.diff", Func);
    BasicBlock *ReturnZeroBB = BasicBlock::Create(Ctx, "return.zero", Func);
    
    IRBuilder<> EntryBuilder(EntryBB);
    Value *Zero64 = ConstantInt::get(Int64Ty, 0);
    EntryBuilder.CreateBr(LoopCondBB);
    
    IRBuilder<> CondBuilder(LoopCondBB);
    PHINode *I = CondBuilder.CreatePHI(Int64Ty, 2, "i");
    Value *Cond = CondBuilder.CreateICmpULT(I, NArg, "cmp");
    CondBuilder.CreateCondBr(Cond, LoopBodyBB, ReturnZeroBB);
    
    IRBuilder<> BodyBuilder(LoopBodyBB);
    Value *Ptr1 = BodyBuilder.CreateGEP(Int8Ty, S1Arg, I, "ptr1");
    Value *Ptr2 = BodyBuilder.CreateGEP(Int8Ty, S2Arg, I, "ptr2");
    Value *Byte1 = BodyBuilder.CreateLoad(Int8Ty, Ptr1, "byte1");
    Value *Byte2 = BodyBuilder.CreateLoad(Int8Ty, Ptr2, "byte2");
    Value *ByteEq = BodyBuilder.CreateICmpEQ(Byte1, Byte2, "byte_eq");
    
    BasicBlock *ContinueBB = BasicBlock::Create(Ctx, "continue", Func);
    BodyBuilder.CreateCondBr(ByteEq, ContinueBB, ReturnDiffBB);
    
    IRBuilder<> ContinueBuilder(ContinueBB);
    Value *NextI = ContinueBuilder.CreateAdd(I, ConstantInt::get(Int64Ty, 1), "next_i");
    ContinueBuilder.CreateBr(LoopCondBB);
    
    I->addIncoming(Zero64, EntryBB);
    I->addIncoming(NextI, ContinueBB);
    
    IRBuilder<> DiffBuilder(ReturnDiffBB);
    Value *Byte1Ext = DiffBuilder.CreateZExt(Byte1, Int32Ty, "byte1_ext");
    Value *Byte2Ext = DiffBuilder.CreateZExt(Byte2, Int32Ty, "byte2_ext");
    Value *Diff = DiffBuilder.CreateSub(Byte1Ext, Byte2Ext, "diff");
    DiffBuilder.CreateRet(Diff);
    
    IRBuilder<> ZeroBuilder(ReturnZeroBB);
    ZeroBuilder.CreateRet(ConstantInt::get(Int32Ty, 0));
    
    return Func;
}

Function* SyscallProtect::createGetenvWrapper(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *Int8Ty = Type::getInt8Ty(Ctx);
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(PtrTy, {PtrTy}, false);
    
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "__syscall_getenv",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    
    Argument *NameArg = &*Func->arg_begin();
    NameArg->setName("name");
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *FoundBB = BasicBlock::Create(Ctx, "found", Func);
    BasicBlock *NotFoundBB = BasicBlock::Create(Ctx, "notfound", Func);
    BasicBlock *LoopBB = BasicBlock::Create(Ctx, "loop", Func);
    BasicBlock *CheckBB = BasicBlock::Create(Ctx, "check", Func);
    BasicBlock *CompareBB = BasicBlock::Create(Ctx, "compare", Func);
    BasicBlock *NextBB = BasicBlock::Create(Ctx, "next", Func);
    
    IRBuilder<> EntryBuilder(EntryBB);
    
    Function *GetEnvironFunc = M.getFunction("get_environ_ptr");
    Value *EnvironPtr = nullptr;
    if (GetEnvironFunc) {
        EnvironPtr = EntryBuilder.CreateCall(GetEnvironFunc);
    } else {
        PointerType *PtrPtrTy = PointerType::get(Ctx, 0);
        GlobalVariable *EnvironGV = M.getGlobalVariable("environ");
        if (!EnvironGV) {
            EnvironGV = new GlobalVariable(
                M, PtrPtrTy, false,
                GlobalValue::ExternalLinkage, nullptr,
                "environ"
            );
        }
        EnvironPtr = EntryBuilder.CreateLoad(PtrPtrTy, EnvironGV, "environ_ptr");
    }
    
    Value *Zero64 = ConstantInt::get(Type::getInt64Ty(Ctx), 0);
    EntryBuilder.CreateBr(LoopBB);
    
    IRBuilder<> LoopBuilder(LoopBB);
    PHINode *Idx = LoopBuilder.CreatePHI(Type::getInt64Ty(Ctx), 2, "idx");
    Value *EnvEntryPtr = LoopBuilder.CreateGEP(PtrTy, EnvironPtr, Idx, "env_entry_ptr");
    Value *EnvEntry = LoopBuilder.CreateLoad(PtrTy, EnvEntryPtr, "env_entry");
    LoopBuilder.CreateBr(CheckBB);
    
    IRBuilder<> CheckBuilder(CheckBB);
    Value *IsNull = CheckBuilder.CreateICmpEQ(EnvEntry, ConstantPointerNull::get(PtrTy), "is_null");
    CheckBuilder.CreateCondBr(IsNull, NotFoundBB, CompareBB);
    
    IRBuilder<> CompareBuilder(CompareBB);
    Value *NameLen = CompareBuilder.CreateCall(
        M.getOrInsertFunction("strlen", FunctionType::get(Type::getInt64Ty(Ctx), {PtrTy}, false)),
        {NameArg}
    );
    
    Value *EqPos = CompareBuilder.CreateCall(
        M.getOrInsertFunction("strchr", FunctionType::get(PtrTy, {PtrTy, Int8Ty}, false)),
        {EnvEntry, ConstantInt::get(Int8Ty, '=')}
    );
    
    Value *KeyLen = CompareBuilder.CreatePtrToInt(
        CompareBuilder.CreateSub(EqPos, EnvEntry),
        Type::getInt64Ty(Ctx)
    );
    
    Value *LenMatch = CompareBuilder.CreateICmpEQ(NameLen, KeyLen, "len_match");
    
    Value *KeyMatch = CompareBuilder.CreateCall(
        M.getOrInsertFunction("strncmp", FunctionType::get(Type::getInt32Ty(Ctx), {PtrTy, PtrTy, Type::getInt64Ty(Ctx)}, false)),
        {NameArg, EnvEntry, NameLen}
    );
    KeyMatch = CompareBuilder.CreateICmpEQ(KeyMatch, ConstantInt::get(Type::getInt32Ty(Ctx), 0), "key_match");
    
    Value *BothMatch = CompareBuilder.CreateAnd(LenMatch, KeyMatch, "both_match");
    CompareBuilder.CreateCondBr(BothMatch, FoundBB, NextBB);
    
    IRBuilder<> NextBuilder(NextBB);
    Value *NextIdx = NextBuilder.CreateAdd(Idx, ConstantInt::get(Type::getInt64Ty(Ctx), 1), "next_idx");
    NextBuilder.CreateBr(LoopBB);
    
    Idx->addIncoming(Zero64, EntryBB);
    Idx->addIncoming(NextIdx, NextBB);
    
    IRBuilder<> FoundBuilder(FoundBB);
    Value *ValuePtr = FoundBuilder.CreateGEP(Int8Ty, EqPos, ConstantInt::get(Type::getInt64Ty(Ctx), 1), "value_ptr");
    FoundBuilder.CreateRet(ValuePtr);
    
    IRBuilder<> NotFoundBuilder(NotFoundBB);
    NotFoundBuilder.CreateRet(ConstantPointerNull::get(PtrTy));
    
    return Func;
}

Function* SyscallProtect::createGetaddrinfoWrapper(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(Int32Ty, {PtrTy, PtrTy, PtrTy, PtrTy}, false);
    
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "__syscall_getaddrinfo",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);
    
    FunctionCallee RealFunc = M.getOrInsertFunction(
        "__real_getaddrinfo",
        FunctionType::get(Int32Ty, {PtrTy, PtrTy, PtrTy, PtrTy}, false)
    );
    
    SmallVector<Value*, 4> Args;
    for (Argument &Arg : Func->args()) {
        Args.push_back(&Arg);
    }
    
    Value *Result = Builder.CreateCall(RealFunc, Args);
    Builder.CreateRet(Result);
    
    return Func;
}

Function* SyscallProtect::createSystemWrapper(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(Int32Ty, {PtrTy}, false);
    
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "__syscall_system",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);
    
    FunctionCallee RealFunc = M.getOrInsertFunction(
        "__real_system",
        FunctionType::get(Int32Ty, {PtrTy}, false)
    );
    
    Value *Result = Builder.CreateCall(RealFunc, {&*Func->arg_begin()});
    Builder.CreateRet(Result);
    
    return Func;
}

Function* SyscallProtect::createPopenWrapper(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    PointerType *FilePtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(FilePtrTy, {PtrTy, PtrTy}, false);
    
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "__syscall_popen",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);
    
    FunctionCallee RealFunc = M.getOrInsertFunction(
        "__real_popen",
        FunctionType::get(FilePtrTy, {PtrTy, PtrTy}, false)
    );
    
    SmallVector<Value*, 2> Args;
    for (Argument &Arg : Func->args()) {
        Args.push_back(&Arg);
    }
    
    Value *Result = Builder.CreateCall(RealFunc, Args);
    Builder.CreateRet(Result);
    
    return Func;
}

Function* SyscallProtect::createExecvpWrapper(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(Int32Ty, {PtrTy, PtrTy}, false);
    
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "__syscall_execvp",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);
    
    FunctionCallee RealFunc = M.getOrInsertFunction(
        "__real_execvp",
        FunctionType::get(Int32Ty, {PtrTy, PtrTy}, false)
    );
    
    SmallVector<Value*, 2> Args;
    for (Argument &Arg : Func->args()) {
        Args.push_back(&Arg);
    }
    
    Value *Result = Builder.CreateCall(RealFunc, Args);
    Builder.CreateRet(Result);
    
    return Func;
}

Function* SyscallProtect::createExecvpeWrapper(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(Int32Ty, {PtrTy, PtrTy, PtrTy}, false);
    
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "__syscall_execvpe",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);
    
    FunctionCallee RealFunc = M.getOrInsertFunction(
        "__real_execvpe",
        FunctionType::get(Int32Ty, {PtrTy, PtrTy, PtrTy}, false)
    );
    
    SmallVector<Value*, 3> Args;
    for (Argument &Arg : Func->args()) {
        Args.push_back(&Arg);
    }
    
    Value *Result = Builder.CreateCall(RealFunc, Args);
    Builder.CreateRet(Result);
    
    return Func;
}

Function* SyscallProtect::createRemoveWrapper(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(Int32Ty, {PtrTy}, false);
    
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "__syscall_remove",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    
    Argument *PathArg = &*Func->arg_begin();
    PathArg->setName("path");
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *TryUnlinkBB = BasicBlock::Create(Ctx, "try_unlink", Func);
    BasicBlock *CheckBB = BasicBlock::Create(Ctx, "check", Func);
    BasicBlock *TryRmdirBB = BasicBlock::Create(Ctx, "try_rmdir", Func);
    BasicBlock *RetBB = BasicBlock::Create(Ctx, "ret", Func);
    
    IRBuilder<> EntryBuilder(EntryBB);
    EntryBuilder.CreateBr(TryUnlinkBB);
    
    IRBuilder<> UnlinkBuilder(TryUnlinkBB);
    Function *UnlinkWrapper = M.getFunction("__syscall_unlink");
    if (!UnlinkWrapper) {
        UnlinkWrapper = createSyscallWrapper(M, "unlink", 87, 1, {PtrTy});
    }
    Value *UnlinkResult = UnlinkBuilder.CreateCall(UnlinkWrapper, {PathArg});
    UnlinkBuilder.CreateBr(CheckBB);
    
    IRBuilder<> CheckBuilder(CheckBB);
    PHINode *Result = CheckBuilder.CreatePHI(Int32Ty, 2, "result");
    Result->addIncoming(UnlinkResult, TryUnlinkBB);
    Value *Success = CheckBuilder.CreateICmpEQ(Result, ConstantInt::get(Int32Ty, 0), "success");
    CheckBuilder.CreateCondBr(Success, RetBB, TryRmdirBB);
    
    IRBuilder<> RmdirBuilder(TryRmdirBB);
    Function *RmdirWrapper = M.getFunction("__syscall_rmdir");
    if (!RmdirWrapper) {
        RmdirWrapper = createSyscallWrapper(M, "rmdir", 84, 1, {PtrTy});
    }
    Value *RmdirResult = RmdirBuilder.CreateCall(RmdirWrapper, {PathArg});
    RmdirBuilder.CreateBr(RetBB);
    
    Result->addIncoming(RmdirResult, TryRmdirBB);
    
    IRBuilder<> RetBuilder(RetBB);
    RetBuilder.CreateRet(Result);
    
    return Func;
}

Function* SyscallProtect::createFopenWrapper(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    PointerType *FilePtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(FilePtrTy, {PtrTy, PtrTy}, false);
    
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "__syscall_fopen",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);
    
    FunctionCallee RealFunc = M.getOrInsertFunction(
        "__real_fopen",
        FunctionType::get(FilePtrTy, {PtrTy, PtrTy}, false)
    );
    
    SmallVector<Value*, 2> Args;
    for (Argument &Arg : Func->args()) {
        Args.push_back(&Arg);
    }
    
    Value *Result = Builder.CreateCall(RealFunc, Args);
    Builder.CreateRet(Result);
    
    return Func;
}

bool SyscallProtect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] SyscallProtect: Starting syscall protection\n";
        errs() << "[DEBUG] SyscallProtect: Target: " << M.getTargetTriple().str() << "\n";
    }
    
    Triple TargetTriple(M.getTargetTriple());
    if (TargetTriple.getArch() != Triple::aarch64) {
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[DEBUG] SyscallProtect: Unsupported architecture (only ARM64 supported)\n";
        }
        return false;
    }
    
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] SyscallProtect: Architecture: AArch64\n";
    }
    
    bool Changed = false;
    LLVMContext &Ctx = M.getContext();
    PointerType *PtrTy = PointerType::get(Ctx, 0);
    Value *NullPtr = ConstantPointerNull::get(PtrTy);
    
    StringMap<Function*> WrapperCache;
    
    int TotalCalls = 0;
    int ReplacedCalls = 0;
    
    for (Function &F : M) {
        if (F.isDeclaration()) continue;
        
        SmallVector<CallInst*, 16> ToReplace;
        
        for (Instruction &I : instructions(F)) {
            if (CallInst *CI = dyn_cast<CallInst>(&I)) {
                Function *Callee = CI->getCalledFunction();
                if (Callee && shouldReplace(Callee->getName())) {
                    ToReplace.push_back(CI);
                    TotalCalls++;
                }
            }
        }
        
        for (CallInst *CI : ToReplace) {
            Function *Callee = CI->getCalledFunction();
            StringRef Name = Callee->getName();
            
            IRBuilder<> Builder(CI);
            
            Function *Wrapper = nullptr;
            
            StringRef BaseName = Name.starts_with("__orig_") ? Name.substr(7) : Name;
            
            if (isManualImplFunction(Name)) {
                auto It = WrapperCache.find(BaseName);
                if (It != WrapperCache.end()) {
                    Wrapper = It->second;
                } else {
                    if (BaseName == "memcmp") {
                        Wrapper = createMemcmpWrapper(M);
                    } else if (BaseName == "getenv") {
                        Wrapper = createGetenvWrapper(M);
                    } else if (BaseName == "getaddrinfo") {
                        Wrapper = createGetaddrinfoWrapper(M);
                    } else if (BaseName == "popen") {
                        Wrapper = createPopenWrapper(M);
                    } else if (BaseName == "system") {
                        Wrapper = createSystemWrapper(M);
                    } else if (BaseName == "execvp") {
                        Wrapper = createExecvpWrapper(M);
                    } else if (BaseName == "execvpe") {
                        Wrapper = createExecvpeWrapper(M);
                    } else if (BaseName == "remove") {
                        Wrapper = createRemoveWrapper(M);
                    } else if (BaseName == "fopen") {
                        Wrapper = createFopenWrapper(M);
                    }
                    if (Wrapper) {
                        WrapperCache[BaseName] = Wrapper;
                    }
                }
                
                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[DEBUG] SyscallProtect: Replacing " << Name 
                           << " with manual implementation in " << F.getName() << "\n";
                }
            } else {
                int SysNum = getSyscallNum(Name);
                int NumArgs = getNumArgs(Name);
                
                if (isIRObfuscationDebugEnabled()) {
                    errs() << "[DEBUG] SyscallProtect: Replacing " << Name 
                           << " with syscall " << SysNum << " in " << F.getName() << "\n";
                }
                
                auto It = WrapperCache.find(Name);
                if (It == WrapperCache.end()) {
                    Wrapper = createSyscallWrapper(M, Name, SysNum, NumArgs);
                    WrapperCache[Name] = Wrapper;
                } else {
                    Wrapper = It->second;
                }
            }
            
            if (!Wrapper) continue;
            
            SmallVector<Value*, 6> Args;
            for (unsigned i = 0; i < CI->arg_size(); ++i) {
                Args.push_back(CI->getArgOperand(i));
            }
            
            if (isSyscallFunction(Name)) {
                int NumArgs = getNumArgs(Name);
                while (Args.size() < (unsigned)NumArgs) {
                    Args.push_back(NullPtr);
                }
            }
            
            Value *Result = Builder.CreateCall(Wrapper, Args);
            
            if (CI->getType() != Result->getType()) {
                Result = Builder.CreateSExtOrTrunc(Result, CI->getType());
            }
            
            CI->replaceAllUsesWith(Result);
            CI->eraseFromParent();
            Changed = true;
            ReplacedCalls++;
        }
    }
    
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] SyscallProtect: Summary:\n";
        errs() << "[DEBUG] SyscallProtect:   Total calls found: " << TotalCalls << "\n";
        errs() << "[DEBUG] SyscallProtect:   Calls replaced: " << ReplacedCalls << "\n";
        errs() << "[DEBUG] SyscallProtect:   Result: " << (Changed ? "MODIFIED" : "UNCHANGED") << "\n";
    }
    
    return Changed;
}

ModulePass *llvm::createSyscallProtectPass() {
    return new SyscallProtect();
}

INITIALIZE_PASS_BEGIN(SyscallProtect, "syscallprotect", "Protect syscall functions with direct syscalls", false, false)
INITIALIZE_PASS_END(SyscallProtect, "syscallprotect", "Protect syscall functions with direct syscalls", false, false)
