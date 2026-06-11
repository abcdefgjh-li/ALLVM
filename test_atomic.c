// 测试原子 CAS 操作
#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#include <stdint.h>

// 测试 atomic_compare_exchange
int test_cas() {
    atomic_int counter = 0;
    int expected = 0;
    int desired = 42;

    // 测试 CAS 成功的情况
    bool success = atomic_compare_exchange_strong(&counter, &expected, desired);
    if (!success || counter != 42 || expected != 0) {
        printf("[FAIL] CAS success case failed: counter=%d, expected=%d, success=%d\n",
               counter, expected, success);
        return 1;
    }
    printf("[PASS] CAS success case: counter=%d\n", counter);

    // 测试 CAS 失败的情况
    expected = 0;  // 期望值不匹配
    desired = 100;
    success = atomic_compare_exchange_strong(&counter, &expected, desired);
    if (success || counter != 42 || expected != 42) {
        printf("[FAIL] CAS failure case failed: counter=%d, expected=%d, success=%d\n",
               counter, expected, success);
        return 1;
    }
    printf("[PASS] CAS failure case: counter=%d, expected updated to %d\n", counter, expected);

    return 0;
}

// 测试 atomic_fetch_add
int test_atomic_add() {
    atomic_int counter = 0;

    int old = atomic_fetch_add(&counter, 10);
    if (old != 0 || counter != 10) {
        printf("[FAIL] atomic_fetch_add failed: old=%d, counter=%d\n", old, counter);
        return 1;
    }
    printf("[PASS] atomic_fetch_add: old=%d, counter=%d\n", old, counter);

    old = atomic_fetch_add(&counter, 5);
    if (old != 10 || counter != 15) {
        printf("[FAIL] atomic_fetch_add second failed: old=%d, counter=%d\n", old, counter);
        return 1;
    }
    printf("[PASS] atomic_fetch_add second: old=%d, counter=%d\n", old, counter);

    return 0;
}

// 测试 atomic_fetch_sub
int test_atomic_sub() {
    atomic_int counter = 20;

    int old = atomic_fetch_sub(&counter, 5);
    if (old != 20 || counter != 15) {
        printf("[FAIL] atomic_fetch_sub failed: old=%d, counter=%d\n", old, counter);
        return 1;
    }
    printf("[PASS] atomic_fetch_sub: old=%d, counter=%d\n", old, counter);

    return 0;
}

// 测试 atomic_exchange
int test_atomic_exchange() {
    atomic_int value = 100;

    int old = atomic_exchange(&value, 200);
    if (old != 100 || value != 200) {
        printf("[FAIL] atomic_exchange failed: old=%d, value=%d\n", old, value);
        return 1;
    }
    printf("[PASS] atomic_exchange: old=%d, value=%d\n", old, value);

    return 0;
}

// 测试 atomic_fetch_or
int test_atomic_or() {
    atomic_int value = 0x0F;

    int old = atomic_fetch_or(&value, 0xF0);
    if (old != 0x0F || value != 0xFF) {
        printf("[FAIL] atomic_fetch_or failed: old=0x%x, value=0x%x\n", old, value);
        return 1;
    }
    printf("[PASS] atomic_fetch_or: old=0x%x, value=0x%x\n", old, value);

    return 0;
}

// 测试 atomic_fetch_and
int test_atomic_and() {
    atomic_int value = 0xFF;

    int old = atomic_fetch_and(&value, 0x0F);
    if (old != 0xFF || value != 0x0F) {
        printf("[FAIL] atomic_fetch_and failed: old=0x%x, value=0x%x\n", old, value);
        return 1;
    }
    printf("[PASS] atomic_fetch_and: old=0x%x, value=0x%x\n", old, value);

    return 0;
}

// 测试 atomic_fetch_xor
int test_atomic_xor() {
    atomic_int value = 0xAA;

    int old = atomic_fetch_xor(&value, 0xFF);
    if (old != 0xAA || value != 0x55) {
        printf("[FAIL] atomic_fetch_xor failed: old=0x%x, value=0x%x\n", old, value);
        return 1;
    }
    printf("[PASS] atomic_fetch_xor: old=0x%x, value=0x%x\n", old, value);

    return 0;
}

// 测试 64 位原子操作
int test_atomic_64bit() {
    atomic_llong counter = 0;
    long long expected = 0;
    long long desired = 0x123456789ABCDEF0LL;

    bool success = atomic_compare_exchange_strong(&counter, &expected, desired);
    if (!success || counter != desired) {
        printf("[FAIL] 64-bit CAS failed: counter=0x%llx, expected=0x%llx, success=%d\n",
               counter, expected, success);
        return 1;
    }
    printf("[PASS] 64-bit CAS: counter=0x%llx\n", counter);

    long long old = atomic_fetch_add(&counter, 100);
    if (old != desired || counter != desired + 100) {
        printf("[FAIL] 64-bit atomic_fetch_add failed: old=0x%llx, counter=0x%llx\n", old, counter);
        return 1;
    }
    printf("[PASS] 64-bit atomic_fetch_add: old=0x%llx, counter=0x%llx\n", old, counter);

    return 0;
}

// 多线程 CAS 测试
#define NUM_THREADS 4
#define NUM_ITERATIONS 1000

atomic_int shared_counter = 0;

void* thread_func(void* arg) {
    int thread_id = *(int*)arg;

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // 使用 CAS 循环递增计数器
        int expected = atomic_load(&shared_counter);
        while (!atomic_compare_exchange_weak(&shared_counter, &expected, expected + 1)) {
            // CAS 失败，expected 已被更新，重试
        }
    }

    return NULL;
}

int test_multithreaded_cas() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    shared_counter = 0;

    // 创建线程
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]) != 0) {
            printf("[FAIL] Failed to create thread %d\n", i);
            return 1;
        }
    }

    // 等待线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 验证结果
    int expected = NUM_THREADS * NUM_ITERATIONS;
    if (shared_counter != expected) {
        printf("[FAIL] Multithreaded CAS failed: counter=%d, expected=%d\n",
               shared_counter, expected);
        return 1;
    }
    printf("[PASS] Multithreaded CAS: counter=%d (expected %d)\n", shared_counter, expected);

    return 0;
}

int main() {
    printf("=== Atomic Operations Test Suite ===\n\n");

    int failed = 0;

    printf("Testing atomic_compare_exchange...\n");
    failed += test_cas();
    printf("\n");

    printf("Testing atomic_fetch_add...\n");
    failed += test_atomic_add();
    printf("\n");

    printf("Testing atomic_fetch_sub...\n");
    failed += test_atomic_sub();
    printf("\n");

    printf("Testing atomic_exchange...\n");
    failed += test_atomic_exchange();
    printf("\n");

    printf("Testing atomic_fetch_or...\n");
    failed += test_atomic_or();
    printf("\n");

    printf("Testing atomic_fetch_and...\n");
    failed += test_atomic_and();
    printf("\n");

    printf("Testing atomic_fetch_xor...\n");
    failed += test_atomic_xor();
    printf("\n");

    printf("Testing 64-bit atomic operations...\n");
    failed += test_atomic_64bit();
    printf("\n");

    printf("Testing multithreaded CAS...\n");
    failed += test_multithreaded_cas();
    printf("\n");

    printf("=== Test Summary ===\n");
    if (failed == 0) {
        printf("✓ All tests passed!\n");
        return 0;
    } else {
        printf("✗ %d test(s) failed\n", failed);
        return 1;
    }
}
