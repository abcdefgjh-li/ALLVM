LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := test_atomic
LOCAL_SRC_FILES := test_atomic.c
LOCAL_LDLIBS := -lpthread

include $(BUILD_EXECUTABLE)
