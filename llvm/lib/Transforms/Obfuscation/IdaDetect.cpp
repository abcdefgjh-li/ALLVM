//===- IdaDetect.cpp - 调试器检测注入Pass -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现调试器检测注入Pass，在程序入口点注入检测代码
// 检测IDA调试器默认端口23946，并复用TracerPid检测
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/IdaDetect.h"
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

#define DEBUG_TYPE "idadetect"

using namespace llvm;

namespace {

struct IdaDetect : public ModulePass {
    static char ID;

    IdaDetect() : ModulePass(ID) {
        initializeIdaDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"DebuggerDetect"};
    }

    bool runOnModule(Module &M) override;
    
    Function* createDebuggerPortCheckFunc(Module &M, Function *reportFunc);
};

}

char IdaDetect::ID = 0;

Function* IdaDetect::createDebuggerPortCheckFunc(Module &M, Function *reportFunc) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int16Ty = Type::getInt16Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "check_debugger_port",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *CheckPortBB = BasicBlock::Create(Ctx, "check_port", Func);
    BasicBlock *FoundBB = BasicBlock::Create(Ctx, "found", Func);
    BasicBlock *CleanupBB = BasicBlock::Create(Ctx, "cleanup", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    // 创建socket
    FunctionCallee SocketFunc = M.getOrInsertFunction(
        "socket",
        FunctionType::get(Int32Ty, {Int32Ty, Int32Ty, Int32Ty}, false)
    );
    
    // AF_INET = 2, SOCK_STREAM = 1
    Value *SockFd = Builder.CreateCall(SocketFunc, {
        ConstantInt::get(Int32Ty, 2),
        ConstantInt::get(Int32Ty, 1),
        ConstantInt::get(Int32Ty, 0)
    });
    
    Value *SockValid = Builder.CreateICmpSGE(SockFd, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(SockValid, CheckPortBB, ExitBB);
    
    Builder.SetInsertPoint(CheckPortBB);
    
    // 创建sockaddr_in结构
    Type *SockaddrInTy = StructType::create(Ctx, "sockaddr_in");
    StructType *SockaddrInStruct = cast<StructType>(SockaddrInTy);
    SockaddrInStruct->setBody({
        Int16Ty,      // sin_family
        Int16Ty,      // sin_port
        Int32Ty,      // sin_addr
        ArrayType::get(Type::getInt8Ty(Ctx), 8)  // sin_zero
    });
    
    Value *Addr = Builder.CreateAlloca(SockaddrInStruct, nullptr, "addr");
    
    // 设置sin_family = AF_INET
    Value *FamilyPtr = Builder.CreateGEP(SockaddrInStruct, Addr, {
        ConstantInt::get(Int32Ty, 0),
        ConstantInt::get(Int32Ty, 0)
    });
    Builder.CreateStore(ConstantInt::get(Int16Ty, 2), FamilyPtr);
    
    // 设置sin_port = htons(23946)
    FunctionCallee HtonsFunc = M.getOrInsertFunction(
        "htons",
        FunctionType::get(Int16Ty, {Int16Ty}, false)
    );
    Value *Port = Builder.CreateCall(HtonsFunc, {ConstantInt::get(Int16Ty, 23946)});
    Value *PortPtr = Builder.CreateGEP(SockaddrInStruct, Addr, {
        ConstantInt::get(Int32Ty, 0),
        ConstantInt::get(Int32Ty, 1)
    });
    Builder.CreateStore(Port, PortPtr);
    
    // 设置sin_addr = inet_addr("127.0.0.1")
    FunctionCallee InetAddrFunc = M.getOrInsertFunction(
        "inet_addr",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    
    Constant *LocalhostStr = ConstantDataArray::getString(Ctx, "127.0.0.1");
    GlobalVariable *LocalhostGV = new GlobalVariable(
        M, LocalhostStr->getType(), true,
        GlobalValue::PrivateLinkage, LocalhostStr,
        ".ida.localhost"
    );
    
    Value *AddrVal = Builder.CreateCall(InetAddrFunc, {
        ConstantExpr::getBitCast(LocalhostGV, CharPtrTy)
    });
    Value *AddrPtr = Builder.CreateGEP(SockaddrInStruct, Addr, {
        ConstantInt::get(Int32Ty, 0),
        ConstantInt::get(Int32Ty, 2)
    });
    Builder.CreateStore(AddrVal, AddrPtr);
    
    // 尝试连接
    FunctionCallee ConnectFunc = M.getOrInsertFunction(
        "connect",
        FunctionType::get(Int32Ty, {Int32Ty, CharPtrTy, Int32Ty}, false)
    );
    
    Value *AddrCast = Builder.CreateBitCast(Addr, CharPtrTy);
    Value *ConnectRet = Builder.CreateCall(ConnectFunc, {
        SockFd, AddrCast, ConstantInt::get(Int32Ty, 16)
    });
    
    // connect返回0表示成功连接（例如IDA调试器正在监听）
    Value *DebuggerFound = Builder.CreateICmpEQ(ConnectRet, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(DebuggerFound, FoundBB, CleanupBB);
    
    Builder.SetInsertPoint(FoundBB);
    
    // 调用报告函数
    Builder.CreateCall(reportFunc);
    Builder.CreateUnreachable();

    Builder.SetInsertPoint(CleanupBB);

    FunctionCallee CloseFunc = M.getOrInsertFunction(
        "close",
        FunctionType::get(Int32Ty, {Int32Ty}, false)
    );
    Builder.CreateCall(CloseFunc, {SockFd});
    Builder.CreateBr(ExitBB);

    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool IdaDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] IdaDetect: Injecting debugger detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 使用公共模块创建报告函数
    Function *ReportFunc = DetectUtils::createReportAndKillFunc(M, "Debugger");
    
    // 在同一选项下整合端口监听和TracerPid两类调试器检测
    Function *CheckFunc = createDebuggerPortCheckFunc(M, ReportFunc);
    Function *TracerPidCheckFunc = DetectUtils::createTracerPidCheckFunc(M, ReportFunc);

    // ptrace自附加反调试：fork子进程PTRACE_TRACEME，父进程作为tracer
    // 外部进程无法再ptrace附加，监控线程检测TracerPid是否被剥离
    Function *PtraceSelfAttachFunc = DetectUtils::createPtraceSelfAttachFunc(M, ReportFunc);
    
    BasicBlock &EntryBB = MainFunc->getEntryBlock();
    IRBuilder<> Builder(&EntryBB, EntryBB.getFirstInsertionPt());
    Builder.CreateCall(CheckFunc);
    Builder.CreateCall(TracerPidCheckFunc);
    Builder.CreateCall(PtraceSelfAttachFunc);
    return true;
}

ModulePass *llvm::createIdaDetectPass() {
    return new IdaDetect();
}

INITIALIZE_PASS_BEGIN(IdaDetect, "idadetect", "Inject debugger detection at program start", false, false)
INITIALIZE_PASS_END(IdaDetect, "idadetect", "Inject debugger detection at program start", false, false)
