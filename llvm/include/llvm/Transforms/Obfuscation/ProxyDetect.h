//===- ProxyDetect.h - 代理/网络检测注入Pass -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件声明代理/网络检测注入Pass
// 检测代理应用、iptables规则等
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_PROXYDETECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_PROXYDETECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createProxyDetectPass();
void initializeProxyDetectPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_PROXYDETECT_H
