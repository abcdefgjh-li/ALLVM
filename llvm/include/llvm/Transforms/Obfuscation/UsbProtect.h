//===- UsbProtect.h - USB调试禁用注入Pass ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件声明USB调试禁用注入Pass
// 在程序入口注入代码禁用USB调试并检测是否成功
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_USBPROTECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_USBPROTECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createUsbProtectPass();
void initializeUsbProtectPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_USBPROTECT_H
