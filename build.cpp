#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

static std::string g_script_dir;
static std::string g_build_dir;
static std::string g_ndk_dir;
static std::string g_ndk_bin;
static std::string g_acode_dir;
static std::string g_apk_output_dir;
static std::string g_zstd_dir;
static std::string g_zlib_dir;

static bool file_exists(const std::string& path) {
    return GetFileAttributesA(path.c_str()) != INVALID_FILE_ATTRIBUTES;
}

static bool dir_exists(const std::string& path) {
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

static void init_paths() {
    char buf[MAX_PATH];
    GetModuleFileNameA(NULL, buf, MAX_PATH);
    std::string exe_path(buf);
    size_t pos = exe_path.find_last_of("\\/");
    g_script_dir = exe_path.substr(0, pos);
    g_build_dir = g_script_dir + "\\build-windows";
    g_acode_dir = g_script_dir + "\\apkUI";
    g_apk_output_dir = g_build_dir + "\\bin";
    g_zstd_dir = g_script_dir + "\\zstd";
    g_zlib_dir = g_script_dir + "\\zlib";

    // Search NDK in current directory first
    std::string local_ndk = g_script_dir + "\\android-ndk-r30-beta1-windows";
    if (dir_exists(local_ndk)) {
        g_ndk_dir = local_ndk;
    } else {
        // Then search in parent directory
        std::string p_ndk = g_script_dir + "\\..\\android-ndk-r30-beta1-windows";
        char abs_path[MAX_PATH];
        if (GetFullPathNameA(p_ndk.c_str(), MAX_PATH, abs_path, NULL)) {
            if (dir_exists(abs_path)) {
                g_ndk_dir = abs_path;
            }
        }
    }

    if (!g_ndk_dir.empty()) {
        g_ndk_bin = g_ndk_dir + "\\toolchains\\llvm\\prebuilt\\windows-x86_64\\bin";
    }
}

static std::string find_vs() {
    const char* vcvars_path = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\VC\\Auxiliary\\Build\\vcvars64.bat";
    if (file_exists(vcvars_path)) return vcvars_path;
    return "";
}

static void dir_create(const std::string& path) {
    CreateDirectoryA(path.c_str(), NULL);
}

static void copy_file(const std::string& src, const std::string& dst) {
    CopyFileA(src.c_str(), dst.c_str(), FALSE);
}

static bool write_text_file(const std::string& path, const std::string& content) {
    FILE *f = fopen(path.c_str(), "wb");
    if (!f) return false;
    fwrite(content.data(), 1, content.size(), f);
    fclose(f);
    return true;
}

static std::string trim_copy(const std::string& input) {
    size_t begin = 0;
    while (begin < input.size() && std::isspace(static_cast<unsigned char>(input[begin]))) begin++;
    size_t end = input.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(input[end - 1]))) end--;
    return input.substr(begin, end - begin);
}

static std::string get_env_var(const char* name) {
    char buf[32767];
    DWORD len = GetEnvironmentVariableA(name, buf, sizeof(buf));
    if (len == 0 || len >= sizeof(buf)) return "";
    return std::string(buf, len);
}

static std::string search_path_binary(const char* name) {
    char buf[MAX_PATH];
    DWORD len = SearchPathA(NULL, name, NULL, MAX_PATH, buf, NULL);
    if (len == 0 || len >= MAX_PATH) return "";
    return std::string(buf, len);
}

static int run_cmd(const std::string& cmd, const std::string& cwd = "") {
    std::string bat_file = cwd.empty() ? "build_tmp.bat" : cwd + "\\build_tmp.bat";
    FILE *f = fopen(bat_file.c_str(), "wb");
    if (!f) return -1;
    fprintf(f, "@echo off\r\nchcp 65001 >nul\r\n");
    if (!cwd.empty()) fprintf(f, "cd /d \"%s\"\r\n", cwd.c_str());
    fprintf(f, "%s\r\n", cmd.c_str());
    fprintf(f, "exit /b %%ERRORLEVEL%%\r\n");
    fclose(f);

    int ret = system(bat_file.c_str());
    DeleteFileA(bat_file.c_str());
    return ret;
}

