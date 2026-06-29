//===- Utils.cpp - 混淆工具函数-----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现混淆Pass所需的工具函数
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/Utils.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/EHPersonalities.h"
#include "llvm/IR/NoFolder.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/CryptoUtils.h"

#include <random>
#include <algorithm>

/**
 * @brief 检查指令的值是否逃逸到当前基本块之外
 * @param Inst 要检查的指令
 * @return 如果值逃逸返回true，否则返回false
 */
bool valueEscapes(Instruction *Inst) {
  BasicBlock *BB = Inst->getParent();
  for (Value::use_iterator UI = Inst->use_begin(), E = Inst->use_end(); UI != E;
       ++UI) {
    Instruction *I = cast<Instruction>(*UI);
    if (I->getParent() != BB || isa<PHINode>(I)) {
      return true;
    }
  }
  return false;
}

/**
 * @brief 修复函数的栈问题，将寄存器降级到栈
 * @param f 要处理的函数
 */
void fixStack(Function *f) {
  // Try to remove phi node and demote reg to stack
  std::vector<PHINode *>     tmpPhi;
  std::vector<Instruction *> tmpReg;
  BasicBlock *               bbEntry = &*f->begin();

  do {
    tmpPhi.clear();
    tmpReg.clear();

    for (Function::iterator i = f->begin(); i != f->end(); ++i) {

      for (BasicBlock::iterator j = i->begin(); j != i->end(); ++j) {

        if (isa<PHINode>(j)) {
          PHINode *phi = cast<PHINode>(j);
          tmpPhi.push_back(phi);
          continue;
        }
        if (!(isa<AllocaInst>(j) && j->getParent() == bbEntry) &&
            (valueEscapes(&*j) || j->isUsedOutsideOfBlock(&*i))) {
          tmpReg.push_back(&*j);
          continue;
        }
      }
    }
    for (unsigned int i = 0; i != tmpReg.size(); ++i) {
      DemoteRegToStack(*tmpReg.at(i));
    }

    for (unsigned int i = 0; i != tmpPhi.size(); ++i) {
      DemotePHIToStack(tmpPhi.at(i));
    }

  } while (tmpReg.size() != 0 || tmpPhi.size() != 0);
}

/**
 * @brief 修复异常处理相关的调用指令
 * @param CB 要修复的调用基类指针
 * @return 返回修复后的调用指令指针
 */
CallBase* fixEH(CallBase* CB) {
  const auto BB = CB->getParent();
  if (!BB) {
    return CB;
  }
  const auto Fn = BB->getParent();
  if (!Fn || !Fn->hasPersonalityFn()
    || !isScopedEHPersonality(classifyEHPersonality(Fn->getPersonalityFn()))) {
    return CB;
  }
  const auto BlockColors = colorEHFunclets(*Fn);
  const auto BBColor = BlockColors.find(BB);
  if (BBColor == BlockColors.end()) {
    return CB;
  }
  const auto& ColorVec = BBColor->getSecond();
  assert(ColorVec.size() == 1 && "non-unique color for block!");

  const auto EHBlock = ColorVec.front();
  if (!EHBlock || !EHBlock->isEHPad()) {
    return CB;
  }
  const auto EHPad = &*EHBlock->getFirstNonPHIIt();

  const OperandBundleDef OB("funclet", EHPad);
  auto *NewCall = CallBase::addOperandBundle(CB, LLVMContext::OB_funclet, OB, CB->getIterator());
  NewCall->copyMetadata(*CB);
  CB->replaceAllUsesWith(NewCall);
  CB->eraseFromParent();
  return NewCall;
}

/**
 * @brief 降低常量表达式为指令
 * @param F 要处理的函数
 */
