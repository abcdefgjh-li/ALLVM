// 原子操作测试
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>

int main() {
    printf("=== Atomic Operations Test ===\n");
    fflush(stdout);

    // 使用普通变量，手动调用原子操作
    int counter = 0;

    printf("[DEBUG] counter initialized\n");
    fflush(stdout);

    // 测试 atomic_compare_exchange_strong
    int expected = 0;
    int desired = 42;
    bool success = __sync_bool_compare_and_swap(&counter, expected, desired);

    printf("[DEBUG] CAS: success=%d, counter=%d\n", success, counter);
    fflush(stdout);

    if (success && counter == 42) {
        printf("[PASS] atomic_compare_exchange_strong\n");
        fflush(stdout);
    } else {
        printf("[FAIL] atomic_compare_exchange_strong\n");
        fflush(stdout);
        return 1;
    }

    // 测试 atomic_fetch_add
    int old_value = __sync_fetch_and_add(&counter, 10);
    printf("[DEBUG] fetch_add: old=%d, counter=%d\n", old_value, counter);
    fflush(stdout);

    if (old_value == 42 && counter == 52) {
        printf("[PASS] atomic_fetch_add\n");
        fflush(stdout);
    } else {
        printf("[FAIL] atomic_fetch_add\n");
        fflush(stdout);
        return 1;
    }

    printf("[PASS] All atomic tests passed!\n");
    fflush(stdout);
    return 0;
}
