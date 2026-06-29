//===- VpnDetect.h - VPN连接检测注入Pass -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件声明VPN连接检测注入Pass
// 在程序入口注入代码检测VPN连接
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_VPNDETECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_VPNDETECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createVpnDetectPass();
void initializeVpnDetectPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_VPNDETECT_H
