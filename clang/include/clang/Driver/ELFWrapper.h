//===- ELFWrapper.h - ELF 加壳工具（集成到 clang driver）-----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef CLANG_DRIVER_ELFWRAPPER_H
#define CLANG_DRIVER_ELFWRAPPER_H

#include <string>

namespace clang {
namespace driver {

/// 执行 ELF 加壳
/// @param InputELF   链接器输出的原始 ELF 文件路径
/// @param OutputPath 最终加壳后的输出路径
/// @param TargetTriple 目标三元组 (如 aarch64-linux-android)
/// @param ClangPath  clang 可执行文件路径（用于编译壳）
/// @param Sysroot    NDK sysroot 路径（用于编译壳时链接 Android 库）
/// @return 成功返回 true
bool performELFWrapping(const std::string &InputELF,
                        const std::string &OutputPath,
                        const std::string &TargetTriple,
                        const std::string &ClangPath,
                        const std::string &Sysroot = "");

} // namespace driver
} // namespace clang

#endif // CLANG_DRIVER_ELFWRAPPER_H
