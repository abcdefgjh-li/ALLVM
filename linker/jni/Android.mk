LOCAL_PATH := $(call my-dir)

# ============================================================================
# 构建 payload (测试程序)
# ============================================================================
include $(CLEAR_VARS)
LOCAL_MODULE := linker_demo
LOCAL_SRC_FILES := linker_demo.cpp

# C++ 标准和编译选项
LOCAL_CFLAGS += -std=c++17 -Wall -Wextra -O2
LOCAL_CPPFLAGS += -std=c++17 -Wall -Wextra -O2 -fexceptions -frtti

# 链接选项 - 静态链接
LOCAL_LDFLAGS += -llog -static

include $(BUILD_EXECUTABLE)

# ============================================================================
# 构建 loader (ELF 加载器)
# ============================================================================
include $(CLEAR_VARS)
LOCAL_MODULE := linker
LOCAL_SRC_FILES := loader.cpp

# C++ 标准和编译选项
LOCAL_CFLAGS += -std=c++17 -Wall -Wextra -O2
LOCAL_CPPFLAGS += -std=c++17 -Wall -Wextra -O2 -fexceptions -frtti

# 链接选项 - 动态链接
LOCAL_LDLIBS += -llog -ldl

include $(BUILD_EXECUTABLE)
