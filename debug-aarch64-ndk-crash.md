Status: [OPEN]
Session: aarch64-ndk-crash
Owner: assistant
Created: 2026-07-01T00:00:00Z

Context
- Symptom: arm64-v8a NDK 构建在 “AArch64 pseudo instruction expansion pass” 崩溃
- Expectation: 四架构编译通过，MIR 与代码生成探针均绿
- Environment: Windows, MSVC toolchain, custom LLVM/Clang replacing NDK clang for arm64

Hypotheses
1) Our pass leaves invalid implicit operands on indirect call/return causing pseudo expansion crash
2) Replacing NDK clang introduces version/ABI mismatch leading to pipeline differences or asserts
3) FrameLowering split sequence interacts with downstream passes even when flags off via stale caches
4) Build wrapper passes wrong target flags/options to clang for arm64 only

Instrumentation Plan
- Add arm64-only build logs: capture full clang/ndk-build command, exit code, and stderr tail
- Gate optional backend debug logs under -mllvm -irobf-debug (off by default)
- Restrict reproduction to APP_ABI := arm64-v8a to isolate

Reproduction
- Run: ./build.exe --only test
- Artifacts: trae-debug-log-aarch64-ndk-crash.ndjson

Evidence Log
- Pending first repro

Next
- Collect logs → map to hypotheses → minimal fix → verify → compare pre/post

