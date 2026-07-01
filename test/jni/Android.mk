LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := allvm_test
LOCAL_SRC_FILES := main.cpp
LOCAL_CFLAGS += -mllvm -irobf-no-cfi
LOCAL_CPPFLAGS += -mllvm -irobf-no-cfi
include $(BUILD_EXECUTABLE)
