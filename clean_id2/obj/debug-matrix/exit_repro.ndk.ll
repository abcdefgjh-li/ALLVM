; ModuleID = 'D:\cpp\obfuscator-ollvm-21.x\clean_id2\obj\debug-matrix\exit_repro.cpp'
source_filename = "D:\\cpp\\obfuscator-ollvm-21.x\\clean_id2\\obj\\debug-matrix\\exit_repro.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-none-linux-android21"

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local noundef i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  %2 = call i64 @__syscall_exit(i32 0)
  trunc i64 %2 to void
  unreachable
}

; Function Attrs: noreturn
declare void @exit(i32 noundef) #1

; Function Attrs: noinline optnone
define internal i64 @__syscall_exit(i32 %0) #2 {
  %2 = zext i32 %0 to i64
  %3 = call i64 asm sideeffect "svc #0", "={x0},{x0},{x1},{x2},{x3},{x4},{x5},{x8},~{memory},~{cc}"(i64 %2, i64 0, i64 0, i64 0, i64 0, i64 0, i64 93)
  ret i64 %3
}

attributes #0 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #1 = { noreturn "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #2 = { noinline optnone }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"\0AA-protector\0ABy abcdefgjh.\0A\E6\98\9F\E7\A7\BB\E6\96\97\E8\BD\AC\EF\BC\8C\E6\B2\A7\E6\B5\B7\E6\A1\91\E7\94\B0\E5\8F\98\E8\BF\81\E3\80\82\E5\88\9D\E5\BF\83\E5\A6\82\E7\A3\90\EF\BC\8C\E7\AC\83\E8\A1\8C\E8\87\B4\E8\BF\9C\E4\B8\8D\E6\80\A0\E3\80\82\0A"}
