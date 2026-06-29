//===- StringEncryption.cpp - 字符串加密Pass----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件实现字符串加密Pass，通过加密字符串常量来增加逆向分析的难度
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/ObfuscationOptions.h"
#include "llvm/Transforms/Obfuscation/ObfuscationPassManager.h"
#include "llvm/Transforms/Obfuscation/StringEncryption.h"
#include "llvm/Transforms/Obfuscation/Utils.h"
#include "llvm/Transforms/Utils/GlobalStatus.h"
#include "llvm/Transforms/IPO/Attributor.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/CryptoUtils.h"
#include <map>
#include <set>
#include <iostream>
#include <algorithm>

#define DEBUG_TYPE "string-encryption"

using namespace llvm;
namespace {
	struct StringEncryption : public ModulePass {
		static char ID;  ///< Pass标识符

		/**
		 * @brief 常量字符串池条目结构体
		 * 存储加密字符串的相关信息
		 */
		struct CSPEntry {
			CSPEntry() : ID(0), Offset(0), DecGV(nullptr), DecStatus(nullptr), IsUTF16(false), DecFunc(nullptr) {}
			unsigned ID;
			unsigned Offset;
			GlobalVariable *DecGV;
			GlobalVariable *DecStatus;
			std::vector<uint8_t> Data;
			std::vector<uint8_t> EncKey;
			bool IsUTF16;
			std::vector<uint16_t> Data16;
			std::vector<uint16_t> EncKey16;
			Function *DecFunc;
		};

		/**
		 * @brief 常量字符串用户结构体
		 * 存储使用常量字符串的全局变量信息
		 */
		struct CSUser {
			CSUser(Type* ETy, GlobalVariable *User, GlobalVariable *NewGV)
				: Ty(ETy), GV(User), DecGV(NewGV), DecStatus(nullptr),
				  InitFunc(nullptr) {}
			Type *Ty;
			GlobalVariable *GV;
			GlobalVariable *DecGV;
			GlobalVariable *DecStatus;
			Function *InitFunc;
		};

		ObfuscationOptions *ArgsOptions;  ///< 混淆选项配置
		CryptoUtils RandomEngine;  ///< 随机数生成引擎
		std::vector<CSPEntry *> ConstantStringPool;  ///< 常量字符串池
		std::map<GlobalVariable *, CSPEntry *> CSPEntryMap;  ///< 全局变量到条目的映射
		std::map<GlobalVariable *, CSUser *> CSUserMap;  ///< 全局变量到用户的映射
		std::map<GlobalVariable *, CSPEntry *> DecryptedCSPEntryMap;  ///< 解密缓冲区到条目的反向映射
		std::map<GlobalVariable *, CSUser *> DecryptedCSUserMap;  ///< 解密用户缓冲区到用户的反向映射
		std::map<const CSUser *, SmallVector<CSPEntry *, 8>> UserReferencedEntries;  ///< 每个全局用户依赖的底层字符串
		std::map<GlobalVariable *, SmallVector<CSPEntry *, 8>> GlobalReferencedEntries;  ///< 任意全局变量依赖的底层字符串
		SmallPtrSet<CSPEntry *, 16> PinnedEntries;  ///< 被指针表/聚合引用的条目，不能激进擦除
		GlobalVariable *EncryptedStringTable = nullptr;  ///< 加密字符串表全局变量
		std::set<GlobalVariable *> MaybeDeadGlobalVars;  ///< 可能死亡的全局变量集合

		/**
		 * @brief 构造函数，初始化字符串加密Pass
		 * @param argsOptions 混淆选项配置对象
		 */
		StringEncryption(ObfuscationOptions *argsOptions) : ModulePass(ID) {
			this->ArgsOptions = argsOptions;
			initializeStringEncryptionPass(*PassRegistry::getPassRegistry());
		}

		/**
		 * @brief Pass结束时的清理工作
		 * @param M LLVM模块
		 * @return 始终返回false
		 */
		bool doFinalization(Module &) override {
			for (CSPEntry *Entry : ConstantStringPool) {
				delete (Entry);
			}
			for (auto &I : CSUserMap) {
				CSUser *User = I.second;
				delete (User);
			}
			ConstantStringPool.clear();
			CSPEntryMap.clear();
			CSUserMap.clear();
			DecryptedCSPEntryMap.clear();
			DecryptedCSUserMap.clear();
			UserReferencedEntries.clear();
			GlobalReferencedEntries.clear();
			PinnedEntries.clear();
			MaybeDeadGlobalVars.clear();
			return false;
		}

		/**
		 * @brief 获取Pass名称
		 * @return Pass名称字符串
		 */
		StringRef getPassName() const override {
			return {"StringEncryption"};
		}

		/**
		 * @brief 对模块执行字符串加密
		 * @param M 要处理的LLVM模块
		 * @return 如果模块被修改返回true，否则返回false
		 */
		bool runOnModule(Module &M) override;
		
		/**
		 * @brief 收集使用常量字符串的全局变量
		 * @param CString 常量字符串全局变量
		 * @param Users 输出的用户集合
		 */
		static void collectConstantStringUser(GlobalVariable *CString, std::set<GlobalVariable *> &Users);
		
		/**
		 * @brief 检查全局变量是否适合加密
		 * @param GV 要检查的全局变量
		 * @return 如果适合加密返回true
		 */
		static bool isValidToEncrypt(GlobalVariable *GV);
		static GlobalVariable *extractReferencedGlobal(Value *V);
		CSPEntry *resolveCSPEntryGlobal(GlobalVariable *GV);
		CSUser *resolveCSUserGlobal(GlobalVariable *GV);
		
		/**
		 * @brief 获取全局变量指向的解密后的全局变量
		 * @param GV 全局变量（指针类型）
		 * @return 如果该全局变量的初始化器指向加密的字符串，返回对应的解密后全局变量；否则返回nullptr
		 */
		GlobalVariable *getPointedDecryptedGlobal(GlobalVariable *GV);
		
		/**
		 * @brief 获取全局变量指向的CSPEntry
		 * @param GV 全局变量（指针类型）
		 * @return 如果该全局变量的初始化器指向加密的字符串，返回对应的CSPEntry；否则返回nullptr
		 */
		CSPEntry *getPointedCSPEntry(GlobalVariable *GV);
		
		/**
		 * @brief 获取全局变量指向的CSUser
		 * @param GV 全局变量（指针类型）
		 * @return 如果该全局变量的初始化器指向加密的字符串，返回对应的CSUser；否则返回nullptr
		 */
		CSUser *getPointedCSUser(GlobalVariable *GV);
		
		/**
		 * @brief 处理函数中的常量字符串使用
		 * @param F 要处理的函数
		 * @return 如果函数被修改返回true
		 */
		bool processConstantStringUse(Function *F);
		
		/**
		 * @brief 删除未使用的全局变量
		 */
		void deleteUnusedGlobalVariable();
		
		/**
		 * @brief 构建解密函数
		 * @param M LLVM模块
		 * @param Entry 常量字符串池条目
		 * @return 构建的解密函数
		 */
		static Function *buildDecryptFunction(Module *M, const CSPEntry *Entry);
		
		/**
		 * @brief 构建初始化函数
		 * @param M LLVM模块
		 * @param User 常量字符串用户
		 * @return 构建的初始化函数
		 */
		Function *buildInitFunction(Module *M, const CSUser *User);
		
