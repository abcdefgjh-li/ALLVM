[OPEN] vmp-exception-crash

## Symptom
- `vmp_exception_test_debug` 在模拟器上执行 `translate_std_exception` 时先得到错误结果 `actual=1, expected=302`，随后触发 `SIGSEGV`.
- 无 VMP 对照版可完整通过。

## Repro
1. 使用 `jni/Android-vmp-exception-debug.mk` 编译 `vmp_exception_test_debug`
2. `adb push` 到 `/data/local/tmp/vmp_exception_test_debug`
3. 执行 `/data/local/tmp/vmp_exception_test_debug`

## Hypotheses
1. `invoke` 虽然在 `CALL_MAP` 中保留，但 `callinst_handler` 生成的 unwind 路径没有把异常状态完整写回 VM 全局，导致后续 `landingpad` 读取到损坏状态。
2. `call_handler_with_exception_handling()` 与新的 `InvokeInst` 方案并存，重复消费/覆盖异常对象，导致 `exception_thrown`、`exception_selector`、`caught_exception_selector` 不一致。
3. `landingpad_handler()` 使用的 `caught_exception_ptr/caught_exception_selector` 与 `exception_ptr/exception_selector` 语义不一致，导致异常匹配前已经访问了无效地址。
4. `Call_OP` 返回后恢复 `ip/opcode_xorshift32_state/vm_code_state` 的逻辑在异常场景下不成立，造成后续 opcode 解码错位并最终崩溃。
5. `__cxa_begin_catch/__cxa_end_catch/__cxa_rethrow/llvm.eh.typeid.for` 这类 EH helper 的编码格式仍和真实 EH CFG 不匹配，导致 VM 控制流偏离。

## Evidence
- 待补充

## Next
- 给运行时异常路径加最小插桩，观察 `Call_OP` 前后和 `landingpad/catchswitch/resume` 读取到的异常状态。
