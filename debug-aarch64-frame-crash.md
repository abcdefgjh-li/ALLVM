# [OPEN] aarch64-frame-crash

## 问题
- 将 `-mllvm -aarch64-obfuscate-frame-record` 真实挂到 `test/jni/Android.mk` 的 `arm64-v8a` 编译参数后，`clang++` 在 `AArch64FrameLowering` 的 `Prologue/Epilogue Insertion & Frame Finalization` 阶段崩溃。
- 复现场景：`.\build.exe --only test -j 8`
- 当前现象：`armeabi-v7a/x86/x86_64` 正常，`arm64-v8a` 在编译 `jni/main.cpp` 时崩溃。

## 当前假设
1. `emitEpilogue` 的 split restore 破坏了 `LastPopI/Pop` 的遍历假设，导致后续访问非法指令/操作数。
2. `fixupCalleeSaveRestoreStackOffset()` 或 `convertCalleeSaveRestoreToSPPrePostIncDec()` 仍要求标准 `LDP/STP` frame-record。
3. `useSimpleSplitFrameRecord()` 的触发条件仍过宽，复杂函数 `main` 误命中本应只用于极简 frame-record 的路径。
4. CFI / callee-saved location 恢复逻辑仍默认成对 frame-record，split 序列造成内部状态不一致。
5. `FP` 的非标准建立序列影响了 PEI 后续依赖 canonical frame setup 的路径。

## 已有证据
- 崩溃点来自 `jni/main.cpp`
- LLVM stack dump 显示阶段为 `Prologue/Epilogue Insertion & Frame Finalization`
- 独立 `frame_record_codegen.cpp` 汇编检测是通过的

## 下一步
- 在 `AArch64FrameLowering.cpp` 的 split trigger、prologue/epilogue 关键分支添加最小化调试输出
- 用现有 `main-*.cpp/.sh` 或 `build.exe --only test` 复现并采集日志
- 根据日志确认到底崩在 trigger、offset fixup、还是 CFI/restore 路径