		/**
		 * @brief 生成随机字节
		 * @param Bytes 输出的字节向量
		 * @param MinSize 最小大小
		 * @param MaxSize 最大大小
		 */
		template <typename T>
		void getRandomBytes(std::vector<T> &Bytes, uint32_t MinSize, uint32_t MaxSize);
		
		/**
		 * @brief 降低全局常量到运行时初始化
		 * @param CV 常量值
		 * @param IRB IR构建器
		 * @param Ptr 目标指针
		 * @param Ty 类型
		 */
		void lowerGlobalConstant(Constant *CV, IRBuilder<> &IRB, Value *Ptr, Type *Ty);
		
		/**
		 * @brief 降低结构体常量
		 * @param CS 结构体常量
		 * @param IRB IR构建器
		 * @param Ptr 目标指针
		 * @param Ty 类型
		 */
		void lowerGlobalConstantStruct(ConstantStruct *CS, IRBuilder<> &IRB, Value *Ptr, Type *Ty);
		
		/**
		 * @brief 降低数组常量
		 * @param CA 数组常量
		 * @param IRB IR构建器
		 * @param Ptr 目标指针
		 * @param Ty 类型
		 */
		void lowerGlobalConstantArray(ConstantArray *CA, IRBuilder<> &IRB, Value *Ptr, Type *Ty);
		void collectPinnedEntries(Constant *CV, SmallPtrSetImpl<Constant *> &Visited);
		void collectReferencedEntries(Constant *CV, SmallPtrSetImpl<Constant *> &Visited,
		                             SmallPtrSetImpl<CSPEntry *> &Entries);

		/**
		 * @brief 在当前基本块结束前擦除解密缓冲区，避免明文长期驻留
		 * @param BB 需要插入擦除逻辑的基本块
		 * @param GV 需要擦除的全局缓冲区
		 */
		void scheduleWipeAtBlockEnd(BasicBlock *BB, GlobalVariable *GV);
		static bool shouldSkipBlockEndWipe(BasicBlock *BB, Instruction *InsertBefore);
	};
} // namespace llvm

char StringEncryption::ID = 0;

/**
 * @brief 对模块执行字符串加密的主函数
 * @param M 要处理的LLVM模块
 * @return 如果模块被修改返回true
 */