void LowerConstantExpr(Function &F) {
  SmallPtrSet<Instruction *, 8> WorkList;

  for (inst_iterator It = inst_begin(F), E = inst_end(F); It != E; ++It) {
    Instruction *I = &*It;

    if (isa<LandingPadInst>(I) || isa<CatchPadInst>(I) || isa<
          CatchSwitchInst>(I) || isa<CatchReturnInst>(I))
      continue;
    if (auto *II = dyn_cast<IntrinsicInst>(I)) {
      if (II->getIntrinsicID() == Intrinsic::eh_typeid_for) {
        continue;
      }
    }

    for (unsigned int i = 0; i < I->getNumOperands(); ++i) {
      if (isa<ConstantExpr>(I->getOperand(i)))
        WorkList.insert(I);
    }
  }

  while (!WorkList.empty()) {
    auto         It = WorkList.begin();
    Instruction *I = *It;
    WorkList.erase(*It);

    if (PHINode *PHI = dyn_cast<PHINode>(I)) {
      for (unsigned int i = 0; i < PHI->getNumIncomingValues(); ++i) {
        Instruction *TI = PHI->getIncomingBlock(i)->getTerminator();
        if (ConstantExpr *CE = dyn_cast<
          ConstantExpr>(PHI->getIncomingValue(i))) {
          Instruction *NewInst = CE->getAsInstruction();
          NewInst->insertBefore(TI->getIterator());
          PHI->setIncomingValue(i, NewInst);
          WorkList.insert(NewInst);
        }
      }
    } else {
      for (unsigned int i = 0; i < I->getNumOperands(); ++i) {
        if (ConstantExpr *CE = dyn_cast<ConstantExpr>(I->getOperand(i))) {
          Instruction *NewInst = CE->getAsInstruction();
          NewInst->insertBefore(I->getIterator());
          I->replaceUsesOfWith(CE, NewInst);
          WorkList.insert(NewInst);
        }
      }
    }
  }
}

/**
 * @brief 展开常量表达式
 * @param F 要处理的函数
 * @return 如果函数被修改返回true，否则返回false
 */
bool expandConstantExpr(Function &F) {
  bool                Changed = false;
  LLVMContext &       Ctx = F.getContext();
  IRBuilder<NoFolder> IRB(Ctx);

  for (auto &BB : F) {
    for (auto &I : BB) {
      if (I.isEHPad() || isa<AllocaInst>(&I) || isa<IntrinsicInst>(&I) ||
        isa<SwitchInst>(&I) || I.isAtomic()) {
        continue;
      }
      auto CI = dyn_cast<CallInst>(&I);
      auto GEP = dyn_cast<GetElementPtrInst>(&I);
      auto IsPhi = isa<PHINode>(&I);
      auto InsertPt = IsPhi
        ? F.getEntryBlock().getFirstInsertionPt()
        : I.getIterator();
      for (unsigned i = 0; i < I.getNumOperands(); ++i) {
        if (CI && CI->isBundleOperand(i)) {
          continue;
        }
        if (GEP && (i < 2 || GEP->getSourceElementType()->isStructTy())) {
          continue;
        }
        auto Opr = I.getOperand(i);
        if (auto CEP = dyn_cast<ConstantExpr>(Opr)) {
          IRB.SetInsertPoint(InsertPt);
          auto CEPInst = CEP->getAsInstruction();
          IRB.Insert(CEPInst);
          I.setOperand(i, CEPInst);
          Changed = true;
        }
      }
    }
  }
  return Changed;
}

/**
 * @brief 对索引应用掩码混淆
 * @param mask 掩码值
 * @param preIndex 预处理索引
 * @param objKey 对象密钥
 * @param newIndex 新索引
 */
void maskCipher(uint8_t mask, APInt &preIndex, unsigned objKey, unsigned newIndex) {
  switch (mask) {
  case 1:
    preIndex = -preIndex;
    break;
  case 2:
    preIndex = preIndex.rotl(objKey + newIndex);
    break;
  case 3:
    preIndex = preIndex.byteSwap();
    break;
  case 4:
    preIndex = ~preIndex;
    break;
  case 5:
    preIndex = preIndex.rotr(objKey - newIndex);
    break;
  default:
    preIndex = preIndex ^ objKey;
    break;
  }
}

/**
 * @brief 创建页表用于间接访问混淆
 * @param args 创建页表的参数
 */
