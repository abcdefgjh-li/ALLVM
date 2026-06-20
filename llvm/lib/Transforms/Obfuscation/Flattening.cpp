//===- Flattening.cpp - 控制流扁平化混淆Pass----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现控制流扁平化混淆Pass，通过将函数的基本块重组为状态机形式
// 来混淆控制流，增加逆向分析的难度
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/Constants.h"
#include "llvm/Transforms/Obfuscation/Flattening.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/Transforms/Obfuscation/LegacyLowerSwitch.h"
#include "llvm/Transforms/Obfuscation/Utils.h"
#include "llvm/CryptoUtils.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Transforms/Obfuscation/ObfuscationOptions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"

#include <array>
#include <functional>
#include <memory>
#include <random>

#define DEBUG_TYPE "flattening"

using namespace std;
using namespace llvm;

// 统计信息
STATISTIC(Flattened, "已扁平化的函数数量");

namespace {
	struct Flattening : public FunctionPass {
		unsigned    pointerSize;
		static char ID;

		ObfuscationOptions *ArgsOptions;
		CryptoUtils         RandomEngine;

		/**
		 * @brief 构造函数，初始化控制流扁平化Pass
		 * @param pointerSize 指针大小（32位或64位）
		 * @param argsOptions 混淆选项配置对象
		 */
		Flattening(unsigned            pointerSize,
		           ObfuscationOptions *argsOptions) : FunctionPass(ID) {
			this->pointerSize = pointerSize;
			this->ArgsOptions = argsOptions;
		}

		/**
		 * @brief 对单个函数执行控制流扁平化
		 * @param F 要处理的函数
		 * @return 如果函数被修改返回true，否则返回false
		 */
		bool runOnFunction(Function &F) override;
		/**
		 * @brief 执行控制流扁平化的核心逻辑
		 * @param f 要处理的函数指针
		 * @param opt 混淆选项
		 * @return 如果扁平化成功返回true，否则返回false
		 */
		bool flatten(Function *f, const ObfOpt &opt);
		GlobalVariable *getOrCreateCallerSeedTLS(Module &M, Type *IntTy);
		bool instrumentCallerSeedStores(Function &F, Type *IntTy);
	};
}

GlobalVariable *Flattening::getOrCreateCallerSeedTLS(Module &M, Type *IntTy) {
	if (auto *Existing = M.getNamedGlobal("__fla_callsite_seed")) {
		return Existing;
	}

	auto *TLS = new GlobalVariable(
	    M, IntTy, false, GlobalValue::InternalLinkage, ConstantInt::get(IntTy, 0),
	    "__fla_callsite_seed");
	TLS->setThreadLocalMode(GlobalVariable::GeneralDynamicTLSModel);
	TLS->setAlignment(Align(pointerSize == 8 ? 8 : 4));
	return TLS;
}

