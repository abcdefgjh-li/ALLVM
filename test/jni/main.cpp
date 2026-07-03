#include <cstdio>
#include <cstdint>

#define VMP_FN __attribute__((noinline, annotate("vmp")))

VMP_FN int vm_leaf(int x) { return (x * 3) ^ 0x55; }

VMP_FN int vm_mid(int a, int b) {
    int left = vm_leaf(a + 1);
    int right = vm_leaf(b + 2);
    return (left + right) ^ (a - b);
}

VMP_FN int vm_top(int n) {
    int acc = 7;
    for (int i = 0; i < n; ++i) {
        acc += vm_mid(i, n - i);
        acc ^= vm_leaf(acc & 15);
    }
    return acc;
}

static int ref_leaf(int x) { return (x * 3) ^ 0x55; }
static int ref_mid(int a, int b) { return (ref_leaf(a + 1) + ref_leaf(b + 2)) ^ (a - b); }
static int ref_top(int n) {
    int acc = 7;
    for (int i = 0; i < n; ++i) {
        acc += ref_mid(i, n - i);
        acc ^= ref_leaf(acc & 15);
    }
    return acc;
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);
    int got = vm_top(9) + vm_mid(11, 4) + vm_leaf(13);
    int expect = ref_top(9) + ref_mid(11, 4) + ref_leaf(13);
    std::printf("ALLVM_VMP_VALUES got=%d expect=%d\n", got, expect);
    std::fprintf(stderr, "ALLVM_VMP_VALUES got=%d expect=%d\n", got, expect);
    if (got != expect) {
        std::printf("ALLVM_TEST_FAIL\n");
        std::fprintf(stderr, "ALLVM_TEST_FAIL\n");
        return 1;
    }
    std::printf("ALLVM_TEST_PASS\n");
    std::fprintf(stderr, "ALLVM_TEST_PASS\n");
    return 0;
}
