#include <cstdio>
#include <cstdint>

#define VMP_FN __attribute__((noinline, annotate("vmp")))

VMP_FN uint32_t vm_leaf_mul(uint32_t x) {
    return ((x * 3u) + 0x55u) ^ (x >> 1);
}

VMP_FN uint32_t vm_branch_mix(uint32_t a, uint32_t b) {
    uint32_t v = (a ^ (b << 1)) + 0x13579bdu;
    if ((v & 1u) != 0u) {
        v = (v * 5u) ^ (a + 0x2468u);
    } else {
        v = (v + b * 7u) ^ (b >> 2);
    }
    return v ^ ((a < b) ? 0xa5a5u : 0x5a5au);
}

VMP_FN uint32_t vm_switch_mix(uint32_t v) {
    switch (v & 7u) {
    case 0u: return (v + 11u) ^ 0x101u;
    case 1u: return (v * 3u) + 0x202u;
    case 2u: return (v ^ (v >> 3)) + 0x303u;
    case 3u: return (v << 1) ^ 0x404u;
    case 4u: return (v + (v >> 5)) ^ 0x505u;
    case 5u: return (v * 9u) ^ 0x606u;
    case 6u: return (v - 17u) + 0x707u;
    default: return (v ^ 0x808u) + (v << 2);
    }
}

VMP_FN uint32_t vm_loop_mix(uint32_t seed, uint32_t rounds) {
    uint32_t acc = seed ^ 0x31415926u;
    for (uint32_t i = 0; i < rounds; ++i) {
        acc += vm_leaf_mul(acc + i);
        acc ^= vm_branch_mix(i + 3u, acc & 31u);
        acc = vm_switch_mix(acc);
    }
    return acc;
}

VMP_FN uint32_t vm_mid(uint32_t a, uint32_t b) {
    uint32_t left = vm_leaf_mul(a + 1u);
    uint32_t right = vm_branch_mix(b + 2u, a ^ b);
    uint32_t folded = vm_switch_mix(left + right);
    return folded ^ vm_loop_mix(a + b + 5u, (a ^ b) % 4u + 1u);
}

VMP_FN uint32_t vm_top(uint32_t n) {
    uint32_t acc = 7u;
    for (uint32_t i = 0; i < n; ++i) {
        acc += vm_mid(i + 1u, n - i + 2u);
        acc ^= vm_loop_mix(acc & 0xffu, (i % 3u) + 1u);
        acc = vm_switch_mix(acc + i);
    }
    return acc;
}

static uint32_t ref_leaf_mul(uint32_t x) {
    return ((x * 3u) + 0x55u) ^ (x >> 1);
}

static uint32_t ref_branch_mix(uint32_t a, uint32_t b) {
    uint32_t v = (a ^ (b << 1)) + 0x13579bdu;
    if ((v & 1u) != 0u) {
        v = (v * 5u) ^ (a + 0x2468u);
    } else {
        v = (v + b * 7u) ^ (b >> 2);
    }
    return v ^ ((a < b) ? 0xa5a5u : 0x5a5au);
}

static uint32_t ref_switch_mix(uint32_t v) {
    switch (v & 7u) {
    case 0u: return (v + 11u) ^ 0x101u;
    case 1u: return (v * 3u) + 0x202u;
    case 2u: return (v ^ (v >> 3)) + 0x303u;
    case 3u: return (v << 1) ^ 0x404u;
    case 4u: return (v + (v >> 5)) ^ 0x505u;
    case 5u: return (v * 9u) ^ 0x606u;
    case 6u: return (v - 17u) + 0x707u;
    default: return (v ^ 0x808u) + (v << 2);
    }
}

static uint32_t ref_loop_mix(uint32_t seed, uint32_t rounds) {
    uint32_t acc = seed ^ 0x31415926u;
    for (uint32_t i = 0; i < rounds; ++i) {
        acc += ref_leaf_mul(acc + i);
        acc ^= ref_branch_mix(i + 3u, acc & 31u);
        acc = ref_switch_mix(acc);
    }
    return acc;
}

static uint32_t ref_mid(uint32_t a, uint32_t b) {
    uint32_t left = ref_leaf_mul(a + 1u);
    uint32_t right = ref_branch_mix(b + 2u, a ^ b);
    uint32_t folded = ref_switch_mix(left + right);
    return folded ^ ref_loop_mix(a + b + 5u, (a ^ b) % 4u + 1u);
}

static uint32_t ref_top(uint32_t n) {
    uint32_t acc = 7u;
    for (uint32_t i = 0; i < n; ++i) {
        acc += ref_mid(i + 1u, n - i + 2u);
        acc ^= ref_loop_mix(acc & 0xffu, (i % 3u) + 1u);
        acc = ref_switch_mix(acc + i);
    }
    return acc;
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);

    uint32_t got = 0;
    uint32_t expect = 0;
    for (uint32_t i = 0; i < 6u; ++i) {
        got ^= vm_top(i + 3u);
        got += vm_mid(i + 11u, i * 2u + 4u);
        got ^= vm_loop_mix(i + 0x33u, (i % 4u) + 1u);

        expect ^= ref_top(i + 3u);
        expect += ref_mid(i + 11u, i * 2u + 4u);
        expect ^= ref_loop_mix(i + 0x33u, (i % 4u) + 1u);
    }

    std::printf("ALLVM_VMP_MULTI_VALUES got=%u expect=%u\n", got, expect);
    std::fprintf(stderr, "ALLVM_VMP_MULTI_VALUES got=%u expect=%u\n", got, expect);
    if (got != expect) {
        std::printf("ALLVM_TEST_FAIL\n");
        std::fprintf(stderr, "ALLVM_TEST_FAIL\n");
        return 1;
    }
    std::printf("ALLVM_TEST_PASS\n");
    std::fprintf(stderr, "ALLVM_TEST_PASS\n");
    return 0;
}