bool Flattening::instrumentCallerSeedStores(Function &F, Type *IntTy) {
	if (F.isDeclaration() || F.isIntrinsic()) {
		return false;
	}

	Instruction *EntryInsertPoint = nullptr;
	for (BasicBlock &BB : F) {
		if (BB.empty()) {
			continue;
		}
		EntryInsertPoint = &*BB.getFirstInsertionPt();
		break;
	}
	if (!EntryInsertPoint) {
		return false;
	}

	IRBuilder<> EntryIRB(EntryInsertPoint);
	auto *CallerAnchor = EntryIRB.CreateAlloca(IntTy, nullptr, "flaCallerAnchor");
	auto *CallerSeedTLS = getOrCreateCallerSeedTLS(*F.getParent(), IntTy);
	bool Changed = false;

	std::function<Value *(IRBuilder<> &, Value *)> normalizeForSeed =
	    [&](IRBuilder<> &IRB, Value *V) -> Value * {
		if (!V) {
			return ConstantInt::get(IntTy, 0);
		}
		Type *VTy = V->getType();
		if (VTy == IntTy) {
			return V;
		}
		if (VTy->isIntegerTy()) {
			unsigned SrcBits = cast<IntegerType>(VTy)->getBitWidth();
			unsigned DstBits = cast<IntegerType>(IntTy)->getBitWidth();
			if (SrcBits < DstBits) {
				return IRB.CreateZExt(V, IntTy, "seedZext");
			}
			if (SrcBits > DstBits) {
				return IRB.CreateTrunc(V, IntTy, "seedTrunc");
			}
			return V;
		}
		if (VTy->isPointerTy()) {
			return IRB.CreatePtrToInt(V, IntTy, "seedPtr");
		}
		if (VTy->isFloatingPointTy()) {
			Type *FloatIntTy = VTy->isFloatTy() ? Type::getInt32Ty(F.getContext())
			                                    : Type::getInt64Ty(F.getContext());
			Value *Bits = IRB.CreateBitCast(V, FloatIntTy, "seedFloatBits");
			return normalizeForSeed(IRB, Bits);
		}
		return ConstantInt::get(IntTy, 0);
	};

	for (BasicBlock &BB : F) {
		for (Instruction &Inst : BB) {
			auto *CI = dyn_cast<CallInst>(&Inst);
			if (!CI || CI->isInlineAsm() || CI->isMustTailCall()) {
				continue;
			}
			if (Function *Callee = CI->getCalledFunction()) {
				if (Callee->isIntrinsic()) {
					continue;
				}
			}

			IRBuilder<> IRB(CI);
			Value *Seed = IRB.CreatePtrToInt(CallerAnchor, IntTy, "callerSeedBase");
			Seed = IRB.CreateXor(Seed,
			                     ConstantInt::get(IntTy, RandomEngine.get_uint64_t()),
			                     "callerSeedSite");
			Seed = IRB.CreateAdd(Seed,
			                     normalizeForSeed(IRB, CI->getCalledOperand()),
			                     "callerSeedCallee",
			                     true,
			                     true);

			const unsigned ArgCount = std::min<unsigned>(CI->arg_size(), 3);
			for (unsigned ArgIndex = 0; ArgIndex < ArgCount; ++ArgIndex) {
				Value *ArgSeed = normalizeForSeed(IRB, CI->getArgOperand(ArgIndex));
				Seed = IRB.CreateXor(Seed, ArgSeed, "callerSeedArg");
				Seed = IRB.CreateAdd(
				    Seed,
				    ConstantInt::get(IntTy, RandomEngine.get_uint64_t() | 1ULL),
				    "callerSeedMix",
				    true,
				    true);
			}

			IRB.CreateStore(Seed, CallerSeedTLS, true);
			Changed = true;
		}
	}

	return Changed;
}

/**
 * @brief 对单个函数执行控制流扁平化
 * @param F 要处理的函数
 * @return 如果函数被修改返回true，否则返回false
 */
bool Flattening::runOnFunction(Function &F) {
	if (isIRObfuscationDebugEnabled()) {
		errs() << "[DEBUG] Flattening: Starting runOnFunction: " << F.getName() << "\n";
	}
	if (F.isIntrinsic()) {
		return false;
	}
	Function *tmp = &F;
	bool      result = false;
	const auto opt = ArgsOptions->toObfuscate(ArgsOptions->flaOpt(), &F);
	if (!opt.isEnabled()) {
		return result;
	}
	auto *IntTy = pointerSize == 8 ? Type::getInt64Ty(F.getContext())
	                               : Type::getInt32Ty(F.getContext());
	bool CallerSeedChanged = instrumentCallerSeedStores(F, IntTy);
	if (flatten(tmp, opt)) {
		++Flattened;
		result = true;
	}
	result = result || CallerSeedChanged;

	return result;
}

/**
 * @brief 执行控制流扁平化的核心逻辑
 * @param f 要处理的函数指针
 * @param opt 混淆选项
 * @return 如果扁平化成功返回true，否则返回false
 */
