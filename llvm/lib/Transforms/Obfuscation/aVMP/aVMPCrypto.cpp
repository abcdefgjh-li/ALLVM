#include "llvm/Transforms/Obfuscation/aVMP/aVMPCrypto.h"
#include <chrono>
#include <string>

using namespace llvm;

static uint32_t vmNonZero32(uint32_t Value, uint32_t Fallback) {
    return Value ? Value : Fallback;
}

static uint64_t vmRotl64(uint64_t Value, unsigned Shift) {
    return (Value << Shift) | (Value >> (64U - Shift));
}

static uint64_t vmNonZero64(uint64_t Value, uint64_t Fallback) {
    return Value ? Value : Fallback;
}

static uint64_t vmMix64(uint64_t Value) {
    Value ^= Value >> 30;
    Value *= 0xbf58476d1ce4e5b9ULL;
    Value ^= Value >> 27;
    Value *= 0x94d049bb133111ebULL;
    Value ^= Value >> 31;
    return Value;
}

static uint64_t splitmix64Next(uint64_t &State) {
    State += 0x9e3779b97f4a7c15ULL;
    return vmMix64(State);
}

static uint64_t getVMBuildSalt() {
    static uint64_t Salt = vmNonZero64(
        vmMix64((uint64_t)std::chrono::high_resolution_clock::now()
                    .time_since_epoch()
                    .count() ^
                (uint64_t)(uintptr_t)&Salt),
        0x7a5f4d3c2b190817ULL);
    return Salt;
}

uint64_t deriveVMFunctionKey(const Function &F) {
    uint64_t Hash = 1469598103934665603ULL;
    std::string Name = F.getName().str();
    for (unsigned char C : Name) {
        Hash ^= C;
        Hash *= 1099511628211ULL;
    }
    Hash ^= (uint64_t)F.arg_size() * 0x9e3779b97f4a7c15ULL;
    Hash ^= (uint64_t)F.size() * 0xbf58476d1ce4e5b9ULL;
    Hash ^= getVMBuildSalt();
    return vmNonZero64(vmMix64(Hash), 0x51f15e5ULL);
}

uint64_t deriveBBToken(uint64_t FunctionKey, uint32_t BBOffset) {
    uint64_t Mixed = FunctionKey ^ ((uint64_t)BBOffset * 0x45d9f3b45d9f3bULL) ^
                     0x9e3779b97f4a7c15ULL;
    return vmNonZero64(vmMix64(Mixed), 0x13579bdf2468ace1ULL);
}

uint32_t deriveBBTag(uint64_t FunctionKey, uint32_t BBOffset) {
    uint64_t Token = deriveBBToken(FunctionKey, BBOffset);
    uint64_t Mixed = Token ^ vmRotl64(FunctionKey ^ 0x85ebca6b27d4eb2dULL, 11) ^
                     ((uint64_t)BBOffset * 0x27d4eb2f165667c5ULL) ^
                     0xc2b2ae3d27d4eb4fULL;
    return vmNonZero32((uint32_t)vmMix64(Mixed), 0x2468ace1u);
}

uint64_t deriveOpcodeSeed(uint64_t BBToken) {
    return vmNonZero64(vmMix64(BBToken ^ 0xa5a5f00d1badb002ULL),
                       0xa5a5f00d1badb002ULL);
}

uint64_t deriveVMSeed(uint64_t FunctionKey, uint64_t BBToken) {
    uint64_t Mixed = BBToken ^ vmRotl64(FunctionKey, 7) ^ 0x3c6ef372fe94f82bULL;
    return vmNonZero64(vmMix64(Mixed), 0x3c6ef372fe94f82bULL);
}

uint64_t deriveChainSeed(uint64_t FunctionKey, uint64_t BBToken,
                         uint32_t BBOffset) {
    uint64_t Mixed = vmRotl64(BBToken, 13) ^ FunctionKey ^
                     ((uint64_t)BBOffset * 0x165667b19e3779f9ULL) ^
                     0xd1b54a32d192ed03ULL;
    return vmNonZero64(vmMix64(Mixed), 0xd1b54a32d192ed03ULL);
}

