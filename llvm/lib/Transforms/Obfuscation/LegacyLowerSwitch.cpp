//===- LowerSwitch.cpp - 消除Switch指令---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// LowerSwitch转换将switch指令重写为分支指令序列，
// 这允许目标在方便之前不实现switch指令
//
//===----------------------------------------------------------------------===//
#include "llvm/Transforms/Obfuscation/LegacyLowerSwitch.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/Pass.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <limits>
#include <vector>

using namespace llvm;

#define DEBUG_TYPE "lower-switch"

namespace {

  struct IntRange {
    int64_t Low, High;
  };

} // end anonymous namespace

/**
 * @brief 检查范围R是否被Ranges覆盖
 * @param R 要检查的范围
 * @param Ranges 范围列表
 * @return 如果被覆盖返回true，否则返回false
 */
static bool IsInRanges(const IntRange &R,
                       const std::vector<IntRange> &Ranges) {
  auto I = std::lower_bound(
      Ranges.begin(), Ranges.end(), R,
      [](const IntRange &A, const IntRange &B) { return A.High < B.High; });
  return I != Ranges.end() && I->Low <= R.Low;
}

namespace {

  /// Replace all SwitchInst instructions with chained branch instructions.
  class LowerSwitch : public FunctionPass {
  public:
    static char ID;

    LowerSwitch() : FunctionPass(ID) {
    }

    /**
     * @brief 对函数执行LowerSwitch转换
     * @param F 要处理的函数
     * @return 如果函数被修改返回true，否则返回false
     */
    bool runOnFunction(Function &F) override;

    struct CaseRange {
      ConstantInt* Low;
      ConstantInt* High;
      BasicBlock* BB;

      CaseRange(ConstantInt *low, ConstantInt *high, BasicBlock *bb)
          : Low(low), High(high), BB(bb) {}
    };

    using CaseVector = std::vector<CaseRange>;
    using CaseItr = std::vector<CaseRange>::iterator;

  private:
    /**
     * @brief 处理switch指令
     * @param SI 要处理的switch指令
     * @param DeleteList 要删除的基本块列表
     */
    void processSwitchInst(SwitchInst *SI, SmallPtrSetImpl<BasicBlock*> &DeleteList);

    /**
     * @brief 将switch转换为二分查找树
     * @param Begin 案例范围开始迭代器
     * @param End 案例范围结束迭代器
     * @param LowerBound 下界常量
     * @param UpperBound 上界常量
     * @param Val switch的值
     * @param Predecessor 前驱基本块
     * @param OrigBlock 原始基本块
     * @param Default 默认基本块
     * @param UnreachableRanges 不可达范围列表
     * @return 返回创建的基本块
     */
    BasicBlock *switchConvert(CaseItr Begin, CaseItr End,
                              ConstantInt *LowerBound, ConstantInt *UpperBound,
                              Value *Val, BasicBlock *Predecessor,
                              BasicBlock *OrigBlock, BasicBlock *Default,
                              const std::vector<IntRange> &UnreachableRanges);
    /**
     * @brief 创建新的叶子基本块
     * @param Leaf 案例范围
     * @param Val switch的值
     * @param OrigBlock 原始基本块
     * @param Default 默认基本块
     * @return 返回创建的叶子基本块
     */
    BasicBlock *newLeafBlock(CaseRange &Leaf, Value *Val, BasicBlock *OrigBlock,
                             BasicBlock *Default);
    /**
     * @brief 将简单案例列表转换为案例范围列表
     * @param Cases 案例向量
     * @param SI switch指令
     * @return 返回比较次数
     */
    unsigned Clusterify(CaseVector &Cases, SwitchInst *SI);
  };

  /// The comparison function for sorting the switch case values in the vector.
  /// WARNING: Case ranges should be disjoint!
  struct CaseCmp {
    bool operator()(const LowerSwitch::CaseRange& C1,
                    const LowerSwitch::CaseRange& C2) {
      const ConstantInt* CI1 = cast<const ConstantInt>(C1.Low);
      const ConstantInt* CI2 = cast<const ConstantInt>(C2.High);
      return CI1->getValue().slt(CI2->getValue());
    }
  };

} // end anonymous namespace

