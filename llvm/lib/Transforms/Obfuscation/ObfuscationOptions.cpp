//===- ObfuscationOptions.cpp - 混淆选项配置文件----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现混淆选项配置，从配置文件读取混淆参数
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/ErrorOr.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Transforms/Obfuscation/ObfuscationOptions.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/Support/JSON.h"

using namespace llvm;

namespace llvm {


/**
 * @brief 读取函数的注解信息
 * @param f 要读取注解的函数
 * @return 返回注解字符串列表
 */
SmallVector<std::string> readAnnotate(Function *f) {
  SmallVector<std::string> annotations;

  auto *Annotations = f->getParent()->getGlobalVariable(
      "llvm.global.annotations");
  auto *C = dyn_cast_or_null<Constant>(Annotations);
  if (!C || C->getNumOperands() != 1)
    return annotations;

  C = cast<Constant>(C->getOperand(0));

  for (auto &Op : C->operands()) {
    auto *OpC = dyn_cast<ConstantStruct>(&Op);
    if (!OpC || OpC->getNumOperands() < 2)
      continue;
    auto *Fn = dyn_cast<Function>(OpC->getOperand(0)->stripPointerCasts());
    if (Fn != f)
      continue;
    auto *StrC = dyn_cast<GlobalValue>(OpC->getOperand(1)->stripPointerCasts());
    if (!StrC)
      continue;
    auto *StrData = dyn_cast<ConstantDataSequential>(StrC->getOperand(0));
    if (!StrData)
      continue;
    annotations.emplace_back(StrData->getAsString());
  }
  return annotations;
}

/**
 * @brief 从配置文件读取混淆选项
 * @param FileName 配置文件路径
 * @return 返回混淆选项对象的智能指针
 */
std::shared_ptr<ObfuscationOptions> ObfuscationOptions::readConfigFile(
    const Twine &FileName) {

  std::shared_ptr<ObfuscationOptions> result = std::make_shared<
    ObfuscationOptions>();
  if (FileName.str().empty()) {
    return result;
  }
  if (!sys::fs::exists(FileName)) {
    report_fatal_error("Config file doesn't exist: " + FileName);
  }

  auto BufOrErr = MemoryBuffer::getFileOrSTDIN(FileName);
  if (const auto ErrCode = BufOrErr.getError()) {
    report_fatal_error(
        ("Can not read config file: " + ErrCode.message()).c_str());
  }

  const auto &    buf = *BufOrErr.get();
  llvm::SourceMgr sm;

  auto jsonRoot = json::parse(buf.getBuffer());
  if (!jsonRoot) {
    report_fatal_error(jsonRoot.takeError());
  }
  auto rootObj = jsonRoot->getAsObject();
  if (!rootObj) {
    report_fatal_error("Json root is not an object.");
  }

  static auto procObj = [](const std::shared_ptr<ObfOpt> &obfOpt,
                           const detail::DenseMapPair<
                             json::ObjectKey, json::Value> &
                           obj) ->bool {

    static auto procOptValue = [](const std::shared_ptr<ObfOpt> &obfOpt,
                                  const json::Value &            value) {
      if (auto optObj = value.getAsObject()) {
        if (auto enable = optObj->getBoolean("enable")) {
          obfOpt->setEnable(enable.value());
        }
        if (auto level = optObj->getInteger("level")) {
          obfOpt->setLevel(static_cast<uint32_t>(level.value()));
        }
      }
    };

    std::string key = obj.getFirst().str();
    auto &      value = obj.getSecond();

    if (key == obfOpt->attributeName()) {
      procOptValue(obfOpt, value);
      return true;
    }
    return false;
  };

  SmallVector<std::shared_ptr<ObfOpt>, 16> allOpt = result->getAllOpt();
  for (auto &obj : *rootObj) {
    if (obj.getFirst().str() == "randomSeed") {
      if (auto objStr = obj.getSecond().getAsString()) {
        const auto &seedStr = objStr.value();
        auto &seed = result->randomSeed();
        seed = seedStr;
        seed.resize(32, 0);
      }
      continue;
    }
    bool objHit = false;
    for (auto &opt : allOpt) {
      if ((objHit = procObj(opt, obj))) {
        break;
      }
    }
    if (!objHit) {
      llvm::errs() << "warning: unknown hikari config node: "
        << obj.getFirst().str() << '\n';
    }
  }
  return result;
}

/**
 * @brief 根据函数和选项确定是否执行混淆
 * @param option 混淆选项
 * @param f 要处理的函数
 * @return 返回混淆选项对象
 */
ObfOpt ObfuscationOptions::toObfuscate(const std::shared_ptr<ObfOpt> &option,
                                       Function *                     f) {
  const auto attrEnable = "+" + option->attributeName();
  const auto attrDisable = "-" + option->attributeName();
  const auto attrLevel = "^" + option->attributeName();
  ObfOpt     result = option->none();
  if (f->isDeclaration()) {
    return result;
  }

  if (f->hasAvailableExternallyLinkage() != 0) {
    return result;
  }

  bool annotationEnableFound = false;
  bool annotationDisableFound = false;

  auto annotations = readAnnotate(f);
  int  levelSet = 0;
  if (!annotations.empty()) {
    for (const auto &annotation : annotations) {
      if (annotation.find(attrDisable) != std::string::npos) {
        result.setEnable(false);
        annotationDisableFound = true;
      }
      if (annotation.find(attrEnable) != std::string::npos) {
        result.setEnable(true);
        annotationEnableFound = true;
      }
      if (const auto levelPos = annotation.find(attrLevel);
        levelPos != std::string::npos) {
        if (annotation.find(attrLevel, levelPos + 1) != std::string::npos) {
          f->getContext().diagnose(DiagnosticInfoUnsupported{
              *f,
              f->getName() + " has multiple annotations for setting " + result.
              attributeName() +
              " factors, What are you the fucking want to do?"});
          return result.none();
        }
        int32_t    level = -1;
        const auto equalPos = annotation.find('=', levelPos + 1);
        if (equalPos == std::string::npos) {
          f->getContext().diagnose(DiagnosticInfoUnsupported{
              *f,
              f->getName() + ": " + annotation +
              " missing equal sign, sample: " + attrLevel + " = 0"});
          return result.none();
        }

        for (size_t i = levelPos + attrLevel.length(); i < equalPos; ++i) {
          if (annotation[i] == ' ') {
            continue;
          }
          f->getContext().diagnose(DiagnosticInfoUnsupported{
              *f,
              f->getName() + ": " + annotation +
              " unexpected characters, sample: " + attrLevel + " = 0"});
          return result.none();
        }

        for (size_t i = equalPos + 1; i < annotation.length(); ++i) {
          if (annotation[i] == ' ') {
            continue;
          }
          level = annotation[i] - '0';
          if (level < 0 || level > 9) {
            f->getContext().diagnose(DiagnosticInfoUnsupported{
                *f,
                f->getName() + ": " + annotation +
                " unexpected character: " + std::string{annotation[i]} +
                ", sample: " + attrLevel + " = 0"});
            return result.none();
          }
          break;
        }
        if (level == -1) {
          f->getContext().diagnose(DiagnosticInfoUnsupported{
              *f,
              f->getName() + ": " + annotation +
              " level value not found, sample: " + attrLevel + " = 0"});
          return result.none();
        }

        ++levelSet;
        result.setLevel(level);
      }
    }
  }

  if (annotationDisableFound && annotationEnableFound) {
    f->getContext().diagnose(DiagnosticInfoUnsupported{
        *f,
        f->getName() +
        " having both enable annotation and disable annotation, What are you the fucking want to do?"});
    return result.none();
  }

  if (levelSet > 1) {
    f->getContext().diagnose(DiagnosticInfoUnsupported{
        *f,
        f->getName() + " has multiple annotations for setting " + result.
        attributeName() + " factors, What are you the fucking want to do?"});
    return result.none();
  }

  if (!annotationDisableFound && !annotationEnableFound) {
    result.setEnable(option->isEnabled());
  }
  if (!levelSet) {
    result.setLevel(option->level());
  }
  return result;
}

}