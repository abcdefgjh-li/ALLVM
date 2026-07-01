//===- AArch64OpaquePredicate.cpp - Insert opaque dead branches -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This late pass inserts a never-taken conditional branch and a dead block
// containing deliberately misaligned byte data. The goal is to perturb linear
// sweep disassemblers without affecting runtime behavior.
//
//===----------------------------------------------------------------------===//

#include "AArch64.h"
#include "AArch64InstrInfo.h"
#include "AArch64Subtarget.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;

#define AARCH64_OPAQUE_PREDICATE_NAME "AArch64 opaque predicate obfuscation"

static cl::opt<bool> EnableAArch64ObfuscateOpaquePredicate(
    "aarch64-obfuscate-opaque-predicate", cl::Hidden,
    cl::desc("Insert never-taken branches and misaligned dead bytes"),
    cl::init(false));

namespace {

class AArch64OpaquePredicate : public MachineFunctionPass {
public:
  static char ID;

  AArch64OpaquePredicate() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override {
    return AARCH64_OPAQUE_PREDICATE_NAME;
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

private:
  const AArch64InstrInfo *TII = nullptr;

  static Register findPredicateRegister(const MachineBasicBlock &MBB);
  static MachineBasicBlock::instr_iterator
  findSplitPoint(MachineBasicBlock &MBB);
};

} // end anonymous namespace

char AArch64OpaquePredicate::ID = 0;

INITIALIZE_PASS(AArch64OpaquePredicate, "aarch64-opaque-predicate-obfuscation",
                AARCH64_OPAQUE_PREDICATE_NAME, false, false)

FunctionPass *llvm::createAArch64OpaquePredicatePass() {
  return new AArch64OpaquePredicate();
}

Register
AArch64OpaquePredicate::findPredicateRegister(const MachineBasicBlock &MBB) {
  static constexpr MCPhysReg Candidates[] = {
      AArch64::X9,  AArch64::X10, AArch64::X11, AArch64::X12,
      AArch64::X13, AArch64::X14, AArch64::X15};
  for (MCPhysReg Reg : Candidates) {
    if (!MBB.isLiveIn(Reg))
      return Reg;
  }
  return AArch64::NoRegister;
}

MachineBasicBlock::instr_iterator
AArch64OpaquePredicate::findSplitPoint(MachineBasicBlock &MBB) {
  auto It = MBB.instr_begin();
  auto End = MBB.instr_end();
  for (; It != End; ++It) {
    if (It->isDebugInstr())
      continue;
    if (It->isCFIInstruction() || It->getFlag(MachineInstr::FrameSetup))
      return End;
    return It;
  }
  return End;
}

bool AArch64OpaquePredicate::runOnMachineFunction(MachineFunction &MF) {
  if (!EnableAArch64ObfuscateOpaquePredicate || MF.empty())
    return false;

  MachineBasicBlock &Entry = MF.front();
  if (Entry.isEHPad() || Entry.hasAddressTaken())
    return false;

  MachineBasicBlock::instr_iterator SplitIt = findSplitPoint(Entry);
  if (SplitIt == Entry.instr_end())
    return false;

  Register PredicateReg = findPredicateRegister(Entry);
  if (PredicateReg == AArch64::NoRegister)
    return false;

  TII = MF.getSubtarget<AArch64Subtarget>().getInstrInfo();
  DebugLoc DL = SplitIt->getDebugLoc();

  auto InsertIt = std::next(Entry.getIterator());
  MachineBasicBlock *DeadBB = MF.CreateMachineBasicBlock(Entry.getBasicBlock());
  MF.insert(InsertIt, DeadBB);
  MachineBasicBlock *ContBB = MF.CreateMachineBasicBlock(Entry.getBasicBlock());
  MF.insert(std::next(DeadBB->getIterator()), ContBB);

  for (const MachineBasicBlock::RegisterMaskPair &LI : Entry.liveins())
    ContBB->addLiveIn(LI.PhysReg, LI.LaneMask);

  ContBB->splice(ContBB->end(), &Entry, SplitIt, Entry.end());
  ContBB->transferSuccessorsAndUpdatePHIs(&Entry);

  BuildMI(&Entry, DL, TII->get(AArch64::ORRXrs), PredicateReg)
      .addUse(AArch64::XZR)
      .addUse(AArch64::XZR)
      .addImm(0);
  BuildMI(&Entry, DL, TII->get(AArch64::CBNZX))
      .addReg(PredicateReg)
      .addMBB(DeadBB);
  BuildMI(&Entry, DL, TII->get(AArch64::B)).addMBB(ContBB);

  BuildMI(*DeadBB, DeadBB->end(), DL, TII->get(AArch64::HINT)).addImm(127);
  BuildMI(*DeadBB, DeadBB->end(), DL, TII->get(AArch64::B)).addMBB(ContBB);

  Entry.addSuccessor(DeadBB);
  Entry.addSuccessor(ContBB);
  DeadBB->addSuccessor(ContBB);
  return true;
}