bool StringEncryption::runOnModule(Module &M) {
	if (isIRObfuscationDebugEnabled()) {
		errs() << "[DEBUG] StringEncryption: Starting runOnModule\n";
	}
	std::set<GlobalVariable *> ConstantStringUsers;

	LLVMContext &Ctx = M.getContext();
	ConstantInt *Zero = ConstantInt::get(Type::getInt32Ty(Ctx), 0);
	
	for (GlobalVariable &GV : M.globals()) {
		// 跳过没有初始化器的全局变量
		if (!GV.hasInitializer() ||
		    GV.hasDLLExportStorageClass() || GV.isDLLImportDependent()) {
			continue;
		}
		Constant *Init = GV.getInitializer();
		if (Init == nullptr)
			continue;
		
		if (isIRObfuscationDebugEnabled()) {
			errs() << "[DEBUG] CSE: Checking global: " << GV.getName() 
			       << " isConstant=" << GV.isConstant()
			       << " type=" << *GV.getType() 
			       << " initType=" << *Init->getType() << "\n";
			
			// 打印初始化器的具体类型
			errs() << "[DEBUG] CSE:   Init->getValueID() = " << Init->getValueID() << "\n";
			errs() << "[DEBUG] CSE:   Init->getName() = " << Init->getName() << "\n";
			
			// 检查是否是 dso_local_equivalent
			if (auto *DSO = dyn_cast<DSOLocalEquivalent>(Init)) {
				errs() << "[DEBUG] CSE:   Init is DSOLocalEquivalent\n";
			}
			
			// 检查是否是 GlobalObject
			if (isa<GlobalObject>(Init)) {
				errs() << "[DEBUG] CSE:   Init is GlobalObject\n";
			}
			
			// 检查是否是 GlobalValue
			if (isa<GlobalValue>(Init)) {
				errs() << "[DEBUG] CSE:   Init is GlobalValue\n";
			}
			
			if (GlobalVariable *InnerGV = dyn_cast<GlobalVariable>(Init)) {
				errs() << "[DEBUG] CSE:   Inner GlobalVariable name: " << InnerGV->getName() << "\n";
			}
		}
		
		// 处理字符串常量（数组类型）
		if (ConstantDataSequential *CDS = dyn_cast<ConstantDataSequential>(Init)) {
			if (CDS->isString()) {
				CSPEntry *Entry = new CSPEntry();
				Entry->IsUTF16 = false;
				StringRef Data = CDS->getRawDataValues();
				Entry->Data.reserve(Data.size());
				for (unsigned i = 0; i < Data.size(); ++i) {
					Entry->Data.push_back(static_cast<uint8_t>(Data[i]));
				}
				Entry->ID = static_cast<unsigned>(ConstantStringPool.size());
				Constant *ZeroInit = Constant::getNullValue(CDS->getType());
				GlobalVariable *DecGV = new GlobalVariable(M, CDS->getType(), false, GlobalValue::PrivateLinkage,
				    ZeroInit, "dec" + Twine::utohexstr(Entry->ID) + GV.getName());
				GlobalVariable *DecStatus = new GlobalVariable(M, Type::getInt32Ty(Ctx), false, GlobalValue::PrivateLinkage,
				    Zero, "dec_status_" + Twine::utohexstr(Entry->ID) + GV.getName());
				DecGV->setSection(".AProtect.data");
				DecStatus->setSection(".AProtect.data");
				DecGV->setAlignment(MaybeAlign(GV.getAlignment()));
				DecGV->setMetadata("noobf", MDNode::get(Ctx, {}));
				DecStatus->setMetadata("noobf", MDNode::get(Ctx, {}));
				Entry->DecGV = DecGV;
				Entry->DecStatus = DecStatus;
				ConstantStringPool.push_back(Entry);
				CSPEntryMap[&GV] = Entry;
				DecryptedCSPEntryMap[DecGV] = Entry;
				collectConstantStringUser(&GV, ConstantStringUsers);
			} else {
				Type *EltTy = CDS->getElementType();
				if (EltTy->isIntegerTy(16)) {
					CSPEntry *Entry = new CSPEntry();
					Entry->IsUTF16 = true;
					unsigned NumElems = CDS->getNumElements();
					Entry->Data16.reserve(NumElems);
					for (unsigned i = 0; i < NumElems; ++i) {
						uint64_t v = CDS->getElementAsInteger(i);
						Entry->Data16.push_back(static_cast<uint16_t>(v));
					}
					Entry->ID = static_cast<unsigned>(ConstantStringPool.size());
					Constant *ZeroInit = Constant::getNullValue(CDS->getType());
					GlobalVariable *DecGV = new GlobalVariable(M, CDS->getType(), false, GlobalValue::PrivateLinkage,
					    ZeroInit, "dec" + Twine::utohexstr(Entry->ID) + GV.getName());
					GlobalVariable *DecStatus = new GlobalVariable(M, Type::getInt32Ty(Ctx), false, GlobalValue::PrivateLinkage,
					    Zero, "dec_status_" + Twine::utohexstr(Entry->ID) + GV.getName());
					DecGV->setSection(".AProtect.data");
					DecStatus->setSection(".AProtect.data");
					DecGV->setAlignment(MaybeAlign(GV.getAlignment()));
					DecGV->setMetadata("noobf", MDNode::get(Ctx, {}));
					DecStatus->setMetadata("noobf", MDNode::get(Ctx, {}));
					Entry->DecGV = DecGV;
					Entry->DecStatus = DecStatus;
					ConstantStringPool.push_back(Entry);
					CSPEntryMap[&GV] = Entry;
					DecryptedCSPEntryMap[DecGV] = Entry;
					collectConstantStringUser(&GV, ConstantStringUsers);
				}
			}
		}
	}

	for (CSPEntry *Entry : ConstantStringPool) {
		if (!Entry->IsUTF16) {
			getRandomBytes(Entry->EncKey, 16, 32);
			uint8_t LastPlainChar = 0;
			for (unsigned i = 0; i < Entry->Data.size(); ++i) {
				const uint32_t KeyIndex = i % Entry->EncKey.size();
				const uint8_t CurrentKey = Entry->EncKey[KeyIndex];
				const uint8_t CurrentPlainChar = Entry->Data[i];
				uint8_t val = CurrentPlainChar;
				val ^= CurrentKey;
				if ((KeyIndex * CurrentKey) % 2 == 0) {
					val = ~val;
					val ^= CurrentKey;
					val = val - LastPlainChar;
				} else {
					val = -val;
					val ^= CurrentKey;
					val = val + LastPlainChar;
				}
				Entry->Data[i] = val;
				LastPlainChar = CurrentPlainChar;
			}
		} else {
			getRandomBytes(Entry->EncKey16, 8, 16);
			uint16_t LastPlainChar = 0;
			for (unsigned i = 0; i < Entry->Data16.size(); ++i) {
				const uint32_t KeyIndex = i % Entry->EncKey16.size();
				const uint16_t CurrentKey = Entry->EncKey16[KeyIndex];
				const uint16_t CurrentPlainChar = Entry->Data16[i];
				uint16_t val = CurrentPlainChar;
				val ^= CurrentKey;
				if (((KeyIndex * CurrentKey) % 2) == 0) {
					val = ~val;
					val ^= CurrentKey;
					val = static_cast<uint16_t>(val - LastPlainChar);
				} else {
					val = static_cast<uint16_t>(-static_cast<int16_t>(val));
					val ^= CurrentKey;
					val = static_cast<uint16_t>(val + LastPlainChar);
				}
				Entry->Data16[i] = val;
				LastPlainChar = CurrentPlainChar;
			}
		}
		Entry->DecFunc = buildDecryptFunction(&M, Entry);
	}

	for (GlobalVariable *GV : ConstantStringUsers) {
		if (isValidToEncrypt(GV)) {
			if (GV->hasInitializer()) {
				SmallPtrSet<Constant *, 16> Visited;
				collectPinnedEntries(GV->getInitializer(), Visited);
			}
			Type *EltType = GV->getValueType();
			Constant *ZeroInit = Constant::getNullValue(EltType);
			GlobalVariable *DecGV = new GlobalVariable(M, EltType, false, GlobalValue::PrivateLinkage,
			    ZeroInit, "dec_" + GV->getName());
			DecGV->setSection(".AProtect.data");
			DecGV->setAlignment(MaybeAlign(GV->getAlignment()));
			DecGV->setMetadata("noobf", MDNode::get(Ctx, {}));
			GlobalVariable *DecStatus = new GlobalVariable(M, Type::getInt32Ty(Ctx), false, GlobalValue::PrivateLinkage,
			    Zero, "dec_status_" + GV->getName());
			DecStatus->setSection(".AProtect.data");
			DecStatus->setMetadata("noobf", MDNode::get(Ctx, {}));
			CSUser *User = new CSUser(EltType, GV, DecGV);
			User->DecStatus = DecStatus;
			User->InitFunc = buildInitFunction(&M, User);
			CSUserMap[GV] = User;
			DecryptedCSUserMap[DecGV] = User;
		}
	}

	for (auto &KV : CSUserMap) {
		CSUser *User = KV.second;
		if (!User || !User->GV || !User->GV->hasInitializer()) {
			continue;
		}
		SmallPtrSet<Constant *, 16> Visited;
		SmallPtrSet<CSPEntry *, 16> Entries;
		collectReferencedEntries(User->GV->getInitializer(), Visited, Entries);
		auto &Collected = UserReferencedEntries[User];
		for (CSPEntry *Entry : Entries) {
			Collected.push_back(Entry);
		}
	}

	for (GlobalVariable &GV : M.globals()) {
		if (!GV.hasInitializer()) {
			continue;
		}
		SmallPtrSet<Constant *, 16> Visited;
		SmallPtrSet<CSPEntry *, 16> Entries;
		collectReferencedEntries(GV.getInitializer(), Visited, Entries);
		if (Entries.empty()) {
			continue;
		}
		auto &Collected = GlobalReferencedEntries[&GV];
		for (CSPEntry *Entry : Entries) {
			Collected.push_back(Entry);
		}
	}

	std::vector<uint8_t> Data;
	std::vector<uint8_t> JunkBytes;

	JunkBytes.reserve(32);
	for (CSPEntry *Entry : ConstantStringPool) {
		JunkBytes.clear();
		getRandomBytes(JunkBytes, 16, 32);
		Data.insert(Data.end(), JunkBytes.begin(), JunkBytes.end());

		if (Entry->IsUTF16 && (Data.size() % 2) != 0) {
			Data.push_back(0);
		}

		Entry->Offset = static_cast<unsigned>(Data.size());
		if (!Entry->IsUTF16) {
			Data.insert(Data.end(), Entry->EncKey.begin(), Entry->EncKey.end());
			Data.insert(Data.end(), Entry->Data.begin(), Entry->Data.end());
		} else {
			for (uint16_t w : Entry->EncKey16) {
				Data.push_back(static_cast<uint8_t>(w & 0xff));
				Data.push_back(static_cast<uint8_t>((w >> 8) & 0xff));
			}
			for (uint16_t w : Entry->Data16) {
				Data.push_back(static_cast<uint8_t>(w & 0xff));
				Data.push_back(static_cast<uint8_t>((w >> 8) & 0xff));
			}
		}
	}

	Constant *CDA = ConstantDataArray::get(M.getContext(), ArrayRef<uint8_t>(Data));
	EncryptedStringTable = new GlobalVariable(M, CDA->getType(), false, GlobalValue::PrivateLinkage,
	    CDA, "EncryptedStringTable");
	EncryptedStringTable->setSection(".AProtect.rodata");
	EncryptedStringTable->setMetadata("noobf", MDNode::get(Ctx, {}));

	auto remapConstantRef = [&](auto &&Self, Constant *C) -> Constant * {
		if (!C) {
			return nullptr;
		}

		if (auto *StrGV = dyn_cast<GlobalVariable>(C)) {
			if (CSPEntry *Entry = resolveCSPEntryGlobal(StrGV)) {
				if (StrGV != Entry->DecGV) {
					MaybeDeadGlobalVars.insert(StrGV);
				}
				return Entry->DecGV;
			}
			if (CSUser *User = resolveCSUserGlobal(StrGV)) {
				if (StrGV != User->DecGV) {
					MaybeDeadGlobalVars.insert(StrGV);
				}
				return User->DecGV;
			}
			return C;
		}

		if (auto *CE = dyn_cast<ConstantExpr>(C)) {
			Constant *Base = cast<Constant>(CE->getOperand(0));
			Constant *NewBase = Self(Self, Base);
			if (NewBase == Base) {
				return C;
			}
			switch (CE->getOpcode()) {
			case Instruction::GetElementPtr: {
				SmallVector<Constant *, 4> Indices;
				for (unsigned i = 1; i < CE->getNumOperands(); ++i) {
					Indices.push_back(cast<Constant>(CE->getOperand(i)));
				}
				return ConstantExpr::getGetElementPtr(cast<GlobalVariable>(NewBase)->getValueType(),
				                                      NewBase, Indices);
			}
			case Instruction::BitCast:
				return ConstantExpr::getBitCast(NewBase, CE->getType());
			case Instruction::AddrSpaceCast:
				return ConstantExpr::getAddrSpaceCast(NewBase, CE->getType());
			default:
				return C;
			}
		}

		if (auto *CA = dyn_cast<ConstantArray>(C)) {
			SmallVector<Constant *, 16> NewOperands;
			bool ChangedOperands = false;
			for (unsigned i = 0; i < CA->getNumOperands(); ++i) {
				Constant *Op = CA->getOperand(i);
				Constant *NewOp = Self(Self, Op);
				NewOperands.push_back(NewOp);
				ChangedOperands |= (NewOp != Op);
			}
			if (ChangedOperands) {
				return ConstantArray::get(cast<ArrayType>(CA->getType()), NewOperands);
			}
			return C;
		}

		if (auto *CS = dyn_cast<ConstantStruct>(C)) {
			SmallVector<Constant *, 16> NewOperands;
			bool ChangedOperands = false;
			for (unsigned i = 0; i < CS->getNumOperands(); ++i) {
				Constant *Op = CS->getOperand(i);
				Constant *NewOp = Self(Self, Op);
				NewOperands.push_back(NewOp);
				ChangedOperands |= (NewOp != Op);
			}
			if (ChangedOperands) {
				return ConstantStruct::get(cast<StructType>(CS->getType()), NewOperands);
			}
			return C;
		}

		return C;
	};

	for (GlobalVariable &GV : M.globals()) {
		if (!GV.hasInitializer())
			continue;
		
		Constant *Init = GV.getInitializer();
		if (!Init)
			continue;

		Constant *NewInit = remapConstantRef(remapConstantRef, Init);
		if (NewInit != Init) {
			GV.setInitializer(NewInit);
		}
	}

	bool Changed = false;
	for (Function &F : M) {
		if (F.isDeclaration())
			continue;
		Changed |= processConstantStringUse(&F);
	}

	for (auto &I : CSUserMap) {
		CSUser *User = I.second;
		Changed |= processConstantStringUse(User->InitFunc);
	}

	deleteUnusedGlobalVariable();
	for (CSPEntry *Entry : ConstantStringPool) {
		if (Entry->DecFunc->use_empty()) {
			Entry->DecFunc->eraseFromParent();
		}
	}
	return Changed;
}