void createPageTable(const CreatePageTableArgs &args) {
  auto& Ctx = args.M->getContext();
  const auto Int32Ty = IntegerType::getInt32Ty(Ctx);
  std::mt19937_64 re(args.RandomEngine->get_uint64_t());
  std::shuffle(args.Objects->begin(), args.Objects->end(), re);

  std::vector<Constant *> GVObjects;
  for (unsigned i = 0; i < args.Objects->size(); ++i) {
    auto Obj = args.Objects->at(i);
    GVObjects.push_back(ConstantExpr::getBitCast(Obj, PointerType::getUnqual(Ctx)));
    args.IndexMap->insert_or_assign(Obj, i);
  }

  {
    auto GVNameObjects(args.GVNamePrefix + "_objects");
    auto ATy = ArrayType::get(GVObjects[0]->getType(), GVObjects.size());
    auto CA = ConstantArray::get(ATy, ArrayRef(GVObjects));
    auto GV = new GlobalVariable(*args.M, ATy, false,
                                 GlobalValue::LinkageTypes::InternalLinkage,
                                 CA, GVNameObjects);
    GV->setSection(".AProtect.data");
    GV->addMetadata("noobf", *MDNode::get(args.M->getContext(), {}));
    args.OutPageTable->push_back(GV);
  }

  for (unsigned i = 0; i < args.CountLoop; ++i) {
    std::shuffle(args.Objects->begin(), args.Objects->end(), re);

    std::vector<Constant *> ConstantObjectIndex;
    for (unsigned j = 0; j < args.Objects->size(); ++j) {
      const auto Obj = args.Objects->at(j);
      const auto ObjFullKey = args.ObjectKeys->at(Obj);
      const auto ObjKey = static_cast<uint32_t>(ObjFullKey);
      const auto ObjMask = static_cast<uint32_t>(ObjFullKey >> 32);

      APInt preIndex(32, args.IndexMap->at(Obj));
      for (unsigned k = 0; k < 8; ++k) {
        const auto mask = static_cast<uint8_t>(ObjMask >> (k * 3)) % 6u;
        maskCipher(mask, preIndex, ObjKey, j);
      }
      auto toWriteData = ConstantInt::get(Int32Ty, preIndex);
      ConstantObjectIndex.push_back(toWriteData);
      args.IndexMap->insert_or_assign(Obj, j);
    }

    {

      auto GVNameObjPageTable(args.GVNamePrefix + "_page_table_" + std::to_string(i));
      auto IATy = ArrayType::get(Int32Ty, ConstantObjectIndex.size());
      auto IA = ConstantArray::get(IATy, ArrayRef(ConstantObjectIndex));
      auto GV = new GlobalVariable(*args.M, IATy, false,
                                   GlobalValue::LinkageTypes::InternalLinkage,
                                   IA, GVNameObjPageTable);
      GV->setSection(".AProtect.data");
      GV->addMetadata("noobf", *MDNode::get(args.M->getContext(), {}));
      args.OutPageTable->push_back(GV);
    }
  }

}

/**
 * @brief 创建增强页表用于间接访问混淆
 * @param args 创建页表的参数
 * @param FuncIndexMap 函数索引映射
 */
void enhancedPageTable(const CreatePageTableArgs &args, std::unordered_map<Constant *, unsigned> *FuncIndexMap) {
  const auto Int32Ty = IntegerType::getInt32Ty(args.M->getContext());
  std::mt19937_64 re(args.RandomEngine->get_uint64_t());

  for (unsigned i = 0; i < args.CountLoop; ++i) {
    std::shuffle(args.Objects->begin(), args.Objects->end(), re);
    std::vector<Constant *> ConstantObjectIndex;
    for (unsigned j = 0; j < args.Objects->size(); ++j) {
      auto Obj = args.Objects->at(j);
      const auto ObjFullKey = args.ObjectKeys->at(Obj);
      const auto ObjKey = static_cast<uint32_t>(ObjFullKey);
      const auto ObjMask = static_cast<uint32_t>(ObjFullKey >> 32);

      APInt preIndex(32, FuncIndexMap->find(Obj) == FuncIndexMap->end() ?
                           args.IndexMap->at(Obj) :
                           FuncIndexMap->at(Obj));

      for (unsigned k = 0; k < 4 * args.CountLoop; ++k) {
        const auto mask = static_cast<uint8_t>(ObjMask >> (k * 2)) % 6u;
        maskCipher(mask, preIndex, ObjKey, j);
      }
      auto toWriteData = ConstantInt::get(Int32Ty, preIndex);
      ConstantObjectIndex.push_back(toWriteData);
      FuncIndexMap->insert_or_assign(Obj, j);
    }

    {
      auto GVNameObjPage(args.GVNamePrefix + "_enhanced_page_table_" + std::to_string(i));
      auto IATy = ArrayType::get(Int32Ty, ConstantObjectIndex.size());
      auto IA = ConstantArray::get(IATy, ArrayRef(ConstantObjectIndex));
      auto GV = new GlobalVariable(*args.M, IATy, false, GlobalValue::LinkageTypes::PrivateLinkage,
        IA, GVNameObjPage);
      GV->setSection(".AProtect.data");
      GV->addMetadata("noobf", *MDNode::get(args.M->getContext(), {}));
      args.OutPageTable->push_back(GV);
    }
  }
}

