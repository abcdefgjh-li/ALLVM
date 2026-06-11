//===- VmProtectDetect.cpp - 虚拟机文件检测注入Pass --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// 本文件实现虚拟机文件检测注入Pass，在程序入口点注入检测代码
// 通过 stat() 扫描已知虚拟机/模拟器特征文件路径
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/VmProtectDetect.h"
#include "llvm/Transforms/Obfuscation/DetectUtils.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "vmprotectdetect"

using namespace llvm;

static const char *VM_FILE_PATHS[] = {
    "/system/bin/androVM-prop",
    "/system/bin/microvirt-prop",
    "/system/lib/libdroid4x.so",
    "/system/bin/windroyed",
    "/system/bin/nox-prop",
    "/system/lib/libnoxspeedup.so",
    "/system/bin/ttVM-prop",
    "/data/.bluestacks.prop",
    "/system/bin/duosconfig",
    "/system/etc/xxzs_prop.sh",
    "/system/etc/mumu-configs/device-prop-configs/mumu.config",
    "/system/priv-app/ldAppStore",
    "/system/bin/ldinit",
    "/system/bin/ldmountsf",
    "/system/app/AntStore",
    "/system/app/AntLauncher",
    "/vmos.prop",
    "/fstab.titan",
    "/init.titan.rc",
    "/x8.prop",
    "/system/lib/libc_malloc_debug_qemu.so",
    "/system/bin/microvirtd",
    "/dev/socket/qemud",
    "/dev/qemu_pipe",
    nullptr
};

namespace {

struct VmProtectDetect : public ModulePass {
    static char ID;

    VmProtectDetect() : ModulePass(ID) {
        initializeVmProtectDetectPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"VmProtectDetect"};
    }

    bool runOnModule(Module &M) override;

    Function* createVMCheckFunc(Module &M, Function *ReportAndKillFunc);
    Function* createCpuinfoCheckFunc(Module &M, Function *ReportAndKillFunc);
};

}

char VmProtectDetect::ID = 0;

