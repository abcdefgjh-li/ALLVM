//===- RootDetect.h - Root检测注入Pass头文件 -------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_ROOTDETECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_ROOTDETECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createRootDetectPass();
void initializeRootDetectPass(PassRegistry &Registry);

}

#endif
