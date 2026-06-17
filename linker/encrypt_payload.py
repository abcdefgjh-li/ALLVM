#!/usr/bin/env python3
"""
ChaCha20 加密 payload 生成器（无压缩）
1. 读取 ELF 文件
2. ChaCha20 加密
3. 生成 payload.h
"""

import sys
import struct
import os

def rotl32(v, n):
    return ((v << n) & 0xFFFFFFFF) | (v >> (32 - n))

def quarter_round(state, a, b, c, d):
    state[a] = (state[a] + state[b]) & 0xFFFFFFFF
    state[d] ^= state[a]; state[d] = rotl32(state[d], 16)
    state[c] = (state[c] + state[d]) & 0xFFFFFFFF
    state[b] ^= state[c]; state[b] = rotl32(state[b], 12)
    state[a] = (state[a] + state[b]) & 0xFFFFFFFF
    state[d] ^= state[a]; state[d] = rotl32(state[d], 8)
    state[c] = (state[c] + state[d]) & 0xFFFFFFFF
    state[b] ^= state[c]; state[b] = rotl32(state[b], 7)

def chacha20_block(key, counter, nonce):
    constants = [0x61707865, 0x3320646e, 0x79622d32, 0x6b206574]
    state = constants[:] + list(key) + [counter] + list(nonce)
    working = state[:]
    for _ in range(10):
        quarter_round(working, 0, 4, 8, 12)
        quarter_round(working, 1, 5, 9, 13)
        quarter_round(working, 2, 6, 10, 14)
        quarter_round(working, 3, 7, 11, 15)
        quarter_round(working, 0, 5, 10, 15)
        quarter_round(working, 1, 6, 11, 12)
        quarter_round(working, 2, 7, 8, 13)
        quarter_round(working, 3, 4, 9, 14)
    return b''.join(struct.pack('<I', (working[i] + state[i]) & 0xFFFFFFFF) for i in range(16))

def chacha20_encrypt(data, key_bytes, nonce_bytes, counter=1):
    key = list(struct.unpack('<8I', key_bytes))
    nonce = list(struct.unpack('<3I', nonce_bytes))
    result = bytearray()
    offset = 0
    while offset < len(data):
        keystream = chacha20_block(key, counter, nonce)
        counter += 1
        chunk_size = min(64, len(data) - offset)
        for i in range(chunk_size):
            result.append(data[offset + i] ^ keystream[i])
        offset += chunk_size
    return bytes(result)

def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <input_elf> <output_header> [variable_name]")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]
    var_name = sys.argv[3] if len(sys.argv) > 3 else "payload_data"

    key_hex = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    nonce_hex = "000000000000004a00000000"
    key_bytes = bytes.fromhex(key_hex)
    nonce_bytes = bytes.fromhex(nonce_hex)

    with open(input_file, 'rb') as f:
        raw_data = f.read()

    raw_size = len(raw_data)
    print(f"Raw size: {raw_size} bytes")

    encrypted = chacha20_encrypt(raw_data, key_bytes, nonce_bytes)
    encrypted_size = len(encrypted)
    print(f"Encrypted size: {encrypted_size} bytes")

    with open(output_file, 'w') as f:
        f.write(f"""/*
 * ChaCha20 加密 payload (无压缩)
 * 原始大小: {raw_size} bytes
 * 加密方式: ChaCha20
 */

#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <stdint.h>
#include <stddef.h>

#define PAYLOAD_RAW_SIZE {raw_size}u

#define CHACHA20_KEY {{ \\
    0x{key_hex[0:2]}, 0x{key_hex[2:4]}, 0x{key_hex[4:6]}, 0x{key_hex[6:8]}, \\
    0x{key_hex[8:10]}, 0x{key_hex[10:12]}, 0x{key_hex[12:14]}, 0x{key_hex[14:16]}, \\
    0x{key_hex[16:18]}, 0x{key_hex[18:20]}, 0x{key_hex[20:22]}, 0x{key_hex[22:24]}, \\
    0x{key_hex[24:26]}, 0x{key_hex[26:28]}, 0x{key_hex[28:30]}, 0x{key_hex[30:32]}, \\
    0x{key_hex[32:34]}, 0x{key_hex[34:36]}, 0x{key_hex[36:38]}, 0x{key_hex[38:40]}, \\
    0x{key_hex[40:42]}, 0x{key_hex[42:44]}, 0x{key_hex[44:46]}, 0x{key_hex[46:48]}, \\
    0x{key_hex[48:50]}, 0x{key_hex[50:52]}, 0x{key_hex[52:54]}, 0x{key_hex[54:56]}, \\
    0x{key_hex[56:58]}, 0x{key_hex[58:60]}, 0x{key_hex[60:62]}, 0x{key_hex[62:64]}  \\
}}

#define CHACHA20_NONCE {{ \\
    0x{nonce_hex[0:2]}, 0x{nonce_hex[2:4]}, 0x{nonce_hex[4:6]}, 0x{nonce_hex[6:8]}, \\
    0x{nonce_hex[8:10]}, 0x{nonce_hex[10:12]}, 0x{nonce_hex[12:14]}, 0x{nonce_hex[14:16]}, \\
    0x{nonce_hex[16:18]}, 0x{nonce_hex[18:20]}, 0x{nonce_hex[20:22]}, 0x{nonce_hex[22:24]}  \\
}}

static const uint8_t {var_name}[] = {{
""")
        for i in range(0, encrypted_size, 16):
            chunk = encrypted[i:i+16]
            hex_str = ', '.join(f'0x{b:02x}' for b in chunk)
            f.write(f'    {hex_str},\n')
        f.write(f"""}};

static const size_t {var_name}_size = {encrypted_size};

#endif // PAYLOAD_H
""")

    print(f"Generated {output_file}, size: {os.path.getsize(output_file)} bytes")

if __name__ == '__main__':
    main()
