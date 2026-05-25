# OLLVM Obfuscator 21.x

基于 LLVM 21.x 的 OLLVM 混淆器，用于 Android NDK 编译的代码混淆和保护。

> **GitHub**: [https://github.com/abcdefgjh-li/ALLVM](https://github.com/abcdefgjh-li/ALLVM)

## 快速开始

### 编译 OLLVM

```bash
.\build.exe
```

### 编译测试程序

```bash
cd test
..\android-ndk-r30-beta1-windows\ndk-build.cmd clean
..\android-ndk-r30-beta1-windows\ndk-build.cmd NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./jni/Android.mk APP_PLATFORM=android-21 APP_ABI=arm64-v8a
```

## 关键文件位置

| 文件 | 说明 |
|------|------|
| `llvm\lib\Transforms\Obfuscation\ObfuscationPassManager.cpp` | Pass 管理器，注册和调度所有混淆 Pass |
| `llvm\lib\Transforms\Obfuscation\SyscallProtect.cpp` | 系统调用保护，替换 libc 函数为直接 syscall |
| `llvm\lib\Transforms\Obfuscation\Flattening.cpp` | 控制流平坦化 |
| `llvm\lib\Transforms\Obfuscation\IndirectBranch.cpp` | 间接分支混淆 |
| `llvm\lib\Transforms\Obfuscation\IndirectCall.cpp` | 间接调用混淆 |
| `llvm\lib\Transforms\Obfuscation\StringEncryption.cpp` | 字符串加密 |
| `llvm\include\llvm\Transforms\Obfuscation\` | 头文件目录 |

## 混淆参数

所有参数通过 `LOCAL_CFLAGS += -mllvm <参数>` 添加到 `Android.mk` 中。

### 总开关

| 参数 | 说明 |
|------|------|
| `-mllvm -irobf` | **混淆总开关**，启用后以下参数才会生效 |
| `-mllvm -irobf-debug` | **调试模式**，启用后输出混淆和检测的调试信息 |

### 代码混淆

| 参数 | 说明 |
|------|------|
| `-mllvm -irobf-indbr` | 启用间接跳转混淆 |
| `-mllvm -level-indbr=3` | 混淆强度 (1-3) |
| `-mllvm -irobf-icall` | 启用间接调用混淆 |
| `-mllvm -level-icall=3` | 混淆强度 (1-3) |
| `-mllvm -irobf-fla` | 启用控制流平坦化 |
| `-mllvm -irobf-indgv` | 启用间接全局变量混淆 |
| `-mllvm -level-indgv=3` | 混淆强度 (1-3) |

### 常量加密

| 参数 | 说明 |
|------|------|
| `-mllvm -irobf-cse` | 启用字符串常量加密 |
| `-mllvm -irobf-cie` | 启用整数常量加密 |
| `-mllvm -level-cie=3` | 混淆强度 (1-3) |
| `-mllvm -irobf-cfe` | 启用浮点常量加密 |
| `-mllvm -level-cfe=3` | 混淆强度 (1-3) |

### RTTI 擦除

| 参数 | 说明 |
|------|------|
| `-mllvm -irobf-rtti` | 启用 RTTI 信息擦除 |

### VMP 虚拟机保护

| 参数 | 说明 |
|------|------|
| `-mllvm -irobf-vmp` | 启用 VMP 虚拟机保护 |

> **重要依赖**: 必须同时开启 `-frtti -fno-exceptions`

**启用方法**: 在需要保护的函数上添加注解：

```cpp
#define VMP_PROTECT __attribute__((annotate("vmp")))

// 保护单个函数
int VMP_PROTECT sensitive_function(int x) {
    return x * 2 + 1;
}

// 保护多个函数
void VMP_PROTECT process_data(char *data, int len);
int VMP_PROTECT calculate_result(int a, int b);
```

### 反调试/完整性检测

| 参数 | 说明 |
|------|------|
| `-mllvm -irobf-ldpreload` | LD_PRELOAD注入检测 |
| `-mllvm -irobf-vmdetect` | VM虚拟机检测 |
| `-mllvm -irobf-usb` | USB调试禁用检测 |
| `-mllvm -irobf-ida` | IDA调试器检测 |
| `-mllvm -irobf-vpn` | VPN连接检测 |
| `-mllvm -irobf-proxy` | 代理/iptables检测 |
| `-mllvm -irobf-time` | 时间差调试检测 |
| `-mllvm -irobf-hosts` | Hosts文件检测 |
| `-mllvm -irobf-mem` | 内存驻留检测 |
| `-mllvm -irobf-ptrace` | Ptrace调试器检测 |
| `-mllvm -irobf-inlinehook` | Inline Hook检测 |
| `-mllvm -irobf-plthook` | PLT Hook检测 |
| `-mllvm -irobf-memprotect` | 内存Dump保护 |
| `-mllvm -irobf-root` | Root检测(有root退出) |
| `-mllvm -irobf-noroot` | 无Root检测(无root退出) |
| `-mllvm -irobf-hidemaps` | 隐藏Maps文件(需Root) |
| `-mllvm -irobf-fakemaps` | 伪造Maps内容 |

### 系统调用保护 (Syscall Protect)

| 参数 | 说明 |
|------|------|
| `-mllvm -irobf-syscall` | 启用系统调用保护 (仅 ARM64) |

将以下 libc 函数替换为直接系统调用，绕过 libc 防止 Hook 注入：

| 原函数 | 系统调用号 | 说明 |
|--------|------------|------|
| `connect` | 203 | Socket 连接 |
| `send` / `sendto` | 206 | 发送数据 |
| `recv` / `recvfrom` | 207 | 接收数据 |
| `read` | 63 | 读取数据 |
| `write` | 64 | 写入数据 |
| `clock_gettime` | 223 | 获取时间 |

## Pass 执行顺序

```
1. SyscallProtect (系统调用保护)
2. VMProtect (虚拟机保护)
3. 检测注入 (反调试等)
4. OLLVM混淆 (代码混淆保护)
   └─ ConstantIntEncryption
   └─ IndirectGlobalVariable
   └─ ConstantFPEncryption
   └─ StringEncryption
   └─ IndirectCall
   └─ Flattening
   └─ IndirectBranch
   └─ MsRttiEraser
```

## Android.mk 完整示例

```makefile
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := myapp
LOCAL_SRC_FILES := main.cpp

LOCAL_CFLAGS := -w

# === OLLVM 总开关 ===
LOCAL_CFLAGS += -mllvm -irobf

# === 调试输出 ===
# LOCAL_CFLAGS += -mllvm -irobf-debug

# === 代码混淆 ===
LOCAL_CFLAGS += -mllvm -irobf-indbr
LOCAL_CFLAGS += -mllvm -level-indbr=3
LOCAL_CFLAGS += -mllvm -irobf-icall
LOCAL_CFLAGS += -mllvm -level-icall=3
LOCAL_CFLAGS += -mllvm -irobf-fla
LOCAL_CFLAGS += -mllvm -irobf-indgv
LOCAL_CFLAGS += -mllvm -level-indgv=3

# === 常量加密 ===
LOCAL_CFLAGS += -mllvm -irobf-cse
LOCAL_CFLAGS += -mllvm -irobf-cie
LOCAL_CFLAGS += -mllvm -level-cie=3
LOCAL_CFLAGS += -mllvm -irobf-cfe
LOCAL_CFLAGS += -mllvm -level-cfe=3

# === RTTI 擦除 ===
LOCAL_CFLAGS += -mllvm -irobf-rtti

# === 系统调用保护 (仅 ARM64) ===
LOCAL_CFLAGS += -mllvm -irobf-syscall

# === VMP 虚拟机保护 (必须搭配 -frtti -fno-exceptions) ===
# LOCAL_CFLAGS += -mllvm -irobf-vmp
# LOCAL_CFLAGS += -frtti -fno-exceptions

# === 反调试/完整性检测 ===
# LOCAL_CFLAGS += -mllvm -irobf-ldpreload
# LOCAL_CFLAGS += -mllvm -irobf-vmdetect
# LOCAL_CFLAGS += -mllvm -irobf-usb
# LOCAL_CFLAGS += -mllvm -irobf-ida
# LOCAL_CFLAGS += -mllvm -irobf-vpn
# LOCAL_CFLAGS += -mllvm -irobf-proxy
# LOCAL_CFLAGS += -mllvm -irobf-time
# LOCAL_CFLAGS += -mllvm -irobf-hosts
# LOCAL_CFLAGS += -mllvm -irobf-mem
# LOCAL_CFLAGS += -mllvm -irobf-ptrace
# LOCAL_CFLAGS += -mllvm -irobf-inlinehook
# LOCAL_CFLAGS += -mllvm -irobf-plthook
# LOCAL_CFLAGS += -mllvm -irobf-memprotect
# LOCAL_CFLAGS += -mllvm -irobf-root
# LOCAL_CFLAGS += -mllvm -irobf-noroot
# LOCAL_CFLAGS += -mllvm -irobf-hidemaps
# LOCAL_CFLAGS += -mllvm -irobf-fakemaps

include $(BUILD_EXECUTABLE)
```

## 开发说明

### 新增 Pass 步骤

1. 在 `llvm\lib\Transforms\Obfuscation\` 创建 `NewPass.cpp`
2. 在 `llvm\include\llvm\Transforms\Obfuscation\` 创建 `NewPass.h`
3. 在 `llvm\lib\Transforms\Obfuscation\CMakeLists.txt` 添加源文件
4. 在 `ObfuscationPassManager.cpp` 中注册 Pass

### SyscallProtect 实现原理

使用内联汇编直接调用 ARM64 的 `svc #0` 指令：

```cpp
InlineAsm *Asm = InlineAsm::get(AsmTy,
    "svc #0",
    "={x0},{x0},{x1},{x2},{x3},{x4},{x5},{x8},~{memory},~{cc}",
    true, false);
```

参数通过寄存器传递：
- x0-x5: 系统调用参数
- x8: 系统调用号
- 返回值在 x0

## 引用库

| 库 | 地址 |
|----|------|
| **LLVM 21.x** | https://github.com/llvm/llvm-project |
| **OLLVM (obfuscator-llvm)** | https://github.com/obfuscator-llvm/obfuscator |
| **Qt 6** | https://www.qt.io/download-open-source |

## 致谢

感谢以下开源项目对本项目的启发和贡献：

| 项目 | 地址 |
|------|------|
| **xVMP** | https://github.com/amunmv/xvmp |

## 更新日志

### 2026-05-25
- **新增 HideMaps Pass**: 通过 mount bind 隐藏 `/proc/self/maps` 文件，防止调试工具读取真实内存映射（需要root权限）
- **新增 FakeMaps Pass**: 生成假的 `/proc/self/maps` 内容，欺骗调试工具显示虚假的内存映射信息

## 作者

**abcdefgjh**

## License

本项目的 OLLVM 扩展部分（ObTransforms）以 GPL v3 协议发布，详见 [LICENSE](LICENSE)。

```
OLLVM Obfuscator 21.x - LLVM-based code obfuscation for Android NDK
Copyright (C) 2024-2026  abcdefgjh

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```

LLVM/Clang/lld 本体遵循 [Apache License 2.0 with LLVM Exceptions](llvm/LICENSE.TXT)。
