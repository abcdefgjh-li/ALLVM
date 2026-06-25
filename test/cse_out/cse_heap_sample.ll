; ModuleID = 'd:\cpp\obfuscator-ollvm-21.x\test\cse_heap_sample.cpp'
source_filename = "d:\\cpp\\obfuscator-ollvm-21.x\\test\\cse_heap_sample.cpp"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.51.36248"

@g_sink = internal global i32 0, align 4
@"decptr_0??_C@_07DLHCIBDH@default?$AA@" = private global i64 0, section ".AProtect.data", !noobf !0
@"dec_status_0??_C@_07DLHCIBDH@default?$AA@" = private global i32 0, section ".AProtect.data", !noobf !0
@"decptr_1??_C@_0BI@NGJHCJLK@CSE_HEAP_SECRET_Alpha42?$AA@" = private global i64 0, section ".AProtect.data", !noobf !0
@"dec_status_1??_C@_0BI@NGJHCJLK@CSE_HEAP_SECRET_Alpha42?$AA@" = private global i32 0, section ".AProtect.data", !noobf !0
@"decptr_2??_C@_0BH@LJFKIDC@CSE_HEAP_BANNER_Beta24?$AA@" = private global i64 0, section ".AProtect.data", !noobf !0
@"dec_status_2??_C@_0BH@LJFKIDC@CSE_HEAP_BANNER_Beta24?$AA@" = private global i32 0, section ".AProtect.data", !noobf !0
@"decptr_3??_C@_0BG@NLIPMHED@CSE_HEAP_FAIL_Gamma66?$AA@" = private global i64 0, section ".AProtect.data", !noobf !0
@"dec_status_3??_C@_0BG@NLIPMHED@CSE_HEAP_FAIL_Gamma66?$AA@" = private global i32 0, section ".AProtect.data", !noobf !0
@EncryptedStringTable = private global [259 x i8] c"gU\BD\FEx\8F\1C\F3\15s\F6'\AF\85E\9EK\C7\80\CF\A5\8Ap)\11k \18\BEh\E3d\FFqx\BA\874\82\94t\E4b \FBb1\08^\9B64\E7)\1E\1F\8BD\D1\80\8A\A9;\C6\CFV\89\A6jV\1Dc\D1\F5*\EA\F7\D6]n*\02\A4\95\0C\08\F2\9BzqJ\E1wJ\EC+g[\98\92\BCig\F4X\FDynPMgwjhfL_R#\066j\990t\BBWf\E7\F5y\F6\12,\B7e\B1D\EFR \22F\AB\AEP`\D1+\B4\A5\92\AA\97S\F9\8BK\7F\EC\8E\E2\08\96O^\BCig\ECX\CDynP\1B|\F1c\07hN^X&\11l\99\CB\B2\1B\85\F9[P\A8v\E8_\89]\B8\B7\FD\DC\11\F1e\17\1B\ED\E6\11\E9B*\F48\B2)\E2\14\\\B8\19\10\BE\D9\FE\D1\AE\F4\A9\15&7H\BCig\E8XrynP\17xujTYW1\021)\93\C9", section ".AProtect.rodata", !noobf !0

; Function Attrs: mustprogress noinline norecurse uwtable
define dso_local noundef i32 @main(i32 noundef %0, ptr noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  store i32 0, ptr %3, align 4
  store ptr %1, ptr %4, align 8
  store i32 %0, ptr %5, align 4
  %7 = load i32, ptr %5, align 4
  %8 = icmp sgt i32 %7, 1
  br i1 %8, label %9, label %13

9:                                                ; preds = %2
  %10 = load ptr, ptr %4, align 8
  %11 = getelementptr inbounds ptr, ptr %10, i64 1
  %12 = load ptr, ptr %11, align 8
  br label %14

13:                                               ; preds = %2
  br label %14

14:                                               ; preds = %13, %9
  %15 = phi ptr [ %12, %9 ], [ %16, %13 ]
  %16 = call ptr @abcdefgjh_decrypt_string_0(ptr getelementptr inbounds ([259 x i8], ptr @EncryptedStringTable, i32 0, i32 30))
  store ptr %15, ptr %6, align 8
  %17 = load ptr, ptr %6, align 8
  call void @"?consume_secret@@YAXPEBD@Z"(ptr noundef %17)
  %18 = load atomic volatile i32, ptr @g_sink acquire, align 4
  %19 = icmp eq i32 %18, 0
  %20 = zext i1 %19 to i64
  %21 = select i1 %19, i32 1, i32 0
  ret i32 %21
}

