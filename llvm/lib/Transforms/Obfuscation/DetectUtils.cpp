//===- DetectUtils.cpp - 检测工具公共模块实现 ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/DetectUtils.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "detectutils"

using namespace llvm;

Constant* DetectUtils::createGlobalString(Module &M, const std::string &str, const std::string &name) {
    LLVMContext &Ctx = M.getContext();
    Constant *StrConst = ConstantDataArray::getString(Ctx, str);
    GlobalVariable *StrGV = new GlobalVariable(
        M, StrConst->getType(), true,
        GlobalValue::PrivateLinkage, StrConst,
        name
    );
    StrGV->setSection(".AProtect.rodata");
    return ConstantExpr::getBitCast(StrGV, PointerType::get(Ctx, 0));
}

static void createStderrWrite(Module &M, IRBuilder<> &Builder,
                              const std::string &message,
                              const std::string &globalName) {
    LLVMContext &Ctx = M.getContext();
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);

    FunctionCallee WriteFunc = M.getOrInsertFunction(
        "write",
        FunctionType::get(Int64Ty, {Int32Ty, CharPtrTy, Int64Ty}, false)
    );

    Constant *MsgPtr = DetectUtils::createGlobalString(M, message, globalName);
    Builder.CreateCall(WriteFunc, {
        ConstantInt::get(Int32Ty, 2),
        MsgPtr,
        ConstantInt::get(Int64Ty, message.size())
    });
}

static void createAProtectBannerWrite(Module &M, IRBuilder<> &Builder) {
    createStderrWrite(M, Builder, "A-Protector\n", ".detect.banner");
    createStderrWrite(M, Builder, "Protection v1.2.0\n", ".detect.version");
}

Function* DetectUtils::createReportAndKillFunc(Module &M, const std::string &detectName) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "detect_report_and_kill",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);
    Func->addFnAttr(Attribute::NoReturn);
    Func->addFnAttr(Attribute::Cold);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);

    createAProtectBannerWrite(M, Builder);

    // 详细检测原因仅在 debug 模式下输出。
    if (isIRObfuscationDebugEnabled()) {
        createStderrWrite(
            M,
            Builder,
            "[AProtect] " + detectName + " detected, exiting.\n",
            ".detect.report.debug"
        );
    }

    // 获取进程ID并终止
    FunctionCallee GetpidFunc = M.getOrInsertFunction(
        "getpid",
        FunctionType::get(Int32Ty, {}, false)
    );
    
    FunctionCallee KillFunc = M.getOrInsertFunction(
        "kill",
        FunctionType::get(Int32Ty, {Int32Ty, Int32Ty}, false)
    );
    
    Value *Pid = Builder.CreateCall(GetpidFunc);
    Value *Sigkill = ConstantInt::get(Int32Ty, 9);
    Builder.CreateCall(KillFunc, {Pid, Sigkill});

    // 使用内联汇编确保终止（根据架构选择正确的指令）
    FunctionType *AsmTy = FunctionType::get(VoidTy, {}, false);
    std::string Triple = M.getTargetTriple().getTriple();
    std::string AsmInst;
    if (Triple.find("aarch64") != std::string::npos || Triple.find("arm64") != std::string::npos) {
        AsmInst = "brk #0";
    } else if (Triple.find("arm") != std::string::npos) {
        AsmInst = "bkpt #0";
    } else if (Triple.find("x86_64") != std::string::npos || Triple.find("amd64") != std::string::npos) {
        AsmInst = "int3";
    } else if (Triple.find("i386") != std::string::npos || Triple.find("i686") != std::string::npos || Triple.find("x86") != std::string::npos) {
        AsmInst = "int3";
    } else {
        // 默认使用空汇编，让 kill 函数处理终止
        Builder.CreateUnreachable();
        return Func;
    }
    InlineAsm *Asm = InlineAsm::get(AsmTy, AsmInst, "", true, false);
    Builder.CreateCall(Asm);

    Builder.CreateUnreachable();
    
    return Func;
}

Function* DetectUtils::createStealthKillFunc(Module &M, const DetectOptions &opts) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "stealth_kill",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *DelayBB = BasicBlock::Create(Ctx, "delay", Func);
    BasicBlock *KillBB = BasicBlock::Create(Ctx, "kill", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    // 隐蔽模式：先随机延迟，再终止
    if (opts.StealthMode) {
        Builder.CreateBr(DelayBB);
        
        Builder.SetInsertPoint(DelayBB);
        
        // 初始化随机数种子
        FunctionCallee SrandFunc = M.getOrInsertFunction(
            "srand",
            FunctionType::get(VoidTy, {Int32Ty}, false)
        );
        
        FunctionCallee TimeFunc = M.getOrInsertFunction(
            "time",
            FunctionType::get(Type::getInt64Ty(Ctx), {CharPtrTy}, false)
        );
        
        Value *TimeVal = Builder.CreateCall(TimeFunc, {ConstantPointerNull::get(CharPtrTy)});
        Value *TimeInt = Builder.CreateTrunc(TimeVal, Int32Ty);
        Builder.CreateCall(SrandFunc, {TimeInt});
        
        // 计算随机延迟
        FunctionCallee RandFunc = M.getOrInsertFunction(
            "rand",
            FunctionType::get(Int32Ty, {}, false)
        );
        
        FunctionCallee UsleepFunc = M.getOrInsertFunction(
            "usleep",
            FunctionType::get(Int32Ty, {Int32Ty}, false)
        );
        
        Value *RandVal = Builder.CreateCall(RandFunc);
        int range = opts.MaxDelayMs - opts.MinDelayMs;
        Value *RangeVal = ConstantInt::get(Int32Ty, range);
        Value *MinVal = ConstantInt::get(Int32Ty, opts.MinDelayMs);
        
        Value *ModVal = Builder.CreateURem(RandVal, RangeVal);
        Value *DelayMs = Builder.CreateAdd(ModVal, MinVal);
        Value *DelayUs = Builder.CreateMul(DelayMs, ConstantInt::get(Int32Ty, 1000));
        
        Builder.CreateCall(UsleepFunc, {DelayUs});
        Builder.CreateBr(KillBB);
    } else {
        Builder.CreateBr(KillBB);
    }
    
    Builder.SetInsertPoint(KillBB);
    
    // 调用统一的报告和终止函数
    Function *ReportFunc = createReportAndKillFunc(M);
    Builder.CreateCall(ReportFunc);
    
    Builder.CreateRetVoid();
    
    return Func;
}

