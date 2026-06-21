LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := vmp_exception_test_plain
LOCAL_SRC_FILES := \
    vmp_exception_test.cpp

LOCAL_CFLAGS := -w -O0
LOCAL_CPPFLAGS := -std=c++17 -w -O0 -fexceptions -frtti
LOCAL_LDLIBS := -llog -pthread -lc

include $(BUILD_EXECUTABLE)
