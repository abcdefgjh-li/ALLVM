LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := allvm_test
LOCAL_SRC_FILES := main.cpp
LOCAL_CFLAGS += -mllvm -irobf-vmp
LOCAL_CPPFLAGS += -mllvm -irobf-vmp
include $(BUILD_EXECUTABLE)
