// 测试 InvokeInst 和 Funclet 语义支持
#include <stdio.h>
#include <exception>
#include <stdexcept>
#include <string>

// ========== 基本异常测试 ==========

// 简单函数，可能抛出异常
void may_throw_exception(int value) {
    if (value < 0) {
        throw std::runtime_error("Negative value not allowed");
    }
    if (value > 100) {
        throw std::out_of_range("Value too large");
    }
}

// 嵌套异常测试
void nested_exception_test(int level) {
    if (level <= 0) {
        throw std::logic_error("Reached bottom level");
    }
    try {
        nested_exception_test(level - 1);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Error at level " + std::to_string(level)));
    }
}

// ========== Funclet 测试 ==========

// 清理测试类
class CleanupTest {
public:
    int id;
    static int cleanup_count;

    CleanupTest(int i) : id(i) {
        printf("[CleanupTest] Constructor %d\n", id);
    }

    ~CleanupTest() {
        printf("[CleanupTest] Destructor %d\n", id);
        cleanup_count++;
    }
};

int CleanupTest::cleanup_count = 0;

// 测试析构函数调用（cleanup funclet）
int test_cleanup_funclet() {
    printf("\n=== Test Cleanup Funclet ===\n");

    int failed = 0;
    CleanupTest::cleanup_count = 0;

    try {
        CleanupTest obj1(1);
        CleanupTest obj2(2);
        CleanupTest obj3(3);

        printf("[INFO] Throwing exception...\n");
        throw std::runtime_error("Test cleanup");

        printf("[FAIL] Should not reach here\n");
        failed = 1;
    } catch (const std::exception& e) {
        printf("[PASS] Exception caught: %s\n", e.what());
    }

    // 检查析构函数是否被调用
    if (CleanupTest::cleanup_count == 3) {
        printf("[PASS] All destructors called (cleanup_count=%d)\n", CleanupTest::cleanup_count);
    } else {
        printf("[FAIL] Not all destructors called (cleanup_count=%d, expected 3)\n", CleanupTest::cleanup_count);
        failed = 1;
    }

    return failed;
}

// ========== 嵌套异常处理测试 ==========

int test_nested_exception_handling() {
    printf("\n=== Test Nested Exception Handling ===\n");

    int failed = 0;

    // 测试1：嵌套 try-catch
    try {
        printf("[INFO] Outer try block\n");
        try {
            printf("[INFO] Inner try block\n");
            throw std::runtime_error("Inner exception");
        } catch (const std::runtime_error& e) {
            printf("[PASS] Inner exception caught: %s\n", e.what());
            throw;  // 重新抛出
        }
    } catch (const std::exception& e) {
        printf("[PASS] Outer exception caught: %s\n", e.what());
    }

    // 测试2：多层嵌套
    try {
        try {
            try {
                throw std::logic_error("Level 3");
            } catch (const std::logic_error& e) {
                printf("[PASS] Level 3 caught: %s\n", e.what());
                throw std::runtime_error("Level 2");
            }
        } catch (const std::runtime_error& e) {
            printf("[PASS] Level 2 caught: %s\n", e.what());
            throw std::exception();
        }
    } catch (const std::exception& e) {
        printf("[PASS] Level 1 caught\n");
    }

    return failed;
}

// ========== 异常重新抛出测试 ==========

int test_exception_rethrow() {
    printf("\n=== Test Exception Rethrow ===\n");

    int failed = 0;

    try {
        try {
            throw std::runtime_error("Original exception");
        } catch (const std::exception& e) {
            printf("[INFO] Caught: %s\n", e.what());
            printf("[INFO] Rethrowing...\n");
            throw;  // 重新抛出当前异常
        }
    } catch (const std::exception& e) {
        printf("[PASS] Rethrown exception caught: %s\n", e.what());
    }

    return failed;
}

// ========== 异常类型匹配测试 ==========

int test_exception_type_matching() {
    printf("\n=== Test Exception Type Matching ===\n");

    int failed = 0;

    // 测试1：精确匹配
    try {
        throw std::runtime_error("Runtime error");
    } catch (const std::runtime_error& e) {
        printf("[PASS] Exact type match: std::runtime_error\n");
    } catch (const std::exception& e) {
        printf("[FAIL] Should be caught by runtime_error handler\n");
        failed = 1;
    }

    // 测试2：基类匹配
    try {
        throw std::runtime_error("Runtime error");
    } catch (const std::exception& e) {
        printf("[PASS] Base class match: std::exception\n");
    }

    // 测试3：catch-all
    try {
        throw 42;  // 抛出 int
    } catch (const std::exception& e) {
        printf("[FAIL] Should not be caught by std::exception handler\n");
        failed = 1;
    } catch (...) {
        printf("[PASS] Catch-all handler\n");
    }

    return failed;
}

// ========== 多个 catch 块测试 ==========

