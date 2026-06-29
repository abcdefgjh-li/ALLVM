//===- BanDump.h - 禁用内存dump注入Pass ------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件声明禁用内存dump注入Pass
// 通过修改内存区域权限防止内存被读取dump
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_BANDUMP_H
#define LLVM_TRANSFORMS_OBFUSCATION_BANDUMP_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createBanDumpPass();
void initializeBanDumpPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_BANDUMP_H