/**
 * @brief 生成随机字节
 * @tparam T 字节类型（uint8_t或uint16_t）
 * @param Bytes 输出的字节向量
 * @param MinSize 最小大小
 * @param MaxSize 最大大小
 */
template <typename T>
void StringEncryption::getRandomBytes(std::vector<T> &Bytes, uint32_t MinSize, uint32_t MaxSize) {
	uint32_t N = RandomEngine.get_uint32_t();
	uint32_t Len;

	assert(MaxSize >= MinSize);

	if (MinSize == MaxSize) {
		Len = MinSize;
	} else {
		Len = MinSize + (N % (MaxSize - MinSize));
	}

	char *Buffer = new char[Len * sizeof(T)];
	RandomEngine.get_bytes(Buffer, Len * sizeof(T));
	for (uint32_t i = 0; i < Len; ++i) {
		if constexpr (std::is_same_v<T, uint8_t>) {
			Bytes.push_back(static_cast<uint8_t>(Buffer[i]));
		} else {
			uint8_t b0 = static_cast<uint8_t>(Buffer[i * 2]);
			uint8_t b1 = static_cast<uint8_t>(Buffer[i * 2 + 1]);
			// little-endian combine
			uint16_t w = static_cast<uint16_t>(b0 | (b1 << 8));
			Bytes.push_back(w);
		}
	}

	delete[] Buffer;
}

/**
 * @brief 构建字符串解密函数
 * @param M LLVM模块
 * @param Entry 常量字符串池条目
 * @return 构建的解密函数
 */
