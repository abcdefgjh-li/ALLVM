//===- GzEnvCheck.cpp - gz环境变量校验Pass实现 ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// gz环境变量校验Pass，配合 -firobf-gz 使用
// 仅在gz壳启用时注入，独立于其他检测Pass
//
// 原理：
// 1. 本Pass生成随机32位密钥，写入当前目录下的 .gz_env_key 文件
// 2. 同时将占位符嵌入到检测代码中
// 3. gz壳程序读取 .gz_env_key 文件获取密钥，在二进制中搜索占位符并替换
// 4. 壳程序设置环境变量 lc_gz=<密钥>
// 5. 如果环境变量不存在或不匹配，说明程序被直接运行，kill进程
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/GzEnvCheck.h"
#include "llvm/Transforms/Obfuscation/DetectUtils.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Process.h"
#include <random>
#include <fstream>

#define DEBUG_TYPE "gzenvcheck"

using namespace llvm;

namespace {

struct GzEnvCheck : public ModulePass {
    static char ID;

    GzEnvCheck() : ModulePass(ID) {
        initializeGzEnvCheckPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"GzEnvCheck"};
    }

    bool runOnModule(Module &M) override;
};

}

char GzEnvCheck::ID = 0;

bool GzEnvCheck::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] GzEnvCheck: Injecting gz environment variable check\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 生成随机32位密钥（hex字符串）
    std::string gz_env_key(32, '\0');
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 15);
        const char hex[] = "0123456789abcdef";
        for (int i = 0; i < 32; i++)
            gz_env_key[i] = hex[dist(gen)];
    }

    // 获取 clang 可执行文件所在目录
    std::string clangDir = ".";
    {
        // 使用 sys::fs::getMainExecutable 获取当前程序路径
        std::string exePath = sys::fs::getMainExecutable(nullptr, nullptr);
        if (!exePath.empty()) {
            clangDir = sys::path::parent_path(exePath).str();
        }
    }

    // 写入 clang 同目录下的 .gz_env_key[.<tag>] 文件
    SmallString<256> keyFilePath;
    std::string keyFileName = ".gz_env_key";
    if (auto keyTag = sys::Process::GetEnv("IROBF_KEY_TAG"))
        keyFileName += "." + *keyTag;
    sys::path::append(keyFilePath, clangDir, keyFileName);
    std::error_code EC;
    raw_fd_ostream keyFile(keyFilePath, EC, sys::fs::OF_None);
    if (!EC) {
        keyFile << gz_env_key;
        keyFile.close();
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[DEBUG] GzEnvCheck: Generated key: " << gz_env_key << "\n";
            errs() << "[DEBUG] GzEnvCheck: Key written to " << keyFilePath << "\n";
        }
    } else {
        errs() << "[GzEnvCheck] Warning: Cannot write " << keyFilePath << ": " << EC.message() << "\n";
    }

    // 创建报告函数
    Function *ReportFunc = DetectUtils::createReportAndKillFunc(M, "GzEnvCheck");

    // 创建gz环境变量校验函数（直接嵌入密钥）
    Function *CheckFunc = DetectUtils::createGzEnvVarCheckFunc(M, ReportFunc, gz_env_key);

    // 注入到main函数入口
    BasicBlock &EntryBB = MainFunc->getEntryBlock();
    IRBuilder<> Builder(&EntryBB, EntryBB.getFirstInsertionPt());
    Builder.CreateCall(CheckFunc);

    return true;
}

ModulePass *llvm::createGzEnvCheckPass() {
    return new GzEnvCheck();
}

INITIALIZE_PASS_BEGIN(GzEnvCheck, "gzenvcheck", "Inject gz environment variable check (requires gz wrapper)", false, false)
INITIALIZE_PASS_END(GzEnvCheck, "gzenvcheck", "Inject gz environment variable check (requires gz wrapper)", false, false)