Function* DetectUtils::createRandomDelayFunc(Module &M, int minMs, int maxMs) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "random_delay",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);
    
    // 初始化随机数种子
    FunctionCallee SrandFunc = M.getOrInsertFunction(
        "srand",
        FunctionType::get(VoidTy, {Int32Ty}, false)
    );
    
    FunctionCallee TimeFunc = M.getOrInsertFunction(
        "time",
        FunctionType::get(Type::getInt64Ty(Ctx), {CharPtrTy}, false)
    );
    
    Value *TimeVal = Builder.CreateCall(TimeFunc, {ConstantPointerNull::get(CharPtrTy)});
    Value *TimeInt = Builder.CreateTrunc(TimeVal, Int32Ty);
    Builder.CreateCall(SrandFunc, {TimeInt});
    
    // 计算随机延迟
    FunctionCallee RandFunc = M.getOrInsertFunction(
        "rand",
        FunctionType::get(Int32Ty, {}, false)
    );
    
    FunctionCallee UsleepFunc = M.getOrInsertFunction(
        "usleep",
        FunctionType::get(Int32Ty, {Int32Ty}, false)
    );
    
    Value *RandVal = Builder.CreateCall(RandFunc);
    int range = maxMs - minMs;
    Value *RangeVal = ConstantInt::get(Int32Ty, range);
    Value *MinVal = ConstantInt::get(Int32Ty, minMs);
    
    Value *ModVal = Builder.CreateURem(RandVal, RangeVal);
    Value *DelayMs = Builder.CreateAdd(ModVal, MinVal);
    Value *DelayUs = Builder.CreateMul(DelayMs, ConstantInt::get(Int32Ty, 1000));
    
    Builder.CreateCall(UsleepFunc, {DelayUs});
    Builder.CreateRetVoid();
    
    return Func;
}

Function* DetectUtils::createThreadFunc(Module &M, Function *checkFunc, const DetectOptions &opts) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    // 创建线程函数
    FunctionType *ThreadFuncTy = FunctionType::get(VoidTy, {CharPtrTy}, false);
    Function *ThreadFunc = Function::Create(
        ThreadFuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "detect_thread_func",
        &M
    );
    
    ThreadFunc->addFnAttr(Attribute::NoInline);
    
    BasicBlock *ThreadBB = BasicBlock::Create(Ctx, "entry", ThreadFunc);
    IRBuilder<> ThreadBuilder(ThreadBB);
    
    // 调用检测函数
    ThreadBuilder.CreateCall(checkFunc);
    ThreadBuilder.CreateRetVoid();
    
    // 创建启动线程函数
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "detect_start_thread",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);
    
    // 随机化线程属性
    if (opts.RandomThreadAttr) {
        // 设置随机线程名（使用prctl PR_SET_NAME）
        FunctionCallee SrandFunc = M.getOrInsertFunction(
            "srand",
            FunctionType::get(VoidTy, {Int32Ty}, false)
        );
        
        FunctionCallee TimeFunc = M.getOrInsertFunction(
            "time",
            FunctionType::get(Type::getInt64Ty(Ctx), {CharPtrTy}, false)
        );
        
        Value *TimeVal = Builder.CreateCall(TimeFunc, {ConstantPointerNull::get(CharPtrTy)});
        Value *TimeInt = Builder.CreateTrunc(TimeVal, Int32Ty);
        Builder.CreateCall(SrandFunc, {TimeInt});
        
        // 创建随机线程名
        FunctionCallee RandFunc = M.getOrInsertFunction(
            "rand",
            FunctionType::get(Int32Ty, {}, false)
        );

        // 创建一个随机名称缓冲区
        Type *NameBufTy = ArrayType::get(Type::getInt8Ty(Ctx), 16);
        Value *NameBuf = Builder.CreateAlloca(NameBufTy, nullptr, "thread_name");
        Value *NameBufPtr = Builder.CreateBitCast(NameBuf, CharPtrTy);

        FunctionCallee SprintfFunc = M.getOrInsertFunction(
            "sprintf",
            FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy}, true)
        );
        
        Constant *FmtStr = ConstantDataArray::getString(Ctx, "thr_%d");
        GlobalVariable *FmtGV = new GlobalVariable(
            M, FmtStr->getType(), true,
            GlobalValue::PrivateLinkage, FmtStr,
            ".thread.fmt"
        );
        FmtGV->setSection(".AProtect.rodata");
        
        Value *RandName = Builder.CreateCall(RandFunc);
        Builder.CreateCall(SprintfFunc, {
            NameBufPtr,
            ConstantExpr::getBitCast(FmtGV, CharPtrTy),
            RandName
        });
    }
    
    // 创建pthread线程
    Type *PthreadTy = StructType::create(Ctx, "pthread_t");
    Value *Thread = Builder.CreateAlloca(PthreadTy, nullptr, "thread");
    
    FunctionCallee PthreadCreateFunc = M.getOrInsertFunction(
        "pthread_create",
        FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy, CharPtrTy, CharPtrTy}, false)
    );
    
    Value *ThreadPtr = Builder.CreateBitCast(Thread, CharPtrTy);
    Value *ThreadFuncPtr = Builder.CreateBitCast(ThreadFunc, CharPtrTy);
    
    Builder.CreateCall(PthreadCreateFunc, {
        ThreadPtr,
        ConstantPointerNull::get(CharPtrTy),
        ThreadFuncPtr,
        ConstantPointerNull::get(CharPtrTy)
    });
    
    Builder.CreateRetVoid();
    
    return Func;
}

