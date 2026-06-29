LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := allvm_test
LOCAL_SRC_FILES := main.cpp
LOCAL_CFLAGS += -mllvm -irobf -mllvm -irobf-noroot -mllvm -irobf-debug -mllvm -irobf-envcheck
LOCAL_CPPFLAGS += -mllvm -irobf -mllvm -irobf-noroot -mllvm -irobf-debug -mllvm -irobf-envcheck
LOCAL_CFLAGS += -firobf-linker
LOCAL_LDFLAGS += -firobf-linker
include $(BUILD_EXECUTABLE)