static int run_cmd_vcvars(const std::string& vcvars, const std::string& cmd, const std::string& cwd = "") {
    std::string bat_file = cwd.empty() ? "build_tmp.bat" : cwd + "\\build_tmp.bat";
    FILE *f = fopen(bat_file.c_str(), "wb");
    if (!f) return -1;
    fprintf(f, "@echo off\r\nchcp 65001 >nul\r\n");
    fprintf(f, "call \"%s\" >nul 2>&1\r\n", vcvars.c_str());
    if (!cwd.empty()) fprintf(f, "cd /d \"%s\"\r\n", cwd.c_str());
    fprintf(f, "%s\r\n", cmd.c_str());
    fprintf(f, "exit /b %%ERRORLEVEL%%\r\n");
    fclose(f);

    int ret = system(bat_file.c_str());
    DeleteFileA(bat_file.c_str());
    return ret;
}

static int run_cmd_capture(const std::string& cmd, std::string& output, const std::string& cwd = "") {
    char suffix[32];
    sprintf(suffix, "%llu", static_cast<unsigned long long>(GetTickCount64()));
    std::string base = cwd.empty() ? g_script_dir : cwd;
    std::string bat_file = base + "\\build_capture_" + suffix + ".bat";
    std::string out_file = base + "\\build_capture_" + suffix + ".log";

    FILE *f = fopen(bat_file.c_str(), "wb");
    if (!f) return -1;
    fprintf(f, "@echo off\r\nchcp 65001 >nul\r\n");
    if (!cwd.empty()) fprintf(f, "cd /d \"%s\"\r\n", cwd.c_str());
    fprintf(f, "%s > \"%s\" 2>&1\r\n", cmd.c_str(), out_file.c_str());
    fprintf(f, "exit /b %%ERRORLEVEL%%\r\n");
    fclose(f);

    int ret = system(bat_file.c_str());
    DeleteFileA(bat_file.c_str());

    std::ifstream in(out_file, std::ios::binary);
    if (in.is_open()) {
        std::ostringstream ss;
        ss << in.rdbuf();
        output = ss.str();
        in.close();
        DeleteFileA(out_file.c_str());
    } else {
        output.clear();
    }
    return ret;
}

static std::string find_adb() {
    std::string adb = search_path_binary("adb.exe");
    if (!adb.empty()) return adb;

    const char* envs[] = {"ANDROID_SDK_ROOT", "ANDROID_HOME"};
    for (auto env : envs) {
        std::string root = get_env_var(env);
        if (root.empty()) continue;
        std::string candidate = root + "\\platform-tools\\adb.exe";
        if (file_exists(candidate)) return candidate;
    }
    return "";
}

static std::string find_test_device(const std::string& adb_path) {
    std::string output;
    std::string cmd = "\"" + adb_path + "\" devices";
    if (run_cmd_capture(cmd, output) != 0) return "";

    std::istringstream iss(output);
    std::string line;
    std::string first_device;
    while (std::getline(iss, line)) {
        line = trim_copy(line);
        if (line.empty()) continue;
        if (line.find("List of devices attached") == 0) continue;
        if (line.find("* daemon") == 0) continue;
        size_t tab = line.find('\t');
        if (tab == std::string::npos) continue;
        std::string serial = trim_copy(line.substr(0, tab));
        std::string state = trim_copy(line.substr(tab + 1));
        if (state != "device") continue;
        if (serial.find("emulator-") == 0) return serial;
        if (first_device.empty()) first_device = serial;
    }
    return first_device;
}

static std::string normalize_abi(const std::string& abi) {
    if (abi.find("arm64-v8a") != std::string::npos) return "arm64-v8a";
    if (abi.find("armeabi-v7a") != std::string::npos) return "armeabi-v7a";
    if (abi.find("x86_64") != std::string::npos) return "x86_64";
    if (abi.find("x86") != std::string::npos) return "x86";
    return trim_copy(abi);
}

static std::string detect_device_abi(const std::string& adb_path, const std::string& serial) {
    std::string output;
    std::string cmd = "\"" + adb_path + "\" -s " + serial + " shell getprop ro.product.cpu.abi";
    if (run_cmd_capture(cmd, output) != 0) return "";

    std::istringstream iss(output);
    std::string line;
    while (std::getline(iss, line)) {
        line = normalize_abi(line);
        if (!line.empty()) return line;
    }
    return "";
}

