/*
 * ARM64-v8a ELF 内存加载器
 * ChaCha20 解密 + fork 执行 + 擦除 ELF 头
 * VMP 保护关键函数
 */

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <android/log.h>

#include "payload.h"



#define LOG_TAG "ELFLoader"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// ============================================================================
// ChaCha20 实现
// ============================================================================

static inline uint32_t rotl32(uint32_t v, int n) {
    return (v << n) | (v >> (32 - n));
}

static inline void qr(uint32_t* s, int a, int b, int c, int d) {
    s[a] += s[b]; s[d] ^= s[a]; s[d] = rotl32(s[d], 16);
    s[c] += s[d]; s[b] ^= s[c]; s[b] = rotl32(s[b], 12);
    s[a] += s[b]; s[d] ^= s[a]; s[d] = rotl32(s[d], 8);
    s[c] += s[d]; s[b] ^= s[c]; s[b] = rotl32(s[b], 7);
}

static void chacha20_block(const uint32_t key[8], uint32_t counter,
                           const uint32_t nonce[3], uint8_t out[64]) {
    uint32_t s[16] = {
        0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,
        key[0], key[1], key[2], key[3],
        key[4], key[5], key[6], key[7],
        counter,
        nonce[0], nonce[1], nonce[2]
    };

    uint32_t w[16];
    memcpy(w, s, sizeof(w));

    for (int i = 0; i < 10; i++) {
        qr(w, 0, 4,  8, 12);
        qr(w, 1, 5,  9, 13);
        qr(w, 2, 6, 10, 14);
        qr(w, 3, 7, 11, 15);
        qr(w, 0, 5, 10, 15);
        qr(w, 1, 6, 11, 12);
        qr(w, 2, 7,  8, 13);
        qr(w, 3, 4,  9, 14);
    }

    for (int i = 0; i < 16; i++) {
        uint32_t val = w[i] + s[i];
        out[i * 4 + 0] = (uint8_t)(val);
        out[i * 4 + 1] = (uint8_t)(val >> 8);
        out[i * 4 + 2] = (uint8_t)(val >> 16);
        out[i * 4 + 3] = (uint8_t)(val >> 24);
    }
}

static size_t chacha20_decrypt(const uint8_t* input, size_t input_len,
                               const uint8_t key[32], const uint8_t nonce[12],
                               uint8_t* output) {
    uint32_t k[8], n[3];
    for (int i = 0; i < 8; i++) {
        k[i] = (uint32_t)key[i*4]
             | ((uint32_t)key[i*4+1] << 8)
             | ((uint32_t)key[i*4+2] << 16)
             | ((uint32_t)key[i*4+3] << 24);
    }
    for (int i = 0; i < 3; i++) {
        n[i] = (uint32_t)nonce[i*4]
             | ((uint32_t)nonce[i*4+1] << 8)
             | ((uint32_t)nonce[i*4+2] << 16)
             | ((uint32_t)nonce[i*4+3] << 24);
    }

    uint32_t counter = 1;
    uint8_t keystream[64];
    size_t offset = 0;

    while (offset < input_len) {
        chacha20_block(k, counter++, n, keystream);
        size_t chunk = (input_len - offset > 64) ? 64 : (input_len - offset);
        for (size_t i = 0; i < chunk; i++) {
            output[offset + i] = input[offset + i] ^ keystream[i];
        }
        offset += chunk;
    }

    return offset;
}

// ============================================================================
// 擦除 ELF 头
// ============================================================================

