LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := clean_id_full_except_root
LOCAL_SRC_FILES := \
    wangzhe_clean.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/include/res

LOCAL_CFLAGS := -w
LOCAL_CPPFLAGS := -std=c++17 -w -frtti
LOCAL_LDLIBS := -llog -lz -pthread -lc
LOCAL_LDFLAGS += -firobf-linker

LOCAL_CFLAGS += -mllvm -irobf \
    -mllvm -irobf-indbr -mllvm -level-indbr=3 \
    -mllvm -irobf-icall -mllvm -level-icall=3 \
    -mllvm -irobf-fla -mllvm -level-fla=3 \
    -mllvm -irobf-indgv -mllvm -level-indgv=3 \
    -mllvm -irobf-cse \
    -mllvm -irobf-cie -mllvm -level-cie=3 \
    -mllvm -irobf-cfe -mllvm -level-cfe=3 \
    -mllvm -irobf-rtti \
    -mllvm -irobf-vmp -mllvm -irobf-vm_functions=main \
    -mllvm -irobf-ldpreload \
    -mllvm -irobf-vmdetect \
    -mllvm -irobf-usb \
    -mllvm -irobf-ida \
    -mllvm -irobf-vpn \
    -mllvm -irobf-proxy \
    -mllvm -irobf-time \
    -mllvm -irobf-hosts \
    -mllvm -irobf-bandump \
    -mllvm -irobf-hidemaps \
    -mllvm -irobf-fakemaps \
    -mllvm -irobf-syscall

LOCAL_CPPFLAGS += -mllvm -irobf \
    -mllvm -irobf-indbr -mllvm -level-indbr=3 \
    -mllvm -irobf-icall -mllvm -level-icall=3 \
    -mllvm -irobf-fla -mllvm -level-fla=3 \
    -mllvm -irobf-indgv -mllvm -level-indgv=3 \
    -mllvm -irobf-cse \
    -mllvm -irobf-cie -mllvm -level-cie=3 \
    -mllvm -irobf-cfe -mllvm -level-cfe=3 \
    -mllvm -irobf-rtti \
    -mllvm -irobf-vmp -mllvm -irobf-vm_functions=main \
    -mllvm -irobf-ldpreload \
    -mllvm -irobf-vmdetect \
    -mllvm -irobf-usb \
    -mllvm -irobf-ida \
    -mllvm -irobf-vpn \
    -mllvm -irobf-proxy \
    -mllvm -irobf-time \
    -mllvm -irobf-hosts \
    -mllvm -irobf-bandump \
    -mllvm -irobf-hidemaps \
    -mllvm -irobf-fakemaps \
    -mllvm -irobf-syscall

include $(BUILD_EXECUTABLE)
