//===- MicrosoftRTTIEraser.cpp - Microsoft RTTI擦除Pass----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现Microsoft RTTI擦除Pass，通过随机化RTTI类型名称
// 来增加逆向分析的难度
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/ObfuscationOptions.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/Transforms/Obfuscation/StringEncryption.h"
#include "llvm/Transforms/Obfuscation/Utils.h"
#include "llvm/Transforms/Utils/GlobalStatus.h"
#include "llvm/Transforms/IPO/Attributor.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/CryptoUtils.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/CryptoUtils.h"
#include "llvm/Transforms/Obfuscation/MicrosoftRTTIEraser.h"

#define DEBUG_TYPE "ms_rtti_eraser"


using namespace llvm;

namespace {
class MsRttiEraser : public ModulePass {
protected:
  ObfuscationOptions *ArgsOptions;
  CryptoUtils         RandomEngine;

public:
  static char ID;

  /**
   * @brief 构造函数，初始化Microsoft RTTI擦除Pass
   * @param argsOptions 混淆选项配置对象
   */
  MsRttiEraser(ObfuscationOptions *argsOptions) : ModulePass(ID) {
    this->ArgsOptions = argsOptions;
    initializeStringEncryptionPass(*PassRegistry::getPassRegistry());
  }

  StringRef getPassName() const override {
    return "MsRttiEraser";
  }

  /**
   * @brief 对模块执行RTTI擦除
   * @param M 要处理的LLVM模块
   * @return 如果模块被修改返回true，否则返回false
   */
  bool runOnModule(Module &M) override {
    if (isIRObfuscationDebugEnabled()) {
      errs() << "[DEBUG] MicrosoftRTTIEraser: Starting runOnModule\n";
    }
    bool         changed = false;
    LLVMContext &ctx = M.getContext();
    for (GlobalVariable &gv : M.globals()) {
      if (gv.isConstant() || !gv.hasInitializer()) {
        continue;
      }
      if (!gv.hasName() || !gv.getName().starts_with("??_R0")) {
        continue;
      }
      Type *tyGv = gv.getValueType();
      if (!tyGv->isStructTy() ||
          !tyGv->getStructName().starts_with("rtti.TypeDescriptor")) {
        continue;
      }

      ConstantStruct *   initStruct = cast<ConstantStruct>(gv.getInitializer());
      ConstantDataArray *rttiDA = cast<ConstantDataArray>(
          initStruct->getOperand(2));
      if (!rttiDA->isString()) {
        report_fatal_error("RTTI[2] is not a string.");
      }
      StringRef tyInfoString = rttiDA->getAsString();
      if (!tyInfoString.starts_with(".?AV") &&
          !tyInfoString.starts_with(".?AU")
      ) {
        continue;
      }
      auto      newRttiName = randomRttiName(tyInfoString);
      Constant *newRttiNameConstant = ConstantDataArray::getString(
          ctx, newRttiName, newRttiName[newRttiName.size() - 1] != '\0');

      initStruct->setOperand(2, newRttiNameConstant);
      changed = true;
    }
    return changed;
  }

  /**
   * @brief 生成随机的RTTI名称
   * @param rtti 原始RTTI字符串
   * @return 返回随机化后的RTTI名称
   */
  SmallString<256> randomRttiName(StringRef rtti) {
    constexpr char pwTable[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    uint32_t randomSeed = RandomEngine.get_uint32_t();

    SmallString<512> passwd;
    passwd.append(std::to_string(randomSeed));
    passwd.append(rtti);
    uint8_t hash[32];
    RandomEngine.sha256(passwd.c_str(), hash);

    SmallString<256> result = rtti;

    for (unsigned i = 4; i < result.size(); ++i) {
      const char currentChar = result[i];
      if (currentChar == '\0') {
        break;
      }
      if (currentChar == '@' || currentChar == '.' || currentChar == '?' ||
          currentChar == '$') {
        continue;
      }
      result[i] = pwTable[(currentChar ^ hash[i % sizeof(hash)]) % (sizeof(pwTable) - 1)];
    }
    return result;
  }

  /**
   * @brief 结束阶段
   * @param M 要处理的LLVM模块
   * @return 返回false
   */
  bool doFinalization(Module &) override {
    return false;
  }
};

} // end anonymous namespace

char MsRttiEraser::ID = 0;

/**
 * @brief 创建Microsoft RTTI擦除Pass
 * @param argsOptions 混淆选项配置对象
 * @return 返回创建的ModulePass指针
 */
ModulePass *llvm::createMsRttiEraserPass(ObfuscationOptions *argsOptions) {
  return new MsRttiEraser(argsOptions);
}

INITIALIZE_PASS(MsRttiEraser, "ms_rtti_eraser", "Enable Microsoft RTTI Eraser",
                false, false)