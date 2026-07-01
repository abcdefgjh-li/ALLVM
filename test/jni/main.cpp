#include <cstdio>
#include <cstdint>
#include <cstring>
#include <thread>
#include <chrono>

#define REF_FN __attribute__((noinline))

REF_FN int ref_add(int a, int b) { return a + b; }

REF_FN int len_sum() {
    const char *s1 = "CSE_TEST_STRING_hello";
    const char *s2 = "中文_世界_字符串";
    std::puts(s1);
    return static_cast<int>(std::strlen(s1) + std::strlen(s2));
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);
    int a = 17, b = 25;
    int r_ref = ref_add(a, b);
    int r_len = len_sum();

    std::printf("ALLVM_TEST_VALUES a=%d b=%d ref=%d len=%d\n",
                a, b, r_ref, r_len);
    std::fprintf(stderr, "ALLVM_TEST_VALUES a=%d b=%d ref=%d len=%d\n",
                 a, b, r_ref, r_len);

    int expect_len = static_cast<int>(std::strlen("CSE_TEST_STRING_hello") +
                                      std::strlen("中文_世界_字符串"));
    if (r_ref != (a + b) || r_len != expect_len) {
        std::printf("ALLVM_TEST_FAIL\n");
        std::fprintf(stderr, "ALLVM_TEST_FAIL\n");
        return 1;
    }

    std::printf("ALLVM_TEST_PASS\n");
    std::fprintf(stderr, "ALLVM_TEST_PASS\n");
    std::this_thread::sleep_for(std::chrono::seconds(60));
    return 0;
}