char LowerSwitch::ID = 0;

// Publicly exposed interface to pass...
//char &llvm::LowerSwitchID = LowerSwitch::ID;

//INITIALIZE_PASS(LowerSwitch, "lowerswitch",
//                "Lower SwitchInst's to branches", false, false)

/**
 * @brief 创建LowerSwitch Pass
 * @return 返回创建的FunctionPass指针
 */
FunctionPass *llvm::createLegacyLowerSwitchPass() {
  return new LowerSwitch();
}

bool LowerSwitch::runOnFunction(Function &F) {
  bool Changed = false;
  SmallPtrSet<BasicBlock*, 8> DeleteList;

  for (Function::iterator I = F.begin(), E = F.end(); I != E; ) {
    BasicBlock *Cur = &*I++;

    if (DeleteList.count(Cur))
      continue;

    if (SwitchInst *SI = dyn_cast<SwitchInst>(Cur->getTerminator())) {
      Changed = true;
      processSwitchInst(SI, DeleteList);
    }
  }

  for (BasicBlock* BB: DeleteList) {
    DeleteDeadBlock(BB);
  }

  return Changed;
}

/// Used for debugging purposes.
LLVM_ATTRIBUTE_USED
static raw_ostream &operator<<(raw_ostream &O,
                               const LowerSwitch::CaseVector &C) {
  O << "[";

  for (LowerSwitch::CaseVector::const_iterator B = C.begin(),
         E = C.end(); B != E; ) {
    O << *B->Low << " -" << *B->High;
    if (++B != E) O << ", ";
  }

  return O << "]";
}

/// Update the first occurrence of the "switch statement" BB in the PHI
/// node with the "new" BB. The other occurrences will:
///
/// 1) Be updated by subsequent calls to this function.  Switch statements may
/// have more than one outcoming edge into the same BB if they all have the same
/// value. When the switch statement is converted these incoming edges are now
/// coming from multiple BBs.
/// 2) Removed if subsequent incoming values now share the same case, i.e.,
/// multiple outcome edges are condensed into one. This is necessary to keep the
/// number of phi values equal to the number of branches to SuccBB.
static void fixPhis(BasicBlock *SuccBB, BasicBlock *OrigBB, BasicBlock *NewBB,
                    unsigned NumMergedCases) {
  for (BasicBlock::iterator I = SuccBB->begin(),
                            IE = SuccBB->getFirstNonPHIIt();
       I != IE; ++I) {
    PHINode *PN = cast<PHINode>(I);

    unsigned Idx = 0, E = PN->getNumIncomingValues();
    unsigned LocalNumMergedCases = NumMergedCases;
    for (; Idx != E; ++Idx) {
      if (PN->getIncomingBlock(Idx) == OrigBB) {
        PN->setIncomingBlock(Idx, NewBB);
        break;
      }
    }

    SmallVector<unsigned, 8> Indices;
    for (++Idx; LocalNumMergedCases > 0 && Idx < E; ++Idx)
      if (PN->getIncomingBlock(Idx) == OrigBB) {
        Indices.push_back(Idx);
        LocalNumMergedCases--;
      }
    for (unsigned III : llvm::reverse(Indices))
      PN->removeIncomingValue(III);
  }
}

