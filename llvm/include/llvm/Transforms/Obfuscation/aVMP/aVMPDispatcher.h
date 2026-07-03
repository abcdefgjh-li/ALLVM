#ifndef LLVM_LIB_TRANSFORMS_OBFUSCATION_AVMPDISPATCHER_H
#define LLVM_LIB_TRANSFORMS_OBFUSCATION_AVMPDISPATCHER_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Module.h"

struct VMPDispatchTarget {
    llvm::GlobalVariable *CodeSeg;
    llvm::Function *CallHandler;
};

llvm::Function *buildSharedCallDispatcher(
    llvm::Module *M,
    llvm::ArrayRef<VMPDispatchTarget> Targets,
    llvm::GlobalVariable *SharedCodeSegAddr);

#endif