Function* VmProtectDetect::createVMCheckFunc(Module &M, Function *ReportAndKillFunc) {
    LLVMContext &Ctx = M.getContext();

    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);

    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "vmprotect_check",
        &M
    );

    Func->addFnAttr(Attribute::NoInline);

    int NumPaths = 0;
    while (VM_FILE_PATHS[NumPaths] != nullptr) {
        NumPaths++;
    }
    int ArraySize = NumPaths + 1;

    std::vector<Constant *> PathConstants;
    for (int i = 0; i < NumPaths; ++i) {
        Constant *PathStr = ConstantDataArray::getString(Ctx, VM_FILE_PATHS[i]);
        GlobalVariable *PathGV = new GlobalVariable(
            M,
            PathStr->getType(),
            true,
            GlobalValue::PrivateLinkage,
            PathStr,
            ".vm.path." + Twine(i)
        );
        Constant *PathPtr = ConstantExpr::getBitCast(PathGV, CharPtrTy);
        PathConstants.push_back(PathPtr);
    }
    PathConstants.push_back(ConstantPointerNull::get(CharPtrTy));

    ArrayType *PathsArrayTy = ArrayType::get(CharPtrTy, ArraySize);
    Constant *PathsArrayInit = ConstantArray::get(PathsArrayTy, PathConstants);
    GlobalVariable *PathsArray = new GlobalVariable(
        M,
        PathsArrayTy,
        true,
        GlobalValue::PrivateLinkage,
        PathsArrayInit,
        ".vm.paths"
    );

    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *LoopCondBB = BasicBlock::Create(Ctx, "loop.cond", Func);
    BasicBlock *LoopBodyBB = BasicBlock::Create(Ctx, "loop.body", Func);
    BasicBlock *FoundBB = BasicBlock::Create(Ctx, "found", Func);
    BasicBlock *ContinueBB = BasicBlock::Create(Ctx, "continue", Func);
    BasicBlock *ExitBB = BasicBlock::Create(Ctx, "exit", Func);

    IRBuilder<> Builder(EntryBB);
    Builder.CreateBr(LoopCondBB);

    Builder.SetInsertPoint(LoopCondBB);
    PHINode *LoopCounter = Builder.CreatePHI(Int32Ty, 2, "i");
    LoopCounter->addIncoming(ConstantInt::get(Int32Ty, 0), EntryBB);

    Value *PathPtr = Builder.CreateInBoundsGEP(
        PathsArrayTy,
        PathsArray,
        {ConstantInt::get(Type::getInt64Ty(Ctx), 0), LoopCounter}
    );
    Value *CurrentPath = Builder.CreateLoad(CharPtrTy, PathPtr);

    Value *IsNull = Builder.CreateIsNull(CurrentPath);
    Builder.CreateCondBr(IsNull, ExitBB, LoopBodyBB);

    Builder.SetInsertPoint(LoopBodyBB);

    FunctionCallee StatFunc = M.getOrInsertFunction(
        "stat",
        FunctionType::get(Int32Ty, {CharPtrTy, CharPtrTy}, false)
    );

    Type *StatBufTy = ArrayType::get(Type::getInt8Ty(Ctx), 256);
    Value *StatBuf = Builder.CreateAlloca(StatBufTy, nullptr, "st");
    Value *StatBufPtr = Builder.CreateBitCast(StatBuf, CharPtrTy);

    Value *StatRet = Builder.CreateCall(StatFunc, {CurrentPath, StatBufPtr});

    Value *Exists = Builder.CreateICmpEQ(StatRet, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(Exists, FoundBB, ContinueBB);

    Builder.SetInsertPoint(FoundBB);
    Builder.CreateCall(ReportAndKillFunc);
    Builder.CreateBr(ContinueBB);

    Builder.SetInsertPoint(ContinueBB);
    Value *NextCounter = Builder.CreateAdd(LoopCounter, ConstantInt::get(Int32Ty, 1), "", true, true);
    LoopCounter->addIncoming(NextCounter, ContinueBB);
    Builder.CreateBr(LoopCondBB);

    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

Function* VmProtectDetect::createCpuinfoCheckFunc(Module &M, Function *ReportAndKillFunc) {
    LLVMContext &Ctx = M.getContext();
    
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    PointerType *CharPtrTy = PointerType::get(Ctx, 0);
    
    FunctionType *FuncTy = FunctionType::get(VoidTy, {}, false);
    Function *Func = Function::Create(
        FuncTy,
        GlobalValue::InternalLinkage,
        M.getDataLayout().getProgramAddressSpace(),
        "vmprotect_cpuinfo_check",
        &M
    );
    
    Func->addFnAttr(Attribute::NoInline);
    
    BasicBlock *EntryBB = BasicBlock::Create(Ctx, "entry", Func);
    BasicBlock *OpenOkBB = BasicBlock::Create(Ctx, "open_ok", Func);
    BasicBlock *OpenFailBB = BasicBlock::Create(Ctx, "open_fail", Func);
    BasicBlock *LoopBB = BasicBlock::Create(Ctx, "loop", Func);
    BasicBlock *CheckLineBB = BasicBlock::Create(Ctx, "check_line", Func);
    BasicBlock *CheckHardwareBB = BasicBlock::Create(Ctx, "check_hardware", Func);
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
        return DetectUtils::createGlobalString(M, str, ".cpuinfo.str");
    };
    
    Constant *CpuinfoPath = makeString("/proc/cpuinfo");
    Constant *ReadMode = makeString("r");
    
    Value *Fp = Builder.CreateCall(FopenFunc, {CpuinfoPath, ReadMode});
    Value *FpNotNull = Builder.CreateICmpNE(Fp, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(FpNotNull, OpenOkBB, OpenFailBB);
    
    Builder.SetInsertPoint(OpenFailBB);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(OpenOkBB);
    
    Type *LineBufTy = ArrayType::get(Type::getInt8Ty(Ctx), 256);
    Value *LineBuf = Builder.CreateAlloca(LineBufTy, nullptr, "linebuf");
    Value *LineBufPtr = Builder.CreateBitCast(LineBuf, CharPtrTy);
    
    Builder.CreateBr(LoopBB);
    
    Builder.SetInsertPoint(LoopBB);
    
    Value *Line = Builder.CreateCall(FgetsFunc, {LineBufPtr, ConstantInt::get(Int32Ty, 256), Fp});
    Value *LineNotNull = Builder.CreateICmpNE(Line, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(LineNotNull, CheckLineBB, ExitBB);
    
    Builder.SetInsertPoint(CheckLineBB);
    
    Constant *HardwareNeedle = makeString("Hardware");
    Value *FoundHardware = Builder.CreateCall(StrstrFunc, {LineBufPtr, HardwareNeedle});
    Value *HasHardware = Builder.CreateICmpNE(FoundHardware, ConstantPointerNull::get(CharPtrTy));
    Builder.CreateCondBr(HasHardware, CheckHardwareBB, LoopBB);
    
    Builder.SetInsertPoint(CheckHardwareBB);
    
    Constant *QemuNeedle = makeString("QEMU");
    Constant *KvmNeedle = makeString("KVM");
    Constant *VboxNeedle = makeString("VirtualBox");
    
    Value *FoundQemu = Builder.CreateCall(StrstrFunc, {LineBufPtr, QemuNeedle});
    Value *FoundQemuNotNull = Builder.CreateICmpNE(FoundQemu, ConstantPointerNull::get(CharPtrTy));
    
    Value *FoundKvm = Builder.CreateCall(StrstrFunc, {LineBufPtr, KvmNeedle});
    Value *FoundKvmNotNull = Builder.CreateICmpNE(FoundKvm, ConstantPointerNull::get(CharPtrTy));
    
    Value *FoundVbox = Builder.CreateCall(StrstrFunc, {LineBufPtr, VboxNeedle});
    Value *FoundVboxNotNull = Builder.CreateICmpNE(FoundVbox, ConstantPointerNull::get(CharPtrTy));
    
    Value *AnyFound = Builder.CreateOr(FoundQemuNotNull, FoundKvmNotNull);
    AnyFound = Builder.CreateOr(AnyFound, FoundVboxNotNull);
    
    BasicBlock *FoundBB = BasicBlock::Create(Ctx, "found", Func);
    Builder.CreateCondBr(AnyFound, FoundBB, LoopBB);
    
    Builder.SetInsertPoint(FoundBB);
    Builder.CreateCall(FcloseFunc, {Fp});
    Builder.CreateCall(ReportAndKillFunc);
    Builder.CreateBr(ExitBB);
    
    Builder.SetInsertPoint(ExitBB);
    Builder.CreateRetVoid();
    
    return Func;
}

bool VmProtectDetect::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] VmProtectDetect: Injecting VM detection\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 使用公共模块创建报告函数
    Function *ReportAndKillFunc = DetectUtils::createReportAndKillFunc(M, "VM/Emulator");
    
    // 创建检测函数
    Function *CheckFunc = createVMCheckFunc(M, ReportAndKillFunc);
    Function *CpuinfoCheckFunc = createCpuinfoCheckFunc(M, ReportAndKillFunc);

    BasicBlock &EntryBB = MainFunc->getEntryBlock();
    IRBuilder<> Builder(&EntryBB, EntryBB.getFirstInsertionPt());

    // 注入检测调用
    Builder.CreateCall(CheckFunc);
    Builder.CreateCall(CpuinfoCheckFunc);

    return true;
}

ModulePass *llvm::createVmProtectDetectPass() {
    return new VmProtectDetect();
}

INITIALIZE_PASS_BEGIN(VmProtectDetect, "vmprotectdetect", "Inject VM file detection at program start", false, false)
INITIALIZE_PASS_END(VmProtectDetect, "vmprotectdetect", "Inject VM file detection at program start", false, false)
