LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := allvm_test
LOCAL_SRC_FILES := main.cpp 加法.cpp 中文目录/乘法.cpp
LOCAL_CPPFLAGS += -std=c++17 -O2 -fno-exceptions -fno-rtti -mllvm -irobf -mllvm -irobf-vmp
include $(BUILD_EXECUTABLE)
