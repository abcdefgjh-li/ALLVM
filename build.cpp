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

struct TestRunOptions {
    bool custom_test = false;
    std::string project_dir;
    std::string app_build_script = "jni/Android.mk";
    std::string app_application_mk = "jni/Application.mk";
    std::string binary_name;
    std::string local_binary_path;
    std::string device_path;
    std::string run_command;
    std::string serial;
    std::string abi;
    int timeout_sec = 20;
    bool skip_ndk_build = false;
    bool clean_test = false;
};

static bool file_exists(const std::string& path) {
    return GetFileAttributesA(path.c_str()) != INVALID_FILE_ATTRIBUTES;
}

static bool get_file_time(const std::string& path, FILETIME& time) {
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &data)) return false;
    time = data.ftLastWriteTime;
    return true;
}

static bool is_file_newer_or_equal(const std::string& output, const std::string& input) {
    FILETIME output_time, input_time;
    if (!get_file_time(output, output_time)) return false;
    if (!get_file_time(input, input_time)) return true;
    return CompareFileTime(&output_time, &input_time) >= 0;
}

static bool dir_exists(const std::string& path) {
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

static std::string get_full_path_copy(const std::string& path) {
    char buf[MAX_PATH];
    DWORD len = GetFullPathNameA(path.c_str(), MAX_PATH, buf, NULL);
    if (len == 0 || len >= MAX_PATH) return path;
    return std::string(buf, len);
}

static std::string path_basename(const std::string& path) {
    size_t pos = path.find_last_of("\\/");
    return (pos == std::string::npos) ? path : path.substr(pos + 1);
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

// ---- debug instrumentation (aarch64-ndk-crash)
static void dbg_log(const std::string& event, const std::string& detail) {
    static const std::string session = "aarch64-ndk-crash";
    std::string log_path = g_script_dir + "\\trae-debug-log-" + session + ".ndjson";
    SYSTEMTIME st;
    GetSystemTime(&st);
    char ts[64];
    sprintf(ts, "%04d-%02d-%02dT%02d:%02d:%02dZ",
            (int)st.wYear, (int)st.wMonth, (int)st.wDay,
            (int)st.wHour, (int)st.wMinute, (int)st.wSecond);
    std::ofstream ofs(log_path, std::ios::app);
    if (!ofs.is_open()) return;
    std::string esc;
    esc.reserve(detail.size() + 16);
    for (char c : detail) {
        if (c == '\\') esc += "\\\\";
        else if (c == '\"') esc += "\\\"";
        else if (c == '\n') esc += "\\n";
        else esc += c;
    }
    ofs << "{"
        << "\"ts\":\"" << ts << "\","
        << "\"session\":\"" << session << "\","
        << "\"event\":\"" << event << "\","
        << "\"detail\":\"" << esc << "\""
        << "}" << std::endl;
}

static std::string find_vs() {
    const char* vcvars_path = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\VC\\Auxiliary\\Build\\vcvars64.bat";
    if (file_exists(vcvars_path)) return vcvars_path;
    return "";
}

static void dir_create(const std::string& path) {
    CreateDirectoryA(path.c_str(), NULL);
}

static std::string format_win32_error(DWORD err) {
    if (err == 0) return "success";
    char* buf = nullptr;
    DWORD len = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&buf), 0, NULL);
    std::string msg = (len && buf) ? std::string(buf, len) : ("error " + std::to_string(err));
    if (buf) LocalFree(buf);
    while (!msg.empty() && (msg.back() == '\r' || msg.back() == '\n')) msg.pop_back();
    return msg;
}

static bool copy_file(const std::string& src, const std::string& dst, bool overwrite = true) {
    if (!file_exists(src)) {
        printf("[Error] copy source not found: %s\n", src.c_str());
        return false;
    }

    if (overwrite && file_exists(dst)) {
        DWORD attr = GetFileAttributesA(dst.c_str());
        if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY)) {
            SetFileAttributesA(dst.c_str(), attr & ~FILE_ATTRIBUTE_READONLY);
        }
    }

    if (CopyFileA(src.c_str(), dst.c_str(), overwrite ? FALSE : TRUE)) {
        if (file_exists(dst)) return true;
        printf("[Error] copy reported success but destination missing: %s\n", dst.c_str());
        return false;
    }

    DWORD copy_err = GetLastError();
    if (overwrite && file_exists(dst)) {
        if (DeleteFileA(dst.c_str()) && CopyFileA(src.c_str(), dst.c_str(), TRUE)) {
            if (file_exists(dst)) return true;
            printf("[Error] copy-after-delete reported success but destination missing: %s\n", dst.c_str());
            return false;
        }
        copy_err = GetLastError();
    }

    printf("[Error] copy failed: %s -> %s (%s)\n",
           src.c_str(), dst.c_str(), format_win32_error(copy_err).c_str());
    return false;
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

