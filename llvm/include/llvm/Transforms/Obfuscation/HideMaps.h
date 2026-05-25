//===- HideMaps.h - 隐藏maps文件注入Pass ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_HIDEMAPS_H
#define LLVM_TRANSFORMS_OBFUSCATION_HIDEMAPS_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createHideMapsPass();
void initializeHideMapsPass(PassRegistry &Registry);

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_HIDEMAPS_H
