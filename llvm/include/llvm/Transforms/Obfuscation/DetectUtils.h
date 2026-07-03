//===- DetectUtils.h - 检测工具公共模块 ------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// 本文件提供检测Pass的公共工具函数，包括：
// - 统一的报告和终止函数
// - 随机化线程创建
// - 隐蔽处理和延迟响应
// - TracerPid检测
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_OBFUSCATION_DETECTUTILS_H
#define LLVM_TRANSFORMS_OBFUSCATION_DETECTUTILS_H

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include <string>

namespace llvm {

/// 检测配置选项
struct DetectOptions {
    bool UseThread = false;           // 是否使用后台线程
    bool StealthMode = true;          // 隐蔽模式（延迟响应）
    int MinDelayMs = 1000;            // 最小延迟（毫秒）
    int MaxDelayMs = 5000;            // 最大延迟（毫秒）
    bool RandomThreadAttr = true;     // 随机线程属性
    
    static DetectOptions create(bool useThread = false) {
        DetectOptions opts;
        opts.UseThread = useThread;
        return opts;
    }
};

class DetectUtils {
public:
    /// 创建统一的报告和终止函数
    /// 检测到威胁时打印:
    ///   - A-Protector
    ///   - Protection v1.2.0
    ///   - [DEBUG] {detectName} detected! Killing process...
    /// @param M 模块
    /// @param detectName 检测类型名称（如 "LD_PRELOAD", "IDA", "Ptrace"等）
    /// @return 创建的函数
    static Function* createReportAndKillFunc(Module &M, const std::string &detectName = "Unknown");
    
    /// 创建隐蔽终止函数（延迟响应）
    /// @param M 模块
    /// @param opts 配置选项
    /// @return 创建的函数
    static Function* createStealthKillFunc(Module &M, const DetectOptions &opts);
    
    /// 创建随机延迟函数
    /// @param M 模块
    /// @param minMs 最小延迟毫秒
    /// @param maxMs 最大延迟毫秒
    /// @return 创建的函数
    static Function* createRandomDelayFunc(Module &M, int minMs = 1000, int maxMs = 5000);
    
    /// 创建后台线程函数（带随机属性）
    /// @param M 模块
    /// @param checkFunc 检测函数
    /// @param opts 配置选项
    /// @return 创建的启动线程函数
    static Function* createThreadFunc(Module &M, Function *checkFunc, const DetectOptions &opts);
    
    /// 创建TracerPid检测函数（检测/proc/self/status中的TracerPid）
    /// @param M 模块
    /// @param reportFunc 报告函数
    /// @return 创建的检测函数
    static Function* createTracerPidCheckFunc(Module &M, Function *reportFunc);

    /// 创建ptrace自附加反调试函数
    /// 原理：fork子进程 -> 子进程PTRACE_TRACEME -> 父进程作为tracer
    ///       execv后trace关系保持，外部进程无法再ptrace附加
    ///       后台线程持续监控子进程TracerPid，若为0则kill子进程并退出
    /// @param M 模块
    /// @param reportFunc 报告函数
    /// @return 创建的启动函数
    static Function* createPtraceSelfAttachFunc(Module &M, Function *reportFunc);

    /// 创建环境变量校验函数（配合linker壳使用）
    /// 原理：壳程序在execv前设置环境变量 lc=<随机32位字符串>
    ///       检测代码读取getenv("lc")并与内嵌密钥比较
    ///       密钥初始为占位符，ELFWrapper加壳时在二进制中替换为实际密钥
    ///       如果环境变量不存在或不匹配，说明程序被直接运行（未通过壳启动）
    /// @param M 模块
    /// @param reportFunc 报告函数
    /// @param envKey 环境变量密钥（32字节），直接嵌入到代码中
    /// @return 创建的检测函数
    static Function* createEnvVarCheckFunc(Module &M, Function *reportFunc, const std::string &envKey = "");

    /// 创建gz环境变量校验函数（配合gz壳使用）
    /// 原理：gz壳程序在execv前设置环境变量 lc_gz=<随机32位字符串>
    ///       检测代码读取getenv("lc_gz")并与内嵌密钥比较
    ///       密钥初始为占位符，GzWrapper加壳时在二进制中替换为实际密钥
    /// @param M 模块
    /// @param reportFunc 报告函数
    /// @param envKey 环境变量密钥（32字节），直接嵌入到代码中
    /// @return 创建的检测函数
    static Function* createGzEnvVarCheckFunc(Module &M, Function *reportFunc, const std::string &envKey = "");

    /// 环境变量校验占位符（32字节），ELFWrapper加壳时在二进制中搜索并替换
    static constexpr const char *ENV_KEY_PLACEHOLDER = "A-PROTECT-ENV-KEY-PLACEHOLDER!!";
    /// gz环境变量校验占位符（32字节），GzWrapper加壳时在二进制中搜索并替换
    static constexpr const char *GZ_ENV_KEY_PLACEHOLDER = "A-PROTECT-GZ-ENV-KEY-PLACE!!!!!!";
    
    /// 创建全局字符串
    /// @param M 模块
    /// @param str 字符串内容
    /// @param name 全局变量名
    /// @return 字符串指针
    static Constant* createGlobalString(Module &M, const std::string &str, const std::string &name);
    
    /// 注入检测代码到main函数
    /// @param M 模块
    /// @param checkFunc 检测函数
    /// @param opts 配置选项
    /// @return 是否成功
    static bool injectToMain(Module &M, Function *checkFunc, const DetectOptions &opts);
    
    /// 创建线程属性随机化函数
    /// @param M 模块
    /// @return 创建的函数
    static Function* createRandomThreadAttrFunc(Module &M);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_OBFUSCATION_DETECTUTILS_H
