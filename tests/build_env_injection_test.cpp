#include <cassert>
#include <string>

#define main build_cpp_entrypoint
#include "../build.cpp"
#undef main

static void test_default_remote_run_injects_lc() {
    const std::string cmd =
        build_remote_run_command("/data/local/tmp/allvm_test", "", "abc123");
    assert(cmd.find("export lc='abc123'") != std::string::npos);
    assert(cmd.find("\"/data/local/tmp/allvm_test\"") != std::string::npos);
}

static void test_custom_remote_run_is_preserved() {
    const std::string cmd =
        build_remote_run_command("/data/local/tmp/allvm_test",
                                 "cd /data/local/tmp && ./allvm_test", "abc123");
    assert(cmd == "cd /data/local/tmp && ./allvm_test");
}

int main() {
    test_default_remote_run_injects_lc();
    test_custom_remote_run_is_preserved();
    return 0;
}