Function* DetectUtils::createTracerPidCheckFunc(Module &M, Function *reportFunc) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int8Ty = Type::getInt8Ty(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "check_tracerpid",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *OpenOkBB = BasicBlock::Create(Ctx, "open_ok", Func);
    BasicBlock *OpenFailBB = BasicBlock::Create(Ctx, "open_fail", Func);
    BasicBlock *LoopBB = BasicBlock::Create(Ctx, "loop", Func);
    BasicBlock *CheckLineBB = BasicBlock::Create(Ctx, "check_line", Func);
    BasicBlock *ParsePidBB = BasicBlock::Create(Ctx, "parse_pid", Func);
    BasicBlock *TracerDetectedBB = BasicBlock::Create(Ctx, "tracer_detected", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);
    
    IRBuilder<> Builder(EntryBB);
    
    // 打开 /proc/self/status
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
    
    FunctionCallee StrtoulFunc = M.getOrInsertFunction(
        "strtoul",
        FunctionType::get(Int64Ty, {CharPtrTy, CharPtrTy, Int32Ty}, false)
    );
    
    auto makeString = [&](const char *str) -> Constant* {
        return createGlobalString(M, str, ".tracer.str");
    };
    
    Constant *StatusPath = makeString("/proc/self/status");
    Constant *ReadMode = makeString("r");
    Constant *TracerPidNeedle = makeString("TracerPid:");
    
    Value *Fp = Builder.CreateCall(FopenFunc, {StatusPath, ReadMode});
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
    
    Value *FoundTracer = Builder.CreateCall(StrstrFunc, {LineBufPtr, TracerPidNeedle});
    Value *FoundTracerNotNull = Builder.CreateICmpNE(FoundTracer, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(FoundTracerNotNull, ParsePidBB, LoopBB);
    
    Builder.SetInsertPoint(ParsePidBB);
    
    // 找到TracerPid行，解析值
    // TracerPid:	0 格式，需要跳过"TracerPid:"和空白
    Constant *ColonNeedle = makeString(":");
    Value *ColonPos = Builder.CreateCall(StrstrFunc, {LineBufPtr, ColonNeedle});
    
    BasicBlock *ParseOkBB = BasicBlock::Create(Ctx, "parse_ok", Func);
    BasicBlock *ParseFailBB = BasicBlock::Create(Ctx, "parse_fail", Func);
    
    Value *ColonNotNull = Builder.CreateICmpNE(ColonPos, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(ColonNotNull, ParseOkBB, ParseFailBB);
    
    Builder.SetInsertPoint(ParseOkBB);
    
    // 跳过冒号和空白
    Value *ColonPlusOne = Builder.CreateGEP(Int8Ty, ColonPos, ConstantInt::get(Int64Ty, 1));
    
    // 跳过空白字符
    FunctionCallee IsspaceFunc = M.getOrInsertFunction(
        "isspace",
        FunctionType::get(Int32Ty, {Int32Ty}, false)
    );
    
    BasicBlock *SkipSpaceBB = BasicBlock::Create(Ctx, "skip_space", Func);
    BasicBlock *CheckPidBB = BasicBlock::Create(Ctx, "check_pid", Func);
    
    Builder.CreateBr(SkipSpaceBB);
    
    Builder.SetInsertPoint(SkipSpaceBB);
    PHINode *CurrentPtr = Builder.CreatePHI(CharPtrTy, 2, "ptr");
    CurrentPtr->addIncoming(ColonPlusOne, ParseOkBB);
    
    Value *CharVal = Builder.CreateLoad(Int8Ty, CurrentPtr);
    Value *CharInt = Builder.CreateSExt(CharVal, Int32Ty);
    Value *IsSpace = Builder.CreateCall(IsspaceFunc, {CharInt});
    Value *IsSpaceBool = Builder.CreateICmpNE(IsSpace, ConstantInt::get(Int32Ty, 0));
    
    Value *NextPtr = Builder.CreateGEP(Int8Ty, CurrentPtr, ConstantInt::get(Int64Ty, 1));
    CurrentPtr->addIncoming(NextPtr, SkipSpaceBB);
    
    Builder.CreateCondBr(IsSpaceBool, SkipSpaceBB, CheckPidBB);
    
    Builder.SetInsertPoint(CheckPidBB);
    
    // 解析PID值
    Value *PidValue = Builder.CreateCall(StrtoulFunc, {CurrentPtr, ConstantPointerNull::get(CharPtrTy), ConstantInt::get(Int32Ty, 10)});
    
    // 检查是否为0（非调试状态）
    Value *IsTracer = Builder.CreateICmpNE(PidValue, ConstantInt::get(Int64Ty, 0));
    Builder.CreateCondBr(IsTracer, TracerDetectedBB, ParseFailBB);
    
    Builder.SetInsertPoint(ParseFailBB);
    Builder.CreateBr(LoopBB);
    
    Builder.SetInsertPoint(TracerDetectedBB);
    Builder.CreateCall(FcloseFunc, {Fp});
    Builder.CreateCall(reportFunc);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool DetectUtils::injectToMain(Module &M, Function *checkFunc, const DetectOptions &opts) {
    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }
    
    BasicBlock &EntryBB = MainFunc->getEntryBlock();
    IRBuilder<> Builder(&EntryBB, EntryBB.getFirstInsertionPt());
    
    if (opts.UseThread) {
        // 使用后台线程检测
        Function *ThreadFunc = createThreadFunc(M, checkFunc, opts);
        Builder.CreateCall(ThreadFunc);
    } else {
        // 直接调用检测函数
        Builder.CreateCall(checkFunc);
    }
    
    return true;
}

Function* DetectUtils::createPtraceSelfAttachFunc(Module &M, Function *reportFunc) {
    LLVMContext &Ctx = M.getContext();

    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int8Ty = Type::getInt8Ty(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);

    // ===== 创建监控线程函数 =====
    // 线程参数：子进程PID
    FunctionType *MonitorThreadTy = FunctionType::get(VoidTy, {CharPtrTy}, false);
    Function *MonitorThread = Function::Create(
        MonitorThreadTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "ptrace_monitor_thread",
        &M
    );
    MonitorThread->addFnAttr(Attribute::NoInline);

    {
        BasicBlock *MonEntryBB = BasicBlock::Create(Ctx, "entry", MonitorThread);
        BasicBlock *MonLoopBB = BasicBlock::Create(Ctx, "loop", MonitorThread);
        BasicBlock *MonOpenOkBB = BasicBlock::Create(Ctx, "open_ok", MonitorThread);
        BasicBlock *MonOpenFailBB = BasicBlock::Create(Ctx, "open_fail", MonitorThread);
        BasicBlock *MonReadLoopBB = BasicBlock::Create(Ctx, "read_loop", MonitorThread);
        BasicBlock *MonCheckLineBB = BasicBlock::Create(Ctx, "check_line", MonitorThread);
        BasicBlock *MonParseBB = BasicBlock::Create(Ctx, "parse", MonitorThread);
        BasicBlock *MonSkipSpaceBB = BasicBlock::Create(Ctx, "skip_space", MonitorThread);
        BasicBlock *MonCheckPidBB = BasicBlock::Create(Ctx, "check_pid", MonitorThread);
        BasicBlock *MonTracerZeroBB = BasicBlock::Create(Ctx, "tracer_zero", MonitorThread);
        BasicBlock *MonTracerOkBB = BasicBlock::Create(Ctx, "tracer_ok", MonitorThread);
        BasicBlock *MonCloseBB = BasicBlock::Create(Ctx, "close", MonitorThread);

        IRBuilder<> Builder(MonEntryBB);

        // 获取子进程PID参数
        Argument *ChildPidArg = &*MonitorThread->arg_begin();
        Value *ChildPid = Builder.CreatePtrToInt(ChildPidArg, Int32Ty, "child_pid");

        // sleep 1秒后进入循环
        FunctionCallee UsleepFunc = M.getOrInsertFunction(
            "usleep", FunctionType::get(Int32Ty, {Int32Ty}, false));
        Builder.CreateCall(UsleepFunc, {ConstantInt::get(Int32Ty, 1000000)});
        Builder.CreateBr(MonLoopBB);

        // ===== 循环：持续检测 =====
        Builder.SetInsertPoint(MonLoopBB);

        // 构造 /proc/<child_pid>/status 路径
        FunctionCallee SprintfFunc = M.getOrInsertFunction(
            "sprintf", FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy}, true));

        Constant *StatusFmt = createGlobalString(M, "/proc/%d/status", ".ptrace.status_fmt");
        Type *PathBufTy = ArrayType::get(Int8Ty, 256);
        Value *PathBuf = Builder.CreateAlloca(PathBufTy, nullptr, "status_path");
        Value *PathBufPtr = Builder.CreateBitCast(PathBuf, CharPtrTy);
        Builder.CreateCall(SprintfFunc, {PathBufPtr, StatusFmt, ChildPid});

        // 打开文件
        FunctionCallee FopenFunc = M.getOrInsertFunction(
            "fopen", FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy}, false));
        Constant *ReadMode = createGlobalString(M, "r", ".ptrace.readmode");
        Value *Fp = Builder.CreateCall(FopenFunc, {PathBufPtr, ReadMode});
        Value *FpNotNull = Builder.CreateICmpNE(Fp, ConstantPointerNull::get(CharPtrTy));
        Builder.CreateCondBr(FpNotNull, MonOpenOkBB, MonOpenFailBB);

        Builder.SetInsertPoint(MonOpenFailBB);
        // 无法打开文件，sleep后重试
        Builder.CreateCall(UsleepFunc, {ConstantInt::get(Int32Ty, 2000000)});
        Builder.CreateBr(MonLoopBB);

        Builder.SetInsertPoint(MonOpenOkBB);
        Type *LineBufTy = ArrayType::get(Int8Ty, 512);
        Value *LineBuf = Builder.CreateAlloca(LineBufTy, nullptr, "linebuf");
        Value *LineBufPtr = Builder.CreateBitCast(LineBuf, CharPtrTy);

        Builder.CreateBr(MonReadLoopBB);

        // ===== 逐行读取 =====
        Builder.SetInsertPoint(MonReadLoopBB);
        FunctionCallee FgetsFunc = M.getOrInsertFunction(
            "fgets", FunctionType::get(CharPtrTy, {CharPtrTy, Int32Ty, CharPtrTy}, false));
        Value *Line = Builder.CreateCall(FgetsFunc, {LineBufPtr, ConstantInt::get(Int32Ty, 512), Fp});
        Value *LineNotNull = Builder.CreateICmpNE(Line, ConstantPointerNull::get(CharPtrTy));
        Builder.CreateCondBr(LineNotNull, MonCheckLineBB, MonCloseBB);

        Builder.SetInsertPoint(MonCheckLineBB);
        FunctionCallee StrstrFunc = M.getOrInsertFunction(
            "strstr", FunctionType::get(CharPtrTy, {CharPtrTy, CharPtrTy}, false));
        Constant *TracerPidNeedle = createGlobalString(M, "TracerPid:", ".ptrace.tracerpid");
        Value *Found = Builder.CreateCall(StrstrFunc, {LineBufPtr, TracerPidNeedle});
        Value *FoundNotNull = Builder.CreateICmpNE(Found, ConstantPointerNull::get(CharPtrTy));
        Builder.CreateCondBr(FoundNotNull, MonParseBB, MonReadLoopBB);

        Builder.SetInsertPoint(MonParseBB);
        Constant *ColonNeedle = createGlobalString(M, ":", ".ptrace.colon");
        Value *ColonPos = Builder.CreateCall(StrstrFunc, {LineBufPtr, ColonNeedle});
        Value *ColonNotNull = Builder.CreateICmpNE(ColonPos, ConstantPointerNull::get(CharPtrTy));

        BasicBlock *ParseOkBB = BasicBlock::Create(Ctx, "parse_ok", MonitorThread);
        Builder.CreateCondBr(ColonNotNull, ParseOkBB, MonReadLoopBB);

        Builder.SetInsertPoint(ParseOkBB);
        Value *ColonPlusOne = Builder.CreateGEP(Int8Ty, ColonPos, ConstantInt::get(Int64Ty, 1));
        Builder.CreateBr(MonSkipSpaceBB);

        // 跳过空白
        Builder.SetInsertPoint(MonSkipSpaceBB);
        PHINode *CurrentPtr = Builder.CreatePHI(CharPtrTy, 2, "ptr");
        CurrentPtr->addIncoming(ColonPlusOne, ParseOkBB);

        FunctionCallee IsspaceFunc = M.getOrInsertFunction(
            "isspace", FunctionType::get(Int32Ty, {Int32Ty}, false));
        Value *CharVal = Builder.CreateLoad(Int8Ty, CurrentPtr);
        Value *CharInt = Builder.CreateSExt(CharVal, Int32Ty);
        Value *IsSpace = Builder.CreateCall(IsspaceFunc, {CharInt});
        Value *IsSpaceBool = Builder.CreateICmpNE(IsSpace, ConstantInt::get(Int32Ty, 0));
        Value *NextPtr = Builder.CreateGEP(Int8Ty, CurrentPtr, ConstantInt::get(Int64Ty, 1));
        CurrentPtr->addIncoming(NextPtr, MonSkipSpaceBB);
        Builder.CreateCondBr(IsSpaceBool, MonSkipSpaceBB, MonCheckPidBB);

        // 检查PID值
        Builder.SetInsertPoint(MonCheckPidBB);
        FunctionCallee StrtoulFunc = M.getOrInsertFunction(
            "strtoul", FunctionType::get(Int64Ty, {CharPtrTy, CharPtrTy, Int32Ty}, false));
        Value *PidValue = Builder.CreateCall(StrtoulFunc,
            {CurrentPtr, ConstantPointerNull::get(CharPtrTy), ConstantInt::get(Int32Ty, 10)});

        // TracerPid == 0 → trace关系断开 → 被调试器剥离
        Value *IsZero = Builder.CreateICmpEQ(PidValue, ConstantInt::get(Int64Ty, 0));
        Builder.CreateCondBr(IsZero, MonTracerZeroBB, MonTracerOkBB);

        // TracerPid == 0：kill子进程 + 调用report
        Builder.SetInsertPoint(MonTracerZeroBB);
        FunctionCallee KillFunc = M.getOrInsertFunction(
            "kill", FunctionType::get(Int32Ty, {Int32Ty, Int32Ty}, false));
        Builder.CreateCall(KillFunc, {ChildPid, ConstantInt::get(Int32Ty, 9)});
        Builder.CreateCall(reportFunc);
        Builder.CreateUnreachable();

        // TracerPid != 0：正常，关闭文件，sleep后继续循环
        Builder.SetInsertPoint(MonTracerOkBB);
        FunctionCallee FcloseFunc = M.getOrInsertFunction(
            "fclose", FunctionType::get(Int32Ty, {CharPtrTy}, false));
        Builder.CreateCall(FcloseFunc, {Fp});
        Builder.CreateCall(UsleepFunc, {ConstantInt::get(Int32Ty, 2000000)});
        Builder.CreateBr(MonLoopBB);

        Builder.SetInsertPoint(MonCloseBB);
        Builder.CreateCall(FcloseFunc, {Fp});
        Builder.CreateCall(UsleepFunc, {ConstantInt::get(Int32Ty, 2000000)});
        Builder.CreateBr(MonLoopBB);
    }

    // ===== 创建主函数：fork + PTRACE_TRACEME + waitpid + PTRACE_CONT =====
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "ptrace_self_attach",
        &M
    );
    Func->addFnAttr(Attribute::NoInline);

    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *ForkDoneBB = BasicBlock::Create(Ctx, "fork_done", Func);
    BasicBlock *ChildBB = BasicBlock::Create(Ctx, "child", Func);
    BasicBlock *ParentBB = BasicBlock::Create(Ctx, "parent", Func);
    BasicBlock *WaitDoneBB = BasicBlock::Create(Ctx, "wait_done", Func);
    BasicBlock *ContDoneBB = BasicBlock::Create(Ctx, "cont_done", Func);
    BasicBlock *StartMonitorBB = BasicBlock::Create(Ctx, "start_monitor", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);

    IRBuilder<> Builder(EntryBB);

    // fork()
    FunctionCallee ForkFunc = M.getOrInsertFunction(
        "fork", FunctionType::get(Int32Ty, {}, false));
    Value *ForkRet = Builder.CreateCall(ForkFunc, {});
    Builder.CreateBr(ForkDoneBB);

    // 判断父子进程
    Builder.SetInsertPoint(ForkDoneBB);
    PHINode *PidPhi = Builder.CreatePHI(Int32Ty, 1, "pid");
    PidPhi->addIncoming(ForkRet, EntryBB);

    Value *IsChild = Builder.CreateICmpEQ(PidPhi, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(IsChild, ChildBB, ParentBB);

    // ===== 子进程：PTRACE_TRACEME =====
    Builder.SetInsertPoint(ChildBB);

    // ptrace(PTRACE_TRACEME, 0, NULL, NULL)
    // PTRACE_TRACEME = 0
    FunctionCallee PtraceFunc = M.getOrInsertFunction(
        "ptrace",
        FunctionType::get(Int64Ty, {Int64Ty, Int64Ty, Int64Ty, Int64Ty}, false));

    Builder.CreateCall(PtraceFunc, {
        ConstantInt::get(Int64Ty, 0),   // PTRACE_TRACEME
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0)
    });

    // 子进程直接返回，继续执行程序正常逻辑
    // raise(SIGSTOP) 让父进程有机会 waitpid
    FunctionCallee RaiseFunc = M.getOrInsertFunction(
        "raise", FunctionType::get(Int32Ty, {Int32Ty}, false));
    Builder.CreateCall(RaiseFunc, {ConstantInt::get(Int32Ty, 19)}); // SIGSTOP = 19

    Builder.CreateRetVoid();

    // ===== 父进程：waitpid + PTRACE_CONT =====
    Builder.SetInsertPoint(ParentBB);

    // waitpid(child_pid, &status, 0)
    FunctionCallee WaitpidFunc = M.getOrInsertFunction(
        "waitpid",
        FunctionType::get(Int32Ty, {Int32Ty, CharPtrTy, Int32Ty}, false));

    Value *StatusAlloca = Builder.CreateAlloca(Int32Ty, nullptr, "status");
    Value *StatusPtr = Builder.CreateBitCast(StatusAlloca, CharPtrTy);
    Builder.CreateCall(WaitpidFunc, {PidPhi, StatusPtr, ConstantInt::get(Int32Ty, 0)});
    Builder.CreateBr(WaitDoneBB);

    Builder.SetInsertPoint(WaitDoneBB);

    // ptrace(PTRACE_CONT, child_pid, NULL, NULL)
    // PTRACE_CONT = 7
    Value *ChildPidInt64 = Builder.CreateSExt(PidPhi, Int64Ty);
    Builder.CreateCall(PtraceFunc, {
        ConstantInt::get(Int64Ty, 7),   // PTRACE_CONT
        ChildPidInt64,
        ConstantInt::get(Int64Ty, 0),
        ConstantInt::get(Int64Ty, 0)
    });
    Builder.CreateBr(ContDoneBB);

    Builder.SetInsertPoint(ContDoneBB);

    // 启动监控线程
    Builder.CreateBr(StartMonitorBB);

    Builder.SetInsertPoint(StartMonitorBB);

    // 创建pthread线程运行监控函数
    Type *PthreadTy = StructType::create(Ctx, "pthread_t");
    Value *Thread = Builder.CreateAlloca(PthreadTy, nullptr, "monitor_thread");

    FunctionCallee PthreadCreateFunc = M.getOrInsertFunction(
        "pthread_create",
        FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy, CharPtrTy, CharPtrTy}, false));

    // 将子进程PID作为线程参数传递（转为指针）
    Value *ChildPidAsPtr = Builder.CreateIntToPtr(PidPhi, CharPtrTy, "pid_as_ptr");

    Value *ThreadPtr = Builder.CreateBitCast(Thread, CharPtrTy);
    Value *MonitorFuncPtr = Builder.CreateBitCast(MonitorThread, CharPtrTy);

    Builder.CreateCall(PthreadCreateFunc, {
        ThreadPtr,
        ConstantPointerNull::get(CharPtrTy),
        MonitorFuncPtr,
        ChildPidAsPtr
    });

    Builder.CreateBr(ExitBB);

    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();

    return Func;
}

