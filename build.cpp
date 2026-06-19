#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

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

    std::string ninja_targets = "clang lld llvm-strip llvm-objcopy llvm-dis ollvm-ui";

    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "--target" && i + 1 < argc) {
            target_triple = argv[++i];
        } else if (arg == "--skip-build") {
            step_cmake = step_interpreter = step_vmh = step_build = false;
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
        } else if (arg == "--only" && i + 1 < argc) {
            std::string only_arg(argv[++i]);
            step_zstd        = (only_arg == "zstd");
            step_cmake       = (only_arg == "cmake");
            step_interpreter = (only_arg == "interpreter");
            step_vmh         = (only_arg == "vmh");
            step_build       = (only_arg == "build");
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