/// Convert the switch statement into a binary lookup of the case values.
/// The function recursively builds this tree. LowerBound and UpperBound are
/// used to keep track of the bounds for Val that have already been checked by
/// a block emitted by one of the previous calls to switchConvert in the call
/// stack.
BasicBlock *
LowerSwitch::switchConvert(CaseItr Begin, CaseItr End, ConstantInt *LowerBound,
                           ConstantInt *UpperBound, Value *Val,
                           BasicBlock *Predecessor, BasicBlock *OrigBlock,
                           BasicBlock *Default,
                           const std::vector<IntRange> &UnreachableRanges) {
  unsigned Size = End - Begin;

  if (Size == 1) {
    if (Begin->Low == LowerBound && Begin->High == UpperBound) {
      unsigned NumMergedCases = 0;
      if (LowerBound && UpperBound)
        NumMergedCases =
            UpperBound->getSExtValue() - LowerBound->getSExtValue();
      fixPhis(Begin->BB, OrigBlock, Predecessor, NumMergedCases);
      return Begin->BB;
    }
    return newLeafBlock(*Begin, Val, OrigBlock, Default);
  }

  unsigned Mid = Size / 2;
  std::vector<CaseRange> LHS(Begin, Begin + Mid);
  LLVM_DEBUG(dbgs() << "LHS: " << LHS << "\n");
  std::vector<CaseRange> RHS(Begin + Mid, End);
  LLVM_DEBUG(dbgs() << "RHS: " << RHS << "\n");

  CaseRange &Pivot = *(Begin + Mid);
  LLVM_DEBUG(dbgs() << "Pivot ==> " << Pivot.Low->getValue() << " -"
                    << Pivot.High->getValue() << "\n");

  ConstantInt *NewLowerBound = Pivot.Low;

  ConstantInt *NewUpperBound = ConstantInt::get(NewLowerBound->getContext(),
                                                NewLowerBound->getValue() - 1);

  if (!UnreachableRanges.empty()) {
    int64_t GapLow = LHS.back().High->getSExtValue() + 1;
    int64_t GapHigh = NewLowerBound->getSExtValue() - 1;
    IntRange Gap = { GapLow, GapHigh };
    if (GapHigh >= GapLow && IsInRanges(Gap, UnreachableRanges))
      NewUpperBound = LHS.back().High;
  }

  LLVM_DEBUG(dbgs() << "LHS Bounds ==> "; if (LowerBound) {
    dbgs() << LowerBound->getSExtValue();
  } else { dbgs() << "NONE"; } dbgs() << " - "
                                      << NewUpperBound->getSExtValue() << "\n";
             dbgs() << "RHS Bounds ==> ";
             dbgs() << NewLowerBound->getSExtValue() << " - "; if (UpperBound) {
               dbgs() << UpperBound->getSExtValue() << "\n";
             } else { dbgs() << "NONE\n"; });

  Function* F = OrigBlock->getParent();
  BasicBlock* NewNode = BasicBlock::Create(Val->getContext(), "NodeBlock");

  ICmpInst* Comp = new ICmpInst(ICmpInst::ICMP_SLT,
                                Val, Pivot.Low, "Pivot");

  BasicBlock *LBranch = switchConvert(LHS.begin(), LHS.end(), LowerBound,
                                      NewUpperBound, Val, NewNode, OrigBlock,
                                      Default, UnreachableRanges);
  BasicBlock *RBranch = switchConvert(RHS.begin(), RHS.end(), NewLowerBound,
                                      UpperBound, Val, NewNode, OrigBlock,
                                      Default, UnreachableRanges);

  F->insert(++OrigBlock->getIterator(), NewNode);
  Comp->insertInto(NewNode, NewNode->end());

  BranchInst::Create(LBranch, RBranch, Comp, NewNode);
  return NewNode;
}

