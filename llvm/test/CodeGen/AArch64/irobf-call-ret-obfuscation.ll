; RUN: llc -mtriple=aarch64-linux-gnu -mattr=+v8a -O2 -mllvm -aarch64-obfuscate-call-ret %s -o - | FileCheck %s

declare i32 @callee(i32)

define i32 @caller(i32 %x) local_unnamed_addr {
; CHECK-LABEL: caller:
; CHECK: adrp x16, callee
; CHECK-NEXT: add x16, x16, :lo12:callee
; CHECK-NEXT: blr x16
; CHECK-NOT: bl callee
; CHECK: br x30
entry:
  %call = call i32 @callee(i32 %x)
  %add = add nsw i32 %call, 1
  ret i32 %add
}