bool Flattening::flatten(Function *f, const ObfOpt &opt) {
	vector<BasicBlock *> origBB;
	auto dumpBlocksWithoutTerminator = [&](const char *Stage) {
		if (!isIRObfuscationDebugEnabled()) {
			return;
		}
		for (BasicBlock &BB : *f) {
			if (BB.getTerminator()) {
				continue;
			}
			errs() << "[DEBUG][FLA][D] missing terminator stage=" << Stage
			       << " func=" << f->getName() << " bb=";
			BB.printAsOperand(errs(), false);
			errs() << "\n";
			BB.print(errs());
		}
	};

	auto &Ctx = f->getContext();
	auto  intType = Type::getInt32Ty(Ctx);

	if (pointerSize == 8) {
		intType = Type::getInt64Ty(Ctx);
	}

	char scrambling_key[16];
	cryptoutils->get_bytes(scrambling_key, 16);

	auto lower = std::unique_ptr<FunctionPass>(createLegacyLowerSwitchPass());
	lower->runOnFunction(*f);

	// #region debug-point A:entry-cfg-scan
	if (isIRObfuscationDebugEnabled()) {
		errs() << "[DEBUG][FLA][A] enter flatten func=" << f->getName()
		       << " ptrSize=" << pointerSize
		       << " blocks-before-scan=" << f->size() << "\n";
	}
	// #endregion

	for (auto i = f->begin(); i != f->end(); ++i) {
		auto bb = &*i;
		origBB.push_back(bb);

		if (isa<InvokeInst>(bb->getTerminator()) || bb->isEHPad()) {
			return false;
		}
	}

	if (origBB.size() <= 1) {
		return false;
	}

	origBB.erase(origBB.begin());

	auto insertBlock = &*(f->begin());

	auto splitPos = --(insertBlock->end());

	if (insertBlock->size() > 1) {
		--splitPos;
	}

	std::mt19937_64 re(RandomEngine.get_uint64_t());
	std::shuffle(origBB.begin(), origBB.end(), re);

	auto bbEndOfEntry = insertBlock->splitBasicBlock(splitPos, "first");
	origBB.insert(origBB.begin(), bbEndOfEntry);

	insertBlock->getTerminator()->eraseFromParent();

	// #region debug-point A:after-entry-split
	if (isIRObfuscationDebugEnabled()) {
		errs() << "[DEBUG][FLA][A] after split func=" << f->getName()
		       << " insertBlock=" << insertBlock->getName()
		       << " firstSplit=" << bbEndOfEntry->getName()
		       << " origBB-size=" << origBB.size() << "\n";
	}
	// #endregion

	IRBuilder<> IRB{insertBlock};
	const auto  switchVar = IRB.CreateAlloca(intType, nullptr, "switchVar");
	const auto  switchXorVar = IRB.CreateAlloca(intType, nullptr, "switchXor");
	const auto  switchAliasVar = IRB.CreateAlloca(intType, nullptr, "switchAlias");
	const auto  switchAliasXorVar = IRB.CreateAlloca(intType, nullptr, "switchAliasXor");
	const auto  switchCallerSeedVar = IRB.CreateAlloca(intType, nullptr, "switchCallerSeed");
	const auto  switchCallerAliasSeedVar = IRB.CreateAlloca(intType, nullptr, "switchCallerAliasSeed");
	auto *callerSeedTLS = getOrCreateCallerSeedTLS(*f->getParent(), intType);
	auto *callerSeed = IRB.CreateLoad(intType, callerSeedTLS, "callerSeed");
	auto *callerAliasSeed = IRB.CreateXor(
	    callerSeed, ConstantInt::get(intType, RandomEngine.get_uint64_t()), "callerAliasSeed");
	IRB.CreateStore(callerSeed, switchCallerSeedVar, true);
	IRB.CreateStore(callerAliasSeed, switchCallerAliasSeedVar, true);

	SmallVector<ConstantInt *, 32> knownCaseValues;
	auto hasKnownCase = [&](ConstantInt *Candidate) {
		for (auto *Known : knownCaseValues) {
			if (Known->getValue() == Candidate->getValue()) {
				return true;
			}
		}
		return false;
	};
	auto registerCase = [&](ConstantInt *Candidate) -> ConstantInt * {
		if (!Candidate || hasKnownCase(Candidate)) {
			return nullptr;
		}
		knownCaseValues.push_back(Candidate);
		return Candidate;
	};
	auto createRandomUniqueCase = [&]() -> ConstantInt * {
		for (unsigned attempt = 0; attempt < 64; ++attempt) {
			auto *Candidate = cast<ConstantInt>(ConstantInt::get(
			                                        intType, RandomEngine.get_uint64_t()));
			if (registerCase(Candidate)) {
				return Candidate;
			}
		}
		return nullptr;
	};
	uint64_t nextStateOrdinal = 0;
	auto createPrimaryCase = [&]() -> ConstantInt * {
		for (unsigned attempt = 0; attempt < 64; ++attempt) {
			ConstantInt *Candidate = nullptr;
			if (pointerSize == 8) {
				Candidate = cast<ConstantInt>(ConstantInt::get(
				                                  intType,
				                                  cryptoutils->scramble64(nextStateOrdinal++,
				                                                          scrambling_key)));
			} else {
				Candidate = cast<ConstantInt>(ConstantInt::get(
				                                  intType,
				                                  cryptoutils->scramble32(static_cast<uint32_t>(nextStateOrdinal++),
				                                                          scrambling_key)));
			}
			if (registerCase(Candidate)) {
				return Candidate;
			}
		}
		return createRandomUniqueCase();
	};

	DenseMap<BasicBlock *, SmallVector<ConstantInt *, 4>> blockCaseAliases;
	SmallVector<std::pair<ConstantInt *, BasicBlock *>, 32> dispatchStates;
	for (auto *BB : origBB) {
		auto &Aliases = blockCaseAliases[BB];
		if (auto *Primary = createPrimaryCase()) {
			Aliases.push_back(Primary);
			dispatchStates.emplace_back(Primary, BB);
		}

		const unsigned aliasCount = 2 + static_cast<unsigned>(RandomEngine.get_uint64_t() & 1ULL);
		while (Aliases.size() < aliasCount) {
			auto *Alias = createRandomUniqueCase();
			if (!Alias) {
				break;
			}
			Aliases.push_back(Alias);
			dispatchStates.emplace_back(Alias, BB);
		}
	}

	auto pickAliasPair = [&](BasicBlock *Target) {
		auto It = blockCaseAliases.find(Target);
		if (It == blockCaseAliases.end() || It->second.empty()) {
			auto *Fallback = blockCaseAliases[bbEndOfEntry].front();
			return std::make_pair(Fallback, Fallback);
		}
		auto &Aliases = It->second;
		size_t FirstIndex = static_cast<size_t>(RandomEngine.get_uint64_t() % Aliases.size());
		size_t SecondIndex = FirstIndex;
		if (Aliases.size() > 1) {
			SecondIndex = (FirstIndex + 1 +
			               static_cast<size_t>(RandomEngine.get_uint64_t() % (Aliases.size() - 1))) %
			              Aliases.size();
		}
		return std::make_pair(Aliases[FirstIndex], Aliases[SecondIndex]);
	};

	const auto EntryAliases = pickAliasPair(bbEndOfEntry);
	ConstantInt *entryCaseValue = EntryAliases.first;
	ConstantInt *entryRandomXor = cast<ConstantInt>(
	                                  ConstantInt::get(intType, RandomEngine.get_uint64_t()));
	ConstantInt *entryAliasRandomXor = cast<ConstantInt>(
	                                       ConstantInt::get(intType, RandomEngine.get_uint64_t()));
	auto entryEncoded = IRB.CreateXor(
	    ConstantExpr::getXor(entryRandomXor, EntryAliases.first), callerSeed, "entryEncoded");
	auto entryAliasEncoded = IRB.CreateXor(ConstantExpr::getXor(entryAliasRandomXor, EntryAliases.second),
	                                       callerAliasSeed,
	                                       "entryAliasEncoded");
	IRB.CreateStore(entryEncoded, switchVar, true);
	IRB.CreateStore(entryRandomXor, switchXorVar, true);
	IRB.CreateStore(entryAliasEncoded, switchAliasVar, true);
	IRB.CreateStore(entryAliasRandomXor, switchAliasXorVar, true);

	auto bbLoopEntry = BasicBlock::Create(f->getContext(), "loopEntry", f,
	                                      insertBlock);
	auto bbDispatchAlias = BasicBlock::Create(f->getContext(), "dispatchAlias", f,
	                                          insertBlock);
	auto bbLoopEnd = BasicBlock::Create(f->getContext(), "loopEnd", f,
	                                    insertBlock);
	IRB.SetInsertPoint(bbLoopEntry);
	insertBlock->moveBefore(bbLoopEntry);
	BranchInst::Create(bbLoopEntry, insertBlock);

	BranchInst::Create(bbDispatchAlias, bbLoopEntry);
	BranchInst::Create(bbLoopEntry, bbLoopEnd);

	auto swDefault = BasicBlock::Create(f->getContext(), "switchDefault", f,
	                                    bbLoopEnd);
	BranchInst::Create(bbLoopEnd, swDefault);

	IRB.SetInsertPoint(bbDispatchAlias);
	auto switchVarLoad = IRB.CreateLoad(intType, switchVar, "switchVar");
	auto switchXorLoad = IRB.CreateLoad(intType, switchXorVar, "switchXor");
	auto callerSeedLoad = IRB.CreateLoad(intType, switchCallerSeedVar, "callerSeedLoad");
	auto primaryState = IRB.CreateXor(IRB.CreateXor(switchVarLoad, switchXorLoad),
	                                  callerSeedLoad,
	                                  "primaryState");
	auto switchAliasLoad = IRB.CreateLoad(intType, switchAliasVar, "switchAlias");
	auto switchAliasXorLoad = IRB.CreateLoad(intType, switchAliasXorVar, "switchAliasXor");
	auto callerAliasSeedLoad = IRB.CreateLoad(intType, switchCallerAliasSeedVar,
	                                          "callerAliasSeedLoad");
	auto aliasState = IRB.CreateXor(IRB.CreateXor(switchAliasLoad, switchAliasXorLoad),
	                                callerAliasSeedLoad,
	                                "aliasState");
	auto opaqueMix = IRB.CreateAdd(IRB.CreateXor(primaryState, aliasState),
	                               ConstantInt::get(intType, 1),
	                               "opaqueMix", true, true);
	auto opaqueCond = IRB.CreateICmpEQ(opaqueMix, ConstantInt::get(intType, 1),
	                                   "opaqueCond");
	auto switchCondition = IRB.CreateSelect(opaqueCond, aliasState, primaryState,
	                                        "dispatchState");

	// #region debug-point B:before-switch-create
	if (isIRObfuscationDebugEnabled()) {
		errs() << "[DEBUG][FLA][B] before switch create func=" << f->getName()
		       << " entryCaseValue=" << *entryCaseValue
		       << " bbLoopEntry=" << bbLoopEntry->getName()
		       << " bbDispatchAlias=" << bbDispatchAlias->getName()
		       << " bbLoopEnd=" << bbLoopEnd->getName()
		       << " logicalBlocks=" << origBB.size()
		       << " dispatchStates=" << dispatchStates.size() << "\n";
	}
	// #endregion
	constexpr unsigned DispatchBucketCount = 4;
	ConstantInt *dispatchBucketSalt = cast<ConstantInt>(
	                                     ConstantInt::get(intType, RandomEngine.get_uint64_t()));
	auto dispatchBucket = IRB.CreateAnd(IRB.CreateXor(switchCondition, dispatchBucketSalt),
	                                    ConstantInt::get(intType, DispatchBucketCount - 1),
	                                    "dispatchBucket");
	auto bucketSwitch = SwitchInst::Create(dispatchBucket, swDefault, DispatchBucketCount,
	                                       bbDispatchAlias);

	std::array<SmallVector<std::pair<ConstantInt *, BasicBlock *>, 8>, DispatchBucketCount> bucketStates;
	DenseMap<ConstantInt *, BasicBlock *> caseLandingBlocks;
	unsigned landingIndex = 0;
	for (const auto &DispatchState : dispatchStates) {
		auto *CaseValue = DispatchState.first;
		auto *TargetBB = DispatchState.second;
		auto *LandingBB = BasicBlock::Create(f->getContext(),
		                                     "dispatchState" + Twine(landingIndex++),
		                                     f,
		                                     bbLoopEnd);
		IRBuilder<> LandingIRB(LandingBB);
		auto *StateNoise = LandingIRB.CreateXor(
		    CaseValue,
		    ConstantInt::get(intType, RandomEngine.get_uint64_t()),
		    "stateNoise");
		(void)StateNoise;
		LandingIRB.CreateBr(TargetBB);
		caseLandingBlocks[CaseValue] = LandingBB;

		const auto BucketIndex = static_cast<size_t>(
		    (((CaseValue->getValue() ^ dispatchBucketSalt->getValue()) &
		      APInt(CaseValue->getBitWidth(), DispatchBucketCount - 1)).getZExtValue()));
		bucketStates[BucketIndex].push_back(DispatchState);
	}

	std::array<BasicBlock *, DispatchBucketCount> bucketBlocks{};
	for (unsigned BucketIndex = 0; BucketIndex < DispatchBucketCount; ++BucketIndex) {
		bucketBlocks[BucketIndex] = BasicBlock::Create(
		    f->getContext(), "dispatchBucket" + Twine(BucketIndex), f, swDefault);
		bucketSwitch->addCase(ConstantInt::get(intType, BucketIndex), bucketBlocks[BucketIndex]);
	}

	f->begin()->getTerminator()->eraseFromParent();
	BranchInst::Create(bbLoopEntry, &*f->begin());

	for (auto *BB : origBB) {
		BB->moveBefore(bbLoopEnd);
	}

	// #region debug-point B:after-real-cases
	if (isIRObfuscationDebugEnabled()) {
		errs() << "[DEBUG][FLA][B] real cases added func=" << f->getName()
		       << " case-count=" << dispatchStates.size()
		       << " bucket-count=" << DispatchBucketCount << "\n";
	}
	// #endregion

	for (unsigned BucketIndex = 0; BucketIndex < DispatchBucketCount; ++BucketIndex) {
		auto *CurrentCheckBB = bucketBlocks[BucketIndex];
		if (bucketStates[BucketIndex].empty()) {
			BranchInst::Create(swDefault, CurrentCheckBB);
			continue;
		}

		for (size_t StateIndex = 0; StateIndex < bucketStates[BucketIndex].size(); ++StateIndex) {
			auto [CaseValue, TargetBB] = bucketStates[BucketIndex][StateIndex];
			(void)TargetBB;
			const bool IsLast = StateIndex + 1 == bucketStates[BucketIndex].size();
			BasicBlock *NextCheckBB = swDefault;
			if (!IsLast) {
				NextCheckBB = BasicBlock::Create(
				    f->getContext(),
				    "dispatchCheck" + Twine(BucketIndex) + "_" + Twine(StateIndex),
				    f,
				    swDefault);
			}

			IRBuilder<> BucketIRB(CurrentCheckBB);
			auto *CaseMatch = BucketIRB.CreateICmpEQ(switchCondition, CaseValue, "caseMatch");
			BucketIRB.CreateCondBr(CaseMatch, caseLandingBlocks[CaseValue], NextCheckBB);
			CurrentCheckBB = NextCheckBB;
		}
	}

	// #region debug-point C:after-fake-cases
	if (isIRObfuscationDebugEnabled()) {
		errs() << "[DEBUG][FLA][C] fake cases added func=" << f->getName()
		       << " total-case-count=" << dispatchStates.size()
		       << " reachable-aliases=yes"
		       << " dispatch-buckets=" << DispatchBucketCount << "\n";
	}
	// #endregion

	for (auto bi = origBB.begin(); bi != origBB.end(); ++bi) {
		const auto bb = *bi;

		if (bb->getTerminator()->getNumSuccessors() == 0) {
			continue;
		}

		IRB.SetInsertPoint(bb->getTerminator());
		if (bb->getTerminator()->getNumSuccessors() == 1) {
			auto tbb = bb->getTerminator()->getSuccessor(0);
			const auto NextAliases = pickAliasPair(tbb);

			// #region debug-point C:single-succ-rewrite
			if (isIRObfuscationDebugEnabled()) {
				errs() << "[DEBUG][FLA][C] rewrite single-succ func=" << f->getName()
				       << " bb=" << bb->getName()
				       << " target=" << tbb->getName()
				       << " aliasStates=" << blockCaseAliases[tbb].size() << "\n";
			}
			// #endregion

			ConstantInt *randomXor = cast<ConstantInt>(
			                             ConstantInt::get(intType, RandomEngine.get_uint64_t()));
			ConstantInt *aliasRandomXor = cast<ConstantInt>(
			                                  ConstantInt::get(intType, RandomEngine.get_uint64_t()));
			auto *callerSeedState = IRB.CreateLoad(intType, switchCallerSeedVar, "callerSeedState");
			auto *callerAliasSeedState =
			    IRB.CreateLoad(intType, switchCallerAliasSeedVar, "callerAliasSeedState");

			auto xorKey = IRB.CreateXor(ConstantExpr::getXor(randomXor, NextAliases.first),
			                            callerSeedState,
			                            "encodedNextState");
			auto aliasXorKey = IRB.CreateXor(
			    ConstantExpr::getXor(aliasRandomXor, NextAliases.second),
			    callerAliasSeedState,
			    "encodedNextAlias");

			IRB.CreateStore(xorKey, switchVar, true);
			IRB.CreateStore(randomXor, switchXorVar, true);
			IRB.CreateStore(aliasXorKey, switchAliasVar, true);
			IRB.CreateStore(aliasRandomXor, switchAliasXorVar, true);
			IRB.CreateBr(bbLoopEnd);
			bb->getTerminator()->eraseFromParent();
			continue;
		}

		if (bb->getTerminator()->getNumSuccessors() == 2) {
			auto numToCaseTrue = pickAliasPair(bb->getTerminator()->getSuccessor(0));
			auto numToCaseFalse = pickAliasPair(bb->getTerminator()->getSuccessor(1));

			// #region debug-point C:double-succ-rewrite
			if (isIRObfuscationDebugEnabled()) {
				errs() << "[DEBUG][FLA][C] rewrite double-succ func=" << f->getName()
				       << " bb=" << bb->getName()
				       << " trueTarget=" << bb->getTerminator()->getSuccessor(0)->getName()
				       << " falseTarget=" << bb->getTerminator()->getSuccessor(1)->getName()
				       << " trueAliases=" << blockCaseAliases[bb->getTerminator()->getSuccessor(0)].size()
				       << " falseAliases=" << blockCaseAliases[bb->getTerminator()->getSuccessor(1)].size()
				       << "\n";
			}
			// #endregion

			ConstantInt *randomXor = cast<ConstantInt>(
			                             ConstantInt::get(intType, RandomEngine.get_uint64_t()));
			ConstantInt *aliasRandomXor = cast<ConstantInt>(
			                                  ConstantInt::get(intType, RandomEngine.get_uint64_t()));
			auto *callerSeedState = IRB.CreateLoad(intType, switchCallerSeedVar, "callerSeedState");
			auto *callerAliasSeedState =
			    IRB.CreateLoad(intType, switchCallerAliasSeedVar, "callerAliasSeedState");

			auto xorKeyT = IRB.CreateXor(ConstantExpr::getXor(numToCaseTrue.first, randomXor),
			                             callerSeedState,
			                             "encodedTrueState");
			auto xorKeyF = IRB.CreateXor(ConstantExpr::getXor(numToCaseFalse.first, randomXor),
			                             callerSeedState,
			                             "encodedFalseState");
			auto aliasXorKeyT =
			    IRB.CreateXor(ConstantExpr::getXor(numToCaseTrue.second, aliasRandomXor),
			                  callerAliasSeedState,
			                  "encodedTrueAlias");
			auto aliasXorKeyF =
			    IRB.CreateXor(ConstantExpr::getXor(numToCaseFalse.second, aliasRandomXor),
			                  callerAliasSeedState,
			                  "encodedFalseAlias");
			IRB.CreateStore(randomXor, switchXorVar, true);
			IRB.CreateStore(aliasRandomXor, switchAliasXorVar, true);

			auto br = cast<BranchInst>(bb->getTerminator());
			auto sel = IRB.CreateSelect(br->getCondition(), xorKeyT, xorKeyF);
			auto aliasSel = IRB.CreateSelect(br->getCondition(), aliasXorKeyT, aliasXorKeyF);

			IRB.CreateStore(sel, switchVar, true);
			IRB.CreateStore(aliasSel, switchAliasVar, true);
			IRB.CreateBr(bbLoopEnd);

			bb->getTerminator()->eraseFromParent();
			continue;
		}
	}

	// #region debug-point D:before-fixstack
	if (isIRObfuscationDebugEnabled()) {
		errs() << "[DEBUG][FLA][D] before fixStack func=" << f->getName()
		       << " total-blocks=" << f->size()
		       << " total-cases=" << dispatchStates.size() << "\n";
	}
	dumpBlocksWithoutTerminator("pre-fixstack");
	// #endregion
	fixStack(f);

	// #region debug-point D:before-lowerswitch
	if (isIRObfuscationDebugEnabled()) {
		errs() << "[DEBUG][FLA][D] before lowerSwitch func=" << f->getName()
		       << " total-blocks=" << f->size() << "\n";
	}
	dumpBlocksWithoutTerminator("post-fixstack");
	// #endregion
	// The strengthened dispatcher currently crashes LegacyLowerSwitch on Android
	// toolchains. Keep the augmented switch-based state machine intact instead of
	// lowering it again so the transformed CFG remains valid and compilable.
	// #region debug-point D:skip-lowerswitch
	if (isIRObfuscationDebugEnabled()) {
		errs() << "[DEBUG][FLA][D] skip lowerSwitch func=" << f->getName()
		       << " total-blocks=" << f->size() << "\n";
	}
	// #endregion

	// #region debug-point D:verify-function
	if (isIRObfuscationDebugEnabled()) {
		bool Broken = verifyFunction(*f, &errs());
		errs() << "[DEBUG][FLA][D] verify function=" << f->getName()
		       << " broken=" << (Broken ? "yes" : "no") << "\n";
	}
	// #endregion

	return true;
}

char                            Flattening::ID = 0;
static RegisterPass<Flattening> X("flattening", "Call graph flattening");

/**
 * @brief 创建控制流扁平化Pass
 * @param pointerSize 指针大小（32位或64位）
 * @param argsOptions 混淆选项配置对象
 * @return 返回创建的FunctionPass指针
 */
FunctionPass *llvm::createFlatteningPass(unsigned            pointerSize,
        ObfuscationOptions *argsOptions) {
	return new Flattening(pointerSize, argsOptions);
}
