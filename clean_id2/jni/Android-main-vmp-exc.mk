LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := clean_id_main_vmp
LOCAL_SRC_FILES := \
    wangzhe_clean.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/include/res

LOCAL_CFLAGS := -w
LOCAL_CPPFLAGS := -std=c++17 -w -frtti
LOCAL_LDLIBS := -llog -lz -pthread -lc

LOCAL_CFLAGS += -mllvm -irobf-vmp -mllvm -irobf-vmp-noinline
LOCAL_CPPFLAGS += -mllvm -irobf-vmp -mllvm -irobf-vmp-noinline

include $(BUILD_EXECUTABLE)
