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
    /// 非调试模式输出"你给我滚出去!!!"
    /// @param M 模块
    /// @return 创建的函数
    static Function* createReportAndKillFunc(Module &M);
    
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