; Function Attrs: mustprogress noinline uwtable
define internal void @"?consume_secret@@YAXPEBD@Z"(ptr noundef %0) #1 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %5 = call ptr @abcdefgjh_decrypt_string_1(ptr getelementptr inbounds ([259 x i8], ptr @EncryptedStringTable, i32 0, i32 73))
  store ptr %5, ptr %3, align 8
  %6 = call ptr @abcdefgjh_decrypt_string_2(ptr getelementptr inbounds ([259 x i8], ptr @EncryptedStringTable, i32 0, i32 145))
  store ptr %6, ptr %4, align 8
  %7 = load ptr, ptr %3, align 8
  %8 = call i64 @strlen(ptr noundef %7) #6
  %9 = trunc i64 %8 to i32
  %10 = load atomic volatile i32, ptr @g_sink acquire, align 4
  %11 = add nsw i32 %10, %9
  store atomic volatile i32 %11, ptr @g_sink release, align 4
  %12 = load ptr, ptr %3, align 8
  %13 = load ptr, ptr %2, align 8
  %14 = call i32 @strcmp(ptr noundef %13, ptr noundef %12) #6
  %15 = icmp eq i32 %14, 0
  br i1 %15, label %16, label %19

16:                                               ; preds = %1
  %17 = load ptr, ptr %4, align 8
  %18 = call i32 @puts(ptr noundef %17)
  br label %24

19:                                               ; preds = %1
  %20 = call ptr @abcdefgjh_decrypt_string_3(ptr getelementptr inbounds ([259 x i8], ptr @EncryptedStringTable, i32 0, i32 210))
  %21 = call i32 @puts(ptr noundef %20)
  %22 = load i64, ptr @"decptr_3??_C@_0BG@NLIPMHED@CSE_HEAP_FAIL_Gamma66?$AA@", align 8
  %23 = inttoptr i64 %22 to ptr
  call void @llvm.memset.p0.i64(ptr align 1 %23, i8 0, i64 22, i1 false)
  call void @free(ptr %23)
  store i64 0, ptr @"decptr_3??_C@_0BG@NLIPMHED@CSE_HEAP_FAIL_Gamma66?$AA@", align 8
  store i32 0, ptr @"dec_status_3??_C@_0BG@NLIPMHED@CSE_HEAP_FAIL_Gamma66?$AA@", align 4
  br label %24

24:                                               ; preds = %19, %16
  ret void
}

; Function Attrs: nounwind
declare dso_local i64 @strlen(ptr noundef) #2

; Function Attrs: nounwind
declare dso_local i32 @strcmp(ptr noundef, ptr noundef) #2

declare dso_local i32 @puts(ptr noundef) #3

define private ptr @abcdefgjh_decrypt_string_0(ptr captures(none) %0) {
  %2 = load i32, ptr @"dec_status_0??_C@_07DLHCIBDH@default?$AA@", align 4
  %3 = icmp eq i32 %2, 1
  br i1 %3, label %4, label %7

4:                                                ; preds = %1
  %5 = load i64, ptr @"decptr_0??_C@_07DLHCIBDH@default?$AA@", align 8
  %6 = inttoptr i64 %5 to ptr
  ret ptr %6

7:                                                ; preds = %1
  %8 = getelementptr inbounds i8, ptr %0, i32 19
  %9 = call ptr @malloc(i64 8)
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %9, ptr align 1 %8, i64 8, i1 false)
  br label %10

