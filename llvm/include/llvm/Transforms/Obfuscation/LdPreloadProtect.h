//===- LdPreloadProtect.h - LD_PRELOAD检测注入Pass -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件声明LD_PRELOAD检测注入Pass
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_LDPRELOADPROTECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_LDPRELOADPROTECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createLdPreloadProtectPass();
void initializeLdPreloadProtectPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_LDPRELOADPROTECT_H
