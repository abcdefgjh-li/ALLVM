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

### 反调试/完整性检测

| 参数 | 说明 |
|------|------|
| `-mllvm -irobf-ldpreload` | LD_PRELOAD注入检测 |
| `-mllvm -irobf-vmdetect` | VM虚拟机检测 |
| `-mllvm -irobf-usb` | USB调试保护 |
| `-mllvm -irobf-ida` | 调试器检测（IDA端口 + TracerPid） |
| `-mllvm -irobf-vpn` | VPN连接检测 |
| `-mllvm -irobf-proxy` | 代理/iptables检测 |
| `-mllvm -irobf-time` | 时间差调试检测 |
| `-mllvm -irobf-hosts` | Hosts文件检测 |
| `-mllvm -irobf-bandump` | 内存 Dump 保护 |
| `-mllvm -irobf-no-aprotect` | 禁用 AProtect 启动输出（默认启用） |
| `-mllvm -irobf-root` | Root检测 (有root退出) |
| `-mllvm -irobf-noroot` | 无Root检测 (无root退出) |
| `-mllvm -irobf-hidemaps` | 隐藏 Maps 保护 (需Root) |
| `-mllvm -irobf-fakemaps` | 伪造Maps内容 |

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
   └─ IdaDetect           (调试器检测)
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
│    - A-protect (随机颜色)            │
│    - Protection v1.6.0               │
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
# LOCAL_CFLAGS += -mllvm -irobf-bandump
# LOCAL_CFLAGS += -mllvm -irobf-no-aprotect  # 禁用 AProtect 输出（默认启用）
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
| `llvm\lib\Transforms\Obfuscation\AProtect.cpp` | AProtect 保护 |
| `llvm\lib\Transforms\Obfuscation\BanDump.cpp` | 内存 Dump 保护 |
| `llvm\lib\Transforms\Obfuscation\LdPreloadProtect.cpp` | LD_PRELOAD 注入检测 |
| `llvm\lib\Transforms\Obfuscation\HideMaps.cpp` | 隐藏 Maps 保护 |
| `llvm\lib\Transforms\Obfuscation\FakeMaps.cpp` | 伪造 maps 文件 |
| `llvm\lib\Transforms\Obfuscation\RootDetect.cpp` | Root 检测 |
| `llvm\lib\Transforms\Obfuscation\NoRootDetect.cpp` | 非Root检测 |
| `llvm\lib\Transforms\Obfuscation\VmProtectDetect.cpp` | VMProtect 检测 |
| `llvm\lib\Transforms\Obfuscation\IdaDetect.cpp` | 调试器检测 |
| `llvm\lib\Transforms\Obfuscation\VpnDetect.cpp` | VPN 检测 |
| `llvm\lib\Transforms\Obfuscation\ProxyDetect.cpp` | 代理检测 |
| `llvm\lib\Transforms\Obfuscation\TimeDetect.cpp` | 时间检测 |
| `llvm\lib\Transforms\Obfuscation\HostsDetect.cpp` | Hosts 文件检测 |
| `llvm\lib\Transforms\Obfuscation\UsbProtect.cpp` | USB 保护 |
| `llvm\lib\Transforms\Obfuscation\Utils.cpp` | 通用工具函数 |
| `llvm\lib\Transforms\Obfuscation\CryptoUtils.cpp` | 加密工具函数 |
| `llvm\lib\Transforms\Obfuscation\ObfuscationOptions.cpp` | 混淆选项 |
| `llvm\lib\Transforms\Obfuscation\LegacyLowerSwitch.cpp` | Switch 降低转换 |
| `llvm\include\llvm\Transforms\Obfuscation\` | 头文件目录 |

## 引用库

| 库 | 地址 |
|----|------|
| **LLVM 21.x** | https://github.com/llvm/llvm-project |
| **OLLVM (obfuscator-llvm)** | https://github.com/obfuscator-llvm/obfuscator |
| **Qt 6** | https://www.qt.io/download-open-source |
| **xVMP** | https://github.com/amunmv/xvmp |

## 更新日志

### v1.6.0 (2026-06-09)
- **VMP兼容性修复**:
  - 修复VMP虚拟机保护与C++异常处理冲突导致崩溃的问题
  - VMP现在要求使用 `-fno-exceptions -frtti` 编译选项
  - UI自动注入VMP所需的编译选项
- **VMP标准库支持增强**:
  - 添加C++随机数库支持（std::random_device, std::mt19937等）
  - 添加C++时间库支持（std::chrono）
  - 跳过所有C++标准库模板实例化的虚拟化
- **Pass注入顺序优化**:
  - 检测类Pass优先注入
  - AProtect打印在main之前执行（全局构造函数）
  - SyscallProtect在VMProtect之前执行
- **AProtect改进**:
  - 改为注入到全局构造函数，避免与VMProtect冲突
  - 移除禁用AProtect的选项，始终启用

### v1.5.0 (2026-06-08)
- **移除问题保护**:
  - 移除 MemProtect（运行时崩溃）
  - 移除 InlineHookDetect 和 PltHookDetect（检测逻辑问题）
- **修复 SyscallProtect**:
  - 修复无限递归问题，使用内联汇编直接调用 syscall
  - 保留 read, write, exit 等核心函数的替换
  - 移除 fopen 替换（无法避免递归）
- **调试器检测整合**:
  - 将 TracerPid 检查并入统一调试器检测
  - 使用检查 /proc/self/status 中的 TracerPid 方式识别附加调试
- **修复 UsbProtect**:
  - 修复当系统文件不存在时的崩溃问题
- **修复 VmProtectDetect**:
  - 修复 PHI 节点使用错误

### v1.4.0 (2026-06-04)
- **VMP 多函数虚拟化支持**:
  - 支持多个VMP保护的函数相互调用
  - 每个VMP函数拥有独立的虚拟机实例和全局变量
  - 修复全局变量冲突问题（添加函数名后缀）
  - 修复解释器函数克隆时的依赖问题
- **混淆Pass管理优化**:
  - 修复混淆Pass无条件启用的问题
  - 现在只有显式启用的混淆才会生效
  - ConstantIntEncryption、Flattening等Pass需要手动启用

### v1.3.0 (2026-06-01)
- **优化使用体验**:
  - UI 重构：移除顶部标题，界面更简洁
  - 新增代码编辑器：带行号显示、语法高亮、暗色主题（类似小熊猫C++）
  - 新增完整帮助文档：涵盖所有混淆功能详解
  - 合并选项：内存Dump & Maps保护（一键开启三项保护）
  - 清理控制台：自动跳转到控制台标签页
  - VMP 提示优化：移除弹窗，改为日志输出
  - 窗口标题更新为 ALLVM

### v1.2.0 (2026-05-30)
- **新增 BanDump Pass**: 通过 mprotect 移除内存读权限，防止内存被 dump
- **移除许可证验证**: 删除所有许可证验证代码，无需卡密即可使用全部功能

### v1.1.0 (2026-05-25)
- **新增 HideMaps Pass**: 通过 mount bind 隐藏 `/proc/self/maps` 文件，防止调试工具读取真实内存映射（需要root权限）
- **新增 FakeMaps Pass**: 生成假的 `/proc/self/maps` 内容，欺骗调试工具显示虚假的内存映射信息
- **AProtect 默认启用**: AProtect 输出现在默认启用，使用 `-irobf-no-aprotect` 禁用
- **移除密钥验证**: 去掉卡密校验机制，无需注入 `-irobf-key`

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
