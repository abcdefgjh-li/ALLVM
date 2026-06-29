//===- AProtect.h - A-protector Pass头文件 ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// A-protector 输出注入 Pass
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_APROTECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_APROTECT_H

namespace llvm {
class ModulePass;
class PassRegistry;

ModulePass* createAProtectPass();
void initializeAProtectPass(PassRegistry &Registry);
}

#endif
