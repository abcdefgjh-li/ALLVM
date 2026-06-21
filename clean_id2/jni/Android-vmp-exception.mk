LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := vmp_exception_test
LOCAL_SRC_FILES := \
    vmp_exception_test.cpp

LOCAL_CFLAGS := -w -O0
LOCAL_CPPFLAGS := -std=c++17 -w -O0 -fexceptions -frtti
LOCAL_LDLIBS := -llog -pthread -lc

# Intentionally keep C++ exceptions enabled for this VMP regression sample.
LOCAL_CFLAGS += -mllvm -irobf-vmp -mllvm -irobf-vmp-noinline
LOCAL_CPPFLAGS += -mllvm -irobf-vmp -mllvm -irobf-vmp-noinline

include $(BUILD_EXECUTABLE)