/// Create a new leaf block for the binary lookup tree. It checks if the
/// switch's value == the case's value. If not, then it jumps to the default
/// branch. At this point in the tree, the value can't be another valid case
/// value, so the jump to the "default" branch is warranted.
BasicBlock* LowerSwitch::newLeafBlock(CaseRange& Leaf, Value* Val,
                                      BasicBlock* OrigBlock,
                                      BasicBlock* Default) {
  Function* F = OrigBlock->getParent();
  BasicBlock* NewLeaf = BasicBlock::Create(Val->getContext(), "LeafBlock");
  F->insert(++OrigBlock->getIterator(), NewLeaf);

  ICmpInst* Comp;
  if (Leaf.Low == Leaf.High) {
    Comp = new ICmpInst(NewLeaf->end(), ICmpInst::ICMP_EQ, Val,
                        Leaf.Low, "SwitchLeaf");
  } else {
    if (Leaf.Low->isMinValue(true /*isSigned*/)) {
      Comp = new ICmpInst(NewLeaf->end(), ICmpInst::ICMP_SLE, Val, Leaf.High,
                          "SwitchLeaf");
    } else if (Leaf.Low->isZero()) {
      Comp = new ICmpInst(NewLeaf->end(), ICmpInst::ICMP_ULE, Val, Leaf.High,
                          "SwitchLeaf");
    } else {
      Constant* NegLo = ConstantExpr::getNeg(Leaf.Low);
      Instruction* Add = BinaryOperator::CreateAdd(Val, NegLo,
                                                   Val->getName()+".off",
                                                   NewLeaf);
      Constant *UpperBound = ConstantExpr::getAdd(NegLo, Leaf.High);
      Comp = new ICmpInst(NewLeaf->end(), ICmpInst::ICMP_ULE, Add, UpperBound,
                          "SwitchLeaf");
    }
  }

  BasicBlock* Succ = Leaf.BB;
  BranchInst::Create(Succ, Default, Comp, NewLeaf);

  for (BasicBlock::iterator I = Succ->begin(); isa<PHINode>(I); ++I) {
    PHINode* PN = cast<PHINode>(I);
    uint64_t Range = Leaf.High->getSExtValue() -
                     Leaf.Low->getSExtValue();
    for (uint64_t j = 0; j < Range; ++j) {
      PN->removeIncomingValue(OrigBlock);
    }

    int BlockIdx = PN->getBasicBlockIndex(OrigBlock);
    assert(BlockIdx != -1 && "Switch didn't go to this successor??");
    PN->setIncomingBlock((unsigned)BlockIdx, NewLeaf);
  }

  return NewLeaf;
}

/// Transform simple list of Cases into list of CaseRange's.
unsigned LowerSwitch::Clusterify(CaseVector& Cases, SwitchInst *SI) {
  unsigned numCmps = 0;

  for (auto Case : SI->cases())
    Cases.push_back(CaseRange(Case.getCaseValue(), Case.getCaseValue(),
                              Case.getCaseSuccessor()));

  llvm::sort(Cases.begin(), Cases.end(), CaseCmp());

  if (Cases.size() >= 2) {
    CaseItr I = Cases.begin();
    for (CaseItr J = std::next(I), E = Cases.end(); J != E; ++J) {
      int64_t nextValue = J->Low->getSExtValue();
      int64_t currentValue = I->High->getSExtValue();
      BasicBlock* nextBB = J->BB;
      BasicBlock* currentBB = I->BB;

      assert(nextValue > currentValue && "Cases should be strictly ascending");
      if ((nextValue == currentValue + 1) && (currentBB == nextBB)) {
        I->High = J->High;
      } else if (++I != J) {
        *I = *J;
      }
    }
    Cases.erase(std::next(I), Cases.end());
  }

  for (CaseItr I=Cases.begin(), E=Cases.end(); I!=E; ++I, ++numCmps) {
    if (I->Low != I->High)
      ++numCmps;
  }

  return numCmps;
}

