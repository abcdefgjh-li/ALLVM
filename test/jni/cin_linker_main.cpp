#include <cstdio>
#include <iostream>

static int read_and_sum() {
    int a = 0;
    int b = 0;
    if (!(std::cin >> a >> b)) {
        std::printf("CIN_READ_FAIL\n");
        return -1;
    }

    std::printf("CIN_VALUES a=%d b=%d\n", a, b);
    return a + b;
}

int main() {
    int sum = read_and_sum();
    if (sum != 42) {
        std::printf("CIN_TEST_FAIL sum=%d\n", sum);
        return 1;
    }

    std::printf("CIN_TEST_PASS sum=%d\n", sum);
    return 0;
}
