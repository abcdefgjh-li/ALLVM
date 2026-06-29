//===- IdaDetect.h - 调试器检测注入Pass -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件声明调试器检测注入Pass
// 在程序入口注入代码检测调试器端口和TracerPid
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_IDADETECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_IDADETECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createIdaDetectPass();
void initializeIdaDetectPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_IDADETECT_H
