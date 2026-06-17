/*
 * 完整的 ARM64-v8a ELF 内存加载器
 * 使用 dlopen 方式加载
 */

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>

#include "payload.h"

// 日志宏
#define LOG_TAG "ELFLoader"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// ============================================================================
// ELF 加载器类 - 使用 dlopen
// ============================================================================

class ELFLoader {
private:
    void* handle_;
    std::string temp_path_;

public:
    ELFLoader() : handle_(nullptr) {}

    ~ELFLoader() {
        if (handle_) {
            dlclose(handle_);
        }
        if (!temp_path_.empty()) {
            unlink(temp_path_.c_str());
        }
    }

    // 加载 ELF
    bool load(const uint8_t* data, size_t size) {
        LOGI("Loading ELF, size: %zu bytes", size);

        // 创建临时文件
        char temp_template[] = "/data/local/tmp/loaded_XXXXXX";
        int fd = mkstemp(temp_template);
        if (fd < 0) {
            LOGE("Failed to create temp file: %s", strerror(errno));
            return false;
        }

        temp_path_ = temp_template;

        // 写入 ELF 数据
        ssize_t written = write(fd, data, size);
        close(fd);

        if (written != static_cast<ssize_t>(size)) {
            LOGE("Failed to write ELF data");
            return false;
        }

        // 设置可执行权限
        chmod(temp_path_.c_str(), 0755);

        LOGI("ELF written to: %s", temp_path_.c_str());

        // 使用 dlopen 加载
        handle_ = dlopen(temp_path_.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!handle_) {
            LOGE("dlopen failed: %s", dlerror());
            return false;
        }

        LOGI("ELF loaded successfully via dlopen");
        return true;
    }

    // 执行
    int execute(int argc, char** argv) {
        if (!handle_) {
            LOGE("ELF not loaded");
            return -1;
        }

        // 查找 main 函数
        typedef int (*MainFunc)(int, char**, char**);
        MainFunc main_func = reinterpret_cast<MainFunc>(dlsym(handle_, "main"));

        if (!main_func) {
            LOGE("main function not found: %s", dlerror());
            return -1;
        }

        LOGI("Found main function, calling with argc=%d", argc);

        // 获取环境变量
        extern char** environ;

        // 调用 main
        int result = main_func(argc, argv, environ);
        LOGI("main returned: %d", result);

        return result;
    }
};

// ============================================================================
// 主函数
// ============================================================================

int main(int argc, char* argv[]) {
    LOGI("========================================");
    LOGI("ELF Loader (dlopen mode) Starting");
    LOGI("Payload size: %zu bytes", sizeof(payload_data));
    LOGI("========================================");

    // 创建加载器
    ELFLoader loader;

    // 加载 ELF
    if (!loader.load(payload_data, sizeof(payload_data))) {
        LOGE("Failed to load ELF");
        return 1;
    }

    LOGI("ELF loaded successfully");

    // 执行
    int result = loader.execute(argc, argv);

    LOGI("Execution completed with result: %d", result);

    return result;
}
