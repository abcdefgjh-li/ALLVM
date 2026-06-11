#include <stdio.h>
#include <stdlib.h>

// VMP 保护注解
#define VMP_PROTECT __attribute__((annotate("vmp")))

// simple recursive factorial
VMP_PROTECT unsigned int test_simple_rec(unsigned int n) {
    if (n <= 1) return 1;
    return n * test_simple_rec(n - 1);
}

// tail recursion
VMP_PROTECT unsigned int test_tail_rec(unsigned int n, unsigned int acc) {
    if (n <= 1) return acc;
    return test_tail_rec(n - 1, acc * n);
}

// mutual recursion
VMP_PROTECT int test_is_even(unsigned int n);
VMP_PROTECT int test_is_odd(unsigned int n);

VMP_PROTECT int test_is_even(unsigned int n) {
    if (n == 0) return 1;
    return test_is_odd(n - 1);
}

VMP_PROTECT int test_is_odd(unsigned int n) {
    if (n == 0) return 0;
    return test_is_even(n - 1);
}

// double recursion (Fibonacci)
VMP_PROTECT unsigned int test_fib(unsigned int n) {
    if (n <= 1) return n;
    return test_fib(n - 1) + test_fib(n - 2);
}

// multi-level recursion
VMP_PROTECT unsigned int test_multi_rec(unsigned int n) {
    if (n <= 1) return 1;
    if (n == 2) return 2;
    return test_multi_rec(n - 1) + test_multi_rec(n - 2) + test_multi_rec(n - 3);
}

int main() {
    unsigned int result;
    int pass = 1;

    setbuf(stdout, 0);
    setbuf(stderr, 0);

    printf("=== VMP Recursion Test ===\n\n");

    // Test 1: Simple recursive factorial
    result = test_simple_rec(5);
    printf("test_simple_rec(5) = %u (expected 120)%s\n",
           result, result == 120 ? " PASS" : " FAIL");
    if (result != 120) pass = 0;

    // Test 2: factorial result check
    result = test_simple_rec(10);
    printf("test_simple_rec(10) = %u (expected 3628800)%s\n",
           result, result == 3628800 ? " PASS" : " FAIL");
    if (result != 3628800) pass = 0;

    // Test 3: base case (n=0)
    result = test_simple_rec(0);
    printf("test_simple_rec(0) = %u (expected 1)%s\n",
           result, result == 1 ? " PASS" : " FAIL");
    if (result != 1) pass = 0;

    // Test 4: base case (n=1)
    result = test_simple_rec(1);
    printf("test_simple_rec(1) = %u (expected 1)%s\n",
           result, result == 1 ? " PASS" : " FAIL");
    if (result != 1) pass = 0;

    // Test 5: Tail recursion
    result = test_tail_rec(5, 1);
    printf("test_tail_rec(5,1) = %u (expected 120)%s\n",
           result, result == 120 ? " PASS" : " FAIL");
    if (result != 120) pass = 0;

    // Test 6: Mutual recursion
    printf("test_is_even(4) = %d (expected 1)%s\n",
           test_is_even(4), test_is_even(4) == 1 ? " PASS" : " FAIL");
    if (test_is_even(4) != 1) pass = 0;
    printf("test_is_odd(4) = %d (expected 0)%s\n",
           test_is_odd(4), test_is_odd(4) == 0 ? " PASS" : " FAIL");
    if (test_is_odd(4) != 0) pass = 0;

    // Test 7: Fibonacci
    result = test_fib(10);
    printf("test_fib(10) = %u (expected 55)%s\n",
           result, result == 55 ? " PASS" : " FAIL");
    if (result != 55) pass = 0;

    // Test 8: Multi-level recursion
    result = test_multi_rec(5);
    printf("test_multi_rec(5) = %u (expected 13)%s\n",
           result, result == 13 ? " PASS" : " FAIL");  // 1,1,2,4,7,13...
    if (result != 13) pass = 0;

    printf("\n=== %s ===\n", pass ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
    return pass ? 0 : 1;
}
