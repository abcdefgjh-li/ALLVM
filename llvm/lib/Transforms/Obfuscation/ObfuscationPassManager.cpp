//===- ObfuscationPassManager.cpp - 混淆Pass管理器------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现混淆Pass管理器，负责协调和执行所有混淆Pass
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/Transforms/Obfuscation/LicenseManager.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Transforms/Obfuscation/ObfuscationOptions.h"
#include "llvm/Transforms/Obfuscation/aVMP.h"
#include "llvm/Transforms/Obfuscation/AProtect.h"
#include "llvm/Transforms/Obfuscation/LdPreloadProtect.h"
#include "llvm/Transforms/Obfuscation/VmProtectDetect.h"
#include "llvm/Transforms/Obfuscation/UsbProtect.h"
#include "llvm/Transforms/Obfuscation/IdaDetect.h"
#include "llvm/Transforms/Obfuscation/VpnDetect.h"
#include "llvm/Transforms/Obfuscation/ProxyDetect.h"
#include "llvm/Transforms/Obfuscation/TimeDetect.h"
#include "llvm/Transforms/Obfuscation/HostsDetect.h"
#include "llvm/Transforms/Obfuscation/MemDetect.h"
#include "llvm/Transforms/Obfuscation/PtraceDetect.h"
#include "llvm/Transforms/Obfuscation/InlineHookDetect.h"
#include "llvm/Transforms/Obfuscation/PltHookDetect.h"
#include "llvm/Transforms/Obfuscation/HideMaps.h"
#include "llvm/Transforms/Obfuscation/FakeMaps.h"
#include "llvm/Transforms/Obfuscation/MemProtect.h"
#include "llvm/Transforms/Obfuscation/RootDetect.h"
#include "llvm/Transforms/Obfuscation/NoRootDetect.h"
#include "llvm/Transforms/Obfuscation/SyscallProtect.h"
#include "llvm/IR/Module.h"


#define DEBUG_TYPE "ir-obfuscation"

using namespace llvm;

static cl::opt<bool>
EnableIRObfuscation("irobf", cl::init(false), cl::NotHidden,
                    cl::desc("Enable IR Code Obfuscation."),
                    cl::ZeroOrMore);

static cl::opt<bool>
EnableIRObfuscationDebug("irobf-debug", cl::init(false), cl::NotHidden,
                         cl::desc("Enable debug output for obfuscation."),
                         cl::ZeroOrMore);

static cl::opt<std::string>
LicenseKey("irobf-key", cl::init(""), cl::NotHidden,
           cl::desc("License key for OLLVM obfuscation (time-based)."),
           cl::value_desc("key"));

bool llvm::isIRObfuscationDebugEnabled() {
    return EnableIRObfuscationDebug;
}

static cl::opt<bool>
EnableIndirectBr("irobf-indbr", cl::init(false), cl::NotHidden,
                 cl::desc("Enable IR Indirect Branch Obfuscation."),
                 cl::ZeroOrMore);
static cl::opt<uint32_t>
LevelIndirectBr("level-indbr", cl::init(0), cl::NotHidden,
                cl::desc("Set IR Indirect Branch Obfuscation Level."),
                cl::ZeroOrMore);


static cl::opt<bool>
EnableIndirectCall("irobf-icall", cl::init(false), cl::NotHidden,
                   cl::desc("Enable IR Indirect Call Obfuscation."),
                   cl::ZeroOrMore);
static cl::opt<uint32_t>
LevelIndirectCall("level-icall", cl::init(0), cl::NotHidden,
                  cl::desc("Set IR Indirect Call Obfuscation Level."),
                  cl::ZeroOrMore);


static cl::opt<bool> EnableIndirectGV(
    "irobf-indgv", cl::init(false), cl::NotHidden,
    cl::desc("Enable IR Indirect Global Variable Obfuscation."),
    cl::ZeroOrMore);
static cl::opt<uint32_t> LevelIndirectGV(
    "level-indgv", cl::init(0), cl::NotHidden,
    cl::desc("Set IR Indirect Global Variable Obfuscation Level."),
    cl::ZeroOrMore);