10:                                               ; preds = %32, %7
  %11 = phi i32 [ 0, %7 ], [ %35, %32 ]
  %12 = phi i8 [ 0, %7 ], [ %34, %32 ]
  %13 = urem i32 %11, 19
  %14 = getelementptr inbounds i8, ptr %0, i32 %13
  %15 = load i8, ptr %14, align 1
  %16 = getelementptr inbounds i8, ptr %9, i32 %11
  %17 = load volatile i8, ptr %16, align 1
  %18 = zext i8 %15 to i32
  %19 = mul i32 %13, %18
  %20 = and i32 %19, 1
  %21 = icmp eq i32 %20, 0
  br i1 %21, label %24, label %28

22:                                               ; preds = %32
  %23 = ptrtoint ptr %9 to i64
  store i64 %23, ptr @"decptr_0??_C@_07DLHCIBDH@default?$AA@", align 8
  store i32 1, ptr @"dec_status_0??_C@_07DLHCIBDH@default?$AA@", align 4
  ret ptr %9

24:                                               ; preds = %10
  %25 = add i8 %17, %12
  %26 = xor i8 %25, %15
  %27 = xor i8 %26, -1
  br label %32

28:                                               ; preds = %10
  %29 = sub i8 %17, %12
  %30 = xor i8 %29, %15
  %31 = sub i8 0, %30
  br label %32

32:                                               ; preds = %28, %24
  %33 = phi i8 [ %27, %24 ], [ %31, %28 ]
  %34 = xor i8 %33, %15
  store i8 %34, ptr %16, align 1
  %35 = add nuw nsw i32 %11, 1
  %36 = icmp eq i32 %35, 8
  br i1 %36, label %22, label %10
}

declare ptr @malloc(i64)

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #4

define private ptr @abcdefgjh_decrypt_string_1(ptr captures(none) %0) {
  %2 = load i32, ptr @"dec_status_1??_C@_0BI@NGJHCJLK@CSE_HEAP_SECRET_Alpha42?$AA@", align 4
  %3 = icmp eq i32 %2, 1
  br i1 %3, label %4, label %7

4:                                                ; preds = %1
  %5 = load i64, ptr @"decptr_1??_C@_0BI@NGJHCJLK@CSE_HEAP_SECRET_Alpha42?$AA@", align 8
  %6 = inttoptr i64 %5 to ptr
  ret ptr %6

7:                                                ; preds = %1
  %8 = getelementptr inbounds i8, ptr %0, i32 27
  %9 = call ptr @malloc(i64 24)
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %9, ptr align 1 %8, i64 24, i1 false)
  br label %10

10:                                               ; preds = %32, %7
  %11 = phi i32 [ 0, %7 ], [ %35, %32 ]
  %12 = phi i8 [ 0, %7 ], [ %34, %32 ]
  %13 = urem i32 %11, 27
  %14 = getelementptr inbounds i8, ptr %0, i32 %13
  %15 = load i8, ptr %14, align 1
  %16 = getelementptr inbounds i8, ptr %9, i32 %11
  %17 = load volatile i8, ptr %16, align 1
  %18 = zext i8 %15 to i32
  %19 = mul i32 %13, %18
  %20 = and i32 %19, 1
  %21 = icmp eq i32 %20, 0
  br i1 %21, label %24, label %28

22:                                               ; preds = %32
  %23 = ptrtoint ptr %9 to i64
  store i64 %23, ptr @"decptr_1??_C@_0BI@NGJHCJLK@CSE_HEAP_SECRET_Alpha42?$AA@", align 8
  store i32 1, ptr @"dec_status_1??_C@_0BI@NGJHCJLK@CSE_HEAP_SECRET_Alpha42?$AA@", align 4
  ret ptr %9

24:                                               ; preds = %10
  %25 = add i8 %17, %12
  %26 = xor i8 %25, %15
  %27 = xor i8 %26, -1
  br label %32

