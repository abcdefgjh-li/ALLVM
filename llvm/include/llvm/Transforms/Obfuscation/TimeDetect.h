//===- TimeDetect.h - 时间差调试检测注入Pass -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件声明时间差调试检测注入Pass
// 通过测量代码执行时间检测调试器
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_TIMEDETECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_TIMEDETECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createTimeDetectPass();
void initializeTimeDetectPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_TIMEDETECT_H
