//===- IndirectCall.cpp - 间接调用混淆Pass-----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现间接调用混淆Pass，通过将直接函数调用转换为间接调用
// 来增加逆向分析的难度
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Obfuscation/IndirectCall.h"
#include "llvm/Transforms/Obfuscation/ObfuscationOptions.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/Transforms/Obfuscation/Utils.h"
#include "llvm/CryptoUtils.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include "llvm/IR/Module.h"
#include "llvm/ADT/APInt.h"

#include <random>

#define DEBUG_TYPE "icall"

using namespace llvm;
namespace {
struct IndirectCall : public FunctionPass {
  static char ID;
  ObfuscationOptions *ArgsOptions;

  std::unordered_map<Function *, std::set<CallInst *>> FunctionCallSites;
  std::unordered_map<Function *, std::set<Function *>> FunctionCallees;

  std::vector<Constant *> Callees;
  std::unordered_map<Constant *, unsigned> CalleeIndex;
  // 63 - 32====31 - 0
  //  Mask=======Key
  std::unordered_map<Constant *, uint64_t> CalleeKeys;
  std::vector<GlobalVariable *> CalleePageTable;

  CryptoUtils RandomEngine;
  bool RunOnFuncChanged = false;

  /**
   * @brief 构造函数，初始化间接调用混淆Pass
   * @param argsOptions 混淆选项配置对象
   */
  IndirectCall(ObfuscationOptions *argsOptions) : FunctionPass(ID) {
    this->ArgsOptions = argsOptions;
  }

  StringRef getPassName() const override { return {"IndirectCall"}; }

  /**
   * @brief 编号被调用函数，收集需要混淆的调用指令
   * @param M 要处理的LLVM模块
   */
  void NumberCallees(Module &M) {
    for (auto &F : M) {
      if (F.isIntrinsic()) {
        continue;
      }

      for (auto &BB : F) {
        for (auto &I : BB) {
          if (auto CI = dyn_cast<CallInst>(&I)) {
            auto CB = dyn_cast<CallBase>(&I);
            auto Callee = CB->getCalledFunction();
            if (Callee == nullptr) {
              Callee = dyn_cast<Function>(CB->getCalledOperand()->stripPointerCasts());
              if (!Callee) {
                continue;
              }
            }
            if (Callee->isIntrinsic()) {
              continue;
            }

            FunctionCallSites[&F].emplace(CI);
            FunctionCallees[&F].emplace(Callee);

            if (CalleeKeys.count(Callee) == 0) {
              Callees.push_back(Callee);
              CalleeKeys[Callee] = RandomEngine.get_uint64_t();
            }
          }
        }
      }
    }
  }

  /**
   * @brief 初始化阶段，创建页表用于间接调用混淆
   * @param M 要处理的LLVM模块
   * @return 如果模块被修改返回true，否则返回false
   */
  bool doInitialization(Module &M) override {
    CalleeIndex.clear();
    FunctionCallSites.clear();
    FunctionCallees.clear();
    Callees.clear();
    CalleePageTable.clear();
    CalleeKeys.clear();

    NumberCallees(M);
    if (!Callees.size()) {
      return false;
    }

    CreatePageTableArgs createPageTableArgs;
    createPageTableArgs.CountLoop = 1;
    createPageTableArgs.GVNamePrefix = M.getName().str() + "_IndirectCallee" ;
    createPageTableArgs.RandomEngine = &RandomEngine;
    createPageTableArgs.M = &M;
    createPageTableArgs.Objects = &Callees;
    createPageTableArgs.IndexMap = &CalleeIndex;
    createPageTableArgs.ObjectKeys = &CalleeKeys;
    createPageTableArgs.OutPageTable = &CalleePageTable;

    createPageTable(createPageTableArgs);
    return false;
  }

