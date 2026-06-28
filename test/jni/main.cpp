#include <cstdio>
#include <cstdint>

#define VMP_FN __attribute__((annotate("+vmp"), noinline))
#define REF_FN __attribute__((noinline))

REF_FN int ref_add(int a, int b) {
    return a + b;
}

VMP_FN int vmp_add(int a, int b) {
    return a + b;
}

extern int 中文加法(int a, int b);
extern int 中文乘法(int a, int b);

int main() {
    int a = 17, b = 25;
    int r_ref = ref_add(a, b);
    int r_vmp = vmp_add(a, b);
    int r_cn_add = 中文加法(a, b);
    int r_cn_mul = 中文乘法(a, b);

    std::printf("ALLVM_TEST_VALUES a=%d b=%d ref=%d vmp=%d cn_add=%d cn_mul=%d\n",
                a, b, r_ref, r_vmp, r_cn_add, r_cn_mul);

    if (r_ref != r_vmp || r_vmp != (a + b) || r_cn_add != (a + b) || r_cn_mul != (a * b)) {
        std::printf("ALLVM_TEST_FAIL\n");
        return 1;
    }

    std::printf("ALLVM_TEST_PASS\n");
    return 0;
}
