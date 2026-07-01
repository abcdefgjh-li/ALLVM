; ModuleID = 'D:\cpp\ALLVM\test\jni\frame_record_codegen.cpp'
source_filename = "D:\\cpp\\ALLVM\\test\\jni\\frame_record_codegen.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-android21"

; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local range(i32 -2147483641, -2147483648) i32 @frame_record_callee(i32 noundef %0) local_unnamed_addr #0 {
  %2 = add nsw i32 %0, 7
  ret i32 %2
}

; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local range(i32 -2147483648, 2147483632) i32 @frame_record_probe(i32 noundef %0) local_unnamed_addr #0 {
  %2 = tail call i32 @frame_record_callee(i32 noundef %0)
  %3 = xor i32 %2, 90
  %4 = add nsw i32 %3, -16
  ret i32 %4
}

attributes #0 = { mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Android (14475230, based on r574158) clang version 21.0.0 (https://android.googlesource.com/toolchain/llvm-project 9f872551d3c681d06fd303b36f16ed5c274735eb)"}
