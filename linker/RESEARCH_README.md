# ARM64-v8a 内存加载研究方案

仅用于 **合法的软件保护研究/教育目的**

## 文件结构

```
linker/
├── jni/
│   ├── Android.mk          # NDK 构建配置
│   ├── Application.mk      # NDK 架构配置
│   ├── linker_demo.cpp     # 合法示例程序（payload）
│   ├── loader.cpp          # 研究用 ELF 加载器
│   └── payload.h           # 嵌入的 payload（自动生成）
├── bin2c.py                # ELF 转 C 数组工具
├── test_on_emulator.sh     # 测试脚本
└── RESEARCH_README.md      # 本文档
```

## 架构说明

### 1. payload (linker_demo)
- 合法示例程序
- 静态链接，无外部依赖
- 展示基本计算和日志功能

### 2. loader
- 透明的 ELF 加载器
- 功能：
  - 解析 ELF 头部
  - 分配内存空间
  - 加载程序段
  - 设置内存保护位
  - 跳转到入口点执行

### 3. 构建流程
```
  linker_demo.cpp → (ndk-build) → linker_demo
                           ↓ (bin2c.py)
                      payload.h
                           ↓
  loader.cpp + payload.h → (ndk-build) → linker
```

## 当前状态

✅ **已完成：**
- 基础项目框架
- payload 正常运行
- loader 框架完整（分配内存、加载段）
- ndk-build 集成

⚠️ **研究中：**
- ELF 重定位（静态链接程序也需要）
- TLS 初始化
- 最终跳转后的运行时初始化

## 使用方法

### 构建
```bash
# 1. 先构建 payload
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk

# 2. 生成 payload.h
python bin2c.py libs/arm64-v8a/linker_demo jni/payload.h payload_data

# 3. 重新构建（包含 loader）
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk
```

### 推送到模拟器测试
```bash
adb push libs/arm64-v8a/linker /data/local/tmp/linker
adb push libs/arm64-v8a/linker_demo /data/local/tmp/linker_demo
adb shell chmod 755 /data/local/tmp/linker /data/local/tmp/linker_demo

# 直接运行 payload（对比）
adb shell /data/local/tmp/linker_demo arg1 arg2

# 通过 loader 运行（研究用）
adb shell /data/local/tmp/linker arg1 arg2
```

## 进一步研究方向

1. **ELF 动态链接与重定位**：支持动态链接 ELF
2. **TLS（线程本地存储）**：正确初始化 TLS 段
3. **异常处理与 C++ 运行时**：加载静态链接的 C++ 程序
4. **代码混淆与加固**：在 loader 中加入保护措施

## 免责声明

本项目仅用于 **软件保护研究和教育目的**。使用者需遵守所在地区法律法规，不得将该技术用于非法用途。

