[OPEN] Debug Session: vmp-syscall-crash

## Context
- Symptom: enabling both VMP and Syscall protection crashes `clang++` during compilation.
- Sample: `D:\cpp\obfuscator-ollvm-21.x\clean_id2`
- Goal: identify the concrete failure point and implement a minimal fix backed by runtime evidence.

## Hypotheses
1. `SyscallProtect` rewrites a function or intrinsic into a shape that `VMProtect` does not expect, leading to a null dereference.
2. A special syscall wrapper such as `__syscall_exit` survives syscall transformation with malformed arguments or return type metadata.
3. `VMProtect` assumes every translated callee/instruction has a valid mapping, but syscall-protected IR leaves an unmapped node.
4. The crash is caused by stale binaries or DLL mismatch, so the source under inspection is not the code path executed by `clang++.exe`.
5. The pass pipeline order or repeated pass invocation causes VMP to process partially transformed syscall IR twice.

## Plan
1. Inspect pass structure with codegraph and local source search.
2. Reproduce the crash on `clean_id2` and capture the exact stack/output.
3. Add minimal instrumentation only around syscall-to-VMP handoff.
4. Reproduce again, confirm or reject hypotheses with evidence.
5. Apply the smallest fix and verify on the same sample.

## Evidence
- `ObfuscationPassManager` pass order is `SyscallProtect -> VMProtect`; therefore VMP consumes syscall-transformed IR.
- `SyscallProtect::shouldReplace()` currently skips base names `exit` and `_exit`, so source under workspace should no longer emit `__syscall_exit`.
- Controlled repro on `clean_id2/obj/debug-matrix/exit_repro.cpp`:
  - `build-windows/bin/clang++.exe` with `-irobf -irobf-syscall` emits direct `call void @exit(i32 0)`.
  - NDK `toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe` still emits `call i64 @__syscall_exit(i32 0)`.
- This strongly supports hypothesis 4: source fix exists, but the NDK `clang++.exe` currently used for Android builds is stale or not replaced successfully.
- User requested follow-up verification without `-irobf-fla`; subsequent tests will use `vmp + syscall` only.
- No-`fla` minimal VMP repro on `clean_id2/obj/debug-matrix/vmp_syscall_repro.cpp`:
  - Before replacement, NDK `clang++.exe` crashes in `ObfuscationPassManagerPass` with `0xC0000005`.
  - After replacing NDK `clang++.exe` with `build-windows/bin/clang++.exe`, the same command exits `0` and generated IR contains `call void @exit(i32 0)` with no `__syscall_exit`.
- No-`fla` direct compile on the real sample source:
  - `clean_id2/jni/wangzhe_clean.cpp` compiled with only `-irobf -irobf-vmp -irobf-vm_functions=main -irobf-syscall` exits `0`.
  - Output file: `clean_id2/obj/debug-matrix/wangzhe_clean.vmp_syscall.ll`
  - `__syscall_exit` is no longer present in the generated IR.

## Status
- Root cause confirmed: the workspace source already contains the `exit/_exit` skip, but the NDK `clang++.exe` used for Android builds was stale.
- Operational fix verified: syncing NDK `clang++.exe` from `build-windows/bin/clang++.exe` removes the no-`fla` `vmp + syscall` crash on both minimal repro and `clean_id2`.
