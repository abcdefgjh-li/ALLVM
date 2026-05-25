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
static std::string g_ndk_bin;
static std::string g_acode_dir;
static std::string g_apk_output_dir;

static void init_paths() {
    char buf[MAX_PATH];
    GetModuleFileNameA(NULL, buf, MAX_PATH);
    std::string exe_path(buf);
    size_t pos = exe_path.find_last_of("\\/");
    g_script_dir = exe_path.substr(0, pos);
    g_build_dir = g_script_dir + "\\build-windows";
    g_ndk_bin = g_script_dir + "\\android-ndk-r30-beta1-windows\\toolchains\\llvm\\prebuilt\\windows-x86_64\\bin";
    g_acode_dir = g_script_dir + "\\apkUI";
    g_apk_output_dir = g_build_dir + "\\bin";
}

static bool file_exists(const std::string& path) {
    return GetFileAttributesA(path.c_str()) != INVALID_FILE_ATTRIBUTES;
}

static bool dir_exists(const std::string& path) {
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
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
    fprintf(f, "@echo off\r\n");
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
    fprintf(f, "@echo off\r\n");
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
    printf("\n============================================================\n");
    printf("Compiling aVMPInterpreter (no obfuscation)...\n");
    printf("Target: %s\n", target_triple.c_str());
    printf("============================================================\n");
    
    std::string interp_dir = g_script_dir + "\\aVMPInterpreter";
    std::string bc_file = interp_dir + "\\aVMPInterpreter.bc";
    std::string src_file = interp_dir + "\\aVMPInterpreter.c";
    
    std::string ndk_clang = g_ndk_bin + "\\clang.exe";
    std::string build_clang = g_build_dir + "\\bin\\clang.exe";
    std::string clang_path;
    
    if (file_exists(ndk_clang)) {
        clang_path = ndk_clang;
        printf("[INFO] Using NDK clang: %s\n", clang_path.c_str());
    } else if (file_exists(build_clang)) {
        clang_path = build_clang;
        printf("[INFO] Using build clang: %s\n", clang_path.c_str());
    } else {
        printf("Error: clang not found\n");
        return false;
    }
    
    std::string cmd = "\"" + clang_path + "\" -O2 -emit-llvm -c \"" + src_file + "\" -o \"" + bc_file + "\" -target " + target_triple;
    printf("Running: %s\n", cmd.c_str());
    
    int ret = run_cmd(cmd);
    if (ret != 0) {
        printf("Compilation failed with code %d\n", ret);
        return false;
    }
    printf("Compilation successful!\n");
    return true;
}

