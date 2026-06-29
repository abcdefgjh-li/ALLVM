#include "LicenseGenerator.h"
#include <cstring>

namespace LicenseGenerator {

namespace {
static const uint32_t OLLVM_CHACHA_CONST[4] = {
    0x6F6C6C76, 0x6D2D6B65, 0x79323178, 0x21212121
};
static const uint8_t OLLVM_KEY[32] = {
    0x4F,0x4C,0x4C,0x56,0x4D,0x4B,0x45,0x59,
    0x32,0x31,0x58,0x21,0x21,0x21,0x21,0x21,
    0xA3,0xF9,0x7B,0x2D,0xC1,0x88,0xE4,0x9F,
    0x6E,0x01,0xDA,0x42,0xB3,0x55,0x71,0x8C
};
static const uint8_t OLLVM_NONCE[12] = {
    0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,
    0x11,0x22,0x33,0x44
};
static const uint64_t CHECKSUM_MAGIC = 0xA5A5A5A5A5A5A5A5ULL;
static const int OLLVM_CHACHA_ROUNDS = 12;

#define OLLVM_QR(a,b,c,d) do { \
    a+=b; d^=a; d=(d<<13)|(d>>19); \
    c+=d; b^=c; b=(b<<9)|(b>>23); \
    a+=b; d^=a; d=(d<<5)|(d>>27); \
    c+=d; b^=c; b=(b<<11)|(b>>21); \
} while(0)

static void ollvm_chacha20_block(uint8_t out[64], uint32_t counter) {
    uint32_t x[16], z[16];
    x[0]=OLLVM_CHACHA_CONST[0]; x[1]=OLLVM_CHACHA_CONST[1];
    x[2]=OLLVM_CHACHA_CONST[2]; x[3]=OLLVM_CHACHA_CONST[3];
    for(int i=0;i<8;i++) std::memcpy(&x[4+i],OLLVM_KEY+i*4,4);
    x[12]=counter;
    for(int i=0;i<3;i++) std::memcpy(&x[13+i],OLLVM_NONCE+i*4,4);
    std::memcpy(z,x,sizeof(z));
    for(int i=0;i<OLLVM_CHACHA_ROUNDS;i++){
        OLLVM_QR(z[0],z[4],z[8],z[12]); OLLVM_QR(z[1],z[5],z[9],z[13]);
        OLLVM_QR(z[2],z[6],z[10],z[14]); OLLVM_QR(z[3],z[7],z[11],z[15]);
        OLLVM_QR(z[0],z[5],z[10],z[15]); OLLVM_QR(z[1],z[6],z[11],z[12]);
        OLLVM_QR(z[2],z[7],z[8],z[13]); OLLVM_QR(z[3],z[4],z[9],z[14]);
    }
    for(int i=0;i<16;i++){
        z[i]+=x[i];
        out[i*4]=(uint8_t)z[i]; out[i*4+1]=(uint8_t)(z[i]>>8);
        out[i*4+2]=(uint8_t)(z[i]>>16); out[i*4+3]=(uint8_t)(z[i]>>24);
    }
}
}

QString generateLicenseKey(uint64_t timestamp) {
    uint64_t checksum = timestamp ^ CHECKSUM_MAGIC;
    uint8_t plain[16];
    std::memcpy(plain,&timestamp,8);
    std::memcpy(plain+8,&checksum,8);
    uint8_t keystream[64];
    size_t offset=0;
    for(uint32_t c=0;offset<16;c++){
        ollvm_chacha20_block(keystream,c);
        size_t chunk=(16-offset<64)?16-offset:64;
        for(size_t i=0;i<chunk;i++) plain[offset+i]^=keystream[i];
        offset+=chunk;
    }
    static const char hex[]="0123456789ABCDEF";
    QString result; result.reserve(32);
    for(int i=0;i<16;i++){
        result.push_back(hex[plain[i]>>4]);
        result.push_back(hex[plain[i]&0x0F]);
    }
    return result;
}

}
