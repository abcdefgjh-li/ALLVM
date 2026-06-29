//===- HarmlessCrypto.h - 独立加密实现 (无 OpenSSL 依赖) -----===//
//
//                     The LLVM Compiler Infrastructure
//
// 移植自 hARMless，使用自实现加密算法
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_HARMLESS_CRYPTO_H
#define LLVM_TRANSFORMS_OBFUSCATION_HARMLESS_CRYPTO_H

#include <cstdint>
#include <cstring>

namespace llvm {
namespace harmless {

//===----------------------------------------------------------------------===//
// CRC32
//===----------------------------------------------------------------------===//

inline uint32_t crc32(const uint8_t* data, size_t len) {
    static const uint32_t table[256] = {
        0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
        0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
        0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
        0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
        0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
        0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
        0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
        0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
        0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
        0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
        0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
        0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
        0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
        0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
        0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
        0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
        0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
        0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
        0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
        0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
        0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
        0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
        0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
        0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
        0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
        0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
        0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
        0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
        0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
        0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
        0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
        0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
        0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
        0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
        0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
        0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
        0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
        0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
        0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
        0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
        0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
        0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
        0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
    };
    
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        crc = table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFF;
}

//===----------------------------------------------------------------------===//
// RC4
//===----------------------------------------------------------------------===//

struct RC4Context {
    uint8_t S[256];
    uint8_t i, j;
};

inline void rc4_init(RC4Context* ctx, const uint8_t* key, size_t key_len) {
    for (int i = 0; i < 256; i++) {
        ctx->S[i] = static_cast<uint8_t>(i);
    }
    
    uint8_t j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + ctx->S[i] + key[i % key_len]) & 0xFF;
        uint8_t tmp = ctx->S[i];
        ctx->S[i] = ctx->S[j];
        ctx->S[j] = tmp;
    }
    
    ctx->i = 0;
    ctx->j = 0;
}

inline void rc4_crypt(RC4Context* ctx, uint8_t* data, size_t len) {
    for (size_t n = 0; n < len; n++) {
        ctx->i = (ctx->i + 1) & 0xFF;
        ctx->j = (ctx->j + ctx->S[ctx->i]) & 0xFF;
        
        uint8_t tmp = ctx->S[ctx->i];
        ctx->S[ctx->i] = ctx->S[ctx->j];
        ctx->S[ctx->j] = tmp;
        
        data[n] ^= ctx->S[(ctx->S[ctx->i] + ctx->S[ctx->j]) & 0xFF];
    }
}

inline void rc4_encrypt_decrypt(const uint8_t* key, size_t key_len, uint8_t* data, size_t len) {
    RC4Context ctx;
    rc4_init(&ctx, key, key_len);
    rc4_crypt(&ctx, data, len);
}

//===----------------------------------------------------------------------===//
// ChaCha20
//===----------------------------------------------------------------------===//

inline void chacha20_quarter(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d) {
    a += b; d ^= a; d = (d << 16) | (d >> 16);
    c += d; b ^= c; b = (b << 12) | (b >> 20);
    a += b; d ^= a; d = (d << 8) | (d >> 24);
    c += d; b ^= c; b = (b << 7) | (b >> 25);
}

inline void chacha20_block(const uint8_t* key, const uint8_t* nonce, uint32_t counter, uint8_t* output) {
    static const char constants[16] = "expand 32-byte k";
    
    uint32_t state[16];
    
    state[0] = *reinterpret_cast<const uint32_t*>(constants);
    state[1] = *reinterpret_cast<const uint32_t*>(constants + 4);
    state[2] = *reinterpret_cast<const uint32_t*>(constants + 8);
    state[3] = *reinterpret_cast<const uint32_t*>(constants + 12);
    
    for (int i = 0; i < 8; i++) {
        state[4 + i] = *reinterpret_cast<const uint32_t*>(key + i * 4);
    }
    
    state[12] = counter;
    for (int i = 0; i < 3; i++) {
        state[13 + i] = *reinterpret_cast<const uint32_t*>(nonce + i * 4);
    }
    
    uint32_t working[16];
    std::memcpy(working, state, sizeof(working));
    
    for (int i = 0; i < 10; i++) {
        chacha20_quarter(working[0], working[4], working[8], working[12]);
        chacha20_quarter(working[1], working[5], working[9], working[13]);
        chacha20_quarter(working[2], working[6], working[10], working[14]);
        chacha20_quarter(working[3], working[7], working[11], working[15]);
        chacha20_quarter(working[0], working[5], working[10], working[15]);
        chacha20_quarter(working[1], working[6], working[11], working[12]);
        chacha20_quarter(working[2], working[7], working[8], working[13]);
        chacha20_quarter(working[3], working[4], working[9], working[14]);
    }
    
    for (int i = 0; i < 16; i++) {
        working[i] += state[i];
        *reinterpret_cast<uint32_t*>(output + i * 4) = working[i];
    }
}

inline void chacha20_encrypt(uint8_t* data, size_t len, const uint8_t* key, const uint8_t* nonce) {
    uint32_t counter = 0;
    uint8_t block[64];
    
    for (size_t offset = 0; offset < len; offset += 64) {
        chacha20_block(key, nonce, counter, block);
        
        size_t block_len = (len - offset < 64) ? (len - offset) : 64;
        for (size_t i = 0; i < block_len; i++) {
            data[offset + i] ^= block[i];
        }
        counter++;
    }
}

//===----------------------------------------------------------------------===//
// AES-256 (简化实现)
//===----------------------------------------------------------------------===//

inline void aes256_encrypt(uint8_t* data, size_t len, const uint8_t* key) {
    static const uint8_t sbox[256] = {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };
    
    size_t num_blocks = len / 16;
    
    for (size_t block = 0; block < num_blocks; block++) {
        size_t offset = block * 16;
        
        for (size_t round = 0; round < 14; round++) {
            for (size_t i = 0; i < 16; i++) {
                size_t key_idx = (round * 16 + i) % 32;
                data[offset + i] ^= key[key_idx];
                data[offset + i] = sbox[data[offset + i]];
            }
        }
    }
}

//===----------------------------------------------------------------------===//
// 多层加密
//===----------------------------------------------------------------------===//

inline void multi_layer_encrypt(uint8_t* data, size_t len, 
                                const uint8_t* key1, const uint8_t* key2, 
                                const uint8_t* key3, const uint8_t* nonce) {
    aes256_encrypt(data, len, key1);
    chacha20_encrypt(data, len, key2, nonce);
    rc4_encrypt_decrypt(key3, 32, data, len);
}

inline void multi_layer_decrypt(uint8_t* data, size_t len,
                                const uint8_t* key1, const uint8_t* key2,
                                const uint8_t* key3, const uint8_t* nonce) {
    rc4_encrypt_decrypt(key3, 32, data, len);
    chacha20_encrypt(data, len, key2, nonce);
    aes256_encrypt(data, len, key1);
}

} // namespace harmless
} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_HARMLESS_CRYPTO_H
