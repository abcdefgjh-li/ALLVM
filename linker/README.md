# 完整的 ARM64 ELF 内存加载器

**商用级别的 ELF 加载器实现**

## 项目概述

这是一个完整的、商用级别的 ARM64 ELF 内存加载器，支持静态和动态链接的 ELF 文件。该加载器实现了完整的 ELF 加载流程，包括重定位、符号解析、TLS 支持等功能。

### 主要特性

✅ **完整的 ELF 解析**
- ELF 头解析
- 程序头解析
- 节头解析
- 动态段解析

✅ **符号处理**
- 动态符号表解析
- 静态符号表解析
- GNU Hash 表支持
- SYSV Hash 表支持
- 外部符号查找

✅ **AArch64 重定位支持**
- R_AARCH64_ABS64/32/16 - 绝对地址重定位
- R_AARCH64_PREL64/32/16 - PC 相对重定位
- R_AARCH64_RELATIVE - 基址重定位
- R_AARCH64_GLOB_DAT - 全局数据
- R_AARCH64_JUMP_SLOT - PLT 跳转槽
- R_AARCH64_ADR_PREL_PG_HI21 - ADRP 指令
- R_AARCH64_ADD_ABS_LO12_NC - ADD 指令
- R_AARCH64_LDST*_ABS_LO12_NC - LDR/STR 指令
- R_AARCH64_CALL26/JUMP26 - 分支指令
- R_AARCH64_CONDBR19/TSTBR14 - 条件分支
- R_AARCH64_ADR_GOT_PAGE - GOT 页访问
- R_AARCH64_LD64_GOT_LO12_NC - GOT 加载
- TLS 相关重定位

✅ **TLS（线程本地存储）支持**
- PT_TLS 段处理
- TLS 模块 ID 分配
- TLS 偏移计算

✅ **动态链接支持**
- .dynamic 段解析
- 动态库符号查找
- dlsym 集成

✅ **初始化/析构函数**
- .init 函数调用
- .init_array 数组处理
- .fini 函数调用
- .fini_array 数组处理
- __attribute__((constructor/destructor)) 支持

✅ **内存管理**
- mmap 内存分配
- 内存权限设置
- 段对齐处理

✅ **错误处理**
- 详细的错误码
- 错误信息追踪
- 日志系统

## 文件结构

```
linker/
├── jni/
│   ├── Android.mk          # NDK 构建配置
│   ├── Application.mk      # NDK 架构配置
│   ├── linker_demo.cpp     # 完整的测试程序
│   ├── loader.cpp          # 完整的 ELF 加载器
│   └── payload.h           # 嵌入的 payload（自动生成）
├── libs/
│   └── arm64-v8a/          # 编译输出
│       ├── linker          # 加载器可执行文件
│       └── linker_demo     # 测试程序可执行文件
├── bin2c.py                # ELF 转 C 数组工具
├── test_on_emulator.sh     # 测试脚本
└── README.md               # 本文档
```

## 架构说明

### 1. ELFLoader 类

核心加载器类，包含以下主要功能：

```cpp
class ELFLoader {
public:
    // 加载 ELF 文件
    LoaderError load(const uint8_t* data, size_t size);

    // 执行入口点
    int execute(int argc, char** argv, char** envp = nullptr);

    // 获取符号地址
    void* get_symbol_address(const std::string& name);

    // 卸载
    void unload();

private:
    // 解析函数
    LoaderError parse_elf_header();
    LoaderError parse_program_headers();
    void parse_section_headers();
    LoaderError parse_dynamic_segment();
    LoaderError parse_symbols();

    // 加载函数
    LoaderError allocate_memory();
    LoaderError load_segments();
    LoaderError process_relocations();
    LoaderError setup_tls();

    // 重定位函数
    LoaderError apply_relocation(const Elf64_Rela* rela);
    LoaderError apply_relocation(const Elf64_Rel* rel);

    // 初始化函数
    LoaderError call_init_functions();
    void call_fini_functions();
};
```

### 2. 加载流程

