//===- HostsDetect.h - Hosts文件检测注入Pass -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件声明Hosts文件检测注入Pass
// 检测/etc/hosts中的可疑内容
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_HOSTSDETECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_HOSTSDETECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createHostsDetectPass();
void initializeHostsDetectPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_HOSTSDETECT_H
