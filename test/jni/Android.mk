LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := test_recursion
LOCAL_SRC_FILES := test_recursion.c

# ALLVM 总开关 + VMP 虚拟机保护（参考 README.md）
LOCAL_CFLAGS := -O2 -fPIC -w
LOCAL_CFLAGS += -mllvm -irobf
LOCAL_CFLAGS += -mllvm -irobf-vmp
LOCAL_CFLAGS += -frtti -fno-exceptions -fvisibility=hidden

LOCAL_LDFLAGS := -fPIE -pie

include $(BUILD_EXECUTABLE)
