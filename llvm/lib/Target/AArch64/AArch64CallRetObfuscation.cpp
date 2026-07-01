//===- AArch64CallRetObfuscation.cpp - Obfuscate call/ret patterns --------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This late pass rewrites direct BL calls into an ADRP/ADD/BLR sequence and
// rewrites RET into BR LR. The intent is to break the canonical call/return
// patterns used by disassemblers to recover function boundaries.
//
//===----------------------------------------------------------------------===//

#include "AArch64.h"
#include "AArch64InstrInfo.h"
#include "AArch64MachineFunctionInfo.h"
#include "AArch64Subtarget.h"
#include "Utils/AArch64BaseInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/Attributes.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;

#define AARCH64_CALL_RET_OBFUSCATION_NAME "AArch64 call/ret obfuscation"

static cl::opt<bool> EnableAArch64ObfuscateCallRet(
    "aarch64-obfuscate-call-ret", cl::Hidden,
    cl::desc("Rewrite direct BL/RET into indirect call/branch sequences"),
    cl::init(false));

namespace {

class AArch64CallRetObfuscation : public MachineFunctionPass {
public:
  static char ID;

  AArch64CallRetObfuscation() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override {
    return AARCH64_CALL_RET_OBFUSCATION_NAME;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

private:
  const AArch64InstrInfo *TII = nullptr;

  bool shouldRewriteReturns(const MachineFunction &MF) const;
  Register findScratchRegister(MachineBasicBlock &MBB,
                               MachineBasicBlock::instr_iterator MBBI) const;
  bool rewriteDirectCall(MachineFunction &MF,
                         MachineBasicBlock::instr_iterator MBBI) const;
  bool rewriteReturn(MachineFunction &MF,
                     MachineBasicBlock::instr_iterator MBBI) const;
};

} // end anonymous namespace

char AArch64CallRetObfuscation::ID = 0;

INITIALIZE_PASS(AArch64CallRetObfuscation, "aarch64-call-ret-obfuscation",
                AARCH64_CALL_RET_OBFUSCATION_NAME, false, false)

FunctionPass *llvm::createAArch64CallRetObfuscationPass() {
  return new AArch64CallRetObfuscation();
}

bool AArch64CallRetObfuscation::shouldRewriteReturns(
    const MachineFunction &MF) const {
  const auto *AFI = MF.getInfo<AArch64FunctionInfo>();
  return !AFI->branchProtectionPAuthLR() &&
         !MF.getFunction().hasFnAttribute(Attribute::ShadowCallStack);
}

Register AArch64CallRetObfuscation::findScratchRegister(
    MachineBasicBlock &MBB, MachineBasicBlock::instr_iterator MBBI) const {
  RegScavenger RS;
  RS.enterBasicBlockEnd(MBB);
  if (MBBI == MBB.instr_begin())
    RS.enterBasicBlock(MBB);
  else
    RS.backward(MBBI);

  Register ScratchReg = RS.FindUnusedReg(&AArch64::GPR64commonRegClass);
  if (ScratchReg == AArch64::NoRegister)
    return AArch64::NoRegister;

  if (ScratchReg == AArch64::LR || ScratchReg == AArch64::FP)
    return AArch64::NoRegister;
  return ScratchReg;
}

bool AArch64CallRetObfuscation::rewriteDirectCall(
    MachineFunction &MF, MachineBasicBlock::instr_iterator MBBI) const {
  MachineInstr &Call = *MBBI;
  if (Call.getOpcode() != AArch64::BL || Call.getNumExplicitOperands() != 1)
    return false;

  const MachineOperand &Target = Call.getOperand(0);
  DebugLoc DL = Call.getDebugLoc();
  Register ScratchReg = findScratchRegister(*Call.getParent(), MBBI);
  if (ScratchReg == AArch64::NoRegister)
    return false;

  unsigned TargetFlags = Target.getTargetFlags();
  if (Target.isGlobal()) {
    BuildMI(*Call.getParent(), MBBI, DL, TII->get(AArch64::ADRP), ScratchReg)
        .addGlobalAddress(Target.getGlobal(), Target.getOffset(),
                          TargetFlags | AArch64II::MO_PAGE);
    BuildMI(*Call.getParent(), MBBI, DL, TII->get(AArch64::ADDXri), ScratchReg)
        .addReg(ScratchReg)
        .addGlobalAddress(Target.getGlobal(), Target.getOffset(),
                          TargetFlags | AArch64II::MO_PAGEOFF |
                              AArch64II::MO_NC)
        .addImm(0);
  } else if (Target.isSymbol()) {
    BuildMI(*Call.getParent(), MBBI, DL, TII->get(AArch64::ADRP), ScratchReg)
        .addExternalSymbol(Target.getSymbolName(),
                           TargetFlags | AArch64II::MO_PAGE);
    BuildMI(*Call.getParent(), MBBI, DL, TII->get(AArch64::ADDXri), ScratchReg)
        .addReg(ScratchReg)
        .addExternalSymbol(Target.getSymbolName(),
                           TargetFlags | AArch64II::MO_PAGEOFF |
                               AArch64II::MO_NC)
        .addImm(0);
  } else if (Target.isMCSymbol()) {
    BuildMI(*Call.getParent(), MBBI, DL, TII->get(AArch64::ADRP), ScratchReg)
        .addSym(Target.getMCSymbol(), TargetFlags | AArch64II::MO_PAGE);
    BuildMI(*Call.getParent(), MBBI, DL, TII->get(AArch64::ADDXri), ScratchReg)
        .addReg(ScratchReg)
        .addSym(Target.getMCSymbol(), TargetFlags | AArch64II::MO_PAGEOFF |
                                         AArch64II::MO_NC)
        .addImm(0);
  } else {
    return false;
  }

  MachineInstr *IndirectCall =
      BuildMI(*Call.getParent(), MBBI, DL, TII->get(getBLRCallOpcode(MF)))
          .addReg(ScratchReg)
          .getInstr();

  // Drop the default SP/LR implicit operands from BLR and inherit the exact
  // call-side metadata from the original BL.
  while (IndirectCall->getNumOperands() > 1)
    IndirectCall->removeOperand(IndirectCall->getNumOperands() - 1);

  IndirectCall->copyImplicitOps(MF, Call);
  IndirectCall->setFlags(Call.getFlags());
  IndirectCall->cloneMemRefs(MF, Call);
  MF.eraseAdditionalCallInfo(&Call);
  Call.eraseFromParent();
  return true;
}

bool AArch64CallRetObfuscation::rewriteReturn(
    MachineFunction &MF, MachineBasicBlock::instr_iterator MBBI) const {
  MachineInstr &Ret = *MBBI;
  if (Ret.getOpcode() != AArch64::RET && Ret.getOpcode() != AArch64::RET_ReallyLR)
    return false;

  unsigned RegState = RegState::Undef;
  Register TargetReg = AArch64::LR;
  if (Ret.getOpcode() == AArch64::RET && Ret.getNumExplicitOperands() != 0) {
    const MachineOperand &RetOp = Ret.getOperand(0);
    if (!RetOp.isReg())
      return false;
    TargetReg = RetOp.getReg();
    RegState = getRegState(RetOp);
  }

  MachineInstr *Branch =
      BuildMI(*Ret.getParent(), MBBI, Ret.getDebugLoc(), TII->get(AArch64::BR))
          .addReg(TargetReg, RegState)
          .getInstr();
  Branch->copyImplicitOps(MF, Ret);
  Branch->setFlags(Ret.getFlags());
  Branch->cloneMemRefs(MF, Ret);
  Ret.eraseFromParent();
  return true;
}

bool AArch64CallRetObfuscation::runOnMachineFunction(MachineFunction &MF) {
  if (!EnableAArch64ObfuscateCallRet)
    return false;

  TII = MF.getSubtarget<AArch64Subtarget>().getInstrInfo();
  bool Modified = false;
  bool RewriteReturns = shouldRewriteReturns(MF);

  for (MachineBasicBlock &MBB : MF) {
    for (MachineBasicBlock::instr_iterator MBBI = MBB.instr_begin(),
                                           E = MBB.instr_end();
         MBBI != E;) {
      MachineBasicBlock::instr_iterator Next = std::next(MBBI);
      switch (MBBI->getOpcode()) {
      case AArch64::BL:
        Modified |= rewriteDirectCall(MF, MBBI);
        break;
      case AArch64::RET:
      case AArch64::RET_ReallyLR:
        if (RewriteReturns)
          Modified |= rewriteReturn(MF, MBBI);
        break;
      default:
        break;
      }
      MBBI = Next;
    }
  }

  return Modified;
}