Function *StringEncryption::buildDecryptFunction(Module *M, const StringEncryption::CSPEntry *Entry) {
	LLVMContext &Ctx = M->getContext();
	IRBuilder<> IRB(Ctx);

	Type *PlainEltTy = Entry->IsUTF16 ? Type::getInt16Ty(Ctx) : Type::getInt8Ty(Ctx);
	PointerType *PlainPtrTy = PointerType::get(Ctx, 0);
	PointerType *DataPtrTy = PointerType::get(Ctx, 0);

	FunctionType *FuncTy = FunctionType::get(
	                           Type::getVoidTy(Ctx),
	{PlainPtrTy, DataPtrTy},
	false);
	Function *DecFunc =
	    Function::Create(FuncTy, GlobalValue::PrivateLinkage, "abcdefgjh_decrypt_string_" + Twine::utohexstr(Entry->ID), M);

	auto ArgIt = DecFunc->arg_begin();
	Argument *PlainString = ArgIt;
	++ArgIt;
	Argument *Data = ArgIt;

	AttrBuilder NoCaptureAttrBuilder{Ctx};
	NoCaptureAttrBuilder.addCapturesAttr(llvm::CaptureInfo(llvm::CaptureComponents::None));

	PlainString->setName("plain_string");
	PlainString->addAttrs(NoCaptureAttrBuilder);
	Data->setName("data");
	Data->addAttrs(NoCaptureAttrBuilder);

	BasicBlock *Enter = BasicBlock::Create(Ctx, "Enter", DecFunc);
	BasicBlock *LoopBody = BasicBlock::Create(Ctx, "LoopBody", DecFunc);
	BasicBlock *LoopBr0 = BasicBlock::Create(Ctx, "LoopBr0", DecFunc);
	BasicBlock *LoopBr1 = BasicBlock::Create(Ctx, "LoopBr1", DecFunc);
	BasicBlock *LoopEnd = BasicBlock::Create(Ctx, "LoopEnd", DecFunc);
	BasicBlock *Exit = BasicBlock::Create(Ctx, "Exit", DecFunc);

	IRB.SetInsertPoint(Enter);
	ConstantInt *KeyElemSize = ConstantInt::get(Type::getInt32Ty(Ctx), Entry->IsUTF16 ? static_cast<uint32_t>(Entry->EncKey16.size()) : static_cast<uint32_t>(Entry->EncKey.size()));
	uint32_t KeySizeInBytes = Entry->IsUTF16 ? static_cast<uint32_t>(Entry->EncKey16.size() * 2) : static_cast<uint32_t>(Entry->EncKey.size());
	ConstantInt *KeySizeBytesConst = ConstantInt::get(Type::getInt32Ty(Ctx), KeySizeInBytes);

	Value *EncPtr = IRB.CreateInBoundsGEP(IRB.getInt8Ty(), Data, KeySizeBytesConst);
	Value *DecStatus = IRB.CreateLoad(Type::getInt32Ty(Ctx), Entry->DecStatus, "dec_status");
	Value *AlreadyDecrypted = IRB.CreateICmpEQ(DecStatus, IRB.getInt32(1), "already_decrypted");
	IRB.CreateCondBr(AlreadyDecrypted, Exit, LoopBody);

	IRB.SetInsertPoint(LoopBody);
	PHINode *LoopCounter = IRB.CreatePHI(IRB.getInt32Ty(), 2);
	LoopCounter->addIncoming(IRB.getInt32(0), Enter);

	PHINode *LastDecrypted = IRB.CreatePHI(PlainEltTy, 2);
	LastDecrypted->addIncoming(Constant::getNullValue(PlainEltTy), Enter);

	Value *KeyIdx = IRB.CreateURem(LoopCounter, KeyElemSize);

	Value *KeyChar = nullptr;
	if (!Entry->IsUTF16) {
		Value *KeyCharPtr = IRB.CreateInBoundsGEP(IRB.getInt8Ty(), Data, KeyIdx);
		KeyChar = IRB.CreateLoad(IRB.getInt8Ty(), KeyCharPtr);
	} else {
		Value *KeyBase = IRB.CreateBitCast(Data, PointerType::get(Ctx, 0));
		Value *KeyCharPtr = IRB.CreateInBoundsGEP(Type::getInt16Ty(Ctx), KeyBase, KeyIdx);
		KeyChar = IRB.CreateLoad(Type::getInt16Ty(Ctx), KeyCharPtr);
	}

	Value *EncChar = nullptr;
	if (!Entry->IsUTF16) {
		Value *EncCharPtr = IRB.CreateInBoundsGEP(IRB.getInt8Ty(), EncPtr, LoopCounter);
		EncChar = IRB.CreateLoad(IRB.getInt8Ty(), EncCharPtr, true);
	} else {
		Value *Two = IRB.getInt32(2);
		Value *IdxBytes = IRB.CreateMul(LoopCounter, Two);
		Value *EncCharBytePtr = IRB.CreateInBoundsGEP(IRB.getInt8Ty(), EncPtr, IdxBytes);
		Value *EncChar16Ptr = IRB.CreateBitCast(EncCharBytePtr, PointerType::get(Ctx, 0));
		EncChar = IRB.CreateLoad(Type::getInt16Ty(Ctx), EncChar16Ptr, true);
	}

	Value *KeyIdxZext = IRB.CreateZExt(KeyIdx, IRB.getInt32Ty());
	Value *KeyCharZext = nullptr;
	if (!Entry->IsUTF16) {
		KeyCharZext = IRB.CreateZExt(KeyChar, IRB.getInt32Ty());
	} else {
		KeyCharZext = IRB.CreateZExt(KeyChar, IRB.getInt32Ty());
	}
	Value *Mul = IRB.CreateMul(KeyIdxZext, KeyCharZext);
	Value *BrKey = IRB.CreateAnd(Mul, IRB.getInt32(1));
	Value *BrCond = IRB.CreateICmpEQ(BrKey, IRB.getInt32(0));
	IRB.CreateCondBr(BrCond, LoopBr0, LoopBr1);

	IRB.SetInsertPoint(LoopBr0);
	Value *DecChar0 = nullptr;
	if (!Entry->IsUTF16) {
		Value *Tmp0 = IRB.CreateAdd(EncChar, LastDecrypted);
		Tmp0 = IRB.CreateXor(Tmp0, KeyChar);
		Tmp0 = IRB.CreateNot(Tmp0);
		DecChar0 = Tmp0;
	} else {
		Value *Tmp0 = IRB.CreateAdd(EncChar, LastDecrypted);
		Tmp0 = IRB.CreateXor(Tmp0, KeyChar);
		Tmp0 = IRB.CreateNot(Tmp0);
		DecChar0 = Tmp0;
	}
	IRB.CreateBr(LoopEnd);

	IRB.SetInsertPoint(LoopBr1);
	Value *DecChar1 = nullptr;
	if (!Entry->IsUTF16) {
		Value *Tmp1 = IRB.CreateSub(EncChar, LastDecrypted);
		Tmp1 = IRB.CreateXor(Tmp1, KeyChar);
		Tmp1 = IRB.CreateNeg(Tmp1);
		DecChar1 = Tmp1;
	} else {
		Value *Tmp1 = IRB.CreateSub(EncChar, LastDecrypted);
		Tmp1 = IRB.CreateXor(Tmp1, KeyChar);
		Tmp1 = IRB.CreateNeg(Tmp1);
		DecChar1 = Tmp1;
	}
	IRB.CreateBr(LoopEnd);

	IRB.SetInsertPoint(LoopEnd);
	PHINode *BrDecChar = IRB.CreatePHI(PlainEltTy, 2);
	BrDecChar->addIncoming(DecChar0, LoopBr0);
	BrDecChar->addIncoming(DecChar1, LoopBr1);
	Value *DecChar = IRB.CreateXor(BrDecChar, KeyChar);

	LastDecrypted->addIncoming(DecChar, LoopEnd);
	Value *DecCharPtr = IRB.CreateInBoundsGEP(PlainEltTy,
	                    PlainString, LoopCounter);
	IRB.CreateStore(DecChar, DecCharPtr);

	Value *NewCounter = IRB.CreateAdd(LoopCounter, IRB.getInt32(1), "", true, true);
	LoopCounter->addIncoming(NewCounter, LoopEnd);

	uint32_t DataSize = Entry->IsUTF16 ? static_cast<uint32_t>(Entry->Data16.size()) : static_cast<uint32_t>(Entry->Data.size());
	Value *Cond = IRB.CreateICmpEQ(NewCounter, IRB.getInt32(static_cast<uint32_t>(DataSize)));
	IRB.CreateCondBr(Cond, Exit, LoopBody);

	IRB.SetInsertPoint(Exit);
	IRB.CreateStore(IRB.getInt32(1), Entry->DecStatus);
	IRB.CreateRetVoid();

	return DecFunc;
}

/**
 * @brief 构建全局变量初始化函数
 * @param M LLVM模块
 * @param User 常量字符串用户
 * @return 构建的初始化函数
 */
Function *StringEncryption::buildInitFunction(Module *M, const StringEncryption::CSUser *User) {
	LLVMContext &Ctx = M->getContext();
	IRBuilder<> IRB(Ctx);
	FunctionType *FuncTy = FunctionType::get(Type::getVoidTy(Ctx), {User->DecGV->getType()}, false);
	Function *InitFunc =
	    Function::Create(FuncTy, GlobalValue::PrivateLinkage, "__global_variable_initializer_" + User->GV->getName(), M);

	auto ArgIt = InitFunc->arg_begin();
	Argument *thiz = ArgIt;


	AttrBuilder NoCaptureAttrBuilder{Ctx};
	NoCaptureAttrBuilder.addCapturesAttr(llvm::CaptureInfo(llvm::CaptureComponents::None));
	thiz->setName("this");
	thiz->addAttrs(NoCaptureAttrBuilder);

	BasicBlock *Enter = BasicBlock::Create(Ctx, "Enter", InitFunc);
	BasicBlock *InitBody = BasicBlock::Create(Ctx, "InitBody", InitFunc);
	BasicBlock *Exit = BasicBlock::Create(Ctx, "Exit", InitFunc);

	IRB.SetInsertPoint(Enter);
	Value *DecStatus = IRB.CreateLoad(Type::getInt32Ty(Ctx), User->DecStatus, "dec_status");
	Value *AlreadyInitialized = IRB.CreateICmpEQ(DecStatus, IRB.getInt32(1), "already_initialized");
	IRB.CreateCondBr(AlreadyInitialized, Exit, InitBody);

	IRB.SetInsertPoint(InitBody);
	Constant *Init = User->GV->getInitializer();
	lowerGlobalConstant(Init, IRB, User->DecGV, User->Ty);
	IRB.CreateStore(IRB.getInt32(1), User->DecStatus);
	IRB.CreateBr(Exit);

	IRB.SetInsertPoint(Exit);
	IRB.CreateRetVoid();
	return InitFunc;
}

