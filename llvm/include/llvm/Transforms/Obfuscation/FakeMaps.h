//===- FakeMaps.h - 生成假maps文件注入Pass -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_FAKEMAPS_H
#define LLVM_TRANSFORMS_OBFUSCATION_FAKEMAPS_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createFakeMapsPass();
void initializeFakeMapsPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_FAKEMAPS_H