static std::string find_test_binary(const std::string& project_dir, const std::string& abi) {
    const std::string candidates[] = {
        project_dir + "\\obj\\local\\" + abi + "\\allvm_fla_test",
        project_dir + "\\libs\\" + abi + "\\allvm_fla_test"
    };
    for (const auto& candidate : candidates) {
        if (file_exists(candidate)) return candidate;
    }
    return "";
}

static bool write_fla_test_project() {
    std::string test_dir = g_script_dir + "\\test";
    std::string jni_dir = test_dir + "\\jni";
    dir_create(test_dir);
    dir_create(jni_dir);

    const char* main_cpp = R"CPP(#include <cstdio>
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
)CPP";

    const char* android_mk = R"MK(LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := allvm_fla_test
LOCAL_SRC_FILES := main.cpp
LOCAL_CPPFLAGS += -std=c++17 -O2 -fno-exceptions -fno-rtti -mllvm -irobf
include $(BUILD_EXECUTABLE)
)MK";

    const char* application_mk = R"APP(APP_ABI := arm64-v8a armeabi-v7a x86 x86_64
APP_PLATFORM := android-21
APP_OPTIM := release
APP_STL := c++_static
)APP";

    if (!write_text_file(jni_dir + "\\main.cpp", main_cpp)) return false;
    if (!write_text_file(jni_dir + "\\Android.mk", android_mk)) return false;
    if (!write_text_file(jni_dir + "\\Application.mk", application_mk)) return false;
    return true;
}

static bool build_and_run_fla_test(int jobs) {
    printf("\n[Test] Generating flattening test project...\n");
    if (g_ndk_dir.empty() || !file_exists(g_ndk_dir + "\\ndk-build.cmd")) {
        printf("[Error] NDK not found, cannot run emulator test\n");
        return false;
    }

    if (!write_fla_test_project()) {
        printf("[Error] Failed to write test/jni project files\n");
        return false;
    }

    std::string test_dir = g_script_dir + "\\test";
    run_cmd("if exist obj rmdir /s /q obj", test_dir);
    run_cmd("if exist libs rmdir /s /q libs", test_dir);

    char jbuf[16];
    sprintf(jbuf, "%d", jobs);
    std::string ndk_build = g_ndk_dir + "\\ndk-build.cmd";
    std::string ndk_cmd = "\"" + ndk_build + "\" NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk -j" + jbuf;
    int ret = run_cmd(ndk_cmd, test_dir);
    if (ret != 0) {
        printf("[Error] ndk-build test project failed (code: %d)\n", ret);
        return false;
    }

    std::string adb_path = find_adb();
    if (adb_path.empty()) {
        printf("[Error] adb not found in PATH / ANDROID_SDK_ROOT / ANDROID_HOME\n");
        return false;
    }

    std::string serial = find_test_device(adb_path);
    if (serial.empty()) {
        printf("[Error] No connected emulator/device found for adb testing\n");
        return false;
    }
    printf("  -> Using device: %s\n", serial.c_str());

    std::string abi = detect_device_abi(adb_path, serial);
    if (abi.empty()) {
        printf("[Error] Failed to detect device ABI\n");
        return false;
    }
    printf("  -> Device ABI: %s\n", abi.c_str());

    std::string binary = find_test_binary(test_dir, abi);
    if (binary.empty()) {
        printf("[Error] Test binary not found for ABI: %s\n", abi.c_str());
        return false;
    }

    std::string output;
    std::string push_cmd = "\"" + adb_path + "\" -s " + serial + " push \"" + binary + "\" /data/local/tmp/allvm_fla_test";
    ret = run_cmd_capture(push_cmd, output);
    if (!output.empty()) printf("%s", output.c_str());
    if (ret != 0) {
        printf("[Error] adb push failed (code: %d)\n", ret);
        return false;
    }

    std::string exec_cmd = "\"" + adb_path + "\" -s " + serial + " shell \"chmod 755 /data/local/tmp/allvm_fla_test && /data/local/tmp/allvm_fla_test\"";
    output.clear();
    ret = run_cmd_capture(exec_cmd, output);
    if (!output.empty()) printf("%s", output.c_str());
    if (ret != 0 || output.find("ALLVM_TEST_PASS") == std::string::npos) {
        printf("[Error] Emulator test failed\n");
        return false;
    }

    printf("[Done] Emulator flattening test passed\n");
    return true;
}

