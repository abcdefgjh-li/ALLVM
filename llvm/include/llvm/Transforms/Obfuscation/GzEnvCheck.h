//===- GzEnvCheck.h - gz环境变量校验Pass -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// gz环境变量校验Pass，配合 -firobf-gz 使用
// gz壳程序在execv前设置环境变量 lc_gz=<随机32位字符串>
// 本Pass注入代码读取getenv("lc_gz")并与内嵌密钥比较
// 密钥初始为占位符，GzWrapper加壳时在二进制中替换为实际密钥
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_GZENVCHECK_H
#define LLVM_TRANSFORMS_OBFUSCATION_GZENVCHECK_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createGzEnvCheckPass();
void initializeGzEnvCheckPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_GZENVCHECK_H
