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

int main() {
    int a = 17, b = 25;
    int r_ref = ref_add(a, b);
    int r_vmp = vmp_add(a, b);

    std::printf("ALLVM_TEST_VALUES a=%d b=%d ref=%d vmp=%d\n", a, b, r_ref, r_vmp);

    if (r_ref != r_vmp || r_vmp != (a + b)) {
        std::printf("ALLVM_TEST_FAIL\n");
        return 1;
    }

    std::printf("ALLVM_TEST_PASS\n");
    return 0;
}