Function* DetectUtils::createEnvVarCheckFunc(Module &M, Function *reportFunc, const std::string &envKey) {
    LLVMContext &Ctx = M.getContext();

    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int8Ty = Type::getInt8Ty(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);

    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "check_env_var",
        &M
    );

    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);

    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *GetEnvOkBB = BasicBlock::Create(Ctx, "getenv_ok", Func);
    BasicBlock *GetEnvFailBB = BasicBlock::Create(Ctx, "getenv_fail", Func);
    BasicBlock *CmpLoopBB = BasicBlock::Create(Ctx, "cmp_loop", Func);
    BasicBlock *CmpBodyBB = BasicBlock::Create(Ctx, "cmp_body", Func);
    BasicBlock *MismatchBB = BasicBlock::Create(Ctx, "mismatch", Func);
    BasicBlock *MatchBB = BasicBlock::Create(Ctx, "match", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);

    IRBuilder<> Builder(EntryBB);

    // 内嵌密钥：如果传入了密钥则直接使用，否则使用占位符
    std::string keyToUse = envKey.empty() ? std::string(ENV_KEY_PLACEHOLDER, 32) : envKey;
    Constant *KeyStr = ConstantDataArray::getString(Ctx, keyToUse, /*AddNull=*/false);
    GlobalVariable *KeyGV = new GlobalVariable(
        M, KeyStr->getType(), true,
        GlobalValue::PrivateLinkage, KeyStr,
        ".env.key"
    );
    KeyGV->setSection(".AProtect.rodata");

    // 环境变量名 "lc"
    Constant *EnvNameStr = ConstantDataArray::getString(Ctx, "lc");
    GlobalVariable *EnvNameGV = new GlobalVariable(
        M, EnvNameStr->getType(), true,
        GlobalValue::PrivateLinkage, EnvNameStr,
        ".env.name"
    );
    EnvNameGV->setSection(".AProtect.rodata");

    // getenv("lc")
    FunctionCallee GetenvFunc = M.getOrInsertFunction(
        "getenv",
        FunctionType::get(CharPtrTy, {CharPtrTy}, false)
    );

    Value *EnvNamePtr = ConstantExpr::getBitCast(EnvNameGV, CharPtrTy);
    Value *EnvVal = Builder.CreateCall(GetenvFunc, {EnvNamePtr});

    // 调试输出（仅在 -irobf-debug 开启时生成）
    if (isIRObfuscationDebugEnabled()) {
        FunctionCallee PrintfFunc = M.getOrInsertFunction(
            "printf",
            FunctionType::get(Int32Ty, {CharPtrTy}, true)
        );
        Constant *DbgStr = ConstantDataArray::getString(Ctx, "[EnvCheck] getenv(lc)=%p\n");
        GlobalVariable *DbgGV = new GlobalVariable(
            M, DbgStr->getType(), true, GlobalValue::PrivateLinkage,
            DbgStr, ".env.dbg");
        Constant *DbgPtr = ConstantExpr::getBitCast(DbgGV, CharPtrTy);
        Builder.CreateCall(PrintfFunc, {DbgPtr, EnvVal});
    }

    // 检查 getenv 返回值是否为 NULL
    Value *EnvNotNull = Builder.CreateICmpNE(EnvVal, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(EnvNotNull, GetEnvOkBB, GetEnvFailBB);

    // getenv 返回 NULL → 环境变量不存在 → kill
    Builder.SetInsertPoint(GetEnvFailBB);
    if (isIRObfuscationDebugEnabled()) {
        createStderrWrite(
            M,
            Builder,
            "[EnvCheck] lc is missing, expected linker wrapper.\n",
            ".env.missing"
        );
    }
    Builder.CreateCall(reportFunc);
    Builder.CreateUnreachable();

    // getenv 返回非空 → 逐字节比较
    Builder.SetInsertPoint(GetEnvOkBB);

    // 循环变量 i
    Value *Zero = ConstantInt::get(Int64Ty, 0);
    Value *ThirtyTwo = ConstantInt::get(Int64Ty, 32);

    Builder.CreateBr(CmpLoopBB);

    Builder.SetInsertPoint(CmpLoopBB);
    PHINode *IPhi = Builder.CreatePHI(Int64Ty, 2, "i");
    IPhi->addIncoming(Zero, GetEnvOkBB);

    // i < 32 ?
    Value *ILt32 = Builder.CreateICmpSLT(IPhi, ThirtyTwo);
    Builder.CreateCondBr(ILt32, CmpBodyBB, MatchBB);

    Builder.SetInsertPoint(CmpBodyBB);

    // 读取 env_val[i]
    Value *EnvCharPtr = Builder.CreateGEP(Int8Ty, EnvVal, IPhi);
    Value *EnvChar = Builder.CreateLoad(Int8Ty, EnvCharPtr);

    // 读取 key[i]
    Value *KeyPtr = Builder.CreateGEP(KeyGV->getValueType(), KeyGV,
        {ConstantInt::get(Int64Ty, 0), IPhi});
    Value *KeyChar = Builder.CreateLoad(Int8Ty, KeyPtr);

    // 比较
    Value *CharsEqual = Builder.CreateICmpEQ(EnvChar, KeyChar);
    Value *INext = Builder.CreateAdd(IPhi, ConstantInt::get(Int64Ty, 1));
    IPhi->addIncoming(INext, CmpBodyBB);
    Builder.CreateCondBr(CharsEqual, CmpLoopBB, MismatchBB);

    // 不匹配 → kill
    Builder.SetInsertPoint(MismatchBB);
    if (isIRObfuscationDebugEnabled()) {
        createStderrWrite(
            M,
            Builder,
            "[EnvCheck] lc mismatch, expected linker wrapper value.\n",
            ".env.mismatch"
        );
    }
    Builder.CreateCall(reportFunc);
    Builder.CreateUnreachable();

    // 全部匹配 → 清除环境变量 → 正常退出
    Builder.SetInsertPoint(MatchBB);

    // unsetenv("lc") - 清除环境变量
    FunctionCallee UnsetenvFunc = M.getOrInsertFunction(
        "unsetenv",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    Builder.CreateCall(UnsetenvFunc, {EnvNamePtr});

    Builder.CreateBr(ExitBB);

    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();

    return Func;
}

Function* DetectUtils::createGzEnvVarCheckFunc(Module &M, Function *reportFunc, const std::string &envKey) {
    LLVMContext &Ctx = M.getContext();

    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int8Ty = Type::getInt8Ty(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *Int64Ty = Type::getInt64Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);

    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "check_gz_env_var",
        &M
    );

    Func->addFnAttr(Attribute::NoInline);
    Func->addFnAttr(Attribute::OptimizeNone);

    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *GetEnvOkBB = BasicBlock::Create(Ctx, "getenv_ok", Func);
    BasicBlock *GetEnvFailBB = BasicBlock::Create(Ctx, "getenv_fail", Func);
    BasicBlock *CmpLoopBB = BasicBlock::Create(Ctx, "cmp_loop", Func);
    BasicBlock *CmpBodyBB = BasicBlock::Create(Ctx, "cmp_body", Func);
    BasicBlock *MismatchBB = BasicBlock::Create(Ctx, "mismatch", Func);
    BasicBlock *MatchBB = BasicBlock::Create(Ctx, "match", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);

    IRBuilder<> Builder(EntryBB);

    // 内嵌密钥：如果传入了密钥则直接使用，否则使用占位符
    std::string keyToUse = envKey.empty() ? std::string(GZ_ENV_KEY_PLACEHOLDER, 32) : envKey;
    Constant *KeyStr = ConstantDataArray::getString(Ctx, keyToUse, /*AddNull=*/false);
    GlobalVariable *KeyGV = new GlobalVariable(
        M, KeyStr->getType(), true,
        GlobalValue::PrivateLinkage, KeyStr,
        ".gz.env.key"
    );
    KeyGV->setSection(".AProtect.rodata");

    // 环境变量名 "lc_gz"
    Constant *EnvNameStr = ConstantDataArray::getString(Ctx, "lc_gz");
    GlobalVariable *EnvNameGV = new GlobalVariable(
        M, EnvNameStr->getType(), true,
        GlobalValue::PrivateLinkage, EnvNameStr,
        ".gz.env.name"
    );
    EnvNameGV->setSection(".AProtect.rodata");

    // getenv("lc_gz")
    FunctionCallee GetenvFunc = M.getOrInsertFunction(
        "getenv",
        FunctionType::get(CharPtrTy, {CharPtrTy}, false)
    );

    Value *EnvNamePtr = ConstantExpr::getBitCast(EnvNameGV, CharPtrTy);
    Value *EnvVal = Builder.CreateCall(GetenvFunc, {EnvNamePtr});

    // 调试输出（仅在 -irobf-debug 开启时生成）
    if (isIRObfuscationDebugEnabled()) {
        FunctionCallee PrintfFunc = M.getOrInsertFunction(
            "printf",
            FunctionType::get(Int32Ty, {CharPtrTy}, true)
        );
        Constant *DbgStr = ConstantDataArray::getString(Ctx, "[GzEnvCheck] getenv(lc_gz)=%p\n");
        GlobalVariable *DbgGV = new GlobalVariable(
            M, DbgStr->getType(), true, GlobalValue::PrivateLinkage,
            DbgStr, ".gz.env.dbg");
        Constant *DbgPtr = ConstantExpr::getBitCast(DbgGV, CharPtrTy);
        Builder.CreateCall(PrintfFunc, {DbgPtr, EnvVal});
    }

    // 检查 getenv 返回值是否为 NULL
    Value *EnvNotNull = Builder.CreateICmpNE(EnvVal, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(EnvNotNull, GetEnvOkBB, GetEnvFailBB);

    // getenv 返回 NULL → 环境变量不存在 → kill
    Builder.SetInsertPoint(GetEnvFailBB);
    if (isIRObfuscationDebugEnabled()) {
        createStderrWrite(
            M,
            Builder,
            "[GzEnvCheck] lc_gz is missing, expected gz wrapper.\n",
            ".gz.env.missing"
        );
    }
    Builder.CreateCall(reportFunc);
    Builder.CreateUnreachable();

    // getenv 返回非空 → 逐字节比较
    Builder.SetInsertPoint(GetEnvOkBB);

    Value *Zero = ConstantInt::get(Int64Ty, 0);
    Value *ThirtyTwo = ConstantInt::get(Int64Ty, 32);

    Builder.CreateBr(CmpLoopBB);

    Builder.SetInsertPoint(CmpLoopBB);
    PHINode *IPhi = Builder.CreatePHI(Int64Ty, 2, "i");
    IPhi->addIncoming(Zero, GetEnvOkBB);

    Value *ILt32 = Builder.CreateICmpSLT(IPhi, ThirtyTwo);
    Builder.CreateCondBr(ILt32, CmpBodyBB, MatchBB);

    Builder.SetInsertPoint(CmpBodyBB);

    Value *EnvCharPtr = Builder.CreateGEP(Int8Ty, EnvVal, IPhi);
    Value *EnvChar = Builder.CreateLoad(Int8Ty, EnvCharPtr);

    Value *KeyPtr = Builder.CreateGEP(KeyGV->getValueType(), KeyGV,
        {ConstantInt::get(Int64Ty, 0), IPhi});
    Value *KeyChar = Builder.CreateLoad(Int8Ty, KeyPtr);

    Value *CharsEqual = Builder.CreateICmpEQ(EnvChar, KeyChar);
    Value *INext = Builder.CreateAdd(IPhi, ConstantInt::get(Int64Ty, 1));
    IPhi->addIncoming(INext, CmpBodyBB);
    Builder.CreateCondBr(CharsEqual, CmpLoopBB, MismatchBB);

    // 不匹配 → kill
    Builder.SetInsertPoint(MismatchBB);
    if (isIRObfuscationDebugEnabled()) {
        createStderrWrite(
            M,
            Builder,
            "[GzEnvCheck] lc_gz mismatch, expected gz wrapper value.\n",
            ".gz.env.mismatch"
        );
    }
    Builder.CreateCall(reportFunc);
    Builder.CreateUnreachable();

    // 全部匹配 → 清除环境变量 → 正常退出
    Builder.SetInsertPoint(MatchBB);

    // unsetenv("lc_gz") - 清除环境变量
    FunctionCallee UnsetenvFunc = M.getOrInsertFunction(
        "unsetenv",
        FunctionType::get(Int32Ty, {CharPtrTy}, false)
    );
    Builder.CreateCall(UnsetenvFunc, {EnvNamePtr});

    Builder.CreateBr(ExitBB);

    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();

    return Func;
}

Function* DetectUtils::createRandomThreadAttrFunc(Module &M) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "random_thread_attr",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", Func);
    IRBuilder<> Builder(BB);
    
    // 初始化随机数种子
    FunctionCallee SrandFunc = M.getOrInsertFunction(
        "srand",
        FunctionType::get(VoidTy, {Int32Ty}, false)
    );
    
    FunctionCallee TimeFunc = M.getOrInsertFunction(
        "time",
        FunctionType::get(Type::getInt64Ty(Ctx), {CharPtrTy}, false)
    );
    
    Value *TimeVal = Builder.CreateCall(TimeFunc, {ConstantPointerNull::get(CharPtrTy)});
    Value *TimeInt = Builder.CreateTrunc(TimeVal, Int32Ty);
    Builder.CreateCall(SrandFunc, {TimeInt});
    
    // 生成随机线程名
    FunctionCallee RandFunc = M.getOrInsertFunction(
        "rand",
        FunctionType::get(Int32Ty, {}, false)
    );
    
    // 使用prctl设置线程名
    FunctionCallee PrctlFunc = M.getOrInsertFunction(
        "prctl",
        FunctionType::get(Int32Ty, {Int32Ty, CharPtrTy, Int32Ty, Int32Ty, Int32Ty}, false)
    );
    
    // PR_SET_NAME = 15
    Value *PrSetName = ConstantInt::get(Int32Ty, 15);
    
    // 创建随机名称缓冲区
    Type *NameBufTy = ArrayType::get(Type::getInt8Ty(Ctx), 16);
    Value *NameBuf = Builder.CreateAlloca(NameBufTy, nullptr, "thread_name");
    Value *NameBufPtr = Builder.CreateBitCast(NameBuf, CharPtrTy);
    
    // 格式化随机名称
    Constant *FmtStr = ConstantDataArray::getString(Ctx, "thr_%d");
    GlobalVariable *FmtGV = new GlobalVariable(
        M, FmtStr->getType(), true,
        GlobalValue::PrivateLinkage, FmtStr,
        ".thread.fmt"
    );
    FmtGV->setSection(".AProtect.rodata");
    
    FunctionCallee SprintfFunc = M.getOrInsertFunction(
        "sprintf",
        FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy}, true)
    );
    
    Value *RandName = Builder.CreateCall(RandFunc);
    Builder.CreateCall(SprintfFunc, {
        NameBufPtr,
        ConstantExpr::getBitCast(FmtGV, CharPtrTy),
        RandName
    });
    
    // 设置线程名
    Builder.CreateCall(PrctlFunc, {PrSetName, NameBufPtr, ConstantInt::get(Int32Ty, 0), ConstantInt::get(Int32Ty, 0), ConstantInt::get(Int32Ty, 0)});
    
    Builder.CreateRetVoid();
    
    return Func;
}