```
1. 解析 ELF 头
   ↓
2. 解析程序头
   ↓
3. 解析节头（如果有）
   ↓
4. 计算加载地址范围
   ↓
5. 分配内存
   ↓
6. 加载段
   ↓
7. 解析动态段
   ↓
8. 解析符号表
   ↓
9. 设置 TLS
   ↓
10. 处理重定位
   ↓
11. 调用初始化函数
   ↓
12. 执行入口点
```

### 3. 测试程序功能

测试程序包含以下测试：

1. **全局变量和静态变量测试**
   - 全局整型、字符串
   - 静态变量
   - 全局对象

2. **TLS（线程本地存储）测试**
   - thread_local 变量
   - 多线程 TLS 访问

3. **类和对象测试**
   - 构造/析构函数
   - 成员函数
   - 静态成员

4. **继承和多态测试**
   - 虚函数
   - 虚函数表
   - 动态绑定

5. **异常处理测试**
   - try-catch 块
   - 标准异常
   - 自定义异常

6. **动态内存分配测试**
   - new/delete
   - 智能指针
   - 数组分配

7. **STL 容器测试**
   - vector
   - map
   - string

8. **多线程测试**
   - std::thread
   - std::mutex
   - 共享数据访问

9. **初始化和析构函数测试**
   - 全局对象初始化
   - __attribute__((constructor))
   - __attribute__((destructor))

10. **函数指针和回调测试**
    - 函数指针
    - Lambda 表达式

11. **递归和复杂算法测试**
    - 斐波那契
    - 阶乘
    - 排序算法

12. **虚函数表测试**
    - 虚函数调用
    - 多态行为

## 构建方法

### 前置要求

- Android NDK r21 或更高版本
- Python 3.x
- ADB（用于测试）

### 构建步骤

```bash
# 1. 进入项目目录
cd linker

# 2. 构建 payload 和 loader
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk

# 3. 生成 payload.h
python bin2c.py libs/arm64-v8a/linker_demo jni/payload.h payload_data

# 4. 重新构建（包含嵌入的 payload）
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk
```

### 清理构建

```bash
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk clean
```

## 测试方法

### 在模拟器上测试

```bash
# 1. 启动模拟器
emulator -avd <your_avd_name> &

# 2. 推送文件
adb push libs/arm64-v8a/linker /data/local/tmp/linker
adb push libs/arm64-v8a/linker_demo /data/local/tmp/linker_demo

# 3. 设置权限
adb shell chmod 755 /data/local/tmp/linker /data/local/tmp/linker_demo

# 4. 直接运行 payload（对比）
adb shell /data/local/tmp/linker_demo arg1 arg2

# 5. 通过 loader 运行
adb shell /data/local/tmp/linker arg1 arg2

# 6. 查看日志
adb logcat -s ELFLoader:* PayloadTest:*
```

### 在真机上测试

```bash
# 1. 连接设备
adb devices

# 2. 推送文件
adb push libs/arm64-v8a/linker /data/local/tmp/linker
adb push libs/arm64-v8a/linker_demo /data/local/tmp/linker_demo

# 3. 设置权限
adb shell chmod 755 /data/local/tmp/linker /data/local/tmp/linker_demo

# 4. 运行测试
adb shell /data/local/tmp/linker arg1 arg2

# 5. 查看日志
adb logcat -s ELFLoader:* PayloadTest:*
```

## 使用示例

### 基本使用

```cpp
#include "loader.cpp"

int main() {
    // 创建加载器
    ELFLoader loader;

    // 加载 ELF
    LoaderError err = loader.load(elf_data, elf_size);
    if (err != LoaderError::SUCCESS) {
        // 处理错误
        const ErrorInfo& error = loader.get_last_error();
        printf("Error: %s - %s\n",
               error.message.c_str(),
               error.detail.c_str());
        return 1;
    }

    // 执行
    char* argv[] = {"program", "arg1", "arg2"};
    int result = loader.execute(3, argv);

    // 卸载
    loader.unload();

    return result;
}
```

