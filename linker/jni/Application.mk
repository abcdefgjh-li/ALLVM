# 目标架构
APP_ABI := arm64-v8a

# Android 平台版本 (API 23 = Android 6.0 Marshmallow)
APP_PLATFORM := android-23

# C++ 标准库
APP_STL := c++_static

# C++ 标准版本
APP_CPPFLAGS := -std=c++17

# 启用异常和 RTTI
APP_CPPFLAGS += -fexceptions -frtti

# 优化级别
APP_OPTIM := release

# 调试信息
APP_DEBUG := false