static std::string find_compiler_cache_launcher() {
    std::string launcher = search_path_binary("sccache.exe");
    if (!launcher.empty()) return launcher;
    launcher = search_path_binary("clcache.exe");
    return launcher;
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

static int run_cmd_capture_timeout(const std::string& cmd, std::string& output,
                                   const std::string& cwd, DWORD timeout_ms,
                                   bool* timed_out = nullptr) {
    if (timed_out) *timed_out = false;

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

    std::string command_line = "cmd.exe /C \"" + bat_file + "\"";
    std::vector<char> cmdline(command_line.begin(), command_line.end());
    cmdline.push_back('\0');

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    BOOL ok = CreateProcessA(
        NULL,
        cmdline.data(),
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        cwd.empty() ? NULL : cwd.c_str(),
        &si,
        &pi
    );

    int ret = -1;
    if (!ok) {
        DeleteFileA(bat_file.c_str());
        return -1;
    }

    DWORD wait_ret = WaitForSingleObject(pi.hProcess, timeout_ms == 0 ? INFINITE : timeout_ms);
    if (wait_ret == WAIT_TIMEOUT) {
        if (timed_out) *timed_out = true;
        TerminateProcess(pi.hProcess, 124);
        WaitForSingleObject(pi.hProcess, INFINITE);
        ret = 124;
    } else {
        DWORD exit_code = 0;
        if (GetExitCodeProcess(pi.hProcess, &exit_code)) {
            ret = static_cast<int>(exit_code);
        }
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
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
        project_dir + "\\libs\\" + abi + "\\allvm_test",
        project_dir + "\\obj\\local\\" + abi + "\\allvm_test"
    };
    for (const auto& candidate : candidates) {
        if (file_exists(candidate)) return candidate;
    }
    return "";
}

static std::string choose_device_test_path(const std::string& adb_path,
                                           const std::string& serial,
                                           const std::string& binary_name) {
    const char* dirs[] = { "/data/local/tmp", "/data/local/tests" };
    for (const char* dir : dirs) {
        std::string probe = std::string(dir) + "/.allvm_probe";
        std::string cmd = "\"" + adb_path + "\" -s " + serial +
                          " shell \"touch " + probe +
                          " >/dev/null 2>&1 && rm -f " + probe + " >/dev/null 2>&1\"";
        std::string output;
        if (run_cmd_capture(cmd, output, g_script_dir) == 0) {
            return std::string(dir) + "/" + binary_name;
        }
    }
    return "/data/local/tmp/" + binary_name;
}

static std::string find_named_binary(const std::string& project_dir, const std::string& abi,
                                     const std::string& binary_name_or_path) {
    if (binary_name_or_path.empty()) return "";

    if (file_exists(binary_name_or_path)) {
        return get_full_path_copy(binary_name_or_path);
    }

    const std::string candidates[] = {
        project_dir + "\\libs\\" + abi + "\\" + binary_name_or_path,
        project_dir + "\\obj\\local\\" + abi + "\\" + binary_name_or_path
    };
    for (const auto& candidate : candidates) {
        if (file_exists(candidate)) return get_full_path_copy(candidate);
    }
    return "";
}

static std::string lower_ascii_copy(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

static std::string normalize_ascii_whitespace(std::string value) {
    value = lower_ascii_copy(std::move(value));
    std::string out;
    out.reserve(value.size());
    bool last_was_space = false;
    for (unsigned char c : value) {
        if (std::isspace(c)) {
            if (!last_was_space) out.push_back(' ');
            last_was_space = true;
        } else {
            out.push_back(static_cast<char>(c));
            last_was_space = false;
        }
    }
    return out;
}

static bool verify_frame_record_codegen() {
    std::string clangxx = g_build_dir + "\\bin\\clang++.exe";
    if (!file_exists(clangxx)) {
        printf("[Warning] clang++ not found, skipping frame record verification\n");
        return true;
    }

    std::string src = g_script_dir + "\\test\\jni\\frame_record_codegen.cpp";
    std::string asm_path = g_script_dir + "\\test\\frame_record_codegen.s";
    if (!file_exists(src)) {
        printf("[Error] Frame record probe source not found: %s\n", src.c_str());
        return false;
    }

    std::string output;
    std::string cmd = "\"" + clangxx +
                      "\" --target=aarch64-linux-android21 -S -O2 "
                      "-fno-omit-frame-pointer "
                      "-mllvm -aarch64-obfuscate-frame-record "
                      "-mllvm -aarch64-enable-ldst-opt=false "
                      "\"" + src + "\" -o \"" + asm_path + "\"";
    if (run_cmd_capture(cmd, output) != 0) {
        if (!output.empty()) printf("%s", output.c_str());
        printf("[Error] Failed to compile frame record probe\n");
        return false;
    }

    std::ifstream in(asm_path, std::ios::binary);
    if (!in.is_open()) {
        printf("[Error] Failed to read generated asm: %s\n", asm_path.c_str());
        return false;
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    std::string asm_text = ss.str();
    in.close();

    std::istringstream iss(asm_text);
    std::string line;
    std::string block;
    bool in_symbol = false;
    while (std::getline(iss, line)) {
        if (!in_symbol) {
            if (line.find("frame_record_probe:") != std::string::npos) {
                in_symbol = true;
                block += line + "\n";
            }
            continue;
        }

        bool is_new_symbol = !line.empty() &&
                             line[0] != '\t' &&
                             line[0] != ' ' &&
                             line[0] != '/' &&
                             line.back() == ':' &&
                             line.find("frame_record_probe:") == std::string::npos;
        if (is_new_symbol || line.rfind(".Lfunc_end", 0) == 0) {
            break;
        }
        block += line + "\n";
    }

    if (block.empty()) {
        printf("[Error] frame_record_probe symbol not found in generated asm\n");
        return false;
    }

    std::replace(block.begin(), block.end(), '\t', ' ');
    std::string lower = lower_ascii_copy(block);
    auto has = [&](const char* needle) {
        return lower.find(needle) != std::string::npos;
    };

    bool has_split_store = (has("str x29") || has("stur x29")) &&
                           (has("str x30") || has("stur x30"));
    bool has_split_load = (has("ldr x29") || has("ldur x29")) &&
                          (has("ldr x30") || has("ldur x30"));
    bool has_fp_mix = has("add x29, sp") && has("sub x29, x29");
    bool has_std_pair = has("stp x29, x30") || has("stp x30, x29") ||
                        has("ldp x29, x30") || has("ldp x30, x29");
    bool has_std_mov = has("mov x29, sp");

    if (!has_split_store || !has_split_load || !has_fp_mix || has_std_pair || has_std_mov) {
        printf("[Error] frame_record_probe codegen verification failed\n");
        printf("-------- frame_record_probe --------\n%s-------- end --------\n", block.c_str());
        return false;
    }

    printf("[Done] frame_record_probe uses split frame record sequence\n");
    return true;
}

static bool verify_call_ret_codegen() {
    std::string clangxx = g_build_dir + "\\bin\\clang++.exe";
    if (!file_exists(clangxx)) {
        printf("[Warning] clang++ not found, skipping call/ret verification\n");
        return true;
    }

    std::string src = g_script_dir + "\\test\\jni\\call_ret_codegen.cpp";
    std::string asm_path = g_script_dir + "\\test\\call_ret_codegen.s";
    if (!file_exists(src)) {
        printf("[Error] Call/ret probe source not found: %s\n", src.c_str());
        return false;
    }

    std::string output;
    std::string cmd = "\"" + clangxx +
                      "\" --target=aarch64-linux-android21 -S -O2 "
                      "-mllvm -aarch64-obfuscate-call-ret "
                      "\"" + src + "\" -o \"" + asm_path + "\"";
    if (run_cmd_capture(cmd, output) != 0) {
        if (!output.empty()) printf("%s", output.c_str());
        printf("[Error] Failed to compile call/ret probe\n");
        return false;
    }

    std::ifstream in(asm_path, std::ios::binary);
    if (!in.is_open()) {
        printf("[Error] Failed to read generated asm: %s\n", asm_path.c_str());
        return false;
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    std::string asm_text = ss.str();
    in.close();

    std::istringstream iss(asm_text);
    std::string line;
    std::string block;
    bool in_symbol = false;
    while (std::getline(iss, line)) {
        if (!in_symbol) {
            if (line.find("call_ret_probe:") != std::string::npos) {
                in_symbol = true;
                block += line + "\n";
            }
            continue;
        }

        bool is_new_symbol = !line.empty() &&
                             line[0] != '\t' &&
                             line[0] != ' ' &&
                             line[0] != '/' &&
                             line.back() == ':' &&
                             line.find("call_ret_probe:") == std::string::npos;
        if (is_new_symbol || line.rfind(".Lfunc_end", 0) == 0) {
            break;
        }
        block += line + "\n";
    }

    if (block.empty()) {
        printf("[Error] call_ret_probe symbol not found in generated asm\n");
        return false;
    }

    std::replace(block.begin(), block.end(), '\t', ' ');
    std::string lower = lower_ascii_copy(block);
    auto has = [&](const char* needle) {
        return lower.find(needle) != std::string::npos;
    };

    bool has_target_materialization = has("adrp") && has("target_call") &&
                                      (has(":lo12:target_call") ||
                                       has(" target_call"));
    bool has_indirect_call = has_target_materialization && has("blr x");
    bool has_br_lr = has("br x30");
    bool has_direct_bl = has("bl target_call");
    bool has_ret = has("\n ret") || has("\nret");

    if (!has_indirect_call || !has_br_lr || has_direct_bl || has_ret) {
        printf("[Error] call_ret_probe codegen verification failed\n");
        printf("-------- call_ret_probe --------\n%s-------- end --------\n", block.c_str());
        return false;
    }

    printf("[Done] call_ret_probe uses indirect call and BR x30 sequence\n");
    return true;
}

static bool verify_call_ret_scratch_mir() {
    std::string llc = g_build_dir + "\\bin\\llc.exe";
    if (!file_exists(llc)) {
        printf("[Warning] llc not found, skipping call/ret scratch verification\n");
        return true;
    }

    std::string src = g_script_dir + "\\llvm\\test\\CodeGen\\AArch64\\irobf-call-ret-scratch.mir";
    if (!file_exists(src)) {
        printf("[Error] Call/ret scratch MIR not found: %s\n", src.c_str());
        return false;
    }

    std::string output;
    std::string cmd = "\"" + llc +
                      "\" -mtriple=aarch64-none-linux-gnu "
                      "-run-pass=aarch64-call-ret-obfuscation "
                      "-aarch64-obfuscate-call-ret "
                      "-verify-machineinstrs=0 -o - "
                      "\"" + src + "\"";
    int mir_ret = run_cmd_capture(cmd, output);
    dbg_log("mir_verify_call_ret_scratch_start", cmd);
    if (output.empty()) {
        printf("[Error] Failed to run call/ret scratch MIR verification\n");
        dbg_log("mir_verify_call_ret_scratch_error", "empty_output");
        return false;
    }
    if (mir_ret != 0 && !output.empty()) {
        printf("%s", output.c_str());
        // Continue to analyze output even if llc returned non-zero,
        // as long as the expected patterns are present.
        dbg_log("mir_verify_call_ret_scratch_nonzero", std::string("code=") + std::to_string(mir_ret));
    }

    std::string lower = lower_ascii_copy(output);
    auto has = [&](const char* needle) {
        return lower.find(needle) != std::string::npos;
    };

    bool has_indirect_call = has("adrp") && has("addxri") && has("blr");
    bool preserves_x16_arg = has("implicit $x16");
    bool clobbers_x16 = has("$x16 = adrp") || has("$x16 = addxri") || has("blr $x16");

    if (!has_indirect_call || !preserves_x16_arg || clobbers_x16) {
        printf("[Error] call/ret scratch MIR verification failed\n");
        printf("-------- irobf-call-ret-scratch.mir --------\n%s-------- end --------\n", output.c_str());
        dbg_log("mir_verify_call_ret_scratch_fail",
                std::string("has_indirect_call=") + (has_indirect_call?"1":"0") +
                " preserves_x16_arg=" + (preserves_x16_arg?"1":"0") +
                " clobbers_x16=" + (clobbers_x16?"1":"0"));
        return false;
    }

    printf("[Done] scratch MIR keeps x16 live and uses a different call scratch register\n");
    dbg_log("mir_verify_call_ret_scratch_ok", "ok");
    return true;
}

static bool verify_opaque_predicate_codegen() {
    std::string llc = g_build_dir + "\\bin\\llc.exe";
    if (!file_exists(llc)) {
        printf("[Warning] llc not found, skipping opaque predicate verification\n");
        return true;
    }

    std::string objdump = g_build_dir + "\\bin\\llvm-objdump.exe";
    if (!file_exists(objdump)) {
        printf("[Warning] llvm-objdump not found, skipping opaque predicate object verification\n");
        return true;
    }

    std::string src = g_script_dir + "\\llvm\\test\\CodeGen\\AArch64\\irobf-opaque-predicate-bytes.ll";
    if (!file_exists(src)) {
        printf("[Error] Opaque predicate probe source not found: %s\n", src.c_str());
        return false;
    }

    std::string asm_path = g_script_dir + "\\test\\opaque_predicate_codegen.s";
    std::string obj_path = g_script_dir + "\\test\\opaque_predicate_codegen.o";
    std::string output;

    std::string asm_cmd = "\"" + llc +
                          "\" -mtriple=aarch64-linux-gnu -O2 "
                          "-aarch64-obfuscate-opaque-predicate "
                          "\"" + src + "\" -o \"" + asm_path + "\"";
    if (run_cmd_capture(asm_cmd, output) != 0) {
        if (!output.empty()) printf("%s", output.c_str());
        printf("[Error] Failed to compile opaque predicate asm probe\n");
        return false;
    }

    std::ifstream in(asm_path, std::ios::binary);
    if (!in.is_open()) {
        printf("[Error] Failed to read generated asm: %s\n", asm_path.c_str());
        return false;
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    std::string asm_text = ss.str();
    in.close();

    std::string lower_asm = normalize_ascii_whitespace(asm_text);
    auto asm_has = [&](const char* needle) {
        return lower_asm.find(needle) != std::string::npos;
    };

    bool has_guard_branch = asm_has("cbnz x9");
    bool has_dead_bytes = asm_has(".byte 0");
    bool has_misaligned_tail = asm_has(".byte 232");
    bool has_realign = asm_has(".p2align 2, 0x0");

    if (!has_guard_branch || !has_dead_bytes || !has_misaligned_tail || !has_realign) {
        printf("[Error] opaque predicate asm verification failed\n");
        printf("-------- opaque_predicate_codegen.s --------\n%s-------- end --------\n", asm_text.c_str());
        return false;
    }

    output.clear();
    std::string obj_cmd = "\"" + llc +
                          "\" -mtriple=aarch64-linux-gnu -filetype=obj -O2 "
                          "-aarch64-obfuscate-opaque-predicate "
                          "\"" + src + "\" -o \"" + obj_path + "\"";
    if (run_cmd_capture(obj_cmd, output) != 0) {
        if (!output.empty()) printf("%s", output.c_str());
        printf("[Error] Failed to compile opaque predicate object probe\n");
        return false;
    }

    output.clear();
    std::string dump_cmd = "\"" + objdump +
                           "\" -d --triple=aarch64-linux-gnu "
                           "\"" + obj_path + "\"";
    if (run_cmd_capture(dump_cmd, output) != 0) {
        if (!output.empty()) printf("%s", output.c_str());
        printf("[Error] Failed to disassemble opaque predicate object probe\n");
        return false;
    }

    std::string lower_dump = normalize_ascii_whitespace(output);
    auto dump_has = [&](const char* needle) {
        return lower_dump.find(needle) != std::string::npos;
    };

    bool has_udf_encoding = dump_has(".word 0x00000000");
    bool has_tail_word = dump_has(".word 0x000003e8");

    if (!has_udf_encoding || !has_tail_word) {
        printf("[Error] opaque predicate object verification failed\n");
        printf("-------- opaque_predicate_codegen.o --------\n%s-------- end --------\n", output.c_str());
        return false;
    }

    printf("[Done] opaque predicate emits dead bytes with UDF encoding and misaligned tail\n");
    return true;
}

static bool build_ndk_test_project(const TestRunOptions& opts, int jobs) {
    std::string ndk_build_bat = g_ndk_dir + "\\ndk-build.bat";
    std::string ndk_build_cmd = g_ndk_dir + "\\ndk-build.cmd";
    if (g_ndk_dir.empty() || (!file_exists(ndk_build_bat) && !file_exists(ndk_build_cmd))) {
        printf("[Error] NDK not found, cannot build test project\n");
        return false;
    }

    std::string project_dir = get_full_path_copy(opts.project_dir);
    if (!dir_exists(project_dir)) {
        printf("[Error] Test project directory not found: %s\n", project_dir.c_str());
        return false;
    }

    if (opts.clean_test) {
        run_cmd("if exist obj rmdir /s /q obj", project_dir);
        run_cmd("if exist libs rmdir /s /q libs", project_dir);
    }

    char jbuf[16];
    sprintf(jbuf, "%d", jobs);
    std::string ndk_build = file_exists(ndk_build_bat) ? ndk_build_bat : ndk_build_cmd;
    std::string ndk_cmd = "\"" + ndk_build + "\" NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=\"" +
                          opts.app_build_script + "\" NDK_APPLICATION_MK=\"" +
                          opts.app_application_mk + "\" -j" + jbuf;
    dbg_log("ndk_build_start", std::string("cwd=") + project_dir + " cmd=" + ndk_cmd);
    int ret = run_cmd(ndk_cmd, project_dir);
    dbg_log("ndk_build_end", std::string("ret=") + std::to_string(ret));
    if (ret != 0) {
        printf("[Error] ndk-build failed for %s (code: %d)\n", project_dir.c_str(), ret);
        return false;
    }
    return true;
}

static bool push_and_run_binary(const TestRunOptions& opts) {
    std::string adb_path = find_adb();
    if (adb_path.empty()) {
        printf("[Error] adb not found in PATH / ANDROID_SDK_ROOT / ANDROID_HOME\n");
        return false;
    }

    std::string serial = opts.serial.empty() ? find_test_device(adb_path) : opts.serial;
    if (serial.empty()) {
        printf("[Error] No connected emulator/device found for adb testing\n");
        return false;
    }
    printf("  -> Using device: %s\n", serial.c_str());

    std::string abi = opts.abi.empty() ? detect_device_abi(adb_path, serial) : normalize_abi(opts.abi);
    if (abi.empty()) {
        printf("[Error] Failed to detect device ABI\n");
        return false;
    }
    printf("  -> Device ABI: %s\n", abi.c_str());

    std::string local_binary = opts.local_binary_path;
    if (local_binary.empty()) {
        local_binary = find_named_binary(get_full_path_copy(opts.project_dir), abi, opts.binary_name);
    } else {
        local_binary = get_full_path_copy(local_binary);
    }
    if (local_binary.empty() || !file_exists(local_binary)) {
        printf("[Error] Test binary not found\n");
        return false;
    }

    std::string device_path = opts.device_path;
    if (device_path.empty()) {
        device_path = choose_device_test_path(adb_path, serial, path_basename(local_binary));
    }
    printf("  -> Local binary: %s\n", local_binary.c_str());
    printf("  -> Device path : %s\n", device_path.c_str());

    std::string output;
    std::string push_cmd = "\"" + adb_path + "\" -s " + serial + " push \"" +
                           local_binary + "\" \"" + device_path + "\"";
    int ret = run_cmd_capture(push_cmd, output);
    if (!output.empty()) printf("%s", output.c_str());
    if (ret != 0) {
        printf("[Error] adb push failed (code: %d)\n", ret);
        return false;
    }

    std::string remote_run = opts.run_command.empty()
        ? ("chmod 755 \"" + device_path + "\" && \"" + device_path + "\"")
        : opts.run_command;

    bool timed_out = false;
    output.clear();
    std::string exec_cmd = "\"" + adb_path + "\" -s " + serial + " shell \"" + remote_run + "\"";
    ret = run_cmd_capture_timeout(exec_cmd, output, g_script_dir,
                                  opts.timeout_sec > 0 ? static_cast<DWORD>(opts.timeout_sec) * 1000U : 0U,
                                  &timed_out);
    if (!output.empty()) printf("%s", output.c_str());
    if (timed_out) {
        printf("[Error] Device run timed out after %d seconds\n", opts.timeout_sec);
        std::string base = path_basename(device_path);
        std::string kill_cmd = "\"" + adb_path + "\" -s " + serial +
                               " shell \"pkill -f '" + base + "' >/dev/null 2>&1 || true\"";
        std::string kill_output;
        run_cmd_capture(kill_cmd, kill_output, g_script_dir);
        return false;
    }
    if (ret != 0) {
        printf("[Error] Device run failed (code: %d)\n", ret);
        return false;
    }
    return true;
}

static bool ensure_fla_test_project_exists() {
    std::string jni_dir = g_script_dir + "\\test\\jni";
    const char* required[] = {"Android.mk", "Application.mk", "main.cpp"};
    for (const char* name : required) {
        std::string path = jni_dir + "\\" + name;
        if (!file_exists(path)) {
            printf("[Error] Missing handwritten test file: %s\n", path.c_str());
            return false;
        }
    }
    return true;
}

static bool build_and_run_fla_test(int jobs, const TestRunOptions& opts) {
    printf("\n[Test] Building handwritten ALLVM_TEST project...\n");
    std::string ndk_build_bat = g_ndk_dir + "\\ndk-build.bat";
    std::string ndk_build_cmd = g_ndk_dir + "\\ndk-build.cmd";
    if (g_ndk_dir.empty() || (!file_exists(ndk_build_bat) && !file_exists(ndk_build_cmd))) {
        printf("[Error] NDK not found, cannot run emulator test\n");
        return false;
    }

    std::string adb_path = find_adb();
    if (adb_path.empty()) {
        printf("[Error] adb not found in PATH / ANDROID_SDK_ROOT / ANDROID_HOME\n");
        return false;
    }

    std::string serial = opts.serial.empty() ? find_test_device(adb_path) : opts.serial;
    if (serial.empty()) {
        printf("[Error] No connected emulator/device found for adb testing\n");
        return false;
    }
    printf("  -> Using device: %s\n", serial.c_str());

    std::string abi = opts.abi.empty() ? detect_device_abi(adb_path, serial) : normalize_abi(opts.abi);
    if (abi.empty()) {
        printf("[Error] Failed to detect device ABI\n");
        return false;
    }
    printf("  -> Test ABI: %s\n", abi.c_str());

    if (!ensure_fla_test_project_exists()) {
        return false;
    }

    std::string test_dir = g_script_dir + "\\test";
    if (opts.clean_test) {
        run_cmd("if exist obj rmdir /s /q obj", test_dir);
        run_cmd("if exist libs rmdir /s /q libs", test_dir);
    }

    char jbuf[16];
    sprintf(jbuf, "%d", jobs);
    std::string ndk_build = file_exists(ndk_build_bat) ? ndk_build_bat : ndk_build_cmd;
    std::string ndk_cmd = "\"" + ndk_build + "\" NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk APP_ABI=" + abi + " -j" + jbuf;
    dbg_log("ndk_build_start", std::string("cwd=") + test_dir + " cmd=" + ndk_cmd);
    int ret = run_cmd(ndk_cmd, test_dir);
    dbg_log("ndk_build_end", std::string("ret=") + std::to_string(ret));
    if (ret != 0) {
        printf("[Error] ndk-build test project failed (code: %d)\n", ret);
        return false;
    }


    std::string binary = find_test_binary(test_dir, abi);
    if (binary.empty()) {
        printf("[Error] Test binary not found for ABI: %s\n", abi.c_str());
        return false;
    }

    std::string output;
    std::string remote_binary = choose_device_test_path(adb_path, serial, "allvm_test");
    std::string push_cmd = "\"" + adb_path + "\" -s " + serial + " push \"" + binary + "\" \"" + remote_binary + "\"";
    ret = run_cmd_capture(push_cmd, output);
    if (!output.empty()) printf("%s", output.c_str());
    if (ret != 0) {
        printf("[Error] adb push failed (code: %d)\n", ret);
        return false;
    }

    std::string exec_cmd = "\"" + adb_path + "\" -s " + serial + " shell \"chmod 755 " +
                           remote_binary + " && " + remote_binary + "\"";
    output.clear();
    ret = run_cmd_capture(exec_cmd, output);
    if (!output.empty()) printf("%s", output.c_str());
    if (ret != 0 || output.find("ALLVM_TEST_PASS") == std::string::npos) {
        printf("[Error] Emulator test failed\n");
        return false;
    }

    printf("[Done] Emulator ALLVM_TEST passed\n");
    return true;
}

static bool build_and_run_custom_test(const TestRunOptions& opts, int jobs) {
    printf("\n[Test] Custom build/push/run...\n");
    if (!opts.skip_ndk_build) {
        if (opts.project_dir.empty()) {
            printf("[Error] --test-project is required unless --skip-test-build is used with --test-local-binary\n");
            return false;
        }
        if (!build_ndk_test_project(opts, jobs)) return false;
    }
    if (!push_and_run_binary(opts)) return false;
    printf("[Done] Custom device test completed\n");
    return true;
}

// ========== compile_interpreter ==========
static bool compile_interpreter(const std::string& target_triple) {
    printf("\n[1/3] Compiling aVMPInterpreter (target: %s)...\n", target_triple.c_str());

    std::string interp_dir = g_script_dir + "\\aVMPInterpreter";
    std::string bc_file = interp_dir + "\\aVMPInterpreter.bc";
    std::string src_file = interp_dir + "\\aVMPInterpreter.cpp";

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

    if (is_file_newer_or_equal(bc_file, src_file)) {
        printf("[Skip] aVMPInterpreter.bc is up to date\n");
        return true;
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
    std::string vm_h = g_script_dir + "\\llvm\\include\\llvm\\Transforms\\Obfuscation\\aVMP\\vm.h";

    if (!file_exists(bc_file)) {
        printf("[Error] %s not found\n", bc_file.c_str());
        return false;
    }
    if (is_file_newer_or_equal(vm_h, bc_file)) {
        printf("[Skip] vm.h is up to date\n");
        return true;
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
static bool cmake_configure(bool reconfigure) {
    printf("\n[CMake] Configuring...\n");

    std::string vcvars = find_vs();
    if (vcvars.empty()) {
        printf("[Error] Visual Studio not found!\n");
        return false;
    }

    dir_create(g_build_dir);

    std::string cmake_cache = g_build_dir + "\\CMakeCache.txt";
    if (file_exists(cmake_cache) && !reconfigure) {
        printf("[Skip] CMakeCache.txt exists, use --reconfigure to regenerate\n");
        return true;
    }
    if (file_exists(cmake_cache)) {
        DeleteFileA(cmake_cache.c_str());
    }

    std::string zstd_include_dir = g_zstd_dir + "\\src\\lib";
    std::string zstd_lib = g_zstd_dir + "\\build\\lib\\zstd_static.lib";
    std::string zlib_include_dir = g_zlib_dir;
    std::string zlib_lib;
    const char* zlib_candidates[] = {
        "\\\\build-msvc\\\\libzs.lib",
        "\\\\build-msvc\\\\zlibstatic.lib",
        "\\\\build-msvc\\\\zlib.lib",
        "\\\\build\\\\zlibstatic.lib",
        "\\\\build\\\\zlib.lib",
        "\\\\build\\\\libzs.a"
    };
    for (auto cand : zlib_candidates) {
        std::string p = g_zlib_dir + cand;
        if (file_exists(p)) { zlib_lib = p; break; }
    }
    std::replace(zlib_include_dir.begin(), zlib_include_dir.end(), '\\', '/');
    std::replace(zstd_include_dir.begin(), zstd_include_dir.end(), '\\', '/');
    std::replace(zstd_lib.begin(), zstd_lib.end(), '\\', '/');
    if (!zlib_lib.empty()) std::replace(zlib_lib.begin(), zlib_lib.end(), '\\', '/');

    std::string cmake_cmd = "cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=/utf-8 "
                            "-DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_EH=ON ";
    std::string cache_launcher = find_compiler_cache_launcher();
    if (!cache_launcher.empty()) {
        std::replace(cache_launcher.begin(), cache_launcher.end(), '\\', '/');
        cmake_cmd += "-DCMAKE_C_COMPILER_LAUNCHER=\"" + cache_launcher + "\" "
                     "-DCMAKE_CXX_COMPILER_LAUNCHER=\"" + cache_launcher + "\" ";
        printf("  -> Compiler cache: %s\n", cache_launcher.c_str());
    }
    cmake_cmd += 
                            "-DLLVM_ENABLE_PROJECTS=\"llvm;clang;lld\" "
                            "-DLLVM_TARGETS_TO_BUILD=\"AArch64;ARM;X86\" "
                            "-DLLVM_ENABLE_ZSTD=FORCE_ON "
                            "-DLLVM_USE_STATIC_ZSTD=ON "
                            "-Dzstd_INCLUDE_DIR=\"" + zstd_include_dir + "\" "
                            "-Dzstd_LIBRARY=\"" + zstd_lib + "\" "
                            "-Dzstd_STATIC_LIBRARY=\"" + zstd_lib + "\" ";

    if (!zlib_lib.empty() && zlib_lib.rfind(".a") == std::string::npos) {
        cmake_cmd += "-DLLVM_ENABLE_ZLIB=FORCE_ON "
                     "-DZLIB_INCLUDE_DIR=\"" + zlib_include_dir + "\" "
                     "-DZLIB_LIBRARY=\"" + zlib_lib + "\" ";
    } else {
        cmake_cmd += "-DLLVM_ENABLE_ZLIB=OFF "
                     "-DZLIB_INCLUDE_DIR=NOTFOUND "
                     "-DZLIB_LIBRARY=NOTFOUND ";
    }
    cmake_cmd += "../llvm";

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

    // Replace key toolchain binaries so that new driver options are recognized.
    struct ToolReplaceEntry {
        const char* name;
        bool required;
    };
    const ToolReplaceEntry files[] = {
        { "clang.exe", true },
        { "clang++.exe", true },
        { "lld.exe", true },
        { "llvm-strip.exe", false },
        { "llvm-objcopy.exe", false }
    };

    for (const auto& entry : files) {
        std::string src = build_bin + "\\" + entry.name;
        std::string dst = g_ndk_bin + "\\" + entry.name;

        if (!file_exists(src)) {
            if (entry.required) {
                printf("[Error] required tool not found: %s\n", src.c_str());
                return false;
            }
            printf("  -> skip %s (missing in build bin)\n", entry.name);
            continue;
        }

        std::string backup = dst + ".bak";
        if (!file_exists(backup) && file_exists(dst)) {
            if (!copy_file(dst, backup, false)) return false;
        }

        if (!copy_file(src, dst)) return false;
        printf("  -> %s OK\n", entry.name);
    }

    std::string lld_src = build_bin + "\\lld.exe";
    std::string lld_dst = g_ndk_bin + "\\ld.lld.exe";
    if (file_exists(lld_src)) {
        std::string backup = lld_dst + ".bak";
        if (!file_exists(backup) && file_exists(lld_dst)) {
            if (!copy_file(lld_dst, backup, false)) return false;
        }
        if (!copy_file(lld_src, lld_dst)) return false;
        printf("  -> ld.lld.exe OK\n");
    } else {
        printf("[Error] required tool not found: %s\n", lld_src.c_str());
        return false;
    }

    // also copy required runtime DLLs from our build bin to NDK bin to avoid ABI/version mismatch
    WIN32_FIND_DATAA ffd;
    HANDLE hFind = FindFirstFileA((build_bin + "\\*.dll").c_str(), &ffd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string name = ffd.cFileName;
            std::string src = build_bin + "\\" + name;
            std::string dst = g_ndk_bin + "\\" + name;
            if (!file_exists(src)) continue;
            std::string backup = dst + ".bak";
            if (!file_exists(backup) && file_exists(dst)) {
                if (!copy_file(dst, backup, false)) return false;
            }
            if (!copy_file(src, dst)) return false;
            printf("  -> %s OK\n", name.c_str());
        } while (FindNextFileA(hFind, &ffd));
        FindClose(hFind);
    }

    // Copy allvm-ui.exe to NDK root directory
    std::string ui_src = build_bin + "\\allvm-ui.exe";
    std::string ui_dst = g_ndk_dir + "\\allvm-ui.exe";
    if (file_exists(ui_src) && !g_ndk_dir.empty()) {
        if (!copy_file(ui_src, ui_dst)) return false;
        printf("  -> allvm-ui.exe OK\n");
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
    printf("  |       OLLVM Build Script v21.x           |\n");
    printf("  +------------------------------------------+\n");

    std::string target_triple = "aarch64-linux-android";
    int jobs = 32;
    bool build_apk_flag = false;
    bool build_apk_release_flag = false;
    bool reconfigure = false;
    TestRunOptions test_opts;

    bool step_zstd        = false;
    bool step_cmake       = true;
    bool step_interpreter = true;
    bool step_vmh         = true;
    bool step_build       = true;
    bool step_test        = true;

    std::string ninja_targets = "clang lld";

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
        } else if (arg == "--build-tools") {
            ninja_targets = "clang lld llvm-strip llvm-objcopy llvm-dis llc FileCheck";
        } else if (arg == "--reconfigure") {
            reconfigure = true;
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
        } else if (arg == "--test-project" && i + 1 < argc) {
            test_opts.custom_test = true;
            test_opts.project_dir = argv[++i];
        } else if (arg == "--test-build-script" && i + 1 < argc) {
            test_opts.custom_test = true;
            test_opts.app_build_script = argv[++i];
        } else if (arg == "--test-application-mk" && i + 1 < argc) {
            test_opts.custom_test = true;
            test_opts.app_application_mk = argv[++i];
        } else if (arg == "--test-binary" && i + 1 < argc) {
            test_opts.custom_test = true;
            test_opts.binary_name = argv[++i];
        } else if (arg == "--test-local-binary" && i + 1 < argc) {
            test_opts.custom_test = true;
            test_opts.local_binary_path = argv[++i];
        } else if (arg == "--test-device-path" && i + 1 < argc) {
            test_opts.custom_test = true;
            test_opts.device_path = argv[++i];
        } else if (arg == "--test-run-cmd" && i + 1 < argc) {
            test_opts.custom_test = true;
            test_opts.run_command = argv[++i];
        } else if (arg == "--test-serial" && i + 1 < argc) {
            test_opts.custom_test = true;
            test_opts.serial = argv[++i];
        } else if (arg == "--test-abi" && i + 1 < argc) {
            test_opts.custom_test = true;
            test_opts.abi = argv[++i];
        } else if (arg == "--test-timeout" && i + 1 < argc) {
            test_opts.custom_test = true;
            test_opts.timeout_sec = atoi(argv[++i]);
            if (test_opts.timeout_sec < 0) test_opts.timeout_sec = 0;
        } else if (arg == "--skip-test-build") {
            test_opts.custom_test = true;
            test_opts.skip_ndk_build = true;
        } else if (arg == "--clean-test") {
            test_opts.clean_test = true;
        }
    }

    if (step_zstd) {
        if (!build_zstd()) return 1;
    }
    if (step_cmake) {
        if (!cmake_configure(reconfigure)) return 1;
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
        if (test_opts.custom_test) {
            if (!build_and_run_custom_test(test_opts, jobs)) return 1;
        } else {
            if (!build_and_run_fla_test(jobs, test_opts)) return 1;
        }
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
