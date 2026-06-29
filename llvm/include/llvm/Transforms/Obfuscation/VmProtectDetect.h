//===- VmProtectDetect.h - 虚拟机文件检测注入Pass --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件声明虚拟机文件检测注入Pass
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_VMPROTECTDETECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_VMPROTECTDETECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createVmProtectDetectPass();
void initializeVmProtectDetectPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_VMPROTECTDETECT_H
