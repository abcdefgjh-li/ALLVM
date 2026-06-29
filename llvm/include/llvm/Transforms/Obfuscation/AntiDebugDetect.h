//===- AntiDebugDetect.h - 反调试检测注入Pass -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件声明反调试检测注入Pass
// 包含时间差调试检测和hosts文件检测
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_ANTIDEBUGDETECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_ANTIDEBUGDETECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createAntiDebugDetectPass();
void initializeAntiDebugDetectPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_ANTIDEBUGDETECT_H