/**
 * @brief 降低全局常量到运行时初始化
 * @param CV 常量值
 * @param IRB IR构建器
 * @param Ptr 目标指针
 * @param Ty 类型
 */
void StringEncryption::lowerGlobalConstant(Constant *CV, IRBuilder<> &IRB, Value *Ptr, Type *Ty) {
	if (isa<ConstantAggregateZero>(CV)) {
		IRB.CreateStore(CV, Ptr);
		return;
	}

	std::function<Constant *(Constant *)> remapLeafConstant = [&](Constant *Leaf) -> Constant * {
		if (!Leaf) {
			return Leaf;
		}
		if (auto *GV = dyn_cast<GlobalVariable>(Leaf)) {
			if (CSPEntry *Entry = resolveCSPEntryGlobal(GV)) {
				return Entry->DecGV;
			}
			if (CSUser *User = resolveCSUserGlobal(GV)) {
				return User->DecGV;
			}
			return Leaf;
		}
		if (auto *CE = dyn_cast<ConstantExpr>(Leaf)) {
			Constant *Base = cast<Constant>(CE->getOperand(0));
			Constant *NewBase = remapLeafConstant(Base);
			if (NewBase == Base) {
				return Leaf;
			}
			switch (CE->getOpcode()) {
			case Instruction::GetElementPtr: {
				SmallVector<Constant *, 4> Indices;
				for (unsigned i = 1; i < CE->getNumOperands(); ++i) {
					Indices.push_back(cast<Constant>(CE->getOperand(i)));
				}
				if (auto *NewGV = dyn_cast<GlobalVariable>(NewBase)) {
					return ConstantExpr::getGetElementPtr(NewGV->getValueType(), NewGV, Indices);
				}
				return Leaf;
			}
			case Instruction::BitCast:
				return ConstantExpr::getBitCast(NewBase, CE->getType());
			case Instruction::AddrSpaceCast:
				return ConstantExpr::getAddrSpaceCast(NewBase, CE->getType());
			default:
				return Leaf;
			}
		}
		return Leaf;
	};

	if (ConstantArray *CA = dyn_cast<ConstantArray>(CV)) {
		lowerGlobalConstantArray(CA, IRB, Ptr, Ty);
	} else if (ConstantStruct *CS = dyn_cast<ConstantStruct>(CV)) {
		lowerGlobalConstantStruct(CS, IRB, Ptr, Ty);
	} else {
		IRB.CreateStore(remapLeafConstant(CV), Ptr);
	}
}

/**
 * @brief 降低数组常量
 * @param CA 数组常量
 * @param IRB IR构建器
 * @param Ptr 目标指针
 * @param Ty 类型
 */
void StringEncryption::lowerGlobalConstantArray(ConstantArray *CA, IRBuilder<> &IRB, Value *Ptr, Type *Ty) {
	for (unsigned i = 0, e = CA->getNumOperands(); i != e; ++i) {
		Constant *CV = CA->getOperand(i);
		Value *GEP = IRB.CreateGEP(Ty,
		                           Ptr, {IRB.getInt32(0), IRB.getInt32(i)});
		lowerGlobalConstant(CV, IRB, GEP, CV->getType());
	}
}

/**
 * @brief 降低结构体常量
 * @param CS 结构体常量
 * @param IRB IR构建器
 * @param Ptr 目标指针
 * @param Ty 类型
 */
void StringEncryption::lowerGlobalConstantStruct(ConstantStruct *CS, IRBuilder<> &IRB, Value *Ptr, Type *Ty) {
	for (unsigned i = 0, e = CS->getNumOperands(); i != e; ++i) {
		Constant* CV = CS->getOperand(i);
		Value *GEP = IRB.CreateGEP(Ty,
		                           Ptr, {IRB.getInt32(0), IRB.getInt32(i)});
		lowerGlobalConstant(CV, IRB, GEP, CV->getType());
	}
}

void StringEncryption::collectPinnedEntries(Constant *CV, SmallPtrSetImpl<Constant *> &Visited) {
	if (!CV || !Visited.insert(CV).second) {
		return;
	}

	if (auto *GV = dyn_cast<GlobalVariable>(CV)) {
		auto Iter = CSPEntryMap.find(GV);
		if (Iter != CSPEntryMap.end()) {
			PinnedEntries.insert(Iter->second);
		}
	}

	for (unsigned i = 0; i < CV->getNumOperands(); ++i) {
		if (auto *Nested = dyn_cast<Constant>(CV->getOperand(i))) {
			collectPinnedEntries(Nested, Visited);
		}
	}
}

void StringEncryption::collectReferencedEntries(Constant *CV,
                                               SmallPtrSetImpl<Constant *> &Visited,
                                               SmallPtrSetImpl<CSPEntry *> &Entries) {
	if (!CV || !Visited.insert(CV).second) {
		return;
	}

	if (auto *GV = dyn_cast<GlobalVariable>(CV)) {
		if (CSPEntry *Entry = resolveCSPEntryGlobal(GV)) {
			Entries.insert(Entry);
		}
	}

	for (unsigned i = 0; i < CV->getNumOperands(); ++i) {
		if (auto *Nested = dyn_cast<Constant>(CV->getOperand(i))) {
			collectReferencedEntries(Nested, Visited, Entries);
		}
	}
}

void StringEncryption::scheduleWipeAtBlockEnd(BasicBlock *BB, GlobalVariable *GV) {
	if (!BB || !GV || !BB->getTerminator()) {
		return;
	}

	const Module *M = BB->getModule();
	if (!M) {
		return;
	}

	const DataLayout &DL = M->getDataLayout();
	uint64_t Size = DL.getTypeAllocSize(GV->getValueType());
	if (Size == 0) {
		return;
	}

	IRBuilder<> IRB(BB->getTerminator());
	Value *Buf = IRB.CreateBitCast(GV, PointerType::get(BB->getContext(), 0));
	IRB.CreateMemSet(Buf, IRB.getInt8(0), Size, MaybeAlign(GV->getAlignment()));

	if (CSPEntry *Entry = resolveCSPEntryGlobal(GV)) {
		if (Entry->DecStatus) {
			IRB.CreateStore(IRB.getInt32(0), Entry->DecStatus);
		}
		return;
	}
	if (CSUser *User = resolveCSUserGlobal(GV)) {
		if (User->DecStatus) {
			IRB.CreateStore(IRB.getInt32(0), User->DecStatus);
		}
	}
}

bool StringEncryption::shouldSkipBlockEndWipe(BasicBlock *BB, Instruction *InsertBefore) {
	if (!BB) {
		return true;
	}
	Instruction *Term = BB->getTerminator();
	if (!Term) {
		return true;
	}
	// If the current use is the terminator itself (most notably an invoke),
	// inserting the wipe "at block end" would place it before the use.
	if (InsertBefore && InsertBefore == Term) {
		return true;
	}
	if (Function *F = BB->getParent()) {
		if (F->getReturnType()->isPointerTy()) {
			return true;
		}
	}
	if (auto *Ret = dyn_cast<ReturnInst>(Term)) {
		if (Value *RetVal = Ret->getReturnValue()) {
			return RetVal->getType()->isPointerTy();
		}
	}
	return false;
}