// ========== generate_vm_h ==========
static bool generate_vm_h() {
    printf("\n============================================================\n");
    printf("Generating vm.h...\n");
    printf("============================================================\n");
    
    std::string bc_file = g_script_dir + "\\aVMPInterpreter\\aVMPInterpreter.bc";
    std::string vm_h = g_script_dir + "\\llvm\\include\\llvm\\Transforms\\Obfuscation\\vm.h";
    
    if (!file_exists(bc_file)) {
        printf("Error: %s not found\n", bc_file.c_str());
        return false;
    }
    
    std::ifstream in(bc_file, std::ios::binary);
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    
    FILE *f = fopen(vm_h.c_str(), "wb");
    if (!f) {
        printf("Error: cannot write %s\n", vm_h.c_str());
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
    
    printf("Generated vm.h with binary_ir_length = %zu\n", data.size());
    return true;
}

// ========== cmake_configure ==========
static bool cmake_configure() {
    printf("\n============================================================\n");
    printf("CMake Configure (Windows)\n");
    printf("============================================================\n");
    printf("Build Dir: %s\n", g_build_dir.c_str());
    printf("============================================================\n");
    
    std::string vcvars = find_vs();
    if (vcvars.empty()) {
        printf("Error: Visual Studio not found!\n");
        return false;
    }
    printf("[INFO] Using Visual Studio: %s\n", vcvars.c_str());
    
    dir_create(g_build_dir);
    
    std::string cmake_cmd = "cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=/utf-8 "
                            "-DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_EH=ON "
                            "-DLLVM_ENABLE_PROJECTS=\"llvm;clang;lld\" "
                            "-DLLVM_TARGETS_TO_BUILD=\"AArch64;ARM;X86\" ../llvm";
    
    printf("\n[INFO] Running CMake configure...\n");
    printf("%s\n\n", cmake_cmd.c_str());
    
    int ret = run_cmd_vcvars(vcvars, cmake_cmd, g_build_dir);
    if (ret != 0) {
        printf("\nCMake configure failed with code %d\n", ret);
        return false;
    }
    
    printf("\n============================================================\n");
    printf("[SUCCESS] CMake configure completed!\n");
    printf("============================================================\n");
    return true;
}

// ========== build_ollvm ==========
static bool build_ollvm(int jobs) {
    printf("\n============================================================\n");
    printf("OLLVM Build (Ninja)\n");
    printf("============================================================\n");
    printf("Build Dir: %s\n", g_build_dir.c_str());
    printf("Jobs: %d\n", jobs);
    printf("============================================================\n");
    
    if (!file_exists(g_build_dir)) {
        printf("Error: Build directory not found\n");
        return false;
    }
    
    std::string vcvars = find_vs();
    if (vcvars.empty()) {
        printf("Error: Visual Studio not found!\n");
        return false;
    }
    
    char jbuf[16];
    sprintf(jbuf, "%d", jobs);
    std::string ninja_cmd = std::string("ninja -j") + jbuf + " clang llvm-strip llvm-objcopy ollvm-ui";
    
    printf("\n[INFO] Building with %s...\n", ninja_cmd.c_str());
    
    int ret = run_cmd_vcvars(vcvars, ninja_cmd, g_build_dir);
    if (ret != 0) {
        printf("\nBuild failed with code %d\n", ret);
        return false;
    }

    printf("\n============================================================\n");
    printf("[SUCCESS] Build completed!\n");
    printf("============================================================\n");
    return true;
}

// ========== replace_ndk_clang ==========
static bool replace_ndk_clang() {
    printf("\n============================================================\n");
    printf("Replacing NDK clang with OLLVM...\n");
    printf("============================================================\n");
    
    if (!file_exists(g_ndk_bin)) {
        printf("[SKIP] NDK not found at %s\n", g_ndk_bin.c_str());
        return true;
    }
    
    std::string build_bin = g_build_dir + "\\bin";
    
    const char* files[] = {
        "clang.exe", "clang++.exe", "clang-cl.exe", "clang-cpp.exe",
        "llvm-strip.exe", "llvm-objcopy.exe"
    };
    
    for (auto name : files) {
        std::string src = build_bin + "\\" + name;
        std::string dst = g_ndk_bin + "\\" + name;
        
        if (!file_exists(src)) {
            printf("  [SKIP] %s not found in build dir\n", name);
            continue;
        }
        
        std::string backup = dst + ".bak";
        if (!file_exists(backup) && file_exists(dst)) {
            printf("  Backing up %s...\n", name);
            copy_file(dst, backup);
        }
        
        copy_file(src, dst);
        printf("  %s - OK\n", name);
    }
    
    printf("\n[SUCCESS] NDK clang replaced with OLLVM!\n");
    return true;
}

// ========== build_apk ==========
static bool build_apk() {
    printf("\n============================================================\n");
    printf("Building Acode APK with Cordova...\n");
    printf("============================================================\n");
    printf("Acode Dir: %s\n", g_acode_dir.c_str());
    printf("Output Dir: %s\n", g_apk_output_dir.c_str());
    printf("============================================================\n");
    
    if (!dir_exists(g_acode_dir)) {
        printf("Error: Acode directory not found at %s\n", g_acode_dir.c_str());
        return false;
    }
    
    dir_create(g_apk_output_dir);
    
    std::string platforms_dir = g_acode_dir + "\\platforms";
    if (!dir_exists(platforms_dir)) {
        printf("\n[0/3] Adding Cordova Android platform...\n");
        int ret = run_cmd("npx cordova platform add android", g_acode_dir);
        if (ret != 0) {
            printf("[WARN] Platform add failed with code %d, continuing...\n", ret);
        }
    }
    
    printf("\n[1/2] Building web assets with rspack...\n");
    int ret = run_cmd("npm run build", g_acode_dir);
    if (ret != 0) {
        printf("rspack build failed with code %d\n", ret);
        return false;
    }
    
    printf("\n[2/2] Building Android APK with Cordova...\n");
    ret = run_cmd("npx cordova build android", g_acode_dir);
    if (ret != 0) {
        printf("Cordova build failed with code %d\n", ret);
        return false;
    }
    
    std::string apk_src = g_acode_dir + "\\platforms\\android\\app\\build\\outputs\\apk\\debug\\app-debug.apk";
    std::string apk_dst = g_apk_output_dir + "\\Acode-OLLVM.apk";
    
    if (file_exists(apk_src)) {
        copy_file(apk_src, apk_dst);
        printf("\n[SUCCESS] APK copied to: %s\n", apk_dst.c_str());
    } else {
        printf("\n[WARN] APK not found at expected location\n");
    }
    
    printf("\n============================================================\n");
    printf("[SUCCESS] APK build completed!\n");
    printf("============================================================\n");
    return true;
}

// ========== build_apk_release ==========
static bool build_apk_release() {
    printf("\n============================================================\n");
    printf("Building Acode Release APK...\n");
    printf("============================================================\n");
    
    if (!dir_exists(g_acode_dir)) {
        printf("Error: Acode directory not found at %s\n", g_acode_dir.c_str());
        return false;
    }
    
    dir_create(g_apk_output_dir);
    
    printf("\n[1/2] Building web assets...\n");
    int ret = run_cmd("npm run build", g_acode_dir);
    if (ret != 0) {
        printf("Build failed with code %d\n", ret);
        return false;
    }
    
    printf("\n[2/2] Building release APK...\n");
    ret = run_cmd("npx cordova build android --release", g_acode_dir);
    if (ret != 0) {
        printf("Cordova release build failed with code %d\n", ret);
        return false;
    }
    
    std::string apk_src = g_acode_dir + "\\platforms\\android\\app\\build\\outputs\\apk\\release\\app-release-unsigned.apk";
    std::string apk_dst = g_apk_output_dir + "\\Acode-OLLVM-release-unsigned.apk";
    
    if (file_exists(apk_src)) {
        copy_file(apk_src, apk_dst);
        printf("\n[SUCCESS] Release APK copied to: %s\n", apk_dst.c_str());
    }
    
    return true;
}

// ========== main ==========
int main(int argc, char* argv[]) {
    init_paths();
    
    printf("============================================================\n");
    printf("OLLVM Build Script\n");
    printf("============================================================\n");
    
    std::string target_triple = "x86_64-pc-windows-msvc";
    bool skip_build = false;
    bool build_apk_flag = false;
    bool build_apk_release_flag = false;
    int jobs = 32;
    
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "--target" && i + 1 < argc) {
            target_triple = argv[++i];
        } else if (arg == "--skip-build") {
            skip_build = true;
        } else if (arg == "-j" && i + 1 < argc) {
            jobs = atoi(argv[++i]);
        } else if (arg == "--apk") {
            build_apk_flag = true;
        } else if (arg == "--apk-release") {
            build_apk_release_flag = true;
        } else if (arg == "--all") {
            build_apk_flag = true;
        }
    }
    
    if (build_apk_flag || build_apk_release_flag) {
        if (!skip_build) {
            if (!cmake_configure()) return 1;
            if (!compile_interpreter(target_triple)) return 1;
        }
        
        if (!generate_vm_h()) return 1;
        
        if (!skip_build) {
            if (!build_ollvm(jobs)) return 1;
            if (!replace_ndk_clang()) return 1;
        }
        
        if (build_apk_release_flag) {
            if (!build_apk_release()) return 1;
        } else {
            if (!build_apk()) return 1;
        }
    } else {
        if (!skip_build) {
            if (!cmake_configure()) return 1;
            if (!compile_interpreter(target_triple)) return 1;
        }
        
        if (!generate_vm_h()) return 1;
        
        if (!skip_build) {
            if (!build_ollvm(jobs)) return 1;
            if (!replace_ndk_clang()) return 1;
        }
    }
    
    printf("\n============================================================\n");
    printf("All steps completed successfully!\n");
    printf("============================================================\n");
    return 0;
}