/// Replace the specified switch instruction with a sequence of chained if-then
/// insts in a balanced binary search.
void LowerSwitch::processSwitchInst(SwitchInst *SI,
                                    SmallPtrSetImpl<BasicBlock*> &DeleteList) {
  BasicBlock *CurBlock = SI->getParent();
  BasicBlock *OrigBlock = CurBlock;
  Function *F = CurBlock->getParent();
  Value *Val = SI->getCondition();
  BasicBlock* Default = SI->getDefaultDest();

  if ((CurBlock != &F->getEntryBlock() && pred_empty(CurBlock)) ||
      CurBlock->getSinglePredecessor() == CurBlock) {
    DeleteList.insert(CurBlock);
    return;
  }

  if (!SI->getNumCases()) {
    BranchInst::Create(Default, CurBlock);
    SI->eraseFromParent();
    return;
  }

  CaseVector Cases;
  unsigned numCmps = Clusterify(Cases, SI);
  LLVM_DEBUG(dbgs() << "Clusterify finished. Total clusters: " << Cases.size()
                    << ". Total compares: " << numCmps << "\n");
  LLVM_DEBUG(dbgs() << "Cases: " << Cases << "\n");
  (void)numCmps;

  ConstantInt *LowerBound = nullptr;
  ConstantInt *UpperBound = nullptr;
  std::vector<IntRange> UnreachableRanges;

  if (isa<UnreachableInst>(Default->getFirstNonPHIOrDbg())) {
    assert(!Cases.empty());
    LowerBound = Cases.front().Low;
    UpperBound = Cases.back().High;

    DenseMap<BasicBlock *, unsigned> Popularity;
    unsigned MaxPop = 0;
    BasicBlock *PopSucc = nullptr;

    IntRange R = {std::numeric_limits<int64_t>::min(),
                  std::numeric_limits<int64_t>::max()};
    UnreachableRanges.push_back(R);
    for (const auto &I : Cases) {
      int64_t Low = I.Low->getSExtValue();
      int64_t High = I.High->getSExtValue();

      IntRange &LastRange = UnreachableRanges.back();
      if (LastRange.Low == Low) {
        UnreachableRanges.pop_back();
      } else {
        assert(Low > LastRange.Low);
        LastRange.High = Low - 1;
      }
      if (High != std::numeric_limits<int64_t>::max()) {
        UnreachableRanges.push_back(
            {High + 1, std::numeric_limits<int64_t>::max()});
      }

      int64_t N = High - Low + 1;
      unsigned &Pop = Popularity[I.BB];
      if ((Pop += N) > MaxPop) {
        MaxPop = Pop;
        PopSucc = I.BB;
      }
    }
#ifndef NDEBUG
    for (auto I = UnreachableRanges.begin(), E = UnreachableRanges.end();
         I != E; ++I) {
      assert(I->Low <= I->High);
      auto Next = I + 1;
      if (Next != E) {
        assert(Next->Low > I->High);
      }
    }
#endif

    Default->removePredecessor(OrigBlock);

    assert(MaxPop > 0 && PopSucc);
    Default = PopSucc;
    Cases.erase(
        llvm::remove_if(
            Cases, [PopSucc](const CaseRange &R) { return R.BB == PopSucc; }),
        Cases.end());

    if (Cases.empty()) {
      BranchInst::Create(Default, CurBlock);
      SI->eraseFromParent();
      for (unsigned I = 0 ; I < (MaxPop - 1) ; ++I)
        PopSucc->removePredecessor(OrigBlock);
      return;
    }
  }

  unsigned NrOfDefaults = (SI->getDefaultDest() == Default) ? 1 : 0;
  for (const auto &Case : SI->cases())
    if (Case.getCaseSuccessor() == Default)
      NrOfDefaults++;

  BasicBlock *NewDefault = BasicBlock::Create(SI->getContext(), "NewDefault");
  F->insert(Default->getIterator(), NewDefault);
  BranchInst::Create(Default, NewDefault);

  BasicBlock *SwitchBlock =
      switchConvert(Cases.begin(), Cases.end(), LowerBound, UpperBound, Val,
                    OrigBlock, OrigBlock, NewDefault, UnreachableRanges);

  fixPhis(Default, OrigBlock, NewDefault, NrOfDefaults);

  BranchInst::Create(SwitchBlock, OrigBlock);

  BasicBlock *OldDefault = SI->getDefaultDest();
  CurBlock->erase(SI->getIterator(), ++SI->getIterator());

  if (pred_begin(OldDefault) == pred_end(OldDefault))
    DeleteList.insert(OldDefault);
}