/**
 * @brief 处理函数中的常量字符串使用
 * @param F 要处理的函数
 * @return 如果函数被修改返回true
 */
bool StringEncryption::processConstantStringUse(Function *F) {
	auto opt = ArgsOptions->toObfuscate(ArgsOptions->cseOpt(), F);
	if (!opt.isEnabled()) {
		return false;
	}
	LLVMContext &Ctx = F->getContext();
	LowerConstantExpr(*F);
	SmallPtrSet<GlobalVariable *, 16> LiveBuffers;
	bool Changed = false;

	auto getSafeInsertPoint = [](Instruction *Inst) -> Instruction * {
		if (!Inst) {
			return nullptr;
		}
		if (!Inst->isEHPad()) {
			return Inst;
		}
		BasicBlock *BB = Inst->getParent();
		if (!BB) {
			return Inst;
		}
		BasicBlock *PrevBB = BB->getPrevNode();
		if (!PrevBB) {
			return Inst;
		}
		return &*PrevBB->getFirstInsertionPt();
	};

	auto ensureUserReady = [&](Instruction *InsertBefore, BasicBlock *WipeBB, CSUser *User) {
		if (!InsertBefore || !WipeBB || !User || LiveBuffers.count(User->DecGV) > 0) {
			return;
		}
		IRBuilder<> IRB(getSafeInsertPoint(InsertBefore));
		auto UserEntriesIt = UserReferencedEntries.find(User);
		if (UserEntriesIt != UserReferencedEntries.end()) {
			for (CSPEntry *Entry : UserEntriesIt->second) {
				if (!Entry || LiveBuffers.count(Entry->DecGV) > 0) {
					continue;
				}
				Value *OutBuf = IRB.CreateBitCast(Entry->DecGV, PointerType::get(Ctx, 0));
				Value *Data = IRB.CreateInBoundsGEP(
				                  EncryptedStringTable->getValueType(),
				                  EncryptedStringTable,
				                  {IRB.getInt32(0), IRB.getInt32(Entry->Offset)});
				fixEH(IRB.CreateCall(Entry->DecFunc, {OutBuf, Data}));
				if (PinnedEntries.count(Entry) == 0 && !shouldSkipBlockEndWipe(WipeBB, InsertBefore)) {
					scheduleWipeAtBlockEnd(WipeBB, Entry->DecGV);
				}
				LiveBuffers.insert(Entry->DecGV);
			}
		}
		fixEH(IRB.CreateCall(User->InitFunc, {User->DecGV}));
		if (!shouldSkipBlockEndWipe(WipeBB, InsertBefore)) {
			scheduleWipeAtBlockEnd(WipeBB, User->DecGV);
		}
		LiveBuffers.insert(User->DecGV);
	};

	auto ensureEntryReady = [&](Instruction *InsertBefore, BasicBlock *WipeBB, CSPEntry *Entry) {
		if (!InsertBefore || !WipeBB || !Entry || LiveBuffers.count(Entry->DecGV) > 0) {
			return;
		}
		IRBuilder<> IRB(getSafeInsertPoint(InsertBefore));
		Value *OutBuf = IRB.CreateBitCast(Entry->DecGV, PointerType::get(Ctx, 0));
		Value *Data = IRB.CreateInBoundsGEP(
		                  EncryptedStringTable->getValueType(),
		                  EncryptedStringTable,
		                  {IRB.getInt32(0), IRB.getInt32(Entry->Offset)});
		fixEH(IRB.CreateCall(Entry->DecFunc, {OutBuf, Data}));
		if (PinnedEntries.count(Entry) == 0 && !shouldSkipBlockEndWipe(WipeBB, InsertBefore)) {
			scheduleWipeAtBlockEnd(WipeBB, Entry->DecGV);
		}
		LiveBuffers.insert(Entry->DecGV);
	};

	auto ensureGlobalEntriesReady = [&](Instruction *InsertBefore, BasicBlock *WipeBB,
	                                    GlobalVariable *GV) {
		if (!InsertBefore || !WipeBB || !GV) {
			return;
		}
		auto It = GlobalReferencedEntries.find(GV);
		if (It == GlobalReferencedEntries.end()) {
			return;
		}
		for (CSPEntry *Entry : It->second) {
			ensureEntryReady(InsertBefore, WipeBB, Entry);
		}
	};

	auto rewriteResolvedGlobal = [&](Instruction &Inst, Value *Carrier, GlobalVariable *From,
	                                 GlobalVariable *To) {
		if (!From || !To || From == To) {
			return;
		}
		if (auto *GEP = dyn_cast<GetElementPtrInst>(Carrier)) {
			GEP->replaceUsesOfWith(From, To);
		} else if (auto *CE = dyn_cast<ConstantExpr>(Carrier)) {
			Constant *NewConst = nullptr;
			switch (CE->getOpcode()) {
			case Instruction::GetElementPtr: {
				SmallVector<Constant *, 4> Indices;
				for (unsigned i = 1; i < CE->getNumOperands(); ++i) {
					Indices.push_back(cast<Constant>(CE->getOperand(i)));
				}
				NewConst = ConstantExpr::getGetElementPtr(To->getValueType(), To, Indices);
				break;
			}
			case Instruction::BitCast:
				NewConst = ConstantExpr::getBitCast(To, CE->getType());
				break;
			case Instruction::AddrSpaceCast:
				NewConst = ConstantExpr::getAddrSpaceCast(To, CE->getType());
				break;
			default:
				break;
			}
			if (NewConst) {
				Inst.replaceUsesOfWith(CE, NewConst);
			}
		} else {
			Inst.replaceUsesOfWith(From, To);
		}
		MaybeDeadGlobalVars.insert(From);
	};

	auto handleResolvedOperand = [&](Instruction &Inst, Value *Carrier,
	                                 Instruction *InsertBefore, BasicBlock *WipeBB) {
		GlobalVariable *GV = extractReferencedGlobal(Carrier);
		if (!GV) {
			return false;
		}

		ensureGlobalEntriesReady(InsertBefore, WipeBB, GV);

		if (CSUser *User = resolveCSUserGlobal(GV)) {
			if (F == User->InitFunc) {
				return false;
			}
			ensureUserReady(InsertBefore, WipeBB, User);
			rewriteResolvedGlobal(Inst, Carrier, GV, User->DecGV);
			return true;
		}

		if (CSPEntry *Entry = resolveCSPEntryGlobal(GV)) {
			ensureEntryReady(InsertBefore, WipeBB, Entry);
			rewriteResolvedGlobal(Inst, Carrier, GV, Entry->DecGV);
			return true;
		}

		if (auto *DirectGV = dyn_cast<GlobalVariable>(Carrier)) {
			if (CSPEntry *PointedEntry = getPointedCSPEntry(DirectGV)) {
				if (isIRObfuscationDebugEnabled()) {
					errs() << "[DEBUG] CSE: Found pointer to encrypted string: " << DirectGV->getName()
					       << " -> " << PointedEntry->DecGV->getName() << "\n";
				}
				ensureEntryReady(InsertBefore, WipeBB, PointedEntry);
				return true;
			}
			if (CSUser *PointedUser = getPointedCSUser(DirectGV)) {
				if (isIRObfuscationDebugEnabled()) {
					errs() << "[DEBUG] CSE: Found pointer to CSUser string: " << DirectGV->getName()
					       << " -> " << PointedUser->DecGV->getName() << "\n";
				}
				ensureUserReady(InsertBefore, WipeBB, PointedUser);
				return true;
			}
		}

		return false;
	};

	for (BasicBlock &BB : *F) {
		LiveBuffers.clear();
		Instruction *BlockInsertPoint = &*BB.getFirstInsertionPt();
		for (Instruction &Inst : BB) {
			if (PHINode *PHI = dyn_cast<PHINode>(&Inst)) {
				for (unsigned int i = 0; i < PHI->getNumIncomingValues(); ++i) {
					Changed |= handleResolvedOperand(Inst, PHI->getIncomingValue(i),
					                                 BlockInsertPoint, &BB);
				}
			} else {
				for (User::op_iterator op = Inst.op_begin(); op != Inst.op_end(); ++op) {
					Changed |= handleResolvedOperand(Inst, *op, &Inst, &BB);
				}
			}
		}
	}
	return Changed;
}

