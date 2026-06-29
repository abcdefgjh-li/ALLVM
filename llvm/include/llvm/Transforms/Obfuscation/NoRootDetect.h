//===- NoRootDetect.h - 无Root检测注入Pass头文件 ----------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_NOROOTDETECT_H
#define LLVM_TRANSFORMS_OBFUSCATION_NOROOTDETECT_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

namespace llvm {

class PassRegistry;

ModulePass *createNoRootDetectPass();
void initializeNoRootDetectPass(PassRegistry &Registry);

}

#endif