/**
 * @brief 构建页表解密的IR代码
 * @param args 构建解密IR的参数
 * @return 返回解密后的值
 */
Value * buildPageTableDecryptIR(const BuildDecryptArgs &args) {
  auto M = args.Fn->getParent();
  auto& Ctx = args.Fn->getContext();
  auto Int32Ty = IntegerType::getInt32Ty(Ctx);
  auto Zero = ConstantInt::getNullValue(Int32Ty);
  const auto ModuleKey = static_cast<uint32_t>(args.ModuleKey);
  const auto ModuleMask = static_cast<uint32_t>(args.ModuleKey >> 32);
  const auto FuncKey = static_cast<uint32_t>(args.FuncKey);
  const auto FuncMask = static_cast<uint32_t>(args.FuncKey >> 32);
  IRBuilder<> IRB{args.InsertBefore};

  Value *NextIndex = args.NextIndexValue;
  if (!NextIndex) {
    auto GVInitIndex = new GlobalVariable(*M, Int32Ty, false, GlobalValue::PrivateLinkage,
      ConstantInt::get(Int32Ty, args.NextIndex),
      M->getName() + args.Fn->getName() + "_InitIndex" +
      std::to_string(args.NextIndex));
    GVInitIndex->setSection(".AProtect.data");
    GVInitIndex->addMetadata("noobf", *MDNode::get(Ctx, {}));
    NextIndex = IRB.CreateAlignedLoad(Int32Ty, GVInitIndex, Align{1}, true);
  }

  auto createDecIndexSwitch = [&IRB, &M](uint8_t mask, Value *NextIndex, Value *PrevIndex, Value* ObjKey) -> Value* {

    switch (mask) {
    case 1:
      NextIndex = IRB.CreateNeg(NextIndex);
      break;
    case 2:
      NextIndex = IRB.CreateCall(
        Intrinsic::getOrInsertDeclaration(M, Intrinsic::fshr, {NextIndex->getType()}),
        {NextIndex, NextIndex, IRB.CreateAdd(ObjKey, PrevIndex)});
      break;
    case 3:
      NextIndex = IRB.CreateCall(
        Intrinsic::getOrInsertDeclaration(M, Intrinsic::bswap, {NextIndex->getType()}),
        {NextIndex});
      break;
    case 4:
      NextIndex = IRB.CreateNot(NextIndex);
      break;
    case 5:
      NextIndex = IRB.CreateCall(
        Intrinsic::getOrInsertDeclaration(M, Intrinsic::fshl, {NextIndex->getType()}),
        {NextIndex, NextIndex, IRB.CreateSub(ObjKey, PrevIndex)});
      break;
    default:
      NextIndex = IRB.CreateXor(NextIndex, ObjKey);
      break;
    }
    return NextIndex;
  };

  if (args.FuncLoopCount && !args.FuncPageTable->empty()) {
    auto ConstantFuncKey = ConstantInt::get(Int32Ty, FuncKey);

    for (int i = args.FuncPageTable->size() - 1; i >= 0; --i) {
      auto TargetPage = args.FuncPageTable->at(i);
      auto PrevIndex = NextIndex;
      Value *GEP = IRB.CreateGEP(
          TargetPage->getValueType(), TargetPage,
          {Zero, NextIndex});
      NextIndex = IRB.CreateLoad(Int32Ty, GEP);
      std::vector<uint8_t> maskIndex;
      for (unsigned j = 0; j < 4 * args.FuncLoopCount; ++j) {
        auto mask = static_cast<uint8_t>(FuncMask >> (j * 2)) % 6u;
        maskIndex.push_back(mask);
      }
      for (int j = maskIndex.size() - 1; j >= 0; --j) {
        NextIndex = createDecIndexSwitch(maskIndex.at(j), NextIndex, PrevIndex, ConstantFuncKey);
      }
    }
  }

  auto ConstantModuleKey = ConstantInt::get(Int32Ty, ModuleKey);

  for (int i = args.ModulePageTable->size() - 1; i >= 0; --i) {
    auto TargetPage = args.ModulePageTable->at(i);
    auto PrevIndex = NextIndex;
    Value *GEP = IRB.CreateGEP(
      TargetPage->getValueType(), TargetPage,
      {Zero, NextIndex});
    if (i) {
      NextIndex = IRB.CreateLoad(Int32Ty, GEP);
      std::vector<uint8_t> maskIndex;
      for (unsigned j = 0; j < 8; ++j) {
        auto mask = static_cast<uint8_t>(ModuleMask >> (j * 3)) % 6u;
        maskIndex.push_back(mask);
      }
      for (int j = maskIndex.size() - 1; j >= 0; --j) {
        NextIndex = createDecIndexSwitch(maskIndex.at(j), NextIndex, PrevIndex, ConstantModuleKey);
      }
      continue;
    }
    return IRB.CreateLoad(args.LoadTy, GEP);
  }
  llvm_unreachable("BuildDecryptIR unreachable!!!");
}