static cl::opt<bool> EnableIRFlattening(
    "irobf-fla", cl::init(false), cl::NotHidden,
    cl::desc("Enable IR Control Flow Flattening Obfuscation."), cl::ZeroOrMore);


static cl::opt<bool>
EnableIRStringEncryption("irobf-cse", cl::init(false), cl::NotHidden,
                         cl::desc("Enable IR Constant String Encryption."),
                         cl::ZeroOrMore);


static cl::opt<bool>
EnableIRConstantIntEncryption("irobf-cie", cl::init(false), cl::NotHidden,
                              cl::desc(
                                  "Enable IR Constant Integer Encryption."),
                              cl::ZeroOrMore);
static cl::opt<uint32_t> LevelIRConstantIntEncryption(
    "level-cie", cl::init(0), cl::NotHidden,
    cl::desc("Set IR Constant Integer Encryption Level."),
    cl::ZeroOrMore);


static cl::opt<bool>
EnableIRConstantFPEncryption("irobf-cfe", cl::init(false), cl::NotHidden,
                             cl::desc("Enable IR Constant FP Encryption."),
                             cl::ZeroOrMore);

static cl::opt<uint32_t> LevelIRConstantFPEncryption(
    "level-cfe", cl::init(0), cl::NotHidden,
    cl::desc("Set IR Constant FP Encryption Level."),
    cl::ZeroOrMore);


static cl::opt<bool>
EnableRttiEraser("irobf-rtti", cl::init(false), cl::NotHidden,
                 cl::desc("Enable RTTI Eraser."),
                 cl::ZeroOrMore);

static cl::opt<bool>
EnableVMProtect("irobf-vmp", cl::init(false), cl::NotHidden,
                cl::desc("Enable VMProtect Obfuscation."),
                cl::ZeroOrMore);

static cl::opt<bool>
EnableLdPreloadProtect("irobf-ldpreload", cl::init(false), cl::NotHidden,
                       cl::desc("Enable LD_PRELOAD injection detection."),
                       cl::ZeroOrMore);

static cl::opt<bool>
EnableVmProtectDetect("irobf-vmdetect", cl::init(false), cl::NotHidden,
                      cl::desc("Enable VM file detection injection."),
                      cl::ZeroOrMore);

static cl::opt<bool>
EnableUsbProtect("irobf-usb", cl::init(false), cl::NotHidden,
                 cl::desc("Enable USB debug disable injection."),
                 cl::ZeroOrMore);

static cl::opt<bool>
EnableIdaDetect("irobf-ida", cl::init(false), cl::NotHidden,
                cl::desc("Enable IDA debugger detection."),
                cl::ZeroOrMore);

static cl::opt<bool>
EnableVpnDetect("irobf-vpn", cl::init(false), cl::NotHidden,
                cl::desc("Enable VPN connection detection."),
                cl::ZeroOrMore);

static cl::opt<bool>
EnableProxyDetect("irobf-proxy", cl::init(false), cl::NotHidden,
                  cl::desc("Enable proxy/iptables detection."),
                  cl::ZeroOrMore);

static cl::opt<bool>
EnableTimeDetect("irobf-time", cl::init(false), cl::NotHidden,
                 cl::desc("Enable time-based debugger detection."),
                 cl::ZeroOrMore);

static cl::opt<bool>
EnableHostsDetect("irobf-hosts", cl::init(false), cl::NotHidden,
                  cl::desc("Enable hosts file detection."),
                  cl::ZeroOrMore);

static cl::opt<bool>
EnableMemDetect("irobf-mem", cl::init(false), cl::NotHidden,
                cl::desc("Enable memory detection."),
                cl::ZeroOrMore);

static cl::opt<bool>
EnablePtraceDetect("irobf-ptrace", cl::init(false), cl::NotHidden,
                   cl::desc("Enable ptrace debugger detection."),
                   cl::ZeroOrMore);

static cl::opt<bool>
EnableInlineHookDetect("irobf-inlinehook", cl::init(false), cl::NotHidden,
                       cl::desc("Enable inline hook detection."),
                       cl::ZeroOrMore);

