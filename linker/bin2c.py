#!/usr/bin/env python3
"""
仅用于软件保护研究/教育目的：将二进制文件转换为 C 数组
"""
import argparse
import sys

def main():
    parser = argparse.ArgumentParser(description="Convert binary to C array")
    parser.add_argument("input", help="Input binary file")
    parser.add_argument("output", help="Output C header file")
    parser.add_argument("name", help="Array name")
    args = parser.parse_args()

    with open(args.input, "rb") as f:
        data = f.read()

    with open(args.output, "w") as f:
        f.write(f"// Auto-generated from {args.input}\n")
        f.write("// 仅用于软件保护研究/教育目的\n\n")
        f.write(f"const unsigned char {args.name}[] = {{\n")
        for i in range(0, len(data), 16):
            chunk = data[i:i+16]
            f.write("    ")
            f.write(", ".join(f"0x{b:02x}" for b in chunk))
            f.write(",\n")
        f.write("};\n\n")
        f.write(f"const size_t {args.name}_size = {len(data)};\n")

    print(f"Generated {args.output}, size: {len(data)} bytes")
    return 0

if __name__ == "__main__":
    sys.exit(main())