static bool erase_elf_header(pid_t pid, const char* target_path) {
    char maps_path[64];
    snprintf(maps_path, sizeof(maps_path), "/proc/%d/maps", pid);

    FILE* maps = fopen(maps_path, "r");
    if (!maps) {
        LOGE("Cannot open %s: %s", maps_path, strerror(errno));
        return false;
    }

    uint64_t base_addr = 0;
    char line[512];
    while (fgets(line, sizeof(line), maps)) {
        if (strstr(line, target_path) && strstr(line, "r-xp")) {
            unsigned long long start, end;
            if (sscanf(line, "%llx-%llx", &start, &end) == 2) {
                base_addr = start;
                break;
            }
        }
    }
    fclose(maps);

    if (base_addr == 0) {
        LOGE("Cannot find ELF mapping for %s in /proc/%d/maps", target_path, pid);
        return false;
    }

    LOGI("Found ELF mapping at 0x%llx for pid %d", (unsigned long long)base_addr, pid);

    char mem_path[64];
    snprintf(mem_path, sizeof(mem_path), "/proc/%d/mem", pid);

    int mem_fd = open(mem_path, O_WRONLY);
    if (mem_fd < 0) {
        LOGE("Cannot open %s: %s", mem_path, strerror(errno));
        return false;
    }

    uint8_t zero_header[64] = {};
    ssize_t written = pwrite(mem_fd, zero_header, sizeof(zero_header), (off_t)base_addr);
    close(mem_fd);

    if (written != sizeof(zero_header)) {
        LOGE("Failed to write ELF header: %s", strerror(errno));
        return false;
    }

    LOGI("ELF header erased at 0x%llx (64 bytes)", (unsigned long long)base_addr);
    return true;
}

// ============================================================================
// ELF 加载器 (全局变量 + 独立函数，兼容 VMP)
// ============================================================================

static char g_temp_path[256] = {};
static pid_t g_child_pid = -1;

static bool do_load(const uint8_t* encrypted_data, size_t encrypted_size) {
    LOGI("Encrypted payload size: %zu bytes", encrypted_size);

    uint8_t* decrypted = reinterpret_cast<uint8_t*>(malloc(encrypted_size));
    if (!decrypted) {
        LOGE("Failed to allocate decryption buffer");
        return false;
    }

    const uint8_t key[] = CHACHA20_KEY;
    const uint8_t nonce[] = CHACHA20_NONCE;

    size_t decrypted_len = chacha20_decrypt(encrypted_data, encrypted_size,
                                             key, nonce, decrypted);
    LOGI("Decrypted: %zu bytes", decrypted_len);

    char temp_template[] = "/data/local/tmp/.ld_XXXXXX";
    int fd = mkstemp(temp_template);
    if (fd < 0) {
        LOGE("mkstemp failed: %s", strerror(errno));
        free(decrypted);
        return false;
    }

    strncpy(g_temp_path, temp_template, sizeof(g_temp_path) - 1);

    ssize_t written = write(fd, decrypted, decrypted_len);
    close(fd);
    free(decrypted);

    if (written != static_cast<ssize_t>(decrypted_len)) {
        LOGE("Write failed");
        return false;
    }

    chmod(g_temp_path, 0755);
    LOGI("ELF written to: %s", g_temp_path);

    return true;
}

static int do_execute(int argc, char** argv) {
    if (g_temp_path[0] == '\0') {
        LOGE("ELF not loaded");
        return -1;
    }

    LOGI("Forking to execute: %s", g_temp_path);

    pid_t pid = fork();
    if (pid < 0) {
        LOGE("fork failed: %s", strerror(errno));
        return -1;
    }

    if (pid == 0) {
        execv(g_temp_path, argv);
        LOGE("execv failed: %s", strerror(errno));
        _exit(127);
    }

    g_child_pid = pid;
    LOGI("Child process started: pid=%d", pid);

    usleep(100000);

    bool erased = erase_elf_header(pid, g_temp_path);
    if (erased) {
        LOGI("ELF header erased successfully from pid %d", pid);
    } else {
        LOGW("Failed to erase ELF header from pid %d", pid);
    }

    int status = 0;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        int exit_code = WEXITSTATUS(status);
        LOGI("Child exited with code: %d", exit_code);
        return exit_code;
    } else if (WIFSIGNALED(status)) {
        int sig = WTERMSIG(status);
        LOGI("Child killed by signal: %d", sig);
        return -sig;
    }

    return -1;
}

// ============================================================================
// 主函数
// ============================================================================

int main(int argc, char* argv[]) {
    LOGI("========================================");
    LOGI("ELF Loader (ChaCha20) Starting");
    LOGI("Payload encrypted size: %zu bytes", payload_data_size);
    LOGI("========================================");

    if (!do_load(payload_data, payload_data_size)) {
        LOGE("Failed to load ELF");
        return 1;
    }

    LOGI("ELF loaded successfully");

    int result = do_execute(argc, argv);

    LOGI("Execution completed with result: %d", result);

    // 清理临时文件
    if (g_temp_path[0] != '\0') {
        unlink(g_temp_path);
    }

    return result;
}
