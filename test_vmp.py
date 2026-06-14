#!/usr/bin/env python3
"""
VMP 测试脚本
用于推送测试文件到 Android 模拟器并运行测试
"""

import subprocess
import sys
import os

# 配置
ADB_PATH = r"C:\Users\Administrator\AppData\Local\Android\Sdk\platform-tools\adb.exe"
NDK_CLANG = r"D:\cpp\obfuscator-ollvm-21.x\android-ndk-r30-beta1-windows\toolchains\llvm\prebuilt\windows-x86_64\bin\clang.exe"

def run_command(cmd, cwd=None, timeout=60):
    """运行命令并返回输出"""
    try:
        print(f"[CMD] {cmd}")
        result = subprocess.run(
            cmd,
            shell=True,
            cwd=cwd,
            capture_output=True,
            text=True,
            encoding='utf-8',
            errors='replace',
            timeout=timeout
        )
        if result.stdout:
            print(result.stdout)
        if result.stderr:
            print(result.stderr, file=sys.stderr)
        return result.returncode, result.stdout, result.stderr
    except subprocess.TimeoutExpired:
        return -1, "", "Timeout"

def compile_test(source_file, output_binary, extra_flags=""):
    """编译测试文件"""
    print(f"\n[COMPILE] {source_file} -> {output_binary}")

    # 使用 -O0 优化级别
    cmd = f'"{NDK_CLANG}" -target aarch64-linux-android21 -fPIC -O0 -mllvm -irobf -mllvm -irobf-vmp {extra_flags} -o {output_binary} {source_file}'
    ret, out, err = run_command(cmd)

    if ret != 0:
        print(f"[ERROR] 编译失败")
        return False

    print(f"[OK] 编译成功")
    return True

def push_and_run_test(binary_path):
    """推送并运行测试"""
    binary_name = os.path.basename(binary_path)
    # 使用 /tmp/ 目录（Android 上可写）
    remote_path = f"/tmp/{binary_name}"

    # 先尝试删除旧文件
    run_command(f'"{ADB_PATH}" shell rm -f {remote_path}')

    # 推送
    print(f"\n[PUSH] {binary_path} -> {remote_path}")
    cmd = f'"{ADB_PATH}" push {binary_path} {remote_path}'
    ret, _, _ = run_command(cmd)
    if ret != 0:
        print(f"[ERROR] 推送失败")
        return False, ""

    # 设置权限
    cmd = f'"{ADB_PATH}" shell chmod 755 {remote_path}'
    ret, _, _ = run_command(cmd)
    if ret != 0:
        print(f"[ERROR] 设置权限失败")
        return False, ""

    # 运行
    print(f"\n[RUN] {remote_path}")
    cmd = f'"{ADB_PATH}" shell {remote_path}'
    ret, out, _ = run_command(cmd, timeout=30)

    return True, out

def test_simple_vmp():
    """测试简单 VMP 功能"""
    print("=" * 70)
    print("测试 1: 简单 VMP 功能")
    print("=" * 70)

    # 创建测试文件
    test_code = '''// 简单的 VMP 测试
#include <stdio.h>

// VMP 保护宏
#define VMP_PROTECT __attribute__((annotate("vmp")))

// 简单函数（VMP 保护）
VMP_PROTECT int add(int a, int b) {
    return a + b;
}

// main 函数也使用 VMP 保护
VMP_PROTECT int main() {
    printf("=== Simple VMP Test ===\\n");

    int result = add(10, 20);
    printf("Result: %d\\n", result);

    if (result == 30) {
        printf("[PASS] VMP test passed!\\n");
        return 0;
    } else {
        printf("[FAIL] VMP test failed! Expected 30, got %d\\n", result);
        return 1;
    }
}
'''

    with open("test/jni/test_simple_vmp.c", "w", encoding="utf-8") as f:
        f.write(test_code)

    # 编译
    if not compile_test("test/jni/test_simple_vmp.c", "test/jni/test_simple_vmp"):
        return False

    # 推送并运行
    success, output = push_and_run_test("test/jni/test_simple_vmp")

    # 检查结果
    if success and "[PASS]" in output:
        print("\n[SUCCESS] 测试通过!")
        return True
    else:
        print("\n[FAILED] 测试失败!")
        return False

def test_atomic():
    """测试原子操作"""
    print("\n" + "=" * 70)
    print("测试 2: 原子操作 (VMP)")
    print("=" * 70)

    # 创建测试文件 - 使用 VMP 保护
    test_code = '''// 原子操作测试 (VMP)
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>

#define VMP_PROTECT __attribute__((annotate("vmp")))

VMP_PROTECT int main() {
    printf("=== Atomic Operations Test (VMP) ===\\n");
    fflush(stdout);

    // 先声明变量，再赋值（避免初始化问题）
    int counter;
    counter = 0;

    printf("[DEBUG] counter initialized\\n");
    fflush(stdout);

    // 测试 atomic_compare_exchange_strong
    int expected = 0;
    int desired = 42;
    bool success = __sync_bool_compare_and_swap(&counter, expected, desired);

    printf("[DEBUG] CAS: success=%d, counter=%d\\n", success, counter);
    fflush(stdout);

    if (success && counter == 42) {
        printf("[PASS] atomic_compare_exchange_strong\\n");
        fflush(stdout);
    } else {
        printf("[FAIL] atomic_compare_exchange_strong\\n");
        fflush(stdout);
        return 1;
    }

    // 测试 atomic_fetch_add
    int old_value = __sync_fetch_and_add(&counter, 10);
    printf("[DEBUG] fetch_add: old=%d, counter=%d\\n", old_value, counter);
    fflush(stdout);

    if (old_value == 42 && counter == 52) {
        printf("[PASS] atomic_fetch_add\\n");
        fflush(stdout);
    } else {
        printf("[FAIL] atomic_fetch_add\\n");
        fflush(stdout);
        return 1;
    }

    printf("[PASS] All atomic tests passed!\\n");
    fflush(stdout);
    return 0;
}
'''

    with open("test/jni/test_atomic.c", "w", encoding="utf-8") as f:
        f.write(test_code)

    # 编译
    if not compile_test("test/jni/test_atomic.c", "test/jni/test_atomic"):
        return False

    # 推送并运行
    success, output = push_and_run_test("test/jni/test_atomic")

    # 检查结果
    if success and "[PASS]" in output and "All atomic tests passed" in output:
        print("\n[SUCCESS] 测试通过!")
        return True
    else:
        print("\n[FAILED] 测试失败!")
        return False

def main():
    print("=" * 70)
    print("VMP 测试套件")
    print("=" * 70)

    results = []

    # 测试 1: 简单 VMP
    results.append(("简单 VMP", test_simple_vmp()))

    # 测试 2: 原子操作
    results.append(("原子操作", test_atomic()))

    # 打印总结
    print("\n" + "=" * 70)
    print("测试总结")
    print("=" * 70)

    passed = sum(1 for _, success in results if success)
    total = len(results)

    for name, success in results:
        status = "[PASS]" if success else "[FAIL]"
        print(f"{status} - {name}")

    print(f"\n总计: {passed}/{total} 通过")

    return 0 if passed == total else 1

if __name__ == "__main__":
    sys.exit(main())