28:                                               ; preds = %10
  %29 = sub i8 %17, %12
  %30 = xor i8 %29, %15
  %31 = sub i8 0, %30
  br label %32

32:                                               ; preds = %28, %24
  %33 = phi i8 [ %27, %24 ], [ %31, %28 ]
  %34 = xor i8 %33, %15
  store i8 %34, ptr %16, align 1
  %35 = add nuw nsw i32 %11, 1
  %36 = icmp eq i32 %35, 24
  br i1 %36, label %22, label %10
}

define private ptr @abcdefgjh_decrypt_string_2(ptr captures(none) %0) {
  %2 = load i32, ptr @"dec_status_2??_C@_0BH@LJFKIDC@CSE_HEAP_BANNER_Beta24?$AA@", align 4
  %3 = icmp eq i32 %2, 1
  br i1 %3, label %4, label %7

4:                                                ; preds = %1
  %5 = load i64, ptr @"decptr_2??_C@_0BH@LJFKIDC@CSE_HEAP_BANNER_Beta24?$AA@", align 8
  %6 = inttoptr i64 %5 to ptr
  ret ptr %6

7:                                                ; preds = %1
  %8 = getelementptr inbounds i8, ptr %0, i32 21
  %9 = call ptr @malloc(i64 23)
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %9, ptr align 1 %8, i64 23, i1 false)
  br label %10

10:                                               ; preds = %32, %7
  %11 = phi i32 [ 0, %7 ], [ %35, %32 ]
  %12 = phi i8 [ 0, %7 ], [ %34, %32 ]
  %13 = urem i32 %11, 21
  %14 = getelementptr inbounds i8, ptr %0, i32 %13
  %15 = load i8, ptr %14, align 1
  %16 = getelementptr inbounds i8, ptr %9, i32 %11
  %17 = load volatile i8, ptr %16, align 1
  %18 = zext i8 %15 to i32
  %19 = mul i32 %13, %18
  %20 = and i32 %19, 1
  %21 = icmp eq i32 %20, 0
  br i1 %21, label %24, label %28

22:                                               ; preds = %32
  %23 = ptrtoint ptr %9 to i64
  store i64 %23, ptr @"decptr_2??_C@_0BH@LJFKIDC@CSE_HEAP_BANNER_Beta24?$AA@", align 8
  store i32 1, ptr @"dec_status_2??_C@_0BH@LJFKIDC@CSE_HEAP_BANNER_Beta24?$AA@", align 4
  ret ptr %9

24:                                               ; preds = %10
  %25 = add i8 %17, %12
  %26 = xor i8 %25, %15
  %27 = xor i8 %26, -1
  br label %32

28:                                               ; preds = %10
  %29 = sub i8 %17, %12
  %30 = xor i8 %29, %15
  %31 = sub i8 0, %30
  br label %32

32:                                               ; preds = %28, %24
  %33 = phi i8 [ %27, %24 ], [ %31, %28 ]
  %34 = xor i8 %33, %15
  store i8 %34, ptr %16, align 1
  %35 = add nuw nsw i32 %11, 1
  %36 = icmp eq i32 %35, 23
  br i1 %36, label %22, label %10
}

define private ptr @abcdefgjh_decrypt_string_3(ptr captures(none) %0) {
  %2 = load i32, ptr @"dec_status_3??_C@_0BG@NLIPMHED@CSE_HEAP_FAIL_Gamma66?$AA@", align 4
  %3 = icmp eq i32 %2, 1
  br i1 %3, label %4, label %7

4:                                                ; preds = %1
  %5 = load i64, ptr @"decptr_3??_C@_0BG@NLIPMHED@CSE_HEAP_FAIL_Gamma66?$AA@", align 8
  %6 = inttoptr i64 %5 to ptr
  ret ptr %6

7:                                                ; preds = %1
  %8 = getelementptr inbounds i8, ptr %0, i32 27
  %9 = call ptr @malloc(i64 22)
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %9, ptr align 1 %8, i64 22, i1 false)
  br label %10