static cl::opt<bool>
EnablePltHookDetect("irobf-plthook", cl::init(false), cl::NotHidden,
                    cl::desc("Enable PLT hook detection."),
                    cl::ZeroOrMore);

static cl::opt<bool>
EnableHideMaps("irobf-hidemaps", cl::init(false), cl::NotHidden,
               cl::desc("Enable hide /proc/self/maps (requires root)."),
               cl::ZeroOrMore);

static cl::opt<bool>
EnableFakeMaps("irobf-fakemaps", cl::init(false), cl::NotHidden,
               cl::desc("Enable generate fake /proc/self/maps content."),
               cl::ZeroOrMore);

static cl::opt<bool>
EnableMemProtect("irobf-memprotect", cl::init(false), cl::NotHidden,
                 cl::desc("Enable memory dump protection."),
                 cl::ZeroOrMore);

static cl::opt<bool>
EnableRootDetect("irobf-root", cl::init(false), cl::NotHidden,
                 cl::desc("Enable root detection (exit if root)."),
                 cl::ZeroOrMore);

static cl::opt<bool>
EnableNoRootDetect("irobf-noroot", cl::init(false), cl::NotHidden,
                   cl::desc("Enable no-root detection (exit if not root)."),
                   cl::ZeroOrMore);

static cl::opt<bool>
EnableSyscallProtect("irobf-syscall", cl::init(false), cl::NotHidden,
                     cl::desc("Protect syscall functions (recv/send/read/write/clock_gettime) with direct syscalls."),
                     cl::ZeroOrMore);

static cl::opt<std::string>
SamsaraConfigPath("samsara-cfg", cl::init(std::string{}), cl::NotHidden,
                  cl::desc("Samsara config path."),
                  cl::ZeroOrMore);

namespace llvm {

	struct ObfuscationPassManager : public ModulePass {
		static char            ID; // Pass identification
		SmallVector<Pass *, 8> Passes;

		/**
		 * @brief 构造函数，初始化混淆Pass管理器
		 */
		ObfuscationPassManager() : ModulePass(ID) {
			initializeObfuscationPassManagerPass(*PassRegistry::getPassRegistry());
		};

		/**
		 * @brief 获取Pass名称
		 * @return 返回Pass的名称字符串
		 */
		StringRef getPassName() const override {
			return "Obfuscation Pass Manager";
		}

		/**
		 * @brief 结束阶段，清理所有Pass
		 * @param M 要处理的LLVM模块
		 * @return 如果模块被修改返回true，否则返回false
		 */
		bool doFinalization(Module &M) override {
			bool Change = false;
			for (Pass *P : Passes) {
				Change |= P->doFinalization(M);
				delete (P);
			}
			Passes.clear();
			return Change;
		}

		/**
		 * @brief 添加Pass到管理器
		 * @param P 要添加的Pass指针
		 */
		void add(Pass *P) {
			Passes.push_back(P);
		}

		/**
		 * @brief 运行所有Pass
		 * @param M 要处理的LLVM模块
		 * @return 如果模块被修改返回true，否则返回false
		 */
		bool run(Module &M) {
			bool Change = false;
			for (Pass *P : Passes) {
				switch (P->getPassKind()) {
					case PassKind::PT_Function:
						Change |= runFunctionPass(M, (FunctionPass *)P);
						break;
					case PassKind::PT_Module:
						Change |= runModulePass(M, (ModulePass *)P);
						break;
					default:
						continue;
				}
			}
			return Change;
		}

		/**
		 * @brief 运行函数Pass
		 * @param M 要处理的LLVM模块
		 * @param P 要运行的函数Pass指针
		 * @return 如果模块被修改返回true，否则返回false
		 */
		bool runFunctionPass(Module &M, FunctionPass *P) {
			bool Changed = false;
			Changed |= P->doInitialization(M);
			for (Function &F : M) {
				// 经测试 VMP 解释器函数可与所有 FunctionPass 安全共存，保留注释以备查阅
				// if (F.getName().contains("vm_interpreter") || F.getName().ends_with("_original")) {
				// 	continue;
				// }
				Changed |= P->runOnFunction(F);
			}
			return Changed;
		}