// ========== compile_interpreter ==========
static bool compile_interpreter(const std::string& target_triple) {
    printf("\n[1/3] Compiling aVMPInterpreter (target: %s)...\n", target_triple.c_str());

    std::string interp_dir = g_script_dir + "\\aVMPInterpreter";
    std::string bc_file = interp_dir + "\\aVMPInterpreter.bc";
    std::string src_file = interp_dir + "\\aVMPInterpreter.c";

    std::string ndk_clang = g_ndk_bin + "\\clang.exe";
    std::string build_clang = g_build_dir + "\\bin\\clang.exe";
    std::string clang_path;

    if (file_exists(ndk_clang)) {
        clang_path = ndk_clang;
    } else if (file_exists(build_clang)) {
        clang_path = build_clang;
    } else {
        printf("[Error] clang not found\n");
        return false;
    }

    std::string cmd = "\"" + clang_path + "\" -O2 -emit-llvm -c \"" + src_file + "\" -o \"" + bc_file + "\" -target " + target_triple;

    int ret = run_cmd(cmd);
    if (ret != 0) {
        printf("[Error] Compilation failed (code: %d)\n", ret);
        return false;
    }
    printf("[Done] aVMPInterpreter compiled successfully\n");
    return true;
}

// ========== generate_vm_h ==========
static bool generate_vm_h() {
    printf("\n[2/3] Generating vm.h...\n");

    std::string bc_file = g_script_dir + "\\aVMPInterpreter\\aVMPInterpreter.bc";
    std::string vm_h = g_script_dir + "\\llvm\\include\\llvm\\Transforms\\Obfuscation\\vm.h";

    if (!file_exists(bc_file)) {
        printf("[Error] %s not found\n", bc_file.c_str());
        return false;
    }

    std::ifstream in(bc_file, std::ios::binary);
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    FILE *f = fopen(vm_h.c_str(), "wb");
    if (!f) {
        printf("[Error] Cannot write to %s\n", vm_h.c_str());
        return false;
    }
    fprintf(f, "#include <string>\n");
    fprintf(f, "#include <vector>\n\n");
    fprintf(f, "static const int binary_ir_length = %zu;\n", data.size());
    fprintf(f, "static const char binary_ir_data[] =\n");

    for (size_t i = 0; i < data.size(); i++) {
        if (i % 16 == 0) fprintf(f, "\"");
        fprintf(f, "\\x%02x", (unsigned int)(unsigned char)data[i]);
        if (i % 16 == 15) fprintf(f, "\"\n");
    }
    if (data.size() % 16 != 0) fprintf(f, "\"");
    fprintf(f, ";\n\n");

    fprintf(f, "static std::vector<char> get_binary_ir() {\n");
    fprintf(f, "    return std::vector<char>(binary_ir_data, binary_ir_data + binary_ir_length);\n");
    fprintf(f, "}\n");
    fclose(f);

    printf("[Done] vm.h generated (size: %zu bytes)\n", data.size());
    return true;
}