int test_multiple_catch_blocks() {
    printf("\n=== Test Multiple Catch Blocks ===\n");

    int failed = 0;

    auto test_exception = [&](int type) {
        try {
            if (type == 1) throw std::runtime_error("Runtime");
            if (type == 2) throw std::logic_error("Logic");
            if (type == 3) throw std::out_of_range("Range");
            if (type == 4) throw 42;
        } catch (const std::out_of_range& e) {
            printf("[PASS] Caught std::out_of_range: %s\n", e.what());
            return type == 3 ? 0 : 1;
        } catch (const std::logic_error& e) {
            printf("[PASS] Caught std::logic_error: %s\n", e.what());
            return type == 2 ? 0 : 1;
        } catch (const std::runtime_error& e) {
            printf("[PASS] Caught std::runtime_error: %s\n", e.what());
            return type == 1 ? 0 : 1;
        } catch (...) {
            printf("[PASS] Caught unknown exception\n");
            return type == 4 ? 0 : 1;
        }
        return 1;
    };

    failed += test_exception(1);
    failed += test_exception(2);
    failed += test_exception(3);
    failed += test_exception(4);

    return failed;
}

// ========== 异常传播测试 ==========

void level3() {
    printf("[INFO] Level 3 throwing exception\n");
    throw std::runtime_error("Exception from level 3");
}

void level2() {
    printf("[INFO] Level 2 calling level 3\n");
    try {
        level3();
    } catch (const std::exception& e) {
        printf("[INFO] Level 2 caught: %s, rethrowing...\n", e.what());
        throw;  // 重新抛出
    }
}

void level1() {
    printf("[INFO] Level 1 calling level 2\n");
    level2();
}

int test_exception_propagation() {
    printf("\n=== Test Exception Propagation ===\n");

    int failed = 0;

    try {
        level1();
        printf("[FAIL] Should not reach here\n");
        failed = 1;
    } catch (const std::exception& e) {
        printf("[PASS] Exception propagated through 3 levels: %s\n", e.what());
    }

    return failed;
}

// ========== 嵌套异常（std::nested_exception）测试 ==========

int test_std_nested_exception() {
    printf("\n=== Test std::nested_exception ===\n");

    int failed = 0;

    try {
        nested_exception_test(3);
    } catch (const std::exception& e) {
        printf("[PASS] Outer exception caught: %s\n", e.what());

        // 尝试提取嵌套异常
        try {
            std::rethrow_if_nested(e);
        } catch (const std::exception& nested) {
            printf("[PASS] Nested exception caught: %s\n", nested.what());

            // 继续提取更深的嵌套
            try {
                std::rethrow_if_nested(nested);
            } catch (const std::exception& deeper) {
                printf("[PASS] Deeper nested exception: %s\n", deeper.what());
            } catch (...) {
                // 没有更深的嵌套了
            }
        }
    }

    return failed;
}

// ========== 异常安全性测试 ==========

class Resource {
public:
    int id;
    static int active_count;

    Resource(int i) : id(i) {
        printf("[Resource %d] Acquired\n", id);
        active_count++;
    }

    ~Resource() {
        printf("[Resource %d] Released\n", id);
        active_count--;
    }
};

int Resource::active_count = 0;

int test_exception_safety() {
    printf("\n=== Test Exception Safety ===\n");

    int failed = 0;
    Resource::active_count = 0;

    try {
        Resource r1(1);
        Resource r2(2);

        printf("[INFO] Resources acquired, active_count=%d\n", Resource::active_count);

        throw std::runtime_error("Exception during resource usage");

        printf("[FAIL] Should not reach here\n");
        failed = 1;
    } catch (const std::exception& e) {
        printf("[PASS] Exception caught: %s\n", e.what());
    }

    // 检查资源是否正确释放
    if (Resource::active_count == 0) {
        printf("[PASS] All resources released (active_count=%d)\n", Resource::active_count);
    } else {
        printf("[FAIL] Resource leak detected (active_count=%d, expected 0)\n", Resource::active_count);
        failed = 1;
    }

    return failed;
}

// ========== 主测试函数 ==========

int main() {
    printf("========================================\n");
    printf("  InvokeInst & Funclet Test Suite\n");
    printf("========================================\n");

    int failed = 0;

    failed += test_cleanup_funclet();
    failed += test_nested_exception_handling();
    failed += test_exception_rethrow();
    failed += test_exception_type_matching();
    failed += test_multiple_catch_blocks();
    failed += test_exception_propagation();
    failed += test_std_nested_exception();
    failed += test_exception_safety();

    printf("\n========================================\n");
    printf("  Test Summary\n");
    printf("========================================\n");

    if (failed == 0) {
        printf("✓ All tests passed!\n");
        return 0;
    } else {
        printf("✗ %d test(s) failed\n", failed);
        return 1;
    }
}
