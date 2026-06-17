# 快速开始指南

## 5 分钟快速上手

### 1. 环境准备

确保已安装：
- Android NDK r21+
- Python 3.x
- ADB（Android Debug Bridge）

### 2. 一键构建和测试

```bash
# 进入项目目录
cd linker

# 运行完整测试（自动构建、推送、测试）
chmod +x test_on_emulator.sh
./test_on_emulator.sh --all
```

### 3. 手动构建步骤

如果需要手动控制每一步：

```bash
# 步骤 1: 构建
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk

# 步骤 2: 生成 payload.h
python3 bin2c.py libs/arm64-v8a/linker_demo jni/payload.h payload_data

# 步骤 3: 重新构建
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk

# 步骤 4: 推送到设备
adb push libs/arm64-v8a/linker /data/local/tmp/
adb push libs/arm64-v8a/linker_demo /data/local/tmp/
adb shell chmod 755 /data/local/tmp/linker /data/local/tmp/linker_demo

# 步骤 5: 运行测试
adb shell /data/local/tmp/linker_demo  # 直接运行
adb shell /data/local/tmp/linker       # 通过 loader 运行
```

### 4. 查看日志

```bash
# 实时查看日志
adb logcat -s ELFLoader:* PayloadTest:*

# 或使用测试脚本
./test_on_emulator.sh --log
```

## 常用命令

```bash
# 仅构建
./test_on_emulator.sh --build

# 仅推送
./test_on_emulator.sh --push

# 仅测试
./test_on_emulator.sh --test

# 清理
./test_on_emulator.sh --clean

# 帮助
./test_on_emulator.sh --help
```

## 预期输出

成功运行后，您应该看到：

```
=== Payload Test Program ===
Testing ELF loader functionality...

Global int: 42
Global string: Global String
Static int: 100
Global object: Global Object
Object1 value: 100
Object2 name: Object2
Instance count: 2
...
=== All Tests Passed ===
```

## 故障排除

### 问题 1: ndk-build 未找到

```bash
# 设置 NDK 路径
export PATH=$PATH:/path/to/android-ndk
```

### 问题 2: 设备未连接

```bash
# 检查设备
adb devices

# 如果为空，检查 USB 调试是否开启
```

### 问题 3: 权限被拒绝

```bash
# 确保文件有执行权限
adb shell chmod 755 /data/local/tmp/linker
```

## 下一步

- 查看 [README.md](README.md) 了解详细功能
- 修改 `linker_demo.cpp` 创建自己的测试程序
- 集成到您的项目中

## 技术支持

遇到问题？查看：
1. [README.md](README.md) - 完整文档
2. 日志输出 - 使用 `adb logcat`
3. 源码注释 - 详细的代码说明
