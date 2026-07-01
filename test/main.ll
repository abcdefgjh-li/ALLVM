; ModuleID = 'D:\cpp\ALLVM\test\jni\main.cpp'
source_filename = "D:\\cpp\\ALLVM\\test\\jni\\main.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-android21"

%struct.__sFILE = type { [152 x i8] }
%"class.std::__ndk1::chrono::duration.1" = type { i64 }

@.str = private unnamed_addr constant [22 x i8] c"CSE_TEST_STRING_hello\00", align 1
@__sF = external global [0 x %struct.__sFILE], align 8
@.str.2 = private unnamed_addr constant [43 x i8] c"ALLVM_TEST_VALUES a=%d b=%d ref=%d len=%d\0A\00", align 1
@.str.3 = private unnamed_addr constant [17 x i8] c"ALLVM_TEST_FAIL\0A\00", align 1
@.str.4 = private unnamed_addr constant [17 x i8] c"ALLVM_TEST_PASS\0A\00", align 1
@str = private unnamed_addr constant [16 x i8] c"ALLVM_TEST_PASS\00", align 4
@str.5 = private unnamed_addr constant [16 x i8] c"ALLVM_TEST_FAIL\00", align 4

; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @_Z7ref_addii(i32 noundef %0, i32 noundef %1) local_unnamed_addr #0 {
  %3 = add nsw i32 %1, %0
  ret i32 %3
}

; Function Attrs: mustprogress nofree noinline nounwind uwtable
define dso_local noundef i32 @_Z7len_sumv() local_unnamed_addr #1 {
  %1 = tail call i32 @puts(ptr noundef nonnull dereferenceable(1) @.str)
  ret i32 44
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #2

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #3

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #2

; Function Attrs: mustprogress norecurse uwtable
define dso_local noundef range(i32 0, 2) i32 @main() local_unnamed_addr #4 personality ptr @__gxx_personality_v0 {
  %1 = alloca %"class.std::__ndk1::chrono::duration.1", align 8
  %2 = tail call i32 @setvbuf(ptr noundef nonnull getelementptr inbounds nuw (i8, ptr @__sF, i64 152), ptr noundef null, i32 noundef 2, i64 noundef 0)
  %3 = tail call i32 @setvbuf(ptr noundef nonnull getelementptr inbounds nuw (i8, ptr @__sF, i64 304), ptr noundef null, i32 noundef 2, i64 noundef 0)
  %4 = tail call noundef i32 @_Z7ref_addii(i32 noundef 17, i32 noundef 25)
  %5 = tail call noundef i32 @_Z7len_sumv()
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.2, i32 noundef 17, i32 noundef 25, i32 noundef %4, i32 noundef %5)
  %7 = tail call i32 (ptr, ptr, ...) @fprintf(ptr noundef nonnull getelementptr inbounds nuw (i8, ptr @__sF, i64 304), ptr noundef nonnull @.str.2, i32 noundef 17, i32 noundef 25, i32 noundef %4, i32 noundef %5) #7
  %8 = icmp eq i32 %4, 42
  %9 = icmp eq i32 %5, 44
  %10 = and i1 %8, %9
  br i1 %10, label %14, label %11

11:                                               ; preds = %0
  %12 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.5)
  %13 = tail call i64 @fwrite(ptr nonnull @.str.3, i64 16, i64 1, ptr nonnull getelementptr inbounds nuw (i8, ptr @__sF, i64 304))
  br label %17

14:                                               ; preds = %0
  %15 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %16 = tail call i64 @fwrite(ptr nonnull @.str.4, i64 16, i64 1, ptr nonnull getelementptr inbounds nuw (i8, ptr @__sF, i64 304))
  call void @llvm.lifetime.start.p0(i64 8, ptr nonnull %1) #7
  store i64 60000000000, ptr %1, align 8, !tbaa !6
  call void @_ZNSt6__ndk111this_thread9sleep_forERKNS_6chrono8durationIxNS_5ratioILl1ELl1000000000EEEEE(ptr noundef nonnull align 8 dereferenceable(8) %1)
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %1) #7
  br label %17

17:                                               ; preds = %14, %11
  %18 = phi i32 [ 1, %11 ], [ 0, %14 ]
  ret i32 %18
}

; Function Attrs: nofree nounwind
declare noundef i32 @setvbuf(ptr noundef captures(none), ptr noundef, i32 noundef, i64 noundef) local_unnamed_addr #3

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

; Function Attrs: nofree nounwind
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #3

declare void @_ZNSt6__ndk111this_thread9sleep_forERKNS_6chrono8durationIxNS_5ratioILl1ELl1000000000EEEEE(ptr noundef nonnull align 8 dereferenceable(8)) local_unnamed_addr #5

declare i32 @__gxx_personality_v0(...)

; Function Attrs: nofree nounwind
declare noundef i64 @fwrite(ptr noundef readonly captures(none), i64 noundef, i64 noundef, ptr noundef captures(none)) local_unnamed_addr #6

attributes #0 = { mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #1 = { mustprogress nofree noinline nounwind uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #2 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { nofree nounwind "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #4 = { mustprogress norecurse uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #5 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #6 = { nofree nounwind }
attributes #7 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"\0AA-protector\0ABy abcdefgjh.\0A\E6\98\A5\E5\8E\BB\E7\A7\8B\E6\9D\A5\EF\BC\8C\E5\B2\81\E6\9C\88\E5\A6\82\E6\AD\8C\E8\BD\BB\E5\94\B1\E3\80\82\E6\82\B2\E6\AC\A2\E7\A6\BB\E5\90\88\EF\BC\8C\E4\BA\BA\E7\94\9F\E5\A6\82\E6\A2\A6\E5\88\9D\E9\86\92\E3\80\82\0A"}
!6 = !{!7, !7, i64 0}
!7 = !{!"long long", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C++ TBAA"}
