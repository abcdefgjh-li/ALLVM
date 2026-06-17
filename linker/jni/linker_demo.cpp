/*
 * 完整的 C++ 测试程序 - 纯 C 风格实现
 * 避免 C++ 运行时依赖
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>

#define LOG_TAG "PayloadTest"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// 全局变量（POD 类型，不需要构造函数）
static int g_global_var = 42;
static char g_global_str[] = "Hello from global";

// 简单函数
int add(int a, int b) {
    return a + b;
}

// 字符串处理
size_t my_strlen(const char* s) {
    size_t len = 0;
    while (s[len]) len++;
    return len;
}

// 整数转字符串
void int_to_str(int n, char* buf, size_t buf_size) {
    if (buf_size == 0) return;
    
    char tmp[32];
    int i = 0;
    int neg = 0;
    
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    
    while (n > 0 && i < 31) {
        tmp[i++] = '0' + (n % 10);
        n /= 10;
    }
    
    size_t j = 0;
    if (neg && j < buf_size - 1) {
        buf[j++] = '-';
    }
    while (i > 0 && j < buf_size - 1) {
        buf[j++] = tmp[--i];
    }
    buf[j] = '\0';
}

// 主函数
int main(int argc, char* argv[]) {
    char buf[64];
    
    LOGI("========================================");
    LOGI("  Payload Test Program (C-Style)");
    LOGI("========================================");
    
    // 打印参数
    LOGI("argc = %d", argc);
    for (int i = 0; i < argc; i++) {
        LOGI("argv[%d] = %s", i, argv[i]);
    }
    
    // 测试全局变量
    LOGI("Global var: %d", g_global_var);
    LOGI("Global str: %s", g_global_str);
    
    // 测试函数调用
    int result = add(10, 20);
    LOGI("add(10, 20) = %d", result);
    
    // 测试字符串操作
    strcpy(buf, "Test string");
    LOGI("Buffer: %s", buf);
    
    // 测试整数转字符串
    int_to_str(12345, buf, sizeof(buf));
    LOGI("int_to_str(12345) = %s", buf);
    
    // 测试动态内存分配
    char* dynamic_buf = (char*)malloc(128);
    if (dynamic_buf) {
        strcpy(dynamic_buf, "Dynamic memory test");
        LOGI("Dynamic buffer: %s", dynamic_buf);
        free(dynamic_buf);
    }
    
    // 打印到 stdout
    printf("Hello from printf!\n");
    printf("Global var: %d\n", g_global_var);
    printf("Result: %d\n", result);
    
    LOGI("========================================");
    LOGI("  Test Completed Successfully!");
    LOGI("========================================");
    
    return 0;
}