// ========== build_zstd ==========
static bool build_zstd() {
    std::string zstd_src_dir = g_zstd_dir + "\\src";
    std::string zstd_build_dir = g_zstd_dir + "\\build";
    std::string zstd_zip = g_zstd_dir + "\\zstd.zip";
    std::string zstd_url = "https://github.com/facebook/zstd/archive/refs/tags/v1.5.6.zip";

    std::string vcvars = find_vs();
    if (vcvars.empty()) {
        printf("[Error] Visual Studio not found!\n");
        return false;
    }

    if (file_exists(zstd_build_dir + "\\lib\\zstd_static.lib")) {
        printf("[Skip] zstd already compiled\n");
        return true;
    }

    printf("\n[Build] zstd...\n");

    dir_create(g_zstd_dir);

    if (!dir_exists(zstd_src_dir)) {
        printf("  -> Downloading zstd source...\n");
        std::string curl_cmd = "curl -L -o \"" + zstd_zip + "\" \"" + zstd_url + "\"";
        int ret = run_cmd(curl_cmd);
        if (ret != 0) {
            printf("[Error] Download failed (code: %d)\n", ret);
            return false;
        }

        printf("  -> Extracting zstd...\n");
        std::string expand_cmd = "powershell -Command \"Expand-Archive -Path '" + zstd_zip + "' -DestinationPath '" + g_zstd_dir + "' -Force\"";
        ret = run_cmd(expand_cmd);
        if (ret != 0) {
            printf("[Error] Extraction failed (code: %d)\n", ret);
            return false;
        }

        std::string extracted_dir = g_zstd_dir + "\\zstd-1.5.6";
        if (!dir_exists(extracted_dir)) {
            printf("[Error] Extracted directory not found\n");
            return false;
        }
        MoveFileExA(extracted_dir.c_str(), zstd_src_dir.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);
        DeleteFileA(zstd_zip.c_str());
    }

    printf("  -> Compiling zstd...\n");
    dir_create(zstd_build_dir);

    std::string cmake_cmd = "cmake -G Ninja -DCMAKE_BUILD_TYPE=Release "
                            "-DZSTD_BUILD_SHARED=OFF -DZSTD_BUILD_STATIC=ON "
                            "-DZSTD_BUILD_PROGRAMS=OFF -DZSTD_BUILD_TESTS=OFF "
                            "\"" + zstd_src_dir + "\\build\\cmake\"";

    int ret = run_cmd_vcvars(vcvars, cmake_cmd, zstd_build_dir);
    if (ret != 0) {
        printf("[Error] CMake configuration failed (code: %d)\n", ret);
        return false;
    }

    std::string ninja_cmd = "ninja";
    ret = run_cmd_vcvars(vcvars, ninja_cmd, zstd_build_dir);
    if (ret != 0) {
        printf("[Error] Compilation failed (code: %d)\n", ret);
        return false;
    }

    printf("[Done] zstd compiled successfully\n");
    return true;
}

// ========== cmake_configure ==========
static bool cmake_configure() {
    printf("\n[CMake] Configuring...\n");

    std::string vcvars = find_vs();
    if (vcvars.empty()) {
        printf("[Error] Visual Studio not found!\n");
        return false;
    }

    dir_create(g_build_dir);

    std::string cmake_cache = g_build_dir + "\\CMakeCache.txt";
    if (file_exists(cmake_cache)) {
        DeleteFileA(cmake_cache.c_str());
    }

    std::string zstd_include_dir = g_zstd_dir + "\\src\\lib";
    std::string zstd_lib = g_zstd_dir + "\\build\\lib\\zstd_static.lib";
    std::string zlib_include_dir = g_zlib_dir;
    std::string zlib_lib = g_zlib_dir + "\\build\\libzs.a";
    std::replace(zlib_include_dir.begin(), zlib_include_dir.end(), '\\', '/');
    std::replace(zlib_lib.begin(), zlib_lib.end(), '\\', '/');
    std::string cmake_cmd = "cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=/utf-8 "
                            "-DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_EH=ON "
                            "-DLLVM_ENABLE_PROJECTS=\"llvm;clang;lld\" "
                            "-DLLVM_TARGETS_TO_BUILD=\"AArch64;ARM;X86\" "
                            "-DLLVM_ENABLE_ZSTD=FORCE_ON "
                            "-DLLVM_USE_STATIC_ZSTD=ON "
                            "-Dzstd_INCLUDE_DIR=\"" + zstd_include_dir + "\" "
                            "-Dzstd_LIBRARY=\"" + zstd_lib + "\" "
                            "-Dzstd_STATIC_LIBRARY=\"" + zstd_lib + "\" "
                            "-DLLVM_ENABLE_ZLIB=FORCE_ON "
                            "-DLLVM_USE_STATIC_ZLIB=ON "
                            "-DZLIB_INCLUDE_DIR=\"" + zlib_include_dir + "\" "
                            "-DZLIB_LIBRARY=\"" + zlib_lib + "\" "
                            "../llvm";

    int ret = run_cmd_vcvars(vcvars, cmake_cmd, g_build_dir);
    if (ret != 0) {
        printf("[Error] CMake configuration failed (code: %d)\n", ret);
        return false;
    }

    printf("[Done] CMake configured successfully\n");
    return true;
}

