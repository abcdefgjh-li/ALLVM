#include <cstdio>
#include <cstring>

#if defined(__ANDROID__)
#include <unistd.h>
#endif

#if defined(_WIN32)
#define CSE_NOINLINE __declspec(noinline)
#else
#define CSE_NOINLINE __attribute__((noinline))
#endif

static volatile int g_sink = 0;

CSE_NOINLINE static void consume_secret(const char *input) {
    const char *secret = "CSE_HEAP_SECRET_Alpha42";
    const char *banner = "CSE_HEAP_BANNER_Beta24";

    g_sink += static_cast<int>(std::strlen(secret));
    if (std::strcmp(input, secret) == 0) {
        std::puts(banner);
    } else {
        std::puts("CSE_HEAP_FAIL_Gamma66");
    }

#if defined(__ANDROID__)
    std::printf("CSE_PID=%d\n", getpid());
    std::fflush(stdout);
    sleep(120);
#endif
}

int main(int argc, char **argv) {
    const char *arg = argc > 1 ? argv[1] : "default";
    consume_secret(arg);
    return g_sink == 0 ? 1 : 0;
}
