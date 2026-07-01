; RUN: llc -mtriple=aarch64-linux-gnu -O2 -irobf-no-cfi %s -o - | FileCheck %s

define i32 @no_cfi(i32 %x) #0 {
entry:
  %slot = alloca i32, align 4
  store volatile i32 %x, ptr %slot, align 4
  %v = load volatile i32, ptr %slot, align 4
  %add = add nsw i32 %v, 1
  ret i32 %add
}

; CHECK-LABEL: no_cfi:
; CHECK-NOT: .cfi
; CHECK: ret

attributes #0 = { nounwind uwtable "frame-pointer"="all" }
