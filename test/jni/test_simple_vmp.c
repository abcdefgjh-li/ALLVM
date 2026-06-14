// 简单的 VMP 测试
#include <stdio.h>

// VMP 保护宏
#define VMP_PROTECT __attribute__((annotate("vmp")))

// 简单函数（VMP 保护）
VMP_PROTECT int add(int a, int b) {
    return a + b;
}

// main 函数也使用 VMP 保护
VMP_PROTECT int main() {
    printf("=== Simple VMP Test ===\n");

    int result = add(10, 20);
    printf("Result: %d\n", result);

    if (result == 30) {
        printf("[PASS] VMP test passed!\n");
        return 0;
    } else {
        printf("[FAIL] VMP test failed! Expected 30, got %d\n", result);
        return 1;
    }
}