10:                                               ; preds = %32, %7
  %11 = phi i32 [ 0, %7 ], [ %35, %32 ]
  %12 = phi i8 [ 0, %7 ], [ %34, %32 ]
  %13 = urem i32 %11, 27
  %14 = getelementptr inbounds i8, ptr %0, i32 %13
  %15 = load i8, ptr %14, align 1
  %16 = getelementptr inbounds i8, ptr %9, i32 %11
  %17 = load volatile i8, ptr %16, align 1
  %18 = zext i8 %15 to i32
  %19 = mul i32 %13, %18
  %20 = and i32 %19, 1
  %21 = icmp eq i32 %20, 0
  br i1 %21, label %24, label %28

22:                                               ; preds = %32
  %23 = ptrtoint ptr %9 to i64
  store i64 %23, ptr @"decptr_3??_C@_0BG@NLIPMHED@CSE_HEAP_FAIL_Gamma66?$AA@", align 8
  store i32 1, ptr @"dec_status_3??_C@_0BG@NLIPMHED@CSE_HEAP_FAIL_Gamma66?$AA@", align 4
  ret ptr %9

24:                                               ; preds = %10
  %25 = add i8 %17, %12
  %26 = xor i8 %25, %15
  %27 = xor i8 %26, -1
  br label %32

28:                                               ; preds = %10
  %29 = sub i8 %17, %12
  %30 = xor i8 %29, %15
  %31 = sub i8 0, %30
  br label %32

32:                                               ; preds = %28, %24
  %33 = phi i8 [ %27, %24 ], [ %31, %28 ]
  %34 = xor i8 %33, %15
  store i8 %34, ptr %16, align 1
  %35 = add nuw nsw i32 %11, 1
  %36 = icmp eq i32 %35, 22
  br i1 %36, label %22, label %10
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #5

declare void @free(ptr)

attributes #0 = { mustprogress noinline norecurse uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress noinline uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #5 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #6 = { nounwind }

!llvm.dbg.cu = !{!1}
!llvm.linker.options = !{!3}
!llvm.module.flags = !{!4, !5, !6, !7, !8}
!llvm.ident = !{!9}

!0 = !{}
!1 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !2, producer: "\0AA-protector\0ABy abcdefgjh.\0A\E6\98\9F\E7\A7\BB\E6\96\97\E8\BD\AC\EF\BC\8C\E6\B2\A7\E6\B5\B7\E6\A1\91\E7\94\B0\E5\8F\98\E8\BF\81\E3\80\82\E5\88\9D\E5\BF\83\E5\A6\82\E7\A3\90\EF\BC\8C\E7\AC\83\E8\A1\8C\E8\87\B4\E8\BF\9C\E4\B8\8D\E6\80\A0\E3\80\82\0A", isOptimized: false, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: None)
!2 = !DIFile(filename: "d:\\cpp\\obfuscator-ollvm-21.x\\test\\cse_heap_sample.cpp", directory: "D:\\cpp\\obfuscator-ollvm-21.x")
!3 = !{!"/FAILIFMISMATCH:\22_CRT_STDIO_ISO_WIDE_SPECIFIERS=0\22"}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 2}
!6 = !{i32 8, !"PIC Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 2}
!8 = !{i32 1, !"MaxTLSAlign", i32 65536}
!9 = !{!"\0AA-protector\0ABy abcdefgjh.\0A\E8\90\BD\E5\8F\B6\E9\A3\98\E9\9B\B6\EF\BC\8C\E7\A7\8B\E5\8E\BB\E5\86\AC\E6\9D\A5\E6\97\A0\E5\A3\B0\E3\80\82\E5\BE\80\E4\BA\8B\E5\A6\82\E7\83\9F\EF\BC\8C\E6\95\85\E5\9B\AD\E5\B7\B2\E8\8D\92\EF\BC\8C\E4\BD\95\E5\A4\84\E6\98\AF\E5\AE\B6\E3\80\82\0A"}
