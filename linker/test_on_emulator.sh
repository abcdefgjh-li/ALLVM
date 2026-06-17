#!/bin/bash
# ============================================================================
# 完整的 ARM64 ELF 加载器测试脚本
# 仅用于软件保护研究/教育目的
# ============================================================================

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 日志函数
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

log_section() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}========================================${NC}"
}

# 检查 ADB 是否可用
check_adb() {
    if ! command -v adb &> /dev/null; then
        log_error "ADB 未找到，请确保 Android SDK 已安装并配置到 PATH"
        exit 1
    fi
}

# 检查设备连接
check_device() {
    local device_count=$(adb devices | grep -v "List of devices" | grep -c "device")
    if [ "$device_count" -eq 0 ]; then
        log_error "没有检测到设备，请确保设备已连接并开启 USB 调试"
        exit 1
    fi
    log_info "检测到 $device_count 个设备"
}

# 构建项目
build_project() {
    log_section "构建项目"

    # 检查 NDK
    if ! command -v ndk-build &> /dev/null; then
        log_error "ndk-build 未找到，请确保 Android NDK 已安装并配置到 PATH"
        exit 1
    fi

    # 清理旧的构建
    log_info "清理旧的构建..."
    ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk clean 2>/dev/null || true

    # 构建
    log_info "构建 payload 和 loader..."
    ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk

    # 检查构建结果
    if [ ! -f "libs/arm64-v8a/linker_demo" ]; then
        log_error "构建失败：linker_demo 未生成"
        exit 1
    fi

    if [ ! -f "libs/arm64-v8a/linker" ]; then
        log_error "构建失败：linker 未生成"
        exit 1
    fi

    log_info "构建成功"
}

# 生成 payload.h
generate_payload() {
    log_section "生成 payload.h"

    if [ ! -f "bin2c.py" ]; then
        log_error "bin2c.py 未找到"
        exit 1
    fi

    log_info "转换 linker_demo 为 payload.h..."
    python3 bin2c.py libs/arm64-v8a/linker_demo jni/payload.h payload_data

    if [ ! -f "jni/payload.h" ]; then
        log_error "payload.h 生成失败"
        exit 1
    fi

    log_info "payload.h 生成成功"
}

# 重新构建（包含 payload）
rebuild_with_payload() {
    log_section "重新构建（包含 payload）"

    log_info "清理旧的构建..."
    ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk clean 2>/dev/null || true

    log_info "重新构建..."
    ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk

    log_info "重新构建成功"
}

# 推送文件到设备
push_files() {
    log_section "推送文件到设备"

    # 检查文件是否存在
    if [ ! -f "libs/arm64-v8a/linker" ]; then
        log_error "linker 文件不存在"
        exit 1
    fi

    if [ ! -f "libs/arm64-v8a/linker_demo" ]; then
        log_error "linker_demo 文件不存在"
        exit 1
    fi

    # 创建目标目录
    log_info "创建目标目录..."
    adb shell "mkdir -p /data/local/tmp"

    # 推送文件
    log_info "推送 linker..."
    adb push libs/arm64-v8a/linker /data/local/tmp/linker

    log_info "推送 linker_demo..."
    adb push libs/arm64-v8a/linker_demo /data/local/tmp/linker_demo

    # 设置权限
    log_info "设置执行权限..."
    adb shell "chmod 755 /data/local/tmp/linker"
    adb shell "chmod 755 /data/local/tmp/linker_demo"

    log_info "文件推送成功"
}

# 运行测试
run_tests() {
    log_section "运行测试"

    # 测试 1: 直接运行 payload
    log_info "测试 1: 直接运行 payload（对比用）"
    echo ""
    adb shell "/data/local/tmp/linker_demo test_arg1 test_arg2"
    local direct_result=$?
    echo ""

    if [ $direct_result -eq 0 ]; then
        log_info "直接运行 payload 成功"
    else
        log_warn "直接运行 payload 失败，退出码: $direct_result"
    fi

    # 测试 2: 通过 loader 运行
    log_info "测试 2: 通过 loader 运行（内存加载）"
    echo ""
    adb shell "/data/local/tmp/linker test_arg1 test_arg2"
    local loader_result=$?
    echo ""

    if [ $loader_result -eq 0 ]; then
        log_info "通过 loader 运行成功"
    else
        log_error "通过 loader 运行失败，退出码: $loader_result"
    fi

    # 比较结果
    echo ""
    if [ $direct_result -eq $loader_result ]; then
        log_info "✓ 测试结果一致"
    else
        log_warn "✗ 测试结果不一致"
    fi
}

# 查看日志
view_logs() {
    log_section "查看日志"

    log_info "清空 logcat 缓冲区..."
    adb logcat -c

    log_info "运行程序并捕获日志..."
    adb shell "/data/local/tmp/linker test_arg" &
    sleep 2

    log_info "ELFLoader 日志:"
    adb logcat -d -s ELFLoader:* | tail -50

    echo ""
    log_info "PayloadTest 日志:"
    adb logcat -d -s PayloadTest:* | tail -50
}

# 清理
cleanup() {
    log_section "清理"

    log_info "清理设备上的文件..."
    adb shell "rm -f /data/local/tmp/linker"
    adb shell "rm -f /data/local/tmp/linker_demo"

    log_info "清理本地构建..."
    ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk clean 2>/dev/null || true

    log_info "清理完成"
}

# 显示帮助
show_help() {
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  --build         仅构建项目"
    echo "  --push          仅推送文件"
    echo "  --test          仅运行测试"
    echo "  --log           仅查看日志"
    echo "  --clean         清理文件"
    echo "  --all           完整流程（默认）"
    echo "  --help          显示帮助信息"
    echo ""
    echo "示例:"
    echo "  $0 --all        # 完整流程"
    echo "  $0 --build      # 仅构建"
    echo "  $0 --test       # 仅测试"
}

# 主函数
main() {
    local mode="${1:-all}"

    echo ""
    log_section "ARM64 ELF 加载器测试"
    log_info "仅用于软件保护研究/教育目的"
    echo ""

    # 检查环境
    check_adb

    case "$mode" in
        --build)
            build_project
            generate_payload
            rebuild_with_payload
            ;;
        --push)
            check_device
            push_files
            ;;
        --test)
            check_device
            run_tests
            ;;
        --log)
            check_device
            view_logs
            ;;
        --clean)
            check_device
            cleanup
            ;;
        --all)
            check_device
            build_project
            generate_payload
            rebuild_with_payload
            push_files
            run_tests
            view_logs
            ;;
        --help)
            show_help
            exit 0
            ;;
        *)
            log_error "未知选项: $mode"
            show_help
            exit 1
            ;;
    esac

    log_section "完成"
    log_info "测试完成"
}

# 运行主函数
main "$@"
