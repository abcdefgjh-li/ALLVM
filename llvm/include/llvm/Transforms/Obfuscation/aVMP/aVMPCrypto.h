#ifndef LLVM_LIB_TRANSFORMS_OBFUSCATION_AVMPCRYPTO_H
#define LLVM_LIB_TRANSFORMS_OBFUSCATION_AVMPCRYPTO_H

#include "llvm/IR/Function.h"
#include <cstdint>

uint64_t deriveVMFunctionKey(const llvm::Function &F);
uint64_t deriveBBToken(uint64_t FunctionKey, uint32_t BBOffset);
uint32_t deriveBBTag(uint64_t FunctionKey, uint32_t BBOffset);
uint64_t deriveOpcodeSeed(uint64_t BBToken);
uint64_t deriveVMSeed(uint64_t FunctionKey, uint64_t BBToken);
uint64_t deriveChainSeed(uint64_t FunctionKey, uint64_t BBToken,
                         uint32_t BBOffset);
void deriveChaCha20Material(uint64_t FunctionKey, uint64_t PayloadSeed,
                            uint64_t ChainSeed, uint32_t BBOffset,
                            uint32_t KeyWords[8], uint32_t NonceWords[3]);
void chacha20Block(const uint32_t KeyWords[8], uint32_t Counter,
                   const uint32_t NonceWords[3], uint8_t Out[64]);
uint8_t chacha20ByteAt(uint64_t FunctionKey, uint64_t PayloadSeed,
                       uint64_t ChainSeed, uint32_t BBOffset,
                       uint32_t PayloadIndex);

#endif
