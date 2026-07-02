<div align="center">
  <img src="qq.jpg" alt="逆向大家庭" width="300">
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

默认会依次执行以下阶段：

1. 构建 `zstd`
2. 运行 `cmake` 生成 `build-windows`
3. 编译 `aVMPInterpreter.bc`
4. 生成 `vm.h`
5. 用 Ninja 构建 LLVM/Clang/lld 相关目标
6. 自动执行 NDK 工具链替换
7. 编译并运行 `test/jni` 回归测试

> **说明**: `build.exe` 在构建阶段结束后会自动调用 `replace_ndk_clang()` 执行替换，不需要手工复制二进制。
>
> 当前自动替换的是链接链路相关工具和运行时依赖：
> - `lld.exe`
> - `ld.lld.exe`
> - `llvm-strip.exe`
> - `llvm-objcopy.exe`
> - `build-windows/bin` 下匹配到的 DLL
>
> 它不会覆盖 NDK 自带的 `clang.exe` / `clang++.exe` 前端，这样可以尽量保持 NDK 前端兼容性，同时让最终链接走 ALLVM 自己构建的 `lld` 链路。

## build.exe 用法

### 基本格式

```bash
.\build.exe [选项]
```

### 阶段控制

| 参数 | 说明 |
|------|------|
| `--skip-build` | 跳过默认构建流程中的 `cmake / interpreter / vmh / build / test` 阶段 |
| `--skip zstd,cmake,interpreter,vmh,build,test` | 跳过指定阶段，传入值按子串匹配 |
| `--only zstd` | 仅执行 `zstd` 阶段 |
| `--only cmake` | 仅执行 `cmake` 配置 |
| `--only interpreter` | 仅编译 `aVMPInterpreter.bc` |
| `--only vmh` | 仅生成 `vm.h` |
| `--only build` | 仅构建 LLVM/Clang/lld 并自动替换 NDK 链接工具 |
| `--only test` | 仅执行测试链，包括机器码验证和设备运行测试 |

### 构建参数

| 参数 | 说明 |
|------|------|
| `--target <triple>` | 指定 `aVMPInterpreter` 编译目标，默认 `aarch64-linux-android` |
| `-j <N>` / `--jobs <N>` | 指定并行构建任务数 |
| `--build "<ninja targets>"` | 指定 Ninja 目标，默认 `clang lld llvm-strip llvm-objcopy llvm-dis llc FileCheck` |

### APK 相关

| 参数 | 说明 |
|------|------|
| `--apk` | 构建 APK |
| `--apk-release` | 构建 Release APK |
| `--all` | 运行默认构建流程并额外构建 APK |

### 自定义测试参数

| 参数 | 说明 |
|------|------|
| `--test-project <dir>` | 指定自定义 NDK 测试工程目录 |
| `--test-build-script <path>` | 指定 `APP_BUILD_SCRIPT`，默认 `jni/Android.mk` |
| `--test-application-mk <path>` | 指定 `NDK_APPLICATION_MK`，默认 `jni/Application.mk` |
| `--test-binary <name>` | 指定设备端要运行的二进制名 |
| `--test-local-binary <path>` | 直接指定本地待推送二进制路径 |
| `--test-device-path <path>` | 指定设备端推送路径 |
| `--test-run-cmd <cmd>` | 指定设备端执行命令；不指定时默认 `chmod + 执行二进制` |
| `--test-serial <serial>` | 指定设备序列号 |
| `--test-abi <abi>` | 指定测试 ABI |
| `--test-timeout <sec>` | 指定设备端运行超时秒数 |
| `--skip-test-build` | 跳过 NDK 构建，直接推送并运行现有二进制 |

### 常用命令示例

```bash
# 全量构建 + 自动替换 + 默认测试
.\build.exe

# 只重编 LLVM/Clang/lld，并自动替换 NDK 链接工具
.\build.exe --only build -j 16

# 只跑后端/设备测试链
.\build.exe --only test -j 8

# 自定义测试工程
.\build.exe --only test --test-project D:\work\demo -j 8

# 直接推送本地二进制到设备运行
.\build.exe --only test --skip-test-build --test-local-binary D:\tmp\demo --test-device-path /data/local/tests/demo
```

## 混淆参数

所有参数通过 `LOCAL_CFLAGS += -mllvm <参数>` 添加到 `Android.mk` 中。

### 总开关

| 参数 | 说明 |
|------|------|
| `-mllvm -irobf` | **混淆总开关**，启用后以下参数才会生效 |
| `-mllvm -irobf-debug` | **调试模式**，启用后输出混淆和检测的调试信息 |
| `-firobf-no-unwind` | **禁用 Unwind/CFI 生成**，强制附加 `-fno-asynchronous-unwind-tables`、`-fno-unwind-tables` 语义，并向后端传递 `-mllvm -irobf-no-cfi` |

### Unwind / CFI 处理

| 参数 | 说明 |
|------|------|
| `-firobf-no-unwind` | 驱动层强制关闭 unwind table 请求，覆盖默认 toolchain unwind 行为 |
| `-mllvm -irobf-no-cfi` | 后端开关，直接屏蔽 `.cfi_*` 发射，阻断 `.eh_frame` / `debug_frame` 的 CFI 指令输出 |

> `-firobf-no-unwind` 是给 `clang/clang++` 用户直接使用的外部参数。
>
> `-irobf-no-cfi` 是后端内部开关，通常不需要手工单独传；`-firobf-no-unwind` 会自动透传它。

### AArch64 后端指令级混淆

> 下面这些是 **AArch64 后端开关**，用于寄存器分配后或汇编输出阶段的机器码级改写。
>
> 它们不走 `-irobf` 总开关，而是直接通过后端命令行开关启用，适用于 `clang/clang++` 的 `-mllvm` 透传，或 `llc` 直接测试。

