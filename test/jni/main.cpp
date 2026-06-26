#include <cstdio>
#include <cstdint>

static volatile int g_sink = 0;

#define FLA_L1 __attribute__((annotate("+fla"), annotate("^fla=1"), noinline))
#define FLA_L2 __attribute__((annotate("+fla"), annotate("^fla=2"), noinline))
#define FLA_L3 __attribute__((annotate("+fla"), annotate("^fla=3"), noinline))
#define REF_FN __attribute__((noinline))

#define DEFINE_CASE(NAME, ATTR, BIAS)                             \
ATTR static int NAME(int seed) {                                 \
    int acc = seed * (BIAS + 3) + 7;                             \
    for (int i = 0; i < 7 + BIAS; ++i) {                         \
        int tag = ((acc >> 1) + i + BIAS) & 3;                   \
        switch (tag) {                                           \
            case 0:                                              \
                acc += i * (BIAS + 5) + 3;                       \
                break;                                            \
            case 1:                                              \
                acc ^= (i + 11 * BIAS);                          \
                break;                                            \
            case 2:                                              \
                acc -= i * (BIAS + 2) - 9;                       \
                break;                                            \
            default:                                             \
                acc = acc * 3 - i + BIAS;                        \
                break;                                            \
        }                                                        \
        if (((acc ^ i) & 1) != 0) {                              \
            acc += (BIAS * 7) - i;                               \
        } else {                                                 \
            acc -= (BIAS * 5) + i;                               \
        }                                                        \
        if ((acc % 3) == 0) {                                    \
            acc ^= (i + 13 * BIAS);                              \
        } else {                                                 \
            acc += (i * BIAS) + 5;                               \
        }                                                        \
    }                                                            \
    g_sink ^= acc;                                               \
    return acc ^ (BIAS * 17);                                    \
}

DEFINE_CASE(ref_level1, REF_FN, 1)
DEFINE_CASE(ref_level2, REF_FN, 2)
DEFINE_CASE(ref_level3, REF_FN, 3)
DEFINE_CASE(fla_level1, FLA_L1, 1)
DEFINE_CASE(fla_level2, FLA_L2, 2)
DEFINE_CASE(fla_level3, FLA_L3, 3)

int main() {
    const int seed = 9;
    const int r1 = ref_level1(seed);
    const int r2 = ref_level2(seed);
    const int r3 = ref_level3(seed);
    const int f1 = fla_level1(seed);
    const int f2 = fla_level2(seed);
    const int f3 = fla_level3(seed);

    std::printf("ALLVM_TEST_VALUES ref=[%d,%d,%d] fla=[%d,%d,%d] sink=%d\n",
                r1, r2, r3, f1, f2, f3, g_sink);

    if (r1 != f1 || r2 != f2 || r3 != f3) {
        std::printf("ALLVM_TEST_FAIL\n");
        return 1;
    }

    std::printf("ALLVM_TEST_PASS\n");
    return 0;
}
