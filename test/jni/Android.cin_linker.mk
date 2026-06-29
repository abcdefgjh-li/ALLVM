LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cin_linker_single
LOCAL_SRC_FILES := cin_linker_main.cpp
LOCAL_CPPFLAGS += -std=c++17 -O2 -fno-exceptions -fno-rtti
LOCAL_LDFLAGS += -firobf-linker
include $(BUILD_EXECUTABLE)