// ========== build_ollvm ==========
static bool build_ollvm(const std::string& targets, int jobs) {
    printf("\n[3/3] Building OLLVM (parallel: %d)...\n", jobs);

    if (!file_exists(g_build_dir)) {
        printf("[Error] Build directory not found\n");
        return false;
    }

    std::string vcvars = find_vs();
    if (vcvars.empty()) {
        printf("[Error] Visual Studio not found!\n");
        return false;
    }

    char jbuf[16];
    sprintf(jbuf, "%d", jobs);
    std::string ninja_cmd = std::string("ninja -j") + jbuf + " " + targets;

    int ret = run_cmd_vcvars(vcvars, ninja_cmd, g_build_dir);
    if (ret != 0) {
        printf("[Error] Build failed (code: %d)\n", ret);
        return false;
    }

    printf("[Done] OLLVM built successfully\n");
    return true;
}

// ========== replace_ndk_clang ==========
static bool replace_ndk_clang() {
    printf("\n[Replace] NDK toolchain...\n");

    if (g_ndk_bin.empty() || !file_exists(g_ndk_bin)) {
        printf("[Skip] NDK not found\n");
        return true;
    }

    std::string build_bin = g_build_dir + "\\bin";

    const char* files[] = {
        "clang.exe", "clang++.exe", "clang-cl.exe", "clang-cpp.exe",
        "lld.exe", "llvm-strip.exe", "llvm-objcopy.exe"
    };

    for (auto name : files) {
        std::string src = build_bin + "\\" + name;
        std::string dst = g_ndk_bin + "\\" + name;

        if (!file_exists(src)) continue;

        std::string backup = dst + ".bak";
        if (!file_exists(backup) && file_exists(dst)) {
            copy_file(dst, backup);
        }

        copy_file(src, dst);
        printf("  -> %s OK\n", name);
    }

    std::string lld_src = build_bin + "\\lld.exe";
    std::string lld_dst = g_ndk_bin + "\\ld.lld.exe";
    if (file_exists(lld_src)) {
        std::string backup = lld_dst + ".bak";
        if (!file_exists(backup) && file_exists(lld_dst)) {
            copy_file(lld_dst, backup);
        }
        copy_file(lld_src, lld_dst);
        printf("  -> ld.lld.exe OK\n");
    }

    // Copy ollvm-ui.exe to NDK root directory
    std::string ui_src = build_bin + "\\ollvm-ui.exe";
    std::string ui_dst = g_ndk_dir + "\\ollvm-ui.exe";
    if (file_exists(ui_src) && !g_ndk_dir.empty()) {
        copy_file(ui_src, ui_dst);
        printf("  -> ollvm-ui.exe OK\n");
    }

    printf("[Done] NDK toolchain replaced\n");
    return true;
}

// ========== build_apk ==========
static bool build_apk() {
    printf("\n[Build] APK...\n");

    if (!dir_exists(g_acode_dir)) {
        printf("[Error] Acode directory not found: %s\n", g_acode_dir.c_str());
        return false;
    }

    dir_create(g_apk_output_dir);

    std::string platforms_dir = g_acode_dir + "\\platforms";
    if (!dir_exists(platforms_dir)) {
        printf("  -> Adding Cordova Android platform...\n");
        run_cmd("npx cordova platform add android", g_acode_dir);
    }

    printf("  -> Building Web resources...\n");
    int ret = run_cmd("npm run build", g_acode_dir);
    if (ret != 0) {
        printf("[Error] Web build failed (code: %d)\n", ret);
        return false;
    }

    printf("  -> Building Android APK...\n");
    ret = run_cmd("npx cordova build android", g_acode_dir);
    if (ret != 0) {
        printf("[Error] Cordova build failed (code: %d)\n", ret);
        return false;
    }

    std::string apk_src = g_acode_dir + "\\platforms\\android\\app\\build\\outputs\\apk\\debug\\app-debug.apk";
    std::string apk_dst = g_apk_output_dir + "\\Acode-OLLVM.apk";

    if (file_exists(apk_src)) {
        copy_file(apk_src, apk_dst);
        printf("[Done] APK saved: %s\n", apk_dst.c_str());
    } else {
        printf("[Warning] APK not found\n");
    }

    return true;
}

