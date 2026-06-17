/*
 * 简单的 C 测试程序
 * 不使用 C++ 标准库
 */

#include <stdio.h>
#include <string.h>
#include <android/log.h>

#define LOG_TAG "SimpleTest"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// 全局变量
int g_global_var = 42;
const char* g_global_str = "Hello from global";

// 简单函数
int add(int a, int b) {
    return a + b;
}

int main(int argc, char* argv[]) {
    LOGI("========================================");
    LOGI("  Simple C Test Program");
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
    char buffer[64];
    strcpy(buffer, "Test string");
    LOGI("Buffer: %s", buffer);
    
    // 打印到 stdout
    printf("Hello from printf!\n");
    printf("Global var: %d\n", g_global_var);
    
    LOGI("========================================");
    LOGI("  Test Completed Successfully!");
    LOGI("========================================");
    
    return 0;
}