| 参数 | 说明 |
|------|------|
| `-mllvm -aarch64-obfuscate-frame-record` | 将标准 `stp/ldp + mov x29, sp` 栈帧记录改写为 `str/ldr + add/sub` 组合，破坏标准 frame-record 特征 |
| `-mllvm -aarch64-obfuscate-call-ret` | 将直接 `BL` 改写为 `ADRP/ADD/BLR` 间接调用序列，并将 `RET` 改写为 `BR X30` |
| `-mllvm -aarch64-obfuscate-opaque-predicate` | 注入永远不走的死分支，并在死分支内发射非 4 字节对齐的 `.byte` 垃圾数据 |
| `-mllvm -aarch64-enable-ldst-opt=false` | 验证 frame-record 改写时建议临时关闭，防止 `ldst-opt` 把序列重新配对回 `STP/LDP` |

### AArch64 后端开关示例

```makefile
# 彻底关闭 unwind / CFI
LOCAL_CPPFLAGS += -firobf-no-unwind

# 拆分 frame record
LOCAL_CPPFLAGS += -mllvm -aarch64-obfuscate-frame-record
LOCAL_CPPFLAGS += -mllvm -aarch64-enable-ldst-opt=false

# Call/Ret 混淆
LOCAL_CPPFLAGS += -mllvm -aarch64-obfuscate-call-ret

# 不透明谓词 + 死分支垃圾字节
LOCAL_CPPFLAGS += -mllvm -aarch64-obfuscate-opaque-predicate
```

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

## 机器码验证与反汇编扰乱

`.\build.exe --only test` 不只是跑 `test/jni`，还会在设备执行前先做一轮 AArch64 后端机器码验证。

### 自动验证项

1. `frame_record_probe`
   - 使用 `build-windows/bin/clang++.exe` 生成 AArch64 汇编
   - 校验是否出现拆分后的 `str/ldr` 与 `add/sub x29` 序列
   - 拒绝出现标准 `stp/ldp` 或 `mov x29, sp`

2. `call_ret_probe`
   - 校验是否把直接 `BL` 改写为间接调用
   - 校验返回是否改写为 `BR X30`

3. `irobf-call-ret-scratch.mir`
   - 使用 `llc -run-pass=aarch64-call-ret-obfuscation`
   - 验证不会固定占用 `X16`
   - 验证 scratch 寄存器来自动态选择

4. `opaque predicate`
   - 使用 `llc` 先生成汇编，检查死分支中是否存在 `.byte 0`、`.byte 232`、`.p2align 2, 0x0`
   - 再生成目标文件，用 `llvm-objdump -d` 校验错位后的机器码
   - 当前验证目标包括：
     - `.word 0x00000000`，对应 `UDF #0` 编码
     - `.word 0x000003e8`，对应错位尾部字节拼接结果

5. `no unwind / no cfi`
   - `clang` driver 侧验证 `-firobf-no-unwind` 会抑制 `-funwind-tables=`
   - `llc` / 后端侧验证 `-irobf-no-cfi` 后汇编中不再出现任何 `.cfi_*`
   - 目标是同时从 driver 和 AsmPrinter 两侧破坏 `.eh_frame` 生成链

### 相关测试输入

| 文件 | 作用 |
|------|------|
| `test/jni/frame_record_codegen.cpp` | 栈帧拆分 codegen 验证输入 |
| `test/jni/call_ret_codegen.cpp` | Call/Ret 改写 codegen 验证输入 |
| `llvm/test/CodeGen/AArch64/irobf-call-ret-scratch.mir` | 动态 scratch 寄存器 MIR 回归 |
| `llvm/test/CodeGen/AArch64/irobf-opaque-predicate-bytes.ll` | 不透明谓词字节输出与目标文件回归 |
| `llvm/test/CodeGen/AArch64/irobf-opaque-predicate.mir` | 不透明谓词 CFG/MIR 形态回归 |
| `llvm/test/MC/AArch64/udf.s` | `0x00000000 == udf #0` 的 MC 侧编码依据 |

### 设备执行路径

- 默认优先推送到 `/data/local/tmp/<binary>`
- 如果当前设备禁止写入 `/data/local/tmp`，`build.exe` 会自动回退到 `/data/local/tests/<binary>`
- 推送成功后自动执行 `chmod 755 + 运行`

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

# === AArch64 后端指令级混淆 (仅 AArch64) ===
# LOCAL_CPPFLAGS += -mllvm -aarch64-obfuscate-frame-record
# LOCAL_CPPFLAGS += -mllvm -aarch64-enable-ldst-opt=false
# LOCAL_CPPFLAGS += -mllvm -aarch64-obfuscate-call-ret
# LOCAL_CPPFLAGS += -mllvm -aarch64-obfuscate-opaque-predicate

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
| `llvm\lib\Target\AArch64\AArch64FrameLowering.cpp` | AArch64 frame-record 拆分与恢复 |
| `llvm\lib\Target\AArch64\AArch64CallRetObfuscation.cpp` | AArch64 Call/Ret 机器码改写 |
| `llvm\lib\Target\AArch64\AArch64OpaquePredicate.cpp` | AArch64 不透明谓词与死分支构造 |
| `llvm\lib\Target\AArch64\AArch64AsmPrinter.cpp` | 死分支 `.byte` 垃圾数据发射 |
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

## 作者

**abcdefgjh**

- **QQ**: 3986612313
- **TG**: [@abcdefgjha](https://t.me/abcdefgjha)

## 特别感谢

- **Saye**：提供linker壳包装

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