		/**
		 * @brief 运行模块Pass
		 * @param M 要处理的LLVM模块
		 * @param P 要运行的模块Pass指针
		 * @return 如果模块被修改返回true，否则返回false
		 */
		bool runModulePass(Module &M, ModulePass *P) {
			return P->doInitialization(M) || P->runOnModule(M);
		}

		/**
		 * @brief 获取混淆选项
		 * @return 返回混淆选项对象的智能指针
		 */
		static std::shared_ptr<ObfuscationOptions> getOptions() {
			auto Opt = ObfuscationOptions::readConfigFile(SamsaraConfigPath);

			Opt->indBrOpt()->readOpt(EnableIndirectBr, LevelIndirectBr);
			Opt->iCallOpt()->readOpt(EnableIndirectCall, LevelIndirectCall);
			Opt->indGvOpt()->readOpt(EnableIndirectGV, LevelIndirectGV);
			Opt->flaOpt()->readOpt(EnableIRFlattening);
			Opt->cseOpt()->readOpt(EnableIRStringEncryption);
			Opt->cieOpt()->readOpt(EnableIRConstantIntEncryption,
			                       LevelIRConstantIntEncryption);
			Opt->cfeOpt()->readOpt(EnableIRConstantFPEncryption,
			                       LevelIRConstantFPEncryption);
			Opt->rttiOpt()->readOpt(EnableRttiEraser);
            return Opt;
		}