/**
 * @brief 收集使用常量字符串的全局变量
 * @param CString 常量字符串全局变量
 * @param Users 输出的用户集合
 */
void StringEncryption::collectConstantStringUser(GlobalVariable *CString, std::set<GlobalVariable *> &Users) {
	SmallPtrSet<Value *, 16> Visited;
	SmallVector<Value *, 16> ToVisit;

	ToVisit.push_back(CString);
	while (!ToVisit.empty()) {
		Value *V = ToVisit.pop_back_val();
		if (Visited.count(V) > 0)
			continue;
		Visited.insert(V);
		for (Value *User : V->users()) {
			if (auto *GV = dyn_cast<GlobalVariable>(User)) {
				Users.insert(GV);
			} else {
				ToVisit.push_back(User);
			}
		}
	}
}

GlobalVariable *StringEncryption::extractReferencedGlobal(Value *V) {
	if (auto *GV = dyn_cast_or_null<GlobalVariable>(V)) {
		return GV;
	}

	if (auto *GEP = dyn_cast<GetElementPtrInst>(V)) {
		return dyn_cast<GlobalVariable>(GEP->getPointerOperand());
	}

	if (auto *CE = dyn_cast<ConstantExpr>(V)) {
		switch (CE->getOpcode()) {
		case Instruction::GetElementPtr:
		case Instruction::BitCast:
		case Instruction::AddrSpaceCast:
			return extractReferencedGlobal(CE->getOperand(0));
		default:
			break;
		}
	}

	return nullptr;
}

StringEncryption::CSPEntry *StringEncryption::resolveCSPEntryGlobal(GlobalVariable *GV) {
	if (!GV) {
		return nullptr;
	}

	auto Iter = CSPEntryMap.find(GV);
	if (Iter != CSPEntryMap.end()) {
		return Iter->second;
	}

	auto DecIter = DecryptedCSPEntryMap.find(GV);
	if (DecIter != DecryptedCSPEntryMap.end()) {
		return DecIter->second;
	}

	return nullptr;
}

StringEncryption::CSUser *StringEncryption::resolveCSUserGlobal(GlobalVariable *GV) {
	if (!GV) {
		return nullptr;
	}

	auto Iter = CSUserMap.find(GV);
	if (Iter != CSUserMap.end()) {
		return Iter->second;
	}

	auto DecIter = DecryptedCSUserMap.find(GV);
	if (DecIter != DecryptedCSUserMap.end()) {
		return DecIter->second;
	}

	return nullptr;
}

/**
 * @brief 获取全局变量指向的解密后的全局变量
 * @param GV 全局变量（指针类型）
 * @return 如果该全局变量的初始化器指向加密的字符串，返回对应的解密后全局变量；否则返回nullptr
 */
GlobalVariable *StringEncryption::getPointedDecryptedGlobal(GlobalVariable *GV) {
	if (!GV->hasInitializer())
		return nullptr;
	
	Constant *Init = GV->getInitializer();
	if (!Init)
		return nullptr;
	
	if (GlobalVariable *StrGV = extractReferencedGlobal(Init)) {
		if (CSPEntry *Entry = resolveCSPEntryGlobal(StrGV)) {
			return Entry->DecGV;
		}
		if (CSUser *User = resolveCSUserGlobal(StrGV)) {
			return User->DecGV;
		}
	}
	
	return nullptr;
}

/**
 * @brief 获取全局变量指向的CSPEntry
 * @param GV 全局变量（指针类型）
 * @return 如果该全局变量的初始化器指向加密的字符串，返回对应的CSPEntry；否则返回nullptr
 */
StringEncryption::CSPEntry *StringEncryption::getPointedCSPEntry(GlobalVariable *GV) {
	if (!GV->hasInitializer())
		return nullptr;
	
	Constant *Init = GV->getInitializer();
	if (!Init)
		return nullptr;
	
	if (GlobalVariable *StrGV = extractReferencedGlobal(Init)) {
		return resolveCSPEntryGlobal(StrGV);
	}
	
	return nullptr;
}

/**
 * @brief 获取全局变量指向的CSUser
 * @param GV 全局变量（指针类型）
 * @return 如果该全局变量的初始化器指向加密的字符串，返回对应的CSUser；否则返回nullptr
 */
StringEncryption::CSUser *StringEncryption::getPointedCSUser(GlobalVariable *GV) {
	if (!GV->hasInitializer())
		return nullptr;
	
	Constant *Init = GV->getInitializer();
	if (!Init)
		return nullptr;
	
	if (GlobalVariable *StrGV = extractReferencedGlobal(Init)) {
		return resolveCSUserGlobal(StrGV);
	}
	
	return nullptr;
}

/**
 * @brief 检查全局变量是否适合加密
 * @param GV 要检查的全局变量
 * @return 如果适合加密返回true
 */
bool StringEncryption::isValidToEncrypt(GlobalVariable *GV) {
	if (GV->isConstant() && GV->hasInitializer()) {
		StringRef Name = GV->getName();
		if (Name.contains("gv_code_seg") || Name.contains("gv_data_seg") ||
		    Name.contains("vm_code_seg") || Name.contains("vm_data_seg") ||
		    Name.contains("scase_code_seg") || Name.contains("scase_data_seg")) {
			return false;
		}
		return GV->getInitializer() != nullptr;
	} else {
		return false;
	}
}

/**
 * @brief 删除未使用的全局变量
 */
void StringEncryption::deleteUnusedGlobalVariable() {
	for (GlobalVariable *GV : MaybeDeadGlobalVars) {
		if (!GV->hasLocalLinkage() && !GV->hasPrivateLinkage()) {
			continue;
		}

		GV->removeDeadConstantUsers();
		
		if (GV->hasInitializer()) {
			Constant *Init = GV->getInitializer();
			GV->setInitializer(nullptr);
			if (isSafeToDestroyConstant(Init))
				Init->destroyConstant();
		}
		
		if (GV->use_empty()) {
			GV->eraseFromParent();
		}
	}
}

/**
 * @brief 创建字符串加密Pass
 * @param argsOptions 混淆选项配置对象
 * @return 字符串加密Pass实例
 */
ModulePass *llvm::createStringEncryptionPass(ObfuscationOptions *argsOptions) {
	return new StringEncryption(argsOptions);
}

INITIALIZE_PASS(StringEncryption, "string-encryption", "Enable IR String Encryption", false, false)