// ========== build_apk_release ==========
static bool build_apk_release() {
    printf("\n[Build] Release APK...\n");

    if (!dir_exists(g_acode_dir)) {
        printf("[Error] Acode directory not found: %s\n", g_acode_dir.c_str());
        return false;
    }

    dir_create(g_apk_output_dir);

    printf("  -> Building Web resources...\n");
    int ret = run_cmd("npm run build", g_acode_dir);
    if (ret != 0) {
        printf("[Error] Build failed (code: %d)\n", ret);
        return false;
    }

    printf("  -> Building Release APK...\n");
    ret = run_cmd("npx cordova build android --release", g_acode_dir);
    if (ret != 0) {
        printf("[Error] Release build failed (code: %d)\n", ret);
        return false;
    }

    std::string apk_src = g_acode_dir + "\\platforms\\android\\app\\build\\outputs\\apk\\release\\app-release-unsigned.apk";
    std::string apk_dst = g_apk_output_dir + "\\Acode-OLLVM-release-unsigned.apk";

    if (file_exists(apk_src)) {
        copy_file(apk_src, apk_dst);
        printf("[Done] Release APK saved: %s\n", apk_dst.c_str());
    }

    return true;
}

// ========== main ==========
int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    init_paths();

    printf("\n");
    printf("  +------------------------------------------+\n");
    printf("  |       OLLVM Build Script v21.x          |\n");
    printf("  +------------------------------------------+\n");

    std::string target_triple = "aarch64-linux-android";
    int jobs = 32;
    bool build_apk_flag = false;
    bool build_apk_release_flag = false;

    bool step_zstd        = false;
    bool step_cmake       = true;
    bool step_interpreter = true;
    bool step_vmh         = true;
    bool step_build       = true;
    bool step_test        = true;

    std::string ninja_targets = "clang lld llvm-strip llvm-objcopy llvm-dis ollvm-ui";

    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "--target" && i + 1 < argc) {
            target_triple = argv[++i];
        } else if (arg == "--skip-build") {
            step_cmake = step_interpreter = step_vmh = step_build = step_test = false;
        } else if (arg == "-j" && i + 1 < argc) {
            jobs = atoi(argv[++i]);
        } else if (arg == "--jobs" && i + 1 < argc) {
            jobs = atoi(argv[++i]);
        } else if (arg == "--build" && i + 1 < argc) {
            ninja_targets = argv[++i];
        } else if (arg == "--skip" && i + 1 < argc) {
            std::string skip_arg(argv[++i]);
            step_zstd        = (skip_arg.find("zstd") == std::string::npos);
            step_cmake       = (skip_arg.find("cmake") == std::string::npos);
            step_interpreter = (skip_arg.find("interpreter") == std::string::npos);
            step_vmh         = (skip_arg.find("vmh") == std::string::npos);
            step_build       = (skip_arg.find("build") == std::string::npos);
            step_test        = (skip_arg.find("test") == std::string::npos);
        } else if (arg == "--only" && i + 1 < argc) {
            std::string only_arg(argv[++i]);
            step_zstd        = (only_arg == "zstd");
            step_cmake       = (only_arg == "cmake");
            step_interpreter = (only_arg == "interpreter");
            step_vmh         = (only_arg == "vmh");
            step_build       = (only_arg == "build");
            step_test        = (only_arg == "test");
        } else if (arg == "--apk") {
            build_apk_flag = true;
        } else if (arg == "--apk-release") {
            build_apk_release_flag = true;
        } else if (arg == "--all") {
            build_apk_flag = true;
        }
    }

    if (step_zstd) {
        if (!build_zstd()) return 1;
    }
    if (step_cmake) {
        if (!cmake_configure()) return 1;
    }
    if (step_interpreter) {
        if (!compile_interpreter(target_triple)) return 1;
    }
    if (step_vmh) {
        if (!generate_vm_h()) return 1;
    }
    if (step_build) {
        if (!build_ollvm(ninja_targets, jobs)) return 1;
        if (!replace_ndk_clang()) return 1;
    }
    if (step_test) {
        if (!build_and_run_fla_test(jobs)) return 1;
    }

    if (build_apk_flag || build_apk_release_flag) {
        if (build_apk_release_flag) {
            if (!build_apk_release()) return 1;
        } else {
            if (!build_apk()) return 1;
        }
    }

    printf("\n");
    printf("  +------------------------------------------+\n");
    printf("  |            Build Complete!               |\n");
    printf("  +------------------------------------------+\n\n");
    return 0;
}
