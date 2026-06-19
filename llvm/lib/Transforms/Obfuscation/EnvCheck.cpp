//===- EnvCheck.cpp - 环境变量校验Pass实现 ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 环境变量校验Pass，配合 -firobf-linker 使用
// 仅在linker壳启用时注入，独立于其他检测Pass
//
// 原理：
// 1. 本Pass生成随机32位密钥，写入当前目录下的 .linker_env_key 文件
// 2. 同时将密钥直接嵌入到检测代码中
// 3. linker壳程序读取 .linker_env_key 文件获取密钥，设置环境变量 lc=<密钥>
// 4. 如果环境变量不存在或不匹配，说明程序被直接运行，kill进程
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/EnvCheck.h"
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
#include <random>
#include <fstream>

#define DEBUG_TYPE "envcheck"

using namespace llvm;

namespace {

struct EnvCheck : public ModulePass {
    static char ID;

    EnvCheck() : ModulePass(ID) {
        initializeEnvCheckPass(*PassRegistry::getPassRegistry());
    }

    StringRef getPassName() const override {
        return {"EnvCheck"};
    }

    bool runOnModule(Module &M) override;
};

}

char EnvCheck::ID = 0;

bool EnvCheck::runOnModule(Module &M) {
    if (isIRObfuscationDebugEnabled()) {
        errs() << "[DEBUG] EnvCheck: Injecting environment variable check\n";
    }

    Function *MainFunc = M.getFunction("main");
    if (!MainFunc || MainFunc->isDeclaration() || MainFunc->empty()) {
        return false;
    }

    // 生成随机32位密钥（hex字符串）
    std::string env_key(32, '\0');
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 15);
        const char hex[] = "0123456789abcdef";
        for (int i = 0; i < 32; i++)
            env_key[i] = hex[dist(gen)];
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

    // 写入 clang 同目录下的 .linker_env_key 文件
    SmallString<256> keyFilePath;
    sys::path::append(keyFilePath, clangDir, ".linker_env_key");
    std::error_code EC;
    raw_fd_ostream keyFile(keyFilePath, EC, sys::fs::OF_None);
    if (!EC) {
        keyFile << env_key;
        keyFile.close();
        if (isIRObfuscationDebugEnabled()) {
            errs() << "[DEBUG] EnvCheck: Generated key: " << env_key << "\n";
            errs() << "[DEBUG] EnvCheck: Key written to " << keyFilePath << "\n";
        }
    } else {
        errs() << "[EnvCheck] Warning: Cannot write " << keyFilePath << ": " << EC.message() << "\n";
    }

    // 创建报告函数
    Function *ReportFunc = DetectUtils::createReportAndKillFunc(M, "EnvCheck");

    // 创建环境变量校验函数（直接嵌入密钥）
    Function *CheckFunc = DetectUtils::createEnvVarCheckFunc(M, ReportFunc, env_key);

    // 注入到main函数入口
    BasicBlock &EntryBB = MainFunc->getEntryBlock();
    IRBuilder<> Builder(&EntryBB, EntryBB.getFirstInsertionPt());
    Builder.CreateCall(CheckFunc);

    return true;
}

ModulePass *llvm::createEnvCheckPass() {
    return new EnvCheck();
}

INITIALIZE_PASS_BEGIN(EnvCheck, "envcheck", "Inject environment variable check (requires linker wrapper)", false, false)
INITIALIZE_PASS_END(EnvCheck, "envcheck", "Inject environment variable check (requires linker wrapper)", false, false)
