//===- SyscallProtect.h - 系统调用保护Pass -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件定义系统调用保护Pass，将recv/recvfrom/send/sendto/read/write/
// clock_gettime替换为直接syscall实现，绕过libc防止Hook注入
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_SYSCALLPROTECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_SYSCALLPROTECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createSyscallProtectPass();
void initializeSyscallProtectPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_SYSCALLPROTECT_H