### 获取符号地址

```cpp
// 获取函数地址
typedef void (*MyFunction)(int);
MyFunction func = (MyFunction)loader.get_symbol_address("my_function");
if (func) {
    func(42);
}

// 获取变量地址
int* global_var = (int*)loader.get_symbol_address("global_variable");
if (global_var) {
    printf("Global variable: %d\n", *global_var);
}
```

## 性能优化

### 1. 内存分配优化

- 使用 mmap 进行大块内存分配
- 按页对齐减少内存碎片
- 延迟设置内存权限

### 2. 符号查找优化

- 使用 GNU Hash 表加速查找
- 缓存常用符号地址
- 批量处理重定位

### 3. 重定位优化

- 按类型批量处理重定位
- 避免重复计算
- 使用内联函数

## 安全考虑

### 1. 输入验证

- 检查 ELF 文件大小
- 验证段地址范围
- 检查符号索引

### 2. 内存安全

- 检查 mmap 返回值
- 验证 mprotect 调用
- 防止缓冲区溢出

### 3. 错误处理

- 详细的错误信息
- 资源清理
- 状态追踪

## 已知限制

1. **TLS 实现**
   - 简化的 TLS 实现
   - 不支持动态 TLS 模型

2. **动态链接**
   - 不支持延迟绑定
   - 简化的符号版本处理

3. **异常处理**
   - 依赖系统 C++ 运行时
   - 需要正确的 .eh_frame 段

## 调试技巧

### 1. 启用详细日志

修改 loader.cpp 中的日志级别：

```cpp
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
```

### 2. 使用 GDB 调试

```bash
# 启动 gdbserver
adb shell gdbserver :5039 /data/local/tmp/linker

# 连接 gdb
adb forward tcp:5039 tcp:5039
aarch64-linux-android-gdb
(gdb) target remote :5039
```

### 3. 检查内存映射

```bash
adb shell cat /proc/<pid>/maps
```

## 常见问题

### Q1: 加载失败，提示 "Failed to allocate memory"

**A:** 可能是地址冲突，尝试修改内存分配策略或使用 ASLR。

### Q2: 重定位失败，提示 "Undefined symbol"

**A:** 检查符号是否在常用库中，或者添加到 loaded_libraries_ 映射。

### Q3: 程序崩溃，没有明显错误

**A:** 检查：
- 内存权限是否正确设置
- 重定位是否正确应用
- 初始化函数是否正确调用

### Q4: TLS 变量不工作

**A:** 确保系统支持 TLS，并且 TLS 重定位正确处理。

## 技术参考

1. **ELF 格式**
   - [ELF Format Specification](https://refspecs.linuxfoundation.org/elf/elf.pdf)
   - [System V ABI](https://wiki.osdev.org/System_V_ABI)

2. **AArch64 架构**
   - [ARM Architecture Reference Manual](https://developer.arm.com/documentation/)
   - [AArch64 ELF Specification](https://github.com/ARM-software/abi-aa/blob/main/aaelf64/aaelf64.rst)

3. **Android NDK**
   - [Android NDK Guide](https://developer.android.com/ndk/guides)
   - [Android Linker](https://android.googlesource.com/platform/bionic/+/master/linker/)

## 许可证

本项目仅用于软件保护研究和教育目的。使用者需遵守所在地区法律法规，不得将该技术用于非法用途。

## 免责声明

本项目仅用于软件保护研究和教育目的。使用者需遵守所在地区法律法规，不得将该技术用于非法用途。作者不对任何因使用本代码而产生的直接或间接损失负责。

## 贡献

欢迎提交 Issue 和 Pull Request。

## 更新日志

### v2.0.0 (2024-01-XX)
- 完整重写加载器
- 支持所有 AArch64 重定位类型
- 添加 TLS 支持
- 添加完整的错误处理
- 添加详细的日志系统
- 添加完整的测试程序

### v1.0.0 (2024-01-XX)
- 初始版本
- 基本的 ELF 加载功能
- 简单的重定位支持