static void store32LE(uint8_t *Dst, uint32_t Value) {
    Dst[0] = (uint8_t)(Value & 0xFF);
    Dst[1] = (uint8_t)((Value >> 8) & 0xFF);
    Dst[2] = (uint8_t)((Value >> 16) & 0xFF);
    Dst[3] = (uint8_t)((Value >> 24) & 0xFF);
}

static uint32_t chachaRotl32(uint32_t Value, unsigned Shift) {
    return (Value << Shift) | (Value >> (32U - Shift));
}

#define CHACHA20_QR(a, b, c, d) \
    a += b; d ^= a; d = chachaRotl32(d, 16); \
    c += d; b ^= c; b = chachaRotl32(b, 12); \
    a += b; d ^= a; d = chachaRotl32(d, 8); \
    c += d; b ^= c; b = chachaRotl32(b, 7)

void chacha20Block(const uint32_t KeyWords[8], uint32_t Counter,
                   const uint32_t NonceWords[3], uint8_t Out[64]) {
    static const uint32_t Constants[4] = {
        0x61707865U, 0x3320646eU, 0x79622d32U, 0x6b206574U
    };
    uint32_t State[16];
    uint32_t Working[16];
    State[0] = Constants[0];
    State[1] = Constants[1];
    State[2] = Constants[2];
    State[3] = Constants[3];
    for (int I = 0; I < 8; ++I) {
        State[4 + I] = KeyWords[I];
    }
    State[12] = Counter;
    State[13] = NonceWords[0];
    State[14] = NonceWords[1];
    State[15] = NonceWords[2];
    for (int I = 0; I < 16; ++I) {
        Working[I] = State[I];
    }
    for (int Round = 0; Round < 10; ++Round) {
        CHACHA20_QR(Working[0], Working[4], Working[8], Working[12]);
        CHACHA20_QR(Working[1], Working[5], Working[9], Working[13]);
        CHACHA20_QR(Working[2], Working[6], Working[10], Working[14]);
        CHACHA20_QR(Working[3], Working[7], Working[11], Working[15]);
        CHACHA20_QR(Working[0], Working[5], Working[10], Working[15]);
        CHACHA20_QR(Working[1], Working[6], Working[11], Working[12]);
        CHACHA20_QR(Working[2], Working[7], Working[8], Working[13]);
        CHACHA20_QR(Working[3], Working[4], Working[9], Working[14]);
    }
    for (int I = 0; I < 16; ++I) {
        Working[I] += State[I];
        store32LE(Out + I * 4, Working[I]);
    }
}

#undef CHACHA20_QR

void deriveChaCha20Material(uint64_t FunctionKey, uint64_t PayloadSeed,
                            uint64_t ChainSeed, uint32_t BBOffset,
                            uint32_t KeyWords[8], uint32_t NonceWords[3]) {
    uint64_t State = FunctionKey ^ vmRotl64(PayloadSeed, 17) ^
                     vmRotl64(ChainSeed, 29) ^
                     ((uint64_t)BBOffset * 0x9e3779b97f4a7c15ULL) ^
                     0x6a09e667f3bcc909ULL;
    for (int I = 0; I < 4; ++I) {
        uint64_t Word = splitmix64Next(State);
        KeyWords[I * 2] = (uint32_t)(Word & 0xFFFFFFFFU);
        KeyWords[I * 2 + 1] = (uint32_t)(Word >> 32);
    }
    for (int I = 0; I < 3; ++I) {
        NonceWords[I] = (uint32_t)splitmix64Next(State);
    }
}

uint8_t chacha20ByteAt(uint64_t FunctionKey, uint64_t PayloadSeed,
                       uint64_t ChainSeed, uint32_t BBOffset,
                       uint32_t PayloadIndex) {
    uint32_t KeyWords[8];
    uint32_t NonceWords[3];
    uint8_t Block[64];
    uint32_t BlockIndex = PayloadIndex / 64U;
    uint32_t BlockOffset = PayloadIndex % 64U;
    deriveChaCha20Material(FunctionKey, PayloadSeed, ChainSeed, BBOffset,
                           KeyWords, NonceWords);
    chacha20Block(KeyWords, BlockIndex, NonceWords, Block);
    return Block[BlockOffset];
}