/**
 * @brief 加密常量
 * @param plainConstant 要加密的常量
 * @param insertBefore 插入点指令
 * @param randomEngine 随机数生成器
 * @param level 加密级别
 * @return 返回加密后的值
 */
Value * encryptConstant(Constant *plainConstant, Instruction *insertBefore, CryptoUtils *randomEngine, unsigned level) {
  auto& Ctx = insertBefore->getContext();
  auto OriginValTy = plainConstant->getType();
  if (OriginValTy->isStructTy() || OriginValTy->isArrayTy() || OriginValTy->isPointerTy()) {
    return plainConstant;
  }
  auto BitWidth = plainConstant->getType()->getPrimitiveSizeInBits().getFixedValue();
  if (BitWidth < 8) {
    return plainConstant;
  }
  
  const auto Key = ConstantInt::get(
      IntegerType::get(Ctx, BitWidth),
      randomEngine->get_uint64_t());

  const auto ConstantInt = ConstantExpr::getBitCast(plainConstant, Key->getType());
  auto Enc = ConstantExpr::getSub(ConstantInt, Key);
  Constant *XorKey = nullptr;
  if (level) {
    XorKey = ConstantInt::get(Key->getType(), randomEngine->get_uint64_t());
    Enc = ConstantExpr::getXor(Enc, XorKey);
    if (level > 1) {
      Enc = ConstantExpr::getXor(Enc, ConstantExpr::get(Instruction::Mul, XorKey, Key));
    }
    if (level > 2) {
      Enc = ConstantExpr::getXor(Enc, ConstantExpr::getNeg(XorKey));
    }
  }
  auto EncGV = new GlobalVariable(*insertBefore->getModule(), Enc->getType(), false,
                                  GlobalValue::InternalLinkage, Enc);
  EncGV->setSection(".AProtect.data");
  EncGV->addMetadata("noobf", *MDNode::get(Ctx, {}));
  IRBuilder<NoFolder> IRB(insertBefore);
  Value *Load = IRB.CreateAlignedLoad(Enc->getType(), EncGV, Align{1}, true);
  if (level) {
    if (level > 2) {
      Load = IRB.CreateXor(Load, IRB.CreateNeg(XorKey));
    }
    if (level > 1) {
      Load = IRB.CreateXor(Load, IRB.CreateMul(XorKey, Key));
    }
    Load = IRB.CreateXor(Load, XorKey);
  }
  Load = IRB.CreateAdd(Load, Key);
  return IRB.CreateBitCast(Load, OriginValTy);
}
