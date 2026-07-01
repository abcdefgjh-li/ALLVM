LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := allvm_test
LOCAL_SRC_FILES := main.cpp
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_CFLAGS += -fno-omit-frame-pointer
LOCAL_CPPFLAGS += -fno-omit-frame-pointer
endif
include $(BUILD_EXECUTABLE)