  /**
   * @brief 对单个函数执行间接调用混淆
   * @param Fn 要处理的函数
   * @return 如果函数被修改返回true，否则返回false
   */
  bool runOnFunction(Function &Fn) override {
    if (isIRObfuscationDebugEnabled()) {
      errs() << "[DEBUG] IndirectCall: Starting runOnFunction: " << Fn.getName() << "\n";
    }
    const auto opt = ArgsOptions->toObfuscate(ArgsOptions->iCallOpt(), &Fn);
    if (!opt.isEnabled()) {
      return false;
    }

    // 跳过 VMP/SCase 解释器函数，icall 与 VMP/SCase 不兼容
    if (Fn.getName().contains("vm_interpreter") || Fn.getName().contains("scase_interpreter") || Fn.getName().ends_with("_original")) {
      return false;
    }

    auto& M = *Fn.getParent();

    if (Callees.empty()) {
      return false;
    }
    const auto& CallSites = FunctionCallSites[&Fn];
    auto& FuncCalleesSet = FunctionCallees[&Fn];

    if (CallSites.empty() || FuncCalleesSet.empty()) {
      return false;
    }

    std::vector<Constant *> FuncCallees;
    std::unordered_map<Constant *, uint64_t> FuncKeys;
    for (auto callee : FuncCalleesSet) {
      FuncCallees.push_back(callee);
      FuncKeys[callee] = RandomEngine.get_uint64_t();
    }

    std::vector<GlobalVariable *> FuncCalleePageTable;
    std::unordered_map<Constant *, unsigned> FuncCalleeIndex;

    if (opt.level()) {
      CreatePageTableArgs createPageTableArgs;
      createPageTableArgs.CountLoop = opt.level();
      createPageTableArgs.GVNamePrefix = M.getName().str() + Fn.getName().str() + "_IndirectCallee" ;
      createPageTableArgs.RandomEngine = &RandomEngine;
      createPageTableArgs.M = &M;
      createPageTableArgs.Objects = &FuncCallees;
      createPageTableArgs.IndexMap = &CalleeIndex;
      createPageTableArgs.ObjectKeys = &FuncKeys;
      createPageTableArgs.OutPageTable = &FuncCalleePageTable;

      enhancedPageTable(createPageTableArgs, &FuncCalleeIndex);
    }

    for (auto CI : CallSites) {

      CallBase *CB = CI;

      Function *Callee = CB->getCalledFunction();
      if (Callee == nullptr) {
        Callee = dyn_cast<Function>(CB->getCalledOperand()->stripPointerCasts());
        if (!Callee) {
          continue;
        }
      }

      BuildDecryptArgs buildDecrypt;
      buildDecrypt.FuncLoopCount = opt.level();
      buildDecrypt.NextIndex = opt.level() ?
                                 FuncCalleeIndex[Callee] :
                                 CalleeIndex[Callee];
      buildDecrypt.NextIndexValue = nullptr;
      buildDecrypt.Fn = &Fn;
      buildDecrypt.InsertBefore = CB;
      buildDecrypt.LoadTy = Callee->getType();
      buildDecrypt.ModulePageTable = &CalleePageTable;
      buildDecrypt.FuncPageTable = &FuncCalleePageTable;
      buildDecrypt.ModuleKey = CalleeKeys[Callee];
      buildDecrypt.FuncKey = FuncKeys[Callee];


      auto FnPtr = buildPageTableDecryptIR(buildDecrypt);
      FnPtr->setName("Call_" + Callee->getName());
      CB->setCalledOperand(FnPtr);
    }

    RunOnFuncChanged = true;
    return true;
  }

  /**
   * @brief 结束阶段，将页表添加到编译器使用的全局变量列表
   * @param M 要处理的LLVM模块
   * @return 如果模块被修改返回true，否则返回false
   */
  bool doFinalization(Module & M) override {
    if (!RunOnFuncChanged || CalleePageTable.empty()) {
      return false;
    }
    for (auto calleePage : CalleePageTable) {
      appendToCompilerUsed(M, {calleePage});
    }
    return true;
  }

};

} // namespace llvm

char IndirectCall::ID = 0;

/**
 * @brief 创建间接调用混淆Pass
 * @param argsOptions 混淆选项配置对象
 * @return 返回创建的FunctionPass指针
 */
FunctionPass *llvm::createIndirectCallPass(ObfuscationOptions *argsOptions) {
  return new IndirectCall(argsOptions);
}

INITIALIZE_PASS(IndirectCall, "icall", "Enable IR Indirect Call Obfuscation", false, false)
