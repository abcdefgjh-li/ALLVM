<div align="center">
  <img src="qrcode_1780325911909.jpg" alt="逆向大家庭" width="300">
  <br>
  <b>逆向大家庭</b>
  <br><br>
  <b>QQ</b>：3986612313
  <br>
  <b>TG</b>：<a href="https://t.me/abcdefgjha">@abcdefgjha</a>
</div>

<br>

# ALLVM Obfuscator 21.x

基于 LLVM 21.x 的 ALLVM 混淆器，用于 Android NDK 编译的代码混淆和保护。

> **GitHub**: [https://github.com/abcdefgjh-li/ALLVM](https://github.com/abcdefgjh-li/ALLVM)

## 快速开始

```bash
.\build.exe
```

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
| `-mllvm -level-fla=3` | 控制流平坦化强度 (1-3) |
| `-mllvm -irobf-cfgnoise` | 启用 CFG 噪声分支 |
| `-mllvm -level-cfgnoise=3` | CFG 噪声分支强度 (1-3) |
| `-mllvm -irobf-indgv` | 启用间接全局变量混淆 |
| `-mllvm -level-indgv=3` | 混淆强度 (1-3) |
| `-mllvm -irobf-cse` | 启用字符串常量加密 |
| `-mllvm -irobf-cie` | 启用整数常量加密 |
| `-mllvm -level-cie=3` | 混淆强度 (1-3) |
| `-mllvm -irobf-cfe` | 启用浮点常量加密 |
| `-mllvm -level-cfe=3` | 混淆强度 (1-3) |
| `-mllvm -irobf-rtti` | 启用 RTTI 信息擦除 |

### VMP 虚拟机保护

| 参数 | 说明 |
|------|------|
| `-mllvm -irobf-vmp` | 启用 VMP 虚拟机保护 |
| `-mllvm -irobf-vm_functions=func1;func2` | 按函数名指定需要虚拟化的函数，多个函数用 `;` 分隔 |

> **重要依赖**: 必须同时开启 `-fno-exceptions -frtti`（UI会自动注入）

**启用方法**: 在需要保护的函数上添加注解：

```cpp
#define VMP_PROTECT __attribute__((annotate("vmp")))

// 保护单个函数
int VMP_PROTECT sensitive_function(int x) {
    return x * 2 + 1;
}

// 保护多个函数 - 支持相互调用
void VMP_PROTECT process_data(char *data, int len);
int VMP_PROTECT calculate_result(int a, int b);

// VMP函数可以调用其他VMP函数
int VMP_PROTECT main(int argc, char **argv) {
    process_data(buffer, len);  // 调用其他VMP函数
    return calculate_result(1, 2);
}
```

> **特性**: 支持多函数虚拟化，VMP保护的函数可以相互调用。每个函数拥有独立的虚拟机实例和全局变量，互不干扰。

**按函数名指定虚拟化**: 如果不想依赖注解，也可以直接通过编译参数指定函数名：

```bash
-mllvm -irobf-vmp -mllvm -irobf-vmp-noinline -mllvm -irobf-vm_functions=main
```

Android NDK `Android.mk` 示例：

```make
LOCAL_CFLAGS += -mllvm -irobf-vmp -mllvm -irobf-vmp-noinline -mllvm -irobf-vm_functions=main
LOCAL_CPPFLAGS += -mllvm -irobf-vmp -mllvm -irobf-vmp-noinline -mllvm -irobf-vm_functions=main
```

> **说明**: 这个用法适合只对 `main` 或少量关键函数开启 VMP；如果要指定多个函数，可以写成 `-mllvm -irobf-vm_functions=main;foo;bar`。

### 反调试/完整性检测

| 参数 | 说明 |
|------|------|
| `-mllvm -irobf-ldpreload` | LD_PRELOAD注入检测 |
| `-mllvm -irobf-vmdetect` | VM虚拟机检测 |
| `-mllvm -irobf-usb` | USB调试保护 |
| `-mllvm -irobf-ida` | 调试器检测（IDA端口 + TracerPid + ptrace自附加） |
| `-mllvm -irobf-vpn` | VPN连接检测 |
| `-mllvm -irobf-proxy` | 代理/iptables检测 |
| `-mllvm -irobf-time` | 时间差调试检测 |
| `-mllvm -irobf-hosts` | Hosts文件检测 |
| `-mllvm -irobf-bandump` | 内存 Dump 保护 |
| `-mllvm -irobf-no-aprotect` | 禁用 A-protector 启动输出（默认启用） |
| `-mllvm -irobf-root` | Root检测 (有root退出) |
| `-mllvm -irobf-noroot` | 无Root检测 (无root退出) |
| `-mllvm -irobf-hidemaps` | 隐藏 Maps 保护 (需Root) |
| `-mllvm -irobf-fakemaps` | 伪造Maps内容 |

### ELF 加壳 (Linker Wrapper)

| 参数 | 说明 |
|------|------|
| `LOCAL_LDFLAGS += -firobf-linker` | 启用 ELF 加壳（仅对可执行文件生效） |

> **注意**: `-firobf-linker` 是链接阶段选项，通过 `LOCAL_LDFLAGS` 传递，不是 `-mllvm` 选项。

**加壳功能**:
- ChaCha20 加密原始 ELF
- fork 执行 + ELF 头擦除
- ptrace 自附加反调试（子进程 PTRACE_TRACEME，父进程作为 tracer）
- TracerPid 监控线程（检测 trace 关系是否被剥离）
- 环境变量校验（壳程序设置 `lc=<随机32位字符串>`，内嵌检测代码校验）
- 壳程序自动以最高强度混淆编译

**Android.mk 示例**:
```makefile
LOCAL_LDFLAGS += -firobf-linker
```

### 系统调用保护

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
| `exit` / `_exit` | 93 | 退出进程 |
| `open` / `openat` | 56 | 打开文件 |
| `unlink` / `unlinkat` | 87/35 | 删除文件 |
| `truncate` / `ftruncate` | 45/46 | 截断文件 |
| `ptrace` | 117 | 进程跟踪 |
| `execve` | 221 | 执行程序 |
| `clock_gettime` | 223 | 获取时间 |
| `memcmp` | - | 内存比较 (手动实现) |
| `getenv` | - | 环境变量获取 (手动实现) |
| `getaddrinfo` | - | 地址信息获取 (手动实现) |
| `popen` | - | 管道打开 (手动实现) |
| `system` | - | 系统命令 (手动实现) |
| `execvp` / `execvpe` | - | 执行程序 (手动实现) |
| `remove` | - | 删除文件 (手动实现) |

## Pass 执行顺序

### 编译时 Pass 注入顺序

```
1. 检测类Pass (最先注入，运行时最先执行)
   └─ LdPreloadProtect    (LD_PRELOAD注入检测)
   └─ VmProtectDetect     (VM虚拟机检测)
   └─ IdaDetect           (调试器检测: IDA端口 + TracerPid + ptrace自附加)
   └─ VpnDetect           (VPN连接检测)
   └─ ProxyDetect         (代理/iptables检测)
   └─ TimeDetect          (时间差调试检测)
   └─ HostsDetect         (Hosts文件检测)
   └─ RootDetect          (Root检测)
   └─ NoRootDetect        (无Root检测)

2. SyscallProtect (系统调用保护)
   └─ 替换libc函数为直接syscall

3. VMProtect (虚拟机保护)
   └─ 函数虚拟化保护

4. 保护类Pass
   └─ UsbProtect          (USB调试保护)
   └─ HideMaps            (隐藏 Maps 保护)
   └─ FakeMaps            (伪造Maps内容)
   └─ BanDump             (内存 Dump 保护)
   └─ EnvCheck            (环境变量校验，配合linker壳使用)
   └─ ConstantIntEncryption
   └─ ConstantFPEncryption
   └─ StringEncryption
   └─ IndirectGlobalVariable
   └─ IndirectCall
   └─ Flattening
   └─ IndirectBranch
   └─ MsRttiEraser
```

### 运行时执行顺序

```
程序启动
    │
    ▼
┌─────────────────────────────────────┐
│ 1. 检测类Pass注入的代码              │
│    (LD_PRELOAD检测、调试器检测等)    │
│    检测到威胁时打印:                 │
│    - A-protector                     │
│    - Protection v1.0.0               │
│    - [DEBUG] XXX detected!           │
└─────────────────────────────────────┘
    │
    ▼
┌─────────────────────────────────────┐
│ 2. main() 函数执行                   │
│    - SyscallProtect保护的函数调用    │
│    - VMProtect虚拟化的函数执行       │
│    - OLLVM混淆后的代码执行           │
└─────────────────────────────────────┘
```

## Android.mk 示例

```makefile
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := myapp
LOCAL_SRC_FILES := main.cpp

LOCAL_CFLAGS := -w

# === ALLVM 总开关 ===
LOCAL_CFLAGS += -mllvm -irobf

# === 代码混淆 ===
LOCAL_CFLAGS += -mllvm -irobf-indbr
LOCAL_CFLAGS += -mllvm -level-indbr=3
LOCAL_CFLAGS += -mllvm -irobf-icall
LOCAL_CFLAGS += -mllvm -level-icall=3
LOCAL_CFLAGS += -mllvm -irobf-fla
LOCAL_CFLAGS += -mllvm -irobf-indgv
LOCAL_CFLAGS += -mllvm -level-indgv=3
LOCAL_CFLAGS += -mllvm -irobf-cse
LOCAL_CFLAGS += -mllvm -irobf-cie
LOCAL_CFLAGS += -mllvm -level-cie=3
LOCAL_CFLAGS += -mllvm -irobf-cfe
LOCAL_CFLAGS += -mllvm -level-cfe=3
LOCAL_CFLAGS += -mllvm -irobf-rtti

# === 系统调用保护 (仅 ARM64) ===
LOCAL_CFLAGS += -mllvm -irobf-syscall

# === VMP 虚拟机保护 ===
# LOCAL_CFLAGS += -mllvm -irobf-vmp
# LOCAL_CFLAGS += -mllvm -irobf-vmp -mllvm -irobf-vmp-noinline -mllvm -irobf-vm_functions=main
# LOCAL_CPPFLAGS += -mllvm -irobf-vmp -mllvm -irobf-vmp-noinline -mllvm -irobf-vm_functions=main
# LOCAL_CFLAGS += -frtti -fno-exceptions

# === ELF 加壳 (仅可执行文件) ===
# LOCAL_LDFLAGS += -firobf-linker

# === 反调试/完整性检测 ===
# LOCAL_CFLAGS += -mllvm -irobf-ldpreload
# LOCAL_CFLAGS += -mllvm -irobf-vmdetect
# LOCAL_CFLAGS += -mllvm -irobf-usb
# LOCAL_CFLAGS += -mllvm -irobf-ida
# LOCAL_CFLAGS += -mllvm -irobf-vpn
# LOCAL_CFLAGS += -mllvm -irobf-proxy
# LOCAL_CFLAGS += -mllvm -irobf-time
# LOCAL_CFLAGS += -mllvm -irobf-hosts
# LOCAL_CFLAGS += -mllvm -irobf-bandump
# LOCAL_CFLAGS += -mllvm -irobf-no-aprotect  # 禁用 A-protector 输出（默认启用）
# LOCAL_CFLAGS += -mllvm -irobf-root
# LOCAL_CFLAGS += -mllvm -irobf-noroot
# LOCAL_CFLAGS += -mllvm -irobf-hidemaps
# LOCAL_CFLAGS += -mllvm -irobf-fakemaps

include $(BUILD_EXECUTABLE)
```

## 关键文件

| 文件 | 说明 |
|------|------|
| `llvm\lib\Transforms\Obfuscation\ObfuscationPassManager.cpp` | Pass 管理器 |
| `llvm\lib\Transforms\Obfuscation\aVMP.cpp` | VMP 虚拟机保护 |
| `llvm\lib\Transforms\Obfuscation\SyscallProtect.cpp` | 系统调用保护 |
| `llvm\lib\Transforms\Obfuscation\Flattening.cpp` | 控制流平坦化 |
| `llvm\lib\Transforms\Obfuscation\IndirectBranch.cpp` | 间接分支混淆 |
| `llvm\lib\Transforms\Obfuscation\IndirectCall.cpp` | 间接调用混淆 |
| `llvm\lib\Transforms\Obfuscation\IndirectGlobalVariable.cpp` | 间接全局变量混淆 |
| `llvm\lib\Transforms\Obfuscation\StringEncryption.cpp` | 字符串加密 |
| `llvm\lib\Transforms\Obfuscation\ConstantIntEncryption.cpp` | 整数常量加密 |
| `llvm\lib\Transforms\Obfuscation\ConstantFPEncryption.cpp` | 浮点常量加密 |
| `llvm\lib\Transforms\Obfuscation\MicrosoftRTTIEraser.cpp` | MSVC RTTI 擦除 |
| `llvm\lib\Transforms\Obfuscation\AProtect.cpp` | A-protector 输出注入 |
| `llvm\lib\Transforms\Obfuscation\BanDump.cpp` | 内存 Dump 保护 |
| `llvm\lib\Transforms\Obfuscation\LdPreloadProtect.cpp` | LD_PRELOAD 注入检测 |
| `llvm\lib\Transforms\Obfuscation\HideMaps.cpp` | 隐藏 Maps 保护 |
| `llvm\lib\Transforms\Obfuscation\FakeMaps.cpp` | 伪造 maps 文件 |
| `llvm\lib\Transforms\Obfuscation\RootDetect.cpp` | Root 检测 |
| `llvm\lib\Transforms\Obfuscation\NoRootDetect.cpp` | 非Root检测 |
| `llvm\lib\Transforms\Obfuscation\VmProtectDetect.cpp` | VMProtect 检测 |
| `llvm\lib\Transforms\Obfuscation\IdaDetect.cpp` | 调试器检测 |
| `llvm\lib\Transforms\Obfuscation\EnvCheck.cpp` | 环境变量校验 (配合linker壳) |
| `llvm\lib\Transforms\Obfuscation\VpnDetect.cpp` | VPN 检测 |
| `llvm\lib\Transforms\Obfuscation\ProxyDetect.cpp` | 代理检测 |
| `llvm\lib\Transforms\Obfuscation\TimeDetect.cpp` | 时间检测 |
| `llvm\lib\Transforms\Obfuscation\HostsDetect.cpp` | Hosts 文件检测 |
| `llvm\lib\Transforms\Obfuscation\UsbProtect.cpp` | USB 保护 |
| `llvm\lib\Transforms\Obfuscation\DetectUtils.cpp` | 检测工具公共模块 |
| `llvm\lib\Transforms\Obfuscation\Utils.cpp` | 通用工具函数 |
| `llvm\lib\Transforms\Obfuscation\CryptoUtils.cpp` | 加密工具函数 |
| `llvm\lib\Transforms\Obfuscation\ObfuscationOptions.cpp` | 混淆选项 |
| `llvm\lib\Transforms\Obfuscation\LegacyLowerSwitch.cpp` | Switch 降低转换 |
| `clang\lib\Driver\ELFWrapper.cpp` | ELF 加壳实现 |
| `clang\include\clang\Driver\ELFWrapper.h` | ELF 加壳头文件 |
| `llvm\include\llvm\Transforms\Obfuscation\` | 头文件目录 |

## 引用库

| 库 | 地址 |
|----|------|
| **LLVM 21.x** | https://github.com/llvm/llvm-project |
| **OLLVM (obfuscator-llvm)** | https://github.com/obfuscator-llvm/obfuscator |
| **Qt 6** | https://www.qt.io/download-open-source |
| **xVMP** | https://github.com/amunmv/xvmp |

## 作者

**abcdefgjh**

- **QQ**: 3986612313
- **TG**: [@abcdefgjha](https://t.me/abcdefgjha)

## License

本项目的 ALLVM 扩展部分（ObTransforms）以 GPL v3 协议发布，详见 [LICENSE](LICENSE)。

```
ALLVM Obfuscator 21.x - LLVM-based code obfuscation for Android NDK
Copyright (C) 2026  abcdefgjh

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
