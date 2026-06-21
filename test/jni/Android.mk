LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := allvm_fla_test
LOCAL_SRC_FILES := main.cpp
LOCAL_CPPFLAGS += -std=c++17 -O2 -fno-exceptions -fno-rtti -mllvm -irobf -mllvm -irobf-fla
include $(BUILD_EXECUTABLE)
