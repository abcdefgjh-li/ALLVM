; RUN: llc -mtriple=aarch64-linux-gnu -O2 -aarch64-obfuscate-opaque-predicate %s -o - | FileCheck %s

define i32 @opaque_bytes(i32 %x) local_unnamed_addr {
; CHECK-LABEL: opaque_bytes:
; CHECK: mov x9, xzr
; CHECK-NEXT: cbnz x9, .LBB
; CHECK-NEXT: b .LBB
; CHECK: .LBB
; CHECK-NEXT: .byte 0
; CHECK-NEXT: .byte 0
; CHECK-NEXT: .byte 0
; CHECK-NEXT: .byte 0
; CHECK-NEXT: .byte 232
; CHECK-NEXT: .byte 3
; CHECK-NEXT: .p2align 2, 0x0
; CHECK-NEXT: b .LBB
; CHECK: .LBB
; CHECK-NEXT: add w0, w0, #1
; CHECK: ret
  %add = add nsw i32 %x, 1
  ret i32 %add
}