		/**
		 * @brief 对模块执行混淆
		 * @param M 要处理的LLVM模块
		 * @return 如果模块被修改返回true，否则返回false
		 */
		bool runOnModule(Module &M) override {

			bool hasObfuscation = EnableIndirectBr || EnableIndirectCall || EnableIndirectGV ||
			    EnableIRFlattening || EnableIRStringEncryption ||
			    EnableIRConstantIntEncryption || EnableIRConstantFPEncryption ||
			    EnableRttiEraser || EnableVMProtect || EnableLdPreloadProtect ||
			    EnableVmProtectDetect || EnableUsbProtect || EnableIdaDetect || EnableVpnDetect ||
			    EnableProxyDetect || EnableTimeDetect || EnableHostsDetect || EnableMemDetect ||
			    EnablePtraceDetect || EnableInlineHookDetect || EnablePltHookDetect || EnableMemProtect ||
			    EnableSyscallProtect ||
			    !SamsaraConfigPath.empty();

			if (hasObfuscation) {
				EnableIRObfuscation = true;
			}

			if (!EnableIRObfuscation) {
				bool Changed = run(M);
				return Changed;
			}

			if (!validateLicense(LicenseKey)) {
				errs() << "[ERROR] OLLVM license key is invalid or expired!\n";
				errs() << "[ERROR] Provide a valid time-based key via -mllvm -irobf-key=<key>\n";
				return false;
			}

			const auto Options(getOptions());
			unsigned   pointerSize = M.getDataLayout().getTypeAllocSize(
			                             PointerType::getUnqual(M.getContext()));

			if (isIRObfuscationDebugEnabled()) {
				errs() << "========================================\n";
				errs() << "[DEBUG] OLLVM protection enabled:\n";
				if (EnableIRFlattening) errs() << "  + Flattening\n";
				if (EnableIndirectBr) errs() << "  + IndirectBranch\n";
				if (EnableIndirectCall) errs() << "  + IndirectCall\n";
				if (EnableIndirectGV) errs() << "  + IndirectGlobalVariable\n";
				if (EnableIRStringEncryption) errs() << "  + StringEncryption\n";
				if (EnableIRConstantIntEncryption) errs() << "  + ConstantIntEncryption\n";
				if (EnableIRConstantFPEncryption) errs() << "  + ConstantFPEncryption\n";
				if (EnableRttiEraser) errs() << "  + RTTIEraser\n";
				if (EnableVMProtect) errs() << "  + VMProtect\n";
				if (EnableSyscallProtect) errs() << "  + SyscallProtect\n";
				if (EnableMemProtect) errs() << "  + MemProtect\n";
				if (EnableLdPreloadProtect) errs() << "  + LdPreloadProtect\n";
				if (EnableVmProtectDetect) errs() << "  + VmProtectDetect\n";
				if (EnableUsbProtect) errs() << "  + UsbProtect\n";
				if (EnableIdaDetect) errs() << "  + IdaDetect\n";
				if (EnableVpnDetect) errs() << "  + VpnDetect\n";
				if (EnableProxyDetect) errs() << "  + ProxyDetect\n";
				if (EnableTimeDetect) errs() << "  + TimeDetect\n";
				if (EnableHostsDetect) errs() << "  + HostsDetect\n";
				if (EnableMemDetect) errs() << "  + MemDetect\n";
				if (EnablePtraceDetect) errs() << "  + PtraceDetect\n";
				if (EnableInlineHookDetect) errs() << "  + InlineHookDetect\n";
				if (EnablePltHookDetect) errs() << "  + PltHookDetect\n";
				if (EnableRootDetect) errs() << "  + RootDetect\n";
				if (EnableNoRootDetect) errs() << "  + NoRootDetect\n";
				errs() << "========================================\n";
			}

			if (EnableSyscallProtect) {
				add(llvm::createSyscallProtectPass());
			}

			if (EnableVMProtect) {
				add(llvm::createVMProtectPass(true));
			}

			if (EnableLdPreloadProtect) {
				add(llvm::createLdPreloadProtectPass());
			}

			if (EnableVmProtectDetect) {
				add(llvm::createVmProtectDetectPass());
			}

			if (EnableUsbProtect) {
				add(llvm::createUsbProtectPass());
			}

			if (EnableIdaDetect) {
				add(llvm::createIdaDetectPass());
			}

			if (EnableVpnDetect) {
				add(llvm::createVpnDetectPass());
			}

			if (EnableProxyDetect) {
				add(llvm::createProxyDetectPass());
			}

			if (EnableTimeDetect) {
				add(llvm::createTimeDetectPass());
			}

			if (EnableHostsDetect) {
				add(llvm::createHostsDetectPass());
			}

			if (EnableMemDetect) {
				add(llvm::createMemDetectPass());
			}

			if (EnablePtraceDetect) {
				add(llvm::createPtraceDetectPass());
			}

			if (EnableInlineHookDetect) {
				add(llvm::createInlineHookDetectPass());
			}

			if (EnablePltHookDetect) {
				add(llvm::createPltHookDetectPass());
			}

			if (EnableHideMaps) {
				add(llvm::createHideMapsPass());
			}

			if (EnableFakeMaps) {
				add(llvm::createFakeMapsPass());
			}

			if (EnableMemProtect) {
				add(llvm::createMemProtectPass());
			}

			if (EnableRootDetect) {
				add(llvm::createRootDetectPass());
			}

			if (EnableNoRootDetect) {
				add(llvm::createNoRootDetectPass());
			}

			add(llvm::createAProtectPass());

		add(llvm::createConstantIntEncryptionPass(Options.get()));
		add(llvm::createConstantFPEncryptionPass(Options.get()));

		if (EnableIRStringEncryption || Options->cseOpt()->isEnabled()) {
			add(llvm::createStringEncryptionPass(Options.get()));
		}

		add(llvm::createIndirectGlobalVariablePass(Options.get()));

			add(llvm::createIndirectCallPass(Options.get()));
			add(llvm::createFlatteningPass(pointerSize, Options.get()));
			add(llvm::createIndirectBranchPass(Options.get()));

			if (EnableRttiEraser || Options->rttiOpt()->isEnabled()) {
				add(llvm::createMsRttiEraserPass(Options.get()));
			}

			bool Changed = run(M);

			return Changed;
		}
	};
} // namespace llvm

char ObfuscationPassManager::ID = 0;

/**
 * @brief 创建混淆Pass管理器
 * @return 返回创建的ModulePass指针
 */
ModulePass *llvm::createObfuscationPassManager() {
	return new ObfuscationPassManager();
}

INITIALIZE_PASS_BEGIN(ObfuscationPassManager, "irobf", "Enable IR Obfuscation",
                      false, false)
INITIALIZE_PASS_END(ObfuscationPassManager, "irobf", "Enable IR Obfuscation",
                    false, false)
