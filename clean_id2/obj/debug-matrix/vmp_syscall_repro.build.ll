; ModuleID = 'D:\cpp\obfuscator-ollvm-21.x\clean_id2\obj\debug-matrix\vmp_syscall_repro.cpp'
source_filename = "D:\\cpp\\obfuscator-ollvm-21.x\\clean_id2\\obj\\debug-matrix\\vmp_syscall_repro.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-none-linux-android21"

%struct.VMTraceEntry = type { i8, i8, i8, i8, i32, i64, i64, i64, i64 }

@.str = private unnamed_addr constant [4 x i8] c"vmp\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [78 x i8] c"D:\\cpp\\obfuscator-ollvm-21.x\\clean_id2\\obj\\debug-matrix\\vmp_syscall_repro.cpp\00", section "llvm.metadata"
@llvm.global.annotations = appending global [1 x { ptr, ptr, ptr, i32, ptr }] [{ ptr, ptr, ptr, i32, ptr } { ptr @_Z8vm_entryv, ptr @.str, ptr @.str.1, i32 3, ptr null }], section "llvm.metadata"
@gv_code_seg__Z8vm_entryv = internal constant [36 x i8] c"6~\00\00zq\00\00\F05\DC\94\D2\1C\02/\98\1D\CD\CD\E2;\01\FB\C8J\D6\01B\B9\81yix\A8\E3", section ".AProtect.data"
@gv_data_seg__Z8vm_entryv = internal global [16 x i8] zeroinitializer, section ".AProtect.bss", align 16
@ip__Z8vm_entryv = internal global i32 0, section ".AProtect.data"
@data_seg_addr__Z8vm_entryv = internal global i64 0, section ".AProtect.data"
@code_seg_addr__Z8vm_entryv = internal global i64 0, section ".AProtect.data"
@exception_thrown__Z8vm_entryv = internal global i8 0, section ".AProtect.data"
@exception_ptr__Z8vm_entryv = internal global ptr null, section ".AProtect.data"
@exception_selector__Z8vm_entryv = internal global i32 0, section ".AProtect.data"
@last_br_from_bb_id__Z8vm_entryv = internal global i64 0, section ".AProtect.data"
@current_bb_id__Z8vm_entryv = internal global i64 0, section ".AProtect.data"
@vmp_debug_enabled__Z8vm_entryv = internal global i8 0, section ".AProtect.data"
@pointer_size__Z8vm_entryv = internal global i32 8, section ".AProtect.data"
@opcode_xorshift32_state__Z8vm_entryv = internal global i32 0, section ".AProtect.data"
@vm_code_state__Z8vm_entryv = internal global i32 0, section ".AProtect.data"
@last_br_from_bb_id__Z8vm_entryv.2 = internal global i64 0, section ".AProtect.data"
@current_bb_id__Z8vm_entryv.3 = internal global i64 0, section ".AProtect.data"
@vmp_debug_enabled__Z8vm_entryv.4 = internal global i8 0, section ".AProtect.data"
@vm_trace_next__Z8vm_entryv = dso_local local_unnamed_addr global i32 0, align 4
@vm_trace_total__Z8vm_entryv = dso_local local_unnamed_addr global i64 0, align 8
@.str__Z8vm_entryv = private unnamed_addr constant [114 x i8] c"\0A[VM_CRASH] reason=%s detail0=%llu detail1=%llu ip=%u current_bb=%llu last_br_from=%llu exception=%u selector=%d\0A\00", align 1
@.str.1__Z8vm_entryv = private unnamed_addr constant [59 x i8] c"[VM_CRASH] opcode_state=0x%08x vm_state=0x%08x code_bytes=\00", align 1
@.str.2__Z8vm_entryv = private unnamed_addr constant [5 x i8] c"%02x\00", align 1
@vm_trace_ring__Z8vm_entryv = dso_local local_unnamed_addr global [32 x %struct.VMTraceEntry] zeroinitializer, align 8
@.str.5__Z8vm_entryv = private unnamed_addr constant [66 x i8] c"[VM_CRASH][TRACE] bb ip=%u opcode_seed=0x%08llx vm_seed=0x%08llx\0A\00", align 1
@.str.6__Z8vm_entryv = private unnamed_addr constant [99 x i8] c"[VM_CRASH][TRACE] opcode ip=%u op=%u raw=0x%02llx bb=%llu opcode_state=0x%08llx vm_state=0x%08llx\0A\00", align 1
@.str.7__Z8vm_entryv = private unnamed_addr constant [82 x i8] c"[VM_CRASH][TRACE] br source=%llu target=%llu type=%u flag=%u aux0=%llu aux1=%llu\0A\00", align 1
@.str.8__Z8vm_entryv = private unnamed_addr constant [96 x i8] c"[VM_CRASH][TRACE] call stage=%s funcid=%llu saved_ip=%llu res_offset=%llu exc=%u selector=%llu\0A\00", align 1
@.str.9__Z8vm_entryv = private unnamed_addr constant [6 x i8] c"leave\00", align 1
@.str.10__Z8vm_entryv = private unnamed_addr constant [6 x i8] c"enter\00", align 1
@.str.11__Z8vm_entryv = private unnamed_addr constant [132 x i8] c"[EH_LPAD] begin res_offset=%llu clauses=%u exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.12__Z8vm_entryv = private unnamed_addr constant [136 x i8] c"[EH_LPAD] end stored_ptr=%p stored_selector=%d exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.13__Z8vm_entryv = private unnamed_addr constant [118 x i8] c"[EH_RESUME] exc_value=0x%llx exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.14__Z8vm_entryv = private unnamed_addr constant [115 x i8] c"[EH_CSW] no-exception handlers=%u unwind=%llu exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.15__Z8vm_entryv = private unnamed_addr constant [128 x i8] c"[EH_CSW] begin handlers=%u unwind=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.16__Z8vm_entryv = private unnamed_addr constant [81 x i8] c"[EH_CSW] matched target=%llu adjusted_ptr=%p exception_ptr=%p selector=%d ip=%u\0A\00", align 1
@.str.17__Z8vm_entryv = private unnamed_addr constant [175 x i8] c"[EH_VM_CALL] before funcid=%llu saved_ip=%u offset=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d opcode_state=0x%08x vm_state=0x%08x\0A\00", align 1
@.str.18__Z8vm_entryv = private unnamed_addr constant [180 x i8] c"[EH_VM_CALL] after funcid=%llu saved_ip=%u offset=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u opcode_state=0x%08x vm_state=0x%08x\0A\00", align 1
@.str.19__Z8vm_entryv = private unnamed_addr constant [167 x i8] c"[EH_VM_CALL] restored funcid=%llu restore_ip=%u exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d opcode_state=0x%08x vm_state=0x%08x\0A\00", align 1
@.str.20__Z8vm_entryv = private unnamed_addr constant [126 x i8] c"\0A[VM_CRASH] reason=unknown_opcode detail0=%llu detail1=%llu ip=%u current_bb=%llu last_br_from=%llu exception=%u selector=%d\0A\00", align 1
@.str.21__Z8vm_entryv = private unnamed_addr constant [39 x i8] c"[VM] Unknown opcode 0x%02x, returning\0A\00", align 1
@vm_trace_next__Z8vm_entryv.5 = dso_local local_unnamed_addr global i32 0, align 4
@vm_trace_total__Z8vm_entryv.6 = dso_local local_unnamed_addr global i64 0, align 8
@.str__Z8vm_entryv.7 = private unnamed_addr constant [114 x i8] c"\0A[VM_CRASH] reason=%s detail0=%llu detail1=%llu ip=%u current_bb=%llu last_br_from=%llu exception=%u selector=%d\0A\00", align 1
@.str.1__Z8vm_entryv.8 = private unnamed_addr constant [59 x i8] c"[VM_CRASH] opcode_state=0x%08x vm_state=0x%08x code_bytes=\00", align 1
@.str.2__Z8vm_entryv.9 = private unnamed_addr constant [5 x i8] c"%02x\00", align 1
@vm_trace_ring__Z8vm_entryv.10 = dso_local local_unnamed_addr global [32 x %struct.VMTraceEntry] zeroinitializer, align 8
@.str.5__Z8vm_entryv.11 = private unnamed_addr constant [66 x i8] c"[VM_CRASH][TRACE] bb ip=%u opcode_seed=0x%08llx vm_seed=0x%08llx\0A\00", align 1
@.str.6__Z8vm_entryv.12 = private unnamed_addr constant [99 x i8] c"[VM_CRASH][TRACE] opcode ip=%u op=%u raw=0x%02llx bb=%llu opcode_state=0x%08llx vm_state=0x%08llx\0A\00", align 1
@.str.7__Z8vm_entryv.13 = private unnamed_addr constant [82 x i8] c"[VM_CRASH][TRACE] br source=%llu target=%llu type=%u flag=%u aux0=%llu aux1=%llu\0A\00", align 1
@.str.8__Z8vm_entryv.14 = private unnamed_addr constant [96 x i8] c"[VM_CRASH][TRACE] call stage=%s funcid=%llu saved_ip=%llu res_offset=%llu exc=%u selector=%llu\0A\00", align 1
@.str.9__Z8vm_entryv.15 = private unnamed_addr constant [6 x i8] c"leave\00", align 1
@.str.10__Z8vm_entryv.16 = private unnamed_addr constant [6 x i8] c"enter\00", align 1
@.str.11__Z8vm_entryv.17 = private unnamed_addr constant [132 x i8] c"[EH_LPAD] begin res_offset=%llu clauses=%u exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.12__Z8vm_entryv.18 = private unnamed_addr constant [136 x i8] c"[EH_LPAD] end stored_ptr=%p stored_selector=%d exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.13__Z8vm_entryv.19 = private unnamed_addr constant [118 x i8] c"[EH_RESUME] exc_value=0x%llx exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.14__Z8vm_entryv.20 = private unnamed_addr constant [115 x i8] c"[EH_CSW] no-exception handlers=%u unwind=%llu exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.15__Z8vm_entryv.21 = private unnamed_addr constant [128 x i8] c"[EH_CSW] begin handlers=%u unwind=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.16__Z8vm_entryv.22 = private unnamed_addr constant [81 x i8] c"[EH_CSW] matched target=%llu adjusted_ptr=%p exception_ptr=%p selector=%d ip=%u\0A\00", align 1
@.str.17__Z8vm_entryv.23 = private unnamed_addr constant [175 x i8] c"[EH_VM_CALL] before funcid=%llu saved_ip=%u offset=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d opcode_state=0x%08x vm_state=0x%08x\0A\00", align 1
@.str.18__Z8vm_entryv.24 = private unnamed_addr constant [180 x i8] c"[EH_VM_CALL] after funcid=%llu saved_ip=%u offset=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u opcode_state=0x%08x vm_state=0x%08x\0A\00", align 1
@.str.19__Z8vm_entryv.25 = private unnamed_addr constant [167 x i8] c"[EH_VM_CALL] restored funcid=%llu restore_ip=%u exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d opcode_state=0x%08x vm_state=0x%08x\0A\00", align 1
@.str.20__Z8vm_entryv.26 = private unnamed_addr constant [126 x i8] c"\0A[VM_CRASH] reason=unknown_opcode detail0=%llu detail1=%llu ip=%u current_bb=%llu last_br_from=%llu exception=%u selector=%d\0A\00", align 1
@.str.21__Z8vm_entryv.27 = private unnamed_addr constant [39 x i8] c"[VM] Unknown opcode 0x%02x, returning\0A\00", align 1
@vm_trace_next__Z8vm_entryv.28 = dso_local local_unnamed_addr global i32 0, align 4
@vm_trace_total__Z8vm_entryv.29 = dso_local local_unnamed_addr global i64 0, align 8
@.str__Z8vm_entryv.30 = private unnamed_addr constant [114 x i8] c"\0A[VM_CRASH] reason=%s detail0=%llu detail1=%llu ip=%u current_bb=%llu last_br_from=%llu exception=%u selector=%d\0A\00", align 1
@.str.1__Z8vm_entryv.31 = private unnamed_addr constant [59 x i8] c"[VM_CRASH] opcode_state=0x%08x vm_state=0x%08x code_bytes=\00", align 1
@.str.2__Z8vm_entryv.32 = private unnamed_addr constant [5 x i8] c"%02x\00", align 1
@vm_trace_ring__Z8vm_entryv.33 = dso_local local_unnamed_addr global [32 x %struct.VMTraceEntry] zeroinitializer, align 8
@.str.5__Z8vm_entryv.34 = private unnamed_addr constant [66 x i8] c"[VM_CRASH][TRACE] bb ip=%u opcode_seed=0x%08llx vm_seed=0x%08llx\0A\00", align 1
@.str.6__Z8vm_entryv.35 = private unnamed_addr constant [99 x i8] c"[VM_CRASH][TRACE] opcode ip=%u op=%u raw=0x%02llx bb=%llu opcode_state=0x%08llx vm_state=0x%08llx\0A\00", align 1
@.str.7__Z8vm_entryv.36 = private unnamed_addr constant [82 x i8] c"[VM_CRASH][TRACE] br source=%llu target=%llu type=%u flag=%u aux0=%llu aux1=%llu\0A\00", align 1
@.str.8__Z8vm_entryv.37 = private unnamed_addr constant [96 x i8] c"[VM_CRASH][TRACE] call stage=%s funcid=%llu saved_ip=%llu res_offset=%llu exc=%u selector=%llu\0A\00", align 1
@.str.9__Z8vm_entryv.38 = private unnamed_addr constant [6 x i8] c"leave\00", align 1
@.str.10__Z8vm_entryv.39 = private unnamed_addr constant [6 x i8] c"enter\00", align 1
@.str.11__Z8vm_entryv.40 = private unnamed_addr constant [132 x i8] c"[EH_LPAD] begin res_offset=%llu clauses=%u exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.12__Z8vm_entryv.41 = private unnamed_addr constant [136 x i8] c"[EH_LPAD] end stored_ptr=%p stored_selector=%d exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.13__Z8vm_entryv.42 = private unnamed_addr constant [118 x i8] c"[EH_RESUME] exc_value=0x%llx exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.14__Z8vm_entryv.43 = private unnamed_addr constant [115 x i8] c"[EH_CSW] no-exception handlers=%u unwind=%llu exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.15__Z8vm_entryv.44 = private unnamed_addr constant [128 x i8] c"[EH_CSW] begin handlers=%u unwind=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\0A\00", align 1
@.str.16__Z8vm_entryv.45 = private unnamed_addr constant [81 x i8] c"[EH_CSW] matched target=%llu adjusted_ptr=%p exception_ptr=%p selector=%d ip=%u\0A\00", align 1
@.str.17__Z8vm_entryv.46 = private unnamed_addr constant [175 x i8] c"[EH_VM_CALL] before funcid=%llu saved_ip=%u offset=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d opcode_state=0x%08x vm_state=0x%08x\0A\00", align 1
@.str.18__Z8vm_entryv.47 = private unnamed_addr constant [180 x i8] c"[EH_VM_CALL] after funcid=%llu saved_ip=%u offset=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u opcode_state=0x%08x vm_state=0x%08x\0A\00", align 1
@.str.19__Z8vm_entryv.48 = private unnamed_addr constant [167 x i8] c"[EH_VM_CALL] restored funcid=%llu restore_ip=%u exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d opcode_state=0x%08x vm_state=0x%08x\0A\00", align 1
@.str.20__Z8vm_entryv.49 = private unnamed_addr constant [126 x i8] c"\0A[VM_CRASH] reason=unknown_opcode detail0=%llu detail1=%llu ip=%u current_bb=%llu last_br_from=%llu exception=%u selector=%d\0A\00", align 1
@.str.21__Z8vm_entryv.50 = private unnamed_addr constant [39 x i8] c"[VM] Unknown opcode 0x%02x, returning\0A\00", align 1

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local noundef i32 @_Z8vm_entryv() #0 {
  store i64 ptrtoint (ptr @gv_data_seg__Z8vm_entryv to i64), ptr @data_seg_addr__Z8vm_entryv, align 8
  store i64 ptrtoint (ptr @gv_code_seg__Z8vm_entryv to i64), ptr @code_seg_addr__Z8vm_entryv, align 8
  store i32 0, ptr @ip__Z8vm_entryv, align 4
  store i8 0, ptr @exception_thrown__Z8vm_entryv, align 1
  store ptr null, ptr @exception_ptr__Z8vm_entryv, align 8
  store i32 0, ptr @exception_selector__Z8vm_entryv, align 4
  call void @vm_interpreter__Z8vm_entryv()
  %1 = load i8, ptr @exception_thrown__Z8vm_entryv, align 1
  %2 = icmp ne i8 %1, 0
  br i1 %2, label %5, label %3

3:                                                ; preds = %0
  %4 = load i32, ptr @gv_data_seg__Z8vm_entryv, align 4
  ret i32 %4

5:                                                ; preds = %0
  %6 = load ptr, ptr @exception_ptr__Z8vm_entryv, align 8
  call void @vmp_resume_unwind__Z8vm_entryv(ptr %6)
  unreachable
}

; Function Attrs: noreturn
declare void @exit(i32 noundef) #1

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local noundef i32 @main() #2 {
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  %2 = call noundef i32 @_Z8vm_entryv()
  ret i32 %2
}

define internal void @vm_interpreter_callinst_dispatch__Z8vm_entryv(i64 %0) section ".AProtect.text" {
  %2 = alloca i64, align 8
  store i64 %0, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = icmp eq i64 %3, 0
  br i1 %4, label %5, label %6

5:                                                ; preds = %1
  call void @exit(i32 0)
  ret void

6:                                                ; preds = %1
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #3

declare ptr @malloc(i64)

declare void @free(ptr)

declare i32 @printf(ptr, ...)

define internal void @vmp_debug_id__Z8vm_entryv(i32 %0, i64 %1) {
  ret void
}

declare void @_Unwind_Resume(ptr)

declare void @call_handler(i64)

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #4

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #4

declare i32 @fflush(ptr)

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #5

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.fmuladd.f32(float, float, float) #6

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.fmuladd.f64(double, double, double) #6

declare void @vmp_debug_id(i32, i64)

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare ptr @llvm.ptrmask.p0.i64(ptr, i64) #6

declare i32 @strcmp(ptr, ptr)

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.umin.i64(i64, i64) #6

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #6

declare i32 @putchar(i32)

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.umin.i32(i32, i32) #6

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.vector.reduce.or.v4i64(<4 x i64>) #6

; Function Attrs: noreturn nounwind uwtable
define internal void @vmp_resume_unwind__Z8vm_entryv(ptr noundef %0) local_unnamed_addr #7 section ".AProtect.text" {
  tail call void @_Unwind_Resume(ptr noundef %0) #10
  unreachable
}

; Function Attrs: nofree nounwind uwtable
define internal void @vm_dump_fault_context__Z8vm_entryv(ptr noundef %0, i64 noundef %1, i64 noundef %2) local_unnamed_addr #8 section ".AProtect.text" {
  %4 = load i64, ptr @vm_trace_total__Z8vm_entryv.6, align 8, !tbaa !6
  %5 = load i32, ptr @vm_trace_next__Z8vm_entryv.5, align 4
  %6 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7 = tail call i32 @llvm.smax.i32(i32 %6, i32 1)
  %8 = add nsw i32 %7, -1
  %9 = load i64, ptr @current_bb_id__Z8vm_entryv.3, align 8, !tbaa !6
  %10 = load i64, ptr @last_br_from_bb_id__Z8vm_entryv.2, align 8, !tbaa !6
  %11 = load i8, ptr @exception_thrown__Z8vm_entryv, align 4, !tbaa !12
  %12 = zext i8 %11 to i32
  %13 = load i32, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %14 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str__Z8vm_entryv.7, ptr noundef %0, i64 noundef %1, i64 noundef %2, i32 noundef %8, i64 noundef %9, i64 noundef %10, i32 noundef %12, i32 noundef %13)
  %15 = load i32, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  %16 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %17 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.1__Z8vm_entryv.8, i32 noundef %15, i32 noundef %16)
  %18 = icmp sgt i32 %6, 9
  %19 = add nsw i32 %7, -9
  %20 = select i1 %18, i32 %19, i32 0
  br label %37

21:                                               ; preds = %37
  %22 = add i32 %20, 23
  %23 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %24 = inttoptr i64 %23 to ptr
  %25 = zext i32 %22 to i64
  %26 = getelementptr inbounds nuw i8, ptr %24, i64 %25
  %27 = load i8, ptr %26, align 1, !tbaa !12
  %28 = zext i8 %27 to i32
  %29 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.2__Z8vm_entryv.9, i32 noundef %28)
  %30 = tail call i64 @llvm.umin.i64(i64 %4, i64 32)
  %31 = icmp ugt i64 %4, 32
  %32 = zext i32 %5 to i64
  %33 = select i1 %31, i64 %32, i64 32
  %34 = tail call i32 @putchar(i32 10)
  %35 = icmp eq i64 %4, 0
  br i1 %35, label %52, label %36

36:                                               ; preds = %21
  br label %54

37:                                               ; preds = %37, %3
  %38 = phi i64 [ 0, %3 ], [ %48, %37 ]
  %39 = trunc nuw nsw i64 %38 to i32
  %40 = add i32 %20, %39
  %41 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %42 = inttoptr i64 %41 to ptr
  %43 = zext i32 %40 to i64
  %44 = getelementptr inbounds nuw i8, ptr %42, i64 %43
  %45 = load i8, ptr %44, align 1, !tbaa !12
  %46 = zext i8 %45 to i32
  %47 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.2__Z8vm_entryv.9, i32 noundef %46)
  %48 = add nuw nsw i64 %38, 1
  %49 = tail call i32 @putchar(i32 32)
  %50 = icmp eq i64 %48, 23
  br i1 %50, label %21, label %37, !llvm.loop !13

51:                                               ; preds = %116
  br label %52

52:                                               ; preds = %51, %21
  %53 = tail call i32 @fflush(ptr noundef null)
  ret void

54:                                               ; preds = %36, %116
  %55 = phi i64 [ %117, %116 ], [ 0, %36 ]
  %56 = add nuw nsw i64 %55, %33
  %57 = and i64 %56, 31
  %58 = getelementptr inbounds nuw [32 x %struct.VMTraceEntry], ptr @vm_trace_ring__Z8vm_entryv.10, i64 0, i64 %57
  %59 = load i8, ptr %58, align 8, !tbaa !16
  switch i8 %59, label %116 [
    i8 1, label %60
    i8 2, label %68
    i8 3, label %83
    i8 4, label %99
  ]

60:                                               ; preds = %54
  %61 = getelementptr inbounds nuw i8, ptr %58, i64 4
  %62 = load i32, ptr %61, align 4, !tbaa !18
  %63 = getelementptr inbounds nuw i8, ptr %58, i64 8
  %64 = load i64, ptr %63, align 8, !tbaa !19
  %65 = getelementptr inbounds nuw i8, ptr %58, i64 16
  %66 = load i64, ptr %65, align 8, !tbaa !20
  %67 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.5__Z8vm_entryv.11, i32 noundef %62, i64 noundef %64, i64 noundef %66)
  br label %116

68:                                               ; preds = %54
  %69 = getelementptr inbounds nuw i8, ptr %58, i64 4
  %70 = load i32, ptr %69, align 4, !tbaa !18
  %71 = getelementptr inbounds nuw i8, ptr %58, i64 1
  %72 = load i8, ptr %71, align 1, !tbaa !21
  %73 = zext i8 %72 to i32
  %74 = getelementptr inbounds nuw i8, ptr %58, i64 8
  %75 = load i64, ptr %74, align 8, !tbaa !19
  %76 = getelementptr inbounds nuw i8, ptr %58, i64 32
  %77 = load i64, ptr %76, align 8, !tbaa !22
  %78 = getelementptr inbounds nuw i8, ptr %58, i64 16
  %79 = load i64, ptr %78, align 8, !tbaa !20
  %80 = getelementptr inbounds nuw i8, ptr %58, i64 24
  %81 = load i64, ptr %80, align 8, !tbaa !23
  %82 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.6__Z8vm_entryv.12, i32 noundef %70, i32 noundef %73, i64 noundef %75, i64 noundef %77, i64 noundef %79, i64 noundef %81)
  br label %116

83:                                               ; preds = %54
  %84 = getelementptr inbounds nuw i8, ptr %58, i64 8
  %85 = load i64, ptr %84, align 8, !tbaa !19
  %86 = getelementptr inbounds nuw i8, ptr %58, i64 16
  %87 = load i64, ptr %86, align 8, !tbaa !20
  %88 = getelementptr inbounds nuw i8, ptr %58, i64 1
  %89 = load i8, ptr %88, align 1, !tbaa !21
  %90 = zext i8 %89 to i32
  %91 = getelementptr inbounds nuw i8, ptr %58, i64 2
  %92 = load i8, ptr %91, align 2, !tbaa !24
  %93 = zext i8 %92 to i32
  %94 = getelementptr inbounds nuw i8, ptr %58, i64 24
  %95 = load i64, ptr %94, align 8, !tbaa !23
  %96 = getelementptr inbounds nuw i8, ptr %58, i64 32
  %97 = load i64, ptr %96, align 8, !tbaa !22
  %98 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.7__Z8vm_entryv.13, i64 noundef %85, i64 noundef %87, i32 noundef %90, i32 noundef %93, i64 noundef %95, i64 noundef %97)
  br label %116

99:                                               ; preds = %54
  %100 = getelementptr inbounds nuw i8, ptr %58, i64 1
  %101 = load i8, ptr %100, align 1, !tbaa !21
  %102 = icmp eq i8 %101, 0
  %103 = select i1 %102, ptr @.str.10__Z8vm_entryv.16, ptr @.str.9__Z8vm_entryv.15
  %104 = getelementptr inbounds nuw i8, ptr %58, i64 8
  %105 = load i64, ptr %104, align 8, !tbaa !19
  %106 = getelementptr inbounds nuw i8, ptr %58, i64 16
  %107 = load i64, ptr %106, align 8, !tbaa !20
  %108 = getelementptr inbounds nuw i8, ptr %58, i64 24
  %109 = load i64, ptr %108, align 8, !tbaa !23
  %110 = getelementptr inbounds nuw i8, ptr %58, i64 2
  %111 = load i8, ptr %110, align 2, !tbaa !24
  %112 = zext i8 %111 to i32
  %113 = getelementptr inbounds nuw i8, ptr %58, i64 32
  %114 = load i64, ptr %113, align 8, !tbaa !22
  %115 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.8__Z8vm_entryv.14, ptr noundef nonnull %103, i64 noundef %105, i64 noundef %107, i64 noundef %109, i32 noundef %112, i64 noundef %114)
  br label %116

116:                                              ; preds = %99, %83, %68, %60, %54
  %117 = add nuw nsw i64 %55, 1
  %118 = icmp eq i64 %117, %30
  br i1 %118, label %51, label %54, !llvm.loop !25
}

; Function Attrs: nounwind uwtable
define internal void @vm_interpreter__Z8vm_entryv() local_unnamed_addr #9 section ".AProtect.text" {
  %1 = alloca [29 x i8], align 1
  %2 = alloca [16 x i8], align 8
  %3 = alloca [16 x i8], align 8
  %4 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %5 = icmp eq i8 %4, 0
  br i1 %5, label %7, label %6

6:                                                ; preds = %0
  tail call void @vmp_debug_id__Z8vm_entryv(i32 noundef 1, i64 noundef 999) #11
  br label %7

7:                                                ; preds = %6, %0
  store i32 8, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  store i32 0, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  br label %8

8:                                                ; preds = %2370, %7
  %9 = phi i64 [ 0, %7 ], [ %11, %2370 ]
  %10 = phi i8 [ 1, %7 ], [ %2371, %2370 ]
  %11 = add nuw nsw i64 %9, 1
  %12 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %13 = icmp eq i8 %12, 0
  br i1 %13, label %15, label %14

14:                                               ; preds = %8
  tail call void @vmp_debug_id__Z8vm_entryv(i32 noundef 10, i64 noundef %11) #11
  br label %15

15:                                               ; preds = %14, %8
  %16 = icmp eq i8 %10, 0
  %17 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %18 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  br i1 %16, label %92, label %19

19:                                               ; preds = %15
  %20 = inttoptr i64 %18 to ptr
  %21 = sext i32 %17 to i64
  %22 = add nsw i64 %21, 1
  %23 = trunc nsw i64 %22 to i32
  store i32 %23, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %24 = getelementptr inbounds i8, ptr %20, i64 %21
  %25 = load i8, ptr %24, align 1, !tbaa !12
  %26 = zext i8 %25 to i32
  %27 = add nsw i64 %21, 2
  %28 = trunc nsw i64 %27 to i32
  store i32 %28, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %29 = getelementptr inbounds i8, ptr %20, i64 %22
  %30 = load i8, ptr %29, align 1, !tbaa !12
  %31 = zext i8 %30 to i32
  %32 = shl nuw nsw i32 %31, 8
  %33 = or disjoint i32 %32, %26
  %34 = add nsw i64 %21, 3
  %35 = trunc nsw i64 %34 to i32
  store i32 %35, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %36 = getelementptr inbounds i8, ptr %20, i64 %27
  %37 = load i8, ptr %36, align 1, !tbaa !12
  %38 = zext i8 %37 to i32
  %39 = shl nuw nsw i32 %38, 16
  %40 = or disjoint i32 %39, %33
  %41 = add i32 %17, 4
  store i32 %41, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %42 = getelementptr inbounds i8, ptr %20, i64 %34
  %43 = load i8, ptr %42, align 1, !tbaa !12
  %44 = zext i8 %43 to i32
  %45 = shl nuw i32 %44, 24
  %46 = or disjoint i32 %45, %40
  store i32 %46, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  %47 = sext i32 %41 to i64
  %48 = add nsw i64 %47, 1
  %49 = trunc nsw i64 %48 to i32
  store i32 %49, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %50 = getelementptr inbounds i8, ptr %20, i64 %47
  %51 = load i8, ptr %50, align 1, !tbaa !12
  %52 = zext i8 %51 to i32
  %53 = add nsw i64 %47, 2
  %54 = trunc nsw i64 %53 to i32
  store i32 %54, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %55 = getelementptr inbounds i8, ptr %20, i64 %48
  %56 = load i8, ptr %55, align 1, !tbaa !12
  %57 = zext i8 %56 to i32
  %58 = shl nuw nsw i32 %57, 8
  %59 = or disjoint i32 %58, %52
  %60 = add nsw i64 %47, 3
  %61 = trunc nsw i64 %60 to i32
  store i32 %61, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %62 = getelementptr inbounds i8, ptr %20, i64 %53
  %63 = load i8, ptr %62, align 1, !tbaa !12
  %64 = zext i8 %63 to i32
  %65 = shl nuw nsw i32 %64, 16
  %66 = or disjoint i32 %65, %59
  %67 = add i32 %17, 8
  store i32 %67, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %68 = getelementptr inbounds i8, ptr %20, i64 %60
  %69 = load i8, ptr %68, align 1, !tbaa !12
  %70 = zext i8 %69 to i32
  %71 = shl nuw i32 %70, 24
  %72 = or disjoint i32 %71, %66
  store i32 %72, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %73 = sext i32 %67 to i64
  store i64 %73, ptr @current_bb_id__Z8vm_entryv.3, align 8, !tbaa !6
  %74 = zext i32 %46 to i64
  %75 = zext i32 %72 to i64
  %76 = load i32, ptr @vm_trace_next__Z8vm_entryv.28, align 4, !tbaa !10
  %77 = zext i32 %76 to i64
  %78 = getelementptr inbounds nuw [32 x %struct.VMTraceEntry], ptr @vm_trace_ring__Z8vm_entryv.33, i64 0, i64 %77
  store <4 x i8> <i8 1, i8 0, i8 0, i8 0>, ptr %78, align 8, !tbaa !12
  %79 = getelementptr inbounds nuw i8, ptr %78, i64 4
  store i32 %67, ptr %79, align 4, !tbaa !18
  %80 = getelementptr inbounds nuw i8, ptr %78, i64 8
  store i64 %74, ptr %80, align 8, !tbaa !19
  %81 = getelementptr inbounds nuw i8, ptr %78, i64 16
  store i64 %75, ptr %81, align 8, !tbaa !20
  %82 = getelementptr inbounds nuw i8, ptr %78, i64 24
  %83 = add i32 %76, 1
  %84 = and i32 %83, 31
  tail call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %82, i8 0, i64 16, i1 false)
  store i32 %84, ptr @vm_trace_next__Z8vm_entryv.28, align 4, !tbaa !10
  %85 = load i64, ptr @vm_trace_total__Z8vm_entryv.29, align 8, !tbaa !6
  %86 = add i64 %85, 1
  store i64 %86, ptr @vm_trace_total__Z8vm_entryv.29, align 8, !tbaa !6
  %87 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %88 = icmp eq i8 %87, 0
  br i1 %88, label %92, label %89

89:                                               ; preds = %19
  tail call void @vmp_debug_id__Z8vm_entryv(i32 noundef 1, i64 noundef %73) #11
  %90 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %91 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  br label %92

92:                                               ; preds = %89, %19, %15
  %93 = phi i64 [ %18, %19 ], [ %91, %89 ], [ %18, %15 ]
  %94 = phi i32 [ %67, %19 ], [ %90, %89 ], [ %17, %15 ]
  call void @llvm.lifetime.start.p0(i64 29, ptr nonnull %1) #11
  %95 = load i32, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  %96 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %97 = inttoptr i64 %93 to ptr
  %98 = add nsw i32 %94, 1
  store i32 %98, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %99 = sext i32 %94 to i64
  %100 = getelementptr inbounds i8, ptr %97, i64 %99
  %101 = load i8, ptr %100, align 1, !tbaa !12
  %102 = shl i32 %96, 13
  %103 = xor i32 %102, %96
  %104 = lshr i32 %103, 17
  %105 = xor i32 %104, %103
  %106 = shl i32 %105, 5
  %107 = xor i32 %106, %105
  store i32 %107, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %108 = trunc i32 %107 to i8
  %109 = xor i8 %101, %108
  br label %110

110:                                              ; preds = %209, %92
  %111 = phi i32 [ 0, %92 ], [ %210, %209 ]
  %112 = phi i32 [ %95, %92 ], [ %118, %209 ]
  %113 = shl i32 %112, 13
  %114 = xor i32 %113, %112
  %115 = lshr i32 %114, 17
  %116 = xor i32 %115, %114
  %117 = shl i32 %116, 5
  %118 = xor i32 %117, %116
  %119 = trunc i32 %118 to i8
  %120 = icmp eq i8 %109, %119
  br i1 %120, label %186, label %121

121:                                              ; preds = %110
  %122 = icmp sgt i32 %111, 0
  br i1 %122, label %123, label %205

123:                                              ; preds = %121
  %124 = zext nneg i32 %111 to i64
  %125 = icmp ult i32 %111, 4
  br i1 %125, label %126, label %129

126:                                              ; preds = %180, %158, %123
  %127 = phi i64 [ 0, %123 ], [ %132, %158 ], [ %165, %180 ]
  %128 = phi i8 [ 1, %123 ], [ %156, %158 ], [ %184, %180 ]
  br label %196

129:                                              ; preds = %123
  %130 = icmp ult i32 %111, 32
  br i1 %130, label %161, label %131

131:                                              ; preds = %129
  %132 = and i64 %124, 2147483616
  %133 = insertelement <16 x i8> poison, i8 %119, i64 0
  %134 = shufflevector <16 x i8> %133, <16 x i8> poison, <16 x i32> zeroinitializer
  br label %135

135:                                              ; preds = %135, %131
  %136 = phi i64 [ 0, %131 ], [ %147, %135 ]
  %137 = phi <16 x i1> [ zeroinitializer, %131 ], [ %145, %135 ]
  %138 = phi <16 x i1> [ zeroinitializer, %131 ], [ %146, %135 ]
  %139 = getelementptr inbounds nuw [29 x i8], ptr %1, i64 0, i64 %136
  %140 = getelementptr inbounds nuw i8, ptr %139, i64 16
  %141 = load <16 x i8>, ptr %139, align 1, !tbaa !12
  %142 = load <16 x i8>, ptr %140, align 1, !tbaa !12
  %143 = icmp eq <16 x i8> %141, %134
  %144 = icmp eq <16 x i8> %142, %134
  %145 = or <16 x i1> %137, %143
  %146 = or <16 x i1> %138, %144
  %147 = add nuw i64 %136, 32
  %148 = icmp eq i64 %147, %132
  br i1 %148, label %149, label %135, !llvm.loop !26

149:                                              ; preds = %135
  %150 = phi <16 x i1> [ %145, %135 ]
  %151 = phi <16 x i1> [ %146, %135 ]
  %152 = or <16 x i1> %151, %150
  %153 = freeze <16 x i1> %152
  %154 = bitcast <16 x i1> %153 to i16
  %155 = icmp eq i16 %154, 0
  %156 = zext i1 %155 to i8
  %157 = icmp eq i64 %132, %124
  br i1 %157, label %193, label %158

158:                                              ; preds = %149
  %159 = and i64 %124, 28
  %160 = icmp eq i64 %159, 0
  br i1 %160, label %126, label %161

161:                                              ; preds = %158, %129
  %162 = phi i64 [ %132, %158 ], [ 0, %129 ]
  %163 = phi i8 [ %156, %158 ], [ 1, %129 ]
  %164 = icmp eq i8 %163, 0
  %165 = and i64 %124, 2147483644
  %166 = insertelement <4 x i8> poison, i8 %119, i64 0
  %167 = shufflevector <4 x i8> %166, <4 x i8> poison, <4 x i32> zeroinitializer
  %168 = insertelement <4 x i1> poison, i1 %164, i64 0
  %169 = shufflevector <4 x i1> %168, <4 x i1> poison, <4 x i32> zeroinitializer
  br label %170

170:                                              ; preds = %170, %161
  %171 = phi i64 [ %162, %161 ], [ %178, %170 ]
  %172 = phi <4 x i1> [ %169, %161 ], [ %177, %170 ]
  %173 = getelementptr inbounds nuw [29 x i8], ptr %1, i64 0, i64 %171
  %174 = load <4 x i8>, ptr %173, align 1, !tbaa !12
  %175 = icmp eq <4 x i8> %174, %167
  %176 = or <4 x i1> %172, %175
  %177 = freeze <4 x i1> %176
  %178 = add nuw i64 %171, 4
  %179 = icmp eq i64 %178, %165
  br i1 %179, label %180, label %170, !llvm.loop !29

180:                                              ; preds = %170
  %181 = phi <4 x i1> [ %177, %170 ]
  %182 = bitcast <4 x i1> %181 to i4
  %183 = icmp eq i4 %182, 0
  %184 = zext i1 %183 to i8
  %185 = icmp eq i64 %165, %124
  br i1 %185, label %193, label %126

186:                                              ; preds = %110
  %187 = phi i32 [ %111, %110 ]
  %188 = phi i32 [ %118, %110 ]
  store i32 %188, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  %189 = trunc i32 %187 to i8
  %190 = add i8 %189, 1
  br label %214

191:                                              ; preds = %196
  %192 = phi i8 [ %202, %196 ]
  br label %193

193:                                              ; preds = %191, %180, %149
  %194 = phi i8 [ %156, %149 ], [ %184, %180 ], [ %192, %191 ]
  %195 = icmp eq i8 %194, 0
  br i1 %195, label %209, label %205

196:                                              ; preds = %196, %126
  %197 = phi i64 [ %203, %196 ], [ %127, %126 ]
  %198 = phi i8 [ %202, %196 ], [ %128, %126 ]
  %199 = getelementptr inbounds nuw [29 x i8], ptr %1, i64 0, i64 %197
  %200 = load i8, ptr %199, align 1, !tbaa !12
  %201 = icmp eq i8 %200, %119
  %202 = select i1 %201, i8 0, i8 %198
  %203 = add nuw nsw i64 %197, 1
  %204 = icmp eq i64 %203, %124
  br i1 %204, label %191, label %196, !llvm.loop !30

205:                                              ; preds = %193, %121
  %206 = sext i32 %111 to i64
  %207 = getelementptr inbounds [29 x i8], ptr %1, i64 0, i64 %206
  store i8 %119, ptr %207, align 1, !tbaa !12
  %208 = add nsw i32 %111, 1
  br label %209

209:                                              ; preds = %205, %193
  %210 = phi i32 [ %208, %205 ], [ %111, %193 ]
  %211 = icmp sgt i32 %210, 28
  br i1 %211, label %212, label %110, !llvm.loop !31

212:                                              ; preds = %209
  %213 = phi i32 [ %118, %209 ]
  store i32 %213, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  br label %214

214:                                              ; preds = %212, %186
  %215 = phi i8 [ %190, %186 ], [ -1, %212 ]
  %216 = zext i8 %101 to i64
  %217 = zext i32 %95 to i64
  %218 = zext i32 %96 to i64
  %219 = load i64, ptr @current_bb_id__Z8vm_entryv.3, align 8, !tbaa !6
  %220 = load i32, ptr @vm_trace_next__Z8vm_entryv.28, align 4, !tbaa !10
  %221 = zext i32 %220 to i64
  %222 = getelementptr inbounds nuw [32 x %struct.VMTraceEntry], ptr @vm_trace_ring__Z8vm_entryv.33, i64 0, i64 %221
  store i8 2, ptr %222, align 8, !tbaa !16
  %223 = getelementptr inbounds nuw i8, ptr %222, i64 1
  store i8 %215, ptr %223, align 1, !tbaa !21
  %224 = getelementptr inbounds nuw i8, ptr %222, i64 2
  store i8 0, ptr %224, align 2, !tbaa !24
  %225 = getelementptr inbounds nuw i8, ptr %222, i64 3
  store i8 0, ptr %225, align 1, !tbaa !32
  %226 = getelementptr inbounds nuw i8, ptr %222, i64 4
  store i32 %94, ptr %226, align 4, !tbaa !18
  %227 = getelementptr inbounds nuw i8, ptr %222, i64 8
  store i64 %216, ptr %227, align 8, !tbaa !19
  %228 = getelementptr inbounds nuw i8, ptr %222, i64 16
  store i64 %217, ptr %228, align 8, !tbaa !20
  %229 = getelementptr inbounds nuw i8, ptr %222, i64 24
  store i64 %218, ptr %229, align 8, !tbaa !23
  %230 = getelementptr inbounds nuw i8, ptr %222, i64 32
  store i64 %219, ptr %230, align 8, !tbaa !22
  %231 = add i32 %220, 1
  %232 = and i32 %231, 31
  store i32 %232, ptr @vm_trace_next__Z8vm_entryv.28, align 4, !tbaa !10
  %233 = load i64, ptr @vm_trace_total__Z8vm_entryv.29, align 8, !tbaa !6
  %234 = add i64 %233, 1
  store i64 %234, ptr @vm_trace_total__Z8vm_entryv.29, align 8, !tbaa !6
  call void @llvm.lifetime.end.p0(i64 29, ptr nonnull %1) #11
  %235 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %236 = icmp eq i8 %235, 0
  br i1 %236, label %239, label %237

237:                                              ; preds = %214
  %238 = zext i8 %215 to i64
  tail call void @vmp_debug_id__Z8vm_entryv(i32 noundef 2, i64 noundef %238) #11
  br label %239

239:                                              ; preds = %237, %214
  switch i8 %215, label %9925 [
    i8 0, label %2370
    i8 1, label %305
    i8 2, label %412
    i8 3, label %576
    i8 4, label %979
    i8 5, label %1543
    i8 6, label %1888
    i8 7, label %2401
    i8 8, label %2683
    i8 11, label %3052
    i8 12, label %3475
    i8 13, label %4221
    i8 14, label %4514
    i8 15, label %4823
    i8 16, label %5263
    i8 17, label %5667
    i8 18, label %5899
    i8 19, label %6264
    i8 20, label %6671
    i8 21, label %7270
    i8 22, label %8022
    i8 23, label %240
    i8 10, label %8430
    i8 9, label %8570
    i8 24, label %8737
    i8 25, label %8815
    i8 26, label %8849
    i8 27, label %9189
    i8 28, label %9560
  ]

240:                                              ; preds = %239
  %241 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %242 = inttoptr i64 %241 to ptr
  %243 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %244 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %245 = sext i32 %243 to i64
  %246 = add nsw i64 %245, 1
  %247 = trunc nsw i64 %246 to i32
  store i32 %247, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %248 = getelementptr inbounds i8, ptr %242, i64 %245
  %249 = load i8, ptr %248, align 1, !tbaa !12
  %250 = shl i32 %244, 13
  %251 = xor i32 %250, %244
  %252 = lshr i32 %251, 17
  %253 = xor i32 %252, %251
  %254 = shl i32 %253, 5
  %255 = xor i32 %254, %253
  store i32 %255, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %256 = trunc i32 %255 to i8
  %257 = xor i8 %249, %256
  %258 = zext i8 %257 to i32
  %259 = add nsw i64 %245, 2
  %260 = trunc nsw i64 %259 to i32
  store i32 %260, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %261 = getelementptr inbounds i8, ptr %242, i64 %246
  %262 = load i8, ptr %261, align 1, !tbaa !12
  %263 = shl i32 %255, 13
  %264 = xor i32 %263, %255
  %265 = lshr i32 %264, 17
  %266 = xor i32 %265, %264
  %267 = shl i32 %266, 5
  %268 = xor i32 %267, %266
  store i32 %268, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %269 = trunc i32 %268 to i8
  %270 = xor i8 %262, %269
  %271 = zext i8 %270 to i32
  %272 = shl nuw nsw i32 %271, 8
  %273 = or disjoint i32 %272, %258
  %274 = add nsw i64 %245, 3
  %275 = trunc nsw i64 %274 to i32
  store i32 %275, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %276 = getelementptr inbounds i8, ptr %242, i64 %259
  %277 = load i8, ptr %276, align 1, !tbaa !12
  %278 = shl i32 %268, 13
  %279 = xor i32 %278, %268
  %280 = lshr i32 %279, 17
  %281 = xor i32 %280, %279
  %282 = shl i32 %281, 5
  %283 = xor i32 %282, %281
  store i32 %283, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %284 = trunc i32 %283 to i8
  %285 = xor i8 %277, %284
  %286 = zext i8 %285 to i32
  %287 = shl nuw nsw i32 %286, 16
  %288 = or disjoint i32 %287, %273
  %289 = add i32 %243, 4
  store i32 %289, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %290 = getelementptr inbounds i8, ptr %242, i64 %274
  %291 = load i8, ptr %290, align 1, !tbaa !12
  %292 = shl i32 %283, 13
  %293 = xor i32 %292, %283
  %294 = lshr i32 %293, 17
  %295 = xor i32 %294, %293
  %296 = shl i32 %295, 5
  %297 = xor i32 %296, %295
  store i32 %297, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %298 = trunc i32 %297 to i8
  %299 = xor i8 %291, %298
  %300 = zext i8 %299 to i32
  %301 = shl nuw i32 %300, 24
  %302 = or disjoint i32 %301, %288
  %303 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %304 = icmp sgt i32 %303, 0
  br i1 %304, label %8214, label %8245

305:                                              ; preds = %239
  %306 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %307 = inttoptr i64 %306 to ptr
  %308 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %309 = add nsw i32 %308, 1
  store i32 %309, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %310 = sext i32 %308 to i64
  %311 = getelementptr inbounds i8, ptr %307, i64 %310
  %312 = load i8, ptr %311, align 1, !tbaa !12
  %313 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %314 = shl i32 %313, 13
  %315 = xor i32 %314, %313
  %316 = lshr i32 %315, 17
  %317 = xor i32 %316, %315
  %318 = shl i32 %317, 5
  %319 = xor i32 %318, %317
  %320 = trunc i32 %319 to i8
  %321 = xor i8 %312, %320
  %322 = add nsw i32 %308, 2
  store i32 %322, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %323 = shl i32 %319, 13
  %324 = xor i32 %323, %319
  %325 = lshr i32 %324, 17
  %326 = xor i32 %325, %324
  %327 = shl i32 %326, 5
  %328 = xor i32 %327, %326
  store i32 %328, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %329 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %330 = icmp sgt i32 %329, 0
  br i1 %330, label %331, label %393

331:                                              ; preds = %305
  %332 = sext i32 %322 to i64
  %333 = zext nneg i32 %329 to i64
  br label %341

334:                                              ; preds = %341
  %335 = phi i64 [ %346, %341 ]
  %336 = phi i32 [ %355, %341 ]
  %337 = phi i64 [ %363, %341 ]
  %338 = shl i64 %335, 32
  %339 = ashr exact i64 %338, 32
  %340 = zext nneg i32 %329 to i64
  br label %366

341:                                              ; preds = %341, %331
  %342 = phi i64 [ %332, %331 ], [ %346, %341 ]
  %343 = phi i64 [ 0, %331 ], [ %364, %341 ]
  %344 = phi i64 [ 0, %331 ], [ %363, %341 ]
  %345 = phi i32 [ %328, %331 ], [ %355, %341 ]
  %346 = add nsw i64 %342, 1
  %347 = trunc nsw i64 %346 to i32
  store i32 %347, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %348 = getelementptr inbounds i8, ptr %307, i64 %342
  %349 = load i8, ptr %348, align 1, !tbaa !12
  %350 = shl i32 %345, 13
  %351 = xor i32 %350, %345
  %352 = lshr i32 %351, 17
  %353 = xor i32 %352, %351
  %354 = shl i32 %353, 5
  %355 = xor i32 %354, %353
  store i32 %355, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %356 = trunc i32 %355 to i8
  %357 = xor i8 %349, %356
  %358 = icmp samesign ult i64 %343, 8
  %359 = zext i8 %357 to i64
  %360 = shl nuw nsw i64 %343, 3
  %361 = shl nuw i64 %359, %360
  %362 = select i1 %358, i64 %361, i64 0
  %363 = or i64 %362, %344
  %364 = add nuw nsw i64 %343, 1
  %365 = icmp eq i64 %364, %333
  br i1 %365, label %334, label %341, !llvm.loop !33

366:                                              ; preds = %366, %334
  %367 = phi i64 [ %339, %334 ], [ %371, %366 ]
  %368 = phi i64 [ 0, %334 ], [ %389, %366 ]
  %369 = phi i64 [ 0, %334 ], [ %388, %366 ]
  %370 = phi i32 [ %336, %334 ], [ %380, %366 ]
  %371 = add nsw i64 %367, 1
  %372 = trunc nsw i64 %371 to i32
  store i32 %372, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %373 = getelementptr inbounds i8, ptr %307, i64 %367
  %374 = load i8, ptr %373, align 1, !tbaa !12
  %375 = shl i32 %370, 13
  %376 = xor i32 %375, %370
  %377 = lshr i32 %376, 17
  %378 = xor i32 %377, %376
  %379 = shl i32 %378, 5
  %380 = xor i32 %379, %378
  store i32 %380, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %381 = trunc i32 %380 to i8
  %382 = xor i8 %374, %381
  %383 = icmp samesign ult i64 %368, 8
  %384 = zext i8 %382 to i64
  %385 = shl nuw nsw i64 %368, 3
  %386 = shl nuw i64 %384, %385
  %387 = select i1 %383, i64 %386, i64 0
  %388 = or i64 %387, %369
  %389 = add nuw nsw i64 %368, 1
  %390 = icmp eq i64 %389, %340
  br i1 %390, label %391, label %366, !llvm.loop !33

391:                                              ; preds = %366
  %392 = phi i64 [ %388, %366 ]
  br label %393

393:                                              ; preds = %391, %305
  %394 = phi i64 [ 0, %305 ], [ %337, %391 ]
  %395 = phi i64 [ 0, %305 ], [ %392, %391 ]
  %396 = icmp eq i8 %312, %320
  br i1 %396, label %2370, label %397

397:                                              ; preds = %393
  %398 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %399 = add i64 %398, %394
  %400 = inttoptr i64 %399 to ptr
  %401 = zext i8 %321 to i32
  %402 = add i64 %398, %395
  br label %403

403:                                              ; preds = %403, %397
  %404 = phi i32 [ %410, %403 ], [ 0, %397 ]
  %405 = phi ptr [ %408, %403 ], [ %400, %397 ]
  %406 = phi i64 [ %409, %403 ], [ %402, %397 ]
  %407 = trunc i64 %406 to i8
  store i8 %407, ptr %405, align 1, !tbaa !12
  %408 = getelementptr inbounds nuw i8, ptr %405, i64 1
  %409 = lshr i64 %406, 8
  %410 = add nuw nsw i32 %404, 1
  %411 = icmp eq i32 %410, %401
  br i1 %411, label %2357, label %403, !llvm.loop !34

412:                                              ; preds = %239
  %413 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %414 = inttoptr i64 %413 to ptr
  %415 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %416 = add nsw i32 %415, 1
  store i32 %416, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %417 = sext i32 %415 to i64
  %418 = getelementptr inbounds i8, ptr %414, i64 %417
  %419 = load i8, ptr %418, align 1, !tbaa !12
  %420 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %421 = shl i32 %420, 13
  %422 = xor i32 %421, %420
  %423 = lshr i32 %422, 17
  %424 = xor i32 %423, %422
  %425 = shl i32 %424, 5
  %426 = xor i32 %425, %424
  %427 = trunc i32 %426 to i8
  %428 = xor i8 %419, %427
  %429 = add nsw i32 %415, 2
  %430 = shl i32 %426, 13
  %431 = xor i32 %430, %426
  %432 = lshr i32 %431, 17
  %433 = xor i32 %432, %431
  %434 = shl i32 %433, 5
  %435 = xor i32 %434, %433
  store i32 %435, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %436 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %437 = icmp sgt i32 %436, 0
  br i1 %437, label %438, label %470

438:                                              ; preds = %412
  %439 = sext i32 %429 to i64
  %440 = zext nneg i32 %436 to i64
  br label %441

441:                                              ; preds = %441, %438
  %442 = phi i64 [ %439, %438 ], [ %446, %441 ]
  %443 = phi i64 [ 0, %438 ], [ %464, %441 ]
  %444 = phi i64 [ 0, %438 ], [ %463, %441 ]
  %445 = phi i32 [ %435, %438 ], [ %455, %441 ]
  %446 = add nsw i64 %442, 1
  %447 = trunc nsw i64 %446 to i32
  store i32 %447, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %448 = getelementptr inbounds i8, ptr %414, i64 %442
  %449 = load i8, ptr %448, align 1, !tbaa !12
  %450 = shl i32 %445, 13
  %451 = xor i32 %450, %445
  %452 = lshr i32 %451, 17
  %453 = xor i32 %452, %451
  %454 = shl i32 %453, 5
  %455 = xor i32 %454, %453
  store i32 %455, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %456 = trunc i32 %455 to i8
  %457 = xor i8 %449, %456
  %458 = icmp samesign ult i64 %443, 8
  %459 = zext i8 %457 to i64
  %460 = shl nuw nsw i64 %443, 3
  %461 = shl nuw i64 %459, %460
  %462 = select i1 %458, i64 %461, i64 0
  %463 = or i64 %462, %444
  %464 = add nuw nsw i64 %443, 1
  %465 = icmp eq i64 %464, %440
  br i1 %465, label %466, label %441, !llvm.loop !33

466:                                              ; preds = %441
  %467 = phi i32 [ %447, %441 ]
  %468 = phi i32 [ %455, %441 ]
  %469 = phi i64 [ %463, %441 ]
  br label %470

470:                                              ; preds = %466, %412
  %471 = phi i32 [ %435, %412 ], [ %468, %466 ]
  %472 = phi i32 [ %429, %412 ], [ %467, %466 ]
  %473 = phi i64 [ 0, %412 ], [ %469, %466 ]
  %474 = shl i32 %471, 13
  %475 = xor i32 %474, %471
  %476 = lshr i32 %475, 17
  %477 = xor i32 %476, %475
  %478 = shl i32 %477, 5
  %479 = xor i32 %478, %477
  %480 = add nsw i32 %472, 2
  store i32 %480, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %481 = shl i32 %479, 13
  %482 = xor i32 %481, %479
  %483 = lshr i32 %482, 17
  %484 = xor i32 %483, %482
  %485 = shl i32 %484, 5
  %486 = xor i32 %485, %484
  store i32 %486, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br i1 %437, label %487, label %540

487:                                              ; preds = %470
  %488 = sext i32 %480 to i64
  %489 = zext nneg i32 %436 to i64
  br label %495

490:                                              ; preds = %495
  %491 = phi i64 [ %517, %495 ]
  %492 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %493 = inttoptr i64 %492 to ptr
  %494 = zext nneg i32 %436 to i64
  br label %520

495:                                              ; preds = %495, %487
  %496 = phi i64 [ %488, %487 ], [ %500, %495 ]
  %497 = phi i64 [ 0, %487 ], [ %518, %495 ]
  %498 = phi i64 [ 0, %487 ], [ %517, %495 ]
  %499 = phi i32 [ %486, %487 ], [ %509, %495 ]
  %500 = add nsw i64 %496, 1
  %501 = trunc nsw i64 %500 to i32
  store i32 %501, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %502 = getelementptr inbounds i8, ptr %414, i64 %496
  %503 = load i8, ptr %502, align 1, !tbaa !12
  %504 = shl i32 %499, 13
  %505 = xor i32 %504, %499
  %506 = lshr i32 %505, 17
  %507 = xor i32 %506, %505
  %508 = shl i32 %507, 5
  %509 = xor i32 %508, %507
  store i32 %509, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %510 = trunc i32 %509 to i8
  %511 = xor i8 %503, %510
  %512 = icmp samesign ult i64 %497, 8
  %513 = zext i8 %511 to i64
  %514 = shl nuw nsw i64 %497, 3
  %515 = shl nuw i64 %513, %514
  %516 = select i1 %512, i64 %515, i64 0
  %517 = or i64 %516, %498
  %518 = add nuw nsw i64 %497, 1
  %519 = icmp eq i64 %518, %489
  br i1 %519, label %490, label %495, !llvm.loop !33

520:                                              ; preds = %533, %490
  %521 = phi i64 [ 0, %490 ], [ %535, %533 ]
  %522 = phi i64 [ %491, %490 ], [ %524, %533 ]
  %523 = phi i64 [ 0, %490 ], [ %534, %533 ]
  %524 = add i64 %522, 1
  %525 = icmp samesign ult i64 %521, 8
  br i1 %525, label %526, label %533

526:                                              ; preds = %520
  %527 = getelementptr inbounds nuw i8, ptr %493, i64 %522
  %528 = load i8, ptr %527, align 1, !tbaa !12
  %529 = zext i8 %528 to i64
  %530 = shl nuw nsw i64 %521, 3
  %531 = shl nuw i64 %529, %530
  %532 = or i64 %531, %523
  br label %533

533:                                              ; preds = %526, %520
  %534 = phi i64 [ %532, %526 ], [ %523, %520 ]
  %535 = add nuw nsw i64 %521, 1
  %536 = icmp eq i64 %535, %494
  br i1 %536, label %537, label %520, !llvm.loop !35

537:                                              ; preds = %533
  %538 = phi i64 [ %534, %533 ]
  %539 = inttoptr i64 %538 to ptr
  br label %540

540:                                              ; preds = %537, %470
  %541 = phi ptr [ %539, %537 ], [ null, %470 ]
  %542 = zext i8 %428 to i32
  %543 = icmp eq i8 %419, %427
  br i1 %543, label %2370, label %544

544:                                              ; preds = %540
  %545 = zext i8 %428 to i64
  br label %546

546:                                              ; preds = %557, %544
  %547 = phi i64 [ 0, %544 ], [ %560, %557 ]
  %548 = phi ptr [ %541, %544 ], [ %559, %557 ]
  %549 = phi i64 [ 0, %544 ], [ %558, %557 ]
  %550 = icmp samesign ult i64 %547, 8
  br i1 %550, label %551, label %557

551:                                              ; preds = %546
  %552 = load i8, ptr %548, align 1, !tbaa !12
  %553 = zext i8 %552 to i64
  %554 = shl nuw nsw i64 %547, 3
  %555 = shl nuw i64 %553, %554
  %556 = or i64 %555, %549
  br label %557

557:                                              ; preds = %551, %546
  %558 = phi i64 [ %556, %551 ], [ %549, %546 ]
  %559 = getelementptr inbounds nuw i8, ptr %548, i64 1
  %560 = add nuw nsw i64 %547, 1
  %561 = icmp eq i64 %560, %545
  br i1 %561, label %562, label %546, !llvm.loop !36

562:                                              ; preds = %557
  %563 = phi i64 [ %558, %557 ]
  %564 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %565 = add i64 %564, %473
  %566 = inttoptr i64 %565 to ptr
  br label %567

567:                                              ; preds = %567, %562
  %568 = phi i32 [ %574, %567 ], [ 0, %562 ]
  %569 = phi ptr [ %572, %567 ], [ %566, %562 ]
  %570 = phi i64 [ %573, %567 ], [ %563, %562 ]
  %571 = trunc i64 %570 to i8
  store i8 %571, ptr %569, align 1, !tbaa !12
  %572 = getelementptr inbounds nuw i8, ptr %569, i64 1
  %573 = lshr i64 %570, 8
  %574 = add nuw nsw i32 %568, 1
  %575 = icmp eq i32 %574, %542
  br i1 %575, label %2358, label %567, !llvm.loop !34

576:                                              ; preds = %239
  %577 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %578 = inttoptr i64 %577 to ptr
  %579 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %580 = add nsw i32 %579, 1
  store i32 %580, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %581 = sext i32 %579 to i64
  %582 = getelementptr inbounds i8, ptr %578, i64 %581
  %583 = load i8, ptr %582, align 1, !tbaa !12
  %584 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %585 = shl i32 %584, 13
  %586 = xor i32 %585, %584
  %587 = lshr i32 %586, 17
  %588 = xor i32 %587, %586
  %589 = shl i32 %588, 5
  %590 = xor i32 %589, %588
  store i32 %590, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %591 = trunc i32 %590 to i8
  %592 = xor i8 %583, %591
  %593 = add nsw i32 %579, 2
  store i32 %593, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %594 = sext i32 %580 to i64
  %595 = getelementptr inbounds i8, ptr %578, i64 %594
  %596 = load i8, ptr %595, align 1, !tbaa !12
  %597 = shl i32 %590, 13
  %598 = xor i32 %597, %590
  %599 = lshr i32 %598, 17
  %600 = xor i32 %599, %598
  %601 = shl i32 %600, 5
  %602 = xor i32 %601, %600
  store i32 %602, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %603 = trunc i32 %602 to i8
  %604 = zext i8 %592 to i32
  %605 = icmp ugt i8 %592, 8
  %606 = icmp eq i8 %596, %603
  br i1 %605, label %607, label %776

607:                                              ; preds = %576
  br i1 %606, label %608, label %650

608:                                              ; preds = %607
  %609 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %610 = icmp sgt i32 %609, 0
  br i1 %610, label %611, label %643

611:                                              ; preds = %608
  %612 = sext i32 %593 to i64
  %613 = zext nneg i32 %609 to i64
  br label %614

614:                                              ; preds = %614, %611
  %615 = phi i64 [ %612, %611 ], [ %619, %614 ]
  %616 = phi i64 [ 0, %611 ], [ %637, %614 ]
  %617 = phi i64 [ 0, %611 ], [ %636, %614 ]
  %618 = phi i32 [ %602, %611 ], [ %628, %614 ]
  %619 = add nsw i64 %615, 1
  %620 = trunc nsw i64 %619 to i32
  store i32 %620, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %621 = getelementptr inbounds i8, ptr %578, i64 %615
  %622 = load i8, ptr %621, align 1, !tbaa !12
  %623 = shl i32 %618, 13
  %624 = xor i32 %623, %618
  %625 = lshr i32 %624, 17
  %626 = xor i32 %625, %624
  %627 = shl i32 %626, 5
  %628 = xor i32 %627, %626
  store i32 %628, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %629 = trunc i32 %628 to i8
  %630 = xor i8 %622, %629
  %631 = icmp samesign ult i64 %616, 8
  %632 = zext i8 %630 to i64
  %633 = shl nuw nsw i64 %616, 3
  %634 = shl nuw i64 %632, %633
  %635 = select i1 %631, i64 %634, i64 0
  %636 = or i64 %635, %617
  %637 = add nuw nsw i64 %616, 1
  %638 = icmp eq i64 %637, %613
  br i1 %638, label %639, label %614, !llvm.loop !33

639:                                              ; preds = %614
  %640 = phi i32 [ %620, %614 ]
  %641 = phi i32 [ %628, %614 ]
  %642 = phi i64 [ %636, %614 ]
  br label %643

643:                                              ; preds = %639, %608
  %644 = phi i32 [ %602, %608 ], [ %641, %639 ]
  %645 = phi i32 [ %593, %608 ], [ %640, %639 ]
  %646 = phi i64 [ 0, %608 ], [ %642, %639 ]
  %647 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %648 = inttoptr i64 %647 to ptr
  %649 = getelementptr inbounds nuw i8, ptr %648, i64 %646
  br label %694

650:                                              ; preds = %607
  %651 = zext i8 %592 to i64
  %652 = tail call noalias ptr @malloc(i64 noundef %651) #12
  %653 = icmp eq ptr %652, null
  br i1 %653, label %654, label %655

654:                                              ; preds = %650
  br label %677

655:                                              ; preds = %650
  %656 = sext i32 %593 to i64
  %657 = zext i8 %592 to i64
  br label %658

658:                                              ; preds = %658, %655
  %659 = phi i64 [ %656, %655 ], [ %662, %658 ]
  %660 = phi i64 [ 0, %655 ], [ %675, %658 ]
  %661 = phi i32 [ %602, %655 ], [ %671, %658 ]
  %662 = add nsw i64 %659, 1
  %663 = trunc nsw i64 %662 to i32
  store i32 %663, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %664 = getelementptr inbounds i8, ptr %578, i64 %659
  %665 = load i8, ptr %664, align 1, !tbaa !12
  %666 = shl i32 %661, 13
  %667 = xor i32 %666, %661
  %668 = lshr i32 %667, 17
  %669 = xor i32 %668, %667
  %670 = shl i32 %669, 5
  %671 = xor i32 %670, %669
  store i32 %671, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %672 = trunc i32 %671 to i8
  %673 = xor i8 %665, %672
  %674 = getelementptr inbounds nuw i8, ptr %652, i64 %660
  store i8 %673, ptr %674, align 1, !tbaa !12
  %675 = add nuw nsw i64 %660, 1
  %676 = icmp eq i64 %675, %657
  br i1 %676, label %691, label %658, !llvm.loop !37

677:                                              ; preds = %654, %677
  %678 = phi i32 [ %686, %677 ], [ 0, %654 ]
  %679 = phi i32 [ %685, %677 ], [ %602, %654 ]
  %680 = shl i32 %679, 13
  %681 = xor i32 %680, %679
  %682 = lshr i32 %681, 17
  %683 = xor i32 %682, %681
  %684 = shl i32 %683, 5
  %685 = xor i32 %684, %683
  %686 = add nuw nsw i32 %678, 1
  %687 = icmp eq i32 %686, %604
  br i1 %687, label %688, label %677, !llvm.loop !38

688:                                              ; preds = %677
  %689 = phi i32 [ %685, %677 ]
  %690 = add i32 %593, %604
  br label %694

691:                                              ; preds = %658
  %692 = phi i32 [ %663, %658 ]
  %693 = phi i32 [ %671, %658 ]
  br label %694

694:                                              ; preds = %691, %688, %643
  %695 = phi i32 [ %644, %643 ], [ %689, %688 ], [ %693, %691 ]
  %696 = phi i32 [ %645, %643 ], [ %690, %688 ], [ %692, %691 ]
  %697 = phi ptr [ null, %643 ], [ null, %688 ], [ %652, %691 ]
  %698 = phi ptr [ %649, %643 ], [ null, %688 ], [ %652, %691 ]
  %699 = shl i32 %695, 13
  %700 = xor i32 %699, %695
  %701 = lshr i32 %700, 17
  %702 = xor i32 %701, %700
  %703 = shl i32 %702, 5
  %704 = xor i32 %703, %702
  %705 = add nsw i32 %696, 2
  store i32 %705, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %706 = shl i32 %704, 13
  %707 = xor i32 %706, %704
  %708 = lshr i32 %707, 17
  %709 = xor i32 %708, %707
  %710 = shl i32 %709, 5
  %711 = xor i32 %710, %709
  store i32 %711, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %712 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %713 = icmp sgt i32 %712, 0
  br i1 %713, label %714, label %767

714:                                              ; preds = %694
  %715 = sext i32 %705 to i64
  %716 = zext nneg i32 %712 to i64
  br label %722

717:                                              ; preds = %722
  %718 = phi i64 [ %744, %722 ]
  %719 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %720 = inttoptr i64 %719 to ptr
  %721 = zext nneg i32 %712 to i64
  br label %747

722:                                              ; preds = %722, %714
  %723 = phi i64 [ %715, %714 ], [ %727, %722 ]
  %724 = phi i64 [ 0, %714 ], [ %745, %722 ]
  %725 = phi i64 [ 0, %714 ], [ %744, %722 ]
  %726 = phi i32 [ %711, %714 ], [ %736, %722 ]
  %727 = add nsw i64 %723, 1
  %728 = trunc nsw i64 %727 to i32
  store i32 %728, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %729 = getelementptr inbounds i8, ptr %578, i64 %723
  %730 = load i8, ptr %729, align 1, !tbaa !12
  %731 = shl i32 %726, 13
  %732 = xor i32 %731, %726
  %733 = lshr i32 %732, 17
  %734 = xor i32 %733, %732
  %735 = shl i32 %734, 5
  %736 = xor i32 %735, %734
  store i32 %736, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %737 = trunc i32 %736 to i8
  %738 = xor i8 %730, %737
  %739 = icmp samesign ult i64 %724, 8
  %740 = zext i8 %738 to i64
  %741 = shl nuw nsw i64 %724, 3
  %742 = shl nuw i64 %740, %741
  %743 = select i1 %739, i64 %742, i64 0
  %744 = or i64 %743, %725
  %745 = add nuw nsw i64 %724, 1
  %746 = icmp eq i64 %745, %716
  br i1 %746, label %717, label %722, !llvm.loop !33

747:                                              ; preds = %760, %717
  %748 = phi i64 [ 0, %717 ], [ %762, %760 ]
  %749 = phi i64 [ %718, %717 ], [ %751, %760 ]
  %750 = phi i64 [ 0, %717 ], [ %761, %760 ]
  %751 = add i64 %749, 1
  %752 = icmp samesign ult i64 %748, 8
  br i1 %752, label %753, label %760

753:                                              ; preds = %747
  %754 = getelementptr inbounds nuw i8, ptr %720, i64 %749
  %755 = load i8, ptr %754, align 1, !tbaa !12
  %756 = zext i8 %755 to i64
  %757 = shl nuw nsw i64 %748, 3
  %758 = shl nuw i64 %756, %757
  %759 = or i64 %758, %750
  br label %760

760:                                              ; preds = %753, %747
  %761 = phi i64 [ %759, %753 ], [ %750, %747 ]
  %762 = add nuw nsw i64 %748, 1
  %763 = icmp eq i64 %762, %721
  br i1 %763, label %764, label %747, !llvm.loop !35

764:                                              ; preds = %760
  %765 = phi i64 [ %761, %760 ]
  %766 = inttoptr i64 %765 to ptr
  br label %767

767:                                              ; preds = %764, %694
  %768 = phi ptr [ %766, %764 ], [ null, %694 ]
  %769 = icmp eq ptr %698, null
  %770 = zext i8 %592 to i64
  br i1 %769, label %772, label %771

771:                                              ; preds = %767
  tail call void @llvm.memcpy.p0.p0.i64(ptr align 1 %768, ptr nonnull align 1 %698, i64 %770, i1 false)
  br label %773

772:                                              ; preds = %767
  tail call void @llvm.memset.p0.i64(ptr align 1 %768, i8 0, i64 %770, i1 false)
  br label %773

773:                                              ; preds = %772, %771
  %774 = icmp eq ptr %697, null
  br i1 %774, label %2370, label %775

775:                                              ; preds = %773
  tail call void @free(ptr noundef nonnull %697)
  br label %2370

776:                                              ; preds = %576
  br i1 %606, label %782, label %777

777:                                              ; preds = %776
  %778 = icmp eq i8 %583, %591
  br i1 %778, label %894, label %779

779:                                              ; preds = %777
  %780 = sext i32 %593 to i64
  %781 = zext nneg i8 %592 to i64
  br label %865

782:                                              ; preds = %776
  %783 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %784 = icmp sgt i32 %783, 0
  br i1 %784, label %785, label %792

785:                                              ; preds = %782
  %786 = sext i32 %593 to i64
  %787 = zext nneg i32 %783 to i64
  br label %827

788:                                              ; preds = %827
  %789 = phi i32 [ %833, %827 ]
  %790 = phi i32 [ %841, %827 ]
  %791 = phi i64 [ %849, %827 ]
  br label %792

792:                                              ; preds = %788, %782
  %793 = phi i32 [ %602, %782 ], [ %790, %788 ]
  %794 = phi i32 [ %593, %782 ], [ %789, %788 ]
  %795 = phi i64 [ 0, %782 ], [ %791, %788 ]
  %796 = icmp eq i8 %583, %591
  br i1 %796, label %894, label %797

797:                                              ; preds = %792
  %798 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %799 = inttoptr i64 %798 to ptr
  %800 = zext nneg i8 %592 to i64
  %801 = icmp ult i8 %592, 4
  br i1 %801, label %802, label %806

802:                                              ; preds = %823, %797
  %803 = phi i64 [ 0, %797 ], [ %807, %823 ]
  %804 = phi i64 [ %795, %797 ], [ %808, %823 ]
  %805 = phi i64 [ 0, %797 ], [ %825, %823 ]
  br label %852

806:                                              ; preds = %797
  %807 = and i64 %800, 12
  %808 = add i64 %795, %807
  %809 = getelementptr i8, ptr %799, i64 %795
  br label %810

810:                                              ; preds = %810, %806
  %811 = phi i64 [ 0, %806 ], [ %820, %810 ]
  %812 = phi <4 x i64> [ <i64 0, i64 1, i64 2, i64 3>, %806 ], [ %821, %810 ]
  %813 = phi <4 x i64> [ zeroinitializer, %806 ], [ %819, %810 ]
  %814 = getelementptr i8, ptr %809, i64 %811
  %815 = load <4 x i8>, ptr %814, align 1, !tbaa !12
  %816 = zext <4 x i8> %815 to <4 x i64>
  %817 = shl nuw nsw <4 x i64> %812, splat (i64 3)
  %818 = shl nuw <4 x i64> %816, %817
  %819 = or <4 x i64> %818, %813
  %820 = add nuw i64 %811, 4
  %821 = add <4 x i64> %812, splat (i64 4)
  %822 = icmp eq i64 %820, %807
  br i1 %822, label %823, label %810, !llvm.loop !39

823:                                              ; preds = %810
  %824 = phi <4 x i64> [ %819, %810 ]
  %825 = tail call i64 @llvm.vector.reduce.or.v4i64(<4 x i64> %824)
  %826 = icmp eq i64 %807, %800
  br i1 %826, label %894, label %802

827:                                              ; preds = %827, %785
  %828 = phi i64 [ %786, %785 ], [ %832, %827 ]
  %829 = phi i64 [ 0, %785 ], [ %850, %827 ]
  %830 = phi i64 [ 0, %785 ], [ %849, %827 ]
  %831 = phi i32 [ %602, %785 ], [ %841, %827 ]
  %832 = add nsw i64 %828, 1
  %833 = trunc nsw i64 %832 to i32
  store i32 %833, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %834 = getelementptr inbounds i8, ptr %578, i64 %828
  %835 = load i8, ptr %834, align 1, !tbaa !12
  %836 = shl i32 %831, 13
  %837 = xor i32 %836, %831
  %838 = lshr i32 %837, 17
  %839 = xor i32 %838, %837
  %840 = shl i32 %839, 5
  %841 = xor i32 %840, %839
  store i32 %841, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %842 = trunc i32 %841 to i8
  %843 = xor i8 %835, %842
  %844 = icmp samesign ult i64 %829, 8
  %845 = zext i8 %843 to i64
  %846 = shl nuw nsw i64 %829, 3
  %847 = shl nuw i64 %845, %846
  %848 = select i1 %844, i64 %847, i64 0
  %849 = or i64 %848, %830
  %850 = add nuw nsw i64 %829, 1
  %851 = icmp eq i64 %850, %787
  br i1 %851, label %788, label %827, !llvm.loop !33

852:                                              ; preds = %852, %802
  %853 = phi i64 [ %863, %852 ], [ %803, %802 ]
  %854 = phi i64 [ %856, %852 ], [ %804, %802 ]
  %855 = phi i64 [ %862, %852 ], [ %805, %802 ]
  %856 = add i64 %854, 1
  %857 = getelementptr inbounds nuw i8, ptr %799, i64 %854
  %858 = load i8, ptr %857, align 1, !tbaa !12
  %859 = zext i8 %858 to i64
  %860 = shl nuw nsw i64 %853, 3
  %861 = shl nuw i64 %859, %860
  %862 = or i64 %861, %855
  %863 = add nuw nsw i64 %853, 1
  %864 = icmp eq i64 %863, %800
  br i1 %864, label %888, label %852, !llvm.loop !40

865:                                              ; preds = %865, %779
  %866 = phi i64 [ %780, %779 ], [ %870, %865 ]
  %867 = phi i64 [ 0, %779 ], [ %886, %865 ]
  %868 = phi i64 [ 0, %779 ], [ %885, %865 ]
  %869 = phi i32 [ %602, %779 ], [ %879, %865 ]
  %870 = add nsw i64 %866, 1
  %871 = trunc nsw i64 %870 to i32
  store i32 %871, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %872 = getelementptr inbounds i8, ptr %578, i64 %866
  %873 = load i8, ptr %872, align 1, !tbaa !12
  %874 = shl i32 %869, 13
  %875 = xor i32 %874, %869
  %876 = lshr i32 %875, 17
  %877 = xor i32 %876, %875
  %878 = shl i32 %877, 5
  %879 = xor i32 %878, %877
  store i32 %879, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %880 = trunc i32 %879 to i8
  %881 = xor i8 %873, %880
  %882 = zext i8 %881 to i64
  %883 = shl nuw nsw i64 %867, 3
  %884 = shl nuw i64 %882, %883
  %885 = or i64 %884, %868
  %886 = add nuw nsw i64 %867, 1
  %887 = icmp eq i64 %886, %781
  br i1 %887, label %890, label %865, !llvm.loop !33

888:                                              ; preds = %852
  %889 = phi i64 [ %862, %852 ]
  br label %894

890:                                              ; preds = %865
  %891 = phi i32 [ %871, %865 ]
  %892 = phi i32 [ %879, %865 ]
  %893 = phi i64 [ %885, %865 ]
  br label %894

894:                                              ; preds = %890, %888, %823, %792, %777
  %895 = phi i32 [ %793, %792 ], [ %602, %777 ], [ %793, %823 ], [ %793, %888 ], [ %892, %890 ]
  %896 = phi i32 [ %794, %792 ], [ %593, %777 ], [ %794, %823 ], [ %794, %888 ], [ %891, %890 ]
  %897 = phi i64 [ 0, %792 ], [ 0, %777 ], [ %825, %823 ], [ %889, %888 ], [ %893, %890 ]
  %898 = shl i32 %895, 13
  %899 = xor i32 %898, %895
  %900 = lshr i32 %899, 17
  %901 = xor i32 %900, %899
  %902 = shl i32 %901, 5
  %903 = xor i32 %902, %901
  %904 = add nsw i32 %896, 2
  store i32 %904, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %905 = shl i32 %903, 13
  %906 = xor i32 %905, %903
  %907 = lshr i32 %906, 17
  %908 = xor i32 %907, %906
  %909 = shl i32 %908, 5
  %910 = xor i32 %909, %908
  store i32 %910, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %911 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %912 = icmp sgt i32 %911, 0
  br i1 %912, label %913, label %966

913:                                              ; preds = %894
  %914 = sext i32 %904 to i64
  %915 = zext nneg i32 %911 to i64
  br label %921

916:                                              ; preds = %921
  %917 = phi i64 [ %943, %921 ]
  %918 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %919 = inttoptr i64 %918 to ptr
  %920 = zext nneg i32 %911 to i64
  br label %946

921:                                              ; preds = %921, %913
  %922 = phi i64 [ %914, %913 ], [ %926, %921 ]
  %923 = phi i64 [ 0, %913 ], [ %944, %921 ]
  %924 = phi i64 [ 0, %913 ], [ %943, %921 ]
  %925 = phi i32 [ %910, %913 ], [ %935, %921 ]
  %926 = add nsw i64 %922, 1
  %927 = trunc nsw i64 %926 to i32
  store i32 %927, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %928 = getelementptr inbounds i8, ptr %578, i64 %922
  %929 = load i8, ptr %928, align 1, !tbaa !12
  %930 = shl i32 %925, 13
  %931 = xor i32 %930, %925
  %932 = lshr i32 %931, 17
  %933 = xor i32 %932, %931
  %934 = shl i32 %933, 5
  %935 = xor i32 %934, %933
  store i32 %935, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %936 = trunc i32 %935 to i8
  %937 = xor i8 %929, %936
  %938 = icmp samesign ult i64 %923, 8
  %939 = zext i8 %937 to i64
  %940 = shl nuw nsw i64 %923, 3
  %941 = shl nuw i64 %939, %940
  %942 = select i1 %938, i64 %941, i64 0
  %943 = or i64 %942, %924
  %944 = add nuw nsw i64 %923, 1
  %945 = icmp eq i64 %944, %915
  br i1 %945, label %916, label %921, !llvm.loop !33

946:                                              ; preds = %959, %916
  %947 = phi i64 [ 0, %916 ], [ %961, %959 ]
  %948 = phi i64 [ %917, %916 ], [ %950, %959 ]
  %949 = phi i64 [ 0, %916 ], [ %960, %959 ]
  %950 = add i64 %948, 1
  %951 = icmp samesign ult i64 %947, 8
  br i1 %951, label %952, label %959

952:                                              ; preds = %946
  %953 = getelementptr inbounds nuw i8, ptr %919, i64 %948
  %954 = load i8, ptr %953, align 1, !tbaa !12
  %955 = zext i8 %954 to i64
  %956 = shl nuw nsw i64 %947, 3
  %957 = shl nuw i64 %955, %956
  %958 = or i64 %957, %949
  br label %959

959:                                              ; preds = %952, %946
  %960 = phi i64 [ %958, %952 ], [ %949, %946 ]
  %961 = add nuw nsw i64 %947, 1
  %962 = icmp eq i64 %961, %920
  br i1 %962, label %963, label %946, !llvm.loop !35

963:                                              ; preds = %959
  %964 = phi i64 [ %960, %959 ]
  %965 = inttoptr i64 %964 to ptr
  br label %966

966:                                              ; preds = %963, %894
  %967 = phi ptr [ %965, %963 ], [ null, %894 ]
  %968 = icmp eq i8 %583, %591
  br i1 %968, label %2370, label %969

969:                                              ; preds = %966
  br label %970

970:                                              ; preds = %969, %970
  %971 = phi i32 [ %977, %970 ], [ 0, %969 ]
  %972 = phi ptr [ %975, %970 ], [ %967, %969 ]
  %973 = phi i64 [ %976, %970 ], [ %897, %969 ]
  %974 = trunc i64 %973 to i8
  store i8 %974, ptr %972, align 1, !tbaa !12
  %975 = getelementptr inbounds nuw i8, ptr %972, i64 1
  %976 = lshr i64 %973, 8
  %977 = add nuw nsw i32 %971, 1
  %978 = icmp eq i32 %977, %604
  br i1 %978, label %2359, label %970, !llvm.loop !34

979:                                              ; preds = %239
  %980 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %981 = inttoptr i64 %980 to ptr
  %982 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %983 = add nsw i32 %982, 1
  store i32 %983, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %984 = sext i32 %982 to i64
  %985 = getelementptr inbounds i8, ptr %981, i64 %984
  %986 = load i8, ptr %985, align 1, !tbaa !12
  %987 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %988 = shl i32 %987, 13
  %989 = xor i32 %988, %987
  %990 = lshr i32 %989, 17
  %991 = xor i32 %990, %989
  %992 = shl i32 %991, 5
  %993 = xor i32 %992, %991
  store i32 %993, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %994 = trunc i32 %993 to i8
  %995 = xor i8 %986, %994
  %996 = add nsw i32 %982, 2
  store i32 %996, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %997 = sext i32 %983 to i64
  %998 = getelementptr inbounds i8, ptr %981, i64 %997
  %999 = load i8, ptr %998, align 1, !tbaa !12
  %1000 = shl i32 %993, 13
  %1001 = xor i32 %1000, %993
  %1002 = lshr i32 %1001, 17
  %1003 = xor i32 %1002, %1001
  %1004 = shl i32 %1003, 5
  %1005 = xor i32 %1004, %1003
  %1006 = trunc i32 %1005 to i8
  %1007 = xor i8 %999, %1006
  %1008 = add nsw i32 %982, 3
  store i32 %1008, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1009 = shl i32 %1005, 13
  %1010 = xor i32 %1009, %1005
  %1011 = lshr i32 %1010, 17
  %1012 = xor i32 %1011, %1010
  %1013 = shl i32 %1012, 5
  %1014 = xor i32 %1013, %1012
  store i32 %1014, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1015 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %1016 = icmp sgt i32 %1015, 0
  br i1 %1016, label %1017, label %1049

1017:                                             ; preds = %979
  %1018 = sext i32 %1008 to i64
  %1019 = zext nneg i32 %1015 to i64
  br label %1020

1020:                                             ; preds = %1020, %1017
  %1021 = phi i64 [ %1018, %1017 ], [ %1025, %1020 ]
  %1022 = phi i64 [ 0, %1017 ], [ %1043, %1020 ]
  %1023 = phi i64 [ 0, %1017 ], [ %1042, %1020 ]
  %1024 = phi i32 [ %1014, %1017 ], [ %1034, %1020 ]
  %1025 = add nsw i64 %1021, 1
  %1026 = trunc nsw i64 %1025 to i32
  store i32 %1026, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1027 = getelementptr inbounds i8, ptr %981, i64 %1021
  %1028 = load i8, ptr %1027, align 1, !tbaa !12
  %1029 = shl i32 %1024, 13
  %1030 = xor i32 %1029, %1024
  %1031 = lshr i32 %1030, 17
  %1032 = xor i32 %1031, %1030
  %1033 = shl i32 %1032, 5
  %1034 = xor i32 %1033, %1032
  store i32 %1034, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1035 = trunc i32 %1034 to i8
  %1036 = xor i8 %1028, %1035
  %1037 = icmp samesign ult i64 %1022, 8
  %1038 = zext i8 %1036 to i64
  %1039 = shl nuw nsw i64 %1022, 3
  %1040 = shl nuw i64 %1038, %1039
  %1041 = select i1 %1037, i64 %1040, i64 0
  %1042 = or i64 %1041, %1023
  %1043 = add nuw nsw i64 %1022, 1
  %1044 = icmp eq i64 %1043, %1019
  br i1 %1044, label %1045, label %1020, !llvm.loop !33

1045:                                             ; preds = %1020
  %1046 = phi i32 [ %1026, %1020 ]
  %1047 = phi i32 [ %1034, %1020 ]
  %1048 = phi i64 [ %1042, %1020 ]
  br label %1049

1049:                                             ; preds = %1045, %979
  %1050 = phi i32 [ %1014, %979 ], [ %1047, %1045 ]
  %1051 = phi i32 [ %1008, %979 ], [ %1046, %1045 ]
  %1052 = phi i64 [ 0, %979 ], [ %1048, %1045 ]
  %1053 = icmp eq i8 %995, 31
  %1054 = add nsw i32 %1051, 1
  store i32 %1054, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1055 = sext i32 %1051 to i64
  %1056 = getelementptr inbounds i8, ptr %981, i64 %1055
  %1057 = load i8, ptr %1056, align 1, !tbaa !12
  %1058 = shl i32 %1050, 13
  %1059 = xor i32 %1058, %1050
  %1060 = lshr i32 %1059, 17
  %1061 = xor i32 %1060, %1059
  %1062 = shl i32 %1061, 5
  %1063 = xor i32 %1062, %1061
  store i32 %1063, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1064 = trunc i32 %1063 to i8
  %1065 = xor i8 %1057, %1064
  %1066 = add nsw i32 %1051, 2
  store i32 %1066, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1067 = sext i32 %1054 to i64
  %1068 = getelementptr inbounds i8, ptr %981, i64 %1067
  %1069 = load i8, ptr %1068, align 1, !tbaa !12
  %1070 = shl i32 %1063, 13
  %1071 = xor i32 %1070, %1063
  %1072 = lshr i32 %1071, 17
  %1073 = xor i32 %1072, %1071
  %1074 = shl i32 %1073, 5
  %1075 = xor i32 %1074, %1073
  store i32 %1075, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1076 = trunc i32 %1075 to i8
  %1077 = icmp eq i8 %1069, %1076
  br i1 %1053, label %1078, label %1181

1078:                                             ; preds = %1049
  br i1 %1077, label %1079, label %1134

1079:                                             ; preds = %1078
  br i1 %1016, label %1080, label %1110

1080:                                             ; preds = %1079
  %1081 = sext i32 %1066 to i64
  %1082 = zext nneg i32 %1015 to i64
  br label %1083

1083:                                             ; preds = %1083, %1080
  %1084 = phi i64 [ %1081, %1080 ], [ %1088, %1083 ]
  %1085 = phi i64 [ 0, %1080 ], [ %1106, %1083 ]
  %1086 = phi i64 [ 0, %1080 ], [ %1105, %1083 ]
  %1087 = phi i32 [ %1075, %1080 ], [ %1097, %1083 ]
  %1088 = add nsw i64 %1084, 1
  %1089 = trunc nsw i64 %1088 to i32
  store i32 %1089, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1090 = getelementptr inbounds i8, ptr %981, i64 %1084
  %1091 = load i8, ptr %1090, align 1, !tbaa !12
  %1092 = shl i32 %1087, 13
  %1093 = xor i32 %1092, %1087
  %1094 = lshr i32 %1093, 17
  %1095 = xor i32 %1094, %1093
  %1096 = shl i32 %1095, 5
  %1097 = xor i32 %1096, %1095
  store i32 %1097, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1098 = trunc i32 %1097 to i8
  %1099 = xor i8 %1091, %1098
  %1100 = icmp samesign ult i64 %1085, 8
  %1101 = zext i8 %1099 to i64
  %1102 = shl nuw nsw i64 %1085, 3
  %1103 = shl nuw i64 %1101, %1102
  %1104 = select i1 %1100, i64 %1103, i64 0
  %1105 = or i64 %1104, %1086
  %1106 = add nuw nsw i64 %1085, 1
  %1107 = icmp eq i64 %1106, %1082
  br i1 %1107, label %1108, label %1083, !llvm.loop !33

1108:                                             ; preds = %1083
  %1109 = phi i64 [ %1105, %1083 ]
  br label %1110

1110:                                             ; preds = %1108, %1079
  %1111 = phi i64 [ 0, %1079 ], [ %1109, %1108 ]
  %1112 = icmp eq i8 %1057, %1064
  br i1 %1112, label %1168, label %1113

1113:                                             ; preds = %1110
  %1114 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %1115 = inttoptr i64 %1114 to ptr
  %1116 = zext i8 %1065 to i64
  br label %1117

1117:                                             ; preds = %1130, %1113
  %1118 = phi i64 [ 0, %1113 ], [ %1132, %1130 ]
  %1119 = phi i64 [ %1111, %1113 ], [ %1121, %1130 ]
  %1120 = phi i64 [ 0, %1113 ], [ %1131, %1130 ]
  %1121 = add i64 %1119, 1
  %1122 = icmp samesign ult i64 %1118, 8
  br i1 %1122, label %1123, label %1130

1123:                                             ; preds = %1117
  %1124 = getelementptr inbounds nuw i8, ptr %1115, i64 %1119
  %1125 = load i8, ptr %1124, align 1, !tbaa !12
  %1126 = zext i8 %1125 to i64
  %1127 = shl nuw nsw i64 %1118, 3
  %1128 = shl nuw i64 %1126, %1127
  %1129 = or i64 %1128, %1120
  br label %1130

1130:                                             ; preds = %1123, %1117
  %1131 = phi i64 [ %1129, %1123 ], [ %1120, %1117 ]
  %1132 = add nuw nsw i64 %1118, 1
  %1133 = icmp eq i64 %1132, %1116
  br i1 %1133, label %1164, label %1117, !llvm.loop !35

1134:                                             ; preds = %1078
  %1135 = icmp eq i8 %1057, %1064
  br i1 %1135, label %1168, label %1136

1136:                                             ; preds = %1134
  %1137 = sext i32 %1066 to i64
  %1138 = zext i8 %1065 to i64
  br label %1139

1139:                                             ; preds = %1139, %1136
  %1140 = phi i64 [ %1137, %1136 ], [ %1144, %1139 ]
  %1141 = phi i64 [ 0, %1136 ], [ %1162, %1139 ]
  %1142 = phi i64 [ 0, %1136 ], [ %1161, %1139 ]
  %1143 = phi i32 [ %1075, %1136 ], [ %1153, %1139 ]
  %1144 = add nsw i64 %1140, 1
  %1145 = trunc nsw i64 %1144 to i32
  store i32 %1145, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1146 = getelementptr inbounds i8, ptr %981, i64 %1140
  %1147 = load i8, ptr %1146, align 1, !tbaa !12
  %1148 = shl i32 %1143, 13
  %1149 = xor i32 %1148, %1143
  %1150 = lshr i32 %1149, 17
  %1151 = xor i32 %1150, %1149
  %1152 = shl i32 %1151, 5
  %1153 = xor i32 %1152, %1151
  store i32 %1153, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1154 = trunc i32 %1153 to i8
  %1155 = xor i8 %1147, %1154
  %1156 = icmp samesign ult i64 %1141, 8
  %1157 = zext i8 %1155 to i64
  %1158 = shl nuw nsw i64 %1141, 3
  %1159 = shl nuw i64 %1157, %1158
  %1160 = select i1 %1156, i64 %1159, i64 0
  %1161 = or i64 %1160, %1142
  %1162 = add nuw nsw i64 %1141, 1
  %1163 = icmp eq i64 %1162, %1138
  br i1 %1163, label %1166, label %1139, !llvm.loop !33

1164:                                             ; preds = %1130
  %1165 = phi i64 [ %1131, %1130 ]
  br label %1168

1166:                                             ; preds = %1139
  %1167 = phi i64 [ %1161, %1139 ]
  br label %1168

1168:                                             ; preds = %1166, %1164, %1134, %1110
  %1169 = phi i64 [ 0, %1110 ], [ 0, %1134 ], [ %1165, %1164 ], [ %1167, %1166 ]
  %1170 = icmp ult i8 %1065, 5
  br i1 %1170, label %1171, label %1177

1171:                                             ; preds = %1168
  %1172 = trunc i64 %1169 to i32
  %1173 = bitcast i32 %1172 to float
  %1174 = fneg float %1173
  %1175 = bitcast float %1174 to i32
  %1176 = zext i32 %1175 to i64
  br label %1526

1177:                                             ; preds = %1168
  %1178 = bitcast i64 %1169 to double
  %1179 = fneg double %1178
  %1180 = bitcast double %1179 to i64
  br label %1526

1181:                                             ; preds = %1049
  br i1 %1077, label %1182, label %1241

1182:                                             ; preds = %1181
  br i1 %1016, label %1183, label %1215

1183:                                             ; preds = %1182
  %1184 = sext i32 %1066 to i64
  %1185 = zext nneg i32 %1015 to i64
  br label %1186

1186:                                             ; preds = %1186, %1183
  %1187 = phi i64 [ %1184, %1183 ], [ %1191, %1186 ]
  %1188 = phi i64 [ 0, %1183 ], [ %1209, %1186 ]
  %1189 = phi i64 [ 0, %1183 ], [ %1208, %1186 ]
  %1190 = phi i32 [ %1075, %1183 ], [ %1200, %1186 ]
  %1191 = add nsw i64 %1187, 1
  %1192 = trunc nsw i64 %1191 to i32
  store i32 %1192, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1193 = getelementptr inbounds i8, ptr %981, i64 %1187
  %1194 = load i8, ptr %1193, align 1, !tbaa !12
  %1195 = shl i32 %1190, 13
  %1196 = xor i32 %1195, %1190
  %1197 = lshr i32 %1196, 17
  %1198 = xor i32 %1197, %1196
  %1199 = shl i32 %1198, 5
  %1200 = xor i32 %1199, %1198
  store i32 %1200, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1201 = trunc i32 %1200 to i8
  %1202 = xor i8 %1194, %1201
  %1203 = icmp samesign ult i64 %1188, 8
  %1204 = zext i8 %1202 to i64
  %1205 = shl nuw nsw i64 %1188, 3
  %1206 = shl nuw i64 %1204, %1205
  %1207 = select i1 %1203, i64 %1206, i64 0
  %1208 = or i64 %1207, %1189
  %1209 = add nuw nsw i64 %1188, 1
  %1210 = icmp eq i64 %1209, %1185
  br i1 %1210, label %1211, label %1186, !llvm.loop !33

1211:                                             ; preds = %1186
  %1212 = phi i32 [ %1192, %1186 ]
  %1213 = phi i32 [ %1200, %1186 ]
  %1214 = phi i64 [ %1208, %1186 ]
  br label %1215

1215:                                             ; preds = %1211, %1182
  %1216 = phi i32 [ %1075, %1182 ], [ %1213, %1211 ]
  %1217 = phi i32 [ %1066, %1182 ], [ %1212, %1211 ]
  %1218 = phi i64 [ 0, %1182 ], [ %1214, %1211 ]
  %1219 = icmp eq i8 %1057, %1064
  br i1 %1219, label %1277, label %1220

1220:                                             ; preds = %1215
  %1221 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %1222 = inttoptr i64 %1221 to ptr
  %1223 = zext i8 %1065 to i64
  br label %1224

1224:                                             ; preds = %1237, %1220
  %1225 = phi i64 [ 0, %1220 ], [ %1239, %1237 ]
  %1226 = phi i64 [ %1218, %1220 ], [ %1228, %1237 ]
  %1227 = phi i64 [ 0, %1220 ], [ %1238, %1237 ]
  %1228 = add i64 %1226, 1
  %1229 = icmp samesign ult i64 %1225, 8
  br i1 %1229, label %1230, label %1237

1230:                                             ; preds = %1224
  %1231 = getelementptr inbounds nuw i8, ptr %1222, i64 %1226
  %1232 = load i8, ptr %1231, align 1, !tbaa !12
  %1233 = zext i8 %1232 to i64
  %1234 = shl nuw nsw i64 %1225, 3
  %1235 = shl nuw i64 %1233, %1234
  %1236 = or i64 %1235, %1227
  br label %1237

1237:                                             ; preds = %1230, %1224
  %1238 = phi i64 [ %1236, %1230 ], [ %1227, %1224 ]
  %1239 = add nuw nsw i64 %1225, 1
  %1240 = icmp eq i64 %1239, %1223
  br i1 %1240, label %1271, label %1224, !llvm.loop !35

1241:                                             ; preds = %1181
  %1242 = icmp eq i8 %1057, %1064
  br i1 %1242, label %1277, label %1243

1243:                                             ; preds = %1241
  %1244 = sext i32 %1066 to i64
  %1245 = zext i8 %1065 to i64
  br label %1246

1246:                                             ; preds = %1246, %1243
  %1247 = phi i64 [ %1244, %1243 ], [ %1251, %1246 ]
  %1248 = phi i64 [ 0, %1243 ], [ %1269, %1246 ]
  %1249 = phi i64 [ 0, %1243 ], [ %1268, %1246 ]
  %1250 = phi i32 [ %1075, %1243 ], [ %1260, %1246 ]
  %1251 = add nsw i64 %1247, 1
  %1252 = trunc nsw i64 %1251 to i32
  store i32 %1252, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1253 = getelementptr inbounds i8, ptr %981, i64 %1247
  %1254 = load i8, ptr %1253, align 1, !tbaa !12
  %1255 = shl i32 %1250, 13
  %1256 = xor i32 %1255, %1250
  %1257 = lshr i32 %1256, 17
  %1258 = xor i32 %1257, %1256
  %1259 = shl i32 %1258, 5
  %1260 = xor i32 %1259, %1258
  store i32 %1260, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1261 = trunc i32 %1260 to i8
  %1262 = xor i8 %1254, %1261
  %1263 = icmp samesign ult i64 %1248, 8
  %1264 = zext i8 %1262 to i64
  %1265 = shl nuw nsw i64 %1248, 3
  %1266 = shl nuw i64 %1264, %1265
  %1267 = select i1 %1263, i64 %1266, i64 0
  %1268 = or i64 %1267, %1249
  %1269 = add nuw nsw i64 %1248, 1
  %1270 = icmp eq i64 %1269, %1245
  br i1 %1270, label %1273, label %1246, !llvm.loop !33

1271:                                             ; preds = %1237
  %1272 = phi i64 [ %1238, %1237 ]
  br label %1277

1273:                                             ; preds = %1246
  %1274 = phi i32 [ %1252, %1246 ]
  %1275 = phi i32 [ %1260, %1246 ]
  %1276 = phi i64 [ %1268, %1246 ]
  br label %1277

1277:                                             ; preds = %1273, %1271, %1241, %1215
  %1278 = phi i32 [ %1216, %1215 ], [ %1075, %1241 ], [ %1216, %1271 ], [ %1275, %1273 ]
  %1279 = phi i32 [ %1217, %1215 ], [ %1066, %1241 ], [ %1217, %1271 ], [ %1274, %1273 ]
  %1280 = phi i64 [ 0, %1215 ], [ 0, %1241 ], [ %1272, %1271 ], [ %1276, %1273 ]
  %1281 = add nsw i32 %1279, 1
  store i32 %1281, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1282 = sext i32 %1279 to i64
  %1283 = getelementptr inbounds i8, ptr %981, i64 %1282
  %1284 = load i8, ptr %1283, align 1, !tbaa !12
  %1285 = shl i32 %1278, 13
  %1286 = xor i32 %1285, %1278
  %1287 = lshr i32 %1286, 17
  %1288 = xor i32 %1287, %1286
  %1289 = shl i32 %1288, 5
  %1290 = xor i32 %1289, %1288
  store i32 %1290, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1291 = trunc i32 %1290 to i8
  %1292 = xor i8 %1284, %1291
  %1293 = add nsw i32 %1279, 2
  store i32 %1293, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1294 = sext i32 %1281 to i64
  %1295 = getelementptr inbounds i8, ptr %981, i64 %1294
  %1296 = load i8, ptr %1295, align 1, !tbaa !12
  %1297 = shl i32 %1290, 13
  %1298 = xor i32 %1297, %1290
  %1299 = lshr i32 %1298, 17
  %1300 = xor i32 %1299, %1298
  %1301 = shl i32 %1300, 5
  %1302 = xor i32 %1301, %1300
  store i32 %1302, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1303 = trunc i32 %1302 to i8
  %1304 = icmp eq i8 %1296, %1303
  br i1 %1304, label %1305, label %1360

1305:                                             ; preds = %1277
  br i1 %1016, label %1306, label %1336

1306:                                             ; preds = %1305
  %1307 = sext i32 %1293 to i64
  %1308 = zext nneg i32 %1015 to i64
  br label %1309

1309:                                             ; preds = %1309, %1306
  %1310 = phi i64 [ %1307, %1306 ], [ %1314, %1309 ]
  %1311 = phi i64 [ 0, %1306 ], [ %1332, %1309 ]
  %1312 = phi i64 [ 0, %1306 ], [ %1331, %1309 ]
  %1313 = phi i32 [ %1302, %1306 ], [ %1323, %1309 ]
  %1314 = add nsw i64 %1310, 1
  %1315 = trunc nsw i64 %1314 to i32
  store i32 %1315, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1316 = getelementptr inbounds i8, ptr %981, i64 %1310
  %1317 = load i8, ptr %1316, align 1, !tbaa !12
  %1318 = shl i32 %1313, 13
  %1319 = xor i32 %1318, %1313
  %1320 = lshr i32 %1319, 17
  %1321 = xor i32 %1320, %1319
  %1322 = shl i32 %1321, 5
  %1323 = xor i32 %1322, %1321
  store i32 %1323, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1324 = trunc i32 %1323 to i8
  %1325 = xor i8 %1317, %1324
  %1326 = icmp samesign ult i64 %1311, 8
  %1327 = zext i8 %1325 to i64
  %1328 = shl nuw nsw i64 %1311, 3
  %1329 = shl nuw i64 %1327, %1328
  %1330 = select i1 %1326, i64 %1329, i64 0
  %1331 = or i64 %1330, %1312
  %1332 = add nuw nsw i64 %1311, 1
  %1333 = icmp eq i64 %1332, %1308
  br i1 %1333, label %1334, label %1309, !llvm.loop !33

1334:                                             ; preds = %1309
  %1335 = phi i64 [ %1331, %1309 ]
  br label %1336

1336:                                             ; preds = %1334, %1305
  %1337 = phi i64 [ 0, %1305 ], [ %1335, %1334 ]
  %1338 = icmp eq i8 %1284, %1291
  br i1 %1338, label %1394, label %1339

1339:                                             ; preds = %1336
  %1340 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %1341 = inttoptr i64 %1340 to ptr
  %1342 = zext i8 %1292 to i64
  br label %1343

1343:                                             ; preds = %1356, %1339
  %1344 = phi i64 [ 0, %1339 ], [ %1358, %1356 ]
  %1345 = phi i64 [ %1337, %1339 ], [ %1347, %1356 ]
  %1346 = phi i64 [ 0, %1339 ], [ %1357, %1356 ]
  %1347 = add i64 %1345, 1
  %1348 = icmp samesign ult i64 %1344, 8
  br i1 %1348, label %1349, label %1356

1349:                                             ; preds = %1343
  %1350 = getelementptr inbounds nuw i8, ptr %1341, i64 %1345
  %1351 = load i8, ptr %1350, align 1, !tbaa !12
  %1352 = zext i8 %1351 to i64
  %1353 = shl nuw nsw i64 %1344, 3
  %1354 = shl nuw i64 %1352, %1353
  %1355 = or i64 %1354, %1346
  br label %1356

1356:                                             ; preds = %1349, %1343
  %1357 = phi i64 [ %1355, %1349 ], [ %1346, %1343 ]
  %1358 = add nuw nsw i64 %1344, 1
  %1359 = icmp eq i64 %1358, %1342
  br i1 %1359, label %1390, label %1343, !llvm.loop !35

1360:                                             ; preds = %1277
  %1361 = icmp eq i8 %1284, %1291
  br i1 %1361, label %1394, label %1362

1362:                                             ; preds = %1360
  %1363 = sext i32 %1293 to i64
  %1364 = zext i8 %1292 to i64
  br label %1365

1365:                                             ; preds = %1365, %1362
  %1366 = phi i64 [ %1363, %1362 ], [ %1370, %1365 ]
  %1367 = phi i64 [ 0, %1362 ], [ %1388, %1365 ]
  %1368 = phi i64 [ 0, %1362 ], [ %1387, %1365 ]
  %1369 = phi i32 [ %1302, %1362 ], [ %1379, %1365 ]
  %1370 = add nsw i64 %1366, 1
  %1371 = trunc nsw i64 %1370 to i32
  store i32 %1371, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1372 = getelementptr inbounds i8, ptr %981, i64 %1366
  %1373 = load i8, ptr %1372, align 1, !tbaa !12
  %1374 = shl i32 %1369, 13
  %1375 = xor i32 %1374, %1369
  %1376 = lshr i32 %1375, 17
  %1377 = xor i32 %1376, %1375
  %1378 = shl i32 %1377, 5
  %1379 = xor i32 %1378, %1377
  store i32 %1379, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1380 = trunc i32 %1379 to i8
  %1381 = xor i8 %1373, %1380
  %1382 = icmp samesign ult i64 %1367, 8
  %1383 = zext i8 %1381 to i64
  %1384 = shl nuw nsw i64 %1367, 3
  %1385 = shl nuw i64 %1383, %1384
  %1386 = select i1 %1382, i64 %1385, i64 0
  %1387 = or i64 %1386, %1368
  %1388 = add nuw nsw i64 %1367, 1
  %1389 = icmp eq i64 %1388, %1364
  br i1 %1389, label %1392, label %1365, !llvm.loop !33

1390:                                             ; preds = %1356
  %1391 = phi i64 [ %1357, %1356 ]
  br label %1394

1392:                                             ; preds = %1365
  %1393 = phi i64 [ %1387, %1365 ]
  br label %1394

1394:                                             ; preds = %1392, %1390, %1360, %1336
  %1395 = phi i64 [ 0, %1336 ], [ 0, %1360 ], [ %1391, %1390 ], [ %1393, %1392 ]
  switch i8 %995, label %1526 [
    i8 13, label %1396
    i8 14, label %1398
    i8 15, label %1413
    i8 16, label %1415
    i8 17, label %1430
    i8 18, label %1432
    i8 19, label %1447
    i8 20, label %1449
    i8 21, label %1458
    i8 22, label %1473
    i8 23, label %1475
    i8 24, label %1484
    i8 25, label %1507
    i8 26, label %1509
    i8 27, label %1511
    i8 28, label %1520
    i8 29, label %1522
    i8 30, label %1524
  ]

1396:                                             ; preds = %1394
  %1397 = add i64 %1395, %1280
  br label %1526

1398:                                             ; preds = %1394
  %1399 = icmp ult i8 %1065, 5
  br i1 %1399, label %1400, label %1408

1400:                                             ; preds = %1398
  %1401 = trunc i64 %1280 to i32
  %1402 = trunc i64 %1395 to i32
  %1403 = bitcast i32 %1401 to float
  %1404 = bitcast i32 %1402 to float
  %1405 = fadd float %1403, %1404
  %1406 = bitcast float %1405 to i32
  %1407 = zext i32 %1406 to i64
  br label %1526

1408:                                             ; preds = %1398
  %1409 = bitcast i64 %1280 to double
  %1410 = bitcast i64 %1395 to double
  %1411 = fadd double %1409, %1410
  %1412 = bitcast double %1411 to i64
  br label %1526

1413:                                             ; preds = %1394
  %1414 = sub i64 %1280, %1395
  br label %1526

1415:                                             ; preds = %1394
  %1416 = icmp ult i8 %1065, 5
  br i1 %1416, label %1417, label %1425

1417:                                             ; preds = %1415
  %1418 = trunc i64 %1280 to i32
  %1419 = trunc i64 %1395 to i32
  %1420 = bitcast i32 %1418 to float
  %1421 = bitcast i32 %1419 to float
  %1422 = fsub float %1420, %1421
  %1423 = bitcast float %1422 to i32
  %1424 = zext i32 %1423 to i64
  br label %1526

1425:                                             ; preds = %1415
  %1426 = bitcast i64 %1280 to double
  %1427 = bitcast i64 %1395 to double
  %1428 = fsub double %1426, %1427
  %1429 = bitcast double %1428 to i64
  br label %1526

1430:                                             ; preds = %1394
  %1431 = mul i64 %1395, %1280
  br label %1526

1432:                                             ; preds = %1394
  %1433 = icmp ult i8 %1065, 5
  br i1 %1433, label %1434, label %1442

1434:                                             ; preds = %1432
  %1435 = trunc i64 %1280 to i32
  %1436 = trunc i64 %1395 to i32
  %1437 = bitcast i32 %1435 to float
  %1438 = bitcast i32 %1436 to float
  %1439 = fmul float %1437, %1438
  %1440 = bitcast float %1439 to i32
  %1441 = zext i32 %1440 to i64
  br label %1526

1442:                                             ; preds = %1432
  %1443 = bitcast i64 %1280 to double
  %1444 = bitcast i64 %1395 to double
  %1445 = fmul double %1443, %1444
  %1446 = bitcast double %1445 to i64
  br label %1526

1447:                                             ; preds = %1394
  %1448 = udiv i64 %1280, %1395
  br label %1526

1449:                                             ; preds = %1394
  %1450 = icmp ult i8 %1065, 5
  br i1 %1450, label %1451, label %1456

1451:                                             ; preds = %1449
  %1452 = trunc i64 %1280 to i32
  %1453 = trunc i64 %1395 to i32
  %1454 = sdiv i32 %1452, %1453
  %1455 = sext i32 %1454 to i64
  br label %1526

1456:                                             ; preds = %1449
  %1457 = sdiv i64 %1280, %1395
  br label %1526

1458:                                             ; preds = %1394
  %1459 = icmp ult i8 %1065, 5
  br i1 %1459, label %1460, label %1468

1460:                                             ; preds = %1458
  %1461 = trunc i64 %1280 to i32
  %1462 = trunc i64 %1395 to i32
  %1463 = bitcast i32 %1461 to float
  %1464 = bitcast i32 %1462 to float
  %1465 = fdiv float %1463, %1464
  %1466 = bitcast float %1465 to i32
  %1467 = zext i32 %1466 to i64
  br label %1526

1468:                                             ; preds = %1458
  %1469 = bitcast i64 %1280 to double
  %1470 = bitcast i64 %1395 to double
  %1471 = fdiv double %1469, %1470
  %1472 = bitcast double %1471 to i64
  br label %1526

1473:                                             ; preds = %1394
  %1474 = urem i64 %1280, %1395
  br label %1526

1475:                                             ; preds = %1394
  %1476 = icmp ult i8 %1065, 5
  br i1 %1476, label %1477, label %1482

1477:                                             ; preds = %1475
  %1478 = trunc i64 %1280 to i32
  %1479 = trunc i64 %1395 to i32
  %1480 = srem i32 %1478, %1479
  %1481 = sext i32 %1480 to i64
  br label %1526

1482:                                             ; preds = %1475
  %1483 = srem i64 %1280, %1395
  br label %1526

1484:                                             ; preds = %1394
  %1485 = icmp ult i8 %1065, 5
  br i1 %1485, label %1486, label %1498

1486:                                             ; preds = %1484
  %1487 = trunc i64 %1280 to i32
  %1488 = trunc i64 %1395 to i32
  %1489 = bitcast i32 %1487 to float
  %1490 = bitcast i32 %1488 to float
  %1491 = fdiv float %1489, %1490
  %1492 = fptosi float %1491 to i64
  %1493 = sitofp i64 %1492 to float
  %1494 = fneg float %1490
  %1495 = tail call float @llvm.fmuladd.f32(float %1494, float %1493, float %1489)
  %1496 = bitcast float %1495 to i32
  %1497 = zext i32 %1496 to i64
  br label %1526

1498:                                             ; preds = %1484
  %1499 = bitcast i64 %1280 to double
  %1500 = bitcast i64 %1395 to double
  %1501 = fdiv double %1499, %1500
  %1502 = fptosi double %1501 to i64
  %1503 = sitofp i64 %1502 to double
  %1504 = fneg double %1500
  %1505 = tail call double @llvm.fmuladd.f64(double %1504, double %1503, double %1499)
  %1506 = bitcast double %1505 to i64
  br label %1526

1507:                                             ; preds = %1394
  %1508 = shl i64 %1280, %1395
  br label %1526

1509:                                             ; preds = %1394
  %1510 = lshr i64 %1280, %1395
  br label %1526

1511:                                             ; preds = %1394
  %1512 = icmp ult i8 %1065, 5
  br i1 %1512, label %1513, label %1518

1513:                                             ; preds = %1511
  %1514 = trunc i64 %1280 to i32
  %1515 = trunc i64 %1395 to i32
  %1516 = ashr i32 %1514, %1515
  %1517 = sext i32 %1516 to i64
  br label %1526

1518:                                             ; preds = %1511
  %1519 = ashr i64 %1280, %1395
  br label %1526

1520:                                             ; preds = %1394
  %1521 = and i64 %1395, %1280
  br label %1526

1522:                                             ; preds = %1394
  %1523 = or i64 %1395, %1280
  br label %1526

1524:                                             ; preds = %1394
  %1525 = xor i64 %1395, %1280
  br label %1526

1526:                                             ; preds = %1524, %1522, %1520, %1518, %1513, %1509, %1507, %1498, %1486, %1482, %1477, %1473, %1468, %1460, %1456, %1451, %1447, %1442, %1434, %1430, %1425, %1417, %1413, %1408, %1400, %1396, %1394, %1177, %1171
  %1527 = phi i64 [ %1176, %1171 ], [ %1180, %1177 ], [ 0, %1394 ], [ %1397, %1396 ], [ %1407, %1400 ], [ %1412, %1408 ], [ %1414, %1413 ], [ %1424, %1417 ], [ %1429, %1425 ], [ %1431, %1430 ], [ %1441, %1434 ], [ %1446, %1442 ], [ %1448, %1447 ], [ %1455, %1451 ], [ %1457, %1456 ], [ %1467, %1460 ], [ %1472, %1468 ], [ %1474, %1473 ], [ %1481, %1477 ], [ %1483, %1482 ], [ %1497, %1486 ], [ %1506, %1498 ], [ %1508, %1507 ], [ %1510, %1509 ], [ %1517, %1513 ], [ %1519, %1518 ], [ %1521, %1520 ], [ %1523, %1522 ], [ %1525, %1524 ]
  %1528 = icmp eq i8 %999, %1006
  br i1 %1528, label %2370, label %1529

1529:                                             ; preds = %1526
  %1530 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %1531 = add i64 %1530, %1052
  %1532 = inttoptr i64 %1531 to ptr
  %1533 = zext i8 %1007 to i32
  br label %1534

1534:                                             ; preds = %1534, %1529
  %1535 = phi i32 [ %1541, %1534 ], [ 0, %1529 ]
  %1536 = phi ptr [ %1539, %1534 ], [ %1532, %1529 ]
  %1537 = phi i64 [ %1540, %1534 ], [ %1527, %1529 ]
  %1538 = trunc i64 %1537 to i8
  store i8 %1538, ptr %1536, align 1, !tbaa !12
  %1539 = getelementptr inbounds nuw i8, ptr %1536, i64 1
  %1540 = lshr i64 %1537, 8
  %1541 = add nuw nsw i32 %1535, 1
  %1542 = icmp eq i32 %1541, %1533
  br i1 %1542, label %2360, label %1534, !llvm.loop !34

1543:                                             ; preds = %239
  %1544 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %1545 = inttoptr i64 %1544 to ptr
  %1546 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1547 = add nsw i32 %1546, 1
  store i32 %1547, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1548 = sext i32 %1546 to i64
  %1549 = getelementptr inbounds i8, ptr %1545, i64 %1548
  %1550 = load i8, ptr %1549, align 1, !tbaa !12
  %1551 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1552 = shl i32 %1551, 13
  %1553 = xor i32 %1552, %1551
  %1554 = lshr i32 %1553, 17
  %1555 = xor i32 %1554, %1553
  %1556 = shl i32 %1555, 5
  %1557 = xor i32 %1556, %1555
  store i32 %1557, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1558 = trunc i32 %1557 to i8
  %1559 = xor i8 %1550, %1558
  %1560 = add nsw i32 %1546, 2
  store i32 %1560, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1561 = sext i32 %1547 to i64
  %1562 = getelementptr inbounds i8, ptr %1545, i64 %1561
  %1563 = load i8, ptr %1562, align 1, !tbaa !12
  %1564 = shl i32 %1557, 13
  %1565 = xor i32 %1564, %1557
  %1566 = lshr i32 %1565, 17
  %1567 = xor i32 %1566, %1565
  %1568 = shl i32 %1567, 5
  %1569 = xor i32 %1568, %1567
  store i32 %1569, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1570 = trunc i32 %1569 to i8
  %1571 = add nsw i32 %1546, 3
  store i32 %1571, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1572 = sext i32 %1560 to i64
  %1573 = getelementptr inbounds i8, ptr %1545, i64 %1572
  %1574 = load i8, ptr %1573, align 1, !tbaa !12
  %1575 = shl i32 %1569, 13
  %1576 = xor i32 %1575, %1569
  %1577 = lshr i32 %1576, 17
  %1578 = xor i32 %1577, %1576
  %1579 = shl i32 %1578, 5
  %1580 = xor i32 %1579, %1578
  %1581 = trunc i32 %1580 to i8
  %1582 = xor i8 %1574, %1581
  %1583 = add nsw i32 %1546, 4
  %1584 = shl i32 %1580, 13
  %1585 = xor i32 %1584, %1580
  %1586 = lshr i32 %1585, 17
  %1587 = xor i32 %1586, %1585
  %1588 = shl i32 %1587, 5
  %1589 = xor i32 %1588, %1587
  store i32 %1589, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1590 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %1591 = icmp sgt i32 %1590, 0
  br i1 %1591, label %1592, label %1624

1592:                                             ; preds = %1543
  %1593 = sext i32 %1583 to i64
  %1594 = zext nneg i32 %1590 to i64
  br label %1595

1595:                                             ; preds = %1595, %1592
  %1596 = phi i64 [ %1593, %1592 ], [ %1600, %1595 ]
  %1597 = phi i64 [ 0, %1592 ], [ %1618, %1595 ]
  %1598 = phi i64 [ 0, %1592 ], [ %1617, %1595 ]
  %1599 = phi i32 [ %1589, %1592 ], [ %1609, %1595 ]
  %1600 = add nsw i64 %1596, 1
  %1601 = trunc nsw i64 %1600 to i32
  store i32 %1601, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1602 = getelementptr inbounds i8, ptr %1545, i64 %1596
  %1603 = load i8, ptr %1602, align 1, !tbaa !12
  %1604 = shl i32 %1599, 13
  %1605 = xor i32 %1604, %1599
  %1606 = lshr i32 %1605, 17
  %1607 = xor i32 %1606, %1605
  %1608 = shl i32 %1607, 5
  %1609 = xor i32 %1608, %1607
  store i32 %1609, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1610 = trunc i32 %1609 to i8
  %1611 = xor i8 %1603, %1610
  %1612 = icmp samesign ult i64 %1597, 8
  %1613 = zext i8 %1611 to i64
  %1614 = shl nuw nsw i64 %1597, 3
  %1615 = shl nuw i64 %1613, %1614
  %1616 = select i1 %1612, i64 %1615, i64 0
  %1617 = or i64 %1616, %1598
  %1618 = add nuw nsw i64 %1597, 1
  %1619 = icmp eq i64 %1618, %1594
  br i1 %1619, label %1620, label %1595, !llvm.loop !33

1620:                                             ; preds = %1595
  %1621 = phi i32 [ %1601, %1595 ]
  %1622 = phi i32 [ %1609, %1595 ]
  %1623 = phi i64 [ %1617, %1595 ]
  br label %1624

1624:                                             ; preds = %1620, %1543
  %1625 = phi i32 [ %1589, %1543 ], [ %1622, %1620 ]
  %1626 = phi i32 [ %1583, %1543 ], [ %1621, %1620 ]
  %1627 = phi i64 [ 0, %1543 ], [ %1623, %1620 ]
  %1628 = add nsw i32 %1626, 1
  store i32 %1628, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1629 = sext i32 %1626 to i64
  %1630 = getelementptr inbounds i8, ptr %1545, i64 %1629
  %1631 = load i8, ptr %1630, align 1, !tbaa !12
  %1632 = shl i32 %1625, 13
  %1633 = xor i32 %1632, %1625
  %1634 = lshr i32 %1633, 17
  %1635 = xor i32 %1634, %1633
  %1636 = shl i32 %1635, 5
  %1637 = xor i32 %1636, %1635
  store i32 %1637, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1638 = trunc i32 %1637 to i8
  %1639 = xor i8 %1631, %1638
  %1640 = add nsw i32 %1626, 2
  store i32 %1640, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1641 = sext i32 %1628 to i64
  %1642 = getelementptr inbounds i8, ptr %1545, i64 %1641
  %1643 = load i8, ptr %1642, align 1, !tbaa !12
  %1644 = shl i32 %1637, 13
  %1645 = xor i32 %1644, %1637
  %1646 = lshr i32 %1645, 17
  %1647 = xor i32 %1646, %1645
  %1648 = shl i32 %1647, 5
  %1649 = xor i32 %1648, %1647
  store i32 %1649, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1650 = trunc i32 %1649 to i8
  %1651 = icmp eq i8 %1643, %1650
  br i1 %1651, label %1652, label %1711

1652:                                             ; preds = %1624
  br i1 %1591, label %1653, label %1685

1653:                                             ; preds = %1652
  %1654 = sext i32 %1640 to i64
  %1655 = zext nneg i32 %1590 to i64
  br label %1656

1656:                                             ; preds = %1656, %1653
  %1657 = phi i64 [ %1654, %1653 ], [ %1661, %1656 ]
  %1658 = phi i64 [ 0, %1653 ], [ %1679, %1656 ]
  %1659 = phi i64 [ 0, %1653 ], [ %1678, %1656 ]
  %1660 = phi i32 [ %1649, %1653 ], [ %1670, %1656 ]
  %1661 = add nsw i64 %1657, 1
  %1662 = trunc nsw i64 %1661 to i32
  store i32 %1662, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1663 = getelementptr inbounds i8, ptr %1545, i64 %1657
  %1664 = load i8, ptr %1663, align 1, !tbaa !12
  %1665 = shl i32 %1660, 13
  %1666 = xor i32 %1665, %1660
  %1667 = lshr i32 %1666, 17
  %1668 = xor i32 %1667, %1666
  %1669 = shl i32 %1668, 5
  %1670 = xor i32 %1669, %1668
  store i32 %1670, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1671 = trunc i32 %1670 to i8
  %1672 = xor i8 %1664, %1671
  %1673 = icmp samesign ult i64 %1658, 8
  %1674 = zext i8 %1672 to i64
  %1675 = shl nuw nsw i64 %1658, 3
  %1676 = shl nuw i64 %1674, %1675
  %1677 = select i1 %1673, i64 %1676, i64 0
  %1678 = or i64 %1677, %1659
  %1679 = add nuw nsw i64 %1658, 1
  %1680 = icmp eq i64 %1679, %1655
  br i1 %1680, label %1681, label %1656, !llvm.loop !33

1681:                                             ; preds = %1656
  %1682 = phi i32 [ %1662, %1656 ]
  %1683 = phi i32 [ %1670, %1656 ]
  %1684 = phi i64 [ %1678, %1656 ]
  br label %1685

1685:                                             ; preds = %1681, %1652
  %1686 = phi i32 [ %1649, %1652 ], [ %1683, %1681 ]
  %1687 = phi i32 [ %1640, %1652 ], [ %1682, %1681 ]
  %1688 = phi i64 [ 0, %1652 ], [ %1684, %1681 ]
  %1689 = icmp eq i8 %1631, %1638
  br i1 %1689, label %1747, label %1690

1690:                                             ; preds = %1685
  %1691 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %1692 = inttoptr i64 %1691 to ptr
  %1693 = zext i8 %1639 to i64
  br label %1694

1694:                                             ; preds = %1707, %1690
  %1695 = phi i64 [ 0, %1690 ], [ %1709, %1707 ]
  %1696 = phi i64 [ %1688, %1690 ], [ %1698, %1707 ]
  %1697 = phi i64 [ 0, %1690 ], [ %1708, %1707 ]
  %1698 = add i64 %1696, 1
  %1699 = icmp samesign ult i64 %1695, 8
  br i1 %1699, label %1700, label %1707

1700:                                             ; preds = %1694
  %1701 = getelementptr inbounds nuw i8, ptr %1692, i64 %1696
  %1702 = load i8, ptr %1701, align 1, !tbaa !12
  %1703 = zext i8 %1702 to i64
  %1704 = shl nuw nsw i64 %1695, 3
  %1705 = shl nuw i64 %1703, %1704
  %1706 = or i64 %1705, %1697
  br label %1707

1707:                                             ; preds = %1700, %1694
  %1708 = phi i64 [ %1706, %1700 ], [ %1697, %1694 ]
  %1709 = add nuw nsw i64 %1695, 1
  %1710 = icmp eq i64 %1709, %1693
  br i1 %1710, label %1741, label %1694, !llvm.loop !35

1711:                                             ; preds = %1624
  %1712 = icmp eq i8 %1631, %1638
  br i1 %1712, label %1747, label %1713

1713:                                             ; preds = %1711
  %1714 = sext i32 %1640 to i64
  %1715 = zext i8 %1639 to i64
  br label %1716

1716:                                             ; preds = %1716, %1713
  %1717 = phi i64 [ %1714, %1713 ], [ %1721, %1716 ]
  %1718 = phi i64 [ 0, %1713 ], [ %1739, %1716 ]
  %1719 = phi i64 [ 0, %1713 ], [ %1738, %1716 ]
  %1720 = phi i32 [ %1649, %1713 ], [ %1730, %1716 ]
  %1721 = add nsw i64 %1717, 1
  %1722 = trunc nsw i64 %1721 to i32
  store i32 %1722, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1723 = getelementptr inbounds i8, ptr %1545, i64 %1717
  %1724 = load i8, ptr %1723, align 1, !tbaa !12
  %1725 = shl i32 %1720, 13
  %1726 = xor i32 %1725, %1720
  %1727 = lshr i32 %1726, 17
  %1728 = xor i32 %1727, %1726
  %1729 = shl i32 %1728, 5
  %1730 = xor i32 %1729, %1728
  store i32 %1730, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1731 = trunc i32 %1730 to i8
  %1732 = xor i8 %1724, %1731
  %1733 = icmp samesign ult i64 %1718, 8
  %1734 = zext i8 %1732 to i64
  %1735 = shl nuw nsw i64 %1718, 3
  %1736 = shl nuw i64 %1734, %1735
  %1737 = select i1 %1733, i64 %1736, i64 0
  %1738 = or i64 %1737, %1719
  %1739 = add nuw nsw i64 %1718, 1
  %1740 = icmp eq i64 %1739, %1715
  br i1 %1740, label %1743, label %1716, !llvm.loop !33

1741:                                             ; preds = %1707
  %1742 = phi i64 [ %1708, %1707 ]
  br label %1747

1743:                                             ; preds = %1716
  %1744 = phi i32 [ %1722, %1716 ]
  %1745 = phi i32 [ %1730, %1716 ]
  %1746 = phi i64 [ %1738, %1716 ]
  br label %1747

1747:                                             ; preds = %1743, %1741, %1711, %1685
  %1748 = phi i32 [ %1686, %1685 ], [ %1649, %1711 ], [ %1686, %1741 ], [ %1745, %1743 ]
  %1749 = phi i32 [ %1687, %1685 ], [ %1640, %1711 ], [ %1687, %1741 ], [ %1744, %1743 ]
  %1750 = phi i64 [ 0, %1685 ], [ 0, %1711 ], [ %1742, %1741 ], [ %1746, %1743 ]
  %1751 = add nsw i32 %1749, 1
  store i32 %1751, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1752 = sext i32 %1749 to i64
  %1753 = getelementptr inbounds i8, ptr %1545, i64 %1752
  %1754 = load i8, ptr %1753, align 1, !tbaa !12
  %1755 = shl i32 %1748, 13
  %1756 = xor i32 %1755, %1748
  %1757 = lshr i32 %1756, 17
  %1758 = xor i32 %1757, %1756
  %1759 = shl i32 %1758, 5
  %1760 = xor i32 %1759, %1758
  store i32 %1760, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1761 = trunc i32 %1760 to i8
  %1762 = xor i8 %1754, %1761
  %1763 = add nsw i32 %1749, 2
  store i32 %1763, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1764 = sext i32 %1751 to i64
  %1765 = getelementptr inbounds i8, ptr %1545, i64 %1764
  %1766 = load i8, ptr %1765, align 1, !tbaa !12
  %1767 = shl i32 %1760, 13
  %1768 = xor i32 %1767, %1760
  %1769 = lshr i32 %1768, 17
  %1770 = xor i32 %1769, %1768
  %1771 = shl i32 %1770, 5
  %1772 = xor i32 %1771, %1770
  store i32 %1772, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1773 = trunc i32 %1772 to i8
  %1774 = icmp eq i8 %1766, %1773
  br i1 %1774, label %1775, label %1830

1775:                                             ; preds = %1747
  br i1 %1591, label %1776, label %1806

1776:                                             ; preds = %1775
  %1777 = sext i32 %1763 to i64
  %1778 = zext nneg i32 %1590 to i64
  br label %1779

1779:                                             ; preds = %1779, %1776
  %1780 = phi i64 [ %1777, %1776 ], [ %1784, %1779 ]
  %1781 = phi i64 [ 0, %1776 ], [ %1802, %1779 ]
  %1782 = phi i64 [ 0, %1776 ], [ %1801, %1779 ]
  %1783 = phi i32 [ %1772, %1776 ], [ %1793, %1779 ]
  %1784 = add nsw i64 %1780, 1
  %1785 = trunc nsw i64 %1784 to i32
  store i32 %1785, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1786 = getelementptr inbounds i8, ptr %1545, i64 %1780
  %1787 = load i8, ptr %1786, align 1, !tbaa !12
  %1788 = shl i32 %1783, 13
  %1789 = xor i32 %1788, %1783
  %1790 = lshr i32 %1789, 17
  %1791 = xor i32 %1790, %1789
  %1792 = shl i32 %1791, 5
  %1793 = xor i32 %1792, %1791
  store i32 %1793, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1794 = trunc i32 %1793 to i8
  %1795 = xor i8 %1787, %1794
  %1796 = icmp samesign ult i64 %1781, 8
  %1797 = zext i8 %1795 to i64
  %1798 = shl nuw nsw i64 %1781, 3
  %1799 = shl nuw i64 %1797, %1798
  %1800 = select i1 %1796, i64 %1799, i64 0
  %1801 = or i64 %1800, %1782
  %1802 = add nuw nsw i64 %1781, 1
  %1803 = icmp eq i64 %1802, %1778
  br i1 %1803, label %1804, label %1779, !llvm.loop !33

1804:                                             ; preds = %1779
  %1805 = phi i64 [ %1801, %1779 ]
  br label %1806

1806:                                             ; preds = %1804, %1775
  %1807 = phi i64 [ 0, %1775 ], [ %1805, %1804 ]
  %1808 = icmp eq i8 %1754, %1761
  br i1 %1808, label %1864, label %1809

1809:                                             ; preds = %1806
  %1810 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %1811 = inttoptr i64 %1810 to ptr
  %1812 = zext i8 %1762 to i64
  br label %1813

1813:                                             ; preds = %1826, %1809
  %1814 = phi i64 [ 0, %1809 ], [ %1828, %1826 ]
  %1815 = phi i64 [ %1807, %1809 ], [ %1817, %1826 ]
  %1816 = phi i64 [ 0, %1809 ], [ %1827, %1826 ]
  %1817 = add i64 %1815, 1
  %1818 = icmp samesign ult i64 %1814, 8
  br i1 %1818, label %1819, label %1826

1819:                                             ; preds = %1813
  %1820 = getelementptr inbounds nuw i8, ptr %1811, i64 %1815
  %1821 = load i8, ptr %1820, align 1, !tbaa !12
  %1822 = zext i8 %1821 to i64
  %1823 = shl nuw nsw i64 %1814, 3
  %1824 = shl nuw i64 %1822, %1823
  %1825 = or i64 %1824, %1816
  br label %1826

1826:                                             ; preds = %1819, %1813
  %1827 = phi i64 [ %1825, %1819 ], [ %1816, %1813 ]
  %1828 = add nuw nsw i64 %1814, 1
  %1829 = icmp eq i64 %1828, %1812
  br i1 %1829, label %1860, label %1813, !llvm.loop !35

1830:                                             ; preds = %1747
  %1831 = icmp eq i8 %1754, %1761
  br i1 %1831, label %1864, label %1832

1832:                                             ; preds = %1830
  %1833 = sext i32 %1763 to i64
  %1834 = zext i8 %1762 to i64
  br label %1835

1835:                                             ; preds = %1835, %1832
  %1836 = phi i64 [ %1833, %1832 ], [ %1840, %1835 ]
  %1837 = phi i64 [ 0, %1832 ], [ %1858, %1835 ]
  %1838 = phi i64 [ 0, %1832 ], [ %1857, %1835 ]
  %1839 = phi i32 [ %1772, %1832 ], [ %1849, %1835 ]
  %1840 = add nsw i64 %1836, 1
  %1841 = trunc nsw i64 %1840 to i32
  store i32 %1841, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1842 = getelementptr inbounds i8, ptr %1545, i64 %1836
  %1843 = load i8, ptr %1842, align 1, !tbaa !12
  %1844 = shl i32 %1839, 13
  %1845 = xor i32 %1844, %1839
  %1846 = lshr i32 %1845, 17
  %1847 = xor i32 %1846, %1845
  %1848 = shl i32 %1847, 5
  %1849 = xor i32 %1848, %1847
  store i32 %1849, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1850 = trunc i32 %1849 to i8
  %1851 = xor i8 %1843, %1850
  %1852 = icmp samesign ult i64 %1837, 8
  %1853 = zext i8 %1851 to i64
  %1854 = shl nuw nsw i64 %1837, 3
  %1855 = shl nuw i64 %1853, %1854
  %1856 = select i1 %1852, i64 %1855, i64 0
  %1857 = or i64 %1856, %1838
  %1858 = add nuw nsw i64 %1837, 1
  %1859 = icmp eq i64 %1858, %1834
  br i1 %1859, label %1862, label %1835, !llvm.loop !33

1860:                                             ; preds = %1826
  %1861 = phi i64 [ %1827, %1826 ]
  br label %1864

1862:                                             ; preds = %1835
  %1863 = phi i64 [ %1857, %1835 ]
  br label %1864

1864:                                             ; preds = %1862, %1860, %1830, %1806
  %1865 = phi i64 [ 0, %1806 ], [ 0, %1830 ], [ %1861, %1860 ], [ %1863, %1862 ]
  %1866 = icmp eq i8 %1574, %1581
  br i1 %1866, label %2370, label %1867

1867:                                             ; preds = %1864
  %1868 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %1869 = add i64 %1868, %1627
  %1870 = inttoptr i64 %1869 to ptr
  %1871 = zext i8 %1582 to i32
  %1872 = icmp ne i8 %1550, %1558
  %1873 = icmp ne i8 %1563, %1570
  %1874 = select i1 %1872, i1 %1873, i1 false
  %1875 = zext i8 %1559 to i64
  %1876 = select i1 %1874, i64 %1875, i64 1
  %1877 = mul i64 %1865, %1876
  %1878 = add i64 %1877, %1750
  br label %1879

1879:                                             ; preds = %1879, %1867
  %1880 = phi i32 [ %1886, %1879 ], [ 0, %1867 ]
  %1881 = phi ptr [ %1884, %1879 ], [ %1870, %1867 ]
  %1882 = phi i64 [ %1885, %1879 ], [ %1878, %1867 ]
  %1883 = trunc i64 %1882 to i8
  store i8 %1883, ptr %1881, align 1, !tbaa !12
  %1884 = getelementptr inbounds nuw i8, ptr %1881, i64 1
  %1885 = lshr i64 %1882, 8
  %1886 = add nuw nsw i32 %1880, 1
  %1887 = icmp eq i32 %1886, %1871
  br i1 %1887, label %2361, label %1879, !llvm.loop !34

1888:                                             ; preds = %239
  %1889 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %1890 = inttoptr i64 %1889 to ptr
  %1891 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1892 = add nsw i32 %1891, 1
  store i32 %1892, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1893 = sext i32 %1891 to i64
  %1894 = getelementptr inbounds i8, ptr %1890, i64 %1893
  %1895 = load i8, ptr %1894, align 1, !tbaa !12
  %1896 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1897 = shl i32 %1896, 13
  %1898 = xor i32 %1897, %1896
  %1899 = lshr i32 %1898, 17
  %1900 = xor i32 %1899, %1898
  %1901 = shl i32 %1900, 5
  %1902 = xor i32 %1901, %1900
  store i32 %1902, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1903 = trunc i32 %1902 to i8
  %1904 = xor i8 %1895, %1903
  %1905 = add nsw i32 %1891, 2
  store i32 %1905, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1906 = sext i32 %1892 to i64
  %1907 = getelementptr inbounds i8, ptr %1890, i64 %1906
  %1908 = load i8, ptr %1907, align 1, !tbaa !12
  %1909 = shl i32 %1902, 13
  %1910 = xor i32 %1909, %1902
  %1911 = lshr i32 %1910, 17
  %1912 = xor i32 %1911, %1910
  %1913 = shl i32 %1912, 5
  %1914 = xor i32 %1913, %1912
  %1915 = trunc i32 %1914 to i8
  %1916 = xor i8 %1908, %1915
  %1917 = add nsw i32 %1891, 3
  %1918 = shl i32 %1914, 13
  %1919 = xor i32 %1918, %1914
  %1920 = lshr i32 %1919, 17
  %1921 = xor i32 %1920, %1919
  %1922 = shl i32 %1921, 5
  %1923 = xor i32 %1922, %1921
  store i32 %1923, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1924 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %1925 = icmp sgt i32 %1924, 0
  br i1 %1925, label %1926, label %1958

1926:                                             ; preds = %1888
  %1927 = sext i32 %1917 to i64
  %1928 = zext nneg i32 %1924 to i64
  br label %1929

1929:                                             ; preds = %1929, %1926
  %1930 = phi i64 [ %1927, %1926 ], [ %1934, %1929 ]
  %1931 = phi i64 [ 0, %1926 ], [ %1952, %1929 ]
  %1932 = phi i64 [ 0, %1926 ], [ %1951, %1929 ]
  %1933 = phi i32 [ %1923, %1926 ], [ %1943, %1929 ]
  %1934 = add nsw i64 %1930, 1
  %1935 = trunc nsw i64 %1934 to i32
  store i32 %1935, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1936 = getelementptr inbounds i8, ptr %1890, i64 %1930
  %1937 = load i8, ptr %1936, align 1, !tbaa !12
  %1938 = shl i32 %1933, 13
  %1939 = xor i32 %1938, %1933
  %1940 = lshr i32 %1939, 17
  %1941 = xor i32 %1940, %1939
  %1942 = shl i32 %1941, 5
  %1943 = xor i32 %1942, %1941
  store i32 %1943, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1944 = trunc i32 %1943 to i8
  %1945 = xor i8 %1937, %1944
  %1946 = icmp samesign ult i64 %1931, 8
  %1947 = zext i8 %1945 to i64
  %1948 = shl nuw nsw i64 %1931, 3
  %1949 = shl nuw i64 %1947, %1948
  %1950 = select i1 %1946, i64 %1949, i64 0
  %1951 = or i64 %1950, %1932
  %1952 = add nuw nsw i64 %1931, 1
  %1953 = icmp eq i64 %1952, %1928
  br i1 %1953, label %1954, label %1929, !llvm.loop !33

1954:                                             ; preds = %1929
  %1955 = phi i32 [ %1935, %1929 ]
  %1956 = phi i32 [ %1943, %1929 ]
  %1957 = phi i64 [ %1951, %1929 ]
  br label %1958

1958:                                             ; preds = %1954, %1888
  %1959 = phi i32 [ %1923, %1888 ], [ %1956, %1954 ]
  %1960 = phi i32 [ %1917, %1888 ], [ %1955, %1954 ]
  %1961 = phi i64 [ 0, %1888 ], [ %1957, %1954 ]
  %1962 = add nsw i32 %1960, 1
  store i32 %1962, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1963 = sext i32 %1960 to i64
  %1964 = getelementptr inbounds i8, ptr %1890, i64 %1963
  %1965 = load i8, ptr %1964, align 1, !tbaa !12
  %1966 = shl i32 %1959, 13
  %1967 = xor i32 %1966, %1959
  %1968 = lshr i32 %1967, 17
  %1969 = xor i32 %1968, %1967
  %1970 = shl i32 %1969, 5
  %1971 = xor i32 %1970, %1969
  store i32 %1971, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1972 = trunc i32 %1971 to i8
  %1973 = xor i8 %1965, %1972
  %1974 = add nsw i32 %1960, 2
  store i32 %1974, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1975 = sext i32 %1962 to i64
  %1976 = getelementptr inbounds i8, ptr %1890, i64 %1975
  %1977 = load i8, ptr %1976, align 1, !tbaa !12
  %1978 = shl i32 %1971, 13
  %1979 = xor i32 %1978, %1971
  %1980 = lshr i32 %1979, 17
  %1981 = xor i32 %1980, %1979
  %1982 = shl i32 %1981, 5
  %1983 = xor i32 %1982, %1981
  store i32 %1983, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %1984 = trunc i32 %1983 to i8
  %1985 = icmp eq i8 %1977, %1984
  br i1 %1985, label %1986, label %2045

1986:                                             ; preds = %1958
  br i1 %1925, label %1987, label %2019

1987:                                             ; preds = %1986
  %1988 = sext i32 %1974 to i64
  %1989 = zext nneg i32 %1924 to i64
  br label %1990

1990:                                             ; preds = %1990, %1987
  %1991 = phi i64 [ %1988, %1987 ], [ %1995, %1990 ]
  %1992 = phi i64 [ 0, %1987 ], [ %2013, %1990 ]
  %1993 = phi i64 [ 0, %1987 ], [ %2012, %1990 ]
  %1994 = phi i32 [ %1983, %1987 ], [ %2004, %1990 ]
  %1995 = add nsw i64 %1991, 1
  %1996 = trunc nsw i64 %1995 to i32
  store i32 %1996, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %1997 = getelementptr inbounds i8, ptr %1890, i64 %1991
  %1998 = load i8, ptr %1997, align 1, !tbaa !12
  %1999 = shl i32 %1994, 13
  %2000 = xor i32 %1999, %1994
  %2001 = lshr i32 %2000, 17
  %2002 = xor i32 %2001, %2000
  %2003 = shl i32 %2002, 5
  %2004 = xor i32 %2003, %2002
  store i32 %2004, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2005 = trunc i32 %2004 to i8
  %2006 = xor i8 %1998, %2005
  %2007 = icmp samesign ult i64 %1992, 8
  %2008 = zext i8 %2006 to i64
  %2009 = shl nuw nsw i64 %1992, 3
  %2010 = shl nuw i64 %2008, %2009
  %2011 = select i1 %2007, i64 %2010, i64 0
  %2012 = or i64 %2011, %1993
  %2013 = add nuw nsw i64 %1992, 1
  %2014 = icmp eq i64 %2013, %1989
  br i1 %2014, label %2015, label %1990, !llvm.loop !33

2015:                                             ; preds = %1990
  %2016 = phi i32 [ %1996, %1990 ]
  %2017 = phi i32 [ %2004, %1990 ]
  %2018 = phi i64 [ %2012, %1990 ]
  br label %2019

2019:                                             ; preds = %2015, %1986
  %2020 = phi i32 [ %1983, %1986 ], [ %2017, %2015 ]
  %2021 = phi i32 [ %1974, %1986 ], [ %2016, %2015 ]
  %2022 = phi i64 [ 0, %1986 ], [ %2018, %2015 ]
  %2023 = icmp eq i8 %1965, %1972
  br i1 %2023, label %2081, label %2024

2024:                                             ; preds = %2019
  %2025 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %2026 = inttoptr i64 %2025 to ptr
  %2027 = zext i8 %1973 to i64
  br label %2028

2028:                                             ; preds = %2041, %2024
  %2029 = phi i64 [ 0, %2024 ], [ %2043, %2041 ]
  %2030 = phi i64 [ %2022, %2024 ], [ %2032, %2041 ]
  %2031 = phi i64 [ 0, %2024 ], [ %2042, %2041 ]
  %2032 = add i64 %2030, 1
  %2033 = icmp samesign ult i64 %2029, 8
  br i1 %2033, label %2034, label %2041

2034:                                             ; preds = %2028
  %2035 = getelementptr inbounds nuw i8, ptr %2026, i64 %2030
  %2036 = load i8, ptr %2035, align 1, !tbaa !12
  %2037 = zext i8 %2036 to i64
  %2038 = shl nuw nsw i64 %2029, 3
  %2039 = shl nuw i64 %2037, %2038
  %2040 = or i64 %2039, %2031
  br label %2041

2041:                                             ; preds = %2034, %2028
  %2042 = phi i64 [ %2040, %2034 ], [ %2031, %2028 ]
  %2043 = add nuw nsw i64 %2029, 1
  %2044 = icmp eq i64 %2043, %2027
  br i1 %2044, label %2075, label %2028, !llvm.loop !35

2045:                                             ; preds = %1958
  %2046 = icmp eq i8 %1965, %1972
  br i1 %2046, label %2081, label %2047

2047:                                             ; preds = %2045
  %2048 = sext i32 %1974 to i64
  %2049 = zext i8 %1973 to i64
  br label %2050

2050:                                             ; preds = %2050, %2047
  %2051 = phi i64 [ %2048, %2047 ], [ %2055, %2050 ]
  %2052 = phi i64 [ 0, %2047 ], [ %2073, %2050 ]
  %2053 = phi i64 [ 0, %2047 ], [ %2072, %2050 ]
  %2054 = phi i32 [ %1983, %2047 ], [ %2064, %2050 ]
  %2055 = add nsw i64 %2051, 1
  %2056 = trunc nsw i64 %2055 to i32
  store i32 %2056, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2057 = getelementptr inbounds i8, ptr %1890, i64 %2051
  %2058 = load i8, ptr %2057, align 1, !tbaa !12
  %2059 = shl i32 %2054, 13
  %2060 = xor i32 %2059, %2054
  %2061 = lshr i32 %2060, 17
  %2062 = xor i32 %2061, %2060
  %2063 = shl i32 %2062, 5
  %2064 = xor i32 %2063, %2062
  store i32 %2064, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2065 = trunc i32 %2064 to i8
  %2066 = xor i8 %2058, %2065
  %2067 = icmp samesign ult i64 %2052, 8
  %2068 = zext i8 %2066 to i64
  %2069 = shl nuw nsw i64 %2052, 3
  %2070 = shl nuw i64 %2068, %2069
  %2071 = select i1 %2067, i64 %2070, i64 0
  %2072 = or i64 %2071, %2053
  %2073 = add nuw nsw i64 %2052, 1
  %2074 = icmp eq i64 %2073, %2049
  br i1 %2074, label %2077, label %2050, !llvm.loop !33

2075:                                             ; preds = %2041
  %2076 = phi i64 [ %2042, %2041 ]
  br label %2081

2077:                                             ; preds = %2050
  %2078 = phi i32 [ %2056, %2050 ]
  %2079 = phi i32 [ %2064, %2050 ]
  %2080 = phi i64 [ %2072, %2050 ]
  br label %2081

2081:                                             ; preds = %2077, %2075, %2045, %2019
  %2082 = phi i32 [ %2020, %2019 ], [ %1983, %2045 ], [ %2020, %2075 ], [ %2079, %2077 ]
  %2083 = phi i32 [ %2021, %2019 ], [ %1974, %2045 ], [ %2021, %2075 ], [ %2078, %2077 ]
  %2084 = phi i64 [ 0, %2019 ], [ 0, %2045 ], [ %2076, %2075 ], [ %2080, %2077 ]
  %2085 = add nsw i32 %2083, 1
  store i32 %2085, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2086 = sext i32 %2083 to i64
  %2087 = getelementptr inbounds i8, ptr %1890, i64 %2086
  %2088 = load i8, ptr %2087, align 1, !tbaa !12
  %2089 = shl i32 %2082, 13
  %2090 = xor i32 %2089, %2082
  %2091 = lshr i32 %2090, 17
  %2092 = xor i32 %2091, %2090
  %2093 = shl i32 %2092, 5
  %2094 = xor i32 %2093, %2092
  store i32 %2094, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2095 = trunc i32 %2094 to i8
  %2096 = xor i8 %2088, %2095
  %2097 = add nsw i32 %2083, 2
  store i32 %2097, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2098 = sext i32 %2085 to i64
  %2099 = getelementptr inbounds i8, ptr %1890, i64 %2098
  %2100 = load i8, ptr %2099, align 1, !tbaa !12
  %2101 = shl i32 %2094, 13
  %2102 = xor i32 %2101, %2094
  %2103 = lshr i32 %2102, 17
  %2104 = xor i32 %2103, %2102
  %2105 = shl i32 %2104, 5
  %2106 = xor i32 %2105, %2104
  store i32 %2106, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2107 = trunc i32 %2106 to i8
  %2108 = icmp eq i8 %2100, %2107
  br i1 %2108, label %2109, label %2164

2109:                                             ; preds = %2081
  br i1 %1925, label %2110, label %2140

2110:                                             ; preds = %2109
  %2111 = sext i32 %2097 to i64
  %2112 = zext nneg i32 %1924 to i64
  br label %2113

2113:                                             ; preds = %2113, %2110
  %2114 = phi i64 [ %2111, %2110 ], [ %2118, %2113 ]
  %2115 = phi i64 [ 0, %2110 ], [ %2136, %2113 ]
  %2116 = phi i64 [ 0, %2110 ], [ %2135, %2113 ]
  %2117 = phi i32 [ %2106, %2110 ], [ %2127, %2113 ]
  %2118 = add nsw i64 %2114, 1
  %2119 = trunc nsw i64 %2118 to i32
  store i32 %2119, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2120 = getelementptr inbounds i8, ptr %1890, i64 %2114
  %2121 = load i8, ptr %2120, align 1, !tbaa !12
  %2122 = shl i32 %2117, 13
  %2123 = xor i32 %2122, %2117
  %2124 = lshr i32 %2123, 17
  %2125 = xor i32 %2124, %2123
  %2126 = shl i32 %2125, 5
  %2127 = xor i32 %2126, %2125
  store i32 %2127, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2128 = trunc i32 %2127 to i8
  %2129 = xor i8 %2121, %2128
  %2130 = icmp samesign ult i64 %2115, 8
  %2131 = zext i8 %2129 to i64
  %2132 = shl nuw nsw i64 %2115, 3
  %2133 = shl nuw i64 %2131, %2132
  %2134 = select i1 %2130, i64 %2133, i64 0
  %2135 = or i64 %2134, %2116
  %2136 = add nuw nsw i64 %2115, 1
  %2137 = icmp eq i64 %2136, %2112
  br i1 %2137, label %2138, label %2113, !llvm.loop !33

2138:                                             ; preds = %2113
  %2139 = phi i64 [ %2135, %2113 ]
  br label %2140

2140:                                             ; preds = %2138, %2109
  %2141 = phi i64 [ 0, %2109 ], [ %2139, %2138 ]
  %2142 = icmp eq i8 %2088, %2095
  br i1 %2142, label %2198, label %2143

2143:                                             ; preds = %2140
  %2144 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %2145 = inttoptr i64 %2144 to ptr
  %2146 = zext i8 %2096 to i64
  br label %2147

2147:                                             ; preds = %2160, %2143
  %2148 = phi i64 [ 0, %2143 ], [ %2162, %2160 ]
  %2149 = phi i64 [ %2141, %2143 ], [ %2151, %2160 ]
  %2150 = phi i64 [ 0, %2143 ], [ %2161, %2160 ]
  %2151 = add i64 %2149, 1
  %2152 = icmp samesign ult i64 %2148, 8
  br i1 %2152, label %2153, label %2160

2153:                                             ; preds = %2147
  %2154 = getelementptr inbounds nuw i8, ptr %2145, i64 %2149
  %2155 = load i8, ptr %2154, align 1, !tbaa !12
  %2156 = zext i8 %2155 to i64
  %2157 = shl nuw nsw i64 %2148, 3
  %2158 = shl nuw i64 %2156, %2157
  %2159 = or i64 %2158, %2150
  br label %2160

2160:                                             ; preds = %2153, %2147
  %2161 = phi i64 [ %2159, %2153 ], [ %2150, %2147 ]
  %2162 = add nuw nsw i64 %2148, 1
  %2163 = icmp eq i64 %2162, %2146
  br i1 %2163, label %2194, label %2147, !llvm.loop !35

2164:                                             ; preds = %2081
  %2165 = icmp eq i8 %2088, %2095
  br i1 %2165, label %2198, label %2166

2166:                                             ; preds = %2164
  %2167 = sext i32 %2097 to i64
  %2168 = zext i8 %2096 to i64
  br label %2169

2169:                                             ; preds = %2169, %2166
  %2170 = phi i64 [ %2167, %2166 ], [ %2174, %2169 ]
  %2171 = phi i64 [ 0, %2166 ], [ %2192, %2169 ]
  %2172 = phi i64 [ 0, %2166 ], [ %2191, %2169 ]
  %2173 = phi i32 [ %2106, %2166 ], [ %2183, %2169 ]
  %2174 = add nsw i64 %2170, 1
  %2175 = trunc nsw i64 %2174 to i32
  store i32 %2175, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2176 = getelementptr inbounds i8, ptr %1890, i64 %2170
  %2177 = load i8, ptr %2176, align 1, !tbaa !12
  %2178 = shl i32 %2173, 13
  %2179 = xor i32 %2178, %2173
  %2180 = lshr i32 %2179, 17
  %2181 = xor i32 %2180, %2179
  %2182 = shl i32 %2181, 5
  %2183 = xor i32 %2182, %2181
  store i32 %2183, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2184 = trunc i32 %2183 to i8
  %2185 = xor i8 %2177, %2184
  %2186 = icmp samesign ult i64 %2171, 8
  %2187 = zext i8 %2185 to i64
  %2188 = shl nuw nsw i64 %2171, 3
  %2189 = shl nuw i64 %2187, %2188
  %2190 = select i1 %2186, i64 %2189, i64 0
  %2191 = or i64 %2190, %2172
  %2192 = add nuw nsw i64 %2171, 1
  %2193 = icmp eq i64 %2192, %2168
  br i1 %2193, label %2196, label %2169, !llvm.loop !33

2194:                                             ; preds = %2160
  %2195 = phi i64 [ %2161, %2160 ]
  br label %2198

2196:                                             ; preds = %2169
  %2197 = phi i64 [ %2191, %2169 ]
  br label %2198

2198:                                             ; preds = %2196, %2194, %2164, %2140
  %2199 = phi i64 [ 0, %2140 ], [ 0, %2164 ], [ %2195, %2194 ], [ %2197, %2196 ]
  %2200 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %2201 = icmp eq i8 %2200, 0
  br i1 %2201, label %2204, label %2202

2202:                                             ; preds = %2198
  %2203 = zext i8 %1904 to i64
  tail call void @vmp_debug_id__Z8vm_entryv(i32 noundef 3, i64 noundef %2203) #11
  br label %2204

2204:                                             ; preds = %2202, %2198
  %2205 = add i8 %1904, -42
  %2206 = icmp ult i8 %2205, 16
  %2207 = icmp ult i8 %1973, 5
  br i1 %2206, label %2208, label %2284

2208:                                             ; preds = %2204
  br i1 %2207, label %2209, label %2248

2209:                                             ; preds = %2208
  %2210 = trunc i64 %2084 to i32
  %2211 = trunc i64 %2199 to i32
  %2212 = bitcast i32 %2210 to float
  %2213 = bitcast i32 %2211 to float
  switch i8 %1904, label %2247 [
    i8 42, label %2311
    i8 43, label %2214
    i8 44, label %2216
    i8 45, label %2218
    i8 46, label %2220
    i8 47, label %2222
    i8 48, label %2224
    i8 49, label %2226
    i8 50, label %2230
    i8 51, label %2234
    i8 52, label %2236
    i8 53, label %2238
    i8 54, label %2240
    i8 55, label %2242
    i8 56, label %2244
    i8 57, label %2246
  ]

2214:                                             ; preds = %2209
  %2215 = fcmp oeq float %2212, %2213
  br label %2311

2216:                                             ; preds = %2209
  %2217 = fcmp ogt float %2212, %2213
  br label %2311

2218:                                             ; preds = %2209
  %2219 = fcmp oge float %2212, %2213
  br label %2311

2220:                                             ; preds = %2209
  %2221 = fcmp olt float %2212, %2213
  br label %2311

2222:                                             ; preds = %2209
  %2223 = fcmp ole float %2212, %2213
  br label %2311

2224:                                             ; preds = %2209
  %2225 = fcmp une float %2212, %2213
  br label %2311

2226:                                             ; preds = %2209
  %2227 = fcmp ord float %2212, 0.000000e+00
  %2228 = fcmp ord float %2213, 0.000000e+00
  %2229 = select i1 %2227, i1 %2228, i1 false
  br label %2311

2230:                                             ; preds = %2209
  %2231 = fcmp uno float %2212, 0.000000e+00
  %2232 = fcmp uno float %2213, 0.000000e+00
  %2233 = select i1 %2231, i1 true, i1 %2232
  br label %2311

2234:                                             ; preds = %2209
  %2235 = fcmp ueq float %2212, %2213
  br label %2311

2236:                                             ; preds = %2209
  %2237 = fcmp ugt float %2212, %2213
  br label %2311

2238:                                             ; preds = %2209
  %2239 = fcmp uge float %2212, %2213
  br label %2311

2240:                                             ; preds = %2209
  %2241 = fcmp ult float %2212, %2213
  br label %2311

2242:                                             ; preds = %2209
  %2243 = fcmp ule float %2212, %2213
  br label %2311

2244:                                             ; preds = %2209
  %2245 = fcmp une float %2212, %2213
  br label %2311

2246:                                             ; preds = %2209
  br label %2311

2247:                                             ; preds = %2248, %2209
  unreachable

2248:                                             ; preds = %2208
  %2249 = bitcast i64 %2084 to double
  %2250 = bitcast i64 %2199 to double
  switch i8 %1904, label %2247 [
    i8 42, label %2311
    i8 43, label %2251
    i8 44, label %2253
    i8 45, label %2255
    i8 46, label %2257
    i8 47, label %2259
    i8 48, label %2261
    i8 49, label %2263
    i8 50, label %2267
    i8 51, label %2271
    i8 52, label %2273
    i8 53, label %2275
    i8 54, label %2277
    i8 55, label %2279
    i8 56, label %2281
    i8 57, label %2283
  ]

2251:                                             ; preds = %2248
  %2252 = fcmp oeq double %2249, %2250
  br label %2311

2253:                                             ; preds = %2248
  %2254 = fcmp ogt double %2249, %2250
  br label %2311

2255:                                             ; preds = %2248
  %2256 = fcmp oge double %2249, %2250
  br label %2311

2257:                                             ; preds = %2248
  %2258 = fcmp olt double %2249, %2250
  br label %2311

2259:                                             ; preds = %2248
  %2260 = fcmp ole double %2249, %2250
  br label %2311

2261:                                             ; preds = %2248
  %2262 = fcmp une double %2249, %2250
  br label %2311

2263:                                             ; preds = %2248
  %2264 = fcmp ord double %2249, 0.000000e+00
  %2265 = fcmp ord double %2250, 0.000000e+00
  %2266 = select i1 %2264, i1 %2265, i1 false
  br label %2311

2267:                                             ; preds = %2248
  %2268 = fcmp uno double %2249, 0.000000e+00
  %2269 = fcmp uno double %2250, 0.000000e+00
  %2270 = select i1 %2268, i1 true, i1 %2269
  br label %2311

2271:                                             ; preds = %2248
  %2272 = fcmp ueq double %2249, %2250
  br label %2311

2273:                                             ; preds = %2248
  %2274 = fcmp ugt double %2249, %2250
  br label %2311

2275:                                             ; preds = %2248
  %2276 = fcmp uge double %2249, %2250
  br label %2311

2277:                                             ; preds = %2248
  %2278 = fcmp ult double %2249, %2250
  br label %2311

2279:                                             ; preds = %2248
  %2280 = fcmp ule double %2249, %2250
  br label %2311

2281:                                             ; preds = %2248
  %2282 = fcmp une double %2249, %2250
  br label %2311

2283:                                             ; preds = %2248
  br label %2311

2284:                                             ; preds = %2204
  %2285 = shl i64 %2084, 32
  %2286 = ashr exact i64 %2285, 32
  %2287 = shl i64 %2199, 32
  %2288 = ashr exact i64 %2287, 32
  %2289 = select i1 %2207, i64 %2286, i64 %2084
  %2290 = select i1 %2207, i64 %2288, i64 %2199
  switch i8 %1904, label %2311 [
    i8 32, label %2291
    i8 33, label %2293
    i8 34, label %2295
    i8 35, label %2297
    i8 36, label %2299
    i8 37, label %2301
    i8 38, label %2303
    i8 39, label %2305
    i8 40, label %2307
    i8 41, label %2309
  ]

2291:                                             ; preds = %2284
  %2292 = icmp eq i64 %2084, %2199
  br label %2311

2293:                                             ; preds = %2284
  %2294 = icmp ne i64 %2084, %2199
  br label %2311

2295:                                             ; preds = %2284
  %2296 = icmp ugt i64 %2084, %2199
  br label %2311

2297:                                             ; preds = %2284
  %2298 = icmp uge i64 %2084, %2199
  br label %2311

2299:                                             ; preds = %2284
  %2300 = icmp ult i64 %2084, %2199
  br label %2311

2301:                                             ; preds = %2284
  %2302 = icmp ule i64 %2084, %2199
  br label %2311

2303:                                             ; preds = %2284
  %2304 = icmp sgt i64 %2289, %2290
  br label %2311

2305:                                             ; preds = %2284
  %2306 = icmp sge i64 %2289, %2290
  br label %2311

2307:                                             ; preds = %2284
  %2308 = icmp slt i64 %2289, %2290
  br label %2311

2309:                                             ; preds = %2284
  %2310 = icmp sle i64 %2289, %2290
  br label %2311

2311:                                             ; preds = %2309, %2307, %2305, %2303, %2301, %2299, %2297, %2295, %2293, %2291, %2284, %2283, %2281, %2279, %2277, %2275, %2273, %2271, %2267, %2263, %2261, %2259, %2257, %2255, %2253, %2251, %2248, %2246, %2244, %2242, %2240, %2238, %2236, %2234, %2230, %2226, %2224, %2222, %2220, %2218, %2216, %2214, %2209
  %2312 = phi i1 [ %2215, %2214 ], [ %2217, %2216 ], [ %2219, %2218 ], [ %2221, %2220 ], [ %2223, %2222 ], [ %2225, %2224 ], [ %2229, %2226 ], [ %2233, %2230 ], [ %2235, %2234 ], [ %2237, %2236 ], [ %2239, %2238 ], [ %2241, %2240 ], [ %2243, %2242 ], [ %2245, %2244 ], [ true, %2246 ], [ false, %2209 ], [ %2252, %2251 ], [ %2254, %2253 ], [ %2256, %2255 ], [ %2258, %2257 ], [ %2260, %2259 ], [ %2262, %2261 ], [ %2266, %2263 ], [ %2270, %2267 ], [ %2272, %2271 ], [ %2274, %2273 ], [ %2276, %2275 ], [ %2278, %2277 ], [ %2280, %2279 ], [ %2282, %2281 ], [ true, %2283 ], [ false, %2248 ], [ false, %2284 ], [ %2292, %2291 ], [ %2294, %2293 ], [ %2296, %2295 ], [ %2298, %2297 ], [ %2300, %2299 ], [ %2302, %2301 ], [ %2304, %2303 ], [ %2306, %2305 ], [ %2308, %2307 ], [ %2310, %2309 ]
  %2313 = icmp eq i8 %1908, %1915
  br i1 %2313, label %2370, label %2314

2314:                                             ; preds = %2311
  %2315 = zext i1 %2312 to i8
  %2316 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %2317 = add i64 %2316, %1961
  %2318 = inttoptr i64 %2317 to ptr
  %2319 = zext i8 %1916 to i32
  store i8 %2315, ptr %2318, align 1, !tbaa !12
  %2320 = icmp eq i8 %1916, 1
  br i1 %2320, label %2370, label %2321

2321:                                             ; preds = %2314
  %2322 = add nsw i32 %2319, -1
  %2323 = zext i32 %2322 to i64
  %2324 = icmp ult i8 %1916, 9
  br i1 %2324, label %2392, label %2325

2325:                                             ; preds = %2321
  %2326 = icmp ult i8 %1916, 33
  br i1 %2326, label %2378, label %2327

2327:                                             ; preds = %2325
  %2328 = and i64 %2323, 4294967264
  %2329 = getelementptr inbounds nuw i8, ptr %2318, i64 1
  %2330 = getelementptr inbounds nuw i8, ptr %2318, i64 17
  store <16 x i8> zeroinitializer, ptr %2329, align 1, !tbaa !12
  store <16 x i8> zeroinitializer, ptr %2330, align 1, !tbaa !12
  %2331 = icmp eq i64 %2328, 32
  br i1 %2331, label %2355, label %2332, !llvm.loop !41

2332:                                             ; preds = %2327
  %2333 = getelementptr i8, ptr %2318, i64 33
  %2334 = getelementptr i8, ptr %2318, i64 49
  store <16 x i8> zeroinitializer, ptr %2333, align 1, !tbaa !12
  store <16 x i8> zeroinitializer, ptr %2334, align 1, !tbaa !12
  %2335 = icmp eq i64 %2328, 64
  br i1 %2335, label %2355, label %2336, !llvm.loop !41

2336:                                             ; preds = %2332
  %2337 = getelementptr i8, ptr %2318, i64 65
  %2338 = getelementptr i8, ptr %2318, i64 81
  store <16 x i8> zeroinitializer, ptr %2337, align 1, !tbaa !12
  store <16 x i8> zeroinitializer, ptr %2338, align 1, !tbaa !12
  %2339 = icmp eq i64 %2328, 96
  br i1 %2339, label %2355, label %2340, !llvm.loop !41

2340:                                             ; preds = %2336
  %2341 = getelementptr i8, ptr %2318, i64 97
  %2342 = getelementptr i8, ptr %2318, i64 113
  store <16 x i8> zeroinitializer, ptr %2341, align 1, !tbaa !12
  store <16 x i8> zeroinitializer, ptr %2342, align 1, !tbaa !12
  %2343 = icmp eq i64 %2328, 128
  br i1 %2343, label %2355, label %2344, !llvm.loop !41

2344:                                             ; preds = %2340
  %2345 = getelementptr i8, ptr %2318, i64 129
  %2346 = getelementptr i8, ptr %2318, i64 145
  store <16 x i8> zeroinitializer, ptr %2345, align 1, !tbaa !12
  store <16 x i8> zeroinitializer, ptr %2346, align 1, !tbaa !12
  %2347 = icmp eq i64 %2328, 160
  br i1 %2347, label %2355, label %2348, !llvm.loop !41

2348:                                             ; preds = %2344
  %2349 = getelementptr i8, ptr %2318, i64 161
  %2350 = getelementptr i8, ptr %2318, i64 177
  store <16 x i8> zeroinitializer, ptr %2349, align 1, !tbaa !12
  store <16 x i8> zeroinitializer, ptr %2350, align 1, !tbaa !12
  %2351 = icmp eq i64 %2328, 192
  br i1 %2351, label %2355, label %2352, !llvm.loop !41

2352:                                             ; preds = %2348
  %2353 = getelementptr i8, ptr %2318, i64 193
  %2354 = getelementptr i8, ptr %2318, i64 209
  store <16 x i8> zeroinitializer, ptr %2353, align 1, !tbaa !12
  store <16 x i8> zeroinitializer, ptr %2354, align 1, !tbaa !12
  br label %2355

2355:                                             ; preds = %2352, %2348, %2344, %2340, %2336, %2332, %2327
  %2356 = icmp eq i64 %2328, %2323
  br i1 %2356, label %2370, label %2372

2357:                                             ; preds = %403
  br label %2370

2358:                                             ; preds = %567
  br label %2370

2359:                                             ; preds = %970
  br label %2370

2360:                                             ; preds = %1534
  br label %2370

2361:                                             ; preds = %1879
  br label %2370

2362:                                             ; preds = %2395
  br label %2370

2363:                                             ; preds = %2674
  br label %2370

2364:                                             ; preds = %4657
  br label %2370

2365:                                             ; preds = %5254
  br label %2370

2366:                                             ; preds = %6662
  br label %2370

2367:                                             ; preds = %7254
  br label %2370

2368:                                             ; preds = %8019
  br label %2370

2369:                                             ; preds = %8205
  br label %2370

2370:                                             ; preds = %2369, %2368, %2367, %2366, %2365, %2364, %2363, %2362, %2361, %2360, %2359, %2358, %2357, %9924, %9556, %9179, %8815, %8811, %8730, %8719, %8718, %8428, %8426, %8423, %8260, %8258, %8197, %7568, %7247, %6654, %6262, %5661, %5446, %5244, %4650, %4513, %4511, %4220, %4210, %3472, %3029, %2666, %2390, %2355, %2314, %2311, %1864, %1526, %966, %775, %773, %540, %393, %239
  %2371 = phi i8 [ 0, %2355 ], [ 0, %2390 ], [ 0, %540 ], [ 0, %2314 ], [ 0, %8197 ], [ 0, %7568 ], [ 0, %7247 ], [ 0, %6654 ], [ 0, %5244 ], [ 0, %4650 ], [ 0, %2666 ], [ 0, %2311 ], [ 0, %1864 ], [ 0, %1526 ], [ 0, %966 ], [ 0, %393 ], [ 0, %8718 ], [ 1, %8428 ], [ 1, %8426 ], [ 1, %8260 ], [ 1, %8258 ], [ 1, %8423 ], [ 0, %5661 ], [ 0, %5446 ], [ 0, %4513 ], [ 0, %4511 ], [ 0, %4220 ], [ 0, %4210 ], [ 0, %775 ], [ 0, %773 ], [ 1, %3029 ], [ 1, %3472 ], [ %215, %239 ], [ 1, %6262 ], [ 0, %8811 ], [ 0, %8815 ], [ 0, %9179 ], [ 0, %9556 ], [ 0, %9924 ], [ 0, %8730 ], [ 0, %8719 ], [ 0, %2357 ], [ 0, %2358 ], [ 0, %2359 ], [ 0, %2360 ], [ 0, %2361 ], [ 0, %2362 ], [ 0, %2363 ], [ 0, %2364 ], [ 0, %2365 ], [ 0, %2366 ], [ 0, %2367 ], [ 0, %2368 ], [ 0, %2369 ]
  br label %8

2372:                                             ; preds = %2355
  %2373 = trunc nuw i64 %2328 to i32
  %2374 = or disjoint i32 %2373, 1
  %2375 = getelementptr i8, ptr %2318, i64 %2328
  %2376 = and i64 %2323, 24
  %2377 = icmp eq i64 %2376, 0
  br i1 %2377, label %2392, label %2378

2378:                                             ; preds = %2372, %2325
  %2379 = phi i64 [ %2328, %2372 ], [ 0, %2325 ]
  %2380 = and i64 %2323, 4294967288
  %2381 = trunc nuw i64 %2380 to i32
  %2382 = or disjoint i32 %2381, 1
  %2383 = getelementptr i8, ptr %2318, i64 %2380
  %2384 = getelementptr i8, ptr %2318, i64 1
  br label %2385

2385:                                             ; preds = %2385, %2378
  %2386 = phi i64 [ %2379, %2378 ], [ %2388, %2385 ]
  %2387 = getelementptr i8, ptr %2384, i64 %2386
  store <8 x i8> zeroinitializer, ptr %2387, align 1, !tbaa !12
  %2388 = add nuw i64 %2386, 8
  %2389 = icmp eq i64 %2388, %2380
  br i1 %2389, label %2390, label %2385, !llvm.loop !42

2390:                                             ; preds = %2385
  %2391 = icmp eq i64 %2380, %2323
  br i1 %2391, label %2370, label %2392

2392:                                             ; preds = %2390, %2372, %2321
  %2393 = phi i32 [ 1, %2321 ], [ %2374, %2372 ], [ %2382, %2390 ]
  %2394 = phi ptr [ %2318, %2321 ], [ %2375, %2372 ], [ %2383, %2390 ]
  br label %2395

2395:                                             ; preds = %2395, %2392
  %2396 = phi i32 [ %2399, %2395 ], [ %2393, %2392 ]
  %2397 = phi ptr [ %2398, %2395 ], [ %2394, %2392 ]
  %2398 = getelementptr inbounds nuw i8, ptr %2397, i64 1
  store i8 0, ptr %2398, align 1, !tbaa !12
  %2399 = add nuw nsw i32 %2396, 1
  %2400 = icmp eq i32 %2399, %2319
  br i1 %2400, label %2362, label %2395, !llvm.loop !43

2401:                                             ; preds = %239
  %2402 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %2403 = inttoptr i64 %2402 to ptr
  %2404 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2405 = add nsw i32 %2404, 1
  store i32 %2405, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2406 = sext i32 %2404 to i64
  %2407 = getelementptr inbounds i8, ptr %2403, i64 %2406
  %2408 = load i8, ptr %2407, align 1, !tbaa !12
  %2409 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2410 = shl i32 %2409, 13
  %2411 = xor i32 %2410, %2409
  %2412 = lshr i32 %2411, 17
  %2413 = xor i32 %2412, %2411
  %2414 = shl i32 %2413, 5
  %2415 = xor i32 %2414, %2413
  store i32 %2415, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2416 = trunc i32 %2415 to i8
  %2417 = xor i8 %2408, %2416
  %2418 = add nsw i32 %2404, 2
  store i32 %2418, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2419 = sext i32 %2405 to i64
  %2420 = getelementptr inbounds i8, ptr %2403, i64 %2419
  %2421 = load i8, ptr %2420, align 1, !tbaa !12
  %2422 = shl i32 %2415, 13
  %2423 = xor i32 %2422, %2415
  %2424 = lshr i32 %2423, 17
  %2425 = xor i32 %2424, %2423
  %2426 = shl i32 %2425, 5
  %2427 = xor i32 %2426, %2425
  store i32 %2427, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2428 = trunc i32 %2427 to i8
  %2429 = xor i8 %2421, %2428
  %2430 = add nsw i32 %2404, 3
  store i32 %2430, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2431 = sext i32 %2418 to i64
  %2432 = getelementptr inbounds i8, ptr %2403, i64 %2431
  %2433 = load i8, ptr %2432, align 1, !tbaa !12
  %2434 = shl i32 %2427, 13
  %2435 = xor i32 %2434, %2427
  %2436 = lshr i32 %2435, 17
  %2437 = xor i32 %2436, %2435
  %2438 = shl i32 %2437, 5
  %2439 = xor i32 %2438, %2437
  %2440 = trunc i32 %2439 to i8
  %2441 = xor i8 %2433, %2440
  %2442 = add nsw i32 %2404, 4
  %2443 = shl i32 %2439, 13
  %2444 = xor i32 %2443, %2439
  %2445 = lshr i32 %2444, 17
  %2446 = xor i32 %2445, %2444
  %2447 = shl i32 %2446, 5
  %2448 = xor i32 %2447, %2446
  store i32 %2448, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2449 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %2450 = icmp sgt i32 %2449, 0
  br i1 %2450, label %2451, label %2483

2451:                                             ; preds = %2401
  %2452 = sext i32 %2442 to i64
  %2453 = zext nneg i32 %2449 to i64
  br label %2454

2454:                                             ; preds = %2454, %2451
  %2455 = phi i64 [ %2452, %2451 ], [ %2459, %2454 ]
  %2456 = phi i64 [ 0, %2451 ], [ %2477, %2454 ]
  %2457 = phi i64 [ 0, %2451 ], [ %2476, %2454 ]
  %2458 = phi i32 [ %2448, %2451 ], [ %2468, %2454 ]
  %2459 = add nsw i64 %2455, 1
  %2460 = trunc nsw i64 %2459 to i32
  store i32 %2460, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2461 = getelementptr inbounds i8, ptr %2403, i64 %2455
  %2462 = load i8, ptr %2461, align 1, !tbaa !12
  %2463 = shl i32 %2458, 13
  %2464 = xor i32 %2463, %2458
  %2465 = lshr i32 %2464, 17
  %2466 = xor i32 %2465, %2464
  %2467 = shl i32 %2466, 5
  %2468 = xor i32 %2467, %2466
  store i32 %2468, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2469 = trunc i32 %2468 to i8
  %2470 = xor i8 %2462, %2469
  %2471 = icmp samesign ult i64 %2456, 8
  %2472 = zext i8 %2470 to i64
  %2473 = shl nuw nsw i64 %2456, 3
  %2474 = shl nuw i64 %2472, %2473
  %2475 = select i1 %2471, i64 %2474, i64 0
  %2476 = or i64 %2475, %2457
  %2477 = add nuw nsw i64 %2456, 1
  %2478 = icmp eq i64 %2477, %2453
  br i1 %2478, label %2479, label %2454, !llvm.loop !33

2479:                                             ; preds = %2454
  %2480 = phi i32 [ %2460, %2454 ]
  %2481 = phi i32 [ %2468, %2454 ]
  %2482 = phi i64 [ %2476, %2454 ]
  br label %2483

2483:                                             ; preds = %2479, %2401
  %2484 = phi i32 [ %2448, %2401 ], [ %2481, %2479 ]
  %2485 = phi i32 [ %2442, %2401 ], [ %2480, %2479 ]
  %2486 = phi i64 [ 0, %2401 ], [ %2482, %2479 ]
  %2487 = add nsw i32 %2485, 1
  store i32 %2487, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2488 = sext i32 %2485 to i64
  %2489 = getelementptr inbounds i8, ptr %2403, i64 %2488
  %2490 = load i8, ptr %2489, align 1, !tbaa !12
  %2491 = shl i32 %2484, 13
  %2492 = xor i32 %2491, %2484
  %2493 = lshr i32 %2492, 17
  %2494 = xor i32 %2493, %2492
  %2495 = shl i32 %2494, 5
  %2496 = xor i32 %2495, %2494
  store i32 %2496, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2497 = trunc i32 %2496 to i8
  %2498 = xor i8 %2490, %2497
  %2499 = add nsw i32 %2485, 2
  store i32 %2499, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2500 = sext i32 %2487 to i64
  %2501 = getelementptr inbounds i8, ptr %2403, i64 %2500
  %2502 = load i8, ptr %2501, align 1, !tbaa !12
  %2503 = shl i32 %2496, 13
  %2504 = xor i32 %2503, %2496
  %2505 = lshr i32 %2504, 17
  %2506 = xor i32 %2505, %2504
  %2507 = shl i32 %2506, 5
  %2508 = xor i32 %2507, %2506
  store i32 %2508, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2509 = trunc i32 %2508 to i8
  %2510 = icmp eq i8 %2502, %2509
  br i1 %2510, label %2511, label %2566

2511:                                             ; preds = %2483
  br i1 %2450, label %2512, label %2542

2512:                                             ; preds = %2511
  %2513 = sext i32 %2499 to i64
  %2514 = zext nneg i32 %2449 to i64
  br label %2515

2515:                                             ; preds = %2515, %2512
  %2516 = phi i64 [ %2513, %2512 ], [ %2520, %2515 ]
  %2517 = phi i64 [ 0, %2512 ], [ %2538, %2515 ]
  %2518 = phi i64 [ 0, %2512 ], [ %2537, %2515 ]
  %2519 = phi i32 [ %2508, %2512 ], [ %2529, %2515 ]
  %2520 = add nsw i64 %2516, 1
  %2521 = trunc nsw i64 %2520 to i32
  store i32 %2521, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2522 = getelementptr inbounds i8, ptr %2403, i64 %2516
  %2523 = load i8, ptr %2522, align 1, !tbaa !12
  %2524 = shl i32 %2519, 13
  %2525 = xor i32 %2524, %2519
  %2526 = lshr i32 %2525, 17
  %2527 = xor i32 %2526, %2525
  %2528 = shl i32 %2527, 5
  %2529 = xor i32 %2528, %2527
  store i32 %2529, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2530 = trunc i32 %2529 to i8
  %2531 = xor i8 %2523, %2530
  %2532 = icmp samesign ult i64 %2517, 8
  %2533 = zext i8 %2531 to i64
  %2534 = shl nuw nsw i64 %2517, 3
  %2535 = shl nuw i64 %2533, %2534
  %2536 = select i1 %2532, i64 %2535, i64 0
  %2537 = or i64 %2536, %2518
  %2538 = add nuw nsw i64 %2517, 1
  %2539 = icmp eq i64 %2538, %2514
  br i1 %2539, label %2540, label %2515, !llvm.loop !33

2540:                                             ; preds = %2515
  %2541 = phi i64 [ %2537, %2515 ]
  br label %2542

2542:                                             ; preds = %2540, %2511
  %2543 = phi i64 [ 0, %2511 ], [ %2541, %2540 ]
  %2544 = icmp eq i8 %2490, %2497
  br i1 %2544, label %2600, label %2545

2545:                                             ; preds = %2542
  %2546 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %2547 = inttoptr i64 %2546 to ptr
  %2548 = zext i8 %2498 to i64
  br label %2549

2549:                                             ; preds = %2562, %2545
  %2550 = phi i64 [ 0, %2545 ], [ %2564, %2562 ]
  %2551 = phi i64 [ %2543, %2545 ], [ %2553, %2562 ]
  %2552 = phi i64 [ 0, %2545 ], [ %2563, %2562 ]
  %2553 = add i64 %2551, 1
  %2554 = icmp samesign ult i64 %2550, 8
  br i1 %2554, label %2555, label %2562

2555:                                             ; preds = %2549
  %2556 = getelementptr inbounds nuw i8, ptr %2547, i64 %2551
  %2557 = load i8, ptr %2556, align 1, !tbaa !12
  %2558 = zext i8 %2557 to i64
  %2559 = shl nuw nsw i64 %2550, 3
  %2560 = shl nuw i64 %2558, %2559
  %2561 = or i64 %2560, %2552
  br label %2562

2562:                                             ; preds = %2555, %2549
  %2563 = phi i64 [ %2561, %2555 ], [ %2552, %2549 ]
  %2564 = add nuw nsw i64 %2550, 1
  %2565 = icmp eq i64 %2564, %2548
  br i1 %2565, label %2596, label %2549, !llvm.loop !35

2566:                                             ; preds = %2483
  %2567 = icmp eq i8 %2490, %2497
  br i1 %2567, label %2600, label %2568

2568:                                             ; preds = %2566
  %2569 = sext i32 %2499 to i64
  %2570 = zext i8 %2498 to i64
  br label %2571

2571:                                             ; preds = %2571, %2568
  %2572 = phi i64 [ %2569, %2568 ], [ %2576, %2571 ]
  %2573 = phi i64 [ 0, %2568 ], [ %2594, %2571 ]
  %2574 = phi i64 [ 0, %2568 ], [ %2593, %2571 ]
  %2575 = phi i32 [ %2508, %2568 ], [ %2585, %2571 ]
  %2576 = add nsw i64 %2572, 1
  %2577 = trunc nsw i64 %2576 to i32
  store i32 %2577, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2578 = getelementptr inbounds i8, ptr %2403, i64 %2572
  %2579 = load i8, ptr %2578, align 1, !tbaa !12
  %2580 = shl i32 %2575, 13
  %2581 = xor i32 %2580, %2575
  %2582 = lshr i32 %2581, 17
  %2583 = xor i32 %2582, %2581
  %2584 = shl i32 %2583, 5
  %2585 = xor i32 %2584, %2583
  store i32 %2585, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2586 = trunc i32 %2585 to i8
  %2587 = xor i8 %2579, %2586
  %2588 = icmp samesign ult i64 %2573, 8
  %2589 = zext i8 %2587 to i64
  %2590 = shl nuw nsw i64 %2573, 3
  %2591 = shl nuw i64 %2589, %2590
  %2592 = select i1 %2588, i64 %2591, i64 0
  %2593 = or i64 %2592, %2574
  %2594 = add nuw nsw i64 %2573, 1
  %2595 = icmp eq i64 %2594, %2570
  br i1 %2595, label %2598, label %2571, !llvm.loop !33

2596:                                             ; preds = %2562
  %2597 = phi i64 [ %2563, %2562 ]
  br label %2600

2598:                                             ; preds = %2571
  %2599 = phi i64 [ %2593, %2571 ]
  br label %2600

2600:                                             ; preds = %2598, %2596, %2566, %2542
  %2601 = phi i64 [ 0, %2542 ], [ 0, %2566 ], [ %2597, %2596 ], [ %2599, %2598 ]
  switch i8 %2417, label %2666 [
    i8 0, label %2602
    i8 8, label %2658
    i8 2, label %2608
    i8 3, label %2618
    i8 4, label %2625
    i8 5, label %2632
    i8 6, label %2641
    i8 7, label %2649
  ]

2602:                                             ; preds = %2600
  %2603 = zext i8 %2441 to i64
  %2604 = shl nuw nsw i64 %2603, 3
  %2605 = shl nsw i64 -1, %2604
  %2606 = xor i64 %2605, -1
  %2607 = and i64 %2601, %2606
  br label %2666

2608:                                             ; preds = %2600
  switch i8 %2429, label %2666 [
    i8 1, label %2609
    i8 2, label %2612
    i8 4, label %2615
  ]

2609:                                             ; preds = %2608
  %2610 = shl i64 %2601, 56
  %2611 = ashr exact i64 %2610, 56
  br label %2666

2612:                                             ; preds = %2608
  %2613 = shl i64 %2601, 48
  %2614 = ashr exact i64 %2613, 48
  br label %2666

2615:                                             ; preds = %2608
  %2616 = shl i64 %2601, 32
  %2617 = ashr exact i64 %2616, 32
  br label %2666

2618:                                             ; preds = %2600
  %2619 = icmp eq i8 %2429, 8
  br i1 %2619, label %2620, label %2666

2620:                                             ; preds = %2618
  %2621 = bitcast i64 %2601 to double
  %2622 = fptrunc double %2621 to float
  %2623 = bitcast float %2622 to i32
  %2624 = zext i32 %2623 to i64
  br label %2666

2625:                                             ; preds = %2600
  %2626 = icmp eq i8 %2429, 4
  br i1 %2626, label %2627, label %2666

2627:                                             ; preds = %2625
  %2628 = trunc i64 %2601 to i32
  %2629 = bitcast i32 %2628 to float
  %2630 = fpext float %2629 to double
  %2631 = bitcast double %2630 to i64
  br label %2666

2632:                                             ; preds = %2600
  %2633 = icmp eq i8 %2429, 4
  br i1 %2633, label %2634, label %2638

2634:                                             ; preds = %2632
  %2635 = trunc i64 %2601 to i32
  %2636 = bitcast i32 %2635 to float
  %2637 = fptoui float %2636 to i64
  br label %2666

2638:                                             ; preds = %2632
  %2639 = bitcast i64 %2601 to double
  %2640 = fptoui double %2639 to i64
  br label %2666

2641:                                             ; preds = %2600
  %2642 = icmp eq i8 %2429, 4
  %2643 = trunc i64 %2601 to i32
  %2644 = bitcast i32 %2643 to float
  %2645 = fptosi float %2644 to i64
  %2646 = bitcast i64 %2601 to double
  %2647 = fptosi double %2646 to i64
  %2648 = select i1 %2642, i64 %2645, i64 %2647
  br label %2666

2649:                                             ; preds = %2600
  %2650 = icmp ult i8 %2441, 5
  br i1 %2650, label %2651, label %2655

2651:                                             ; preds = %2649
  %2652 = uitofp i64 %2601 to float
  %2653 = bitcast float %2652 to i32
  %2654 = zext nneg i32 %2653 to i64
  br label %2666

2655:                                             ; preds = %2649
  %2656 = uitofp i64 %2601 to double
  %2657 = bitcast double %2656 to i64
  br label %2666

2658:                                             ; preds = %2600
  %2659 = icmp ult i8 %2441, 5
  %2660 = sitofp i64 %2601 to float
  %2661 = bitcast float %2660 to i32
  %2662 = zext i32 %2661 to i64
  %2663 = sitofp i64 %2601 to double
  %2664 = bitcast double %2663 to i64
  %2665 = select i1 %2659, i64 %2662, i64 %2664
  br label %2666

2666:                                             ; preds = %2658, %2655, %2651, %2641, %2638, %2634, %2627, %2625, %2620, %2618, %2615, %2612, %2609, %2608, %2602, %2600
  %2667 = phi i64 [ %2607, %2602 ], [ %2624, %2620 ], [ %2631, %2627 ], [ %2637, %2634 ], [ %2640, %2638 ], [ %2648, %2641 ], [ %2654, %2651 ], [ %2657, %2655 ], [ %2665, %2658 ], [ %2611, %2609 ], [ %2614, %2612 ], [ %2617, %2615 ], [ %2601, %2608 ], [ %2601, %2618 ], [ %2601, %2625 ], [ %2601, %2600 ]
  %2668 = icmp eq i8 %2433, %2440
  br i1 %2668, label %2370, label %2669

2669:                                             ; preds = %2666
  %2670 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %2671 = add i64 %2670, %2486
  %2672 = inttoptr i64 %2671 to ptr
  %2673 = zext i8 %2441 to i32
  br label %2674

2674:                                             ; preds = %2674, %2669
  %2675 = phi i32 [ %2681, %2674 ], [ 0, %2669 ]
  %2676 = phi ptr [ %2679, %2674 ], [ %2672, %2669 ]
  %2677 = phi i64 [ %2680, %2674 ], [ %2667, %2669 ]
  %2678 = trunc i64 %2677 to i8
  store i8 %2678, ptr %2676, align 1, !tbaa !12
  %2679 = getelementptr inbounds nuw i8, ptr %2676, i64 1
  %2680 = lshr i64 %2677, 8
  %2681 = add nuw nsw i32 %2675, 1
  %2682 = icmp eq i32 %2681, %2673
  br i1 %2682, label %2363, label %2674, !llvm.loop !34

2683:                                             ; preds = %239
  %2684 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %2685 = inttoptr i64 %2684 to ptr
  %2686 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2687 = add nsw i32 %2686, 1
  store i32 %2687, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2688 = sext i32 %2686 to i64
  %2689 = getelementptr inbounds i8, ptr %2685, i64 %2688
  %2690 = load i8, ptr %2689, align 1, !tbaa !12
  %2691 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2692 = shl i32 %2691, 13
  %2693 = xor i32 %2692, %2691
  %2694 = lshr i32 %2693, 17
  %2695 = xor i32 %2694, %2693
  %2696 = shl i32 %2695, 5
  %2697 = xor i32 %2696, %2695
  store i32 %2697, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2698 = trunc i32 %2697 to i8
  %2699 = xor i8 %2690, %2698
  %2700 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %2701 = icmp sgt i32 %2700, 0
  br i1 %2701, label %2702, label %2734

2702:                                             ; preds = %2683
  %2703 = sext i32 %2687 to i64
  %2704 = zext nneg i32 %2700 to i64
  br label %2705

2705:                                             ; preds = %2705, %2702
  %2706 = phi i64 [ %2703, %2702 ], [ %2710, %2705 ]
  %2707 = phi i64 [ 0, %2702 ], [ %2728, %2705 ]
  %2708 = phi i64 [ 0, %2702 ], [ %2727, %2705 ]
  %2709 = phi i32 [ %2697, %2702 ], [ %2719, %2705 ]
  %2710 = add nsw i64 %2706, 1
  %2711 = trunc nsw i64 %2710 to i32
  store i32 %2711, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2712 = getelementptr inbounds i8, ptr %2685, i64 %2706
  %2713 = load i8, ptr %2712, align 1, !tbaa !12
  %2714 = shl i32 %2709, 13
  %2715 = xor i32 %2714, %2709
  %2716 = lshr i32 %2715, 17
  %2717 = xor i32 %2716, %2715
  %2718 = shl i32 %2717, 5
  %2719 = xor i32 %2718, %2717
  store i32 %2719, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2720 = trunc i32 %2719 to i8
  %2721 = xor i8 %2713, %2720
  %2722 = icmp samesign ult i64 %2707, 8
  %2723 = zext i8 %2721 to i64
  %2724 = shl nuw nsw i64 %2707, 3
  %2725 = shl nuw i64 %2723, %2724
  %2726 = select i1 %2722, i64 %2725, i64 0
  %2727 = or i64 %2726, %2708
  %2728 = add nuw nsw i64 %2707, 1
  %2729 = icmp eq i64 %2728, %2704
  br i1 %2729, label %2730, label %2705, !llvm.loop !33

2730:                                             ; preds = %2705
  %2731 = phi i32 [ %2711, %2705 ]
  %2732 = phi i32 [ %2719, %2705 ]
  %2733 = phi i64 [ %2727, %2705 ]
  br label %2734

2734:                                             ; preds = %2730, %2683
  %2735 = phi i32 [ %2697, %2683 ], [ %2732, %2730 ]
  %2736 = phi i32 [ %2687, %2683 ], [ %2731, %2730 ]
  %2737 = phi i64 [ 0, %2683 ], [ %2733, %2730 ]
  store i64 %2737, ptr @last_br_from_bb_id__Z8vm_entryv.2, align 8, !tbaa !6
  switch i8 %2699, label %2836 [
    i8 0, label %2742
    i8 2, label %2738
  ]

2738:                                             ; preds = %2734
  br i1 %2701, label %2739, label %2830

2739:                                             ; preds = %2738
  %2740 = sext i32 %2736 to i64
  %2741 = zext nneg i32 %2700 to i64
  br label %2778

2742:                                             ; preds = %2734
  br i1 %2701, label %2743, label %3029

2743:                                             ; preds = %2742
  %2744 = sext i32 %2736 to i64
  %2745 = zext nneg i32 %2700 to i64
  br label %2746

2746:                                             ; preds = %2746, %2743
  %2747 = phi i64 [ %2744, %2743 ], [ %2751, %2746 ]
  %2748 = phi i64 [ 0, %2743 ], [ %2769, %2746 ]
  %2749 = phi i64 [ 0, %2743 ], [ %2768, %2746 ]
  %2750 = phi i32 [ %2735, %2743 ], [ %2760, %2746 ]
  %2751 = add nsw i64 %2747, 1
  %2752 = trunc nsw i64 %2751 to i32
  store i32 %2752, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2753 = getelementptr inbounds i8, ptr %2685, i64 %2747
  %2754 = load i8, ptr %2753, align 1, !tbaa !12
  %2755 = shl i32 %2750, 13
  %2756 = xor i32 %2755, %2750
  %2757 = lshr i32 %2756, 17
  %2758 = xor i32 %2757, %2756
  %2759 = shl i32 %2758, 5
  %2760 = xor i32 %2759, %2758
  store i32 %2760, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2761 = trunc i32 %2760 to i8
  %2762 = xor i8 %2754, %2761
  %2763 = icmp samesign ult i64 %2748, 8
  %2764 = zext i8 %2762 to i64
  %2765 = shl nuw nsw i64 %2748, 3
  %2766 = shl nuw i64 %2764, %2765
  %2767 = select i1 %2763, i64 %2766, i64 0
  %2768 = or i64 %2767, %2749
  %2769 = add nuw nsw i64 %2748, 1
  %2770 = icmp eq i64 %2769, %2745
  br i1 %2770, label %3027, label %2746, !llvm.loop !33

2771:                                             ; preds = %2778
  %2772 = phi i64 [ %2783, %2778 ]
  %2773 = phi i32 [ %2792, %2778 ]
  %2774 = phi i64 [ %2800, %2778 ]
  %2775 = shl i64 %2772, 32
  %2776 = ashr exact i64 %2775, 32
  %2777 = zext nneg i32 %2700 to i64
  br label %2803

2778:                                             ; preds = %2778, %2739
  %2779 = phi i64 [ %2740, %2739 ], [ %2783, %2778 ]
  %2780 = phi i64 [ 0, %2739 ], [ %2801, %2778 ]
  %2781 = phi i64 [ 0, %2739 ], [ %2800, %2778 ]
  %2782 = phi i32 [ %2735, %2739 ], [ %2792, %2778 ]
  %2783 = add nsw i64 %2779, 1
  %2784 = trunc nsw i64 %2783 to i32
  store i32 %2784, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2785 = getelementptr inbounds i8, ptr %2685, i64 %2779
  %2786 = load i8, ptr %2785, align 1, !tbaa !12
  %2787 = shl i32 %2782, 13
  %2788 = xor i32 %2787, %2782
  %2789 = lshr i32 %2788, 17
  %2790 = xor i32 %2789, %2788
  %2791 = shl i32 %2790, 5
  %2792 = xor i32 %2791, %2790
  store i32 %2792, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2793 = trunc i32 %2792 to i8
  %2794 = xor i8 %2786, %2793
  %2795 = icmp samesign ult i64 %2780, 8
  %2796 = zext i8 %2794 to i64
  %2797 = shl nuw nsw i64 %2780, 3
  %2798 = shl nuw i64 %2796, %2797
  %2799 = select i1 %2795, i64 %2798, i64 0
  %2800 = or i64 %2799, %2781
  %2801 = add nuw nsw i64 %2780, 1
  %2802 = icmp eq i64 %2801, %2741
  br i1 %2802, label %2771, label %2778, !llvm.loop !33

2803:                                             ; preds = %2803, %2771
  %2804 = phi i64 [ %2776, %2771 ], [ %2808, %2803 ]
  %2805 = phi i64 [ 0, %2771 ], [ %2826, %2803 ]
  %2806 = phi i64 [ 0, %2771 ], [ %2825, %2803 ]
  %2807 = phi i32 [ %2773, %2771 ], [ %2817, %2803 ]
  %2808 = add nsw i64 %2804, 1
  %2809 = trunc nsw i64 %2808 to i32
  store i32 %2809, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2810 = getelementptr inbounds i8, ptr %2685, i64 %2804
  %2811 = load i8, ptr %2810, align 1, !tbaa !12
  %2812 = shl i32 %2807, 13
  %2813 = xor i32 %2812, %2807
  %2814 = lshr i32 %2813, 17
  %2815 = xor i32 %2814, %2813
  %2816 = shl i32 %2815, 5
  %2817 = xor i32 %2816, %2815
  store i32 %2817, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2818 = trunc i32 %2817 to i8
  %2819 = xor i8 %2811, %2818
  %2820 = icmp samesign ult i64 %2805, 8
  %2821 = zext i8 %2819 to i64
  %2822 = shl nuw nsw i64 %2805, 3
  %2823 = shl nuw i64 %2821, %2822
  %2824 = select i1 %2820, i64 %2823, i64 0
  %2825 = or i64 %2824, %2806
  %2826 = add nuw nsw i64 %2805, 1
  %2827 = icmp eq i64 %2826, %2777
  br i1 %2827, label %2828, label %2803, !llvm.loop !33

2828:                                             ; preds = %2803
  %2829 = phi i64 [ %2825, %2803 ]
  br label %2830

2830:                                             ; preds = %2828, %2738
  %2831 = phi i64 [ 0, %2738 ], [ %2774, %2828 ]
  %2832 = phi i64 [ 0, %2738 ], [ %2829, %2828 ]
  %2833 = load i8, ptr @exception_thrown__Z8vm_entryv, align 4, !tbaa !12
  %2834 = icmp ne i8 %2833, 0
  %2835 = select i1 %2834, i64 %2831, i64 %2832
  br label %3029

2836:                                             ; preds = %2734
  %2837 = add nsw i32 %2736, 1
  store i32 %2837, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2838 = sext i32 %2736 to i64
  %2839 = getelementptr inbounds i8, ptr %2685, i64 %2838
  %2840 = load i8, ptr %2839, align 1, !tbaa !12
  %2841 = shl i32 %2735, 13
  %2842 = xor i32 %2841, %2735
  %2843 = lshr i32 %2842, 17
  %2844 = xor i32 %2843, %2842
  %2845 = shl i32 %2844, 5
  %2846 = xor i32 %2845, %2844
  store i32 %2846, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2847 = trunc i32 %2846 to i8
  %2848 = xor i8 %2840, %2847
  %2849 = add nsw i32 %2736, 2
  store i32 %2849, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2850 = sext i32 %2837 to i64
  %2851 = getelementptr inbounds i8, ptr %2685, i64 %2850
  %2852 = load i8, ptr %2851, align 1, !tbaa !12
  %2853 = shl i32 %2846, 13
  %2854 = xor i32 %2853, %2846
  %2855 = lshr i32 %2854, 17
  %2856 = xor i32 %2855, %2854
  %2857 = shl i32 %2856, 5
  %2858 = xor i32 %2857, %2856
  store i32 %2858, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2859 = trunc i32 %2858 to i8
  %2860 = icmp eq i8 %2852, %2859
  br i1 %2860, label %2861, label %2920

2861:                                             ; preds = %2836
  br i1 %2701, label %2862, label %2894

2862:                                             ; preds = %2861
  %2863 = sext i32 %2849 to i64
  %2864 = zext nneg i32 %2700 to i64
  br label %2865

2865:                                             ; preds = %2865, %2862
  %2866 = phi i64 [ %2863, %2862 ], [ %2870, %2865 ]
  %2867 = phi i64 [ 0, %2862 ], [ %2888, %2865 ]
  %2868 = phi i64 [ 0, %2862 ], [ %2887, %2865 ]
  %2869 = phi i32 [ %2858, %2862 ], [ %2879, %2865 ]
  %2870 = add nsw i64 %2866, 1
  %2871 = trunc nsw i64 %2870 to i32
  store i32 %2871, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2872 = getelementptr inbounds i8, ptr %2685, i64 %2866
  %2873 = load i8, ptr %2872, align 1, !tbaa !12
  %2874 = shl i32 %2869, 13
  %2875 = xor i32 %2874, %2869
  %2876 = lshr i32 %2875, 17
  %2877 = xor i32 %2876, %2875
  %2878 = shl i32 %2877, 5
  %2879 = xor i32 %2878, %2877
  store i32 %2879, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2880 = trunc i32 %2879 to i8
  %2881 = xor i8 %2873, %2880
  %2882 = icmp samesign ult i64 %2867, 8
  %2883 = zext i8 %2881 to i64
  %2884 = shl nuw nsw i64 %2867, 3
  %2885 = shl nuw i64 %2883, %2884
  %2886 = select i1 %2882, i64 %2885, i64 0
  %2887 = or i64 %2886, %2868
  %2888 = add nuw nsw i64 %2867, 1
  %2889 = icmp eq i64 %2888, %2864
  br i1 %2889, label %2890, label %2865, !llvm.loop !33

2890:                                             ; preds = %2865
  %2891 = phi i32 [ %2871, %2865 ]
  %2892 = phi i32 [ %2879, %2865 ]
  %2893 = phi i64 [ %2887, %2865 ]
  br label %2894

2894:                                             ; preds = %2890, %2861
  %2895 = phi i32 [ %2858, %2861 ], [ %2892, %2890 ]
  %2896 = phi i32 [ %2849, %2861 ], [ %2891, %2890 ]
  %2897 = phi i64 [ 0, %2861 ], [ %2893, %2890 ]
  %2898 = icmp eq i8 %2840, %2847
  br i1 %2898, label %2956, label %2899

2899:                                             ; preds = %2894
  %2900 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %2901 = inttoptr i64 %2900 to ptr
  %2902 = zext i8 %2848 to i64
  br label %2903

2903:                                             ; preds = %2916, %2899
  %2904 = phi i64 [ 0, %2899 ], [ %2918, %2916 ]
  %2905 = phi i64 [ %2897, %2899 ], [ %2907, %2916 ]
  %2906 = phi i64 [ 0, %2899 ], [ %2917, %2916 ]
  %2907 = add i64 %2905, 1
  %2908 = icmp samesign ult i64 %2904, 8
  br i1 %2908, label %2909, label %2916

2909:                                             ; preds = %2903
  %2910 = getelementptr inbounds nuw i8, ptr %2901, i64 %2905
  %2911 = load i8, ptr %2910, align 1, !tbaa !12
  %2912 = zext i8 %2911 to i64
  %2913 = shl nuw nsw i64 %2904, 3
  %2914 = shl nuw i64 %2912, %2913
  %2915 = or i64 %2914, %2906
  br label %2916

2916:                                             ; preds = %2909, %2903
  %2917 = phi i64 [ %2915, %2909 ], [ %2906, %2903 ]
  %2918 = add nuw nsw i64 %2904, 1
  %2919 = icmp eq i64 %2918, %2902
  br i1 %2919, label %2950, label %2903, !llvm.loop !35

2920:                                             ; preds = %2836
  %2921 = icmp eq i8 %2840, %2847
  br i1 %2921, label %2956, label %2922

2922:                                             ; preds = %2920
  %2923 = sext i32 %2849 to i64
  %2924 = zext i8 %2848 to i64
  br label %2925

2925:                                             ; preds = %2925, %2922
  %2926 = phi i64 [ %2923, %2922 ], [ %2930, %2925 ]
  %2927 = phi i64 [ 0, %2922 ], [ %2948, %2925 ]
  %2928 = phi i64 [ 0, %2922 ], [ %2947, %2925 ]
  %2929 = phi i32 [ %2858, %2922 ], [ %2939, %2925 ]
  %2930 = add nsw i64 %2926, 1
  %2931 = trunc nsw i64 %2930 to i32
  store i32 %2931, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2932 = getelementptr inbounds i8, ptr %2685, i64 %2926
  %2933 = load i8, ptr %2932, align 1, !tbaa !12
  %2934 = shl i32 %2929, 13
  %2935 = xor i32 %2934, %2929
  %2936 = lshr i32 %2935, 17
  %2937 = xor i32 %2936, %2935
  %2938 = shl i32 %2937, 5
  %2939 = xor i32 %2938, %2937
  store i32 %2939, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2940 = trunc i32 %2939 to i8
  %2941 = xor i8 %2933, %2940
  %2942 = icmp samesign ult i64 %2927, 8
  %2943 = zext i8 %2941 to i64
  %2944 = shl nuw nsw i64 %2927, 3
  %2945 = shl nuw i64 %2943, %2944
  %2946 = select i1 %2942, i64 %2945, i64 0
  %2947 = or i64 %2946, %2928
  %2948 = add nuw nsw i64 %2927, 1
  %2949 = icmp eq i64 %2948, %2924
  br i1 %2949, label %2952, label %2925, !llvm.loop !33

2950:                                             ; preds = %2916
  %2951 = phi i64 [ %2917, %2916 ]
  br label %2956

2952:                                             ; preds = %2925
  %2953 = phi i32 [ %2931, %2925 ]
  %2954 = phi i32 [ %2939, %2925 ]
  %2955 = phi i64 [ %2947, %2925 ]
  br label %2956

2956:                                             ; preds = %2952, %2950, %2920, %2894
  %2957 = phi i32 [ %2895, %2894 ], [ %2858, %2920 ], [ %2895, %2950 ], [ %2954, %2952 ]
  %2958 = phi i32 [ %2896, %2894 ], [ %2849, %2920 ], [ %2896, %2950 ], [ %2953, %2952 ]
  %2959 = phi i64 [ 0, %2894 ], [ 0, %2920 ], [ %2951, %2950 ], [ %2955, %2952 ]
  br i1 %2701, label %2960, label %3022

2960:                                             ; preds = %2956
  %2961 = sext i32 %2958 to i64
  %2962 = zext nneg i32 %2700 to i64
  br label %2970

2963:                                             ; preds = %2970
  %2964 = phi i64 [ %2975, %2970 ]
  %2965 = phi i32 [ %2984, %2970 ]
  %2966 = phi i64 [ %2992, %2970 ]
  %2967 = shl i64 %2964, 32
  %2968 = ashr exact i64 %2967, 32
  %2969 = zext nneg i32 %2700 to i64
  br label %2995

2970:                                             ; preds = %2970, %2960
  %2971 = phi i64 [ %2961, %2960 ], [ %2975, %2970 ]
  %2972 = phi i64 [ 0, %2960 ], [ %2993, %2970 ]
  %2973 = phi i64 [ 0, %2960 ], [ %2992, %2970 ]
  %2974 = phi i32 [ %2957, %2960 ], [ %2984, %2970 ]
  %2975 = add nsw i64 %2971, 1
  %2976 = trunc nsw i64 %2975 to i32
  store i32 %2976, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %2977 = getelementptr inbounds i8, ptr %2685, i64 %2971
  %2978 = load i8, ptr %2977, align 1, !tbaa !12
  %2979 = shl i32 %2974, 13
  %2980 = xor i32 %2979, %2974
  %2981 = lshr i32 %2980, 17
  %2982 = xor i32 %2981, %2980
  %2983 = shl i32 %2982, 5
  %2984 = xor i32 %2983, %2982
  store i32 %2984, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %2985 = trunc i32 %2984 to i8
  %2986 = xor i8 %2978, %2985
  %2987 = icmp samesign ult i64 %2972, 8
  %2988 = zext i8 %2986 to i64
  %2989 = shl nuw nsw i64 %2972, 3
  %2990 = shl nuw i64 %2988, %2989
  %2991 = select i1 %2987, i64 %2990, i64 0
  %2992 = or i64 %2991, %2973
  %2993 = add nuw nsw i64 %2972, 1
  %2994 = icmp eq i64 %2993, %2962
  br i1 %2994, label %2963, label %2970, !llvm.loop !33

2995:                                             ; preds = %2995, %2963
  %2996 = phi i64 [ %2968, %2963 ], [ %3000, %2995 ]
  %2997 = phi i64 [ 0, %2963 ], [ %3018, %2995 ]
  %2998 = phi i64 [ 0, %2963 ], [ %3017, %2995 ]
  %2999 = phi i32 [ %2965, %2963 ], [ %3009, %2995 ]
  %3000 = add nsw i64 %2996, 1
  %3001 = trunc nsw i64 %3000 to i32
  store i32 %3001, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3002 = getelementptr inbounds i8, ptr %2685, i64 %2996
  %3003 = load i8, ptr %3002, align 1, !tbaa !12
  %3004 = shl i32 %2999, 13
  %3005 = xor i32 %3004, %2999
  %3006 = lshr i32 %3005, 17
  %3007 = xor i32 %3006, %3005
  %3008 = shl i32 %3007, 5
  %3009 = xor i32 %3008, %3007
  store i32 %3009, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3010 = trunc i32 %3009 to i8
  %3011 = xor i8 %3003, %3010
  %3012 = icmp samesign ult i64 %2997, 8
  %3013 = zext i8 %3011 to i64
  %3014 = shl nuw nsw i64 %2997, 3
  %3015 = shl nuw i64 %3013, %3014
  %3016 = select i1 %3012, i64 %3015, i64 0
  %3017 = or i64 %3016, %2998
  %3018 = add nuw nsw i64 %2997, 1
  %3019 = icmp eq i64 %3018, %2969
  br i1 %3019, label %3020, label %2995, !llvm.loop !33

3020:                                             ; preds = %2995
  %3021 = phi i64 [ %3017, %2995 ]
  br label %3022

3022:                                             ; preds = %3020, %2956
  %3023 = phi i64 [ 0, %2956 ], [ %2966, %3020 ]
  %3024 = phi i64 [ 0, %2956 ], [ %3021, %3020 ]
  %3025 = icmp ne i64 %2959, 0
  %3026 = select i1 %3025, i64 %3023, i64 %3024
  br label %3029

3027:                                             ; preds = %2746
  %3028 = phi i64 [ %2768, %2746 ]
  br label %3029

3029:                                             ; preds = %3027, %3022, %2830, %2742
  %3030 = phi i1 [ %2834, %2830 ], [ %3025, %3022 ], [ false, %2742 ], [ false, %3027 ]
  %3031 = phi i64 [ %2832, %2830 ], [ %3024, %3022 ], [ 0, %2742 ], [ 0, %3027 ]
  %3032 = phi i64 [ %2831, %2830 ], [ %3023, %3022 ], [ 0, %2742 ], [ %3028, %3027 ]
  %3033 = phi i64 [ %2835, %2830 ], [ %3026, %3022 ], [ 0, %2742 ], [ %3028, %3027 ]
  %3034 = zext i1 %3030 to i8
  %3035 = trunc i64 %2737 to i32
  %3036 = load i32, ptr @vm_trace_next__Z8vm_entryv.28, align 4, !tbaa !10
  %3037 = zext i32 %3036 to i64
  %3038 = getelementptr inbounds nuw [32 x %struct.VMTraceEntry], ptr @vm_trace_ring__Z8vm_entryv.33, i64 0, i64 %3037
  store i8 3, ptr %3038, align 8, !tbaa !16
  %3039 = getelementptr inbounds nuw i8, ptr %3038, i64 1
  store i8 %2699, ptr %3039, align 1, !tbaa !21
  %3040 = getelementptr inbounds nuw i8, ptr %3038, i64 2
  store i8 %3034, ptr %3040, align 2, !tbaa !24
  %3041 = getelementptr inbounds nuw i8, ptr %3038, i64 3
  store i8 0, ptr %3041, align 1, !tbaa !32
  %3042 = getelementptr inbounds nuw i8, ptr %3038, i64 4
  store i32 %3035, ptr %3042, align 4, !tbaa !18
  %3043 = getelementptr inbounds nuw i8, ptr %3038, i64 8
  store i64 %2737, ptr %3043, align 8, !tbaa !19
  %3044 = getelementptr inbounds nuw i8, ptr %3038, i64 16
  store i64 %3033, ptr %3044, align 8, !tbaa !20
  %3045 = getelementptr inbounds nuw i8, ptr %3038, i64 24
  store i64 %3032, ptr %3045, align 8, !tbaa !23
  %3046 = getelementptr inbounds nuw i8, ptr %3038, i64 32
  store i64 %3031, ptr %3046, align 8, !tbaa !22
  %3047 = add i32 %3036, 1
  %3048 = and i32 %3047, 31
  store i32 %3048, ptr @vm_trace_next__Z8vm_entryv.28, align 4, !tbaa !10
  %3049 = load i64, ptr @vm_trace_total__Z8vm_entryv.29, align 8, !tbaa !6
  %3050 = add i64 %3049, 1
  store i64 %3050, ptr @vm_trace_total__Z8vm_entryv.29, align 8, !tbaa !6
  %3051 = trunc i64 %3033 to i32
  store i32 %3051, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  br label %2370

3052:                                             ; preds = %239
  %3053 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %3054 = inttoptr i64 %3053 to ptr
  %3055 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3056 = add nsw i32 %3055, 1
  store i32 %3056, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3057 = sext i32 %3055 to i64
  %3058 = getelementptr inbounds i8, ptr %3054, i64 %3057
  %3059 = load i8, ptr %3058, align 1, !tbaa !12
  %3060 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3061 = shl i32 %3060, 13
  %3062 = xor i32 %3061, %3060
  %3063 = lshr i32 %3062, 17
  %3064 = xor i32 %3063, %3062
  %3065 = shl i32 %3064, 5
  %3066 = xor i32 %3065, %3064
  store i32 %3066, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3067 = trunc i32 %3066 to i8
  %3068 = xor i8 %3059, %3067
  %3069 = add nsw i32 %3055, 2
  store i32 %3069, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3070 = sext i32 %3056 to i64
  %3071 = getelementptr inbounds i8, ptr %3054, i64 %3070
  %3072 = load i8, ptr %3071, align 1, !tbaa !12
  %3073 = shl i32 %3066, 13
  %3074 = xor i32 %3073, %3066
  %3075 = lshr i32 %3074, 17
  %3076 = xor i32 %3075, %3074
  %3077 = shl i32 %3076, 5
  %3078 = xor i32 %3077, %3076
  store i32 %3078, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3079 = trunc i32 %3078 to i8
  %3080 = icmp eq i8 %3072, %3079
  br i1 %3080, label %3081, label %3142

3081:                                             ; preds = %3052
  %3082 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %3083 = icmp sgt i32 %3082, 0
  br i1 %3083, label %3084, label %3116

3084:                                             ; preds = %3081
  %3085 = sext i32 %3069 to i64
  %3086 = zext nneg i32 %3082 to i64
  br label %3087

3087:                                             ; preds = %3087, %3084
  %3088 = phi i64 [ %3085, %3084 ], [ %3092, %3087 ]
  %3089 = phi i64 [ 0, %3084 ], [ %3110, %3087 ]
  %3090 = phi i64 [ 0, %3084 ], [ %3109, %3087 ]
  %3091 = phi i32 [ %3078, %3084 ], [ %3101, %3087 ]
  %3092 = add nsw i64 %3088, 1
  %3093 = trunc nsw i64 %3092 to i32
  store i32 %3093, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3094 = getelementptr inbounds i8, ptr %3054, i64 %3088
  %3095 = load i8, ptr %3094, align 1, !tbaa !12
  %3096 = shl i32 %3091, 13
  %3097 = xor i32 %3096, %3091
  %3098 = lshr i32 %3097, 17
  %3099 = xor i32 %3098, %3097
  %3100 = shl i32 %3099, 5
  %3101 = xor i32 %3100, %3099
  store i32 %3101, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3102 = trunc i32 %3101 to i8
  %3103 = xor i8 %3095, %3102
  %3104 = icmp samesign ult i64 %3089, 8
  %3105 = zext i8 %3103 to i64
  %3106 = shl nuw nsw i64 %3089, 3
  %3107 = shl nuw i64 %3105, %3106
  %3108 = select i1 %3104, i64 %3107, i64 0
  %3109 = or i64 %3108, %3090
  %3110 = add nuw nsw i64 %3089, 1
  %3111 = icmp eq i64 %3110, %3086
  br i1 %3111, label %3112, label %3087, !llvm.loop !33

3112:                                             ; preds = %3087
  %3113 = phi i32 [ %3093, %3087 ]
  %3114 = phi i32 [ %3101, %3087 ]
  %3115 = phi i64 [ %3109, %3087 ]
  br label %3116

3116:                                             ; preds = %3112, %3081
  %3117 = phi i32 [ %3078, %3081 ], [ %3114, %3112 ]
  %3118 = phi i32 [ %3069, %3081 ], [ %3113, %3112 ]
  %3119 = phi i64 [ 0, %3081 ], [ %3115, %3112 ]
  %3120 = icmp eq i8 %3059, %3067
  br i1 %3120, label %3178, label %3121

3121:                                             ; preds = %3116
  %3122 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %3123 = inttoptr i64 %3122 to ptr
  %3124 = zext i8 %3068 to i64
  br label %3125

3125:                                             ; preds = %3138, %3121
  %3126 = phi i64 [ 0, %3121 ], [ %3140, %3138 ]
  %3127 = phi i64 [ %3119, %3121 ], [ %3129, %3138 ]
  %3128 = phi i64 [ 0, %3121 ], [ %3139, %3138 ]
  %3129 = add i64 %3127, 1
  %3130 = icmp samesign ult i64 %3126, 8
  br i1 %3130, label %3131, label %3138

3131:                                             ; preds = %3125
  %3132 = getelementptr inbounds nuw i8, ptr %3123, i64 %3127
  %3133 = load i8, ptr %3132, align 1, !tbaa !12
  %3134 = zext i8 %3133 to i64
  %3135 = shl nuw nsw i64 %3126, 3
  %3136 = shl nuw i64 %3134, %3135
  %3137 = or i64 %3136, %3128
  br label %3138

3138:                                             ; preds = %3131, %3125
  %3139 = phi i64 [ %3137, %3131 ], [ %3128, %3125 ]
  %3140 = add nuw nsw i64 %3126, 1
  %3141 = icmp eq i64 %3140, %3124
  br i1 %3141, label %3172, label %3125, !llvm.loop !35

3142:                                             ; preds = %3052
  %3143 = icmp eq i8 %3059, %3067
  br i1 %3143, label %3178, label %3144

3144:                                             ; preds = %3142
  %3145 = sext i32 %3069 to i64
  %3146 = zext i8 %3068 to i64
  br label %3147

3147:                                             ; preds = %3147, %3144
  %3148 = phi i64 [ %3145, %3144 ], [ %3152, %3147 ]
  %3149 = phi i64 [ 0, %3144 ], [ %3170, %3147 ]
  %3150 = phi i64 [ 0, %3144 ], [ %3169, %3147 ]
  %3151 = phi i32 [ %3078, %3144 ], [ %3161, %3147 ]
  %3152 = add nsw i64 %3148, 1
  %3153 = trunc nsw i64 %3152 to i32
  store i32 %3153, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3154 = getelementptr inbounds i8, ptr %3054, i64 %3148
  %3155 = load i8, ptr %3154, align 1, !tbaa !12
  %3156 = shl i32 %3151, 13
  %3157 = xor i32 %3156, %3151
  %3158 = lshr i32 %3157, 17
  %3159 = xor i32 %3158, %3157
  %3160 = shl i32 %3159, 5
  %3161 = xor i32 %3160, %3159
  store i32 %3161, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3162 = trunc i32 %3161 to i8
  %3163 = xor i8 %3155, %3162
  %3164 = icmp samesign ult i64 %3149, 8
  %3165 = zext i8 %3163 to i64
  %3166 = shl nuw nsw i64 %3149, 3
  %3167 = shl nuw i64 %3165, %3166
  %3168 = select i1 %3164, i64 %3167, i64 0
  %3169 = or i64 %3168, %3150
  %3170 = add nuw nsw i64 %3149, 1
  %3171 = icmp eq i64 %3170, %3146
  br i1 %3171, label %3174, label %3147, !llvm.loop !33

3172:                                             ; preds = %3138
  %3173 = phi i64 [ %3139, %3138 ]
  br label %3178

3174:                                             ; preds = %3147
  %3175 = phi i32 [ %3153, %3147 ]
  %3176 = phi i32 [ %3161, %3147 ]
  %3177 = phi i64 [ %3169, %3147 ]
  br label %3178

3178:                                             ; preds = %3174, %3172, %3142, %3116
  %3179 = phi i32 [ %3117, %3116 ], [ %3078, %3142 ], [ %3117, %3172 ], [ %3176, %3174 ]
  %3180 = phi i32 [ %3118, %3116 ], [ %3069, %3142 ], [ %3118, %3172 ], [ %3175, %3174 ]
  %3181 = phi i64 [ 0, %3116 ], [ 0, %3142 ], [ %3173, %3172 ], [ %3177, %3174 ]
  %3182 = sext i32 %3180 to i64
  %3183 = add nsw i64 %3182, 1
  %3184 = trunc nsw i64 %3183 to i32
  store i32 %3184, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3185 = getelementptr inbounds i8, ptr %3054, i64 %3182
  %3186 = load i8, ptr %3185, align 1, !tbaa !12
  %3187 = shl i32 %3179, 13
  %3188 = xor i32 %3187, %3179
  %3189 = lshr i32 %3188, 17
  %3190 = xor i32 %3189, %3188
  %3191 = shl i32 %3190, 5
  %3192 = xor i32 %3191, %3190
  store i32 %3192, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3193 = add nsw i64 %3182, 2
  %3194 = trunc nsw i64 %3193 to i32
  store i32 %3194, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3195 = getelementptr inbounds i8, ptr %3054, i64 %3183
  %3196 = load i8, ptr %3195, align 1, !tbaa !12
  %3197 = shl i32 %3192, 13
  %3198 = xor i32 %3197, %3192
  %3199 = lshr i32 %3198, 17
  %3200 = xor i32 %3199, %3198
  %3201 = shl i32 %3200, 5
  %3202 = xor i32 %3201, %3200
  store i32 %3202, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3203 = add nsw i64 %3182, 3
  %3204 = trunc nsw i64 %3203 to i32
  store i32 %3204, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3205 = getelementptr inbounds i8, ptr %3054, i64 %3193
  %3206 = load i8, ptr %3205, align 1, !tbaa !12
  %3207 = shl i32 %3202, 13
  %3208 = xor i32 %3207, %3202
  %3209 = lshr i32 %3208, 17
  %3210 = xor i32 %3209, %3208
  %3211 = shl i32 %3210, 5
  %3212 = xor i32 %3211, %3210
  store i32 %3212, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3213 = add i32 %3180, 4
  store i32 %3213, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3214 = getelementptr inbounds i8, ptr %3054, i64 %3203
  %3215 = load i8, ptr %3214, align 1, !tbaa !12
  %3216 = shl i32 %3212, 13
  %3217 = xor i32 %3216, %3212
  %3218 = lshr i32 %3217, 17
  %3219 = xor i32 %3218, %3217
  %3220 = shl i32 %3219, 5
  %3221 = xor i32 %3220, %3219
  store i32 %3221, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3222 = sext i32 %3213 to i64
  %3223 = add nsw i64 %3222, 1
  %3224 = trunc nsw i64 %3223 to i32
  store i32 %3224, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3225 = getelementptr inbounds i8, ptr %3054, i64 %3222
  %3226 = load i8, ptr %3225, align 1, !tbaa !12
  %3227 = shl i32 %3221, 13
  %3228 = xor i32 %3227, %3221
  %3229 = lshr i32 %3228, 17
  %3230 = xor i32 %3229, %3228
  %3231 = shl i32 %3230, 5
  %3232 = xor i32 %3231, %3230
  store i32 %3232, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3233 = trunc i32 %3232 to i8
  %3234 = xor i8 %3226, %3233
  %3235 = zext i8 %3234 to i32
  %3236 = add nsw i64 %3222, 2
  %3237 = trunc nsw i64 %3236 to i32
  store i32 %3237, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3238 = getelementptr inbounds i8, ptr %3054, i64 %3223
  %3239 = load i8, ptr %3238, align 1, !tbaa !12
  %3240 = shl i32 %3232, 13
  %3241 = xor i32 %3240, %3232
  %3242 = lshr i32 %3241, 17
  %3243 = xor i32 %3242, %3241
  %3244 = shl i32 %3243, 5
  %3245 = xor i32 %3244, %3243
  store i32 %3245, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3246 = trunc i32 %3245 to i8
  %3247 = xor i8 %3239, %3246
  %3248 = zext i8 %3247 to i32
  %3249 = shl nuw nsw i32 %3248, 8
  %3250 = or disjoint i32 %3249, %3235
  %3251 = add nsw i64 %3222, 3
  %3252 = trunc nsw i64 %3251 to i32
  store i32 %3252, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3253 = getelementptr inbounds i8, ptr %3054, i64 %3236
  %3254 = load i8, ptr %3253, align 1, !tbaa !12
  %3255 = shl i32 %3245, 13
  %3256 = xor i32 %3255, %3245
  %3257 = lshr i32 %3256, 17
  %3258 = xor i32 %3257, %3256
  %3259 = shl i32 %3258, 5
  %3260 = xor i32 %3259, %3258
  store i32 %3260, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3261 = trunc i32 %3260 to i8
  %3262 = xor i8 %3254, %3261
  %3263 = zext i8 %3262 to i32
  %3264 = shl nuw nsw i32 %3263, 16
  %3265 = or disjoint i32 %3264, %3250
  %3266 = add i32 %3180, 8
  store i32 %3266, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3267 = getelementptr inbounds i8, ptr %3054, i64 %3251
  %3268 = load i8, ptr %3267, align 1, !tbaa !12
  %3269 = shl i32 %3260, 13
  %3270 = xor i32 %3269, %3260
  %3271 = lshr i32 %3270, 17
  %3272 = xor i32 %3271, %3270
  %3273 = shl i32 %3272, 5
  %3274 = xor i32 %3273, %3272
  store i32 %3274, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3275 = trunc i32 %3274 to i8
  %3276 = xor i8 %3268, %3275
  %3277 = zext i8 %3276 to i32
  %3278 = shl nuw i32 %3277, 24
  %3279 = or disjoint i32 %3278, %3265
  %3280 = trunc i32 %3192 to i8
  %3281 = xor i8 %3186, %3280
  %3282 = zext i8 %3281 to i32
  %3283 = trunc i32 %3202 to i8
  %3284 = xor i8 %3196, %3283
  %3285 = zext i8 %3284 to i32
  %3286 = shl nuw nsw i32 %3285, 8
  %3287 = or disjoint i32 %3286, %3282
  %3288 = trunc i32 %3212 to i8
  %3289 = xor i8 %3206, %3288
  %3290 = zext i8 %3289 to i32
  %3291 = shl nuw nsw i32 %3290, 16
  %3292 = or disjoint i32 %3291, %3287
  %3293 = trunc i32 %3221 to i8
  %3294 = xor i8 %3215, %3293
  %3295 = zext i8 %3294 to i32
  %3296 = shl nuw i32 %3295, 24
  %3297 = or disjoint i32 %3296, %3292
  %3298 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %3299 = icmp sgt i32 %3298, 0
  br i1 %3299, label %3300, label %3307

3300:                                             ; preds = %3178
  %3301 = sext i32 %3266 to i64
  %3302 = zext nneg i32 %3298 to i64
  br label %3316

3303:                                             ; preds = %3316
  %3304 = phi i32 [ %3322, %3316 ]
  %3305 = phi i32 [ %3330, %3316 ]
  %3306 = phi i64 [ %3338, %3316 ]
  br label %3307

3307:                                             ; preds = %3303, %3178
  %3308 = phi i32 [ %3266, %3178 ], [ %3304, %3303 ]
  %3309 = phi i32 [ %3274, %3178 ], [ %3305, %3303 ]
  %3310 = phi i64 [ 0, %3178 ], [ %3306, %3303 ]
  %3311 = icmp eq i32 %3297, 0
  br i1 %3311, label %3472, label %3312

3312:                                             ; preds = %3307
  %3313 = icmp sgt i32 %3279, 0
  %3314 = zext nneg i32 %3279 to i64
  %3315 = zext nneg i32 %3298 to i64
  br label %3341

3316:                                             ; preds = %3316, %3300
  %3317 = phi i64 [ %3301, %3300 ], [ %3321, %3316 ]
  %3318 = phi i64 [ 0, %3300 ], [ %3339, %3316 ]
  %3319 = phi i64 [ 0, %3300 ], [ %3338, %3316 ]
  %3320 = phi i32 [ %3274, %3300 ], [ %3330, %3316 ]
  %3321 = add nsw i64 %3317, 1
  %3322 = trunc nsw i64 %3321 to i32
  store i32 %3322, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3323 = getelementptr inbounds i8, ptr %3054, i64 %3317
  %3324 = load i8, ptr %3323, align 1, !tbaa !12
  %3325 = shl i32 %3320, 13
  %3326 = xor i32 %3325, %3320
  %3327 = lshr i32 %3326, 17
  %3328 = xor i32 %3327, %3326
  %3329 = shl i32 %3328, 5
  %3330 = xor i32 %3329, %3328
  store i32 %3330, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3331 = trunc i32 %3330 to i8
  %3332 = xor i8 %3324, %3331
  %3333 = icmp samesign ult i64 %3318, 8
  %3334 = zext i8 %3332 to i64
  %3335 = shl nuw nsw i64 %3318, 3
  %3336 = shl nuw i64 %3334, %3335
  %3337 = select i1 %3333, i64 %3336, i64 0
  %3338 = or i64 %3337, %3319
  %3339 = add nuw nsw i64 %3318, 1
  %3340 = icmp eq i64 %3339, %3302
  br i1 %3340, label %3303, label %3316, !llvm.loop !33

3341:                                             ; preds = %3468, %3312
  %3342 = phi i32 [ 0, %3312 ], [ %3416, %3468 ]
  %3343 = phi i32 [ %3308, %3312 ], [ %3413, %3468 ]
  %3344 = phi i32 [ %3309, %3312 ], [ %3412, %3468 ]
  br i1 %3313, label %3345, label %3351

3345:                                             ; preds = %3341
  %3346 = sext i32 %3343 to i64
  br label %3357

3347:                                             ; preds = %3357
  %3348 = phi i32 [ %3363, %3357 ]
  %3349 = phi i32 [ %3371, %3357 ]
  %3350 = phi i64 [ %3379, %3357 ]
  br label %3351

3351:                                             ; preds = %3347, %3341
  %3352 = phi i32 [ %3344, %3341 ], [ %3349, %3347 ]
  %3353 = phi i32 [ %3343, %3341 ], [ %3348, %3347 ]
  %3354 = phi i64 [ 0, %3341 ], [ %3350, %3347 ]
  br i1 %3299, label %3355, label %3411

3355:                                             ; preds = %3351
  %3356 = sext i32 %3353 to i64
  br label %3382

3357:                                             ; preds = %3357, %3345
  %3358 = phi i64 [ %3346, %3345 ], [ %3362, %3357 ]
  %3359 = phi i64 [ 0, %3345 ], [ %3380, %3357 ]
  %3360 = phi i64 [ 0, %3345 ], [ %3379, %3357 ]
  %3361 = phi i32 [ %3344, %3345 ], [ %3371, %3357 ]
  %3362 = add nsw i64 %3358, 1
  %3363 = trunc nsw i64 %3362 to i32
  store i32 %3363, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3364 = getelementptr inbounds i8, ptr %3054, i64 %3358
  %3365 = load i8, ptr %3364, align 1, !tbaa !12
  %3366 = shl i32 %3361, 13
  %3367 = xor i32 %3366, %3361
  %3368 = lshr i32 %3367, 17
  %3369 = xor i32 %3368, %3367
  %3370 = shl i32 %3369, 5
  %3371 = xor i32 %3370, %3369
  store i32 %3371, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3372 = trunc i32 %3371 to i8
  %3373 = xor i8 %3365, %3372
  %3374 = icmp samesign ult i64 %3359, 8
  %3375 = zext i8 %3373 to i64
  %3376 = shl nuw nsw i64 %3359, 3
  %3377 = shl nuw i64 %3375, %3376
  %3378 = select i1 %3374, i64 %3377, i64 0
  %3379 = or i64 %3378, %3360
  %3380 = add nuw nsw i64 %3359, 1
  %3381 = icmp eq i64 %3380, %3314
  br i1 %3381, label %3347, label %3357, !llvm.loop !33

3382:                                             ; preds = %3382, %3355
  %3383 = phi i64 [ %3356, %3355 ], [ %3387, %3382 ]
  %3384 = phi i64 [ 0, %3355 ], [ %3405, %3382 ]
  %3385 = phi i64 [ 0, %3355 ], [ %3404, %3382 ]
  %3386 = phi i32 [ %3352, %3355 ], [ %3396, %3382 ]
  %3387 = add nsw i64 %3383, 1
  %3388 = trunc nsw i64 %3387 to i32
  store i32 %3388, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3389 = getelementptr inbounds i8, ptr %3054, i64 %3383
  %3390 = load i8, ptr %3389, align 1, !tbaa !12
  %3391 = shl i32 %3386, 13
  %3392 = xor i32 %3391, %3386
  %3393 = lshr i32 %3392, 17
  %3394 = xor i32 %3393, %3392
  %3395 = shl i32 %3394, 5
  %3396 = xor i32 %3395, %3394
  store i32 %3396, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3397 = trunc i32 %3396 to i8
  %3398 = xor i8 %3390, %3397
  %3399 = icmp samesign ult i64 %3384, 8
  %3400 = zext i8 %3398 to i64
  %3401 = shl nuw nsw i64 %3384, 3
  %3402 = shl nuw i64 %3400, %3401
  %3403 = select i1 %3399, i64 %3402, i64 0
  %3404 = or i64 %3403, %3385
  %3405 = add nuw nsw i64 %3384, 1
  %3406 = icmp eq i64 %3405, %3315
  br i1 %3406, label %3407, label %3382, !llvm.loop !33

3407:                                             ; preds = %3382
  %3408 = phi i32 [ %3388, %3382 ]
  %3409 = phi i32 [ %3396, %3382 ]
  %3410 = phi i64 [ %3404, %3382 ]
  br label %3411

3411:                                             ; preds = %3407, %3351
  %3412 = phi i32 [ %3352, %3351 ], [ %3409, %3407 ]
  %3413 = phi i32 [ %3353, %3351 ], [ %3408, %3407 ]
  %3414 = phi i64 [ 0, %3351 ], [ %3410, %3407 ]
  %3415 = icmp eq i64 %3181, %3354
  %3416 = add i32 %3342, 1
  br i1 %3415, label %3417, label %3468

3417:                                             ; preds = %3411
  %3418 = phi i32 [ %3412, %3411 ]
  %3419 = phi i32 [ %3413, %3411 ]
  %3420 = phi i64 [ %3414, %3411 ]
  %3421 = phi i32 [ %3416, %3411 ]
  %3422 = icmp ult i32 %3421, %3297
  br i1 %3422, label %3423, label %3472

3423:                                             ; preds = %3417
  %3424 = tail call i32 @llvm.smax.i32(i32 %3279, i32 0)
  %3425 = add i32 %3298, %3424
  br label %3434

3426:                                             ; preds = %3457
  %3427 = phi i32 [ %3465, %3457 ]
  %3428 = add i32 %3425, %3436
  store i32 %3428, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  store i32 %3427, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br label %3429

3429:                                             ; preds = %3442, %3426
  %3430 = phi i32 [ %3427, %3426 ], [ %3443, %3442 ]
  %3431 = phi i32 [ %3428, %3426 ], [ %3444, %3442 ]
  %3432 = add nuw i32 %3435, 1
  %3433 = icmp eq i32 %3432, %3297
  br i1 %3433, label %3470, label %3434, !llvm.loop !44

3434:                                             ; preds = %3429, %3423
  %3435 = phi i32 [ %3421, %3423 ], [ %3432, %3429 ]
  %3436 = phi i32 [ %3419, %3423 ], [ %3431, %3429 ]
  %3437 = phi i32 [ %3418, %3423 ], [ %3430, %3429 ]
  br i1 %3313, label %3438, label %3442

3438:                                             ; preds = %3434
  br label %3446

3439:                                             ; preds = %3446
  %3440 = phi i32 [ %3454, %3446 ]
  %3441 = add i32 %3279, %3436
  store i32 %3440, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br label %3442

3442:                                             ; preds = %3439, %3434
  %3443 = phi i32 [ %3440, %3439 ], [ %3437, %3434 ]
  %3444 = phi i32 [ %3441, %3439 ], [ %3436, %3434 ]
  br i1 %3299, label %3445, label %3429

3445:                                             ; preds = %3442
  br label %3457

3446:                                             ; preds = %3438, %3446
  %3447 = phi i32 [ %3455, %3446 ], [ 0, %3438 ]
  %3448 = phi i32 [ %3454, %3446 ], [ %3437, %3438 ]
  %3449 = shl i32 %3448, 13
  %3450 = xor i32 %3449, %3448
  %3451 = lshr i32 %3450, 17
  %3452 = xor i32 %3451, %3450
  %3453 = shl i32 %3452, 5
  %3454 = xor i32 %3453, %3452
  %3455 = add nuw nsw i32 %3447, 1
  %3456 = icmp eq i32 %3455, %3279
  br i1 %3456, label %3439, label %3446, !llvm.loop !33

3457:                                             ; preds = %3445, %3457
  %3458 = phi i32 [ %3466, %3457 ], [ 0, %3445 ]
  %3459 = phi i32 [ %3465, %3457 ], [ %3443, %3445 ]
  %3460 = shl i32 %3459, 13
  %3461 = xor i32 %3460, %3459
  %3462 = lshr i32 %3461, 17
  %3463 = xor i32 %3462, %3461
  %3464 = shl i32 %3463, 5
  %3465 = xor i32 %3464, %3463
  %3466 = add nuw nsw i32 %3458, 1
  %3467 = icmp eq i32 %3466, %3298
  br i1 %3467, label %3426, label %3457, !llvm.loop !33

3468:                                             ; preds = %3411
  %3469 = icmp eq i32 %3416, %3297
  br i1 %3469, label %3471, label %3341, !llvm.loop !45

3470:                                             ; preds = %3429
  br label %3472

3471:                                             ; preds = %3468
  br label %3472

3472:                                             ; preds = %3471, %3470, %3417, %3307
  %3473 = phi i64 [ %3420, %3417 ], [ %3310, %3307 ], [ %3420, %3470 ], [ %3310, %3471 ]
  %3474 = trunc i64 %3473 to i32
  store i32 %3474, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  br label %2370

3475:                                             ; preds = %239
  %3476 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %3477 = inttoptr i64 %3476 to ptr
  %3478 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3479 = add nsw i32 %3478, 1
  store i32 %3479, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3480 = sext i32 %3478 to i64
  %3481 = getelementptr inbounds i8, ptr %3477, i64 %3480
  %3482 = load i8, ptr %3481, align 1, !tbaa !12
  %3483 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3484 = shl i32 %3483, 13
  %3485 = xor i32 %3484, %3483
  %3486 = lshr i32 %3485, 17
  %3487 = xor i32 %3486, %3485
  %3488 = shl i32 %3487, 5
  %3489 = xor i32 %3488, %3487
  %3490 = trunc i32 %3489 to i8
  %3491 = xor i8 %3482, %3490
  %3492 = add nsw i32 %3478, 2
  %3493 = shl i32 %3489, 13
  %3494 = xor i32 %3493, %3489
  %3495 = lshr i32 %3494, 17
  %3496 = xor i32 %3495, %3494
  %3497 = shl i32 %3496, 5
  %3498 = xor i32 %3497, %3496
  store i32 %3498, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3499 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %3500 = icmp sgt i32 %3499, 0
  br i1 %3500, label %3501, label %3533

3501:                                             ; preds = %3475
  %3502 = sext i32 %3492 to i64
  %3503 = zext nneg i32 %3499 to i64
  br label %3504

3504:                                             ; preds = %3504, %3501
  %3505 = phi i64 [ %3502, %3501 ], [ %3509, %3504 ]
  %3506 = phi i64 [ 0, %3501 ], [ %3527, %3504 ]
  %3507 = phi i64 [ 0, %3501 ], [ %3526, %3504 ]
  %3508 = phi i32 [ %3498, %3501 ], [ %3518, %3504 ]
  %3509 = add nsw i64 %3505, 1
  %3510 = trunc nsw i64 %3509 to i32
  store i32 %3510, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3511 = getelementptr inbounds i8, ptr %3477, i64 %3505
  %3512 = load i8, ptr %3511, align 1, !tbaa !12
  %3513 = shl i32 %3508, 13
  %3514 = xor i32 %3513, %3508
  %3515 = lshr i32 %3514, 17
  %3516 = xor i32 %3515, %3514
  %3517 = shl i32 %3516, 5
  %3518 = xor i32 %3517, %3516
  store i32 %3518, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3519 = trunc i32 %3518 to i8
  %3520 = xor i8 %3512, %3519
  %3521 = icmp samesign ult i64 %3506, 8
  %3522 = zext i8 %3520 to i64
  %3523 = shl nuw nsw i64 %3506, 3
  %3524 = shl nuw i64 %3522, %3523
  %3525 = select i1 %3521, i64 %3524, i64 0
  %3526 = or i64 %3525, %3507
  %3527 = add nuw nsw i64 %3506, 1
  %3528 = icmp eq i64 %3527, %3503
  br i1 %3528, label %3529, label %3504, !llvm.loop !33

3529:                                             ; preds = %3504
  %3530 = phi i32 [ %3510, %3504 ]
  %3531 = phi i32 [ %3518, %3504 ]
  %3532 = phi i64 [ %3526, %3504 ]
  br label %3533

3533:                                             ; preds = %3529, %3475
  %3534 = phi i32 [ %3498, %3475 ], [ %3531, %3529 ]
  %3535 = phi i32 [ %3492, %3475 ], [ %3530, %3529 ]
  %3536 = phi i64 [ 0, %3475 ], [ %3532, %3529 ]
  %3537 = add nsw i32 %3535, 1
  store i32 %3537, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3538 = sext i32 %3535 to i64
  %3539 = getelementptr inbounds i8, ptr %3477, i64 %3538
  %3540 = load i8, ptr %3539, align 1, !tbaa !12
  %3541 = shl i32 %3534, 13
  %3542 = xor i32 %3541, %3534
  %3543 = lshr i32 %3542, 17
  %3544 = xor i32 %3543, %3542
  %3545 = shl i32 %3544, 5
  %3546 = xor i32 %3545, %3544
  store i32 %3546, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3547 = trunc i32 %3546 to i8
  %3548 = xor i8 %3540, %3547
  %3549 = add nsw i32 %3535, 2
  store i32 %3549, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3550 = sext i32 %3537 to i64
  %3551 = getelementptr inbounds i8, ptr %3477, i64 %3550
  %3552 = load i8, ptr %3551, align 1, !tbaa !12
  %3553 = shl i32 %3546, 13
  %3554 = xor i32 %3553, %3546
  %3555 = lshr i32 %3554, 17
  %3556 = xor i32 %3555, %3554
  %3557 = shl i32 %3556, 5
  %3558 = xor i32 %3557, %3556
  store i32 %3558, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3559 = trunc i32 %3558 to i8
  %3560 = icmp eq i8 %3552, %3559
  br i1 %3560, label %3561, label %3601

3561:                                             ; preds = %3533
  br i1 %3500, label %3562, label %3594

3562:                                             ; preds = %3561
  %3563 = sext i32 %3549 to i64
  %3564 = zext nneg i32 %3499 to i64
  br label %3565

3565:                                             ; preds = %3565, %3562
  %3566 = phi i64 [ %3563, %3562 ], [ %3570, %3565 ]
  %3567 = phi i64 [ 0, %3562 ], [ %3588, %3565 ]
  %3568 = phi i64 [ 0, %3562 ], [ %3587, %3565 ]
  %3569 = phi i32 [ %3558, %3562 ], [ %3579, %3565 ]
  %3570 = add nsw i64 %3566, 1
  %3571 = trunc nsw i64 %3570 to i32
  store i32 %3571, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3572 = getelementptr inbounds i8, ptr %3477, i64 %3566
  %3573 = load i8, ptr %3572, align 1, !tbaa !12
  %3574 = shl i32 %3569, 13
  %3575 = xor i32 %3574, %3569
  %3576 = lshr i32 %3575, 17
  %3577 = xor i32 %3576, %3575
  %3578 = shl i32 %3577, 5
  %3579 = xor i32 %3578, %3577
  store i32 %3579, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3580 = trunc i32 %3579 to i8
  %3581 = xor i8 %3573, %3580
  %3582 = icmp samesign ult i64 %3567, 8
  %3583 = zext i8 %3581 to i64
  %3584 = shl nuw nsw i64 %3567, 3
  %3585 = shl nuw i64 %3583, %3584
  %3586 = select i1 %3582, i64 %3585, i64 0
  %3587 = or i64 %3586, %3568
  %3588 = add nuw nsw i64 %3567, 1
  %3589 = icmp eq i64 %3588, %3564
  br i1 %3589, label %3590, label %3565, !llvm.loop !33

3590:                                             ; preds = %3565
  %3591 = phi i32 [ %3571, %3565 ]
  %3592 = phi i32 [ %3579, %3565 ]
  %3593 = phi i64 [ %3587, %3565 ]
  br label %3594

3594:                                             ; preds = %3590, %3561
  %3595 = phi i32 [ %3558, %3561 ], [ %3592, %3590 ]
  %3596 = phi i32 [ %3549, %3561 ], [ %3591, %3590 ]
  %3597 = phi i64 [ 0, %3561 ], [ %3593, %3590 ]
  %3598 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %3599 = inttoptr i64 %3598 to ptr
  %3600 = getelementptr inbounds nuw i8, ptr %3599, i64 %3597
  br label %3649

3601:                                             ; preds = %3533
  %3602 = zext i8 %3548 to i64
  %3603 = tail call noalias ptr @malloc(i64 noundef %3602) #12
  %3604 = icmp eq ptr %3603, null
  %3605 = icmp eq i8 %3540, %3547
  br i1 %3604, label %3606, label %3609

3606:                                             ; preds = %3601
  br i1 %3605, label %3649, label %3607

3607:                                             ; preds = %3606
  %3608 = zext i8 %3548 to i32
  br label %3632

3609:                                             ; preds = %3601
  br i1 %3605, label %3649, label %3610

3610:                                             ; preds = %3609
  %3611 = sext i32 %3549 to i64
  %3612 = zext i8 %3548 to i64
  br label %3613

3613:                                             ; preds = %3613, %3610
  %3614 = phi i64 [ %3611, %3610 ], [ %3617, %3613 ]
  %3615 = phi i64 [ 0, %3610 ], [ %3630, %3613 ]
  %3616 = phi i32 [ %3558, %3610 ], [ %3626, %3613 ]
  %3617 = add nsw i64 %3614, 1
  %3618 = trunc nsw i64 %3617 to i32
  store i32 %3618, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3619 = getelementptr inbounds i8, ptr %3477, i64 %3614
  %3620 = load i8, ptr %3619, align 1, !tbaa !12
  %3621 = shl i32 %3616, 13
  %3622 = xor i32 %3621, %3616
  %3623 = lshr i32 %3622, 17
  %3624 = xor i32 %3623, %3622
  %3625 = shl i32 %3624, 5
  %3626 = xor i32 %3625, %3624
  store i32 %3626, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3627 = trunc i32 %3626 to i8
  %3628 = xor i8 %3620, %3627
  %3629 = getelementptr inbounds nuw i8, ptr %3603, i64 %3615
  store i8 %3628, ptr %3629, align 1, !tbaa !12
  %3630 = add nuw nsw i64 %3615, 1
  %3631 = icmp eq i64 %3630, %3612
  br i1 %3631, label %3646, label %3613, !llvm.loop !37

3632:                                             ; preds = %3632, %3607
  %3633 = phi i32 [ %3641, %3632 ], [ 0, %3607 ]
  %3634 = phi i32 [ %3640, %3632 ], [ %3558, %3607 ]
  %3635 = shl i32 %3634, 13
  %3636 = xor i32 %3635, %3634
  %3637 = lshr i32 %3636, 17
  %3638 = xor i32 %3637, %3636
  %3639 = shl i32 %3638, 5
  %3640 = xor i32 %3639, %3638
  %3641 = add nuw nsw i32 %3633, 1
  %3642 = icmp eq i32 %3641, %3608
  br i1 %3642, label %3643, label %3632, !llvm.loop !46

3643:                                             ; preds = %3632
  %3644 = phi i32 [ %3640, %3632 ]
  %3645 = add i32 %3549, %3608
  store i32 %3644, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br label %3649

3646:                                             ; preds = %3613
  %3647 = phi i32 [ %3618, %3613 ]
  %3648 = phi i32 [ %3626, %3613 ]
  br label %3649

3649:                                             ; preds = %3646, %3643, %3609, %3606, %3594
  %3650 = phi i32 [ %3595, %3594 ], [ %3644, %3643 ], [ %3558, %3606 ], [ %3558, %3609 ], [ %3648, %3646 ]
  %3651 = phi i32 [ %3596, %3594 ], [ %3645, %3643 ], [ %3549, %3606 ], [ %3549, %3609 ], [ %3647, %3646 ]
  %3652 = phi ptr [ null, %3594 ], [ null, %3643 ], [ null, %3606 ], [ %3603, %3609 ], [ %3603, %3646 ]
  %3653 = phi ptr [ %3600, %3594 ], [ null, %3643 ], [ null, %3606 ], [ %3603, %3609 ], [ %3603, %3646 ]
  %3654 = ptrtoint ptr %3653 to i64
  %3655 = add nsw i32 %3651, 1
  store i32 %3655, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3656 = sext i32 %3651 to i64
  %3657 = getelementptr inbounds i8, ptr %3477, i64 %3656
  %3658 = load i8, ptr %3657, align 1, !tbaa !12
  %3659 = shl i32 %3650, 13
  %3660 = xor i32 %3659, %3650
  %3661 = lshr i32 %3660, 17
  %3662 = xor i32 %3661, %3660
  %3663 = shl i32 %3662, 5
  %3664 = xor i32 %3663, %3662
  store i32 %3664, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3665 = trunc i32 %3664 to i8
  %3666 = xor i8 %3658, %3665
  %3667 = add nsw i32 %3651, 2
  store i32 %3667, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3668 = sext i32 %3655 to i64
  %3669 = getelementptr inbounds i8, ptr %3477, i64 %3668
  %3670 = load i8, ptr %3669, align 1, !tbaa !12
  %3671 = shl i32 %3664, 13
  %3672 = xor i32 %3671, %3664
  %3673 = lshr i32 %3672, 17
  %3674 = xor i32 %3673, %3672
  %3675 = shl i32 %3674, 5
  %3676 = xor i32 %3675, %3674
  store i32 %3676, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3677 = trunc i32 %3676 to i8
  %3678 = icmp eq i8 %3670, %3677
  br i1 %3678, label %3679, label %3738

3679:                                             ; preds = %3649
  br i1 %3500, label %3680, label %3712

3680:                                             ; preds = %3679
  %3681 = sext i32 %3667 to i64
  %3682 = zext nneg i32 %3499 to i64
  br label %3683

3683:                                             ; preds = %3683, %3680
  %3684 = phi i64 [ %3681, %3680 ], [ %3688, %3683 ]
  %3685 = phi i64 [ 0, %3680 ], [ %3706, %3683 ]
  %3686 = phi i64 [ 0, %3680 ], [ %3705, %3683 ]
  %3687 = phi i32 [ %3676, %3680 ], [ %3697, %3683 ]
  %3688 = add nsw i64 %3684, 1
  %3689 = trunc nsw i64 %3688 to i32
  store i32 %3689, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3690 = getelementptr inbounds i8, ptr %3477, i64 %3684
  %3691 = load i8, ptr %3690, align 1, !tbaa !12
  %3692 = shl i32 %3687, 13
  %3693 = xor i32 %3692, %3687
  %3694 = lshr i32 %3693, 17
  %3695 = xor i32 %3694, %3693
  %3696 = shl i32 %3695, 5
  %3697 = xor i32 %3696, %3695
  store i32 %3697, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3698 = trunc i32 %3697 to i8
  %3699 = xor i8 %3691, %3698
  %3700 = icmp samesign ult i64 %3685, 8
  %3701 = zext i8 %3699 to i64
  %3702 = shl nuw nsw i64 %3685, 3
  %3703 = shl nuw i64 %3701, %3702
  %3704 = select i1 %3700, i64 %3703, i64 0
  %3705 = or i64 %3704, %3686
  %3706 = add nuw nsw i64 %3685, 1
  %3707 = icmp eq i64 %3706, %3682
  br i1 %3707, label %3708, label %3683, !llvm.loop !33

3708:                                             ; preds = %3683
  %3709 = phi i32 [ %3689, %3683 ]
  %3710 = phi i32 [ %3697, %3683 ]
  %3711 = phi i64 [ %3705, %3683 ]
  br label %3712

3712:                                             ; preds = %3708, %3679
  %3713 = phi i32 [ %3676, %3679 ], [ %3710, %3708 ]
  %3714 = phi i32 [ %3667, %3679 ], [ %3709, %3708 ]
  %3715 = phi i64 [ 0, %3679 ], [ %3711, %3708 ]
  %3716 = icmp eq i8 %3658, %3665
  br i1 %3716, label %3909, label %3717

3717:                                             ; preds = %3712
  %3718 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %3719 = inttoptr i64 %3718 to ptr
  %3720 = zext i8 %3666 to i64
  br label %3721

3721:                                             ; preds = %3734, %3717
  %3722 = phi i64 [ 0, %3717 ], [ %3736, %3734 ]
  %3723 = phi i64 [ %3715, %3717 ], [ %3725, %3734 ]
  %3724 = phi i64 [ 0, %3717 ], [ %3735, %3734 ]
  %3725 = add i64 %3723, 1
  %3726 = icmp samesign ult i64 %3722, 8
  br i1 %3726, label %3727, label %3734

3727:                                             ; preds = %3721
  %3728 = getelementptr inbounds nuw i8, ptr %3719, i64 %3723
  %3729 = load i8, ptr %3728, align 1, !tbaa !12
  %3730 = zext i8 %3729 to i64
  %3731 = shl nuw nsw i64 %3722, 3
  %3732 = shl nuw i64 %3730, %3731
  %3733 = or i64 %3732, %3724
  br label %3734

3734:                                             ; preds = %3727, %3721
  %3735 = phi i64 [ %3733, %3727 ], [ %3724, %3721 ]
  %3736 = add nuw nsw i64 %3722, 1
  %3737 = icmp eq i64 %3736, %3720
  br i1 %3737, label %3903, label %3721, !llvm.loop !35

3738:                                             ; preds = %3649
  %3739 = zext i8 %3666 to i32
  %3740 = icmp ult i8 %3666, 9
  br i1 %3740, label %3741, label %3769

3741:                                             ; preds = %3738
  %3742 = icmp eq i8 %3658, %3665
  br i1 %3742, label %3909, label %3743

3743:                                             ; preds = %3741
  %3744 = sext i32 %3667 to i64
  %3745 = zext nneg i8 %3666 to i64
  br label %3746

3746:                                             ; preds = %3746, %3743
  %3747 = phi i64 [ %3744, %3743 ], [ %3751, %3746 ]
  %3748 = phi i64 [ 0, %3743 ], [ %3767, %3746 ]
  %3749 = phi i64 [ 0, %3743 ], [ %3766, %3746 ]
  %3750 = phi i32 [ %3676, %3743 ], [ %3760, %3746 ]
  %3751 = add nsw i64 %3747, 1
  %3752 = trunc nsw i64 %3751 to i32
  store i32 %3752, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3753 = getelementptr inbounds i8, ptr %3477, i64 %3747
  %3754 = load i8, ptr %3753, align 1, !tbaa !12
  %3755 = shl i32 %3750, 13
  %3756 = xor i32 %3755, %3750
  %3757 = lshr i32 %3756, 17
  %3758 = xor i32 %3757, %3756
  %3759 = shl i32 %3758, 5
  %3760 = xor i32 %3759, %3758
  store i32 %3760, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3761 = trunc i32 %3760 to i8
  %3762 = xor i8 %3754, %3761
  %3763 = zext i8 %3762 to i64
  %3764 = shl nuw nsw i64 %3748, 3
  %3765 = shl nuw i64 %3763, %3764
  %3766 = or i64 %3765, %3749
  %3767 = add nuw nsw i64 %3748, 1
  %3768 = icmp eq i64 %3767, %3745
  br i1 %3768, label %3905, label %3746, !llvm.loop !33

3769:                                             ; preds = %3738
  %3770 = sext i32 %3667 to i64
  %3771 = add nsw i64 %3770, 1
  %3772 = trunc nsw i64 %3771 to i32
  store i32 %3772, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3773 = getelementptr inbounds i8, ptr %3477, i64 %3770
  %3774 = load i8, ptr %3773, align 1, !tbaa !12
  %3775 = shl i32 %3676, 13
  %3776 = xor i32 %3775, %3676
  %3777 = lshr i32 %3776, 17
  %3778 = xor i32 %3777, %3776
  %3779 = shl i32 %3778, 5
  %3780 = xor i32 %3779, %3778
  store i32 %3780, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3781 = trunc i32 %3780 to i8
  %3782 = xor i8 %3774, %3781
  %3783 = zext i8 %3782 to i64
  %3784 = add nsw i64 %3770, 2
  %3785 = trunc nsw i64 %3784 to i32
  store i32 %3785, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3786 = getelementptr inbounds i8, ptr %3477, i64 %3771
  %3787 = load i8, ptr %3786, align 1, !tbaa !12
  %3788 = shl i32 %3780, 13
  %3789 = xor i32 %3788, %3780
  %3790 = lshr i32 %3789, 17
  %3791 = xor i32 %3790, %3789
  %3792 = shl i32 %3791, 5
  %3793 = xor i32 %3792, %3791
  store i32 %3793, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3794 = trunc i32 %3793 to i8
  %3795 = xor i8 %3787, %3794
  %3796 = zext i8 %3795 to i64
  %3797 = shl nuw nsw i64 %3796, 8
  %3798 = or disjoint i64 %3797, %3783
  %3799 = add nsw i64 %3770, 3
  %3800 = trunc nsw i64 %3799 to i32
  store i32 %3800, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3801 = getelementptr inbounds i8, ptr %3477, i64 %3784
  %3802 = load i8, ptr %3801, align 1, !tbaa !12
  %3803 = shl i32 %3793, 13
  %3804 = xor i32 %3803, %3793
  %3805 = lshr i32 %3804, 17
  %3806 = xor i32 %3805, %3804
  %3807 = shl i32 %3806, 5
  %3808 = xor i32 %3807, %3806
  store i32 %3808, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3809 = trunc i32 %3808 to i8
  %3810 = xor i8 %3802, %3809
  %3811 = zext i8 %3810 to i64
  %3812 = shl nuw nsw i64 %3811, 16
  %3813 = or disjoint i64 %3812, %3798
  %3814 = add nsw i64 %3770, 4
  %3815 = trunc nsw i64 %3814 to i32
  store i32 %3815, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3816 = getelementptr inbounds i8, ptr %3477, i64 %3799
  %3817 = load i8, ptr %3816, align 1, !tbaa !12
  %3818 = shl i32 %3808, 13
  %3819 = xor i32 %3818, %3808
  %3820 = lshr i32 %3819, 17
  %3821 = xor i32 %3820, %3819
  %3822 = shl i32 %3821, 5
  %3823 = xor i32 %3822, %3821
  store i32 %3823, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3824 = trunc i32 %3823 to i8
  %3825 = xor i8 %3817, %3824
  %3826 = zext i8 %3825 to i64
  %3827 = shl nuw nsw i64 %3826, 24
  %3828 = or disjoint i64 %3827, %3813
  %3829 = add nsw i64 %3770, 5
  %3830 = trunc nsw i64 %3829 to i32
  store i32 %3830, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3831 = getelementptr inbounds i8, ptr %3477, i64 %3814
  %3832 = load i8, ptr %3831, align 1, !tbaa !12
  %3833 = shl i32 %3823, 13
  %3834 = xor i32 %3833, %3823
  %3835 = lshr i32 %3834, 17
  %3836 = xor i32 %3835, %3834
  %3837 = shl i32 %3836, 5
  %3838 = xor i32 %3837, %3836
  store i32 %3838, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3839 = trunc i32 %3838 to i8
  %3840 = xor i8 %3832, %3839
  %3841 = zext i8 %3840 to i64
  %3842 = shl nuw nsw i64 %3841, 32
  %3843 = or disjoint i64 %3842, %3828
  %3844 = add nsw i64 %3770, 6
  %3845 = trunc nsw i64 %3844 to i32
  store i32 %3845, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3846 = getelementptr inbounds i8, ptr %3477, i64 %3829
  %3847 = load i8, ptr %3846, align 1, !tbaa !12
  %3848 = shl i32 %3838, 13
  %3849 = xor i32 %3848, %3838
  %3850 = lshr i32 %3849, 17
  %3851 = xor i32 %3850, %3849
  %3852 = shl i32 %3851, 5
  %3853 = xor i32 %3852, %3851
  store i32 %3853, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3854 = trunc i32 %3853 to i8
  %3855 = xor i8 %3847, %3854
  %3856 = zext i8 %3855 to i64
  %3857 = shl nuw nsw i64 %3856, 40
  %3858 = or i64 %3857, %3843
  %3859 = add nsw i64 %3770, 7
  %3860 = trunc nsw i64 %3859 to i32
  store i32 %3860, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3861 = getelementptr inbounds i8, ptr %3477, i64 %3844
  %3862 = load i8, ptr %3861, align 1, !tbaa !12
  %3863 = shl i32 %3853, 13
  %3864 = xor i32 %3863, %3853
  %3865 = lshr i32 %3864, 17
  %3866 = xor i32 %3865, %3864
  %3867 = shl i32 %3866, 5
  %3868 = xor i32 %3867, %3866
  store i32 %3868, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3869 = trunc i32 %3868 to i8
  %3870 = xor i8 %3862, %3869
  %3871 = zext i8 %3870 to i64
  %3872 = shl nuw nsw i64 %3871, 48
  %3873 = or i64 %3872, %3858
  %3874 = add i32 %3651, 10
  store i32 %3874, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3875 = getelementptr inbounds i8, ptr %3477, i64 %3859
  %3876 = load i8, ptr %3875, align 1, !tbaa !12
  %3877 = shl i32 %3868, 13
  %3878 = xor i32 %3877, %3868
  %3879 = lshr i32 %3878, 17
  %3880 = xor i32 %3879, %3878
  %3881 = shl i32 %3880, 5
  %3882 = xor i32 %3881, %3880
  %3883 = trunc i32 %3882 to i8
  %3884 = xor i8 %3876, %3883
  %3885 = zext i8 %3884 to i64
  %3886 = shl nuw i64 %3885, 56
  %3887 = or i64 %3886, %3873
  br label %3888

3888:                                             ; preds = %3888, %3769
  %3889 = phi i32 [ %3897, %3888 ], [ 8, %3769 ]
  %3890 = phi i32 [ %3896, %3888 ], [ %3882, %3769 ]
  %3891 = shl i32 %3890, 13
  %3892 = xor i32 %3891, %3890
  %3893 = lshr i32 %3892, 17
  %3894 = xor i32 %3893, %3892
  %3895 = shl i32 %3894, 5
  %3896 = xor i32 %3895, %3894
  %3897 = add nuw nsw i32 %3889, 1
  %3898 = icmp eq i32 %3897, %3739
  br i1 %3898, label %3899, label %3888, !llvm.loop !47

3899:                                             ; preds = %3888
  %3900 = phi i32 [ %3896, %3888 ]
  %3901 = add i32 %3651, 2
  %3902 = add i32 %3901, %3739
  store i32 %3900, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br label %3909

3903:                                             ; preds = %3734
  %3904 = phi i64 [ %3735, %3734 ]
  br label %3909

3905:                                             ; preds = %3746
  %3906 = phi i32 [ %3752, %3746 ]
  %3907 = phi i32 [ %3760, %3746 ]
  %3908 = phi i64 [ %3766, %3746 ]
  br label %3909

3909:                                             ; preds = %3905, %3903, %3899, %3741, %3712
  %3910 = phi i32 [ %3713, %3712 ], [ %3676, %3741 ], [ %3900, %3899 ], [ %3713, %3903 ], [ %3907, %3905 ]
  %3911 = phi i32 [ %3714, %3712 ], [ %3667, %3741 ], [ %3902, %3899 ], [ %3714, %3903 ], [ %3906, %3905 ]
  %3912 = phi i64 [ 0, %3712 ], [ 0, %3741 ], [ %3887, %3899 ], [ %3904, %3903 ], [ %3908, %3905 ]
  %3913 = add nsw i32 %3911, 1
  store i32 %3913, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3914 = sext i32 %3911 to i64
  %3915 = getelementptr inbounds i8, ptr %3477, i64 %3914
  %3916 = load i8, ptr %3915, align 1, !tbaa !12
  %3917 = shl i32 %3910, 13
  %3918 = xor i32 %3917, %3910
  %3919 = lshr i32 %3918, 17
  %3920 = xor i32 %3919, %3918
  %3921 = shl i32 %3920, 5
  %3922 = xor i32 %3921, %3920
  store i32 %3922, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3923 = trunc i32 %3922 to i8
  %3924 = xor i8 %3916, %3923
  %3925 = add nsw i32 %3911, 2
  store i32 %3925, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3926 = sext i32 %3913 to i64
  %3927 = getelementptr inbounds i8, ptr %3477, i64 %3926
  %3928 = load i8, ptr %3927, align 1, !tbaa !12
  %3929 = shl i32 %3922, 13
  %3930 = xor i32 %3929, %3922
  %3931 = lshr i32 %3930, 17
  %3932 = xor i32 %3931, %3930
  %3933 = shl i32 %3932, 5
  %3934 = xor i32 %3933, %3932
  store i32 %3934, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3935 = trunc i32 %3934 to i8
  %3936 = icmp eq i8 %3928, %3935
  br i1 %3936, label %3937, label %3996

3937:                                             ; preds = %3909
  br i1 %3500, label %3938, label %3970

3938:                                             ; preds = %3937
  %3939 = sext i32 %3925 to i64
  %3940 = zext nneg i32 %3499 to i64
  br label %3941

3941:                                             ; preds = %3941, %3938
  %3942 = phi i64 [ %3939, %3938 ], [ %3946, %3941 ]
  %3943 = phi i64 [ 0, %3938 ], [ %3964, %3941 ]
  %3944 = phi i64 [ 0, %3938 ], [ %3963, %3941 ]
  %3945 = phi i32 [ %3934, %3938 ], [ %3955, %3941 ]
  %3946 = add nsw i64 %3942, 1
  %3947 = trunc nsw i64 %3946 to i32
  store i32 %3947, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %3948 = getelementptr inbounds i8, ptr %3477, i64 %3942
  %3949 = load i8, ptr %3948, align 1, !tbaa !12
  %3950 = shl i32 %3945, 13
  %3951 = xor i32 %3950, %3945
  %3952 = lshr i32 %3951, 17
  %3953 = xor i32 %3952, %3951
  %3954 = shl i32 %3953, 5
  %3955 = xor i32 %3954, %3953
  store i32 %3955, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %3956 = trunc i32 %3955 to i8
  %3957 = xor i8 %3949, %3956
  %3958 = icmp samesign ult i64 %3943, 8
  %3959 = zext i8 %3957 to i64
  %3960 = shl nuw nsw i64 %3943, 3
  %3961 = shl nuw i64 %3959, %3960
  %3962 = select i1 %3958, i64 %3961, i64 0
  %3963 = or i64 %3962, %3944
  %3964 = add nuw nsw i64 %3943, 1
  %3965 = icmp eq i64 %3964, %3940
  br i1 %3965, label %3966, label %3941, !llvm.loop !33

3966:                                             ; preds = %3941
  %3967 = phi i32 [ %3947, %3941 ]
  %3968 = phi i32 [ %3955, %3941 ]
  %3969 = phi i64 [ %3963, %3941 ]
  br label %3970

3970:                                             ; preds = %3966, %3937
  %3971 = phi i32 [ %3934, %3937 ], [ %3968, %3966 ]
  %3972 = phi i32 [ %3925, %3937 ], [ %3967, %3966 ]
  %3973 = phi i64 [ 0, %3937 ], [ %3969, %3966 ]
  %3974 = icmp eq i8 %3916, %3923
  br i1 %3974, label %4032, label %3975

3975:                                             ; preds = %3970
  %3976 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %3977 = inttoptr i64 %3976 to ptr
  %3978 = zext i8 %3924 to i64
  br label %3979

3979:                                             ; preds = %3992, %3975
  %3980 = phi i64 [ 0, %3975 ], [ %3994, %3992 ]
  %3981 = phi i64 [ %3973, %3975 ], [ %3983, %3992 ]
  %3982 = phi i64 [ 0, %3975 ], [ %3993, %3992 ]
  %3983 = add i64 %3981, 1
  %3984 = icmp samesign ult i64 %3980, 8
  br i1 %3984, label %3985, label %3992

3985:                                             ; preds = %3979
  %3986 = getelementptr inbounds nuw i8, ptr %3977, i64 %3981
  %3987 = load i8, ptr %3986, align 1, !tbaa !12
  %3988 = zext i8 %3987 to i64
  %3989 = shl nuw nsw i64 %3980, 3
  %3990 = shl nuw i64 %3988, %3989
  %3991 = or i64 %3990, %3982
  br label %3992

3992:                                             ; preds = %3985, %3979
  %3993 = phi i64 [ %3991, %3985 ], [ %3982, %3979 ]
  %3994 = add nuw nsw i64 %3980, 1
  %3995 = icmp eq i64 %3994, %3978
  br i1 %3995, label %4026, label %3979, !llvm.loop !35

3996:                                             ; preds = %3909
  %3997 = icmp eq i8 %3916, %3923
  br i1 %3997, label %4032, label %3998

3998:                                             ; preds = %3996
  %3999 = sext i32 %3925 to i64
  %4000 = zext i8 %3924 to i64
  br label %4001

4001:                                             ; preds = %4001, %3998
  %4002 = phi i64 [ %3999, %3998 ], [ %4006, %4001 ]
  %4003 = phi i64 [ 0, %3998 ], [ %4024, %4001 ]
  %4004 = phi i64 [ 0, %3998 ], [ %4023, %4001 ]
  %4005 = phi i32 [ %3934, %3998 ], [ %4015, %4001 ]
  %4006 = add nsw i64 %4002, 1
  %4007 = trunc nsw i64 %4006 to i32
  store i32 %4007, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4008 = getelementptr inbounds i8, ptr %3477, i64 %4002
  %4009 = load i8, ptr %4008, align 1, !tbaa !12
  %4010 = shl i32 %4005, 13
  %4011 = xor i32 %4010, %4005
  %4012 = lshr i32 %4011, 17
  %4013 = xor i32 %4012, %4011
  %4014 = shl i32 %4013, 5
  %4015 = xor i32 %4014, %4013
  store i32 %4015, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4016 = trunc i32 %4015 to i8
  %4017 = xor i8 %4009, %4016
  %4018 = icmp samesign ult i64 %4003, 8
  %4019 = zext i8 %4017 to i64
  %4020 = shl nuw nsw i64 %4003, 3
  %4021 = shl nuw i64 %4019, %4020
  %4022 = select i1 %4018, i64 %4021, i64 0
  %4023 = or i64 %4022, %4004
  %4024 = add nuw nsw i64 %4003, 1
  %4025 = icmp eq i64 %4024, %4000
  br i1 %4025, label %4028, label %4001, !llvm.loop !33

4026:                                             ; preds = %3992
  %4027 = phi i64 [ %3993, %3992 ]
  br label %4032

4028:                                             ; preds = %4001
  %4029 = phi i32 [ %4007, %4001 ]
  %4030 = phi i32 [ %4015, %4001 ]
  %4031 = phi i64 [ %4023, %4001 ]
  br label %4032

4032:                                             ; preds = %4028, %4026, %3996, %3970
  %4033 = phi i32 [ %3971, %3970 ], [ %3934, %3996 ], [ %3971, %4026 ], [ %4030, %4028 ]
  %4034 = phi i32 [ %3972, %3970 ], [ %3925, %3996 ], [ %3972, %4026 ], [ %4029, %4028 ]
  %4035 = phi i64 [ 0, %3970 ], [ 0, %3996 ], [ %4027, %4026 ], [ %4031, %4028 ]
  %4036 = sext i32 %4034 to i64
  %4037 = add nsw i64 %4036, 1
  %4038 = trunc nsw i64 %4037 to i32
  store i32 %4038, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4039 = getelementptr inbounds i8, ptr %3477, i64 %4036
  %4040 = load i8, ptr %4039, align 1, !tbaa !12
  %4041 = shl i32 %4033, 13
  %4042 = xor i32 %4041, %4033
  %4043 = lshr i32 %4042, 17
  %4044 = xor i32 %4043, %4042
  %4045 = shl i32 %4044, 5
  %4046 = xor i32 %4045, %4044
  store i32 %4046, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4047 = trunc i32 %4046 to i8
  %4048 = xor i8 %4040, %4047
  %4049 = zext i8 %4048 to i32
  %4050 = add nsw i64 %4036, 2
  %4051 = trunc nsw i64 %4050 to i32
  store i32 %4051, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4052 = getelementptr inbounds i8, ptr %3477, i64 %4037
  %4053 = load i8, ptr %4052, align 1, !tbaa !12
  %4054 = shl i32 %4046, 13
  %4055 = xor i32 %4054, %4046
  %4056 = lshr i32 %4055, 17
  %4057 = xor i32 %4056, %4055
  %4058 = shl i32 %4057, 5
  %4059 = xor i32 %4058, %4057
  store i32 %4059, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4060 = trunc i32 %4059 to i8
  %4061 = xor i8 %4053, %4060
  %4062 = zext i8 %4061 to i32
  %4063 = shl nuw nsw i32 %4062, 8
  %4064 = or disjoint i32 %4063, %4049
  %4065 = add nsw i64 %4036, 3
  %4066 = trunc nsw i64 %4065 to i32
  store i32 %4066, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4067 = getelementptr inbounds i8, ptr %3477, i64 %4050
  %4068 = load i8, ptr %4067, align 1, !tbaa !12
  %4069 = shl i32 %4059, 13
  %4070 = xor i32 %4069, %4059
  %4071 = lshr i32 %4070, 17
  %4072 = xor i32 %4071, %4070
  %4073 = shl i32 %4072, 5
  %4074 = xor i32 %4073, %4072
  store i32 %4074, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4075 = trunc i32 %4074 to i8
  %4076 = xor i8 %4068, %4075
  %4077 = zext i8 %4076 to i32
  %4078 = shl nuw nsw i32 %4077, 16
  %4079 = or disjoint i32 %4078, %4064
  %4080 = add i32 %4034, 4
  store i32 %4080, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4081 = getelementptr inbounds i8, ptr %3477, i64 %4065
  %4082 = load i8, ptr %4081, align 1, !tbaa !12
  %4083 = shl i32 %4074, 13
  %4084 = xor i32 %4083, %4074
  %4085 = lshr i32 %4084, 17
  %4086 = xor i32 %4085, %4084
  %4087 = shl i32 %4086, 5
  %4088 = xor i32 %4087, %4086
  store i32 %4088, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4089 = trunc i32 %4088 to i8
  %4090 = xor i8 %4082, %4089
  %4091 = zext i8 %4090 to i32
  %4092 = shl nuw i32 %4091, 24
  %4093 = or disjoint i32 %4092, %4079
  %4094 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %4095 = add i64 %4094, %3536
  %4096 = icmp eq ptr %3653, null
  br i1 %4096, label %4188, label %4097

4097:                                             ; preds = %4032
  %4098 = icmp eq i8 %3482, %3490
  br i1 %4098, label %4192, label %4099

4099:                                             ; preds = %4097
  %4100 = inttoptr i64 %4095 to ptr
  %4101 = zext i8 %3491 to i64
  %4102 = icmp ult i8 %3491, 8
  %4103 = sub i64 %4095, %3654
  %4104 = icmp ult i64 %4103, 32
  %4105 = select i1 %4102, i1 true, i1 %4104
  br i1 %4105, label %4179, label %4106

4106:                                             ; preds = %4099
  %4107 = icmp ult i8 %3491, 32
  br i1 %4107, label %4167, label %4108

4108:                                             ; preds = %4106
  %4109 = and i64 %4101, 224
  %4110 = getelementptr inbounds nuw i8, ptr %3653, i64 16
  %4111 = load <16 x i8>, ptr %3653, align 1, !tbaa !12
  %4112 = load <16 x i8>, ptr %4110, align 1, !tbaa !12
  %4113 = getelementptr inbounds nuw i8, ptr %4100, i64 16
  store <16 x i8> %4111, ptr %4100, align 1, !tbaa !12
  store <16 x i8> %4112, ptr %4113, align 1, !tbaa !12
  %4114 = icmp eq i64 %4109, 32
  br i1 %4114, label %4162, label %4115, !llvm.loop !48

4115:                                             ; preds = %4108
  %4116 = getelementptr inbounds nuw i8, ptr %3653, i64 32
  %4117 = getelementptr inbounds nuw i8, ptr %3653, i64 48
  %4118 = load <16 x i8>, ptr %4116, align 1, !tbaa !12
  %4119 = load <16 x i8>, ptr %4117, align 1, !tbaa !12
  %4120 = getelementptr inbounds nuw i8, ptr %4100, i64 32
  %4121 = getelementptr inbounds nuw i8, ptr %4100, i64 48
  store <16 x i8> %4118, ptr %4120, align 1, !tbaa !12
  store <16 x i8> %4119, ptr %4121, align 1, !tbaa !12
  %4122 = icmp eq i64 %4109, 64
  br i1 %4122, label %4162, label %4123, !llvm.loop !48

4123:                                             ; preds = %4115
  %4124 = getelementptr inbounds nuw i8, ptr %3653, i64 64
  %4125 = getelementptr inbounds nuw i8, ptr %3653, i64 80
  %4126 = load <16 x i8>, ptr %4124, align 1, !tbaa !12
  %4127 = load <16 x i8>, ptr %4125, align 1, !tbaa !12
  %4128 = getelementptr inbounds nuw i8, ptr %4100, i64 64
  %4129 = getelementptr inbounds nuw i8, ptr %4100, i64 80
  store <16 x i8> %4126, ptr %4128, align 1, !tbaa !12
  store <16 x i8> %4127, ptr %4129, align 1, !tbaa !12
  %4130 = icmp eq i64 %4109, 96
  br i1 %4130, label %4162, label %4131, !llvm.loop !48

4131:                                             ; preds = %4123
  %4132 = getelementptr inbounds nuw i8, ptr %3653, i64 96
  %4133 = getelementptr inbounds nuw i8, ptr %3653, i64 112
  %4134 = load <16 x i8>, ptr %4132, align 1, !tbaa !12
  %4135 = load <16 x i8>, ptr %4133, align 1, !tbaa !12
  %4136 = getelementptr inbounds nuw i8, ptr %4100, i64 96
  %4137 = getelementptr inbounds nuw i8, ptr %4100, i64 112
  store <16 x i8> %4134, ptr %4136, align 1, !tbaa !12
  store <16 x i8> %4135, ptr %4137, align 1, !tbaa !12
  %4138 = icmp eq i64 %4109, 128
  br i1 %4138, label %4162, label %4139, !llvm.loop !48

4139:                                             ; preds = %4131
  %4140 = getelementptr inbounds nuw i8, ptr %3653, i64 128
  %4141 = getelementptr inbounds nuw i8, ptr %3653, i64 144
  %4142 = load <16 x i8>, ptr %4140, align 1, !tbaa !12
  %4143 = load <16 x i8>, ptr %4141, align 1, !tbaa !12
  %4144 = getelementptr inbounds nuw i8, ptr %4100, i64 128
  %4145 = getelementptr inbounds nuw i8, ptr %4100, i64 144
  store <16 x i8> %4142, ptr %4144, align 1, !tbaa !12
  store <16 x i8> %4143, ptr %4145, align 1, !tbaa !12
  %4146 = icmp eq i64 %4109, 160
  br i1 %4146, label %4162, label %4147, !llvm.loop !48

4147:                                             ; preds = %4139
  %4148 = getelementptr inbounds nuw i8, ptr %3653, i64 160
  %4149 = getelementptr inbounds nuw i8, ptr %3653, i64 176
  %4150 = load <16 x i8>, ptr %4148, align 1, !tbaa !12
  %4151 = load <16 x i8>, ptr %4149, align 1, !tbaa !12
  %4152 = getelementptr inbounds nuw i8, ptr %4100, i64 160
  %4153 = getelementptr inbounds nuw i8, ptr %4100, i64 176
  store <16 x i8> %4150, ptr %4152, align 1, !tbaa !12
  store <16 x i8> %4151, ptr %4153, align 1, !tbaa !12
  %4154 = icmp eq i64 %4109, 192
  br i1 %4154, label %4162, label %4155, !llvm.loop !48

4155:                                             ; preds = %4147
  %4156 = getelementptr inbounds nuw i8, ptr %3653, i64 192
  %4157 = getelementptr inbounds nuw i8, ptr %3653, i64 208
  %4158 = load <16 x i8>, ptr %4156, align 1, !tbaa !12
  %4159 = load <16 x i8>, ptr %4157, align 1, !tbaa !12
  %4160 = getelementptr inbounds nuw i8, ptr %4100, i64 192
  %4161 = getelementptr inbounds nuw i8, ptr %4100, i64 208
  store <16 x i8> %4158, ptr %4160, align 1, !tbaa !12
  store <16 x i8> %4159, ptr %4161, align 1, !tbaa !12
  br label %4162

4162:                                             ; preds = %4155, %4147, %4139, %4131, %4123, %4115, %4108
  %4163 = icmp eq i64 %4109, %4101
  br i1 %4163, label %4192, label %4164

4164:                                             ; preds = %4162
  %4165 = and i64 %4101, 24
  %4166 = icmp eq i64 %4165, 0
  br i1 %4166, label %4179, label %4167

4167:                                             ; preds = %4164, %4106
  %4168 = phi i64 [ %4109, %4164 ], [ 0, %4106 ]
  %4169 = and i64 %4101, 248
  br label %4170

4170:                                             ; preds = %4170, %4167
  %4171 = phi i64 [ %4168, %4167 ], [ %4175, %4170 ]
  %4172 = getelementptr inbounds nuw i8, ptr %3653, i64 %4171
  %4173 = load <8 x i8>, ptr %4172, align 1, !tbaa !12
  %4174 = getelementptr inbounds nuw i8, ptr %4100, i64 %4171
  store <8 x i8> %4173, ptr %4174, align 1, !tbaa !12
  %4175 = add nuw i64 %4171, 8
  %4176 = icmp eq i64 %4175, %4169
  br i1 %4176, label %4177, label %4170, !llvm.loop !49

4177:                                             ; preds = %4170
  %4178 = icmp eq i64 %4169, %4101
  br i1 %4178, label %4192, label %4179

4179:                                             ; preds = %4177, %4164, %4099
  %4180 = phi i64 [ 0, %4099 ], [ %4109, %4164 ], [ %4169, %4177 ]
  br label %4181

4181:                                             ; preds = %4181, %4179
  %4182 = phi i64 [ %4186, %4181 ], [ %4180, %4179 ]
  %4183 = getelementptr inbounds nuw i8, ptr %3653, i64 %4182
  %4184 = load i8, ptr %4183, align 1, !tbaa !12
  %4185 = getelementptr inbounds nuw i8, ptr %4100, i64 %4182
  store i8 %4184, ptr %4185, align 1, !tbaa !12
  %4186 = add nuw nsw i64 %4182, 1
  %4187 = icmp eq i64 %4186, %4101
  br i1 %4187, label %4191, label %4181, !llvm.loop !50

4188:                                             ; preds = %4032
  %4189 = inttoptr i64 %4095 to ptr
  %4190 = zext i8 %3491 to i64
  tail call void @llvm.memset.p0.i64(ptr align 1 %4189, i8 0, i64 %4190, i1 false)
  br label %4192

4191:                                             ; preds = %4181
  br label %4192

4192:                                             ; preds = %4191, %4188, %4177, %4162, %4097
  %4193 = icmp eq i32 %4093, 0
  br i1 %4193, label %4210, label %4194

4194:                                             ; preds = %4192
  %4195 = tail call i32 @llvm.umin.i32(i32 %4093, i32 8)
  %4196 = inttoptr i64 %4095 to ptr
  %4197 = getelementptr i8, ptr %4196, i64 %4035
  %4198 = zext nneg i32 %4195 to i64
  %4199 = icmp ult i32 %4093, 8
  br i1 %4199, label %4200, label %4202

4200:                                             ; preds = %4202, %4194
  %4201 = phi i64 [ 0, %4194 ], [ %4203, %4202 ]
  br label %4212

4202:                                             ; preds = %4194
  %4203 = and i64 %4198, 8
  %4204 = insertelement <8 x i64> poison, i64 %3912, i64 0
  %4205 = shufflevector <8 x i64> %4204, <8 x i64> poison, <8 x i32> zeroinitializer
  %4206 = lshr <8 x i64> %4205, <i64 0, i64 8, i64 16, i64 24, i64 32, i64 40, i64 48, i64 56>
  %4207 = trunc <8 x i64> %4206 to <8 x i8>
  store <8 x i8> %4207, ptr %4197, align 1, !tbaa !12
  %4208 = icmp eq i64 %4203, %4198
  br i1 %4208, label %4210, label %4200

4209:                                             ; preds = %4212
  br label %4210

4210:                                             ; preds = %4209, %4202, %4192
  %4211 = icmp eq ptr %3652, null
  br i1 %4211, label %2370, label %4220

4212:                                             ; preds = %4212, %4200
  %4213 = phi i64 [ %4218, %4212 ], [ %4201, %4200 ]
  %4214 = shl nuw nsw i64 %4213, 3
  %4215 = lshr i64 %3912, %4214
  %4216 = trunc i64 %4215 to i8
  %4217 = getelementptr i8, ptr %4197, i64 %4213
  store i8 %4216, ptr %4217, align 1, !tbaa !12
  %4218 = add nuw nsw i64 %4213, 1
  %4219 = icmp eq i64 %4218, %4198
  br i1 %4219, label %4209, label %4212, !llvm.loop !51

4220:                                             ; preds = %4210
  tail call void @free(ptr noundef nonnull %3652)
  br label %2370

4221:                                             ; preds = %239
  %4222 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %4223 = inttoptr i64 %4222 to ptr
  %4224 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4225 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4226 = shl i32 %4225, 13
  %4227 = xor i32 %4226, %4225
  %4228 = lshr i32 %4227, 17
  %4229 = xor i32 %4228, %4227
  %4230 = shl i32 %4229, 5
  %4231 = xor i32 %4230, %4229
  %4232 = add nsw i32 %4224, 2
  %4233 = shl i32 %4231, 13
  %4234 = xor i32 %4233, %4231
  %4235 = lshr i32 %4234, 17
  %4236 = xor i32 %4235, %4234
  %4237 = shl i32 %4236, 5
  %4238 = xor i32 %4237, %4236
  store i32 %4238, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4239 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %4240 = icmp sgt i32 %4239, 0
  br i1 %4240, label %4241, label %4273

4241:                                             ; preds = %4221
  %4242 = sext i32 %4232 to i64
  %4243 = zext nneg i32 %4239 to i64
  br label %4244

4244:                                             ; preds = %4244, %4241
  %4245 = phi i64 [ %4242, %4241 ], [ %4249, %4244 ]
  %4246 = phi i64 [ 0, %4241 ], [ %4267, %4244 ]
  %4247 = phi i64 [ 0, %4241 ], [ %4266, %4244 ]
  %4248 = phi i32 [ %4238, %4241 ], [ %4258, %4244 ]
  %4249 = add nsw i64 %4245, 1
  %4250 = trunc nsw i64 %4249 to i32
  store i32 %4250, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4251 = getelementptr inbounds i8, ptr %4223, i64 %4245
  %4252 = load i8, ptr %4251, align 1, !tbaa !12
  %4253 = shl i32 %4248, 13
  %4254 = xor i32 %4253, %4248
  %4255 = lshr i32 %4254, 17
  %4256 = xor i32 %4255, %4254
  %4257 = shl i32 %4256, 5
  %4258 = xor i32 %4257, %4256
  store i32 %4258, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4259 = trunc i32 %4258 to i8
  %4260 = xor i8 %4252, %4259
  %4261 = icmp samesign ult i64 %4246, 8
  %4262 = zext i8 %4260 to i64
  %4263 = shl nuw nsw i64 %4246, 3
  %4264 = shl nuw i64 %4262, %4263
  %4265 = select i1 %4261, i64 %4264, i64 0
  %4266 = or i64 %4265, %4247
  %4267 = add nuw nsw i64 %4246, 1
  %4268 = icmp eq i64 %4267, %4243
  br i1 %4268, label %4269, label %4244, !llvm.loop !33

4269:                                             ; preds = %4244
  %4270 = phi i32 [ %4250, %4244 ]
  %4271 = phi i32 [ %4258, %4244 ]
  %4272 = phi i64 [ %4266, %4244 ]
  br label %4273

4273:                                             ; preds = %4269, %4221
  %4274 = phi i32 [ %4238, %4221 ], [ %4271, %4269 ]
  %4275 = phi i32 [ %4232, %4221 ], [ %4270, %4269 ]
  %4276 = phi i64 [ 0, %4221 ], [ %4272, %4269 ]
  %4277 = add nsw i32 %4275, 1
  store i32 %4277, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4278 = sext i32 %4275 to i64
  %4279 = getelementptr inbounds i8, ptr %4223, i64 %4278
  %4280 = load i8, ptr %4279, align 1, !tbaa !12
  %4281 = shl i32 %4274, 13
  %4282 = xor i32 %4281, %4274
  %4283 = lshr i32 %4282, 17
  %4284 = xor i32 %4283, %4282
  %4285 = shl i32 %4284, 5
  %4286 = xor i32 %4285, %4284
  store i32 %4286, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4287 = trunc i32 %4286 to i8
  %4288 = xor i8 %4280, %4287
  %4289 = add nsw i32 %4275, 2
  store i32 %4289, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4290 = sext i32 %4277 to i64
  %4291 = getelementptr inbounds i8, ptr %4223, i64 %4290
  %4292 = load i8, ptr %4291, align 1, !tbaa !12
  %4293 = shl i32 %4286, 13
  %4294 = xor i32 %4293, %4286
  %4295 = lshr i32 %4294, 17
  %4296 = xor i32 %4295, %4294
  %4297 = shl i32 %4296, 5
  %4298 = xor i32 %4297, %4296
  store i32 %4298, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4299 = trunc i32 %4298 to i8
  %4300 = icmp eq i8 %4292, %4299
  br i1 %4300, label %4301, label %4341

4301:                                             ; preds = %4273
  br i1 %4240, label %4302, label %4334

4302:                                             ; preds = %4301
  %4303 = sext i32 %4289 to i64
  %4304 = zext nneg i32 %4239 to i64
  br label %4305

4305:                                             ; preds = %4305, %4302
  %4306 = phi i64 [ %4303, %4302 ], [ %4310, %4305 ]
  %4307 = phi i64 [ 0, %4302 ], [ %4328, %4305 ]
  %4308 = phi i64 [ 0, %4302 ], [ %4327, %4305 ]
  %4309 = phi i32 [ %4298, %4302 ], [ %4319, %4305 ]
  %4310 = add nsw i64 %4306, 1
  %4311 = trunc nsw i64 %4310 to i32
  store i32 %4311, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4312 = getelementptr inbounds i8, ptr %4223, i64 %4306
  %4313 = load i8, ptr %4312, align 1, !tbaa !12
  %4314 = shl i32 %4309, 13
  %4315 = xor i32 %4314, %4309
  %4316 = lshr i32 %4315, 17
  %4317 = xor i32 %4316, %4315
  %4318 = shl i32 %4317, 5
  %4319 = xor i32 %4318, %4317
  store i32 %4319, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4320 = trunc i32 %4319 to i8
  %4321 = xor i8 %4313, %4320
  %4322 = icmp samesign ult i64 %4307, 8
  %4323 = zext i8 %4321 to i64
  %4324 = shl nuw nsw i64 %4307, 3
  %4325 = shl nuw i64 %4323, %4324
  %4326 = select i1 %4322, i64 %4325, i64 0
  %4327 = or i64 %4326, %4308
  %4328 = add nuw nsw i64 %4307, 1
  %4329 = icmp eq i64 %4328, %4304
  br i1 %4329, label %4330, label %4305, !llvm.loop !33

4330:                                             ; preds = %4305
  %4331 = phi i32 [ %4311, %4305 ]
  %4332 = phi i32 [ %4319, %4305 ]
  %4333 = phi i64 [ %4327, %4305 ]
  br label %4334

4334:                                             ; preds = %4330, %4301
  %4335 = phi i32 [ %4298, %4301 ], [ %4332, %4330 ]
  %4336 = phi i32 [ %4289, %4301 ], [ %4331, %4330 ]
  %4337 = phi i64 [ 0, %4301 ], [ %4333, %4330 ]
  %4338 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %4339 = inttoptr i64 %4338 to ptr
  %4340 = getelementptr inbounds nuw i8, ptr %4339, i64 %4337
  br label %4389

4341:                                             ; preds = %4273
  %4342 = zext i8 %4288 to i64
  %4343 = tail call noalias ptr @malloc(i64 noundef %4342) #12
  %4344 = icmp eq ptr %4343, null
  %4345 = icmp eq i8 %4280, %4287
  br i1 %4344, label %4346, label %4349

4346:                                             ; preds = %4341
  br i1 %4345, label %4389, label %4347

4347:                                             ; preds = %4346
  %4348 = zext i8 %4288 to i32
  br label %4372

4349:                                             ; preds = %4341
  br i1 %4345, label %4389, label %4350

4350:                                             ; preds = %4349
  %4351 = sext i32 %4289 to i64
  %4352 = zext i8 %4288 to i64
  br label %4353

4353:                                             ; preds = %4353, %4350
  %4354 = phi i64 [ %4351, %4350 ], [ %4357, %4353 ]
  %4355 = phi i64 [ 0, %4350 ], [ %4370, %4353 ]
  %4356 = phi i32 [ %4298, %4350 ], [ %4366, %4353 ]
  %4357 = add nsw i64 %4354, 1
  %4358 = trunc nsw i64 %4357 to i32
  store i32 %4358, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4359 = getelementptr inbounds i8, ptr %4223, i64 %4354
  %4360 = load i8, ptr %4359, align 1, !tbaa !12
  %4361 = shl i32 %4356, 13
  %4362 = xor i32 %4361, %4356
  %4363 = lshr i32 %4362, 17
  %4364 = xor i32 %4363, %4362
  %4365 = shl i32 %4364, 5
  %4366 = xor i32 %4365, %4364
  store i32 %4366, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4367 = trunc i32 %4366 to i8
  %4368 = xor i8 %4360, %4367
  %4369 = getelementptr inbounds nuw i8, ptr %4343, i64 %4355
  store i8 %4368, ptr %4369, align 1, !tbaa !12
  %4370 = add nuw nsw i64 %4355, 1
  %4371 = icmp eq i64 %4370, %4352
  br i1 %4371, label %4386, label %4353, !llvm.loop !37

4372:                                             ; preds = %4372, %4347
  %4373 = phi i32 [ %4381, %4372 ], [ 0, %4347 ]
  %4374 = phi i32 [ %4380, %4372 ], [ %4298, %4347 ]
  %4375 = shl i32 %4374, 13
  %4376 = xor i32 %4375, %4374
  %4377 = lshr i32 %4376, 17
  %4378 = xor i32 %4377, %4376
  %4379 = shl i32 %4378, 5
  %4380 = xor i32 %4379, %4378
  %4381 = add nuw nsw i32 %4373, 1
  %4382 = icmp eq i32 %4381, %4348
  br i1 %4382, label %4383, label %4372, !llvm.loop !52

4383:                                             ; preds = %4372
  %4384 = phi i32 [ %4380, %4372 ]
  %4385 = add i32 %4289, %4348
  br label %4389

4386:                                             ; preds = %4353
  %4387 = phi i32 [ %4358, %4353 ]
  %4388 = phi i32 [ %4366, %4353 ]
  br label %4389

4389:                                             ; preds = %4386, %4383, %4349, %4346, %4334
  %4390 = phi i32 [ %4335, %4334 ], [ %4384, %4383 ], [ %4298, %4346 ], [ %4298, %4349 ], [ %4388, %4386 ]
  %4391 = phi i32 [ %4336, %4334 ], [ %4385, %4383 ], [ %4289, %4346 ], [ %4289, %4349 ], [ %4387, %4386 ]
  %4392 = phi ptr [ null, %4334 ], [ null, %4383 ], [ null, %4346 ], [ %4343, %4349 ], [ %4343, %4386 ]
  %4393 = phi ptr [ %4340, %4334 ], [ null, %4383 ], [ null, %4346 ], [ %4343, %4349 ], [ %4343, %4386 ]
  %4394 = shl i32 %4390, 13
  %4395 = xor i32 %4394, %4390
  %4396 = lshr i32 %4395, 17
  %4397 = xor i32 %4396, %4395
  %4398 = shl i32 %4397, 5
  %4399 = xor i32 %4398, %4397
  %4400 = add nsw i32 %4391, 2
  %4401 = shl i32 %4399, 13
  %4402 = xor i32 %4401, %4399
  %4403 = lshr i32 %4402, 17
  %4404 = xor i32 %4403, %4402
  %4405 = shl i32 %4404, 5
  %4406 = xor i32 %4405, %4404
  store i32 %4406, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br i1 %4240, label %4407, label %4414

4407:                                             ; preds = %4389
  %4408 = sext i32 %4400 to i64
  %4409 = zext nneg i32 %4239 to i64
  br label %4477

4410:                                             ; preds = %4477
  %4411 = phi i32 [ %4483, %4477 ]
  %4412 = phi i32 [ %4491, %4477 ]
  %4413 = phi i64 [ %4499, %4477 ]
  br label %4414

4414:                                             ; preds = %4410, %4389
  %4415 = phi i32 [ %4406, %4389 ], [ %4412, %4410 ]
  %4416 = phi i32 [ %4400, %4389 ], [ %4411, %4410 ]
  %4417 = phi i64 [ 0, %4389 ], [ %4413, %4410 ]
  %4418 = sext i32 %4416 to i64
  %4419 = add nsw i64 %4418, 1
  %4420 = trunc nsw i64 %4419 to i32
  store i32 %4420, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4421 = getelementptr inbounds i8, ptr %4223, i64 %4418
  %4422 = load i8, ptr %4421, align 1, !tbaa !12
  %4423 = shl i32 %4415, 13
  %4424 = xor i32 %4423, %4415
  %4425 = lshr i32 %4424, 17
  %4426 = xor i32 %4425, %4424
  %4427 = shl i32 %4426, 5
  %4428 = xor i32 %4427, %4426
  store i32 %4428, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4429 = trunc i32 %4428 to i8
  %4430 = xor i8 %4422, %4429
  %4431 = zext i8 %4430 to i64
  %4432 = add nsw i64 %4418, 2
  %4433 = trunc nsw i64 %4432 to i32
  store i32 %4433, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4434 = getelementptr inbounds i8, ptr %4223, i64 %4419
  %4435 = load i8, ptr %4434, align 1, !tbaa !12
  %4436 = shl i32 %4428, 13
  %4437 = xor i32 %4436, %4428
  %4438 = lshr i32 %4437, 17
  %4439 = xor i32 %4438, %4437
  %4440 = shl i32 %4439, 5
  %4441 = xor i32 %4440, %4439
  store i32 %4441, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4442 = trunc i32 %4441 to i8
  %4443 = xor i8 %4435, %4442
  %4444 = zext i8 %4443 to i64
  %4445 = shl nuw nsw i64 %4444, 8
  %4446 = or disjoint i64 %4445, %4431
  %4447 = add nsw i64 %4418, 3
  %4448 = trunc nsw i64 %4447 to i32
  store i32 %4448, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4449 = getelementptr inbounds i8, ptr %4223, i64 %4432
  %4450 = load i8, ptr %4449, align 1, !tbaa !12
  %4451 = shl i32 %4441, 13
  %4452 = xor i32 %4451, %4441
  %4453 = lshr i32 %4452, 17
  %4454 = xor i32 %4453, %4452
  %4455 = shl i32 %4454, 5
  %4456 = xor i32 %4455, %4454
  store i32 %4456, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4457 = trunc i32 %4456 to i8
  %4458 = xor i8 %4450, %4457
  %4459 = zext i8 %4458 to i64
  %4460 = shl nuw nsw i64 %4459, 16
  %4461 = or disjoint i64 %4460, %4446
  %4462 = add i32 %4416, 4
  store i32 %4462, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4463 = getelementptr inbounds i8, ptr %4223, i64 %4447
  %4464 = load i8, ptr %4463, align 1, !tbaa !12
  %4465 = shl i32 %4456, 13
  %4466 = xor i32 %4465, %4456
  %4467 = lshr i32 %4466, 17
  %4468 = xor i32 %4467, %4466
  %4469 = shl i32 %4468, 5
  %4470 = xor i32 %4469, %4468
  store i32 %4470, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4471 = trunc i32 %4470 to i8
  %4472 = xor i8 %4464, %4471
  %4473 = zext i8 %4472 to i64
  %4474 = shl nuw nsw i64 %4473, 24
  %4475 = or disjoint i64 %4474, %4461
  %4476 = icmp eq ptr %4393, null
  br i1 %4476, label %4507, label %4502

4477:                                             ; preds = %4477, %4407
  %4478 = phi i64 [ %4408, %4407 ], [ %4482, %4477 ]
  %4479 = phi i64 [ 0, %4407 ], [ %4500, %4477 ]
  %4480 = phi i64 [ 0, %4407 ], [ %4499, %4477 ]
  %4481 = phi i32 [ %4406, %4407 ], [ %4491, %4477 ]
  %4482 = add nsw i64 %4478, 1
  %4483 = trunc nsw i64 %4482 to i32
  store i32 %4483, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4484 = getelementptr inbounds i8, ptr %4223, i64 %4478
  %4485 = load i8, ptr %4484, align 1, !tbaa !12
  %4486 = shl i32 %4481, 13
  %4487 = xor i32 %4486, %4481
  %4488 = lshr i32 %4487, 17
  %4489 = xor i32 %4488, %4487
  %4490 = shl i32 %4489, 5
  %4491 = xor i32 %4490, %4489
  store i32 %4491, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4492 = trunc i32 %4491 to i8
  %4493 = xor i8 %4485, %4492
  %4494 = icmp samesign ult i64 %4479, 8
  %4495 = zext i8 %4493 to i64
  %4496 = shl nuw nsw i64 %4479, 3
  %4497 = shl nuw i64 %4495, %4496
  %4498 = select i1 %4494, i64 %4497, i64 0
  %4499 = or i64 %4498, %4480
  %4500 = add nuw nsw i64 %4479, 1
  %4501 = icmp eq i64 %4500, %4409
  br i1 %4501, label %4410, label %4477, !llvm.loop !33

4502:                                             ; preds = %4414
  %4503 = getelementptr inbounds nuw i8, ptr %4393, i64 %4417
  %4504 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %4505 = inttoptr i64 %4504 to ptr
  %4506 = getelementptr inbounds nuw i8, ptr %4505, i64 %4276
  tail call void @llvm.memcpy.p0.p0.i64(ptr align 1 %4506, ptr nonnull align 1 %4503, i64 %4475, i1 false)
  br label %4511

4507:                                             ; preds = %4414
  %4508 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %4509 = inttoptr i64 %4508 to ptr
  %4510 = getelementptr inbounds nuw i8, ptr %4509, i64 %4276
  tail call void @llvm.memset.p0.i64(ptr align 1 %4510, i8 0, i64 %4475, i1 false)
  br label %4511

4511:                                             ; preds = %4507, %4502
  %4512 = icmp eq ptr %4392, null
  br i1 %4512, label %2370, label %4513

4513:                                             ; preds = %4511
  tail call void @free(ptr noundef nonnull %4392)
  br label %2370

4514:                                             ; preds = %239
  %4515 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %4516 = inttoptr i64 %4515 to ptr
  %4517 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4518 = add nsw i32 %4517, 1
  store i32 %4518, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4519 = sext i32 %4517 to i64
  %4520 = getelementptr inbounds i8, ptr %4516, i64 %4519
  %4521 = load i8, ptr %4520, align 1, !tbaa !12
  %4522 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4523 = shl i32 %4522, 13
  %4524 = xor i32 %4523, %4522
  %4525 = lshr i32 %4524, 17
  %4526 = xor i32 %4525, %4524
  %4527 = shl i32 %4526, 5
  %4528 = xor i32 %4527, %4526
  %4529 = trunc i32 %4528 to i8
  %4530 = xor i8 %4521, %4529
  %4531 = add nsw i32 %4517, 2
  %4532 = shl i32 %4528, 13
  %4533 = xor i32 %4532, %4528
  %4534 = lshr i32 %4533, 17
  %4535 = xor i32 %4534, %4533
  %4536 = shl i32 %4535, 5
  %4537 = xor i32 %4536, %4535
  store i32 %4537, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4538 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %4539 = icmp sgt i32 %4538, 0
  br i1 %4539, label %4540, label %4547

4540:                                             ; preds = %4514
  %4541 = sext i32 %4531 to i64
  %4542 = zext nneg i32 %4538 to i64
  br label %4611

4543:                                             ; preds = %4611
  %4544 = phi i32 [ %4617, %4611 ]
  %4545 = phi i32 [ %4625, %4611 ]
  %4546 = phi i64 [ %4633, %4611 ]
  br label %4547

4547:                                             ; preds = %4543, %4514
  %4548 = phi i32 [ %4537, %4514 ], [ %4545, %4543 ]
  %4549 = phi i32 [ %4531, %4514 ], [ %4544, %4543 ]
  %4550 = phi i64 [ 0, %4514 ], [ %4546, %4543 ]
  %4551 = sext i32 %4549 to i64
  %4552 = add nsw i64 %4551, 1
  %4553 = trunc nsw i64 %4552 to i32
  store i32 %4553, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4554 = getelementptr inbounds i8, ptr %4516, i64 %4551
  %4555 = load i8, ptr %4554, align 1, !tbaa !12
  %4556 = shl i32 %4548, 13
  %4557 = xor i32 %4556, %4548
  %4558 = lshr i32 %4557, 17
  %4559 = xor i32 %4558, %4557
  %4560 = shl i32 %4559, 5
  %4561 = xor i32 %4560, %4559
  store i32 %4561, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4562 = trunc i32 %4561 to i8
  %4563 = xor i8 %4555, %4562
  %4564 = zext i8 %4563 to i32
  %4565 = add nsw i64 %4551, 2
  %4566 = trunc nsw i64 %4565 to i32
  store i32 %4566, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4567 = getelementptr inbounds i8, ptr %4516, i64 %4552
  %4568 = load i8, ptr %4567, align 1, !tbaa !12
  %4569 = shl i32 %4561, 13
  %4570 = xor i32 %4569, %4561
  %4571 = lshr i32 %4570, 17
  %4572 = xor i32 %4571, %4570
  %4573 = shl i32 %4572, 5
  %4574 = xor i32 %4573, %4572
  store i32 %4574, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4575 = trunc i32 %4574 to i8
  %4576 = xor i8 %4568, %4575
  %4577 = zext i8 %4576 to i32
  %4578 = shl nuw nsw i32 %4577, 8
  %4579 = or disjoint i32 %4578, %4564
  %4580 = add nsw i64 %4551, 3
  %4581 = trunc nsw i64 %4580 to i32
  store i32 %4581, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4582 = getelementptr inbounds i8, ptr %4516, i64 %4565
  %4583 = load i8, ptr %4582, align 1, !tbaa !12
  %4584 = shl i32 %4574, 13
  %4585 = xor i32 %4584, %4574
  %4586 = lshr i32 %4585, 17
  %4587 = xor i32 %4586, %4585
  %4588 = shl i32 %4587, 5
  %4589 = xor i32 %4588, %4587
  store i32 %4589, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4590 = trunc i32 %4589 to i8
  %4591 = xor i8 %4583, %4590
  %4592 = zext i8 %4591 to i32
  %4593 = shl nuw nsw i32 %4592, 16
  %4594 = or disjoint i32 %4593, %4579
  %4595 = add i32 %4549, 4
  store i32 %4595, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4596 = getelementptr inbounds i8, ptr %4516, i64 %4580
  %4597 = load i8, ptr %4596, align 1, !tbaa !12
  %4598 = shl i32 %4589, 13
  %4599 = xor i32 %4598, %4589
  %4600 = lshr i32 %4599, 17
  %4601 = xor i32 %4600, %4599
  %4602 = shl i32 %4601, 5
  %4603 = xor i32 %4602, %4601
  store i32 %4603, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4604 = trunc i32 %4603 to i8
  %4605 = xor i8 %4597, %4604
  %4606 = zext i8 %4605 to i32
  %4607 = shl nuw i32 %4606, 24
  %4608 = or disjoint i32 %4607, %4594
  %4609 = icmp eq i32 %4608, 0
  %4610 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  br i1 %4609, label %4650, label %4636

4611:                                             ; preds = %4611, %4540
  %4612 = phi i64 [ %4541, %4540 ], [ %4616, %4611 ]
  %4613 = phi i64 [ 0, %4540 ], [ %4634, %4611 ]
  %4614 = phi i64 [ 0, %4540 ], [ %4633, %4611 ]
  %4615 = phi i32 [ %4537, %4540 ], [ %4625, %4611 ]
  %4616 = add nsw i64 %4612, 1
  %4617 = trunc nsw i64 %4616 to i32
  store i32 %4617, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4618 = getelementptr inbounds i8, ptr %4516, i64 %4612
  %4619 = load i8, ptr %4618, align 1, !tbaa !12
  %4620 = shl i32 %4615, 13
  %4621 = xor i32 %4620, %4615
  %4622 = lshr i32 %4621, 17
  %4623 = xor i32 %4622, %4621
  %4624 = shl i32 %4623, 5
  %4625 = xor i32 %4624, %4623
  store i32 %4625, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4626 = trunc i32 %4625 to i8
  %4627 = xor i8 %4619, %4626
  %4628 = icmp samesign ult i64 %4613, 8
  %4629 = zext i8 %4627 to i64
  %4630 = shl nuw nsw i64 %4613, 3
  %4631 = shl nuw i64 %4629, %4630
  %4632 = select i1 %4628, i64 %4631, i64 0
  %4633 = or i64 %4632, %4614
  %4634 = add nuw nsw i64 %4613, 1
  %4635 = icmp eq i64 %4634, %4542
  br i1 %4635, label %4543, label %4611, !llvm.loop !33

4636:                                             ; preds = %4547
  %4637 = inttoptr i64 %4610 to ptr
  %4638 = load i64, ptr @last_br_from_bb_id__Z8vm_entryv.2, align 8, !tbaa !6
  %4639 = zext nneg i32 %4538 to i64
  %4640 = zext nneg i32 %4538 to i64
  br label %4641

4641:                                             ; preds = %4815, %4636
  %4642 = phi i64 [ 0, %4636 ], [ %4820, %4815 ]
  %4643 = phi i32 [ 0, %4636 ], [ %4821, %4815 ]
  %4644 = phi i32 [ %4595, %4636 ], [ %4817, %4815 ]
  %4645 = phi i32 [ %4603, %4636 ], [ %4816, %4815 ]
  br i1 %4539, label %4646, label %4695

4646:                                             ; preds = %4641
  %4647 = sext i32 %4644 to i64
  br label %4666

4648:                                             ; preds = %4815
  %4649 = phi i64 [ %4820, %4815 ]
  br label %4650

4650:                                             ; preds = %4648, %4547
  %4651 = phi i64 [ 0, %4547 ], [ %4649, %4648 ]
  %4652 = icmp eq i8 %4521, %4529
  br i1 %4652, label %2370, label %4653

4653:                                             ; preds = %4650
  %4654 = add i64 %4610, %4550
  %4655 = inttoptr i64 %4654 to ptr
  %4656 = zext i8 %4530 to i32
  br label %4657

4657:                                             ; preds = %4657, %4653
  %4658 = phi i32 [ %4664, %4657 ], [ 0, %4653 ]
  %4659 = phi ptr [ %4662, %4657 ], [ %4655, %4653 ]
  %4660 = phi i64 [ %4663, %4657 ], [ %4651, %4653 ]
  %4661 = trunc i64 %4660 to i8
  store i8 %4661, ptr %4659, align 1, !tbaa !12
  %4662 = getelementptr inbounds nuw i8, ptr %4659, i64 1
  %4663 = lshr i64 %4660, 8
  %4664 = add nuw nsw i32 %4658, 1
  %4665 = icmp eq i32 %4664, %4656
  br i1 %4665, label %2364, label %4657, !llvm.loop !34

4666:                                             ; preds = %4666, %4646
  %4667 = phi i64 [ %4647, %4646 ], [ %4671, %4666 ]
  %4668 = phi i64 [ 0, %4646 ], [ %4689, %4666 ]
  %4669 = phi i64 [ 0, %4646 ], [ %4688, %4666 ]
  %4670 = phi i32 [ %4645, %4646 ], [ %4680, %4666 ]
  %4671 = add nsw i64 %4667, 1
  %4672 = trunc nsw i64 %4671 to i32
  store i32 %4672, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4673 = getelementptr inbounds i8, ptr %4516, i64 %4667
  %4674 = load i8, ptr %4673, align 1, !tbaa !12
  %4675 = shl i32 %4670, 13
  %4676 = xor i32 %4675, %4670
  %4677 = lshr i32 %4676, 17
  %4678 = xor i32 %4677, %4676
  %4679 = shl i32 %4678, 5
  %4680 = xor i32 %4679, %4678
  store i32 %4680, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4681 = trunc i32 %4680 to i8
  %4682 = xor i8 %4674, %4681
  %4683 = icmp samesign ult i64 %4668, 8
  %4684 = zext i8 %4682 to i64
  %4685 = shl nuw nsw i64 %4668, 3
  %4686 = shl nuw i64 %4684, %4685
  %4687 = select i1 %4683, i64 %4686, i64 0
  %4688 = or i64 %4687, %4669
  %4689 = add nuw nsw i64 %4668, 1
  %4690 = icmp eq i64 %4689, %4639
  br i1 %4690, label %4691, label %4666, !llvm.loop !33

4691:                                             ; preds = %4666
  %4692 = phi i32 [ %4672, %4666 ]
  %4693 = phi i32 [ %4680, %4666 ]
  %4694 = phi i64 [ %4688, %4666 ]
  br label %4695

4695:                                             ; preds = %4691, %4641
  %4696 = phi i32 [ %4645, %4641 ], [ %4693, %4691 ]
  %4697 = phi i32 [ %4644, %4641 ], [ %4692, %4691 ]
  %4698 = phi i64 [ 0, %4641 ], [ %4694, %4691 ]
  %4699 = add nsw i32 %4697, 1
  store i32 %4699, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4700 = sext i32 %4697 to i64
  %4701 = getelementptr inbounds i8, ptr %4516, i64 %4700
  %4702 = load i8, ptr %4701, align 1, !tbaa !12
  %4703 = shl i32 %4696, 13
  %4704 = xor i32 %4703, %4696
  %4705 = lshr i32 %4704, 17
  %4706 = xor i32 %4705, %4704
  %4707 = shl i32 %4706, 5
  %4708 = xor i32 %4707, %4706
  store i32 %4708, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4709 = trunc i32 %4708 to i8
  %4710 = xor i8 %4702, %4709
  %4711 = add i32 %4697, 2
  store i32 %4711, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4712 = sext i32 %4699 to i64
  %4713 = getelementptr inbounds i8, ptr %4516, i64 %4712
  %4714 = load i8, ptr %4713, align 1, !tbaa !12
  %4715 = shl i32 %4708, 13
  %4716 = xor i32 %4715, %4708
  %4717 = lshr i32 %4716, 17
  %4718 = xor i32 %4717, %4716
  %4719 = shl i32 %4718, 5
  %4720 = xor i32 %4719, %4718
  store i32 %4720, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4721 = trunc i32 %4720 to i8
  %4722 = icmp eq i8 %4714, %4721
  br i1 %4722, label %4723, label %4779

4723:                                             ; preds = %4695
  br i1 %4539, label %4724, label %4755

4724:                                             ; preds = %4723
  %4725 = sext i32 %4711 to i64
  br label %4726

4726:                                             ; preds = %4726, %4724
  %4727 = phi i64 [ %4725, %4724 ], [ %4731, %4726 ]
  %4728 = phi i64 [ 0, %4724 ], [ %4749, %4726 ]
  %4729 = phi i64 [ 0, %4724 ], [ %4748, %4726 ]
  %4730 = phi i32 [ %4720, %4724 ], [ %4740, %4726 ]
  %4731 = add nsw i64 %4727, 1
  %4732 = trunc nsw i64 %4731 to i32
  store i32 %4732, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4733 = getelementptr inbounds i8, ptr %4516, i64 %4727
  %4734 = load i8, ptr %4733, align 1, !tbaa !12
  %4735 = shl i32 %4730, 13
  %4736 = xor i32 %4735, %4730
  %4737 = lshr i32 %4736, 17
  %4738 = xor i32 %4737, %4736
  %4739 = shl i32 %4738, 5
  %4740 = xor i32 %4739, %4738
  store i32 %4740, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4741 = trunc i32 %4740 to i8
  %4742 = xor i8 %4734, %4741
  %4743 = icmp samesign ult i64 %4728, 8
  %4744 = zext i8 %4742 to i64
  %4745 = shl nuw nsw i64 %4728, 3
  %4746 = shl nuw i64 %4744, %4745
  %4747 = select i1 %4743, i64 %4746, i64 0
  %4748 = or i64 %4747, %4729
  %4749 = add nuw nsw i64 %4728, 1
  %4750 = icmp eq i64 %4749, %4640
  br i1 %4750, label %4751, label %4726, !llvm.loop !33

4751:                                             ; preds = %4726
  %4752 = phi i32 [ %4732, %4726 ]
  %4753 = phi i32 [ %4740, %4726 ]
  %4754 = phi i64 [ %4748, %4726 ]
  br label %4755

4755:                                             ; preds = %4751, %4723
  %4756 = phi i32 [ %4720, %4723 ], [ %4753, %4751 ]
  %4757 = phi i32 [ %4711, %4723 ], [ %4752, %4751 ]
  %4758 = phi i64 [ 0, %4723 ], [ %4754, %4751 ]
  %4759 = icmp eq i8 %4702, %4709
  br i1 %4759, label %4815, label %4760

4760:                                             ; preds = %4755
  %4761 = zext i8 %4710 to i64
  br label %4762

4762:                                             ; preds = %4775, %4760
  %4763 = phi i64 [ 0, %4760 ], [ %4777, %4775 ]
  %4764 = phi i64 [ %4758, %4760 ], [ %4766, %4775 ]
  %4765 = phi i64 [ 0, %4760 ], [ %4776, %4775 ]
  %4766 = add i64 %4764, 1
  %4767 = icmp samesign ult i64 %4763, 8
  br i1 %4767, label %4768, label %4775

4768:                                             ; preds = %4762
  %4769 = getelementptr inbounds nuw i8, ptr %4637, i64 %4764
  %4770 = load i8, ptr %4769, align 1, !tbaa !12
  %4771 = zext i8 %4770 to i64
  %4772 = shl nuw nsw i64 %4763, 3
  %4773 = shl nuw i64 %4771, %4772
  %4774 = or i64 %4773, %4765
  br label %4775

4775:                                             ; preds = %4768, %4762
  %4776 = phi i64 [ %4774, %4768 ], [ %4765, %4762 ]
  %4777 = add nuw nsw i64 %4763, 1
  %4778 = icmp eq i64 %4777, %4761
  br i1 %4778, label %4809, label %4762, !llvm.loop !35

4779:                                             ; preds = %4695
  %4780 = icmp eq i8 %4702, %4709
  br i1 %4780, label %4815, label %4781

4781:                                             ; preds = %4779
  %4782 = sext i32 %4711 to i64
  %4783 = zext i8 %4710 to i64
  br label %4784

4784:                                             ; preds = %4784, %4781
  %4785 = phi i64 [ %4782, %4781 ], [ %4789, %4784 ]
  %4786 = phi i64 [ 0, %4781 ], [ %4807, %4784 ]
  %4787 = phi i64 [ 0, %4781 ], [ %4806, %4784 ]
  %4788 = phi i32 [ %4720, %4781 ], [ %4798, %4784 ]
  %4789 = add nsw i64 %4785, 1
  %4790 = trunc nsw i64 %4789 to i32
  store i32 %4790, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4791 = getelementptr inbounds i8, ptr %4516, i64 %4785
  %4792 = load i8, ptr %4791, align 1, !tbaa !12
  %4793 = shl i32 %4788, 13
  %4794 = xor i32 %4793, %4788
  %4795 = lshr i32 %4794, 17
  %4796 = xor i32 %4795, %4794
  %4797 = shl i32 %4796, 5
  %4798 = xor i32 %4797, %4796
  store i32 %4798, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4799 = trunc i32 %4798 to i8
  %4800 = xor i8 %4792, %4799
  %4801 = icmp samesign ult i64 %4786, 8
  %4802 = zext i8 %4800 to i64
  %4803 = shl nuw nsw i64 %4786, 3
  %4804 = shl nuw i64 %4802, %4803
  %4805 = select i1 %4801, i64 %4804, i64 0
  %4806 = or i64 %4805, %4787
  %4807 = add nuw nsw i64 %4786, 1
  %4808 = icmp eq i64 %4807, %4783
  br i1 %4808, label %4811, label %4784, !llvm.loop !33

4809:                                             ; preds = %4775
  %4810 = phi i64 [ %4776, %4775 ]
  br label %4815

4811:                                             ; preds = %4784
  %4812 = phi i32 [ %4790, %4784 ]
  %4813 = phi i32 [ %4798, %4784 ]
  %4814 = phi i64 [ %4806, %4784 ]
  br label %4815

4815:                                             ; preds = %4811, %4809, %4779, %4755
  %4816 = phi i32 [ %4756, %4755 ], [ %4720, %4779 ], [ %4756, %4809 ], [ %4813, %4811 ]
  %4817 = phi i32 [ %4757, %4755 ], [ %4711, %4779 ], [ %4757, %4809 ], [ %4812, %4811 ]
  %4818 = phi i64 [ 0, %4755 ], [ 0, %4779 ], [ %4810, %4809 ], [ %4814, %4811 ]
  %4819 = icmp eq i64 %4698, %4638
  %4820 = select i1 %4819, i64 %4818, i64 %4642
  %4821 = add nuw i32 %4643, 1
  %4822 = icmp eq i32 %4821, %4608
  br i1 %4822, label %4648, label %4641, !llvm.loop !53

4823:                                             ; preds = %239
  %4824 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %4825 = inttoptr i64 %4824 to ptr
  %4826 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4827 = add nsw i32 %4826, 1
  store i32 %4827, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4828 = sext i32 %4826 to i64
  %4829 = getelementptr inbounds i8, ptr %4825, i64 %4828
  %4830 = load i8, ptr %4829, align 1, !tbaa !12
  %4831 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4832 = shl i32 %4831, 13
  %4833 = xor i32 %4832, %4831
  %4834 = lshr i32 %4833, 17
  %4835 = xor i32 %4834, %4833
  %4836 = shl i32 %4835, 5
  %4837 = xor i32 %4836, %4835
  %4838 = trunc i32 %4837 to i8
  %4839 = xor i8 %4830, %4838
  %4840 = add nsw i32 %4826, 2
  %4841 = shl i32 %4837, 13
  %4842 = xor i32 %4841, %4837
  %4843 = lshr i32 %4842, 17
  %4844 = xor i32 %4843, %4842
  %4845 = shl i32 %4844, 5
  %4846 = xor i32 %4845, %4844
  store i32 %4846, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4847 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %4848 = icmp sgt i32 %4847, 0
  br i1 %4848, label %4849, label %4881

4849:                                             ; preds = %4823
  %4850 = sext i32 %4840 to i64
  %4851 = zext nneg i32 %4847 to i64
  br label %4852

4852:                                             ; preds = %4852, %4849
  %4853 = phi i64 [ %4850, %4849 ], [ %4857, %4852 ]
  %4854 = phi i64 [ 0, %4849 ], [ %4875, %4852 ]
  %4855 = phi i64 [ 0, %4849 ], [ %4874, %4852 ]
  %4856 = phi i32 [ %4846, %4849 ], [ %4866, %4852 ]
  %4857 = add nsw i64 %4853, 1
  %4858 = trunc nsw i64 %4857 to i32
  store i32 %4858, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4859 = getelementptr inbounds i8, ptr %4825, i64 %4853
  %4860 = load i8, ptr %4859, align 1, !tbaa !12
  %4861 = shl i32 %4856, 13
  %4862 = xor i32 %4861, %4856
  %4863 = lshr i32 %4862, 17
  %4864 = xor i32 %4863, %4862
  %4865 = shl i32 %4864, 5
  %4866 = xor i32 %4865, %4864
  store i32 %4866, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4867 = trunc i32 %4866 to i8
  %4868 = xor i8 %4860, %4867
  %4869 = icmp samesign ult i64 %4854, 8
  %4870 = zext i8 %4868 to i64
  %4871 = shl nuw nsw i64 %4854, 3
  %4872 = shl nuw i64 %4870, %4871
  %4873 = select i1 %4869, i64 %4872, i64 0
  %4874 = or i64 %4873, %4855
  %4875 = add nuw nsw i64 %4854, 1
  %4876 = icmp eq i64 %4875, %4851
  br i1 %4876, label %4877, label %4852, !llvm.loop !33

4877:                                             ; preds = %4852
  %4878 = phi i32 [ %4858, %4852 ]
  %4879 = phi i32 [ %4866, %4852 ]
  %4880 = phi i64 [ %4874, %4852 ]
  br label %4881

4881:                                             ; preds = %4877, %4823
  %4882 = phi i32 [ %4846, %4823 ], [ %4879, %4877 ]
  %4883 = phi i32 [ %4840, %4823 ], [ %4878, %4877 ]
  %4884 = phi i64 [ 0, %4823 ], [ %4880, %4877 ]
  %4885 = add nsw i32 %4883, 1
  store i32 %4885, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4886 = sext i32 %4883 to i64
  %4887 = getelementptr inbounds i8, ptr %4825, i64 %4886
  %4888 = load i8, ptr %4887, align 1, !tbaa !12
  %4889 = shl i32 %4882, 13
  %4890 = xor i32 %4889, %4882
  %4891 = lshr i32 %4890, 17
  %4892 = xor i32 %4891, %4890
  %4893 = shl i32 %4892, 5
  %4894 = xor i32 %4893, %4892
  store i32 %4894, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4895 = trunc i32 %4894 to i8
  %4896 = xor i8 %4888, %4895
  %4897 = add nsw i32 %4883, 2
  store i32 %4897, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4898 = sext i32 %4885 to i64
  %4899 = getelementptr inbounds i8, ptr %4825, i64 %4898
  %4900 = load i8, ptr %4899, align 1, !tbaa !12
  %4901 = shl i32 %4894, 13
  %4902 = xor i32 %4901, %4894
  %4903 = lshr i32 %4902, 17
  %4904 = xor i32 %4903, %4902
  %4905 = shl i32 %4904, 5
  %4906 = xor i32 %4905, %4904
  store i32 %4906, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4907 = trunc i32 %4906 to i8
  %4908 = icmp eq i8 %4900, %4907
  br i1 %4908, label %4909, label %4968

4909:                                             ; preds = %4881
  br i1 %4848, label %4910, label %4942

4910:                                             ; preds = %4909
  %4911 = sext i32 %4897 to i64
  %4912 = zext nneg i32 %4847 to i64
  br label %4913

4913:                                             ; preds = %4913, %4910
  %4914 = phi i64 [ %4911, %4910 ], [ %4918, %4913 ]
  %4915 = phi i64 [ 0, %4910 ], [ %4936, %4913 ]
  %4916 = phi i64 [ 0, %4910 ], [ %4935, %4913 ]
  %4917 = phi i32 [ %4906, %4910 ], [ %4927, %4913 ]
  %4918 = add nsw i64 %4914, 1
  %4919 = trunc nsw i64 %4918 to i32
  store i32 %4919, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4920 = getelementptr inbounds i8, ptr %4825, i64 %4914
  %4921 = load i8, ptr %4920, align 1, !tbaa !12
  %4922 = shl i32 %4917, 13
  %4923 = xor i32 %4922, %4917
  %4924 = lshr i32 %4923, 17
  %4925 = xor i32 %4924, %4923
  %4926 = shl i32 %4925, 5
  %4927 = xor i32 %4926, %4925
  store i32 %4927, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4928 = trunc i32 %4927 to i8
  %4929 = xor i8 %4921, %4928
  %4930 = icmp samesign ult i64 %4915, 8
  %4931 = zext i8 %4929 to i64
  %4932 = shl nuw nsw i64 %4915, 3
  %4933 = shl nuw i64 %4931, %4932
  %4934 = select i1 %4930, i64 %4933, i64 0
  %4935 = or i64 %4934, %4916
  %4936 = add nuw nsw i64 %4915, 1
  %4937 = icmp eq i64 %4936, %4912
  br i1 %4937, label %4938, label %4913, !llvm.loop !33

4938:                                             ; preds = %4913
  %4939 = phi i32 [ %4919, %4913 ]
  %4940 = phi i32 [ %4927, %4913 ]
  %4941 = phi i64 [ %4935, %4913 ]
  br label %4942

4942:                                             ; preds = %4938, %4909
  %4943 = phi i32 [ %4906, %4909 ], [ %4940, %4938 ]
  %4944 = phi i32 [ %4897, %4909 ], [ %4939, %4938 ]
  %4945 = phi i64 [ 0, %4909 ], [ %4941, %4938 ]
  %4946 = icmp eq i8 %4888, %4895
  br i1 %4946, label %5004, label %4947

4947:                                             ; preds = %4942
  %4948 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %4949 = inttoptr i64 %4948 to ptr
  %4950 = zext i8 %4896 to i64
  br label %4951

4951:                                             ; preds = %4964, %4947
  %4952 = phi i64 [ 0, %4947 ], [ %4966, %4964 ]
  %4953 = phi i64 [ %4945, %4947 ], [ %4955, %4964 ]
  %4954 = phi i64 [ 0, %4947 ], [ %4965, %4964 ]
  %4955 = add i64 %4953, 1
  %4956 = icmp samesign ult i64 %4952, 8
  br i1 %4956, label %4957, label %4964

4957:                                             ; preds = %4951
  %4958 = getelementptr inbounds nuw i8, ptr %4949, i64 %4953
  %4959 = load i8, ptr %4958, align 1, !tbaa !12
  %4960 = zext i8 %4959 to i64
  %4961 = shl nuw nsw i64 %4952, 3
  %4962 = shl nuw i64 %4960, %4961
  %4963 = or i64 %4962, %4954
  br label %4964

4964:                                             ; preds = %4957, %4951
  %4965 = phi i64 [ %4963, %4957 ], [ %4954, %4951 ]
  %4966 = add nuw nsw i64 %4952, 1
  %4967 = icmp eq i64 %4966, %4950
  br i1 %4967, label %4998, label %4951, !llvm.loop !35

4968:                                             ; preds = %4881
  %4969 = icmp eq i8 %4888, %4895
  br i1 %4969, label %5004, label %4970

4970:                                             ; preds = %4968
  %4971 = sext i32 %4897 to i64
  %4972 = zext i8 %4896 to i64
  br label %4973

4973:                                             ; preds = %4973, %4970
  %4974 = phi i64 [ %4971, %4970 ], [ %4978, %4973 ]
  %4975 = phi i64 [ 0, %4970 ], [ %4996, %4973 ]
  %4976 = phi i64 [ 0, %4970 ], [ %4995, %4973 ]
  %4977 = phi i32 [ %4906, %4970 ], [ %4987, %4973 ]
  %4978 = add nsw i64 %4974, 1
  %4979 = trunc nsw i64 %4978 to i32
  store i32 %4979, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %4980 = getelementptr inbounds i8, ptr %4825, i64 %4974
  %4981 = load i8, ptr %4980, align 1, !tbaa !12
  %4982 = shl i32 %4977, 13
  %4983 = xor i32 %4982, %4977
  %4984 = lshr i32 %4983, 17
  %4985 = xor i32 %4984, %4983
  %4986 = shl i32 %4985, 5
  %4987 = xor i32 %4986, %4985
  store i32 %4987, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %4988 = trunc i32 %4987 to i8
  %4989 = xor i8 %4981, %4988
  %4990 = icmp samesign ult i64 %4975, 8
  %4991 = zext i8 %4989 to i64
  %4992 = shl nuw nsw i64 %4975, 3
  %4993 = shl nuw i64 %4991, %4992
  %4994 = select i1 %4990, i64 %4993, i64 0
  %4995 = or i64 %4994, %4976
  %4996 = add nuw nsw i64 %4975, 1
  %4997 = icmp eq i64 %4996, %4972
  br i1 %4997, label %5000, label %4973, !llvm.loop !33

4998:                                             ; preds = %4964
  %4999 = phi i64 [ %4965, %4964 ]
  br label %5004

5000:                                             ; preds = %4973
  %5001 = phi i32 [ %4979, %4973 ]
  %5002 = phi i32 [ %4987, %4973 ]
  %5003 = phi i64 [ %4995, %4973 ]
  br label %5004

5004:                                             ; preds = %5000, %4998, %4968, %4942
  %5005 = phi i32 [ %4943, %4942 ], [ %4906, %4968 ], [ %4943, %4998 ], [ %5002, %5000 ]
  %5006 = phi i32 [ %4944, %4942 ], [ %4897, %4968 ], [ %4944, %4998 ], [ %5001, %5000 ]
  %5007 = phi i64 [ 0, %4942 ], [ 0, %4968 ], [ %4999, %4998 ], [ %5003, %5000 ]
  %5008 = add nsw i32 %5006, 1
  store i32 %5008, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5009 = sext i32 %5006 to i64
  %5010 = getelementptr inbounds i8, ptr %4825, i64 %5009
  %5011 = load i8, ptr %5010, align 1, !tbaa !12
  %5012 = shl i32 %5005, 13
  %5013 = xor i32 %5012, %5005
  %5014 = lshr i32 %5013, 17
  %5015 = xor i32 %5014, %5013
  %5016 = shl i32 %5015, 5
  %5017 = xor i32 %5016, %5015
  store i32 %5017, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5018 = trunc i32 %5017 to i8
  %5019 = xor i8 %5011, %5018
  %5020 = add nsw i32 %5006, 2
  store i32 %5020, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5021 = sext i32 %5008 to i64
  %5022 = getelementptr inbounds i8, ptr %4825, i64 %5021
  %5023 = load i8, ptr %5022, align 1, !tbaa !12
  %5024 = shl i32 %5017, 13
  %5025 = xor i32 %5024, %5017
  %5026 = lshr i32 %5025, 17
  %5027 = xor i32 %5026, %5025
  %5028 = shl i32 %5027, 5
  %5029 = xor i32 %5028, %5027
  store i32 %5029, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5030 = trunc i32 %5029 to i8
  %5031 = icmp eq i8 %5023, %5030
  br i1 %5031, label %5032, label %5091

5032:                                             ; preds = %5004
  br i1 %4848, label %5033, label %5065

5033:                                             ; preds = %5032
  %5034 = sext i32 %5020 to i64
  %5035 = zext nneg i32 %4847 to i64
  br label %5036

5036:                                             ; preds = %5036, %5033
  %5037 = phi i64 [ %5034, %5033 ], [ %5041, %5036 ]
  %5038 = phi i64 [ 0, %5033 ], [ %5059, %5036 ]
  %5039 = phi i64 [ 0, %5033 ], [ %5058, %5036 ]
  %5040 = phi i32 [ %5029, %5033 ], [ %5050, %5036 ]
  %5041 = add nsw i64 %5037, 1
  %5042 = trunc nsw i64 %5041 to i32
  store i32 %5042, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5043 = getelementptr inbounds i8, ptr %4825, i64 %5037
  %5044 = load i8, ptr %5043, align 1, !tbaa !12
  %5045 = shl i32 %5040, 13
  %5046 = xor i32 %5045, %5040
  %5047 = lshr i32 %5046, 17
  %5048 = xor i32 %5047, %5046
  %5049 = shl i32 %5048, 5
  %5050 = xor i32 %5049, %5048
  store i32 %5050, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5051 = trunc i32 %5050 to i8
  %5052 = xor i8 %5044, %5051
  %5053 = icmp samesign ult i64 %5038, 8
  %5054 = zext i8 %5052 to i64
  %5055 = shl nuw nsw i64 %5038, 3
  %5056 = shl nuw i64 %5054, %5055
  %5057 = select i1 %5053, i64 %5056, i64 0
  %5058 = or i64 %5057, %5039
  %5059 = add nuw nsw i64 %5038, 1
  %5060 = icmp eq i64 %5059, %5035
  br i1 %5060, label %5061, label %5036, !llvm.loop !33

5061:                                             ; preds = %5036
  %5062 = phi i32 [ %5042, %5036 ]
  %5063 = phi i32 [ %5050, %5036 ]
  %5064 = phi i64 [ %5058, %5036 ]
  br label %5065

5065:                                             ; preds = %5061, %5032
  %5066 = phi i32 [ %5029, %5032 ], [ %5063, %5061 ]
  %5067 = phi i32 [ %5020, %5032 ], [ %5062, %5061 ]
  %5068 = phi i64 [ 0, %5032 ], [ %5064, %5061 ]
  %5069 = icmp eq i8 %5011, %5018
  br i1 %5069, label %5127, label %5070

5070:                                             ; preds = %5065
  %5071 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %5072 = inttoptr i64 %5071 to ptr
  %5073 = zext i8 %5019 to i64
  br label %5074

5074:                                             ; preds = %5087, %5070
  %5075 = phi i64 [ 0, %5070 ], [ %5089, %5087 ]
  %5076 = phi i64 [ %5068, %5070 ], [ %5078, %5087 ]
  %5077 = phi i64 [ 0, %5070 ], [ %5088, %5087 ]
  %5078 = add i64 %5076, 1
  %5079 = icmp samesign ult i64 %5075, 8
  br i1 %5079, label %5080, label %5087

5080:                                             ; preds = %5074
  %5081 = getelementptr inbounds nuw i8, ptr %5072, i64 %5076
  %5082 = load i8, ptr %5081, align 1, !tbaa !12
  %5083 = zext i8 %5082 to i64
  %5084 = shl nuw nsw i64 %5075, 3
  %5085 = shl nuw i64 %5083, %5084
  %5086 = or i64 %5085, %5077
  br label %5087

5087:                                             ; preds = %5080, %5074
  %5088 = phi i64 [ %5086, %5080 ], [ %5077, %5074 ]
  %5089 = add nuw nsw i64 %5075, 1
  %5090 = icmp eq i64 %5089, %5073
  br i1 %5090, label %5121, label %5074, !llvm.loop !35

5091:                                             ; preds = %5004
  %5092 = icmp eq i8 %5011, %5018
  br i1 %5092, label %5127, label %5093

5093:                                             ; preds = %5091
  %5094 = sext i32 %5020 to i64
  %5095 = zext i8 %5019 to i64
  br label %5096

5096:                                             ; preds = %5096, %5093
  %5097 = phi i64 [ %5094, %5093 ], [ %5101, %5096 ]
  %5098 = phi i64 [ 0, %5093 ], [ %5119, %5096 ]
  %5099 = phi i64 [ 0, %5093 ], [ %5118, %5096 ]
  %5100 = phi i32 [ %5029, %5093 ], [ %5110, %5096 ]
  %5101 = add nsw i64 %5097, 1
  %5102 = trunc nsw i64 %5101 to i32
  store i32 %5102, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5103 = getelementptr inbounds i8, ptr %4825, i64 %5097
  %5104 = load i8, ptr %5103, align 1, !tbaa !12
  %5105 = shl i32 %5100, 13
  %5106 = xor i32 %5105, %5100
  %5107 = lshr i32 %5106, 17
  %5108 = xor i32 %5107, %5106
  %5109 = shl i32 %5108, 5
  %5110 = xor i32 %5109, %5108
  store i32 %5110, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5111 = trunc i32 %5110 to i8
  %5112 = xor i8 %5104, %5111
  %5113 = icmp samesign ult i64 %5098, 8
  %5114 = zext i8 %5112 to i64
  %5115 = shl nuw nsw i64 %5098, 3
  %5116 = shl nuw i64 %5114, %5115
  %5117 = select i1 %5113, i64 %5116, i64 0
  %5118 = or i64 %5117, %5099
  %5119 = add nuw nsw i64 %5098, 1
  %5120 = icmp eq i64 %5119, %5095
  br i1 %5120, label %5123, label %5096, !llvm.loop !33

5121:                                             ; preds = %5087
  %5122 = phi i64 [ %5088, %5087 ]
  br label %5127

5123:                                             ; preds = %5096
  %5124 = phi i32 [ %5102, %5096 ]
  %5125 = phi i32 [ %5110, %5096 ]
  %5126 = phi i64 [ %5118, %5096 ]
  br label %5127

5127:                                             ; preds = %5123, %5121, %5091, %5065
  %5128 = phi i32 [ %5066, %5065 ], [ %5029, %5091 ], [ %5066, %5121 ], [ %5125, %5123 ]
  %5129 = phi i32 [ %5067, %5065 ], [ %5020, %5091 ], [ %5067, %5121 ], [ %5124, %5123 ]
  %5130 = phi i64 [ 0, %5065 ], [ 0, %5091 ], [ %5122, %5121 ], [ %5126, %5123 ]
  %5131 = add nsw i32 %5129, 1
  store i32 %5131, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5132 = sext i32 %5129 to i64
  %5133 = getelementptr inbounds i8, ptr %4825, i64 %5132
  %5134 = load i8, ptr %5133, align 1, !tbaa !12
  %5135 = shl i32 %5128, 13
  %5136 = xor i32 %5135, %5128
  %5137 = lshr i32 %5136, 17
  %5138 = xor i32 %5137, %5136
  %5139 = shl i32 %5138, 5
  %5140 = xor i32 %5139, %5138
  store i32 %5140, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5141 = trunc i32 %5140 to i8
  %5142 = xor i8 %5134, %5141
  %5143 = add nsw i32 %5129, 2
  store i32 %5143, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5144 = sext i32 %5131 to i64
  %5145 = getelementptr inbounds i8, ptr %4825, i64 %5144
  %5146 = load i8, ptr %5145, align 1, !tbaa !12
  %5147 = shl i32 %5140, 13
  %5148 = xor i32 %5147, %5140
  %5149 = lshr i32 %5148, 17
  %5150 = xor i32 %5149, %5148
  %5151 = shl i32 %5150, 5
  %5152 = xor i32 %5151, %5150
  store i32 %5152, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5153 = trunc i32 %5152 to i8
  %5154 = icmp eq i8 %5146, %5153
  br i1 %5154, label %5155, label %5210

5155:                                             ; preds = %5127
  br i1 %4848, label %5156, label %5186

5156:                                             ; preds = %5155
  %5157 = sext i32 %5143 to i64
  %5158 = zext nneg i32 %4847 to i64
  br label %5159

5159:                                             ; preds = %5159, %5156
  %5160 = phi i64 [ %5157, %5156 ], [ %5164, %5159 ]
  %5161 = phi i64 [ 0, %5156 ], [ %5182, %5159 ]
  %5162 = phi i64 [ 0, %5156 ], [ %5181, %5159 ]
  %5163 = phi i32 [ %5152, %5156 ], [ %5173, %5159 ]
  %5164 = add nsw i64 %5160, 1
  %5165 = trunc nsw i64 %5164 to i32
  store i32 %5165, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5166 = getelementptr inbounds i8, ptr %4825, i64 %5160
  %5167 = load i8, ptr %5166, align 1, !tbaa !12
  %5168 = shl i32 %5163, 13
  %5169 = xor i32 %5168, %5163
  %5170 = lshr i32 %5169, 17
  %5171 = xor i32 %5170, %5169
  %5172 = shl i32 %5171, 5
  %5173 = xor i32 %5172, %5171
  store i32 %5173, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5174 = trunc i32 %5173 to i8
  %5175 = xor i8 %5167, %5174
  %5176 = icmp samesign ult i64 %5161, 8
  %5177 = zext i8 %5175 to i64
  %5178 = shl nuw nsw i64 %5161, 3
  %5179 = shl nuw i64 %5177, %5178
  %5180 = select i1 %5176, i64 %5179, i64 0
  %5181 = or i64 %5180, %5162
  %5182 = add nuw nsw i64 %5161, 1
  %5183 = icmp eq i64 %5182, %5158
  br i1 %5183, label %5184, label %5159, !llvm.loop !33

5184:                                             ; preds = %5159
  %5185 = phi i64 [ %5181, %5159 ]
  br label %5186

5186:                                             ; preds = %5184, %5155
  %5187 = phi i64 [ 0, %5155 ], [ %5185, %5184 ]
  %5188 = icmp eq i8 %5134, %5141
  br i1 %5188, label %5244, label %5189

5189:                                             ; preds = %5186
  %5190 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %5191 = inttoptr i64 %5190 to ptr
  %5192 = zext i8 %5142 to i64
  br label %5193

5193:                                             ; preds = %5206, %5189
  %5194 = phi i64 [ 0, %5189 ], [ %5208, %5206 ]
  %5195 = phi i64 [ %5187, %5189 ], [ %5197, %5206 ]
  %5196 = phi i64 [ 0, %5189 ], [ %5207, %5206 ]
  %5197 = add i64 %5195, 1
  %5198 = icmp samesign ult i64 %5194, 8
  br i1 %5198, label %5199, label %5206

5199:                                             ; preds = %5193
  %5200 = getelementptr inbounds nuw i8, ptr %5191, i64 %5195
  %5201 = load i8, ptr %5200, align 1, !tbaa !12
  %5202 = zext i8 %5201 to i64
  %5203 = shl nuw nsw i64 %5194, 3
  %5204 = shl nuw i64 %5202, %5203
  %5205 = or i64 %5204, %5196
  br label %5206

5206:                                             ; preds = %5199, %5193
  %5207 = phi i64 [ %5205, %5199 ], [ %5196, %5193 ]
  %5208 = add nuw nsw i64 %5194, 1
  %5209 = icmp eq i64 %5208, %5192
  br i1 %5209, label %5240, label %5193, !llvm.loop !35

5210:                                             ; preds = %5127
  %5211 = icmp eq i8 %5134, %5141
  br i1 %5211, label %5244, label %5212

5212:                                             ; preds = %5210
  %5213 = sext i32 %5143 to i64
  %5214 = zext i8 %5142 to i64
  br label %5215

5215:                                             ; preds = %5215, %5212
  %5216 = phi i64 [ %5213, %5212 ], [ %5220, %5215 ]
  %5217 = phi i64 [ 0, %5212 ], [ %5238, %5215 ]
  %5218 = phi i64 [ 0, %5212 ], [ %5237, %5215 ]
  %5219 = phi i32 [ %5152, %5212 ], [ %5229, %5215 ]
  %5220 = add nsw i64 %5216, 1
  %5221 = trunc nsw i64 %5220 to i32
  store i32 %5221, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5222 = getelementptr inbounds i8, ptr %4825, i64 %5216
  %5223 = load i8, ptr %5222, align 1, !tbaa !12
  %5224 = shl i32 %5219, 13
  %5225 = xor i32 %5224, %5219
  %5226 = lshr i32 %5225, 17
  %5227 = xor i32 %5226, %5225
  %5228 = shl i32 %5227, 5
  %5229 = xor i32 %5228, %5227
  store i32 %5229, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5230 = trunc i32 %5229 to i8
  %5231 = xor i8 %5223, %5230
  %5232 = icmp samesign ult i64 %5217, 8
  %5233 = zext i8 %5231 to i64
  %5234 = shl nuw nsw i64 %5217, 3
  %5235 = shl nuw i64 %5233, %5234
  %5236 = select i1 %5232, i64 %5235, i64 0
  %5237 = or i64 %5236, %5218
  %5238 = add nuw nsw i64 %5217, 1
  %5239 = icmp eq i64 %5238, %5214
  br i1 %5239, label %5242, label %5215, !llvm.loop !33

5240:                                             ; preds = %5206
  %5241 = phi i64 [ %5207, %5206 ]
  br label %5244

5242:                                             ; preds = %5215
  %5243 = phi i64 [ %5237, %5215 ]
  br label %5244

5244:                                             ; preds = %5242, %5240, %5210, %5186
  %5245 = phi i64 [ 0, %5186 ], [ 0, %5210 ], [ %5241, %5240 ], [ %5243, %5242 ]
  %5246 = icmp eq i8 %4830, %4838
  br i1 %5246, label %2370, label %5247

5247:                                             ; preds = %5244
  %5248 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5249 = add i64 %5248, %4884
  %5250 = inttoptr i64 %5249 to ptr
  %5251 = zext i8 %4839 to i32
  %5252 = icmp eq i64 %5007, 0
  %5253 = select i1 %5252, i64 %5245, i64 %5130
  br label %5254

5254:                                             ; preds = %5254, %5247
  %5255 = phi i32 [ %5261, %5254 ], [ 0, %5247 ]
  %5256 = phi ptr [ %5259, %5254 ], [ %5250, %5247 ]
  %5257 = phi i64 [ %5260, %5254 ], [ %5253, %5247 ]
  %5258 = trunc i64 %5257 to i8
  store i8 %5258, ptr %5256, align 1, !tbaa !12
  %5259 = getelementptr inbounds nuw i8, ptr %5256, i64 1
  %5260 = lshr i64 %5257, 8
  %5261 = add nuw nsw i32 %5255, 1
  %5262 = icmp eq i32 %5261, %5251
  br i1 %5262, label %2365, label %5254, !llvm.loop !34

5263:                                             ; preds = %239
  %5264 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5265 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5266 = shl i32 %5265, 13
  %5267 = xor i32 %5266, %5265
  %5268 = lshr i32 %5267, 17
  %5269 = xor i32 %5268, %5267
  %5270 = shl i32 %5269, 5
  %5271 = xor i32 %5270, %5269
  %5272 = add nsw i32 %5264, 2
  %5273 = shl i32 %5271, 13
  %5274 = xor i32 %5273, %5271
  %5275 = lshr i32 %5274, 17
  %5276 = xor i32 %5275, %5274
  %5277 = shl i32 %5276, 5
  %5278 = xor i32 %5277, %5276
  store i32 %5278, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5279 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %5280 = icmp sgt i32 %5279, 0
  %5281 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5282 = inttoptr i64 %5281 to ptr
  br i1 %5280, label %5283, label %5290

5283:                                             ; preds = %5263
  %5284 = sext i32 %5272 to i64
  %5285 = zext nneg i32 %5279 to i64
  br label %5356

5286:                                             ; preds = %5356
  %5287 = phi i32 [ %5362, %5356 ]
  %5288 = phi i32 [ %5370, %5356 ]
  %5289 = phi i64 [ %5378, %5356 ]
  br label %5290

5290:                                             ; preds = %5286, %5263
  %5291 = phi i32 [ %5278, %5263 ], [ %5288, %5286 ]
  %5292 = phi i32 [ %5272, %5263 ], [ %5287, %5286 ]
  %5293 = phi i64 [ 0, %5263 ], [ %5289, %5286 ]
  %5294 = sext i32 %5292 to i64
  %5295 = add nsw i64 %5294, 1
  %5296 = trunc nsw i64 %5295 to i32
  store i32 %5296, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5297 = getelementptr inbounds i8, ptr %5282, i64 %5294
  %5298 = load i8, ptr %5297, align 1, !tbaa !12
  %5299 = shl i32 %5291, 13
  %5300 = xor i32 %5299, %5291
  %5301 = lshr i32 %5300, 17
  %5302 = xor i32 %5301, %5300
  %5303 = shl i32 %5302, 5
  %5304 = xor i32 %5303, %5302
  store i32 %5304, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5305 = trunc i32 %5304 to i8
  %5306 = xor i8 %5298, %5305
  %5307 = zext i8 %5306 to i64
  %5308 = add nsw i64 %5294, 2
  %5309 = trunc nsw i64 %5308 to i32
  store i32 %5309, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5310 = getelementptr inbounds i8, ptr %5282, i64 %5295
  %5311 = load i8, ptr %5310, align 1, !tbaa !12
  %5312 = shl i32 %5304, 13
  %5313 = xor i32 %5312, %5304
  %5314 = lshr i32 %5313, 17
  %5315 = xor i32 %5314, %5313
  %5316 = shl i32 %5315, 5
  %5317 = xor i32 %5316, %5315
  store i32 %5317, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5318 = trunc i32 %5317 to i8
  %5319 = xor i8 %5311, %5318
  %5320 = zext i8 %5319 to i64
  %5321 = shl nuw nsw i64 %5320, 8
  %5322 = or disjoint i64 %5321, %5307
  %5323 = add nsw i64 %5294, 3
  %5324 = trunc nsw i64 %5323 to i32
  store i32 %5324, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5325 = getelementptr inbounds i8, ptr %5282, i64 %5308
  %5326 = load i8, ptr %5325, align 1, !tbaa !12
  %5327 = shl i32 %5317, 13
  %5328 = xor i32 %5327, %5317
  %5329 = lshr i32 %5328, 17
  %5330 = xor i32 %5329, %5328
  %5331 = shl i32 %5330, 5
  %5332 = xor i32 %5331, %5330
  store i32 %5332, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5333 = trunc i32 %5332 to i8
  %5334 = xor i8 %5326, %5333
  %5335 = zext i8 %5334 to i64
  %5336 = shl nuw nsw i64 %5335, 16
  %5337 = add i32 %5292, 4
  store i32 %5337, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5338 = getelementptr inbounds i8, ptr %5282, i64 %5323
  %5339 = load i8, ptr %5338, align 1, !tbaa !12
  %5340 = shl i32 %5332, 13
  %5341 = xor i32 %5340, %5332
  %5342 = lshr i32 %5341, 17
  %5343 = xor i32 %5342, %5341
  %5344 = shl i32 %5343, 5
  %5345 = xor i32 %5344, %5343
  store i32 %5345, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5346 = trunc i32 %5345 to i8
  %5347 = xor i8 %5339, %5346
  %5348 = zext i8 %5347 to i64
  %5349 = shl nuw nsw i64 %5348, 24
  %5350 = or disjoint i64 %5336, %5322
  %5351 = or disjoint i64 %5350, %5349
  %5352 = trunc nuw i64 %5351 to i32
  %5353 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %5354 = icmp eq i8 %5353, 0
  %5355 = load i8, ptr @exception_thrown__Z8vm_entryv, align 4
  br i1 %5354, label %5389, label %5381

5356:                                             ; preds = %5356, %5283
  %5357 = phi i64 [ %5284, %5283 ], [ %5361, %5356 ]
  %5358 = phi i64 [ 0, %5283 ], [ %5379, %5356 ]
  %5359 = phi i64 [ 0, %5283 ], [ %5378, %5356 ]
  %5360 = phi i32 [ %5278, %5283 ], [ %5370, %5356 ]
  %5361 = add nsw i64 %5357, 1
  %5362 = trunc nsw i64 %5361 to i32
  store i32 %5362, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5363 = getelementptr inbounds i8, ptr %5282, i64 %5357
  %5364 = load i8, ptr %5363, align 1, !tbaa !12
  %5365 = shl i32 %5360, 13
  %5366 = xor i32 %5365, %5360
  %5367 = lshr i32 %5366, 17
  %5368 = xor i32 %5367, %5366
  %5369 = shl i32 %5368, 5
  %5370 = xor i32 %5369, %5368
  store i32 %5370, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5371 = trunc i32 %5370 to i8
  %5372 = xor i8 %5364, %5371
  %5373 = icmp samesign ult i64 %5358, 8
  %5374 = zext i8 %5372 to i64
  %5375 = shl nuw nsw i64 %5358, 3
  %5376 = shl nuw i64 %5374, %5375
  %5377 = select i1 %5373, i64 %5376, i64 0
  %5378 = or i64 %5377, %5359
  %5379 = add nuw nsw i64 %5358, 1
  %5380 = icmp eq i64 %5379, %5285
  br i1 %5380, label %5286, label %5356, !llvm.loop !33

5381:                                             ; preds = %5290
  %5382 = zext i8 %5355 to i32
  %5383 = load ptr, ptr @exception_ptr__Z8vm_entryv, align 8, !tbaa !54
  %5384 = load i32, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %5385 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.11__Z8vm_entryv.40, i64 noundef %5293, i32 noundef %5352, i32 noundef %5382, ptr noundef %5383, i32 noundef %5384, ptr noundef null, i32 noundef 0, i32 noundef %5337)
  %5386 = tail call i32 @fflush(ptr noundef null)
  %5387 = load i8, ptr @exception_thrown__Z8vm_entryv, align 4
  %5388 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  br label %5389

5389:                                             ; preds = %5381, %5290
  %5390 = phi i32 [ %5388, %5381 ], [ %5279, %5290 ]
  %5391 = phi i8 [ %5387, %5381 ], [ %5355, %5290 ]
  %5392 = icmp eq i8 %5391, 0
  %5393 = load ptr, ptr @exception_ptr__Z8vm_entryv, align 8
  %5394 = load i32, ptr @exception_selector__Z8vm_entryv, align 4
  %5395 = select i1 %5392, i32 0, i32 %5394
  %5396 = select i1 %5392, ptr null, ptr %5393
  %5397 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5398 = add i64 %5397, %5293
  %5399 = icmp sgt i32 %5390, 0
  br i1 %5399, label %5400, label %5416

5400:                                             ; preds = %5389
  %5401 = inttoptr i64 %5398 to ptr
  %5402 = ptrtoint ptr %5396 to i64
  br label %5403

5403:                                             ; preds = %5403, %5400
  %5404 = phi i32 [ %5410, %5403 ], [ 0, %5400 ]
  %5405 = phi ptr [ %5408, %5403 ], [ %5401, %5400 ]
  %5406 = phi i64 [ %5409, %5403 ], [ %5402, %5400 ]
  %5407 = trunc i64 %5406 to i8
  store i8 %5407, ptr %5405, align 1, !tbaa !12
  %5408 = getelementptr inbounds nuw i8, ptr %5405, i64 1
  %5409 = lshr i64 %5406, 8
  %5410 = add nuw nsw i32 %5404, 1
  %5411 = icmp eq i32 %5410, %5390
  br i1 %5411, label %5412, label %5403, !llvm.loop !34

5412:                                             ; preds = %5403
  %5413 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5414 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %5415 = add i64 %5413, %5293
  br label %5416

5416:                                             ; preds = %5412, %5389
  %5417 = phi i64 [ %5415, %5412 ], [ %5398, %5389 ]
  %5418 = phi i32 [ %5414, %5412 ], [ %5390, %5389 ]
  %5419 = zext i32 %5418 to i64
  %5420 = add i64 %5417, %5419
  %5421 = inttoptr i64 %5420 to ptr
  %5422 = trunc i32 %5395 to i8
  store i8 %5422, ptr %5421, align 1, !tbaa !12
  %5423 = getelementptr inbounds nuw i8, ptr %5421, i64 1
  %5424 = lshr i32 %5395, 8
  %5425 = trunc i32 %5424 to i8
  store i8 %5425, ptr %5423, align 1, !tbaa !12
  %5426 = getelementptr inbounds nuw i8, ptr %5421, i64 2
  %5427 = lshr i32 %5395, 16
  %5428 = trunc i32 %5427 to i8
  store i8 %5428, ptr %5426, align 1, !tbaa !12
  %5429 = getelementptr inbounds nuw i8, ptr %5421, i64 3
  %5430 = lshr i32 %5395, 24
  %5431 = trunc nuw i32 %5430 to i8
  store i8 %5431, ptr %5429, align 1, !tbaa !12
  %5432 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %5433 = zext i32 %5432 to i64
  %5434 = add i64 %5293, %5433
  %5435 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5436 = add i64 %5435, 4
  %5437 = add i64 %5436, %5434
  %5438 = inttoptr i64 %5437 to ptr
  store i8 %5306, ptr %5438, align 1, !tbaa !12
  %5439 = getelementptr inbounds nuw i8, ptr %5438, i64 1
  store i8 %5319, ptr %5439, align 1, !tbaa !12
  %5440 = getelementptr inbounds nuw i8, ptr %5438, i64 2
  store i8 %5334, ptr %5440, align 1, !tbaa !12
  %5441 = getelementptr inbounds nuw i8, ptr %5438, i64 3
  store i8 %5347, ptr %5441, align 1, !tbaa !12
  %5442 = icmp eq i64 %5351, 0
  br i1 %5442, label %5446, label %5443

5443:                                             ; preds = %5416
  %5444 = add i64 %5434, 8
  br label %5449

5445:                                             ; preds = %5658
  br label %5446

5446:                                             ; preds = %5445, %5416
  store i8 0, ptr @exception_thrown__Z8vm_entryv, align 4, !tbaa !12
  %5447 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %5448 = icmp eq i8 %5447, 0
  br i1 %5448, label %2370, label %5661

5449:                                             ; preds = %5658, %5443
  %5450 = phi i32 [ 0, %5443 ], [ %5659, %5658 ]
  %5451 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5452 = inttoptr i64 %5451 to ptr
  %5453 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5454 = add nsw i32 %5453, 1
  store i32 %5454, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5455 = sext i32 %5453 to i64
  %5456 = getelementptr inbounds i8, ptr %5452, i64 %5455
  %5457 = load i8, ptr %5456, align 1, !tbaa !12
  %5458 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5459 = shl i32 %5458, 13
  %5460 = xor i32 %5459, %5458
  %5461 = lshr i32 %5460, 17
  %5462 = xor i32 %5461, %5460
  %5463 = shl i32 %5462, 5
  %5464 = xor i32 %5463, %5462
  store i32 %5464, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5465 = trunc i32 %5464 to i8
  %5466 = xor i8 %5457, %5465
  %5467 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %5468 = add i32 %5467, 1
  %5469 = mul i32 %5468, %5450
  %5470 = zext i32 %5469 to i64
  %5471 = add i64 %5444, %5470
  %5472 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5473 = add i64 %5471, %5472
  %5474 = inttoptr i64 %5473 to ptr
  store i8 %5466, ptr %5474, align 1, !tbaa !12
  %5475 = icmp eq i8 %5457, %5465
  br i1 %5475, label %5476, label %5547

5476:                                             ; preds = %5449
  %5477 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5478 = inttoptr i64 %5477 to ptr
  %5479 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5480 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5481 = sext i32 %5479 to i64
  %5482 = add nsw i64 %5481, 1
  %5483 = trunc nsw i64 %5482 to i32
  store i32 %5483, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5484 = getelementptr inbounds i8, ptr %5478, i64 %5481
  %5485 = load i8, ptr %5484, align 1, !tbaa !12
  %5486 = shl i32 %5480, 13
  %5487 = xor i32 %5486, %5480
  %5488 = lshr i32 %5487, 17
  %5489 = xor i32 %5488, %5487
  %5490 = shl i32 %5489, 5
  %5491 = xor i32 %5490, %5489
  store i32 %5491, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5492 = trunc i32 %5491 to i8
  %5493 = xor i8 %5485, %5492
  %5494 = zext i8 %5493 to i32
  %5495 = add nsw i64 %5481, 2
  %5496 = trunc nsw i64 %5495 to i32
  store i32 %5496, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5497 = getelementptr inbounds i8, ptr %5478, i64 %5482
  %5498 = load i8, ptr %5497, align 1, !tbaa !12
  %5499 = shl i32 %5491, 13
  %5500 = xor i32 %5499, %5491
  %5501 = lshr i32 %5500, 17
  %5502 = xor i32 %5501, %5500
  %5503 = shl i32 %5502, 5
  %5504 = xor i32 %5503, %5502
  store i32 %5504, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5505 = trunc i32 %5504 to i8
  %5506 = xor i8 %5498, %5505
  %5507 = zext i8 %5506 to i32
  %5508 = shl nuw nsw i32 %5507, 8
  %5509 = or disjoint i32 %5508, %5494
  %5510 = add nsw i64 %5481, 3
  %5511 = trunc nsw i64 %5510 to i32
  store i32 %5511, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5512 = getelementptr inbounds i8, ptr %5478, i64 %5495
  %5513 = load i8, ptr %5512, align 1, !tbaa !12
  %5514 = shl i32 %5504, 13
  %5515 = xor i32 %5514, %5504
  %5516 = lshr i32 %5515, 17
  %5517 = xor i32 %5516, %5515
  %5518 = shl i32 %5517, 5
  %5519 = xor i32 %5518, %5517
  store i32 %5519, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5520 = trunc i32 %5519 to i8
  %5521 = xor i8 %5513, %5520
  %5522 = zext i8 %5521 to i32
  %5523 = shl nuw nsw i32 %5522, 16
  %5524 = add i32 %5479, 4
  store i32 %5524, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5525 = getelementptr inbounds i8, ptr %5478, i64 %5510
  %5526 = load i8, ptr %5525, align 1, !tbaa !12
  %5527 = shl i32 %5519, 13
  %5528 = xor i32 %5527, %5519
  %5529 = lshr i32 %5528, 17
  %5530 = xor i32 %5529, %5528
  %5531 = shl i32 %5530, 5
  %5532 = xor i32 %5531, %5530
  store i32 %5532, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5533 = trunc i32 %5532 to i8
  %5534 = xor i8 %5526, %5533
  %5535 = zext i8 %5534 to i32
  %5536 = shl nuw i32 %5535, 24
  %5537 = or disjoint i32 %5523, %5509
  %5538 = or disjoint i32 %5537, %5536
  %5539 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5540 = add i64 %5471, 1
  %5541 = add i64 %5540, %5539
  %5542 = inttoptr i64 %5541 to ptr
  store i8 %5493, ptr %5542, align 1, !tbaa !12
  %5543 = getelementptr inbounds nuw i8, ptr %5542, i64 1
  store i8 %5506, ptr %5543, align 1, !tbaa !12
  %5544 = getelementptr inbounds nuw i8, ptr %5542, i64 2
  store i8 %5521, ptr %5544, align 1, !tbaa !12
  %5545 = getelementptr inbounds nuw i8, ptr %5542, i64 3
  store i8 %5534, ptr %5545, align 1, !tbaa !12
  %5546 = icmp eq i32 %5538, 0
  br i1 %5546, label %5658, label %5597

5547:                                             ; preds = %5449
  %5548 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %5549 = icmp sgt i32 %5548, 0
  br i1 %5549, label %5550, label %5658

5550:                                             ; preds = %5547
  %5551 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4
  %5552 = load i32, ptr @ip__Z8vm_entryv, align 4
  %5553 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5554 = inttoptr i64 %5553 to ptr
  %5555 = sext i32 %5552 to i64
  %5556 = zext nneg i32 %5548 to i64
  br label %5557

5557:                                             ; preds = %5557, %5550
  %5558 = phi i64 [ %5555, %5550 ], [ %5562, %5557 ]
  %5559 = phi i64 [ 0, %5550 ], [ %5580, %5557 ]
  %5560 = phi i64 [ 0, %5550 ], [ %5579, %5557 ]
  %5561 = phi i32 [ %5551, %5550 ], [ %5571, %5557 ]
  %5562 = add nsw i64 %5558, 1
  %5563 = trunc nsw i64 %5562 to i32
  store i32 %5563, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5564 = getelementptr inbounds i8, ptr %5554, i64 %5558
  %5565 = load i8, ptr %5564, align 1, !tbaa !12
  %5566 = shl i32 %5561, 13
  %5567 = xor i32 %5566, %5561
  %5568 = lshr i32 %5567, 17
  %5569 = xor i32 %5568, %5567
  %5570 = shl i32 %5569, 5
  %5571 = xor i32 %5570, %5569
  store i32 %5571, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5572 = trunc i32 %5571 to i8
  %5573 = xor i8 %5565, %5572
  %5574 = icmp samesign ult i64 %5559, 8
  %5575 = zext i8 %5573 to i64
  %5576 = shl nuw nsw i64 %5559, 3
  %5577 = shl nuw i64 %5575, %5576
  %5578 = select i1 %5574, i64 %5577, i64 0
  %5579 = or i64 %5578, %5560
  %5580 = add nuw nsw i64 %5559, 1
  %5581 = icmp eq i64 %5580, %5556
  br i1 %5581, label %5582, label %5557, !llvm.loop !33

5582:                                             ; preds = %5557
  %5583 = phi i64 [ %5579, %5557 ]
  %5584 = add i64 %5471, 1
  %5585 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5586 = add i64 %5584, %5585
  %5587 = inttoptr i64 %5586 to ptr
  br label %5588

5588:                                             ; preds = %5588, %5582
  %5589 = phi i32 [ %5595, %5588 ], [ 0, %5582 ]
  %5590 = phi ptr [ %5593, %5588 ], [ %5587, %5582 ]
  %5591 = phi i64 [ %5594, %5588 ], [ %5583, %5582 ]
  %5592 = trunc i64 %5591 to i8
  store i8 %5592, ptr %5590, align 1, !tbaa !12
  %5593 = getelementptr inbounds nuw i8, ptr %5590, i64 1
  %5594 = lshr i64 %5591, 8
  %5595 = add nuw nsw i32 %5589, 1
  %5596 = icmp eq i32 %5595, %5548
  br i1 %5596, label %5657, label %5588, !llvm.loop !34

5597:                                             ; preds = %5476
  %5598 = add i64 %5471, 5
  br label %5599

5599:                                             ; preds = %5653, %5597
  %5600 = phi i32 [ 0, %5597 ], [ %5654, %5653 ]
  %5601 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %5602 = icmp sgt i32 %5601, 0
  br i1 %5602, label %5603, label %5653

5603:                                             ; preds = %5599
  %5604 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4
  %5605 = load i32, ptr @ip__Z8vm_entryv, align 4
  %5606 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5607 = inttoptr i64 %5606 to ptr
  %5608 = sext i32 %5605 to i64
  %5609 = zext nneg i32 %5601 to i64
  br label %5610

5610:                                             ; preds = %5610, %5603
  %5611 = phi i64 [ %5608, %5603 ], [ %5615, %5610 ]
  %5612 = phi i64 [ 0, %5603 ], [ %5633, %5610 ]
  %5613 = phi i64 [ 0, %5603 ], [ %5632, %5610 ]
  %5614 = phi i32 [ %5604, %5603 ], [ %5624, %5610 ]
  %5615 = add nsw i64 %5611, 1
  %5616 = trunc nsw i64 %5615 to i32
  store i32 %5616, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5617 = getelementptr inbounds i8, ptr %5607, i64 %5611
  %5618 = load i8, ptr %5617, align 1, !tbaa !12
  %5619 = shl i32 %5614, 13
  %5620 = xor i32 %5619, %5614
  %5621 = lshr i32 %5620, 17
  %5622 = xor i32 %5621, %5620
  %5623 = shl i32 %5622, 5
  %5624 = xor i32 %5623, %5622
  store i32 %5624, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5625 = trunc i32 %5624 to i8
  %5626 = xor i8 %5618, %5625
  %5627 = icmp samesign ult i64 %5612, 8
  %5628 = zext i8 %5626 to i64
  %5629 = shl nuw nsw i64 %5612, 3
  %5630 = shl nuw i64 %5628, %5629
  %5631 = select i1 %5627, i64 %5630, i64 0
  %5632 = or i64 %5631, %5613
  %5633 = add nuw nsw i64 %5612, 1
  %5634 = icmp eq i64 %5633, %5609
  br i1 %5634, label %5635, label %5610, !llvm.loop !33

5635:                                             ; preds = %5610
  %5636 = phi i64 [ %5632, %5610 ]
  %5637 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5638 = add i64 %5598, %5637
  %5639 = mul i32 %5601, %5600
  %5640 = zext i32 %5639 to i64
  %5641 = add i64 %5638, %5640
  %5642 = inttoptr i64 %5641 to ptr
  br label %5643

5643:                                             ; preds = %5643, %5635
  %5644 = phi i32 [ %5650, %5643 ], [ 0, %5635 ]
  %5645 = phi ptr [ %5648, %5643 ], [ %5642, %5635 ]
  %5646 = phi i64 [ %5649, %5643 ], [ %5636, %5635 ]
  %5647 = trunc i64 %5646 to i8
  store i8 %5647, ptr %5645, align 1, !tbaa !12
  %5648 = getelementptr inbounds nuw i8, ptr %5645, i64 1
  %5649 = lshr i64 %5646, 8
  %5650 = add nuw nsw i32 %5644, 1
  %5651 = icmp eq i32 %5650, %5601
  br i1 %5651, label %5652, label %5643, !llvm.loop !34

5652:                                             ; preds = %5643
  br label %5653

5653:                                             ; preds = %5652, %5599
  %5654 = add nuw i32 %5600, 1
  %5655 = icmp eq i32 %5654, %5538
  br i1 %5655, label %5656, label %5599, !llvm.loop !56

5656:                                             ; preds = %5653
  br label %5658

5657:                                             ; preds = %5588
  br label %5658

5658:                                             ; preds = %5657, %5656, %5547, %5476
  %5659 = add nuw i32 %5450, 1
  %5660 = icmp eq i32 %5659, %5352
  br i1 %5660, label %5445, label %5449, !llvm.loop !57

5661:                                             ; preds = %5446
  %5662 = load ptr, ptr @exception_ptr__Z8vm_entryv, align 8, !tbaa !54
  %5663 = load i32, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %5664 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5665 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.12__Z8vm_entryv.41, ptr noundef %5396, i32 noundef %5395, i32 noundef 0, ptr noundef %5662, i32 noundef %5663, ptr noundef null, i32 noundef 0, i32 noundef %5664)
  %5666 = tail call i32 @fflush(ptr noundef null)
  br label %2370

5667:                                             ; preds = %239
  %5668 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5669 = inttoptr i64 %5668 to ptr
  %5670 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5671 = add nsw i32 %5670, 1
  store i32 %5671, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5672 = sext i32 %5670 to i64
  %5673 = getelementptr inbounds i8, ptr %5669, i64 %5672
  %5674 = load i8, ptr %5673, align 1, !tbaa !12
  %5675 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5676 = shl i32 %5675, 13
  %5677 = xor i32 %5676, %5675
  %5678 = lshr i32 %5677, 17
  %5679 = xor i32 %5678, %5677
  %5680 = shl i32 %5679, 5
  %5681 = xor i32 %5680, %5679
  store i32 %5681, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5682 = trunc i32 %5681 to i8
  %5683 = xor i8 %5674, %5682
  %5684 = add nsw i32 %5670, 2
  store i32 %5684, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5685 = sext i32 %5671 to i64
  %5686 = getelementptr inbounds i8, ptr %5669, i64 %5685
  %5687 = load i8, ptr %5686, align 1, !tbaa !12
  %5688 = shl i32 %5681, 13
  %5689 = xor i32 %5688, %5681
  %5690 = lshr i32 %5689, 17
  %5691 = xor i32 %5690, %5689
  %5692 = shl i32 %5691, 5
  %5693 = xor i32 %5692, %5691
  store i32 %5693, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5694 = trunc i32 %5693 to i8
  %5695 = load i32, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %5696 = icmp eq i8 %5687, %5694
  br i1 %5696, label %5697, label %5789

5697:                                             ; preds = %5667
  %5698 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %5699 = icmp sgt i32 %5698, 0
  br i1 %5699, label %5700, label %5731

5700:                                             ; preds = %5697
  %5701 = sext i32 %5684 to i64
  %5702 = zext nneg i32 %5698 to i64
  br label %5703

5703:                                             ; preds = %5703, %5700
  %5704 = phi i64 [ %5701, %5700 ], [ %5708, %5703 ]
  %5705 = phi i64 [ 0, %5700 ], [ %5726, %5703 ]
  %5706 = phi i64 [ 0, %5700 ], [ %5725, %5703 ]
  %5707 = phi i32 [ %5693, %5700 ], [ %5717, %5703 ]
  %5708 = add nsw i64 %5704, 1
  %5709 = trunc nsw i64 %5708 to i32
  store i32 %5709, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5710 = getelementptr inbounds i8, ptr %5669, i64 %5704
  %5711 = load i8, ptr %5710, align 1, !tbaa !12
  %5712 = shl i32 %5707, 13
  %5713 = xor i32 %5712, %5707
  %5714 = lshr i32 %5713, 17
  %5715 = xor i32 %5714, %5713
  %5716 = shl i32 %5715, 5
  %5717 = xor i32 %5716, %5715
  store i32 %5717, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5718 = trunc i32 %5717 to i8
  %5719 = xor i8 %5711, %5718
  %5720 = icmp samesign ult i64 %5705, 8
  %5721 = zext i8 %5719 to i64
  %5722 = shl nuw nsw i64 %5705, 3
  %5723 = shl nuw i64 %5721, %5722
  %5724 = select i1 %5720, i64 %5723, i64 0
  %5725 = or i64 %5724, %5706
  %5726 = add nuw nsw i64 %5705, 1
  %5727 = icmp eq i64 %5726, %5702
  br i1 %5727, label %5728, label %5703, !llvm.loop !33

5728:                                             ; preds = %5703
  %5729 = phi i32 [ %5709, %5703 ]
  %5730 = phi i64 [ %5725, %5703 ]
  br label %5731

5731:                                             ; preds = %5728, %5697
  %5732 = phi i32 [ %5684, %5697 ], [ %5729, %5728 ]
  %5733 = phi i64 [ 0, %5697 ], [ %5730, %5728 ]
  %5734 = zext i8 %5683 to i32
  %5735 = icmp eq i8 %5674, %5682
  br i1 %5735, label %5759, label %5736

5736:                                             ; preds = %5731
  %5737 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %5738 = inttoptr i64 %5737 to ptr
  %5739 = zext i8 %5683 to i64
  br label %5740

5740:                                             ; preds = %5753, %5736
  %5741 = phi i64 [ 0, %5736 ], [ %5755, %5753 ]
  %5742 = phi i64 [ %5733, %5736 ], [ %5744, %5753 ]
  %5743 = phi i64 [ 0, %5736 ], [ %5754, %5753 ]
  %5744 = add i64 %5742, 1
  %5745 = icmp samesign ult i64 %5741, 8
  br i1 %5745, label %5746, label %5753

5746:                                             ; preds = %5740
  %5747 = getelementptr inbounds nuw i8, ptr %5738, i64 %5742
  %5748 = load i8, ptr %5747, align 1, !tbaa !12
  %5749 = zext i8 %5748 to i64
  %5750 = shl nuw nsw i64 %5741, 3
  %5751 = shl nuw i64 %5749, %5750
  %5752 = or i64 %5751, %5743
  br label %5753

5753:                                             ; preds = %5746, %5740
  %5754 = phi i64 [ %5752, %5746 ], [ %5743, %5740 ]
  %5755 = add nuw nsw i64 %5741, 1
  %5756 = icmp eq i64 %5755, %5739
  br i1 %5756, label %5757, label %5740, !llvm.loop !35

5757:                                             ; preds = %5753
  %5758 = phi i64 [ %5754, %5753 ]
  br label %5759

5759:                                             ; preds = %5757, %5731
  %5760 = phi i64 [ 0, %5731 ], [ %5758, %5757 ]
  %5761 = add i32 %5698, 4
  %5762 = icmp ugt i32 %5761, %5734
  br i1 %5762, label %5890, label %5763

5763:                                             ; preds = %5759
  %5764 = zext i32 %5698 to i64
  %5765 = add i64 %5733, %5764
  %5766 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5767 = inttoptr i64 %5766 to ptr
  %5768 = getelementptr inbounds nuw i8, ptr %5767, i64 %5765
  %5769 = load i8, ptr %5768, align 1, !tbaa !12
  %5770 = zext i8 %5769 to i32
  %5771 = getelementptr i8, ptr %5767, i64 %5765
  %5772 = getelementptr i8, ptr %5771, i64 1
  %5773 = load i8, ptr %5772, align 1, !tbaa !12
  %5774 = zext i8 %5773 to i32
  %5775 = shl nuw nsw i32 %5774, 8
  %5776 = or disjoint i32 %5775, %5770
  %5777 = getelementptr i8, ptr %5767, i64 %5765
  %5778 = getelementptr i8, ptr %5777, i64 2
  %5779 = load i8, ptr %5778, align 1, !tbaa !12
  %5780 = zext i8 %5779 to i32
  %5781 = shl nuw nsw i32 %5780, 16
  %5782 = or disjoint i32 %5781, %5776
  %5783 = getelementptr i8, ptr %5767, i64 %5765
  %5784 = getelementptr i8, ptr %5783, i64 3
  %5785 = load i8, ptr %5784, align 1, !tbaa !12
  %5786 = zext i8 %5785 to i32
  %5787 = shl nuw i32 %5786, 24
  %5788 = or disjoint i32 %5787, %5782
  br label %5890

5789:                                             ; preds = %5667
  %5790 = zext i8 %5683 to i32
  %5791 = icmp eq i8 %5674, %5682
  br i1 %5791, label %5824, label %5792

5792:                                             ; preds = %5789
  %5793 = sext i32 %5684 to i64
  %5794 = zext i8 %5683 to i64
  br label %5795

5795:                                             ; preds = %5795, %5792
  %5796 = phi i64 [ %5793, %5792 ], [ %5800, %5795 ]
  %5797 = phi i64 [ 0, %5792 ], [ %5818, %5795 ]
  %5798 = phi i64 [ 0, %5792 ], [ %5817, %5795 ]
  %5799 = phi i32 [ %5693, %5792 ], [ %5809, %5795 ]
  %5800 = add nsw i64 %5796, 1
  %5801 = trunc nsw i64 %5800 to i32
  store i32 %5801, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5802 = getelementptr inbounds i8, ptr %5669, i64 %5796
  %5803 = load i8, ptr %5802, align 1, !tbaa !12
  %5804 = shl i32 %5799, 13
  %5805 = xor i32 %5804, %5799
  %5806 = lshr i32 %5805, 17
  %5807 = xor i32 %5806, %5805
  %5808 = shl i32 %5807, 5
  %5809 = xor i32 %5808, %5807
  store i32 %5809, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5810 = trunc i32 %5809 to i8
  %5811 = xor i8 %5803, %5810
  %5812 = icmp samesign ult i64 %5797, 8
  %5813 = zext i8 %5811 to i64
  %5814 = shl nuw nsw i64 %5797, 3
  %5815 = shl nuw i64 %5813, %5814
  %5816 = select i1 %5812, i64 %5815, i64 0
  %5817 = or i64 %5816, %5798
  %5818 = add nuw nsw i64 %5797, 1
  %5819 = icmp eq i64 %5818, %5794
  br i1 %5819, label %5820, label %5795, !llvm.loop !33

5820:                                             ; preds = %5795
  %5821 = phi i32 [ %5801, %5795 ]
  %5822 = phi i32 [ %5809, %5795 ]
  %5823 = phi i64 [ %5817, %5795 ]
  br label %5824

5824:                                             ; preds = %5820, %5789
  %5825 = phi i32 [ %5693, %5789 ], [ %5822, %5820 ]
  %5826 = phi i32 [ %5684, %5789 ], [ %5821, %5820 ]
  %5827 = phi i64 [ 0, %5789 ], [ %5823, %5820 ]
  %5828 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %5829 = add i32 %5828, 4
  %5830 = icmp ugt i32 %5829, %5790
  br i1 %5830, label %5890, label %5831

5831:                                             ; preds = %5824
  %5832 = sext i32 %5826 to i64
  %5833 = add nsw i64 %5832, 1
  %5834 = trunc nsw i64 %5833 to i32
  store i32 %5834, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5835 = getelementptr inbounds i8, ptr %5669, i64 %5832
  %5836 = load i8, ptr %5835, align 1, !tbaa !12
  %5837 = shl i32 %5825, 13
  %5838 = xor i32 %5837, %5825
  %5839 = lshr i32 %5838, 17
  %5840 = xor i32 %5839, %5838
  %5841 = shl i32 %5840, 5
  %5842 = xor i32 %5841, %5840
  store i32 %5842, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5843 = trunc i32 %5842 to i8
  %5844 = xor i8 %5836, %5843
  %5845 = zext i8 %5844 to i32
  %5846 = add nsw i64 %5832, 2
  %5847 = trunc nsw i64 %5846 to i32
  store i32 %5847, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5848 = getelementptr inbounds i8, ptr %5669, i64 %5833
  %5849 = load i8, ptr %5848, align 1, !tbaa !12
  %5850 = shl i32 %5842, 13
  %5851 = xor i32 %5850, %5842
  %5852 = lshr i32 %5851, 17
  %5853 = xor i32 %5852, %5851
  %5854 = shl i32 %5853, 5
  %5855 = xor i32 %5854, %5853
  store i32 %5855, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5856 = trunc i32 %5855 to i8
  %5857 = xor i8 %5849, %5856
  %5858 = zext i8 %5857 to i32
  %5859 = shl nuw nsw i32 %5858, 8
  %5860 = or disjoint i32 %5859, %5845
  %5861 = add nsw i64 %5832, 3
  %5862 = trunc nsw i64 %5861 to i32
  store i32 %5862, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5863 = getelementptr inbounds i8, ptr %5669, i64 %5846
  %5864 = load i8, ptr %5863, align 1, !tbaa !12
  %5865 = shl i32 %5855, 13
  %5866 = xor i32 %5865, %5855
  %5867 = lshr i32 %5866, 17
  %5868 = xor i32 %5867, %5866
  %5869 = shl i32 %5868, 5
  %5870 = xor i32 %5869, %5868
  store i32 %5870, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5871 = trunc i32 %5870 to i8
  %5872 = xor i8 %5864, %5871
  %5873 = zext i8 %5872 to i32
  %5874 = shl nuw nsw i32 %5873, 16
  %5875 = or disjoint i32 %5874, %5860
  %5876 = add i32 %5826, 4
  store i32 %5876, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5877 = getelementptr inbounds i8, ptr %5669, i64 %5861
  %5878 = load i8, ptr %5877, align 1, !tbaa !12
  %5879 = shl i32 %5870, 13
  %5880 = xor i32 %5879, %5870
  %5881 = lshr i32 %5880, 17
  %5882 = xor i32 %5881, %5880
  %5883 = shl i32 %5882, 5
  %5884 = xor i32 %5883, %5882
  store i32 %5884, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5885 = trunc i32 %5884 to i8
  %5886 = xor i8 %5878, %5885
  %5887 = zext i8 %5886 to i32
  %5888 = shl nuw i32 %5887, 24
  %5889 = or disjoint i32 %5888, %5875
  br label %5890

5890:                                             ; preds = %5831, %5824, %5763, %5759
  %5891 = phi i32 [ %5876, %5831 ], [ %5826, %5824 ], [ %5732, %5763 ], [ %5732, %5759 ]
  %5892 = phi i32 [ %5889, %5831 ], [ %5695, %5824 ], [ %5788, %5763 ], [ %5695, %5759 ]
  %5893 = phi i64 [ %5827, %5831 ], [ %5827, %5824 ], [ %5760, %5763 ], [ %5760, %5759 ]
  store i8 1, ptr @exception_thrown__Z8vm_entryv, align 4, !tbaa !12
  %5894 = inttoptr i64 %5893 to ptr
  store ptr %5894, ptr @exception_ptr__Z8vm_entryv, align 8, !tbaa !54
  store i32 %5892, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %5895 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %5896 = icmp eq i8 %5895, 0
  br i1 %5896, label %10047, label %5897

5897:                                             ; preds = %5890
  %5898 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.13__Z8vm_entryv.42, i64 noundef %5893, i32 noundef 1, ptr noundef %5894, i32 noundef %5892, ptr noundef null, i32 noundef 0, i32 noundef %5891)
  br label %10044

5899:                                             ; preds = %239
  %5900 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %5901 = inttoptr i64 %5900 to ptr
  %5902 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5903 = add nsw i32 %5902, 1
  store i32 %5903, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5904 = sext i32 %5902 to i64
  %5905 = getelementptr inbounds i8, ptr %5901, i64 %5904
  %5906 = load i8, ptr %5905, align 1, !tbaa !12
  %5907 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5908 = shl i32 %5907, 13
  %5909 = xor i32 %5908, %5907
  %5910 = lshr i32 %5909, 17
  %5911 = xor i32 %5910, %5909
  %5912 = shl i32 %5911, 5
  %5913 = xor i32 %5912, %5911
  store i32 %5913, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5914 = trunc i32 %5913 to i8
  %5915 = xor i8 %5906, %5914
  %5916 = add nsw i32 %5902, 2
  store i32 %5916, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5917 = sext i32 %5903 to i64
  %5918 = getelementptr inbounds i8, ptr %5901, i64 %5917
  %5919 = load i8, ptr %5918, align 1, !tbaa !12
  %5920 = shl i32 %5913, 13
  %5921 = xor i32 %5920, %5913
  %5922 = lshr i32 %5921, 17
  %5923 = xor i32 %5922, %5921
  %5924 = shl i32 %5923, 5
  %5925 = xor i32 %5924, %5923
  store i32 %5925, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5926 = trunc i32 %5925 to i8
  %5927 = icmp eq i8 %5919, %5926
  br i1 %5927, label %5928, label %5989

5928:                                             ; preds = %5899
  %5929 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %5930 = icmp sgt i32 %5929, 0
  br i1 %5930, label %5931, label %5963

5931:                                             ; preds = %5928
  %5932 = sext i32 %5916 to i64
  %5933 = zext nneg i32 %5929 to i64
  br label %5934

5934:                                             ; preds = %5934, %5931
  %5935 = phi i64 [ %5932, %5931 ], [ %5939, %5934 ]
  %5936 = phi i64 [ 0, %5931 ], [ %5957, %5934 ]
  %5937 = phi i64 [ 0, %5931 ], [ %5956, %5934 ]
  %5938 = phi i32 [ %5925, %5931 ], [ %5948, %5934 ]
  %5939 = add nsw i64 %5935, 1
  %5940 = trunc nsw i64 %5939 to i32
  store i32 %5940, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %5941 = getelementptr inbounds i8, ptr %5901, i64 %5935
  %5942 = load i8, ptr %5941, align 1, !tbaa !12
  %5943 = shl i32 %5938, 13
  %5944 = xor i32 %5943, %5938
  %5945 = lshr i32 %5944, 17
  %5946 = xor i32 %5945, %5944
  %5947 = shl i32 %5946, 5
  %5948 = xor i32 %5947, %5946
  store i32 %5948, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %5949 = trunc i32 %5948 to i8
  %5950 = xor i8 %5942, %5949
  %5951 = icmp samesign ult i64 %5936, 8
  %5952 = zext i8 %5950 to i64
  %5953 = shl nuw nsw i64 %5936, 3
  %5954 = shl nuw i64 %5952, %5953
  %5955 = select i1 %5951, i64 %5954, i64 0
  %5956 = or i64 %5955, %5937
  %5957 = add nuw nsw i64 %5936, 1
  %5958 = icmp eq i64 %5957, %5933
  br i1 %5958, label %5959, label %5934, !llvm.loop !33

5959:                                             ; preds = %5934
  %5960 = phi i32 [ %5940, %5934 ]
  %5961 = phi i32 [ %5948, %5934 ]
  %5962 = phi i64 [ %5956, %5934 ]
  br label %5963

5963:                                             ; preds = %5959, %5928
  %5964 = phi i32 [ %5925, %5928 ], [ %5961, %5959 ]
  %5965 = phi i32 [ %5916, %5928 ], [ %5960, %5959 ]
  %5966 = phi i64 [ 0, %5928 ], [ %5962, %5959 ]
  %5967 = icmp eq i8 %5906, %5914
  br i1 %5967, label %6025, label %5968

5968:                                             ; preds = %5963
  %5969 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %5970 = inttoptr i64 %5969 to ptr
  %5971 = zext i8 %5915 to i64
  br label %5972

5972:                                             ; preds = %5985, %5968
  %5973 = phi i64 [ 0, %5968 ], [ %5987, %5985 ]
  %5974 = phi i64 [ %5966, %5968 ], [ %5976, %5985 ]
  %5975 = phi i64 [ 0, %5968 ], [ %5986, %5985 ]
  %5976 = add i64 %5974, 1
  %5977 = icmp samesign ult i64 %5973, 8
  br i1 %5977, label %5978, label %5985

5978:                                             ; preds = %5972
  %5979 = getelementptr inbounds nuw i8, ptr %5970, i64 %5974
  %5980 = load i8, ptr %5979, align 1, !tbaa !12
  %5981 = zext i8 %5980 to i64
  %5982 = shl nuw nsw i64 %5973, 3
  %5983 = shl nuw i64 %5981, %5982
  %5984 = or i64 %5983, %5975
  br label %5985

5985:                                             ; preds = %5978, %5972
  %5986 = phi i64 [ %5984, %5978 ], [ %5975, %5972 ]
  %5987 = add nuw nsw i64 %5973, 1
  %5988 = icmp eq i64 %5987, %5971
  br i1 %5988, label %6019, label %5972, !llvm.loop !35

5989:                                             ; preds = %5899
  %5990 = icmp eq i8 %5906, %5914
  br i1 %5990, label %6025, label %5991

5991:                                             ; preds = %5989
  %5992 = sext i32 %5916 to i64
  %5993 = zext i8 %5915 to i64
  br label %5994

5994:                                             ; preds = %5994, %5991
  %5995 = phi i64 [ %5992, %5991 ], [ %5999, %5994 ]
  %5996 = phi i64 [ 0, %5991 ], [ %6017, %5994 ]
  %5997 = phi i64 [ 0, %5991 ], [ %6016, %5994 ]
  %5998 = phi i32 [ %5925, %5991 ], [ %6008, %5994 ]
  %5999 = add nsw i64 %5995, 1
  %6000 = trunc nsw i64 %5999 to i32
  store i32 %6000, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6001 = getelementptr inbounds i8, ptr %5901, i64 %5995
  %6002 = load i8, ptr %6001, align 1, !tbaa !12
  %6003 = shl i32 %5998, 13
  %6004 = xor i32 %6003, %5998
  %6005 = lshr i32 %6004, 17
  %6006 = xor i32 %6005, %6004
  %6007 = shl i32 %6006, 5
  %6008 = xor i32 %6007, %6006
  store i32 %6008, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6009 = trunc i32 %6008 to i8
  %6010 = xor i8 %6002, %6009
  %6011 = icmp samesign ult i64 %5996, 8
  %6012 = zext i8 %6010 to i64
  %6013 = shl nuw nsw i64 %5996, 3
  %6014 = shl nuw i64 %6012, %6013
  %6015 = select i1 %6011, i64 %6014, i64 0
  %6016 = or i64 %6015, %5997
  %6017 = add nuw nsw i64 %5996, 1
  %6018 = icmp eq i64 %6017, %5993
  br i1 %6018, label %6021, label %5994, !llvm.loop !33

6019:                                             ; preds = %5985
  %6020 = phi i64 [ %5986, %5985 ]
  br label %6025

6021:                                             ; preds = %5994
  %6022 = phi i32 [ %6000, %5994 ]
  %6023 = phi i32 [ %6008, %5994 ]
  %6024 = phi i64 [ %6016, %5994 ]
  br label %6025

6025:                                             ; preds = %6021, %6019, %5989, %5963
  %6026 = phi i32 [ %5964, %5963 ], [ %5925, %5989 ], [ %5964, %6019 ], [ %6023, %6021 ]
  %6027 = phi i32 [ %5965, %5963 ], [ %5916, %5989 ], [ %5965, %6019 ], [ %6022, %6021 ]
  %6028 = phi i64 [ 0, %5963 ], [ 0, %5989 ], [ %6020, %6019 ], [ %6024, %6021 ]
  %6029 = sext i32 %6027 to i64
  %6030 = add nsw i64 %6029, 1
  %6031 = trunc nsw i64 %6030 to i32
  store i32 %6031, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6032 = getelementptr inbounds i8, ptr %5901, i64 %6029
  %6033 = load i8, ptr %6032, align 1, !tbaa !12
  %6034 = shl i32 %6026, 13
  %6035 = xor i32 %6034, %6026
  %6036 = lshr i32 %6035, 17
  %6037 = xor i32 %6036, %6035
  %6038 = shl i32 %6037, 5
  %6039 = xor i32 %6038, %6037
  store i32 %6039, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6040 = trunc i32 %6039 to i8
  %6041 = xor i8 %6033, %6040
  %6042 = zext i8 %6041 to i32
  %6043 = add nsw i64 %6029, 2
  %6044 = trunc nsw i64 %6043 to i32
  store i32 %6044, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6045 = getelementptr inbounds i8, ptr %5901, i64 %6030
  %6046 = load i8, ptr %6045, align 1, !tbaa !12
  %6047 = shl i32 %6039, 13
  %6048 = xor i32 %6047, %6039
  %6049 = lshr i32 %6048, 17
  %6050 = xor i32 %6049, %6048
  %6051 = shl i32 %6050, 5
  %6052 = xor i32 %6051, %6050
  store i32 %6052, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6053 = trunc i32 %6052 to i8
  %6054 = xor i8 %6046, %6053
  %6055 = zext i8 %6054 to i32
  %6056 = shl nuw nsw i32 %6055, 8
  %6057 = or disjoint i32 %6056, %6042
  %6058 = add nsw i64 %6029, 3
  %6059 = trunc nsw i64 %6058 to i32
  store i32 %6059, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6060 = getelementptr inbounds i8, ptr %5901, i64 %6043
  %6061 = load i8, ptr %6060, align 1, !tbaa !12
  %6062 = shl i32 %6052, 13
  %6063 = xor i32 %6062, %6052
  %6064 = lshr i32 %6063, 17
  %6065 = xor i32 %6064, %6063
  %6066 = shl i32 %6065, 5
  %6067 = xor i32 %6066, %6065
  store i32 %6067, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6068 = trunc i32 %6067 to i8
  %6069 = xor i8 %6061, %6068
  %6070 = zext i8 %6069 to i32
  %6071 = shl nuw nsw i32 %6070, 16
  %6072 = or disjoint i32 %6071, %6057
  %6073 = add i32 %6027, 4
  store i32 %6073, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6074 = getelementptr inbounds i8, ptr %5901, i64 %6058
  %6075 = load i8, ptr %6074, align 1, !tbaa !12
  %6076 = shl i32 %6067, 13
  %6077 = xor i32 %6076, %6067
  %6078 = lshr i32 %6077, 17
  %6079 = xor i32 %6078, %6077
  %6080 = shl i32 %6079, 5
  %6081 = xor i32 %6080, %6079
  store i32 %6081, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6082 = trunc i32 %6081 to i8
  %6083 = xor i8 %6075, %6082
  %6084 = zext i8 %6083 to i32
  %6085 = shl nuw i32 %6084, 24
  %6086 = or disjoint i32 %6085, %6072
  %6087 = icmp eq i32 %6086, 0
  br i1 %6087, label %6262, label %6088

6088:                                             ; preds = %6025
  %6089 = load i32, ptr @pointer_size__Z8vm_entryv, align 4
  %6090 = icmp sgt i32 %6089, 0
  %6091 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %6092 = inttoptr i64 %6091 to ptr
  %6093 = zext nneg i32 %6089 to i64
  %6094 = zext nneg i32 %6089 to i64
  br label %6095

6095:                                             ; preds = %6251, %6088
  %6096 = phi i64 [ 0, %6088 ], [ %6256, %6251 ]
  %6097 = phi i32 [ 0, %6088 ], [ %6257, %6251 ]
  %6098 = phi i32 [ %6073, %6088 ], [ %6253, %6251 ]
  %6099 = phi i32 [ %6081, %6088 ], [ %6252, %6251 ]
  %6100 = add nsw i32 %6098, 1
  store i32 %6100, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6101 = sext i32 %6098 to i64
  %6102 = getelementptr inbounds i8, ptr %5901, i64 %6101
  %6103 = load i8, ptr %6102, align 1, !tbaa !12
  %6104 = shl i32 %6099, 13
  %6105 = xor i32 %6104, %6099
  %6106 = lshr i32 %6105, 17
  %6107 = xor i32 %6106, %6105
  %6108 = shl i32 %6107, 5
  %6109 = xor i32 %6108, %6107
  store i32 %6109, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6110 = trunc i32 %6109 to i8
  %6111 = xor i8 %6103, %6110
  %6112 = add i32 %6098, 2
  store i32 %6112, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6113 = sext i32 %6100 to i64
  %6114 = getelementptr inbounds i8, ptr %5901, i64 %6113
  %6115 = load i8, ptr %6114, align 1, !tbaa !12
  %6116 = shl i32 %6109, 13
  %6117 = xor i32 %6116, %6109
  %6118 = lshr i32 %6117, 17
  %6119 = xor i32 %6118, %6117
  %6120 = shl i32 %6119, 5
  %6121 = xor i32 %6120, %6119
  store i32 %6121, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6122 = trunc i32 %6121 to i8
  %6123 = icmp eq i8 %6115, %6122
  br i1 %6123, label %6124, label %6180

6124:                                             ; preds = %6095
  br i1 %6090, label %6125, label %6156

6125:                                             ; preds = %6124
  %6126 = sext i32 %6112 to i64
  br label %6127

6127:                                             ; preds = %6127, %6125
  %6128 = phi i64 [ %6126, %6125 ], [ %6132, %6127 ]
  %6129 = phi i64 [ 0, %6125 ], [ %6150, %6127 ]
  %6130 = phi i64 [ 0, %6125 ], [ %6149, %6127 ]
  %6131 = phi i32 [ %6121, %6125 ], [ %6141, %6127 ]
  %6132 = add nsw i64 %6128, 1
  %6133 = trunc nsw i64 %6132 to i32
  store i32 %6133, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6134 = getelementptr inbounds i8, ptr %5901, i64 %6128
  %6135 = load i8, ptr %6134, align 1, !tbaa !12
  %6136 = shl i32 %6131, 13
  %6137 = xor i32 %6136, %6131
  %6138 = lshr i32 %6137, 17
  %6139 = xor i32 %6138, %6137
  %6140 = shl i32 %6139, 5
  %6141 = xor i32 %6140, %6139
  store i32 %6141, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6142 = trunc i32 %6141 to i8
  %6143 = xor i8 %6135, %6142
  %6144 = icmp samesign ult i64 %6129, 8
  %6145 = zext i8 %6143 to i64
  %6146 = shl nuw nsw i64 %6129, 3
  %6147 = shl nuw i64 %6145, %6146
  %6148 = select i1 %6144, i64 %6147, i64 0
  %6149 = or i64 %6148, %6130
  %6150 = add nuw nsw i64 %6129, 1
  %6151 = icmp eq i64 %6150, %6093
  br i1 %6151, label %6152, label %6127, !llvm.loop !33

6152:                                             ; preds = %6127
  %6153 = phi i32 [ %6133, %6127 ]
  %6154 = phi i32 [ %6141, %6127 ]
  %6155 = phi i64 [ %6149, %6127 ]
  br label %6156

6156:                                             ; preds = %6152, %6124
  %6157 = phi i32 [ %6121, %6124 ], [ %6154, %6152 ]
  %6158 = phi i32 [ %6112, %6124 ], [ %6153, %6152 ]
  %6159 = phi i64 [ 0, %6124 ], [ %6155, %6152 ]
  %6160 = icmp eq i8 %6103, %6110
  br i1 %6160, label %6216, label %6161

6161:                                             ; preds = %6156
  %6162 = zext i8 %6111 to i64
  br label %6163

6163:                                             ; preds = %6176, %6161
  %6164 = phi i64 [ 0, %6161 ], [ %6178, %6176 ]
  %6165 = phi i64 [ %6159, %6161 ], [ %6167, %6176 ]
  %6166 = phi i64 [ 0, %6161 ], [ %6177, %6176 ]
  %6167 = add i64 %6165, 1
  %6168 = icmp samesign ult i64 %6164, 8
  br i1 %6168, label %6169, label %6176

6169:                                             ; preds = %6163
  %6170 = getelementptr inbounds nuw i8, ptr %6092, i64 %6165
  %6171 = load i8, ptr %6170, align 1, !tbaa !12
  %6172 = zext i8 %6171 to i64
  %6173 = shl nuw nsw i64 %6164, 3
  %6174 = shl nuw i64 %6172, %6173
  %6175 = or i64 %6174, %6166
  br label %6176

6176:                                             ; preds = %6169, %6163
  %6177 = phi i64 [ %6175, %6169 ], [ %6166, %6163 ]
  %6178 = add nuw nsw i64 %6164, 1
  %6179 = icmp eq i64 %6178, %6162
  br i1 %6179, label %6210, label %6163, !llvm.loop !35

6180:                                             ; preds = %6095
  %6181 = icmp eq i8 %6103, %6110
  br i1 %6181, label %6216, label %6182

6182:                                             ; preds = %6180
  %6183 = sext i32 %6112 to i64
  %6184 = zext i8 %6111 to i64
  br label %6185

6185:                                             ; preds = %6185, %6182
  %6186 = phi i64 [ %6183, %6182 ], [ %6190, %6185 ]
  %6187 = phi i64 [ 0, %6182 ], [ %6208, %6185 ]
  %6188 = phi i64 [ 0, %6182 ], [ %6207, %6185 ]
  %6189 = phi i32 [ %6121, %6182 ], [ %6199, %6185 ]
  %6190 = add nsw i64 %6186, 1
  %6191 = trunc nsw i64 %6190 to i32
  store i32 %6191, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6192 = getelementptr inbounds i8, ptr %5901, i64 %6186
  %6193 = load i8, ptr %6192, align 1, !tbaa !12
  %6194 = shl i32 %6189, 13
  %6195 = xor i32 %6194, %6189
  %6196 = lshr i32 %6195, 17
  %6197 = xor i32 %6196, %6195
  %6198 = shl i32 %6197, 5
  %6199 = xor i32 %6198, %6197
  store i32 %6199, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6200 = trunc i32 %6199 to i8
  %6201 = xor i8 %6193, %6200
  %6202 = icmp samesign ult i64 %6187, 8
  %6203 = zext i8 %6201 to i64
  %6204 = shl nuw nsw i64 %6187, 3
  %6205 = shl nuw i64 %6203, %6204
  %6206 = select i1 %6202, i64 %6205, i64 0
  %6207 = or i64 %6206, %6188
  %6208 = add nuw nsw i64 %6187, 1
  %6209 = icmp eq i64 %6208, %6184
  br i1 %6209, label %6212, label %6185, !llvm.loop !33

6210:                                             ; preds = %6176
  %6211 = phi i64 [ %6177, %6176 ]
  br label %6216

6212:                                             ; preds = %6185
  %6213 = phi i32 [ %6191, %6185 ]
  %6214 = phi i32 [ %6199, %6185 ]
  %6215 = phi i64 [ %6207, %6185 ]
  br label %6216

6216:                                             ; preds = %6212, %6210, %6180, %6156
  %6217 = phi i32 [ %6157, %6156 ], [ %6121, %6180 ], [ %6157, %6210 ], [ %6214, %6212 ]
  %6218 = phi i32 [ %6158, %6156 ], [ %6112, %6180 ], [ %6158, %6210 ], [ %6213, %6212 ]
  %6219 = phi i64 [ 0, %6156 ], [ 0, %6180 ], [ %6211, %6210 ], [ %6215, %6212 ]
  br i1 %6090, label %6220, label %6251

6220:                                             ; preds = %6216
  %6221 = sext i32 %6218 to i64
  br label %6222

6222:                                             ; preds = %6222, %6220
  %6223 = phi i64 [ %6221, %6220 ], [ %6227, %6222 ]
  %6224 = phi i64 [ 0, %6220 ], [ %6245, %6222 ]
  %6225 = phi i64 [ 0, %6220 ], [ %6244, %6222 ]
  %6226 = phi i32 [ %6217, %6220 ], [ %6236, %6222 ]
  %6227 = add nsw i64 %6223, 1
  %6228 = trunc nsw i64 %6227 to i32
  store i32 %6228, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6229 = getelementptr inbounds i8, ptr %5901, i64 %6223
  %6230 = load i8, ptr %6229, align 1, !tbaa !12
  %6231 = shl i32 %6226, 13
  %6232 = xor i32 %6231, %6226
  %6233 = lshr i32 %6232, 17
  %6234 = xor i32 %6233, %6232
  %6235 = shl i32 %6234, 5
  %6236 = xor i32 %6235, %6234
  store i32 %6236, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6237 = trunc i32 %6236 to i8
  %6238 = xor i8 %6230, %6237
  %6239 = icmp samesign ult i64 %6224, 8
  %6240 = zext i8 %6238 to i64
  %6241 = shl nuw nsw i64 %6224, 3
  %6242 = shl nuw i64 %6240, %6241
  %6243 = select i1 %6239, i64 %6242, i64 0
  %6244 = or i64 %6243, %6225
  %6245 = add nuw nsw i64 %6224, 1
  %6246 = icmp eq i64 %6245, %6094
  br i1 %6246, label %6247, label %6222, !llvm.loop !33

6247:                                             ; preds = %6222
  %6248 = phi i32 [ %6228, %6222 ]
  %6249 = phi i32 [ %6236, %6222 ]
  %6250 = phi i64 [ %6244, %6222 ]
  br label %6251

6251:                                             ; preds = %6247, %6216
  %6252 = phi i32 [ %6217, %6216 ], [ %6249, %6247 ]
  %6253 = phi i32 [ %6218, %6216 ], [ %6248, %6247 ]
  %6254 = phi i64 [ 0, %6216 ], [ %6250, %6247 ]
  %6255 = icmp eq i64 %6028, %6219
  %6256 = select i1 %6255, i64 %6254, i64 %6096
  %6257 = add nuw i32 %6097, 1
  %6258 = icmp eq i32 %6257, %6086
  br i1 %6258, label %6259, label %6095, !llvm.loop !58

6259:                                             ; preds = %6251
  %6260 = phi i64 [ %6256, %6251 ]
  %6261 = trunc i64 %6260 to i32
  br label %6262

6262:                                             ; preds = %6259, %6025
  %6263 = phi i32 [ 0, %6025 ], [ %6261, %6259 ]
  store i32 %6263, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  br label %2370

6264:                                             ; preds = %239
  %6265 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %6266 = inttoptr i64 %6265 to ptr
  %6267 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6268 = add nsw i32 %6267, 1
  store i32 %6268, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6269 = sext i32 %6267 to i64
  %6270 = getelementptr inbounds i8, ptr %6266, i64 %6269
  %6271 = load i8, ptr %6270, align 1, !tbaa !12
  %6272 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6273 = shl i32 %6272, 13
  %6274 = xor i32 %6273, %6272
  %6275 = lshr i32 %6274, 17
  %6276 = xor i32 %6275, %6274
  %6277 = shl i32 %6276, 5
  %6278 = xor i32 %6277, %6276
  %6279 = trunc i32 %6278 to i8
  %6280 = xor i8 %6271, %6279
  %6281 = add nsw i32 %6267, 2
  %6282 = shl i32 %6278, 13
  %6283 = xor i32 %6282, %6278
  %6284 = lshr i32 %6283, 17
  %6285 = xor i32 %6284, %6283
  %6286 = shl i32 %6285, 5
  %6287 = xor i32 %6286, %6285
  store i32 %6287, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6288 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %6289 = icmp sgt i32 %6288, 0
  br i1 %6289, label %6290, label %6322

6290:                                             ; preds = %6264
  %6291 = sext i32 %6281 to i64
  %6292 = zext nneg i32 %6288 to i64
  br label %6293

6293:                                             ; preds = %6293, %6290
  %6294 = phi i64 [ %6291, %6290 ], [ %6298, %6293 ]
  %6295 = phi i64 [ 0, %6290 ], [ %6316, %6293 ]
  %6296 = phi i64 [ 0, %6290 ], [ %6315, %6293 ]
  %6297 = phi i32 [ %6287, %6290 ], [ %6307, %6293 ]
  %6298 = add nsw i64 %6294, 1
  %6299 = trunc nsw i64 %6298 to i32
  store i32 %6299, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6300 = getelementptr inbounds i8, ptr %6266, i64 %6294
  %6301 = load i8, ptr %6300, align 1, !tbaa !12
  %6302 = shl i32 %6297, 13
  %6303 = xor i32 %6302, %6297
  %6304 = lshr i32 %6303, 17
  %6305 = xor i32 %6304, %6303
  %6306 = shl i32 %6305, 5
  %6307 = xor i32 %6306, %6305
  store i32 %6307, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6308 = trunc i32 %6307 to i8
  %6309 = xor i8 %6301, %6308
  %6310 = icmp samesign ult i64 %6295, 8
  %6311 = zext i8 %6309 to i64
  %6312 = shl nuw nsw i64 %6295, 3
  %6313 = shl nuw i64 %6311, %6312
  %6314 = select i1 %6310, i64 %6313, i64 0
  %6315 = or i64 %6314, %6296
  %6316 = add nuw nsw i64 %6295, 1
  %6317 = icmp eq i64 %6316, %6292
  br i1 %6317, label %6318, label %6293, !llvm.loop !33

6318:                                             ; preds = %6293
  %6319 = phi i32 [ %6299, %6293 ]
  %6320 = phi i32 [ %6307, %6293 ]
  %6321 = phi i64 [ %6315, %6293 ]
  br label %6322

6322:                                             ; preds = %6318, %6264
  %6323 = phi i32 [ %6287, %6264 ], [ %6320, %6318 ]
  %6324 = phi i32 [ %6281, %6264 ], [ %6319, %6318 ]
  %6325 = phi i64 [ 0, %6264 ], [ %6321, %6318 ]
  %6326 = add nsw i32 %6324, 1
  store i32 %6326, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6327 = sext i32 %6324 to i64
  %6328 = getelementptr inbounds i8, ptr %6266, i64 %6327
  %6329 = load i8, ptr %6328, align 1, !tbaa !12
  %6330 = shl i32 %6323, 13
  %6331 = xor i32 %6330, %6323
  %6332 = lshr i32 %6331, 17
  %6333 = xor i32 %6332, %6331
  %6334 = shl i32 %6333, 5
  %6335 = xor i32 %6334, %6333
  store i32 %6335, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6336 = trunc i32 %6335 to i8
  %6337 = xor i8 %6329, %6336
  %6338 = add nsw i32 %6324, 2
  store i32 %6338, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6339 = sext i32 %6326 to i64
  %6340 = getelementptr inbounds i8, ptr %6266, i64 %6339
  %6341 = load i8, ptr %6340, align 1, !tbaa !12
  %6342 = shl i32 %6335, 13
  %6343 = xor i32 %6342, %6335
  %6344 = lshr i32 %6343, 17
  %6345 = xor i32 %6344, %6343
  %6346 = shl i32 %6345, 5
  %6347 = xor i32 %6346, %6345
  store i32 %6347, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6348 = trunc i32 %6347 to i8
  %6349 = icmp eq i8 %6341, %6348
  br i1 %6349, label %6350, label %6409

6350:                                             ; preds = %6322
  br i1 %6289, label %6351, label %6383

6351:                                             ; preds = %6350
  %6352 = sext i32 %6338 to i64
  %6353 = zext nneg i32 %6288 to i64
  br label %6354

6354:                                             ; preds = %6354, %6351
  %6355 = phi i64 [ %6352, %6351 ], [ %6359, %6354 ]
  %6356 = phi i64 [ 0, %6351 ], [ %6377, %6354 ]
  %6357 = phi i64 [ 0, %6351 ], [ %6376, %6354 ]
  %6358 = phi i32 [ %6347, %6351 ], [ %6368, %6354 ]
  %6359 = add nsw i64 %6355, 1
  %6360 = trunc nsw i64 %6359 to i32
  store i32 %6360, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6361 = getelementptr inbounds i8, ptr %6266, i64 %6355
  %6362 = load i8, ptr %6361, align 1, !tbaa !12
  %6363 = shl i32 %6358, 13
  %6364 = xor i32 %6363, %6358
  %6365 = lshr i32 %6364, 17
  %6366 = xor i32 %6365, %6364
  %6367 = shl i32 %6366, 5
  %6368 = xor i32 %6367, %6366
  store i32 %6368, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6369 = trunc i32 %6368 to i8
  %6370 = xor i8 %6362, %6369
  %6371 = icmp samesign ult i64 %6356, 8
  %6372 = zext i8 %6370 to i64
  %6373 = shl nuw nsw i64 %6356, 3
  %6374 = shl nuw i64 %6372, %6373
  %6375 = select i1 %6371, i64 %6374, i64 0
  %6376 = or i64 %6375, %6357
  %6377 = add nuw nsw i64 %6356, 1
  %6378 = icmp eq i64 %6377, %6353
  br i1 %6378, label %6379, label %6354, !llvm.loop !33

6379:                                             ; preds = %6354
  %6380 = phi i32 [ %6360, %6354 ]
  %6381 = phi i32 [ %6368, %6354 ]
  %6382 = phi i64 [ %6376, %6354 ]
  br label %6383

6383:                                             ; preds = %6379, %6350
  %6384 = phi i32 [ %6347, %6350 ], [ %6381, %6379 ]
  %6385 = phi i32 [ %6338, %6350 ], [ %6380, %6379 ]
  %6386 = phi i64 [ 0, %6350 ], [ %6382, %6379 ]
  %6387 = icmp eq i8 %6329, %6336
  br i1 %6387, label %6445, label %6388

6388:                                             ; preds = %6383
  %6389 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %6390 = inttoptr i64 %6389 to ptr
  %6391 = zext i8 %6337 to i64
  br label %6392

6392:                                             ; preds = %6405, %6388
  %6393 = phi i64 [ 0, %6388 ], [ %6407, %6405 ]
  %6394 = phi i64 [ %6386, %6388 ], [ %6396, %6405 ]
  %6395 = phi i64 [ 0, %6388 ], [ %6406, %6405 ]
  %6396 = add i64 %6394, 1
  %6397 = icmp samesign ult i64 %6393, 8
  br i1 %6397, label %6398, label %6405

6398:                                             ; preds = %6392
  %6399 = getelementptr inbounds nuw i8, ptr %6390, i64 %6394
  %6400 = load i8, ptr %6399, align 1, !tbaa !12
  %6401 = zext i8 %6400 to i64
  %6402 = shl nuw nsw i64 %6393, 3
  %6403 = shl nuw i64 %6401, %6402
  %6404 = or i64 %6403, %6395
  br label %6405

6405:                                             ; preds = %6398, %6392
  %6406 = phi i64 [ %6404, %6398 ], [ %6395, %6392 ]
  %6407 = add nuw nsw i64 %6393, 1
  %6408 = icmp eq i64 %6407, %6391
  br i1 %6408, label %6439, label %6392, !llvm.loop !35

6409:                                             ; preds = %6322
  %6410 = icmp eq i8 %6329, %6336
  br i1 %6410, label %6445, label %6411

6411:                                             ; preds = %6409
  %6412 = sext i32 %6338 to i64
  %6413 = zext i8 %6337 to i64
  br label %6414

6414:                                             ; preds = %6414, %6411
  %6415 = phi i64 [ %6412, %6411 ], [ %6419, %6414 ]
  %6416 = phi i64 [ 0, %6411 ], [ %6437, %6414 ]
  %6417 = phi i64 [ 0, %6411 ], [ %6436, %6414 ]
  %6418 = phi i32 [ %6347, %6411 ], [ %6428, %6414 ]
  %6419 = add nsw i64 %6415, 1
  %6420 = trunc nsw i64 %6419 to i32
  store i32 %6420, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6421 = getelementptr inbounds i8, ptr %6266, i64 %6415
  %6422 = load i8, ptr %6421, align 1, !tbaa !12
  %6423 = shl i32 %6418, 13
  %6424 = xor i32 %6423, %6418
  %6425 = lshr i32 %6424, 17
  %6426 = xor i32 %6425, %6424
  %6427 = shl i32 %6426, 5
  %6428 = xor i32 %6427, %6426
  store i32 %6428, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6429 = trunc i32 %6428 to i8
  %6430 = xor i8 %6422, %6429
  %6431 = icmp samesign ult i64 %6416, 8
  %6432 = zext i8 %6430 to i64
  %6433 = shl nuw nsw i64 %6416, 3
  %6434 = shl nuw i64 %6432, %6433
  %6435 = select i1 %6431, i64 %6434, i64 0
  %6436 = or i64 %6435, %6417
  %6437 = add nuw nsw i64 %6416, 1
  %6438 = icmp eq i64 %6437, %6413
  br i1 %6438, label %6441, label %6414, !llvm.loop !33

6439:                                             ; preds = %6405
  %6440 = phi i64 [ %6406, %6405 ]
  br label %6445

6441:                                             ; preds = %6414
  %6442 = phi i32 [ %6420, %6414 ]
  %6443 = phi i32 [ %6428, %6414 ]
  %6444 = phi i64 [ %6436, %6414 ]
  br label %6445

6445:                                             ; preds = %6441, %6439, %6409, %6383
  %6446 = phi i32 [ %6384, %6383 ], [ %6347, %6409 ], [ %6384, %6439 ], [ %6443, %6441 ]
  %6447 = phi i32 [ %6385, %6383 ], [ %6338, %6409 ], [ %6385, %6439 ], [ %6442, %6441 ]
  %6448 = phi i64 [ 0, %6383 ], [ 0, %6409 ], [ %6440, %6439 ], [ %6444, %6441 ]
  %6449 = add nsw i32 %6447, 1
  store i32 %6449, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6450 = sext i32 %6447 to i64
  %6451 = getelementptr inbounds i8, ptr %6266, i64 %6450
  %6452 = load i8, ptr %6451, align 1, !tbaa !12
  %6453 = shl i32 %6446, 13
  %6454 = xor i32 %6453, %6446
  %6455 = lshr i32 %6454, 17
  %6456 = xor i32 %6455, %6454
  %6457 = shl i32 %6456, 5
  %6458 = xor i32 %6457, %6456
  store i32 %6458, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6459 = trunc i32 %6458 to i8
  %6460 = xor i8 %6452, %6459
  %6461 = add nsw i32 %6447, 2
  store i32 %6461, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6462 = sext i32 %6449 to i64
  %6463 = getelementptr inbounds i8, ptr %6266, i64 %6462
  %6464 = load i8, ptr %6463, align 1, !tbaa !12
  %6465 = shl i32 %6458, 13
  %6466 = xor i32 %6465, %6458
  %6467 = lshr i32 %6466, 17
  %6468 = xor i32 %6467, %6466
  %6469 = shl i32 %6468, 5
  %6470 = xor i32 %6469, %6468
  store i32 %6470, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6471 = trunc i32 %6470 to i8
  %6472 = icmp eq i8 %6464, %6471
  br i1 %6472, label %6473, label %6532

6473:                                             ; preds = %6445
  br i1 %6289, label %6474, label %6506

6474:                                             ; preds = %6473
  %6475 = sext i32 %6461 to i64
  %6476 = zext nneg i32 %6288 to i64
  br label %6477

6477:                                             ; preds = %6477, %6474
  %6478 = phi i64 [ %6475, %6474 ], [ %6482, %6477 ]
  %6479 = phi i64 [ 0, %6474 ], [ %6500, %6477 ]
  %6480 = phi i64 [ 0, %6474 ], [ %6499, %6477 ]
  %6481 = phi i32 [ %6470, %6474 ], [ %6491, %6477 ]
  %6482 = add nsw i64 %6478, 1
  %6483 = trunc nsw i64 %6482 to i32
  store i32 %6483, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6484 = getelementptr inbounds i8, ptr %6266, i64 %6478
  %6485 = load i8, ptr %6484, align 1, !tbaa !12
  %6486 = shl i32 %6481, 13
  %6487 = xor i32 %6486, %6481
  %6488 = lshr i32 %6487, 17
  %6489 = xor i32 %6488, %6487
  %6490 = shl i32 %6489, 5
  %6491 = xor i32 %6490, %6489
  store i32 %6491, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6492 = trunc i32 %6491 to i8
  %6493 = xor i8 %6485, %6492
  %6494 = icmp samesign ult i64 %6479, 8
  %6495 = zext i8 %6493 to i64
  %6496 = shl nuw nsw i64 %6479, 3
  %6497 = shl nuw i64 %6495, %6496
  %6498 = select i1 %6494, i64 %6497, i64 0
  %6499 = or i64 %6498, %6480
  %6500 = add nuw nsw i64 %6479, 1
  %6501 = icmp eq i64 %6500, %6476
  br i1 %6501, label %6502, label %6477, !llvm.loop !33

6502:                                             ; preds = %6477
  %6503 = phi i32 [ %6483, %6477 ]
  %6504 = phi i32 [ %6491, %6477 ]
  %6505 = phi i64 [ %6499, %6477 ]
  br label %6506

6506:                                             ; preds = %6502, %6473
  %6507 = phi i32 [ %6470, %6473 ], [ %6504, %6502 ]
  %6508 = phi i32 [ %6461, %6473 ], [ %6503, %6502 ]
  %6509 = phi i64 [ 0, %6473 ], [ %6505, %6502 ]
  %6510 = icmp eq i8 %6452, %6459
  br i1 %6510, label %6568, label %6511

6511:                                             ; preds = %6506
  %6512 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %6513 = inttoptr i64 %6512 to ptr
  %6514 = zext i8 %6460 to i64
  br label %6515

6515:                                             ; preds = %6528, %6511
  %6516 = phi i64 [ 0, %6511 ], [ %6530, %6528 ]
  %6517 = phi i64 [ %6509, %6511 ], [ %6519, %6528 ]
  %6518 = phi i64 [ 0, %6511 ], [ %6529, %6528 ]
  %6519 = add i64 %6517, 1
  %6520 = icmp samesign ult i64 %6516, 8
  br i1 %6520, label %6521, label %6528

6521:                                             ; preds = %6515
  %6522 = getelementptr inbounds nuw i8, ptr %6513, i64 %6517
  %6523 = load i8, ptr %6522, align 1, !tbaa !12
  %6524 = zext i8 %6523 to i64
  %6525 = shl nuw nsw i64 %6516, 3
  %6526 = shl nuw i64 %6524, %6525
  %6527 = or i64 %6526, %6518
  br label %6528

6528:                                             ; preds = %6521, %6515
  %6529 = phi i64 [ %6527, %6521 ], [ %6518, %6515 ]
  %6530 = add nuw nsw i64 %6516, 1
  %6531 = icmp eq i64 %6530, %6514
  br i1 %6531, label %6562, label %6515, !llvm.loop !35

6532:                                             ; preds = %6445
  %6533 = icmp eq i8 %6452, %6459
  br i1 %6533, label %6568, label %6534

6534:                                             ; preds = %6532
  %6535 = sext i32 %6461 to i64
  %6536 = zext i8 %6460 to i64
  br label %6537

6537:                                             ; preds = %6537, %6534
  %6538 = phi i64 [ %6535, %6534 ], [ %6542, %6537 ]
  %6539 = phi i64 [ 0, %6534 ], [ %6560, %6537 ]
  %6540 = phi i64 [ 0, %6534 ], [ %6559, %6537 ]
  %6541 = phi i32 [ %6470, %6534 ], [ %6551, %6537 ]
  %6542 = add nsw i64 %6538, 1
  %6543 = trunc nsw i64 %6542 to i32
  store i32 %6543, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6544 = getelementptr inbounds i8, ptr %6266, i64 %6538
  %6545 = load i8, ptr %6544, align 1, !tbaa !12
  %6546 = shl i32 %6541, 13
  %6547 = xor i32 %6546, %6541
  %6548 = lshr i32 %6547, 17
  %6549 = xor i32 %6548, %6547
  %6550 = shl i32 %6549, 5
  %6551 = xor i32 %6550, %6549
  store i32 %6551, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6552 = trunc i32 %6551 to i8
  %6553 = xor i8 %6545, %6552
  %6554 = icmp samesign ult i64 %6539, 8
  %6555 = zext i8 %6553 to i64
  %6556 = shl nuw nsw i64 %6539, 3
  %6557 = shl nuw i64 %6555, %6556
  %6558 = select i1 %6554, i64 %6557, i64 0
  %6559 = or i64 %6558, %6540
  %6560 = add nuw nsw i64 %6539, 1
  %6561 = icmp eq i64 %6560, %6536
  br i1 %6561, label %6564, label %6537, !llvm.loop !33

6562:                                             ; preds = %6528
  %6563 = phi i64 [ %6529, %6528 ]
  br label %6568

6564:                                             ; preds = %6537
  %6565 = phi i32 [ %6543, %6537 ]
  %6566 = phi i32 [ %6551, %6537 ]
  %6567 = phi i64 [ %6559, %6537 ]
  br label %6568

6568:                                             ; preds = %6564, %6562, %6532, %6506
  %6569 = phi i32 [ %6507, %6506 ], [ %6470, %6532 ], [ %6507, %6562 ], [ %6566, %6564 ]
  %6570 = phi i32 [ %6508, %6506 ], [ %6461, %6532 ], [ %6508, %6562 ], [ %6565, %6564 ]
  %6571 = phi i64 [ 0, %6506 ], [ 0, %6532 ], [ %6563, %6562 ], [ %6567, %6564 ]
  %6572 = sext i32 %6570 to i64
  %6573 = add nsw i64 %6572, 1
  %6574 = trunc nsw i64 %6573 to i32
  store i32 %6574, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6575 = getelementptr inbounds i8, ptr %6266, i64 %6572
  %6576 = load i8, ptr %6575, align 1, !tbaa !12
  %6577 = shl i32 %6569, 13
  %6578 = xor i32 %6577, %6569
  %6579 = lshr i32 %6578, 17
  %6580 = xor i32 %6579, %6578
  %6581 = shl i32 %6580, 5
  %6582 = xor i32 %6581, %6580
  store i32 %6582, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6583 = trunc i32 %6582 to i8
  %6584 = xor i8 %6576, %6583
  %6585 = zext i8 %6584 to i64
  %6586 = add nsw i64 %6572, 2
  %6587 = trunc nsw i64 %6586 to i32
  store i32 %6587, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6588 = getelementptr inbounds i8, ptr %6266, i64 %6573
  %6589 = load i8, ptr %6588, align 1, !tbaa !12
  %6590 = shl i32 %6582, 13
  %6591 = xor i32 %6590, %6582
  %6592 = lshr i32 %6591, 17
  %6593 = xor i32 %6592, %6591
  %6594 = shl i32 %6593, 5
  %6595 = xor i32 %6594, %6593
  store i32 %6595, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6596 = trunc i32 %6595 to i8
  %6597 = xor i8 %6589, %6596
  %6598 = zext i8 %6597 to i64
  %6599 = shl nuw nsw i64 %6598, 8
  %6600 = or disjoint i64 %6599, %6585
  %6601 = add nsw i64 %6572, 3
  %6602 = trunc nsw i64 %6601 to i32
  store i32 %6602, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6603 = getelementptr inbounds i8, ptr %6266, i64 %6586
  %6604 = load i8, ptr %6603, align 1, !tbaa !12
  %6605 = shl i32 %6595, 13
  %6606 = xor i32 %6605, %6595
  %6607 = lshr i32 %6606, 17
  %6608 = xor i32 %6607, %6606
  %6609 = shl i32 %6608, 5
  %6610 = xor i32 %6609, %6608
  store i32 %6610, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6611 = trunc i32 %6610 to i8
  %6612 = xor i8 %6604, %6611
  %6613 = zext i8 %6612 to i64
  %6614 = shl nuw nsw i64 %6613, 16
  %6615 = or disjoint i64 %6614, %6600
  %6616 = add i32 %6570, 4
  store i32 %6616, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6617 = getelementptr inbounds i8, ptr %6266, i64 %6601
  %6618 = load i8, ptr %6617, align 1, !tbaa !12
  %6619 = shl i32 %6610, 13
  %6620 = xor i32 %6619, %6610
  %6621 = lshr i32 %6620, 17
  %6622 = xor i32 %6621, %6620
  %6623 = shl i32 %6622, 5
  %6624 = xor i32 %6623, %6622
  store i32 %6624, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6625 = trunc i32 %6624 to i8
  %6626 = xor i8 %6618, %6625
  %6627 = zext i8 %6626 to i64
  %6628 = shl nuw nsw i64 %6627, 24
  %6629 = or disjoint i64 %6628, %6615
  %6630 = trunc nuw i64 %6629 to i32
  %6631 = icmp sgt i32 %6630, 0
  br i1 %6631, label %6632, label %6654

6632:                                             ; preds = %6568
  %6633 = mul i64 %6629, %6571
  %6634 = add i64 %6633, %6448
  %6635 = inttoptr i64 %6634 to ptr
  br label %6636

6636:                                             ; preds = %6647, %6632
  %6637 = phi i64 [ 0, %6632 ], [ %6650, %6647 ]
  %6638 = phi ptr [ %6635, %6632 ], [ %6649, %6647 ]
  %6639 = phi i64 [ 0, %6632 ], [ %6648, %6647 ]
  %6640 = icmp samesign ult i64 %6637, 8
  br i1 %6640, label %6641, label %6647

6641:                                             ; preds = %6636
  %6642 = load i8, ptr %6638, align 1, !tbaa !12
  %6643 = zext i8 %6642 to i64
  %6644 = shl nuw nsw i64 %6637, 3
  %6645 = shl nuw i64 %6643, %6644
  %6646 = or i64 %6645, %6639
  br label %6647

6647:                                             ; preds = %6641, %6636
  %6648 = phi i64 [ %6646, %6641 ], [ %6639, %6636 ]
  %6649 = getelementptr inbounds nuw i8, ptr %6638, i64 1
  %6650 = add nuw nsw i64 %6637, 1
  %6651 = icmp eq i64 %6650, %6629
  br i1 %6651, label %6652, label %6636, !llvm.loop !36

6652:                                             ; preds = %6647
  %6653 = phi i64 [ %6648, %6647 ]
  br label %6654

6654:                                             ; preds = %6652, %6568
  %6655 = phi i64 [ 0, %6568 ], [ %6653, %6652 ]
  %6656 = icmp eq i8 %6271, %6279
  br i1 %6656, label %2370, label %6657

6657:                                             ; preds = %6654
  %6658 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %6659 = add i64 %6658, %6325
  %6660 = inttoptr i64 %6659 to ptr
  %6661 = zext i8 %6280 to i32
  br label %6662

6662:                                             ; preds = %6662, %6657
  %6663 = phi i32 [ %6669, %6662 ], [ 0, %6657 ]
  %6664 = phi ptr [ %6667, %6662 ], [ %6660, %6657 ]
  %6665 = phi i64 [ %6668, %6662 ], [ %6655, %6657 ]
  %6666 = trunc i64 %6665 to i8
  store i8 %6666, ptr %6664, align 1, !tbaa !12
  %6667 = getelementptr inbounds nuw i8, ptr %6664, i64 1
  %6668 = lshr i64 %6665, 8
  %6669 = add nuw nsw i32 %6663, 1
  %6670 = icmp eq i32 %6669, %6661
  br i1 %6670, label %2366, label %6662, !llvm.loop !34

6671:                                             ; preds = %239
  %6672 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %6673 = inttoptr i64 %6672 to ptr
  %6674 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6675 = add nsw i32 %6674, 1
  store i32 %6675, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6676 = sext i32 %6674 to i64
  %6677 = getelementptr inbounds i8, ptr %6673, i64 %6676
  %6678 = load i8, ptr %6677, align 1, !tbaa !12
  %6679 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6680 = shl i32 %6679, 13
  %6681 = xor i32 %6680, %6679
  %6682 = lshr i32 %6681, 17
  %6683 = xor i32 %6682, %6681
  %6684 = shl i32 %6683, 5
  %6685 = xor i32 %6684, %6683
  %6686 = trunc i32 %6685 to i8
  %6687 = xor i8 %6678, %6686
  %6688 = add nsw i32 %6674, 2
  %6689 = shl i32 %6685, 13
  %6690 = xor i32 %6689, %6685
  %6691 = lshr i32 %6690, 17
  %6692 = xor i32 %6691, %6690
  %6693 = shl i32 %6692, 5
  %6694 = xor i32 %6693, %6692
  store i32 %6694, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6695 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %6696 = icmp sgt i32 %6695, 0
  br i1 %6696, label %6697, label %6729

6697:                                             ; preds = %6671
  %6698 = sext i32 %6688 to i64
  %6699 = zext nneg i32 %6695 to i64
  br label %6700

6700:                                             ; preds = %6700, %6697
  %6701 = phi i64 [ %6698, %6697 ], [ %6705, %6700 ]
  %6702 = phi i64 [ 0, %6697 ], [ %6723, %6700 ]
  %6703 = phi i64 [ 0, %6697 ], [ %6722, %6700 ]
  %6704 = phi i32 [ %6694, %6697 ], [ %6714, %6700 ]
  %6705 = add nsw i64 %6701, 1
  %6706 = trunc nsw i64 %6705 to i32
  store i32 %6706, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6707 = getelementptr inbounds i8, ptr %6673, i64 %6701
  %6708 = load i8, ptr %6707, align 1, !tbaa !12
  %6709 = shl i32 %6704, 13
  %6710 = xor i32 %6709, %6704
  %6711 = lshr i32 %6710, 17
  %6712 = xor i32 %6711, %6710
  %6713 = shl i32 %6712, 5
  %6714 = xor i32 %6713, %6712
  store i32 %6714, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6715 = trunc i32 %6714 to i8
  %6716 = xor i8 %6708, %6715
  %6717 = icmp samesign ult i64 %6702, 8
  %6718 = zext i8 %6716 to i64
  %6719 = shl nuw nsw i64 %6702, 3
  %6720 = shl nuw i64 %6718, %6719
  %6721 = select i1 %6717, i64 %6720, i64 0
  %6722 = or i64 %6721, %6703
  %6723 = add nuw nsw i64 %6702, 1
  %6724 = icmp eq i64 %6723, %6699
  br i1 %6724, label %6725, label %6700, !llvm.loop !33

6725:                                             ; preds = %6700
  %6726 = phi i32 [ %6706, %6700 ]
  %6727 = phi i32 [ %6714, %6700 ]
  %6728 = phi i64 [ %6722, %6700 ]
  br label %6729

6729:                                             ; preds = %6725, %6671
  %6730 = phi i32 [ %6694, %6671 ], [ %6727, %6725 ]
  %6731 = phi i32 [ %6688, %6671 ], [ %6726, %6725 ]
  %6732 = phi i64 [ 0, %6671 ], [ %6728, %6725 ]
  %6733 = add nsw i32 %6731, 1
  store i32 %6733, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6734 = sext i32 %6731 to i64
  %6735 = getelementptr inbounds i8, ptr %6673, i64 %6734
  %6736 = load i8, ptr %6735, align 1, !tbaa !12
  %6737 = shl i32 %6730, 13
  %6738 = xor i32 %6737, %6730
  %6739 = lshr i32 %6738, 17
  %6740 = xor i32 %6739, %6738
  %6741 = shl i32 %6740, 5
  %6742 = xor i32 %6741, %6740
  store i32 %6742, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6743 = trunc i32 %6742 to i8
  %6744 = xor i8 %6736, %6743
  %6745 = add nsw i32 %6731, 2
  store i32 %6745, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6746 = sext i32 %6733 to i64
  %6747 = getelementptr inbounds i8, ptr %6673, i64 %6746
  %6748 = load i8, ptr %6747, align 1, !tbaa !12
  %6749 = shl i32 %6742, 13
  %6750 = xor i32 %6749, %6742
  %6751 = lshr i32 %6750, 17
  %6752 = xor i32 %6751, %6750
  %6753 = shl i32 %6752, 5
  %6754 = xor i32 %6753, %6752
  store i32 %6754, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6755 = trunc i32 %6754 to i8
  %6756 = icmp eq i8 %6748, %6755
  br i1 %6756, label %6757, label %6816

6757:                                             ; preds = %6729
  br i1 %6696, label %6758, label %6790

6758:                                             ; preds = %6757
  %6759 = sext i32 %6745 to i64
  %6760 = zext nneg i32 %6695 to i64
  br label %6761

6761:                                             ; preds = %6761, %6758
  %6762 = phi i64 [ %6759, %6758 ], [ %6766, %6761 ]
  %6763 = phi i64 [ 0, %6758 ], [ %6784, %6761 ]
  %6764 = phi i64 [ 0, %6758 ], [ %6783, %6761 ]
  %6765 = phi i32 [ %6754, %6758 ], [ %6775, %6761 ]
  %6766 = add nsw i64 %6762, 1
  %6767 = trunc nsw i64 %6766 to i32
  store i32 %6767, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6768 = getelementptr inbounds i8, ptr %6673, i64 %6762
  %6769 = load i8, ptr %6768, align 1, !tbaa !12
  %6770 = shl i32 %6765, 13
  %6771 = xor i32 %6770, %6765
  %6772 = lshr i32 %6771, 17
  %6773 = xor i32 %6772, %6771
  %6774 = shl i32 %6773, 5
  %6775 = xor i32 %6774, %6773
  store i32 %6775, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6776 = trunc i32 %6775 to i8
  %6777 = xor i8 %6769, %6776
  %6778 = icmp samesign ult i64 %6763, 8
  %6779 = zext i8 %6777 to i64
  %6780 = shl nuw nsw i64 %6763, 3
  %6781 = shl nuw i64 %6779, %6780
  %6782 = select i1 %6778, i64 %6781, i64 0
  %6783 = or i64 %6782, %6764
  %6784 = add nuw nsw i64 %6763, 1
  %6785 = icmp eq i64 %6784, %6760
  br i1 %6785, label %6786, label %6761, !llvm.loop !33

6786:                                             ; preds = %6761
  %6787 = phi i32 [ %6767, %6761 ]
  %6788 = phi i32 [ %6775, %6761 ]
  %6789 = phi i64 [ %6783, %6761 ]
  br label %6790

6790:                                             ; preds = %6786, %6757
  %6791 = phi i32 [ %6754, %6757 ], [ %6788, %6786 ]
  %6792 = phi i32 [ %6745, %6757 ], [ %6787, %6786 ]
  %6793 = phi i64 [ 0, %6757 ], [ %6789, %6786 ]
  %6794 = icmp eq i8 %6736, %6743
  br i1 %6794, label %6852, label %6795

6795:                                             ; preds = %6790
  %6796 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %6797 = inttoptr i64 %6796 to ptr
  %6798 = zext i8 %6744 to i64
  br label %6799

6799:                                             ; preds = %6812, %6795
  %6800 = phi i64 [ 0, %6795 ], [ %6814, %6812 ]
  %6801 = phi i64 [ %6793, %6795 ], [ %6803, %6812 ]
  %6802 = phi i64 [ 0, %6795 ], [ %6813, %6812 ]
  %6803 = add i64 %6801, 1
  %6804 = icmp samesign ult i64 %6800, 8
  br i1 %6804, label %6805, label %6812

6805:                                             ; preds = %6799
  %6806 = getelementptr inbounds nuw i8, ptr %6797, i64 %6801
  %6807 = load i8, ptr %6806, align 1, !tbaa !12
  %6808 = zext i8 %6807 to i64
  %6809 = shl nuw nsw i64 %6800, 3
  %6810 = shl nuw i64 %6808, %6809
  %6811 = or i64 %6810, %6802
  br label %6812

6812:                                             ; preds = %6805, %6799
  %6813 = phi i64 [ %6811, %6805 ], [ %6802, %6799 ]
  %6814 = add nuw nsw i64 %6800, 1
  %6815 = icmp eq i64 %6814, %6798
  br i1 %6815, label %6846, label %6799, !llvm.loop !35

6816:                                             ; preds = %6729
  %6817 = icmp eq i8 %6736, %6743
  br i1 %6817, label %6852, label %6818

6818:                                             ; preds = %6816
  %6819 = sext i32 %6745 to i64
  %6820 = zext i8 %6744 to i64
  br label %6821

6821:                                             ; preds = %6821, %6818
  %6822 = phi i64 [ %6819, %6818 ], [ %6826, %6821 ]
  %6823 = phi i64 [ 0, %6818 ], [ %6844, %6821 ]
  %6824 = phi i64 [ 0, %6818 ], [ %6843, %6821 ]
  %6825 = phi i32 [ %6754, %6818 ], [ %6835, %6821 ]
  %6826 = add nsw i64 %6822, 1
  %6827 = trunc nsw i64 %6826 to i32
  store i32 %6827, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6828 = getelementptr inbounds i8, ptr %6673, i64 %6822
  %6829 = load i8, ptr %6828, align 1, !tbaa !12
  %6830 = shl i32 %6825, 13
  %6831 = xor i32 %6830, %6825
  %6832 = lshr i32 %6831, 17
  %6833 = xor i32 %6832, %6831
  %6834 = shl i32 %6833, 5
  %6835 = xor i32 %6834, %6833
  store i32 %6835, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6836 = trunc i32 %6835 to i8
  %6837 = xor i8 %6829, %6836
  %6838 = icmp samesign ult i64 %6823, 8
  %6839 = zext i8 %6837 to i64
  %6840 = shl nuw nsw i64 %6823, 3
  %6841 = shl nuw i64 %6839, %6840
  %6842 = select i1 %6838, i64 %6841, i64 0
  %6843 = or i64 %6842, %6824
  %6844 = add nuw nsw i64 %6823, 1
  %6845 = icmp eq i64 %6844, %6820
  br i1 %6845, label %6848, label %6821, !llvm.loop !33

6846:                                             ; preds = %6812
  %6847 = phi i64 [ %6813, %6812 ]
  br label %6852

6848:                                             ; preds = %6821
  %6849 = phi i32 [ %6827, %6821 ]
  %6850 = phi i32 [ %6835, %6821 ]
  %6851 = phi i64 [ %6843, %6821 ]
  br label %6852

6852:                                             ; preds = %6848, %6846, %6816, %6790
  %6853 = phi i32 [ %6791, %6790 ], [ %6754, %6816 ], [ %6791, %6846 ], [ %6850, %6848 ]
  %6854 = phi i32 [ %6792, %6790 ], [ %6745, %6816 ], [ %6792, %6846 ], [ %6849, %6848 ]
  %6855 = phi i64 [ 0, %6790 ], [ 0, %6816 ], [ %6847, %6846 ], [ %6851, %6848 ]
  %6856 = add nsw i32 %6854, 1
  store i32 %6856, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6857 = sext i32 %6854 to i64
  %6858 = getelementptr inbounds i8, ptr %6673, i64 %6857
  %6859 = load i8, ptr %6858, align 1, !tbaa !12
  %6860 = shl i32 %6853, 13
  %6861 = xor i32 %6860, %6853
  %6862 = lshr i32 %6861, 17
  %6863 = xor i32 %6862, %6861
  %6864 = shl i32 %6863, 5
  %6865 = xor i32 %6864, %6863
  store i32 %6865, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6866 = trunc i32 %6865 to i8
  %6867 = xor i8 %6859, %6866
  %6868 = add nsw i32 %6854, 2
  store i32 %6868, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6869 = sext i32 %6856 to i64
  %6870 = getelementptr inbounds i8, ptr %6673, i64 %6869
  %6871 = load i8, ptr %6870, align 1, !tbaa !12
  %6872 = shl i32 %6865, 13
  %6873 = xor i32 %6872, %6865
  %6874 = lshr i32 %6873, 17
  %6875 = xor i32 %6874, %6873
  %6876 = shl i32 %6875, 5
  %6877 = xor i32 %6876, %6875
  store i32 %6877, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6878 = trunc i32 %6877 to i8
  %6879 = icmp eq i8 %6871, %6878
  br i1 %6879, label %6880, label %6939

6880:                                             ; preds = %6852
  br i1 %6696, label %6881, label %6913

6881:                                             ; preds = %6880
  %6882 = sext i32 %6868 to i64
  %6883 = zext nneg i32 %6695 to i64
  br label %6884

6884:                                             ; preds = %6884, %6881
  %6885 = phi i64 [ %6882, %6881 ], [ %6889, %6884 ]
  %6886 = phi i64 [ 0, %6881 ], [ %6907, %6884 ]
  %6887 = phi i64 [ 0, %6881 ], [ %6906, %6884 ]
  %6888 = phi i32 [ %6877, %6881 ], [ %6898, %6884 ]
  %6889 = add nsw i64 %6885, 1
  %6890 = trunc nsw i64 %6889 to i32
  store i32 %6890, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6891 = getelementptr inbounds i8, ptr %6673, i64 %6885
  %6892 = load i8, ptr %6891, align 1, !tbaa !12
  %6893 = shl i32 %6888, 13
  %6894 = xor i32 %6893, %6888
  %6895 = lshr i32 %6894, 17
  %6896 = xor i32 %6895, %6894
  %6897 = shl i32 %6896, 5
  %6898 = xor i32 %6897, %6896
  store i32 %6898, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6899 = trunc i32 %6898 to i8
  %6900 = xor i8 %6892, %6899
  %6901 = icmp samesign ult i64 %6886, 8
  %6902 = zext i8 %6900 to i64
  %6903 = shl nuw nsw i64 %6886, 3
  %6904 = shl nuw i64 %6902, %6903
  %6905 = select i1 %6901, i64 %6904, i64 0
  %6906 = or i64 %6905, %6887
  %6907 = add nuw nsw i64 %6886, 1
  %6908 = icmp eq i64 %6907, %6883
  br i1 %6908, label %6909, label %6884, !llvm.loop !33

6909:                                             ; preds = %6884
  %6910 = phi i32 [ %6890, %6884 ]
  %6911 = phi i32 [ %6898, %6884 ]
  %6912 = phi i64 [ %6906, %6884 ]
  br label %6913

6913:                                             ; preds = %6909, %6880
  %6914 = phi i32 [ %6877, %6880 ], [ %6911, %6909 ]
  %6915 = phi i32 [ %6868, %6880 ], [ %6910, %6909 ]
  %6916 = phi i64 [ 0, %6880 ], [ %6912, %6909 ]
  %6917 = icmp eq i8 %6859, %6866
  br i1 %6917, label %6975, label %6918

6918:                                             ; preds = %6913
  %6919 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %6920 = inttoptr i64 %6919 to ptr
  %6921 = zext i8 %6867 to i64
  br label %6922

6922:                                             ; preds = %6935, %6918
  %6923 = phi i64 [ 0, %6918 ], [ %6937, %6935 ]
  %6924 = phi i64 [ %6916, %6918 ], [ %6926, %6935 ]
  %6925 = phi i64 [ 0, %6918 ], [ %6936, %6935 ]
  %6926 = add i64 %6924, 1
  %6927 = icmp samesign ult i64 %6923, 8
  br i1 %6927, label %6928, label %6935

6928:                                             ; preds = %6922
  %6929 = getelementptr inbounds nuw i8, ptr %6920, i64 %6924
  %6930 = load i8, ptr %6929, align 1, !tbaa !12
  %6931 = zext i8 %6930 to i64
  %6932 = shl nuw nsw i64 %6923, 3
  %6933 = shl nuw i64 %6931, %6932
  %6934 = or i64 %6933, %6925
  br label %6935

6935:                                             ; preds = %6928, %6922
  %6936 = phi i64 [ %6934, %6928 ], [ %6925, %6922 ]
  %6937 = add nuw nsw i64 %6923, 1
  %6938 = icmp eq i64 %6937, %6921
  br i1 %6938, label %6969, label %6922, !llvm.loop !35

6939:                                             ; preds = %6852
  %6940 = icmp eq i8 %6859, %6866
  br i1 %6940, label %6975, label %6941

6941:                                             ; preds = %6939
  %6942 = sext i32 %6868 to i64
  %6943 = zext i8 %6867 to i64
  br label %6944

6944:                                             ; preds = %6944, %6941
  %6945 = phi i64 [ %6942, %6941 ], [ %6949, %6944 ]
  %6946 = phi i64 [ 0, %6941 ], [ %6967, %6944 ]
  %6947 = phi i64 [ 0, %6941 ], [ %6966, %6944 ]
  %6948 = phi i32 [ %6877, %6941 ], [ %6958, %6944 ]
  %6949 = add nsw i64 %6945, 1
  %6950 = trunc nsw i64 %6949 to i32
  store i32 %6950, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6951 = getelementptr inbounds i8, ptr %6673, i64 %6945
  %6952 = load i8, ptr %6951, align 1, !tbaa !12
  %6953 = shl i32 %6948, 13
  %6954 = xor i32 %6953, %6948
  %6955 = lshr i32 %6954, 17
  %6956 = xor i32 %6955, %6954
  %6957 = shl i32 %6956, 5
  %6958 = xor i32 %6957, %6956
  store i32 %6958, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6959 = trunc i32 %6958 to i8
  %6960 = xor i8 %6952, %6959
  %6961 = icmp samesign ult i64 %6946, 8
  %6962 = zext i8 %6960 to i64
  %6963 = shl nuw nsw i64 %6946, 3
  %6964 = shl nuw i64 %6962, %6963
  %6965 = select i1 %6961, i64 %6964, i64 0
  %6966 = or i64 %6965, %6947
  %6967 = add nuw nsw i64 %6946, 1
  %6968 = icmp eq i64 %6967, %6943
  br i1 %6968, label %6971, label %6944, !llvm.loop !33

6969:                                             ; preds = %6935
  %6970 = phi i64 [ %6936, %6935 ]
  br label %6975

6971:                                             ; preds = %6944
  %6972 = phi i32 [ %6950, %6944 ]
  %6973 = phi i32 [ %6958, %6944 ]
  %6974 = phi i64 [ %6966, %6944 ]
  br label %6975

6975:                                             ; preds = %6971, %6969, %6939, %6913
  %6976 = phi i32 [ %6914, %6913 ], [ %6877, %6939 ], [ %6914, %6969 ], [ %6973, %6971 ]
  %6977 = phi i32 [ %6915, %6913 ], [ %6868, %6939 ], [ %6915, %6969 ], [ %6972, %6971 ]
  %6978 = phi i64 [ 0, %6913 ], [ 0, %6939 ], [ %6970, %6969 ], [ %6974, %6971 ]
  %6979 = add nsw i32 %6977, 1
  store i32 %6979, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6980 = sext i32 %6977 to i64
  %6981 = getelementptr inbounds i8, ptr %6673, i64 %6980
  %6982 = load i8, ptr %6981, align 1, !tbaa !12
  %6983 = shl i32 %6976, 13
  %6984 = xor i32 %6983, %6976
  %6985 = lshr i32 %6984, 17
  %6986 = xor i32 %6985, %6984
  %6987 = shl i32 %6986, 5
  %6988 = xor i32 %6987, %6986
  store i32 %6988, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %6989 = trunc i32 %6988 to i8
  %6990 = xor i8 %6982, %6989
  %6991 = add nsw i32 %6977, 2
  store i32 %6991, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %6992 = sext i32 %6979 to i64
  %6993 = getelementptr inbounds i8, ptr %6673, i64 %6992
  %6994 = load i8, ptr %6993, align 1, !tbaa !12
  %6995 = shl i32 %6988, 13
  %6996 = xor i32 %6995, %6988
  %6997 = lshr i32 %6996, 17
  %6998 = xor i32 %6997, %6996
  %6999 = shl i32 %6998, 5
  %7000 = xor i32 %6999, %6998
  store i32 %7000, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7001 = trunc i32 %7000 to i8
  %7002 = icmp eq i8 %6994, %7001
  br i1 %7002, label %7003, label %7062

7003:                                             ; preds = %6975
  br i1 %6696, label %7004, label %7036

7004:                                             ; preds = %7003
  %7005 = sext i32 %6991 to i64
  %7006 = zext nneg i32 %6695 to i64
  br label %7007

7007:                                             ; preds = %7007, %7004
  %7008 = phi i64 [ %7005, %7004 ], [ %7012, %7007 ]
  %7009 = phi i64 [ 0, %7004 ], [ %7030, %7007 ]
  %7010 = phi i64 [ 0, %7004 ], [ %7029, %7007 ]
  %7011 = phi i32 [ %7000, %7004 ], [ %7021, %7007 ]
  %7012 = add nsw i64 %7008, 1
  %7013 = trunc nsw i64 %7012 to i32
  store i32 %7013, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7014 = getelementptr inbounds i8, ptr %6673, i64 %7008
  %7015 = load i8, ptr %7014, align 1, !tbaa !12
  %7016 = shl i32 %7011, 13
  %7017 = xor i32 %7016, %7011
  %7018 = lshr i32 %7017, 17
  %7019 = xor i32 %7018, %7017
  %7020 = shl i32 %7019, 5
  %7021 = xor i32 %7020, %7019
  store i32 %7021, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7022 = trunc i32 %7021 to i8
  %7023 = xor i8 %7015, %7022
  %7024 = icmp samesign ult i64 %7009, 8
  %7025 = zext i8 %7023 to i64
  %7026 = shl nuw nsw i64 %7009, 3
  %7027 = shl nuw i64 %7025, %7026
  %7028 = select i1 %7024, i64 %7027, i64 0
  %7029 = or i64 %7028, %7010
  %7030 = add nuw nsw i64 %7009, 1
  %7031 = icmp eq i64 %7030, %7006
  br i1 %7031, label %7032, label %7007, !llvm.loop !33

7032:                                             ; preds = %7007
  %7033 = phi i32 [ %7013, %7007 ]
  %7034 = phi i32 [ %7021, %7007 ]
  %7035 = phi i64 [ %7029, %7007 ]
  br label %7036

7036:                                             ; preds = %7032, %7003
  %7037 = phi i32 [ %7000, %7003 ], [ %7034, %7032 ]
  %7038 = phi i32 [ %6991, %7003 ], [ %7033, %7032 ]
  %7039 = phi i64 [ 0, %7003 ], [ %7035, %7032 ]
  %7040 = icmp eq i8 %6982, %6989
  br i1 %7040, label %7098, label %7041

7041:                                             ; preds = %7036
  %7042 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %7043 = inttoptr i64 %7042 to ptr
  %7044 = zext i8 %6990 to i64
  br label %7045

7045:                                             ; preds = %7058, %7041
  %7046 = phi i64 [ 0, %7041 ], [ %7060, %7058 ]
  %7047 = phi i64 [ %7039, %7041 ], [ %7049, %7058 ]
  %7048 = phi i64 [ 0, %7041 ], [ %7059, %7058 ]
  %7049 = add i64 %7047, 1
  %7050 = icmp samesign ult i64 %7046, 8
  br i1 %7050, label %7051, label %7058

7051:                                             ; preds = %7045
  %7052 = getelementptr inbounds nuw i8, ptr %7043, i64 %7047
  %7053 = load i8, ptr %7052, align 1, !tbaa !12
  %7054 = zext i8 %7053 to i64
  %7055 = shl nuw nsw i64 %7046, 3
  %7056 = shl nuw i64 %7054, %7055
  %7057 = or i64 %7056, %7048
  br label %7058

7058:                                             ; preds = %7051, %7045
  %7059 = phi i64 [ %7057, %7051 ], [ %7048, %7045 ]
  %7060 = add nuw nsw i64 %7046, 1
  %7061 = icmp eq i64 %7060, %7044
  br i1 %7061, label %7092, label %7045, !llvm.loop !35

7062:                                             ; preds = %6975
  %7063 = icmp eq i8 %6982, %6989
  br i1 %7063, label %7098, label %7064

7064:                                             ; preds = %7062
  %7065 = sext i32 %6991 to i64
  %7066 = zext i8 %6990 to i64
  br label %7067

7067:                                             ; preds = %7067, %7064
  %7068 = phi i64 [ %7065, %7064 ], [ %7072, %7067 ]
  %7069 = phi i64 [ 0, %7064 ], [ %7090, %7067 ]
  %7070 = phi i64 [ 0, %7064 ], [ %7089, %7067 ]
  %7071 = phi i32 [ %7000, %7064 ], [ %7081, %7067 ]
  %7072 = add nsw i64 %7068, 1
  %7073 = trunc nsw i64 %7072 to i32
  store i32 %7073, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7074 = getelementptr inbounds i8, ptr %6673, i64 %7068
  %7075 = load i8, ptr %7074, align 1, !tbaa !12
  %7076 = shl i32 %7071, 13
  %7077 = xor i32 %7076, %7071
  %7078 = lshr i32 %7077, 17
  %7079 = xor i32 %7078, %7077
  %7080 = shl i32 %7079, 5
  %7081 = xor i32 %7080, %7079
  store i32 %7081, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7082 = trunc i32 %7081 to i8
  %7083 = xor i8 %7075, %7082
  %7084 = icmp samesign ult i64 %7069, 8
  %7085 = zext i8 %7083 to i64
  %7086 = shl nuw nsw i64 %7069, 3
  %7087 = shl nuw i64 %7085, %7086
  %7088 = select i1 %7084, i64 %7087, i64 0
  %7089 = or i64 %7088, %7070
  %7090 = add nuw nsw i64 %7069, 1
  %7091 = icmp eq i64 %7090, %7066
  br i1 %7091, label %7094, label %7067, !llvm.loop !33

7092:                                             ; preds = %7058
  %7093 = phi i64 [ %7059, %7058 ]
  br label %7098

7094:                                             ; preds = %7067
  %7095 = phi i32 [ %7073, %7067 ]
  %7096 = phi i32 [ %7081, %7067 ]
  %7097 = phi i64 [ %7089, %7067 ]
  br label %7098

7098:                                             ; preds = %7094, %7092, %7062, %7036
  %7099 = phi i32 [ %7037, %7036 ], [ %7000, %7062 ], [ %7037, %7092 ], [ %7096, %7094 ]
  %7100 = phi i32 [ %7038, %7036 ], [ %6991, %7062 ], [ %7038, %7092 ], [ %7095, %7094 ]
  %7101 = phi i64 [ 0, %7036 ], [ 0, %7062 ], [ %7093, %7092 ], [ %7097, %7094 ]
  %7102 = sext i32 %7100 to i64
  %7103 = add nsw i64 %7102, 1
  %7104 = trunc nsw i64 %7103 to i32
  store i32 %7104, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7105 = getelementptr inbounds i8, ptr %6673, i64 %7102
  %7106 = load i8, ptr %7105, align 1, !tbaa !12
  %7107 = shl i32 %7099, 13
  %7108 = xor i32 %7107, %7099
  %7109 = lshr i32 %7108, 17
  %7110 = xor i32 %7109, %7108
  %7111 = shl i32 %7110, 5
  %7112 = xor i32 %7111, %7110
  store i32 %7112, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7113 = trunc i32 %7112 to i8
  %7114 = xor i8 %7106, %7113
  %7115 = zext i8 %7114 to i64
  %7116 = add nsw i64 %7102, 2
  %7117 = trunc nsw i64 %7116 to i32
  store i32 %7117, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7118 = getelementptr inbounds i8, ptr %6673, i64 %7103
  %7119 = load i8, ptr %7118, align 1, !tbaa !12
  %7120 = shl i32 %7112, 13
  %7121 = xor i32 %7120, %7112
  %7122 = lshr i32 %7121, 17
  %7123 = xor i32 %7122, %7121
  %7124 = shl i32 %7123, 5
  %7125 = xor i32 %7124, %7123
  store i32 %7125, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7126 = trunc i32 %7125 to i8
  %7127 = xor i8 %7119, %7126
  %7128 = zext i8 %7127 to i64
  %7129 = shl nuw nsw i64 %7128, 8
  %7130 = or disjoint i64 %7129, %7115
  %7131 = add nsw i64 %7102, 3
  %7132 = trunc nsw i64 %7131 to i32
  store i32 %7132, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7133 = getelementptr inbounds i8, ptr %6673, i64 %7116
  %7134 = load i8, ptr %7133, align 1, !tbaa !12
  %7135 = shl i32 %7125, 13
  %7136 = xor i32 %7135, %7125
  %7137 = lshr i32 %7136, 17
  %7138 = xor i32 %7137, %7136
  %7139 = shl i32 %7138, 5
  %7140 = xor i32 %7139, %7138
  store i32 %7140, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7141 = trunc i32 %7140 to i8
  %7142 = xor i8 %7134, %7141
  %7143 = zext i8 %7142 to i64
  %7144 = shl nuw nsw i64 %7143, 16
  %7145 = or disjoint i64 %7144, %7130
  %7146 = add i32 %7100, 4
  store i32 %7146, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7147 = getelementptr inbounds i8, ptr %6673, i64 %7131
  %7148 = load i8, ptr %7147, align 1, !tbaa !12
  %7149 = shl i32 %7140, 13
  %7150 = xor i32 %7149, %7140
  %7151 = lshr i32 %7150, 17
  %7152 = xor i32 %7151, %7150
  %7153 = shl i32 %7152, 5
  %7154 = xor i32 %7153, %7152
  store i32 %7154, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7155 = trunc i32 %7154 to i8
  %7156 = xor i8 %7148, %7155
  %7157 = zext i8 %7156 to i64
  %7158 = shl nuw nsw i64 %7157, 24
  %7159 = or disjoint i64 %7158, %7145
  %7160 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %7161 = add i64 %7160, %6732
  %7162 = icmp eq i8 %6678, %6686
  br i1 %7162, label %7247, label %7163

7163:                                             ; preds = %7098
  %7164 = inttoptr i64 %6855 to ptr
  %7165 = inttoptr i64 %7161 to ptr
  %7166 = zext i8 %6687 to i64
  %7167 = icmp ult i8 %6687, 8
  %7168 = sub i64 %7161, %6855
  %7169 = icmp ult i64 %7168, 32
  %7170 = select i1 %7167, i1 true, i1 %7169
  br i1 %7170, label %7171, label %7173

7171:                                             ; preds = %7244, %7231, %7163
  %7172 = phi i64 [ 0, %7163 ], [ %7176, %7231 ], [ %7236, %7244 ]
  br label %7263

7173:                                             ; preds = %7163
  %7174 = icmp ult i8 %6687, 32
  br i1 %7174, label %7234, label %7175

7175:                                             ; preds = %7173
  %7176 = and i64 %7166, 224
  %7177 = getelementptr inbounds nuw i8, ptr %7164, i64 16
  %7178 = load <16 x i8>, ptr %7164, align 1, !tbaa !12
  %7179 = load <16 x i8>, ptr %7177, align 1, !tbaa !12
  %7180 = getelementptr inbounds nuw i8, ptr %7165, i64 16
  store <16 x i8> %7178, ptr %7165, align 1, !tbaa !12
  store <16 x i8> %7179, ptr %7180, align 1, !tbaa !12
  %7181 = icmp eq i64 %7176, 32
  br i1 %7181, label %7229, label %7182, !llvm.loop !59

7182:                                             ; preds = %7175
  %7183 = getelementptr inbounds nuw i8, ptr %7164, i64 32
  %7184 = getelementptr inbounds nuw i8, ptr %7164, i64 48
  %7185 = load <16 x i8>, ptr %7183, align 1, !tbaa !12
  %7186 = load <16 x i8>, ptr %7184, align 1, !tbaa !12
  %7187 = getelementptr inbounds nuw i8, ptr %7165, i64 32
  %7188 = getelementptr inbounds nuw i8, ptr %7165, i64 48
  store <16 x i8> %7185, ptr %7187, align 1, !tbaa !12
  store <16 x i8> %7186, ptr %7188, align 1, !tbaa !12
  %7189 = icmp eq i64 %7176, 64
  br i1 %7189, label %7229, label %7190, !llvm.loop !59

7190:                                             ; preds = %7182
  %7191 = getelementptr inbounds nuw i8, ptr %7164, i64 64
  %7192 = getelementptr inbounds nuw i8, ptr %7164, i64 80
  %7193 = load <16 x i8>, ptr %7191, align 1, !tbaa !12
  %7194 = load <16 x i8>, ptr %7192, align 1, !tbaa !12
  %7195 = getelementptr inbounds nuw i8, ptr %7165, i64 64
  %7196 = getelementptr inbounds nuw i8, ptr %7165, i64 80
  store <16 x i8> %7193, ptr %7195, align 1, !tbaa !12
  store <16 x i8> %7194, ptr %7196, align 1, !tbaa !12
  %7197 = icmp eq i64 %7176, 96
  br i1 %7197, label %7229, label %7198, !llvm.loop !59

7198:                                             ; preds = %7190
  %7199 = getelementptr inbounds nuw i8, ptr %7164, i64 96
  %7200 = getelementptr inbounds nuw i8, ptr %7164, i64 112
  %7201 = load <16 x i8>, ptr %7199, align 1, !tbaa !12
  %7202 = load <16 x i8>, ptr %7200, align 1, !tbaa !12
  %7203 = getelementptr inbounds nuw i8, ptr %7165, i64 96
  %7204 = getelementptr inbounds nuw i8, ptr %7165, i64 112
  store <16 x i8> %7201, ptr %7203, align 1, !tbaa !12
  store <16 x i8> %7202, ptr %7204, align 1, !tbaa !12
  %7205 = icmp eq i64 %7176, 128
  br i1 %7205, label %7229, label %7206, !llvm.loop !59

7206:                                             ; preds = %7198
  %7207 = getelementptr inbounds nuw i8, ptr %7164, i64 128
  %7208 = getelementptr inbounds nuw i8, ptr %7164, i64 144
  %7209 = load <16 x i8>, ptr %7207, align 1, !tbaa !12
  %7210 = load <16 x i8>, ptr %7208, align 1, !tbaa !12
  %7211 = getelementptr inbounds nuw i8, ptr %7165, i64 128
  %7212 = getelementptr inbounds nuw i8, ptr %7165, i64 144
  store <16 x i8> %7209, ptr %7211, align 1, !tbaa !12
  store <16 x i8> %7210, ptr %7212, align 1, !tbaa !12
  %7213 = icmp eq i64 %7176, 160
  br i1 %7213, label %7229, label %7214, !llvm.loop !59

7214:                                             ; preds = %7206
  %7215 = getelementptr inbounds nuw i8, ptr %7164, i64 160
  %7216 = getelementptr inbounds nuw i8, ptr %7164, i64 176
  %7217 = load <16 x i8>, ptr %7215, align 1, !tbaa !12
  %7218 = load <16 x i8>, ptr %7216, align 1, !tbaa !12
  %7219 = getelementptr inbounds nuw i8, ptr %7165, i64 160
  %7220 = getelementptr inbounds nuw i8, ptr %7165, i64 176
  store <16 x i8> %7217, ptr %7219, align 1, !tbaa !12
  store <16 x i8> %7218, ptr %7220, align 1, !tbaa !12
  %7221 = icmp eq i64 %7176, 192
  br i1 %7221, label %7229, label %7222, !llvm.loop !59

7222:                                             ; preds = %7214
  %7223 = getelementptr inbounds nuw i8, ptr %7164, i64 192
  %7224 = getelementptr inbounds nuw i8, ptr %7164, i64 208
  %7225 = load <16 x i8>, ptr %7223, align 1, !tbaa !12
  %7226 = load <16 x i8>, ptr %7224, align 1, !tbaa !12
  %7227 = getelementptr inbounds nuw i8, ptr %7165, i64 192
  %7228 = getelementptr inbounds nuw i8, ptr %7165, i64 208
  store <16 x i8> %7225, ptr %7227, align 1, !tbaa !12
  store <16 x i8> %7226, ptr %7228, align 1, !tbaa !12
  br label %7229

7229:                                             ; preds = %7222, %7214, %7206, %7198, %7190, %7182, %7175
  %7230 = icmp eq i64 %7176, %7166
  br i1 %7230, label %7247, label %7231

7231:                                             ; preds = %7229
  %7232 = and i64 %7166, 24
  %7233 = icmp eq i64 %7232, 0
  br i1 %7233, label %7171, label %7234

7234:                                             ; preds = %7231, %7173
  %7235 = phi i64 [ %7176, %7231 ], [ 0, %7173 ]
  %7236 = and i64 %7166, 248
  br label %7237

7237:                                             ; preds = %7237, %7234
  %7238 = phi i64 [ %7235, %7234 ], [ %7242, %7237 ]
  %7239 = getelementptr inbounds nuw i8, ptr %7164, i64 %7238
  %7240 = load <8 x i8>, ptr %7239, align 1, !tbaa !12
  %7241 = getelementptr inbounds nuw i8, ptr %7165, i64 %7238
  store <8 x i8> %7240, ptr %7241, align 1, !tbaa !12
  %7242 = add nuw i64 %7238, 8
  %7243 = icmp eq i64 %7242, %7236
  br i1 %7243, label %7244, label %7237, !llvm.loop !60

7244:                                             ; preds = %7237
  %7245 = icmp eq i64 %7236, %7166
  br i1 %7245, label %7247, label %7171

7246:                                             ; preds = %7263
  br label %7247

7247:                                             ; preds = %7246, %7244, %7229, %7098
  %7248 = trunc nuw i64 %7159 to i32
  %7249 = icmp sgt i32 %7248, 0
  br i1 %7249, label %7250, label %2370

7250:                                             ; preds = %7247
  %7251 = mul i64 %7159, %7101
  %7252 = add i64 %7161, %7251
  %7253 = inttoptr i64 %7252 to ptr
  br label %7254

7254:                                             ; preds = %7254, %7250
  %7255 = phi i32 [ %7261, %7254 ], [ 0, %7250 ]
  %7256 = phi ptr [ %7259, %7254 ], [ %7253, %7250 ]
  %7257 = phi i64 [ %7260, %7254 ], [ %6978, %7250 ]
  %7258 = trunc i64 %7257 to i8
  store i8 %7258, ptr %7256, align 1, !tbaa !12
  %7259 = getelementptr inbounds nuw i8, ptr %7256, i64 1
  %7260 = lshr i64 %7257, 8
  %7261 = add nuw nsw i32 %7255, 1
  %7262 = icmp eq i32 %7261, %7248
  br i1 %7262, label %2367, label %7254, !llvm.loop !34

7263:                                             ; preds = %7263, %7171
  %7264 = phi i64 [ %7268, %7263 ], [ %7172, %7171 ]
  %7265 = getelementptr inbounds nuw i8, ptr %7164, i64 %7264
  %7266 = load i8, ptr %7265, align 1, !tbaa !12
  %7267 = getelementptr inbounds nuw i8, ptr %7165, i64 %7264
  store i8 %7266, ptr %7267, align 1, !tbaa !12
  %7268 = add nuw nsw i64 %7264, 1
  %7269 = icmp eq i64 %7268, %7166
  br i1 %7269, label %7246, label %7263, !llvm.loop !61

7270:                                             ; preds = %239
  %7271 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7272 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7273 = shl i32 %7272, 13
  %7274 = xor i32 %7273, %7272
  %7275 = lshr i32 %7274, 17
  %7276 = xor i32 %7275, %7274
  %7277 = shl i32 %7276, 5
  %7278 = xor i32 %7277, %7276
  %7279 = add nsw i32 %7271, 2
  %7280 = shl i32 %7278, 13
  %7281 = xor i32 %7280, %7278
  %7282 = lshr i32 %7281, 17
  %7283 = xor i32 %7282, %7281
  %7284 = shl i32 %7283, 5
  %7285 = xor i32 %7284, %7283
  store i32 %7285, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7286 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %7287 = icmp sgt i32 %7286, 0
  %7288 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %7289 = inttoptr i64 %7288 to ptr
  br i1 %7287, label %7290, label %7322

7290:                                             ; preds = %7270
  %7291 = sext i32 %7279 to i64
  %7292 = zext nneg i32 %7286 to i64
  br label %7293

7293:                                             ; preds = %7293, %7290
  %7294 = phi i64 [ %7291, %7290 ], [ %7298, %7293 ]
  %7295 = phi i64 [ 0, %7290 ], [ %7316, %7293 ]
  %7296 = phi i64 [ 0, %7290 ], [ %7315, %7293 ]
  %7297 = phi i32 [ %7285, %7290 ], [ %7307, %7293 ]
  %7298 = add nsw i64 %7294, 1
  %7299 = trunc nsw i64 %7298 to i32
  store i32 %7299, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7300 = getelementptr inbounds i8, ptr %7289, i64 %7294
  %7301 = load i8, ptr %7300, align 1, !tbaa !12
  %7302 = shl i32 %7297, 13
  %7303 = xor i32 %7302, %7297
  %7304 = lshr i32 %7303, 17
  %7305 = xor i32 %7304, %7303
  %7306 = shl i32 %7305, 5
  %7307 = xor i32 %7306, %7305
  store i32 %7307, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7308 = trunc i32 %7307 to i8
  %7309 = xor i8 %7301, %7308
  %7310 = icmp samesign ult i64 %7295, 8
  %7311 = zext i8 %7309 to i64
  %7312 = shl nuw nsw i64 %7295, 3
  %7313 = shl nuw i64 %7311, %7312
  %7314 = select i1 %7310, i64 %7313, i64 0
  %7315 = or i64 %7314, %7296
  %7316 = add nuw nsw i64 %7295, 1
  %7317 = icmp eq i64 %7316, %7292
  br i1 %7317, label %7318, label %7293, !llvm.loop !33

7318:                                             ; preds = %7293
  %7319 = phi i32 [ %7299, %7293 ]
  %7320 = phi i32 [ %7307, %7293 ]
  %7321 = phi i64 [ %7315, %7293 ]
  br label %7322

7322:                                             ; preds = %7318, %7270
  %7323 = phi i32 [ %7285, %7270 ], [ %7320, %7318 ]
  %7324 = phi i32 [ %7279, %7270 ], [ %7319, %7318 ]
  %7325 = phi i64 [ 0, %7270 ], [ %7321, %7318 ]
  %7326 = add nsw i32 %7324, 1
  store i32 %7326, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7327 = sext i32 %7324 to i64
  %7328 = getelementptr inbounds i8, ptr %7289, i64 %7327
  %7329 = load i8, ptr %7328, align 1, !tbaa !12
  %7330 = shl i32 %7323, 13
  %7331 = xor i32 %7330, %7323
  %7332 = lshr i32 %7331, 17
  %7333 = xor i32 %7332, %7331
  %7334 = shl i32 %7333, 5
  %7335 = xor i32 %7334, %7333
  store i32 %7335, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7336 = trunc i32 %7335 to i8
  %7337 = xor i8 %7329, %7336
  %7338 = add nsw i32 %7324, 2
  store i32 %7338, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7339 = sext i32 %7326 to i64
  %7340 = getelementptr inbounds i8, ptr %7289, i64 %7339
  %7341 = load i8, ptr %7340, align 1, !tbaa !12
  %7342 = shl i32 %7335, 13
  %7343 = xor i32 %7342, %7335
  %7344 = lshr i32 %7343, 17
  %7345 = xor i32 %7344, %7343
  %7346 = shl i32 %7345, 5
  %7347 = xor i32 %7346, %7345
  store i32 %7347, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7348 = trunc i32 %7347 to i8
  %7349 = icmp eq i8 %7341, %7348
  br i1 %7349, label %7350, label %7409

7350:                                             ; preds = %7322
  br i1 %7287, label %7351, label %7383

7351:                                             ; preds = %7350
  %7352 = sext i32 %7338 to i64
  %7353 = zext nneg i32 %7286 to i64
  br label %7354

7354:                                             ; preds = %7354, %7351
  %7355 = phi i64 [ %7352, %7351 ], [ %7359, %7354 ]
  %7356 = phi i64 [ 0, %7351 ], [ %7377, %7354 ]
  %7357 = phi i64 [ 0, %7351 ], [ %7376, %7354 ]
  %7358 = phi i32 [ %7347, %7351 ], [ %7368, %7354 ]
  %7359 = add nsw i64 %7355, 1
  %7360 = trunc nsw i64 %7359 to i32
  store i32 %7360, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7361 = getelementptr inbounds i8, ptr %7289, i64 %7355
  %7362 = load i8, ptr %7361, align 1, !tbaa !12
  %7363 = shl i32 %7358, 13
  %7364 = xor i32 %7363, %7358
  %7365 = lshr i32 %7364, 17
  %7366 = xor i32 %7365, %7364
  %7367 = shl i32 %7366, 5
  %7368 = xor i32 %7367, %7366
  store i32 %7368, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7369 = trunc i32 %7368 to i8
  %7370 = xor i8 %7362, %7369
  %7371 = icmp samesign ult i64 %7356, 8
  %7372 = zext i8 %7370 to i64
  %7373 = shl nuw nsw i64 %7356, 3
  %7374 = shl nuw i64 %7372, %7373
  %7375 = select i1 %7371, i64 %7374, i64 0
  %7376 = or i64 %7375, %7357
  %7377 = add nuw nsw i64 %7356, 1
  %7378 = icmp eq i64 %7377, %7353
  br i1 %7378, label %7379, label %7354, !llvm.loop !33

7379:                                             ; preds = %7354
  %7380 = phi i32 [ %7360, %7354 ]
  %7381 = phi i32 [ %7368, %7354 ]
  %7382 = phi i64 [ %7376, %7354 ]
  br label %7383

7383:                                             ; preds = %7379, %7350
  %7384 = phi i32 [ %7347, %7350 ], [ %7381, %7379 ]
  %7385 = phi i32 [ %7338, %7350 ], [ %7380, %7379 ]
  %7386 = phi i64 [ 0, %7350 ], [ %7382, %7379 ]
  %7387 = icmp eq i8 %7329, %7336
  br i1 %7387, label %7445, label %7388

7388:                                             ; preds = %7383
  %7389 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %7390 = inttoptr i64 %7389 to ptr
  %7391 = zext i8 %7337 to i64
  br label %7392

7392:                                             ; preds = %7405, %7388
  %7393 = phi i64 [ 0, %7388 ], [ %7407, %7405 ]
  %7394 = phi i64 [ %7386, %7388 ], [ %7396, %7405 ]
  %7395 = phi i64 [ 0, %7388 ], [ %7406, %7405 ]
  %7396 = add i64 %7394, 1
  %7397 = icmp samesign ult i64 %7393, 8
  br i1 %7397, label %7398, label %7405

7398:                                             ; preds = %7392
  %7399 = getelementptr inbounds nuw i8, ptr %7390, i64 %7394
  %7400 = load i8, ptr %7399, align 1, !tbaa !12
  %7401 = zext i8 %7400 to i64
  %7402 = shl nuw nsw i64 %7393, 3
  %7403 = shl nuw i64 %7401, %7402
  %7404 = or i64 %7403, %7395
  br label %7405

7405:                                             ; preds = %7398, %7392
  %7406 = phi i64 [ %7404, %7398 ], [ %7395, %7392 ]
  %7407 = add nuw nsw i64 %7393, 1
  %7408 = icmp eq i64 %7407, %7391
  br i1 %7408, label %7439, label %7392, !llvm.loop !35

7409:                                             ; preds = %7322
  %7410 = icmp eq i8 %7329, %7336
  br i1 %7410, label %7445, label %7411

7411:                                             ; preds = %7409
  %7412 = sext i32 %7338 to i64
  %7413 = zext i8 %7337 to i64
  br label %7414

7414:                                             ; preds = %7414, %7411
  %7415 = phi i64 [ %7412, %7411 ], [ %7419, %7414 ]
  %7416 = phi i64 [ 0, %7411 ], [ %7437, %7414 ]
  %7417 = phi i64 [ 0, %7411 ], [ %7436, %7414 ]
  %7418 = phi i32 [ %7347, %7411 ], [ %7428, %7414 ]
  %7419 = add nsw i64 %7415, 1
  %7420 = trunc nsw i64 %7419 to i32
  store i32 %7420, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7421 = getelementptr inbounds i8, ptr %7289, i64 %7415
  %7422 = load i8, ptr %7421, align 1, !tbaa !12
  %7423 = shl i32 %7418, 13
  %7424 = xor i32 %7423, %7418
  %7425 = lshr i32 %7424, 17
  %7426 = xor i32 %7425, %7424
  %7427 = shl i32 %7426, 5
  %7428 = xor i32 %7427, %7426
  store i32 %7428, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7429 = trunc i32 %7428 to i8
  %7430 = xor i8 %7422, %7429
  %7431 = icmp samesign ult i64 %7416, 8
  %7432 = zext i8 %7430 to i64
  %7433 = shl nuw nsw i64 %7416, 3
  %7434 = shl nuw i64 %7432, %7433
  %7435 = select i1 %7431, i64 %7434, i64 0
  %7436 = or i64 %7435, %7417
  %7437 = add nuw nsw i64 %7416, 1
  %7438 = icmp eq i64 %7437, %7413
  br i1 %7438, label %7441, label %7414, !llvm.loop !33

7439:                                             ; preds = %7405
  %7440 = phi i64 [ %7406, %7405 ]
  br label %7445

7441:                                             ; preds = %7414
  %7442 = phi i32 [ %7420, %7414 ]
  %7443 = phi i32 [ %7428, %7414 ]
  %7444 = phi i64 [ %7436, %7414 ]
  br label %7445

7445:                                             ; preds = %7441, %7439, %7409, %7383
  %7446 = phi i32 [ %7384, %7383 ], [ %7347, %7409 ], [ %7384, %7439 ], [ %7443, %7441 ]
  %7447 = phi i32 [ %7385, %7383 ], [ %7338, %7409 ], [ %7385, %7439 ], [ %7442, %7441 ]
  %7448 = phi i64 [ 0, %7383 ], [ 0, %7409 ], [ %7440, %7439 ], [ %7444, %7441 ]
  %7449 = add nsw i32 %7447, 1
  store i32 %7449, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7450 = sext i32 %7447 to i64
  %7451 = getelementptr inbounds i8, ptr %7289, i64 %7450
  %7452 = load i8, ptr %7451, align 1, !tbaa !12
  %7453 = shl i32 %7446, 13
  %7454 = xor i32 %7453, %7446
  %7455 = lshr i32 %7454, 17
  %7456 = xor i32 %7455, %7454
  %7457 = shl i32 %7456, 5
  %7458 = xor i32 %7457, %7456
  store i32 %7458, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7459 = trunc i32 %7458 to i8
  %7460 = xor i8 %7452, %7459
  %7461 = add nsw i32 %7447, 2
  store i32 %7461, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7462 = sext i32 %7449 to i64
  %7463 = getelementptr inbounds i8, ptr %7289, i64 %7462
  %7464 = load i8, ptr %7463, align 1, !tbaa !12
  %7465 = shl i32 %7458, 13
  %7466 = xor i32 %7465, %7458
  %7467 = lshr i32 %7466, 17
  %7468 = xor i32 %7467, %7466
  %7469 = shl i32 %7468, 5
  %7470 = xor i32 %7469, %7468
  store i32 %7470, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7471 = trunc i32 %7470 to i8
  %7472 = icmp eq i8 %7464, %7471
  br i1 %7472, label %7473, label %7532

7473:                                             ; preds = %7445
  br i1 %7287, label %7474, label %7506

7474:                                             ; preds = %7473
  %7475 = sext i32 %7461 to i64
  %7476 = zext nneg i32 %7286 to i64
  br label %7477

7477:                                             ; preds = %7477, %7474
  %7478 = phi i64 [ %7475, %7474 ], [ %7482, %7477 ]
  %7479 = phi i64 [ 0, %7474 ], [ %7500, %7477 ]
  %7480 = phi i64 [ 0, %7474 ], [ %7499, %7477 ]
  %7481 = phi i32 [ %7470, %7474 ], [ %7491, %7477 ]
  %7482 = add nsw i64 %7478, 1
  %7483 = trunc nsw i64 %7482 to i32
  store i32 %7483, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7484 = getelementptr inbounds i8, ptr %7289, i64 %7478
  %7485 = load i8, ptr %7484, align 1, !tbaa !12
  %7486 = shl i32 %7481, 13
  %7487 = xor i32 %7486, %7481
  %7488 = lshr i32 %7487, 17
  %7489 = xor i32 %7488, %7487
  %7490 = shl i32 %7489, 5
  %7491 = xor i32 %7490, %7489
  store i32 %7491, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7492 = trunc i32 %7491 to i8
  %7493 = xor i8 %7485, %7492
  %7494 = icmp samesign ult i64 %7479, 8
  %7495 = zext i8 %7493 to i64
  %7496 = shl nuw nsw i64 %7479, 3
  %7497 = shl nuw i64 %7495, %7496
  %7498 = select i1 %7494, i64 %7497, i64 0
  %7499 = or i64 %7498, %7480
  %7500 = add nuw nsw i64 %7479, 1
  %7501 = icmp eq i64 %7500, %7476
  br i1 %7501, label %7502, label %7477, !llvm.loop !33

7502:                                             ; preds = %7477
  %7503 = phi i32 [ %7483, %7477 ]
  %7504 = phi i32 [ %7491, %7477 ]
  %7505 = phi i64 [ %7499, %7477 ]
  br label %7506

7506:                                             ; preds = %7502, %7473
  %7507 = phi i32 [ %7470, %7473 ], [ %7504, %7502 ]
  %7508 = phi i32 [ %7461, %7473 ], [ %7503, %7502 ]
  %7509 = phi i64 [ 0, %7473 ], [ %7505, %7502 ]
  %7510 = icmp eq i8 %7452, %7459
  br i1 %7510, label %7568, label %7511

7511:                                             ; preds = %7506
  %7512 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %7513 = inttoptr i64 %7512 to ptr
  %7514 = zext i8 %7460 to i64
  br label %7515

7515:                                             ; preds = %7528, %7511
  %7516 = phi i64 [ 0, %7511 ], [ %7530, %7528 ]
  %7517 = phi i64 [ %7509, %7511 ], [ %7519, %7528 ]
  %7518 = phi i64 [ 0, %7511 ], [ %7529, %7528 ]
  %7519 = add i64 %7517, 1
  %7520 = icmp samesign ult i64 %7516, 8
  br i1 %7520, label %7521, label %7528

7521:                                             ; preds = %7515
  %7522 = getelementptr inbounds nuw i8, ptr %7513, i64 %7517
  %7523 = load i8, ptr %7522, align 1, !tbaa !12
  %7524 = zext i8 %7523 to i64
  %7525 = shl nuw nsw i64 %7516, 3
  %7526 = shl nuw i64 %7524, %7525
  %7527 = or i64 %7526, %7518
  br label %7528

7528:                                             ; preds = %7521, %7515
  %7529 = phi i64 [ %7527, %7521 ], [ %7518, %7515 ]
  %7530 = add nuw nsw i64 %7516, 1
  %7531 = icmp eq i64 %7530, %7514
  br i1 %7531, label %7562, label %7515, !llvm.loop !35

7532:                                             ; preds = %7445
  %7533 = icmp eq i8 %7452, %7459
  br i1 %7533, label %7568, label %7534

7534:                                             ; preds = %7532
  %7535 = sext i32 %7461 to i64
  %7536 = zext i8 %7460 to i64
  br label %7537

7537:                                             ; preds = %7537, %7534
  %7538 = phi i64 [ %7535, %7534 ], [ %7542, %7537 ]
  %7539 = phi i64 [ 0, %7534 ], [ %7560, %7537 ]
  %7540 = phi i64 [ 0, %7534 ], [ %7559, %7537 ]
  %7541 = phi i32 [ %7470, %7534 ], [ %7551, %7537 ]
  %7542 = add nsw i64 %7538, 1
  %7543 = trunc nsw i64 %7542 to i32
  store i32 %7543, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7544 = getelementptr inbounds i8, ptr %7289, i64 %7538
  %7545 = load i8, ptr %7544, align 1, !tbaa !12
  %7546 = shl i32 %7541, 13
  %7547 = xor i32 %7546, %7541
  %7548 = lshr i32 %7547, 17
  %7549 = xor i32 %7548, %7547
  %7550 = shl i32 %7549, 5
  %7551 = xor i32 %7550, %7549
  store i32 %7551, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7552 = trunc i32 %7551 to i8
  %7553 = xor i8 %7545, %7552
  %7554 = icmp samesign ult i64 %7539, 8
  %7555 = zext i8 %7553 to i64
  %7556 = shl nuw nsw i64 %7539, 3
  %7557 = shl nuw i64 %7555, %7556
  %7558 = select i1 %7554, i64 %7557, i64 0
  %7559 = or i64 %7558, %7540
  %7560 = add nuw nsw i64 %7539, 1
  %7561 = icmp eq i64 %7560, %7536
  br i1 %7561, label %7564, label %7537, !llvm.loop !33

7562:                                             ; preds = %7528
  %7563 = phi i64 [ %7529, %7528 ]
  br label %7568

7564:                                             ; preds = %7537
  %7565 = phi i32 [ %7543, %7537 ]
  %7566 = phi i32 [ %7551, %7537 ]
  %7567 = phi i64 [ %7559, %7537 ]
  br label %7568

7568:                                             ; preds = %7564, %7562, %7532, %7506
  %7569 = phi i32 [ %7507, %7506 ], [ %7470, %7532 ], [ %7507, %7562 ], [ %7566, %7564 ]
  %7570 = phi i32 [ %7508, %7506 ], [ %7461, %7532 ], [ %7508, %7562 ], [ %7565, %7564 ]
  %7571 = phi i64 [ 0, %7506 ], [ 0, %7532 ], [ %7563, %7562 ], [ %7567, %7564 ]
  %7572 = sext i32 %7570 to i64
  %7573 = add nsw i64 %7572, 1
  %7574 = trunc nsw i64 %7573 to i32
  store i32 %7574, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7575 = getelementptr inbounds i8, ptr %7289, i64 %7572
  %7576 = load i8, ptr %7575, align 1, !tbaa !12
  %7577 = shl i32 %7569, 13
  %7578 = xor i32 %7577, %7569
  %7579 = lshr i32 %7578, 17
  %7580 = xor i32 %7579, %7578
  %7581 = shl i32 %7580, 5
  %7582 = xor i32 %7581, %7580
  store i32 %7582, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7583 = add nsw i64 %7572, 2
  %7584 = trunc nsw i64 %7583 to i32
  store i32 %7584, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7585 = getelementptr inbounds i8, ptr %7289, i64 %7573
  %7586 = load i8, ptr %7585, align 1, !tbaa !12
  %7587 = shl i32 %7582, 13
  %7588 = xor i32 %7587, %7582
  %7589 = lshr i32 %7588, 17
  %7590 = xor i32 %7589, %7588
  %7591 = shl i32 %7590, 5
  %7592 = xor i32 %7591, %7590
  store i32 %7592, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7593 = add nsw i64 %7572, 3
  %7594 = trunc nsw i64 %7593 to i32
  store i32 %7594, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7595 = getelementptr inbounds i8, ptr %7289, i64 %7583
  %7596 = load i8, ptr %7595, align 1, !tbaa !12
  %7597 = shl i32 %7592, 13
  %7598 = xor i32 %7597, %7592
  %7599 = lshr i32 %7598, 17
  %7600 = xor i32 %7599, %7598
  %7601 = shl i32 %7600, 5
  %7602 = xor i32 %7601, %7600
  store i32 %7602, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7603 = add i32 %7570, 4
  store i32 %7603, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7604 = getelementptr inbounds i8, ptr %7289, i64 %7593
  %7605 = load i8, ptr %7604, align 1, !tbaa !12
  %7606 = shl i32 %7602, 13
  %7607 = xor i32 %7606, %7602
  %7608 = lshr i32 %7607, 17
  %7609 = xor i32 %7608, %7607
  %7610 = shl i32 %7609, 5
  %7611 = xor i32 %7610, %7609
  store i32 %7611, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7612 = sext i32 %7603 to i64
  %7613 = add nsw i64 %7612, 1
  %7614 = trunc nsw i64 %7613 to i32
  store i32 %7614, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7615 = getelementptr inbounds i8, ptr %7289, i64 %7612
  %7616 = load i8, ptr %7615, align 1, !tbaa !12
  %7617 = shl i32 %7611, 13
  %7618 = xor i32 %7617, %7611
  %7619 = lshr i32 %7618, 17
  %7620 = xor i32 %7619, %7618
  %7621 = shl i32 %7620, 5
  %7622 = xor i32 %7621, %7620
  store i32 %7622, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7623 = add nsw i64 %7612, 2
  %7624 = trunc nsw i64 %7623 to i32
  store i32 %7624, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7625 = getelementptr inbounds i8, ptr %7289, i64 %7613
  %7626 = load i8, ptr %7625, align 1, !tbaa !12
  %7627 = shl i32 %7622, 13
  %7628 = xor i32 %7627, %7622
  %7629 = lshr i32 %7628, 17
  %7630 = xor i32 %7629, %7628
  %7631 = shl i32 %7630, 5
  %7632 = xor i32 %7631, %7630
  store i32 %7632, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7633 = add nsw i64 %7612, 3
  %7634 = trunc nsw i64 %7633 to i32
  store i32 %7634, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7635 = getelementptr inbounds i8, ptr %7289, i64 %7623
  %7636 = load i8, ptr %7635, align 1, !tbaa !12
  %7637 = shl i32 %7632, 13
  %7638 = xor i32 %7637, %7632
  %7639 = lshr i32 %7638, 17
  %7640 = xor i32 %7639, %7638
  %7641 = shl i32 %7640, 5
  %7642 = xor i32 %7641, %7640
  store i32 %7642, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7643 = add i32 %7570, 8
  store i32 %7643, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7644 = getelementptr inbounds i8, ptr %7289, i64 %7633
  %7645 = load i8, ptr %7644, align 1, !tbaa !12
  %7646 = shl i32 %7642, 13
  %7647 = xor i32 %7646, %7642
  %7648 = lshr i32 %7647, 17
  %7649 = xor i32 %7648, %7647
  %7650 = shl i32 %7649, 5
  %7651 = xor i32 %7650, %7649
  %7652 = shl i32 %7651, 13
  %7653 = xor i32 %7652, %7651
  %7654 = lshr i32 %7653, 17
  %7655 = xor i32 %7654, %7653
  %7656 = shl i32 %7655, 5
  %7657 = xor i32 %7656, %7655
  %7658 = shl i32 %7657, 13
  %7659 = xor i32 %7658, %7657
  %7660 = lshr i32 %7659, 17
  %7661 = xor i32 %7660, %7659
  %7662 = shl i32 %7661, 5
  %7663 = xor i32 %7662, %7661
  %7664 = shl i32 %7663, 13
  %7665 = xor i32 %7664, %7663
  %7666 = lshr i32 %7665, 17
  %7667 = xor i32 %7666, %7665
  %7668 = shl i32 %7667, 5
  %7669 = xor i32 %7668, %7667
  %7670 = shl i32 %7669, 13
  %7671 = xor i32 %7670, %7669
  %7672 = lshr i32 %7671, 17
  %7673 = xor i32 %7672, %7671
  %7674 = shl i32 %7673, 5
  %7675 = xor i32 %7674, %7673
  %7676 = add i32 %7570, 12
  store i32 %7675, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7677 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %7678 = inttoptr i64 %7677 to ptr
  %7679 = sext i32 %7676 to i64
  %7680 = add nsw i64 %7679, 1
  %7681 = trunc nsw i64 %7680 to i32
  store i32 %7681, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7682 = getelementptr inbounds i8, ptr %7678, i64 %7679
  %7683 = load i8, ptr %7682, align 1, !tbaa !12
  %7684 = shl i32 %7675, 13
  %7685 = xor i32 %7684, %7675
  %7686 = lshr i32 %7685, 17
  %7687 = xor i32 %7686, %7685
  %7688 = shl i32 %7687, 5
  %7689 = xor i32 %7688, %7687
  store i32 %7689, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7690 = trunc i32 %7689 to i8
  %7691 = xor i8 %7683, %7690
  %7692 = zext i8 %7691 to i32
  %7693 = add nsw i64 %7679, 2
  %7694 = trunc nsw i64 %7693 to i32
  store i32 %7694, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7695 = getelementptr inbounds i8, ptr %7678, i64 %7680
  %7696 = load i8, ptr %7695, align 1, !tbaa !12
  %7697 = shl i32 %7689, 13
  %7698 = xor i32 %7697, %7689
  %7699 = lshr i32 %7698, 17
  %7700 = xor i32 %7699, %7698
  %7701 = shl i32 %7700, 5
  %7702 = xor i32 %7701, %7700
  store i32 %7702, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7703 = trunc i32 %7702 to i8
  %7704 = xor i8 %7696, %7703
  %7705 = zext i8 %7704 to i32
  %7706 = shl nuw nsw i32 %7705, 8
  %7707 = or disjoint i32 %7706, %7692
  %7708 = add nsw i64 %7679, 3
  %7709 = trunc nsw i64 %7708 to i32
  store i32 %7709, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7710 = getelementptr inbounds i8, ptr %7678, i64 %7693
  %7711 = load i8, ptr %7710, align 1, !tbaa !12
  %7712 = shl i32 %7702, 13
  %7713 = xor i32 %7712, %7702
  %7714 = lshr i32 %7713, 17
  %7715 = xor i32 %7714, %7713
  %7716 = shl i32 %7715, 5
  %7717 = xor i32 %7716, %7715
  store i32 %7717, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7718 = trunc i32 %7717 to i8
  %7719 = xor i8 %7711, %7718
  %7720 = zext i8 %7719 to i32
  %7721 = shl nuw nsw i32 %7720, 16
  %7722 = or disjoint i32 %7721, %7707
  %7723 = add i32 %7570, 16
  store i32 %7723, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7724 = getelementptr inbounds i8, ptr %7678, i64 %7708
  %7725 = load i8, ptr %7724, align 1, !tbaa !12
  %7726 = shl i32 %7717, 13
  %7727 = xor i32 %7726, %7717
  %7728 = lshr i32 %7727, 17
  %7729 = xor i32 %7728, %7727
  %7730 = shl i32 %7729, 5
  %7731 = xor i32 %7730, %7729
  store i32 %7731, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7732 = trunc i32 %7731 to i8
  %7733 = xor i8 %7725, %7732
  %7734 = zext i8 %7733 to i32
  %7735 = shl nuw i32 %7734, 24
  %7736 = or disjoint i32 %7735, %7722
  %7737 = trunc i32 %7622 to i8
  %7738 = xor i8 %7616, %7737
  %7739 = zext i8 %7738 to i64
  %7740 = trunc i32 %7632 to i8
  %7741 = xor i8 %7626, %7740
  %7742 = zext i8 %7741 to i64
  %7743 = shl nuw nsw i64 %7742, 8
  %7744 = or disjoint i64 %7743, %7739
  %7745 = trunc i32 %7642 to i8
  %7746 = xor i8 %7636, %7745
  %7747 = zext i8 %7746 to i64
  %7748 = shl nuw nsw i64 %7747, 16
  %7749 = or disjoint i64 %7748, %7744
  %7750 = trunc i32 %7651 to i8
  %7751 = xor i8 %7645, %7750
  %7752 = zext i8 %7751 to i64
  %7753 = shl nuw nsw i64 %7752, 24
  %7754 = or disjoint i64 %7753, %7749
  %7755 = trunc i32 %7582 to i8
  %7756 = xor i8 %7576, %7755
  %7757 = zext i8 %7756 to i64
  %7758 = trunc i32 %7592 to i8
  %7759 = xor i8 %7586, %7758
  %7760 = zext i8 %7759 to i64
  %7761 = shl nuw nsw i64 %7760, 8
  %7762 = or disjoint i64 %7761, %7757
  %7763 = trunc i32 %7602 to i8
  %7764 = xor i8 %7596, %7763
  %7765 = zext i8 %7764 to i64
  %7766 = shl nuw nsw i64 %7765, 16
  %7767 = or disjoint i64 %7766, %7762
  %7768 = trunc i32 %7611 to i8
  %7769 = xor i8 %7605, %7768
  %7770 = zext i8 %7769 to i64
  %7771 = shl nuw nsw i64 %7770, 24
  %7772 = or disjoint i64 %7771, %7767
  %7773 = trunc nuw i64 %7772 to i32
  %7774 = icmp eq i32 %7736, 0
  br i1 %7774, label %2370, label %7775

7775:                                             ; preds = %7568
  %7776 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %7777 = add i64 %7776, %7325
  %7778 = icmp eq i64 %7772, 0
  %7779 = inttoptr i64 %7777 to ptr
  %7780 = add nsw i64 %7771, -1
  %7781 = add nsw i64 %7780, %7766
  %7782 = add nsw i64 %7781, %7761
  %7783 = add nsw i64 %7782, %7757
  %7784 = zext i8 %7769 to i32
  %7785 = shl nuw i32 %7784, 24
  %7786 = zext i8 %7764 to i32
  %7787 = shl nuw nsw i32 %7786, 16
  %7788 = or disjoint i32 %7785, %7787
  %7789 = zext i8 %7759 to i32
  %7790 = shl nuw nsw i32 %7789, 8
  %7791 = or disjoint i32 %7788, %7790
  %7792 = zext i8 %7756 to i32
  %7793 = or disjoint i32 %7791, %7792
  %7794 = zext i8 %7769 to i32
  %7795 = shl nuw i32 %7794, 24
  %7796 = zext i8 %7764 to i32
  %7797 = shl nuw nsw i32 %7796, 16
  %7798 = or disjoint i32 %7795, %7797
  %7799 = zext i8 %7759 to i32
  %7800 = shl nuw nsw i32 %7799, 8
  %7801 = or disjoint i32 %7798, %7800
  %7802 = zext i8 %7756 to i32
  %7803 = or disjoint i32 %7801, %7802
  %7804 = or disjoint i64 %7771, %7766
  %7805 = or disjoint i64 %7804, %7761
  %7806 = or disjoint i64 %7805, %7757
  %7807 = add nsw i64 %7771, -1
  %7808 = add nsw i64 %7807, %7766
  %7809 = add nsw i64 %7808, %7761
  %7810 = add nsw i64 %7809, %7757
  %7811 = zext i8 %7769 to i32
  %7812 = shl nuw i32 %7811, 24
  %7813 = zext i8 %7764 to i32
  %7814 = shl nuw nsw i32 %7813, 16
  %7815 = or disjoint i32 %7812, %7814
  %7816 = zext i8 %7759 to i32
  %7817 = shl nuw nsw i32 %7816, 8
  %7818 = or disjoint i32 %7815, %7817
  %7819 = zext i8 %7756 to i32
  %7820 = or disjoint i32 %7818, %7819
  %7821 = or disjoint i64 %7771, %7766
  %7822 = or disjoint i64 %7821, %7761
  %7823 = or disjoint i64 %7822, %7757
  %7824 = icmp samesign ult i64 %7823, 8
  %7825 = trunc i64 %7810 to i32
  %7826 = icmp ugt i64 %7810, 4294967295
  %7827 = icmp samesign ult i64 %7823, 32
  %7828 = and i64 %7757, 31
  %7829 = sub nuw nsw i64 %7823, %7828
  %7830 = icmp eq i64 %7828, 0
  %7831 = icmp samesign ult i64 %7828, 8
  %7832 = and i64 %7757, 7
  %7833 = sub nsw i64 %7823, %7832
  %7834 = icmp eq i64 %7832, 0
  %7835 = icmp samesign ult i64 %7806, 8
  %7836 = trunc i64 %7783 to i32
  %7837 = icmp ugt i64 %7783, 4294967295
  %7838 = icmp samesign ult i64 %7806, 32
  %7839 = and i64 %7757, 31
  %7840 = sub nuw nsw i64 %7806, %7839
  %7841 = icmp eq i64 %7839, 0
  %7842 = icmp samesign ult i64 %7839, 8
  %7843 = and i64 %7757, 7
  %7844 = sub nsw i64 %7806, %7843
  %7845 = icmp eq i64 %7843, 0
  br label %7846

7846:                                             ; preds = %8019, %7775
  %7847 = phi i32 [ 0, %7775 ], [ %8020, %8019 ]
  %7848 = mul i32 %7820, %7847
  %7849 = mul i32 %7803, %7847
  %7850 = zext i32 %7849 to i64
  %7851 = mul i32 %7793, %7847
  %7852 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %7853 = inttoptr i64 %7852 to ptr
  %7854 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7855 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7856 = sext i32 %7854 to i64
  %7857 = add nsw i64 %7856, 1
  %7858 = trunc nsw i64 %7857 to i32
  store i32 %7858, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7859 = getelementptr inbounds i8, ptr %7853, i64 %7856
  %7860 = load i8, ptr %7859, align 1, !tbaa !12
  %7861 = shl i32 %7855, 13
  %7862 = xor i32 %7861, %7855
  %7863 = lshr i32 %7862, 17
  %7864 = xor i32 %7863, %7862
  %7865 = shl i32 %7864, 5
  %7866 = xor i32 %7865, %7864
  store i32 %7866, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7867 = add nsw i64 %7856, 2
  %7868 = trunc nsw i64 %7867 to i32
  store i32 %7868, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7869 = getelementptr inbounds i8, ptr %7853, i64 %7857
  %7870 = load i8, ptr %7869, align 1, !tbaa !12
  %7871 = shl i32 %7866, 13
  %7872 = xor i32 %7871, %7866
  %7873 = lshr i32 %7872, 17
  %7874 = xor i32 %7873, %7872
  %7875 = shl i32 %7874, 5
  %7876 = xor i32 %7875, %7874
  store i32 %7876, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7877 = add nsw i64 %7856, 3
  %7878 = trunc nsw i64 %7877 to i32
  store i32 %7878, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7879 = getelementptr inbounds i8, ptr %7853, i64 %7867
  %7880 = load i8, ptr %7879, align 1, !tbaa !12
  %7881 = shl i32 %7876, 13
  %7882 = xor i32 %7881, %7876
  %7883 = lshr i32 %7882, 17
  %7884 = xor i32 %7883, %7882
  %7885 = shl i32 %7884, 5
  %7886 = xor i32 %7885, %7884
  store i32 %7886, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7887 = add i32 %7854, 4
  store i32 %7887, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %7888 = getelementptr inbounds i8, ptr %7853, i64 %7877
  %7889 = load i8, ptr %7888, align 1, !tbaa !12
  %7890 = shl i32 %7886, 13
  %7891 = xor i32 %7890, %7886
  %7892 = lshr i32 %7891, 17
  %7893 = xor i32 %7892, %7891
  %7894 = shl i32 %7893, 5
  %7895 = xor i32 %7894, %7893
  store i32 %7895, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %7896 = trunc i32 %7895 to i8
  %7897 = xor i8 %7889, %7896
  %7898 = icmp sgt i8 %7897, -1
  br i1 %7898, label %7932, label %7899

7899:                                             ; preds = %7846
  br i1 %7778, label %8019, label %7900

7900:                                             ; preds = %7899
  %7901 = mul i32 %7847, %7773
  br i1 %7824, label %7902, label %7904

7902:                                             ; preds = %7931, %7920, %7904, %7900
  %7903 = phi i64 [ 0, %7900 ], [ 0, %7904 ], [ %7829, %7920 ], [ %7833, %7931 ]
  br label %8009

7904:                                             ; preds = %7900
  %7905 = xor i32 %7848, -1
  %7906 = icmp ult i32 %7905, %7825
  %7907 = or i1 %7906, %7826
  br i1 %7907, label %7902, label %7908

7908:                                             ; preds = %7904
  br i1 %7827, label %7921, label %7909

7909:                                             ; preds = %7908
  br label %7910

7910:                                             ; preds = %7909, %7910
  %7911 = phi i64 [ %7917, %7910 ], [ 0, %7909 ]
  %7912 = trunc i64 %7911 to i32
  %7913 = add i32 %7901, %7912
  %7914 = zext i32 %7913 to i64
  %7915 = getelementptr inbounds nuw i8, ptr %7779, i64 %7914
  %7916 = getelementptr inbounds nuw i8, ptr %7915, i64 16
  store <16 x i8> zeroinitializer, ptr %7915, align 1, !tbaa !12
  store <16 x i8> zeroinitializer, ptr %7916, align 1, !tbaa !12
  %7917 = add nuw i64 %7911, 32
  %7918 = icmp eq i64 %7917, %7829
  br i1 %7918, label %7919, label %7910, !llvm.loop !62

7919:                                             ; preds = %7910
  br i1 %7830, label %8019, label %7920

7920:                                             ; preds = %7919
  br i1 %7831, label %7902, label %7921

7921:                                             ; preds = %7920, %7908
  %7922 = phi i64 [ %7829, %7920 ], [ 0, %7908 ]
  br label %7923

7923:                                             ; preds = %7923, %7921
  %7924 = phi i64 [ %7922, %7921 ], [ %7929, %7923 ]
  %7925 = trunc i64 %7924 to i32
  %7926 = add i32 %7901, %7925
  %7927 = zext i32 %7926 to i64
  %7928 = getelementptr inbounds nuw i8, ptr %7779, i64 %7927
  store <8 x i8> zeroinitializer, ptr %7928, align 1, !tbaa !12
  %7929 = add nuw i64 %7924, 8
  %7930 = icmp eq i64 %7929, %7833
  br i1 %7930, label %7931, label %7923, !llvm.loop !63

7931:                                             ; preds = %7923
  br i1 %7834, label %8019, label %7902

7932:                                             ; preds = %7846
  br i1 %7778, label %8019, label %7933

7933:                                             ; preds = %7932
  %7934 = zext nneg i8 %7897 to i64
  %7935 = shl nuw nsw i64 %7934, 24
  %7936 = trunc i32 %7886 to i8
  %7937 = xor i8 %7880, %7936
  %7938 = zext i8 %7937 to i64
  %7939 = shl nuw nsw i64 %7938, 16
  %7940 = trunc i32 %7876 to i8
  %7941 = xor i8 %7870, %7940
  %7942 = zext i8 %7941 to i64
  %7943 = shl nuw nsw i64 %7942, 8
  %7944 = trunc i32 %7866 to i8
  %7945 = xor i8 %7860, %7944
  %7946 = zext i8 %7945 to i64
  %7947 = or disjoint i64 %7943, %7946
  %7948 = or disjoint i64 %7939, %7947
  %7949 = or disjoint i64 %7935, %7948
  %7950 = icmp samesign ult i64 %7949, %7754
  %7951 = select i1 %7950, i64 0, i64 %7754
  %7952 = sub nuw nsw i64 %7949, %7951
  %7953 = mul nuw nsw i64 %7952, %7772
  %7954 = and i64 %7953, 4294967295
  %7955 = select i1 %7950, i64 %7448, i64 %7571
  %7956 = add i64 %7954, %7955
  %7957 = inttoptr i64 %7956 to ptr
  %7958 = mul i32 %7847, %7773
  br i1 %7835, label %7997, label %7959

7959:                                             ; preds = %7933
  %7960 = xor i32 %7851, -1
  %7961 = icmp ult i32 %7960, %7836
  %7962 = or i1 %7961, %7837
  br i1 %7962, label %7997, label %7963

7963:                                             ; preds = %7959
  %7964 = add i64 %7777, %7850
  %7965 = sub i64 %7964, %7956
  %7966 = icmp ult i64 %7965, 32
  br i1 %7966, label %7997, label %7967

7967:                                             ; preds = %7963
  br i1 %7838, label %7984, label %7968

7968:                                             ; preds = %7967
  br label %7969

7969:                                             ; preds = %7968, %7969
  %7970 = phi i64 [ %7980, %7969 ], [ 0, %7968 ]
  %7971 = trunc i64 %7970 to i32
  %7972 = getelementptr inbounds nuw i8, ptr %7957, i64 %7970
  %7973 = getelementptr inbounds nuw i8, ptr %7972, i64 16
  %7974 = load <16 x i8>, ptr %7972, align 1, !tbaa !12
  %7975 = load <16 x i8>, ptr %7973, align 1, !tbaa !12
  %7976 = add i32 %7958, %7971
  %7977 = zext i32 %7976 to i64
  %7978 = getelementptr inbounds nuw i8, ptr %7779, i64 %7977
  %7979 = getelementptr inbounds nuw i8, ptr %7978, i64 16
  store <16 x i8> %7974, ptr %7978, align 1, !tbaa !12
  store <16 x i8> %7975, ptr %7979, align 1, !tbaa !12
  %7980 = add nuw i64 %7970, 32
  %7981 = icmp eq i64 %7980, %7840
  br i1 %7981, label %7982, label %7969, !llvm.loop !64

7982:                                             ; preds = %7969
  br i1 %7841, label %8019, label %7983

7983:                                             ; preds = %7982
  br i1 %7842, label %7997, label %7984

7984:                                             ; preds = %7983, %7967
  %7985 = phi i64 [ %7840, %7983 ], [ 0, %7967 ]
  br label %7986

7986:                                             ; preds = %7986, %7984
  %7987 = phi i64 [ %7985, %7984 ], [ %7994, %7986 ]
  %7988 = trunc i64 %7987 to i32
  %7989 = getelementptr inbounds nuw i8, ptr %7957, i64 %7987
  %7990 = load <8 x i8>, ptr %7989, align 1, !tbaa !12
  %7991 = add i32 %7958, %7988
  %7992 = zext i32 %7991 to i64
  %7993 = getelementptr inbounds nuw i8, ptr %7779, i64 %7992
  store <8 x i8> %7990, ptr %7993, align 1, !tbaa !12
  %7994 = add nuw i64 %7987, 8
  %7995 = icmp eq i64 %7994, %7844
  br i1 %7995, label %7996, label %7986, !llvm.loop !65

7996:                                             ; preds = %7986
  br i1 %7845, label %8019, label %7997

7997:                                             ; preds = %7996, %7983, %7963, %7959, %7933
  %7998 = phi i64 [ 0, %7933 ], [ 0, %7959 ], [ 0, %7963 ], [ %7840, %7983 ], [ %7844, %7996 ]
  br label %7999

7999:                                             ; preds = %7999, %7997
  %8000 = phi i64 [ %8007, %7999 ], [ %7998, %7997 ]
  %8001 = getelementptr inbounds nuw i8, ptr %7957, i64 %8000
  %8002 = load i8, ptr %8001, align 1, !tbaa !12
  %8003 = trunc nuw i64 %8000 to i32
  %8004 = add i32 %7958, %8003
  %8005 = zext i32 %8004 to i64
  %8006 = getelementptr inbounds nuw i8, ptr %7779, i64 %8005
  store i8 %8002, ptr %8006, align 1, !tbaa !12
  %8007 = add nuw nsw i64 %8000, 1
  %8008 = icmp eq i64 %8007, %7772
  br i1 %8008, label %8017, label %7999, !llvm.loop !66

8009:                                             ; preds = %8009, %7902
  %8010 = phi i64 [ %8015, %8009 ], [ %7903, %7902 ]
  %8011 = trunc nuw i64 %8010 to i32
  %8012 = add i32 %7901, %8011
  %8013 = zext i32 %8012 to i64
  %8014 = getelementptr inbounds nuw i8, ptr %7779, i64 %8013
  store i8 0, ptr %8014, align 1, !tbaa !12
  %8015 = add nuw nsw i64 %8010, 1
  %8016 = icmp eq i64 %8015, %7772
  br i1 %8016, label %8018, label %8009, !llvm.loop !67

8017:                                             ; preds = %7999
  br label %8019

8018:                                             ; preds = %8009
  br label %8019

8019:                                             ; preds = %8018, %8017, %7996, %7982, %7932, %7931, %7919, %7899
  %8020 = add nuw i32 %7847, 1
  %8021 = icmp eq i32 %8020, %7736
  br i1 %8021, label %2368, label %7846, !llvm.loop !68

8022:                                             ; preds = %239
  %8023 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %8024 = inttoptr i64 %8023 to ptr
  %8025 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8026 = add nsw i32 %8025, 1
  store i32 %8026, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8027 = sext i32 %8025 to i64
  %8028 = getelementptr inbounds i8, ptr %8024, i64 %8027
  %8029 = load i8, ptr %8028, align 1, !tbaa !12
  %8030 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8031 = shl i32 %8030, 13
  %8032 = xor i32 %8031, %8030
  %8033 = lshr i32 %8032, 17
  %8034 = xor i32 %8033, %8032
  %8035 = shl i32 %8034, 5
  %8036 = xor i32 %8035, %8034
  %8037 = trunc i32 %8036 to i8
  %8038 = xor i8 %8029, %8037
  %8039 = add nsw i32 %8025, 2
  %8040 = shl i32 %8036, 13
  %8041 = xor i32 %8040, %8036
  %8042 = lshr i32 %8041, 17
  %8043 = xor i32 %8042, %8041
  %8044 = shl i32 %8043, 5
  %8045 = xor i32 %8044, %8043
  store i32 %8045, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8046 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %8047 = icmp sgt i32 %8046, 0
  br i1 %8047, label %8048, label %8080

8048:                                             ; preds = %8022
  %8049 = sext i32 %8039 to i64
  %8050 = zext nneg i32 %8046 to i64
  br label %8051

8051:                                             ; preds = %8051, %8048
  %8052 = phi i64 [ %8049, %8048 ], [ %8056, %8051 ]
  %8053 = phi i64 [ 0, %8048 ], [ %8074, %8051 ]
  %8054 = phi i64 [ 0, %8048 ], [ %8073, %8051 ]
  %8055 = phi i32 [ %8045, %8048 ], [ %8065, %8051 ]
  %8056 = add nsw i64 %8052, 1
  %8057 = trunc nsw i64 %8056 to i32
  store i32 %8057, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8058 = getelementptr inbounds i8, ptr %8024, i64 %8052
  %8059 = load i8, ptr %8058, align 1, !tbaa !12
  %8060 = shl i32 %8055, 13
  %8061 = xor i32 %8060, %8055
  %8062 = lshr i32 %8061, 17
  %8063 = xor i32 %8062, %8061
  %8064 = shl i32 %8063, 5
  %8065 = xor i32 %8064, %8063
  store i32 %8065, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8066 = trunc i32 %8065 to i8
  %8067 = xor i8 %8059, %8066
  %8068 = icmp samesign ult i64 %8053, 8
  %8069 = zext i8 %8067 to i64
  %8070 = shl nuw nsw i64 %8053, 3
  %8071 = shl nuw i64 %8069, %8070
  %8072 = select i1 %8068, i64 %8071, i64 0
  %8073 = or i64 %8072, %8054
  %8074 = add nuw nsw i64 %8053, 1
  %8075 = icmp eq i64 %8074, %8050
  br i1 %8075, label %8076, label %8051, !llvm.loop !33

8076:                                             ; preds = %8051
  %8077 = phi i32 [ %8057, %8051 ]
  %8078 = phi i32 [ %8065, %8051 ]
  %8079 = phi i64 [ %8073, %8051 ]
  br label %8080

8080:                                             ; preds = %8076, %8022
  %8081 = phi i32 [ %8045, %8022 ], [ %8078, %8076 ]
  %8082 = phi i32 [ %8039, %8022 ], [ %8077, %8076 ]
  %8083 = phi i64 [ 0, %8022 ], [ %8079, %8076 ]
  %8084 = add nsw i32 %8082, 1
  store i32 %8084, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8085 = sext i32 %8082 to i64
  %8086 = getelementptr inbounds i8, ptr %8024, i64 %8085
  %8087 = load i8, ptr %8086, align 1, !tbaa !12
  %8088 = shl i32 %8081, 13
  %8089 = xor i32 %8088, %8081
  %8090 = lshr i32 %8089, 17
  %8091 = xor i32 %8090, %8089
  %8092 = shl i32 %8091, 5
  %8093 = xor i32 %8092, %8091
  store i32 %8093, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8094 = trunc i32 %8093 to i8
  %8095 = xor i8 %8087, %8094
  %8096 = add nsw i32 %8082, 2
  store i32 %8096, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8097 = sext i32 %8084 to i64
  %8098 = getelementptr inbounds i8, ptr %8024, i64 %8097
  %8099 = load i8, ptr %8098, align 1, !tbaa !12
  %8100 = shl i32 %8093, 13
  %8101 = xor i32 %8100, %8093
  %8102 = lshr i32 %8101, 17
  %8103 = xor i32 %8102, %8101
  %8104 = shl i32 %8103, 5
  %8105 = xor i32 %8104, %8103
  store i32 %8105, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8106 = trunc i32 %8105 to i8
  %8107 = icmp eq i8 %8099, %8106
  br i1 %8107, label %8108, label %8163

8108:                                             ; preds = %8080
  br i1 %8047, label %8109, label %8139

8109:                                             ; preds = %8108
  %8110 = sext i32 %8096 to i64
  %8111 = zext nneg i32 %8046 to i64
  br label %8112

8112:                                             ; preds = %8112, %8109
  %8113 = phi i64 [ %8110, %8109 ], [ %8117, %8112 ]
  %8114 = phi i64 [ 0, %8109 ], [ %8135, %8112 ]
  %8115 = phi i64 [ 0, %8109 ], [ %8134, %8112 ]
  %8116 = phi i32 [ %8105, %8109 ], [ %8126, %8112 ]
  %8117 = add nsw i64 %8113, 1
  %8118 = trunc nsw i64 %8117 to i32
  store i32 %8118, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8119 = getelementptr inbounds i8, ptr %8024, i64 %8113
  %8120 = load i8, ptr %8119, align 1, !tbaa !12
  %8121 = shl i32 %8116, 13
  %8122 = xor i32 %8121, %8116
  %8123 = lshr i32 %8122, 17
  %8124 = xor i32 %8123, %8122
  %8125 = shl i32 %8124, 5
  %8126 = xor i32 %8125, %8124
  store i32 %8126, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8127 = trunc i32 %8126 to i8
  %8128 = xor i8 %8120, %8127
  %8129 = icmp samesign ult i64 %8114, 8
  %8130 = zext i8 %8128 to i64
  %8131 = shl nuw nsw i64 %8114, 3
  %8132 = shl nuw i64 %8130, %8131
  %8133 = select i1 %8129, i64 %8132, i64 0
  %8134 = or i64 %8133, %8115
  %8135 = add nuw nsw i64 %8114, 1
  %8136 = icmp eq i64 %8135, %8111
  br i1 %8136, label %8137, label %8112, !llvm.loop !33

8137:                                             ; preds = %8112
  %8138 = phi i64 [ %8134, %8112 ]
  br label %8139

8139:                                             ; preds = %8137, %8108
  %8140 = phi i64 [ 0, %8108 ], [ %8138, %8137 ]
  %8141 = icmp eq i8 %8087, %8094
  br i1 %8141, label %8197, label %8142

8142:                                             ; preds = %8139
  %8143 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %8144 = inttoptr i64 %8143 to ptr
  %8145 = zext i8 %8095 to i64
  br label %8146

8146:                                             ; preds = %8159, %8142
  %8147 = phi i64 [ 0, %8142 ], [ %8161, %8159 ]
  %8148 = phi i64 [ %8140, %8142 ], [ %8150, %8159 ]
  %8149 = phi i64 [ 0, %8142 ], [ %8160, %8159 ]
  %8150 = add i64 %8148, 1
  %8151 = icmp samesign ult i64 %8147, 8
  br i1 %8151, label %8152, label %8159

8152:                                             ; preds = %8146
  %8153 = getelementptr inbounds nuw i8, ptr %8144, i64 %8148
  %8154 = load i8, ptr %8153, align 1, !tbaa !12
  %8155 = zext i8 %8154 to i64
  %8156 = shl nuw nsw i64 %8147, 3
  %8157 = shl nuw i64 %8155, %8156
  %8158 = or i64 %8157, %8149
  br label %8159

8159:                                             ; preds = %8152, %8146
  %8160 = phi i64 [ %8158, %8152 ], [ %8149, %8146 ]
  %8161 = add nuw nsw i64 %8147, 1
  %8162 = icmp eq i64 %8161, %8145
  br i1 %8162, label %8193, label %8146, !llvm.loop !35

8163:                                             ; preds = %8080
  %8164 = icmp eq i8 %8087, %8094
  br i1 %8164, label %8197, label %8165

8165:                                             ; preds = %8163
  %8166 = sext i32 %8096 to i64
  %8167 = zext i8 %8095 to i64
  br label %8168

8168:                                             ; preds = %8168, %8165
  %8169 = phi i64 [ %8166, %8165 ], [ %8173, %8168 ]
  %8170 = phi i64 [ 0, %8165 ], [ %8191, %8168 ]
  %8171 = phi i64 [ 0, %8165 ], [ %8190, %8168 ]
  %8172 = phi i32 [ %8105, %8165 ], [ %8182, %8168 ]
  %8173 = add nsw i64 %8169, 1
  %8174 = trunc nsw i64 %8173 to i32
  store i32 %8174, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8175 = getelementptr inbounds i8, ptr %8024, i64 %8169
  %8176 = load i8, ptr %8175, align 1, !tbaa !12
  %8177 = shl i32 %8172, 13
  %8178 = xor i32 %8177, %8172
  %8179 = lshr i32 %8178, 17
  %8180 = xor i32 %8179, %8178
  %8181 = shl i32 %8180, 5
  %8182 = xor i32 %8181, %8180
  store i32 %8182, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8183 = trunc i32 %8182 to i8
  %8184 = xor i8 %8176, %8183
  %8185 = icmp samesign ult i64 %8170, 8
  %8186 = zext i8 %8184 to i64
  %8187 = shl nuw nsw i64 %8170, 3
  %8188 = shl nuw i64 %8186, %8187
  %8189 = select i1 %8185, i64 %8188, i64 0
  %8190 = or i64 %8189, %8171
  %8191 = add nuw nsw i64 %8170, 1
  %8192 = icmp eq i64 %8191, %8167
  br i1 %8192, label %8195, label %8168, !llvm.loop !33

8193:                                             ; preds = %8159
  %8194 = phi i64 [ %8160, %8159 ]
  br label %8197

8195:                                             ; preds = %8168
  %8196 = phi i64 [ %8190, %8168 ]
  br label %8197

8197:                                             ; preds = %8195, %8193, %8163, %8139
  %8198 = phi i64 [ 0, %8139 ], [ 0, %8163 ], [ %8194, %8193 ], [ %8196, %8195 ]
  %8199 = icmp eq i8 %8029, %8037
  br i1 %8199, label %2370, label %8200

8200:                                             ; preds = %8197
  %8201 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %8202 = add i64 %8201, %8083
  %8203 = inttoptr i64 %8202 to ptr
  %8204 = zext i8 %8038 to i32
  br label %8205

8205:                                             ; preds = %8205, %8200
  %8206 = phi i32 [ %8212, %8205 ], [ 0, %8200 ]
  %8207 = phi ptr [ %8210, %8205 ], [ %8203, %8200 ]
  %8208 = phi i64 [ %8211, %8205 ], [ %8198, %8200 ]
  %8209 = trunc i64 %8208 to i8
  store i8 %8209, ptr %8207, align 1, !tbaa !12
  %8210 = getelementptr inbounds nuw i8, ptr %8207, i64 1
  %8211 = lshr i64 %8208, 8
  %8212 = add nuw nsw i32 %8206, 1
  %8213 = icmp eq i32 %8212, %8204
  br i1 %8213, label %2369, label %8205, !llvm.loop !34

8214:                                             ; preds = %240
  %8215 = sext i32 %289 to i64
  %8216 = zext nneg i32 %303 to i64
  br label %8217

8217:                                             ; preds = %8217, %8214
  %8218 = phi i64 [ %8215, %8214 ], [ %8222, %8217 ]
  %8219 = phi i64 [ 0, %8214 ], [ %8240, %8217 ]
  %8220 = phi i64 [ 0, %8214 ], [ %8239, %8217 ]
  %8221 = phi i32 [ %297, %8214 ], [ %8231, %8217 ]
  %8222 = add nsw i64 %8218, 1
  %8223 = trunc nsw i64 %8222 to i32
  store i32 %8223, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8224 = getelementptr inbounds i8, ptr %242, i64 %8218
  %8225 = load i8, ptr %8224, align 1, !tbaa !12
  %8226 = shl i32 %8221, 13
  %8227 = xor i32 %8226, %8221
  %8228 = lshr i32 %8227, 17
  %8229 = xor i32 %8228, %8227
  %8230 = shl i32 %8229, 5
  %8231 = xor i32 %8230, %8229
  store i32 %8231, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8232 = trunc i32 %8231 to i8
  %8233 = xor i8 %8225, %8232
  %8234 = icmp samesign ult i64 %8219, 8
  %8235 = zext i8 %8233 to i64
  %8236 = shl nuw nsw i64 %8219, 3
  %8237 = shl nuw i64 %8235, %8236
  %8238 = select i1 %8234, i64 %8237, i64 0
  %8239 = or i64 %8238, %8220
  %8240 = add nuw nsw i64 %8219, 1
  %8241 = icmp eq i64 %8240, %8216
  br i1 %8241, label %8242, label %8217, !llvm.loop !33

8242:                                             ; preds = %8217
  %8243 = phi i32 [ %8223, %8217 ]
  %8244 = phi i64 [ %8239, %8217 ]
  br label %8245

8245:                                             ; preds = %8242, %240
  %8246 = phi i32 [ %289, %240 ], [ %8243, %8242 ]
  %8247 = phi i64 [ 0, %240 ], [ %8244, %8242 ]
  %8248 = load i8, ptr @exception_thrown__Z8vm_entryv, align 4, !tbaa !12
  %8249 = icmp eq i8 %8248, 0
  br i1 %8249, label %8250, label %8262

8250:                                             ; preds = %8245
  %8251 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %8252 = icmp eq i8 %8251, 0
  br i1 %8252, label %8258, label %8253

8253:                                             ; preds = %8250
  %8254 = load ptr, ptr @exception_ptr__Z8vm_entryv, align 8, !tbaa !54
  %8255 = load i32, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %8256 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.14__Z8vm_entryv.43, i32 noundef %302, i64 noundef %8247, ptr noundef %8254, i32 noundef %8255, ptr noundef null, i32 noundef 0, i32 noundef %8246)
  %8257 = tail call i32 @fflush(ptr noundef null)
  br label %8258

8258:                                             ; preds = %8253, %8250
  %8259 = icmp eq i64 %8247, 0
  br i1 %8259, label %2370, label %8260

8260:                                             ; preds = %8258
  %8261 = trunc i64 %8247 to i32
  store i32 %8261, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  br label %2370

8262:                                             ; preds = %8245
  %8263 = load i32, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %8264 = sext i32 %8263 to i64
  %8265 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %8266 = icmp eq i8 %8265, 0
  br i1 %8266, label %8272, label %8267

8267:                                             ; preds = %8262
  %8268 = zext i8 %8248 to i32
  %8269 = load ptr, ptr @exception_ptr__Z8vm_entryv, align 8, !tbaa !54
  %8270 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.15__Z8vm_entryv.44, i32 noundef %302, i64 noundef %8247, i32 noundef %8268, ptr noundef %8269, i32 noundef %8263, ptr noundef null, i32 noundef 0, i32 noundef %8246)
  %8271 = tail call i32 @fflush(ptr noundef null)
  br label %8272

8272:                                             ; preds = %8267, %8262
  %8273 = icmp eq i32 %302, 0
  br i1 %8273, label %8426, label %8274

8274:                                             ; preds = %8272
  %8275 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4
  %8276 = load i32, ptr @ip__Z8vm_entryv, align 4
  %8277 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %8278 = icmp sgt i32 %8277, 0
  %8279 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8
  %8280 = inttoptr i64 %8279 to ptr
  %8281 = load ptr, ptr @exception_ptr__Z8vm_entryv, align 8, !tbaa !54
  %8282 = inttoptr i64 %8264 to ptr
  %8283 = icmp eq i32 %8263, 0
  %8284 = icmp ne ptr %8281, null
  %8285 = and i1 %8283, %8284
  %8286 = getelementptr inbounds i8, ptr %8281, i64 -128
  %8287 = zext nneg i32 %8277 to i64
  %8288 = zext nneg i32 %8277 to i64
  br label %8289

8289:                                             ; preds = %8420, %8274
  %8290 = phi i32 [ 0, %8274 ], [ %8421, %8420 ]
  %8291 = phi i32 [ %8276, %8274 ], [ %8352, %8420 ]
  %8292 = phi i32 [ %8275, %8274 ], [ %8353, %8420 ]
  br i1 %8278, label %8293, label %8404

8293:                                             ; preds = %8289
  %8294 = sext i32 %8291 to i64
  br label %8295

8295:                                             ; preds = %8295, %8293
  %8296 = phi i64 [ %8294, %8293 ], [ %8300, %8295 ]
  %8297 = phi i64 [ 0, %8293 ], [ %8318, %8295 ]
  %8298 = phi i64 [ 0, %8293 ], [ %8317, %8295 ]
  %8299 = phi i32 [ %8292, %8293 ], [ %8309, %8295 ]
  %8300 = add nsw i64 %8296, 1
  %8301 = trunc nsw i64 %8300 to i32
  store i32 %8301, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8302 = getelementptr inbounds i8, ptr %8280, i64 %8296
  %8303 = load i8, ptr %8302, align 1, !tbaa !12
  %8304 = shl i32 %8299, 13
  %8305 = xor i32 %8304, %8299
  %8306 = lshr i32 %8305, 17
  %8307 = xor i32 %8306, %8305
  %8308 = shl i32 %8307, 5
  %8309 = xor i32 %8308, %8307
  store i32 %8309, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8310 = trunc i32 %8309 to i8
  %8311 = xor i8 %8303, %8310
  %8312 = icmp samesign ult i64 %8297, 8
  %8313 = zext i8 %8311 to i64
  %8314 = shl nuw nsw i64 %8297, 3
  %8315 = shl nuw i64 %8313, %8314
  %8316 = select i1 %8312, i64 %8315, i64 0
  %8317 = or i64 %8316, %8298
  %8318 = add nuw nsw i64 %8297, 1
  %8319 = icmp eq i64 %8318, %8287
  br i1 %8319, label %8320, label %8295, !llvm.loop !33

8320:                                             ; preds = %8295
  %8321 = phi i64 [ %8300, %8295 ]
  %8322 = phi i32 [ %8309, %8295 ]
  %8323 = phi i64 [ %8317, %8295 ]
  %8324 = shl i64 %8321, 32
  %8325 = ashr exact i64 %8324, 32
  br label %8326

8326:                                             ; preds = %8326, %8320
  %8327 = phi i64 [ %8325, %8320 ], [ %8331, %8326 ]
  %8328 = phi i64 [ 0, %8320 ], [ %8349, %8326 ]
  %8329 = phi i64 [ 0, %8320 ], [ %8348, %8326 ]
  %8330 = phi i32 [ %8322, %8320 ], [ %8340, %8326 ]
  %8331 = add nsw i64 %8327, 1
  %8332 = trunc nsw i64 %8331 to i32
  store i32 %8332, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8333 = getelementptr inbounds i8, ptr %8280, i64 %8327
  %8334 = load i8, ptr %8333, align 1, !tbaa !12
  %8335 = shl i32 %8330, 13
  %8336 = xor i32 %8335, %8330
  %8337 = lshr i32 %8336, 17
  %8338 = xor i32 %8337, %8336
  %8339 = shl i32 %8338, 5
  %8340 = xor i32 %8339, %8338
  store i32 %8340, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8341 = trunc i32 %8340 to i8
  %8342 = xor i8 %8334, %8341
  %8343 = icmp samesign ult i64 %8328, 8
  %8344 = zext i8 %8342 to i64
  %8345 = shl nuw nsw i64 %8328, 3
  %8346 = shl nuw i64 %8344, %8345
  %8347 = select i1 %8343, i64 %8346, i64 0
  %8348 = or i64 %8347, %8329
  %8349 = add nuw nsw i64 %8328, 1
  %8350 = icmp eq i64 %8349, %8288
  br i1 %8350, label %8351, label %8326, !llvm.loop !33

8351:                                             ; preds = %8326
  %8352 = phi i32 [ %8332, %8326 ]
  %8353 = phi i32 [ %8340, %8326 ]
  %8354 = phi i64 [ %8348, %8326 ]
  %8355 = inttoptr i64 %8354 to ptr
  %8356 = icmp eq i64 %8354, 0
  br i1 %8356, label %8404, label %8357

8357:                                             ; preds = %8351
  br i1 %8285, label %8358, label %8363

8358:                                             ; preds = %8357
  %8359 = load ptr, ptr %8286, align 8, !tbaa !69
  %8360 = icmp eq ptr %8359, null
  br i1 %8360, label %8361, label %8366

8361:                                             ; preds = %8358
  %8362 = load ptr, ptr %8281, align 8, !tbaa !54
  br label %8363

8363:                                             ; preds = %8361, %8357
  %8364 = phi ptr [ %8282, %8357 ], [ %8362, %8361 ]
  %8365 = icmp eq ptr %8364, null
  br i1 %8365, label %8420, label %8366

8366:                                             ; preds = %8363, %8358
  %8367 = phi ptr [ %8364, %8363 ], [ %8359, %8358 ]
  %8368 = icmp eq ptr %8367, %8355
  br i1 %8368, label %8404, label %8369

8369:                                             ; preds = %8366
  %8370 = getelementptr inbounds nuw i8, ptr %8367, i64 8
  %8371 = load ptr, ptr %8370, align 8, !tbaa !72
  %8372 = icmp eq ptr %8371, null
  br i1 %8372, label %8380, label %8373

8373:                                             ; preds = %8369
  %8374 = getelementptr inbounds nuw i8, ptr %8355, i64 8
  %8375 = load ptr, ptr %8374, align 8, !tbaa !72
  %8376 = icmp eq ptr %8375, null
  br i1 %8376, label %8380, label %8377

8377:                                             ; preds = %8373
  %8378 = tail call i32 @strcmp(ptr noundef nonnull dereferenceable(1) %8371, ptr noundef nonnull dereferenceable(1) %8375) #13
  %8379 = icmp eq i32 %8378, 0
  br i1 %8379, label %8404, label %8380

8380:                                             ; preds = %8377, %8373, %8369
  %8381 = getelementptr inbounds nuw i8, ptr %8355, i64 8
  br label %8382

8382:                                             ; preds = %8395, %8380
  %8383 = phi ptr [ %8367, %8380 ], [ %8397, %8395 ]
  %8384 = icmp eq ptr %8383, %8355
  br i1 %8384, label %8401, label %8385

8385:                                             ; preds = %8382
  %8386 = getelementptr inbounds nuw i8, ptr %8383, i64 8
  %8387 = load ptr, ptr %8386, align 8, !tbaa !72
  %8388 = icmp eq ptr %8387, null
  br i1 %8388, label %8395, label %8389

8389:                                             ; preds = %8385
  %8390 = load ptr, ptr %8381, align 8, !tbaa !72
  %8391 = icmp eq ptr %8390, null
  br i1 %8391, label %8395, label %8392

8392:                                             ; preds = %8389
  %8393 = tail call i32 @strcmp(ptr noundef nonnull dereferenceable(1) %8387, ptr noundef nonnull dereferenceable(1) %8390) #13
  %8394 = icmp eq i32 %8393, 0
  br i1 %8394, label %8401, label %8395

8395:                                             ; preds = %8392, %8389, %8385
  %8396 = getelementptr inbounds nuw i8, ptr %8383, i64 16
  %8397 = load ptr, ptr %8396, align 8, !tbaa !74
  %8398 = icmp eq ptr %8397, null
  %8399 = icmp eq ptr %8397, %8383
  %8400 = or i1 %8398, %8399
  br i1 %8400, label %8419, label %8382

8401:                                             ; preds = %8382, %8392
  %8402 = phi i32 [ %8352, %8382 ], [ %8352, %8392 ]
  %8403 = phi i64 [ %8323, %8382 ], [ %8323, %8392 ]
  br label %8407

8404:                                             ; preds = %8289, %8351, %8366, %8377
  %8405 = phi i32 [ %8352, %8351 ], [ %8352, %8366 ], [ %8352, %8377 ], [ %8291, %8289 ]
  %8406 = phi i64 [ %8323, %8351 ], [ %8323, %8366 ], [ %8323, %8377 ], [ 0, %8289 ]
  br label %8407

8407:                                             ; preds = %8404, %8401
  %8408 = phi i32 [ %8402, %8401 ], [ %8405, %8404 ]
  %8409 = phi i64 [ %8403, %8401 ], [ %8406, %8404 ]
  %8410 = icmp eq ptr %8281, null
  br i1 %8410, label %8412, label %8411

8411:                                             ; preds = %8407
  store ptr %8281, ptr @exception_ptr__Z8vm_entryv, align 8, !tbaa !54
  br label %8412

8412:                                             ; preds = %8411, %8407
  %8413 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %8414 = icmp eq i8 %8413, 0
  br i1 %8414, label %8423, label %8415

8415:                                             ; preds = %8412
  %8416 = load i32, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %8417 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.16__Z8vm_entryv.45, i64 noundef %8409, ptr noundef %8281, ptr noundef %8281, i32 noundef %8416, i32 noundef %8408)
  %8418 = tail call i32 @fflush(ptr noundef null)
  br label %8423

8419:                                             ; preds = %8395
  br label %8420

8420:                                             ; preds = %8419, %8363
  %8421 = add nuw i32 %8290, 1
  %8422 = icmp eq i32 %8421, %302
  br i1 %8422, label %8425, label %8289, !llvm.loop !76

8423:                                             ; preds = %8415, %8412
  %8424 = trunc i64 %8409 to i32
  store i32 %8424, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  br label %2370

8425:                                             ; preds = %8420
  br label %8426

8426:                                             ; preds = %8425, %8272
  %8427 = icmp eq i64 %8247, 0
  br i1 %8427, label %2370, label %8428

8428:                                             ; preds = %8426
  %8429 = trunc i64 %8247 to i32
  store i32 %8429, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  br label %2370

8430:                                             ; preds = %239
  %8431 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %8432 = inttoptr i64 %8431 to ptr
  %8433 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8434 = add nsw i32 %8433, 1
  store i32 %8434, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8435 = sext i32 %8433 to i64
  %8436 = getelementptr inbounds i8, ptr %8432, i64 %8435
  %8437 = load i8, ptr %8436, align 1, !tbaa !12
  %8438 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8439 = shl i32 %8438, 13
  %8440 = xor i32 %8439, %8438
  %8441 = lshr i32 %8440, 17
  %8442 = xor i32 %8441, %8440
  %8443 = shl i32 %8442, 5
  %8444 = xor i32 %8443, %8442
  store i32 %8444, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8445 = trunc i32 %8444 to i8
  %8446 = xor i8 %8437, %8445
  %8447 = add nsw i32 %8433, 2
  store i32 %8447, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8448 = sext i32 %8434 to i64
  %8449 = getelementptr inbounds i8, ptr %8432, i64 %8448
  %8450 = load i8, ptr %8449, align 1, !tbaa !12
  %8451 = shl i32 %8444, 13
  %8452 = xor i32 %8451, %8444
  %8453 = lshr i32 %8452, 17
  %8454 = xor i32 %8453, %8452
  %8455 = shl i32 %8454, 5
  %8456 = xor i32 %8455, %8454
  store i32 %8456, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8457 = trunc i32 %8456 to i8
  %8458 = xor i8 %8450, %8457
  %8459 = icmp eq i8 %8450, %8457
  br i1 %8459, label %8460, label %8517

8460:                                             ; preds = %8430
  %8461 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %8462 = icmp sgt i32 %8461, 0
  br i1 %8462, label %8463, label %8493

8463:                                             ; preds = %8460
  %8464 = sext i32 %8447 to i64
  %8465 = zext nneg i32 %8461 to i64
  br label %8466

8466:                                             ; preds = %8466, %8463
  %8467 = phi i64 [ %8464, %8463 ], [ %8471, %8466 ]
  %8468 = phi i64 [ 0, %8463 ], [ %8489, %8466 ]
  %8469 = phi i64 [ 0, %8463 ], [ %8488, %8466 ]
  %8470 = phi i32 [ %8456, %8463 ], [ %8480, %8466 ]
  %8471 = add nsw i64 %8467, 1
  %8472 = trunc nsw i64 %8471 to i32
  store i32 %8472, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8473 = getelementptr inbounds i8, ptr %8432, i64 %8467
  %8474 = load i8, ptr %8473, align 1, !tbaa !12
  %8475 = shl i32 %8470, 13
  %8476 = xor i32 %8475, %8470
  %8477 = lshr i32 %8476, 17
  %8478 = xor i32 %8477, %8476
  %8479 = shl i32 %8478, 5
  %8480 = xor i32 %8479, %8478
  store i32 %8480, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8481 = trunc i32 %8480 to i8
  %8482 = xor i8 %8474, %8481
  %8483 = icmp samesign ult i64 %8468, 8
  %8484 = zext i8 %8482 to i64
  %8485 = shl nuw nsw i64 %8468, 3
  %8486 = shl nuw i64 %8484, %8485
  %8487 = select i1 %8483, i64 %8486, i64 0
  %8488 = or i64 %8487, %8469
  %8489 = add nuw nsw i64 %8468, 1
  %8490 = icmp eq i64 %8489, %8465
  br i1 %8490, label %8491, label %8466, !llvm.loop !33

8491:                                             ; preds = %8466
  %8492 = phi i64 [ %8488, %8466 ]
  br label %8493

8493:                                             ; preds = %8491, %8460
  %8494 = phi i64 [ 0, %8460 ], [ %8492, %8491 ]
  %8495 = icmp eq i8 %8437, %8445
  br i1 %8495, label %8551, label %8496

8496:                                             ; preds = %8493
  %8497 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %8498 = inttoptr i64 %8497 to ptr
  %8499 = zext i8 %8446 to i64
  br label %8500

8500:                                             ; preds = %8513, %8496
  %8501 = phi i64 [ 0, %8496 ], [ %8515, %8513 ]
  %8502 = phi i64 [ %8494, %8496 ], [ %8504, %8513 ]
  %8503 = phi i64 [ 0, %8496 ], [ %8514, %8513 ]
  %8504 = add i64 %8502, 1
  %8505 = icmp samesign ult i64 %8501, 8
  br i1 %8505, label %8506, label %8513

8506:                                             ; preds = %8500
  %8507 = getelementptr inbounds nuw i8, ptr %8498, i64 %8502
  %8508 = load i8, ptr %8507, align 1, !tbaa !12
  %8509 = zext i8 %8508 to i64
  %8510 = shl nuw nsw i64 %8501, 3
  %8511 = shl nuw i64 %8509, %8510
  %8512 = or i64 %8511, %8503
  br label %8513

8513:                                             ; preds = %8506, %8500
  %8514 = phi i64 [ %8512, %8506 ], [ %8503, %8500 ]
  %8515 = add nuw nsw i64 %8501, 1
  %8516 = icmp eq i64 %8515, %8499
  br i1 %8516, label %8547, label %8500, !llvm.loop !35

8517:                                             ; preds = %8430
  %8518 = icmp eq i8 %8437, %8445
  br i1 %8518, label %8551, label %8519

8519:                                             ; preds = %8517
  %8520 = sext i32 %8447 to i64
  %8521 = zext i8 %8446 to i64
  br label %8522

8522:                                             ; preds = %8522, %8519
  %8523 = phi i64 [ %8520, %8519 ], [ %8527, %8522 ]
  %8524 = phi i64 [ 0, %8519 ], [ %8545, %8522 ]
  %8525 = phi i64 [ 0, %8519 ], [ %8544, %8522 ]
  %8526 = phi i32 [ %8456, %8519 ], [ %8536, %8522 ]
  %8527 = add nsw i64 %8523, 1
  %8528 = trunc nsw i64 %8527 to i32
  store i32 %8528, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8529 = getelementptr inbounds i8, ptr %8432, i64 %8523
  %8530 = load i8, ptr %8529, align 1, !tbaa !12
  %8531 = shl i32 %8526, 13
  %8532 = xor i32 %8531, %8526
  %8533 = lshr i32 %8532, 17
  %8534 = xor i32 %8533, %8532
  %8535 = shl i32 %8534, 5
  %8536 = xor i32 %8535, %8534
  store i32 %8536, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8537 = trunc i32 %8536 to i8
  %8538 = xor i8 %8530, %8537
  %8539 = icmp samesign ult i64 %8524, 8
  %8540 = zext i8 %8538 to i64
  %8541 = shl nuw nsw i64 %8524, 3
  %8542 = shl nuw i64 %8540, %8541
  %8543 = select i1 %8539, i64 %8542, i64 0
  %8544 = or i64 %8543, %8525
  %8545 = add nuw nsw i64 %8524, 1
  %8546 = icmp eq i64 %8545, %8521
  br i1 %8546, label %8549, label %8522, !llvm.loop !33

8547:                                             ; preds = %8513
  %8548 = phi i64 [ %8514, %8513 ]
  br label %8551

8549:                                             ; preds = %8522
  %8550 = phi i64 [ %8544, %8522 ]
  br label %8551

8551:                                             ; preds = %8549, %8547, %8517, %8493
  %8552 = phi i64 [ 0, %8493 ], [ 0, %8517 ], [ %8548, %8547 ], [ %8550, %8549 ]
  %8553 = or i8 %8458, %8446
  %8554 = icmp eq i8 %8553, 0
  %8555 = icmp eq i8 %8437, %8445
  %8556 = select i1 %8554, i1 true, i1 %8555
  br i1 %8556, label %10047, label %8557

8557:                                             ; preds = %8551
  %8558 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %8559 = inttoptr i64 %8558 to ptr
  %8560 = zext i8 %8446 to i32
  br label %8561

8561:                                             ; preds = %8561, %8557
  %8562 = phi i32 [ %8568, %8561 ], [ 0, %8557 ]
  %8563 = phi ptr [ %8566, %8561 ], [ %8559, %8557 ]
  %8564 = phi i64 [ %8567, %8561 ], [ %8552, %8557 ]
  %8565 = trunc i64 %8564 to i8
  store i8 %8565, ptr %8563, align 1, !tbaa !12
  %8566 = getelementptr inbounds nuw i8, ptr %8563, i64 1
  %8567 = lshr i64 %8564, 8
  %8568 = add nuw nsw i32 %8562, 1
  %8569 = icmp eq i32 %8568, %8560
  br i1 %8569, label %10046, label %8561, !llvm.loop !34

8570:                                             ; preds = %239
  %8571 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %8572 = icmp sgt i32 %8571, 0
  br i1 %8572, label %8576, label %8573

8573:                                             ; preds = %8570
  %8574 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8575 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br label %8612

8576:                                             ; preds = %8570
  %8577 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4
  %8578 = load i32, ptr @ip__Z8vm_entryv, align 4
  %8579 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %8580 = inttoptr i64 %8579 to ptr
  %8581 = sext i32 %8578 to i64
  %8582 = zext nneg i32 %8571 to i64
  br label %8583

8583:                                             ; preds = %8583, %8576
  %8584 = phi i64 [ %8581, %8576 ], [ %8588, %8583 ]
  %8585 = phi i64 [ 0, %8576 ], [ %8606, %8583 ]
  %8586 = phi i64 [ 0, %8576 ], [ %8605, %8583 ]
  %8587 = phi i32 [ %8577, %8576 ], [ %8597, %8583 ]
  %8588 = add nsw i64 %8584, 1
  %8589 = trunc nsw i64 %8588 to i32
  store i32 %8589, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8590 = getelementptr inbounds i8, ptr %8580, i64 %8584
  %8591 = load i8, ptr %8590, align 1, !tbaa !12
  %8592 = shl i32 %8587, 13
  %8593 = xor i32 %8592, %8587
  %8594 = lshr i32 %8593, 17
  %8595 = xor i32 %8594, %8593
  %8596 = shl i32 %8595, 5
  %8597 = xor i32 %8596, %8595
  store i32 %8597, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8598 = trunc i32 %8597 to i8
  %8599 = xor i8 %8591, %8598
  %8600 = icmp samesign ult i64 %8585, 8
  %8601 = zext i8 %8599 to i64
  %8602 = shl nuw nsw i64 %8585, 3
  %8603 = shl nuw i64 %8601, %8602
  %8604 = select i1 %8600, i64 %8603, i64 0
  %8605 = or i64 %8604, %8586
  %8606 = add nuw nsw i64 %8585, 1
  %8607 = icmp eq i64 %8606, %8582
  br i1 %8607, label %8608, label %8583, !llvm.loop !33

8608:                                             ; preds = %8583
  %8609 = phi i32 [ %8589, %8583 ]
  %8610 = phi i32 [ %8597, %8583 ]
  %8611 = phi i64 [ %8605, %8583 ]
  br label %8612

8612:                                             ; preds = %8608, %8573
  %8613 = phi i32 [ %8575, %8573 ], [ %8610, %8608 ]
  %8614 = phi i32 [ %8574, %8573 ], [ %8609, %8608 ]
  %8615 = phi i64 [ 0, %8573 ], [ %8611, %8608 ]
  %8616 = shl i32 %8613, 13
  %8617 = xor i32 %8616, %8613
  %8618 = lshr i32 %8617, 17
  %8619 = xor i32 %8618, %8617
  %8620 = shl i32 %8619, 5
  %8621 = xor i32 %8620, %8619
  %8622 = add nsw i32 %8614, 2
  store i32 %8622, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8623 = shl i32 %8621, 13
  %8624 = xor i32 %8623, %8621
  %8625 = lshr i32 %8624, 17
  %8626 = xor i32 %8625, %8624
  %8627 = shl i32 %8626, 5
  %8628 = xor i32 %8627, %8626
  store i32 %8628, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br i1 %8572, label %8629, label %8661

8629:                                             ; preds = %8612
  %8630 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %8631 = inttoptr i64 %8630 to ptr
  %8632 = sext i32 %8622 to i64
  %8633 = zext nneg i32 %8571 to i64
  br label %8634

8634:                                             ; preds = %8634, %8629
  %8635 = phi i64 [ %8632, %8629 ], [ %8639, %8634 ]
  %8636 = phi i64 [ 0, %8629 ], [ %8657, %8634 ]
  %8637 = phi i64 [ 0, %8629 ], [ %8656, %8634 ]
  %8638 = phi i32 [ %8628, %8629 ], [ %8648, %8634 ]
  %8639 = add nsw i64 %8635, 1
  %8640 = trunc nsw i64 %8639 to i32
  store i32 %8640, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8641 = getelementptr inbounds i8, ptr %8631, i64 %8635
  %8642 = load i8, ptr %8641, align 1, !tbaa !12
  %8643 = shl i32 %8638, 13
  %8644 = xor i32 %8643, %8638
  %8645 = lshr i32 %8644, 17
  %8646 = xor i32 %8645, %8644
  %8647 = shl i32 %8646, 5
  %8648 = xor i32 %8647, %8646
  store i32 %8648, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8649 = trunc i32 %8648 to i8
  %8650 = xor i8 %8642, %8649
  %8651 = icmp samesign ult i64 %8636, 8
  %8652 = zext i8 %8650 to i64
  %8653 = shl nuw nsw i64 %8636, 3
  %8654 = shl nuw i64 %8652, %8653
  %8655 = select i1 %8651, i64 %8654, i64 0
  %8656 = or i64 %8655, %8637
  %8657 = add nuw nsw i64 %8636, 1
  %8658 = icmp eq i64 %8657, %8633
  br i1 %8658, label %8659, label %8634, !llvm.loop !33

8659:                                             ; preds = %8634
  %8660 = phi i64 [ %8656, %8634 ]
  br label %8661

8661:                                             ; preds = %8659, %8612
  %8662 = phi i64 [ 0, %8612 ], [ %8660, %8659 ]
  %8663 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %8664 = icmp eq i8 %8663, 0
  br i1 %8664, label %8671, label %8665

8665:                                             ; preds = %8661
  tail call void @vmp_debug_id__Z8vm_entryv(i32 noundef 1, i64 noundef %8615) #11
  %8666 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %8667 = icmp eq i8 %8666, 0
  br i1 %8667, label %8671, label %8668

8668:                                             ; preds = %8665
  tail call void @vmp_debug_id__Z8vm_entryv(i32 noundef 2, i64 noundef %8662) #11
  %8669 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %8670 = icmp eq i8 %8669, 0
  br label %8671

8671:                                             ; preds = %8668, %8665, %8661
  %8672 = phi i1 [ true, %8661 ], [ %8670, %8668 ], [ true, %8665 ]
  %8673 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8674 = load i32, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  %8675 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8676 = load i8, ptr @exception_thrown__Z8vm_entryv, align 4, !tbaa !12
  %8677 = sext i32 %8673 to i64
  %8678 = load i32, ptr @vm_trace_next__Z8vm_entryv.28, align 4, !tbaa !10
  %8679 = zext i32 %8678 to i64
  %8680 = getelementptr inbounds nuw [32 x %struct.VMTraceEntry], ptr @vm_trace_ring__Z8vm_entryv.33, i64 0, i64 %8679
  store i8 4, ptr %8680, align 8, !tbaa !16
  %8681 = getelementptr inbounds nuw i8, ptr %8680, i64 1
  store i8 0, ptr %8681, align 1, !tbaa !21
  %8682 = getelementptr inbounds nuw i8, ptr %8680, i64 2
  store i8 %8676, ptr %8682, align 2, !tbaa !24
  %8683 = getelementptr inbounds nuw i8, ptr %8680, i64 3
  store i8 0, ptr %8683, align 1, !tbaa !32
  %8684 = getelementptr inbounds nuw i8, ptr %8680, i64 4
  store i32 %8673, ptr %8684, align 4, !tbaa !18
  %8685 = getelementptr inbounds nuw i8, ptr %8680, i64 8
  store i64 %8615, ptr %8685, align 8, !tbaa !19
  %8686 = getelementptr inbounds nuw i8, ptr %8680, i64 16
  store i64 %8677, ptr %8686, align 8, !tbaa !20
  %8687 = getelementptr inbounds nuw i8, ptr %8680, i64 24
  store i64 %8662, ptr %8687, align 8, !tbaa !23
  %8688 = getelementptr inbounds nuw i8, ptr %8680, i64 32
  store i64 0, ptr %8688, align 8, !tbaa !22
  %8689 = add i32 %8678, 1
  %8690 = and i32 %8689, 31
  store i32 %8690, ptr @vm_trace_next__Z8vm_entryv.28, align 4, !tbaa !10
  %8691 = load i64, ptr @vm_trace_total__Z8vm_entryv.29, align 8, !tbaa !6
  %8692 = add i64 %8691, 1
  store i64 %8692, ptr @vm_trace_total__Z8vm_entryv.29, align 8, !tbaa !6
  br i1 %8672, label %8699, label %8693

8693:                                             ; preds = %8671
  %8694 = zext i8 %8676 to i32
  %8695 = load ptr, ptr @exception_ptr__Z8vm_entryv, align 8, !tbaa !54
  %8696 = load i32, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %8697 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.17__Z8vm_entryv.46, i64 noundef %8615, i32 noundef %8673, i64 noundef %8662, i32 noundef %8694, ptr noundef %8695, i32 noundef %8696, ptr noundef null, i32 noundef 0, i32 noundef %8674, i32 noundef %8675)
  %8698 = tail call i32 @fflush(ptr noundef null)
  br label %8699

8699:                                             ; preds = %8693, %8671
  tail call void @vm_interpreter_callinst_dispatch__Z8vm_entryv(i64 noundef %8615) #11
  %8700 = load i8, ptr @exception_thrown__Z8vm_entryv, align 4, !tbaa !12
  %8701 = load i32, ptr @vm_trace_next__Z8vm_entryv.28, align 4, !tbaa !10
  %8702 = zext i32 %8701 to i64
  %8703 = getelementptr inbounds nuw [32 x %struct.VMTraceEntry], ptr @vm_trace_ring__Z8vm_entryv.33, i64 0, i64 %8702
  store i8 4, ptr %8703, align 8, !tbaa !16
  %8704 = getelementptr inbounds nuw i8, ptr %8703, i64 1
  store i8 1, ptr %8704, align 1, !tbaa !21
  %8705 = getelementptr inbounds nuw i8, ptr %8703, i64 2
  store i8 %8700, ptr %8705, align 2, !tbaa !24
  %8706 = getelementptr inbounds nuw i8, ptr %8703, i64 3
  store i8 0, ptr %8706, align 1, !tbaa !32
  %8707 = getelementptr inbounds nuw i8, ptr %8703, i64 4
  store i32 %8673, ptr %8707, align 4, !tbaa !18
  %8708 = getelementptr inbounds nuw i8, ptr %8703, i64 8
  store i64 %8615, ptr %8708, align 8, !tbaa !19
  %8709 = getelementptr inbounds nuw i8, ptr %8703, i64 16
  store i64 %8677, ptr %8709, align 8, !tbaa !20
  %8710 = getelementptr inbounds nuw i8, ptr %8703, i64 24
  store i64 %8662, ptr %8710, align 8, !tbaa !23
  %8711 = getelementptr inbounds nuw i8, ptr %8703, i64 32
  store i64 0, ptr %8711, align 8, !tbaa !22
  %8712 = add i32 %8701, 1
  %8713 = and i32 %8712, 31
  store i32 %8713, ptr @vm_trace_next__Z8vm_entryv.28, align 4, !tbaa !10
  %8714 = load i64, ptr @vm_trace_total__Z8vm_entryv.29, align 8, !tbaa !6
  %8715 = add i64 %8714, 1
  store i64 %8715, ptr @vm_trace_total__Z8vm_entryv.29, align 8, !tbaa !6
  %8716 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  %8717 = icmp eq i8 %8716, 0
  br i1 %8717, label %8718, label %8719

8718:                                             ; preds = %8699
  store i32 %8673, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  store i32 %8674, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  store i32 %8675, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br label %2370

8719:                                             ; preds = %8699
  %8720 = zext i8 %8700 to i32
  %8721 = load ptr, ptr @exception_ptr__Z8vm_entryv, align 8, !tbaa !54
  %8722 = load i32, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %8723 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8724 = load i32, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  %8725 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8726 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.18__Z8vm_entryv.47, i64 noundef %8615, i32 noundef %8673, i64 noundef %8662, i32 noundef %8720, ptr noundef %8721, i32 noundef %8722, ptr noundef null, i32 noundef 0, i32 noundef %8723, i32 noundef %8724, i32 noundef %8725)
  %8727 = tail call i32 @fflush(ptr noundef null)
  %8728 = load i8, ptr @vmp_debug_enabled__Z8vm_entryv.4, align 1, !tbaa !12
  store i32 %8673, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  store i32 %8674, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  store i32 %8675, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8729 = icmp eq i8 %8728, 0
  br i1 %8729, label %2370, label %8730

8730:                                             ; preds = %8719
  %8731 = load i8, ptr @exception_thrown__Z8vm_entryv, align 4, !tbaa !12
  %8732 = zext i8 %8731 to i32
  %8733 = load ptr, ptr @exception_ptr__Z8vm_entryv, align 8, !tbaa !54
  %8734 = load i32, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %8735 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.19__Z8vm_entryv.48, i64 noundef %8615, i32 noundef %8673, i32 noundef %8732, ptr noundef %8733, i32 noundef %8734, ptr noundef null, i32 noundef 0, i32 noundef %8674, i32 noundef %8675)
  %8736 = tail call i32 @fflush(ptr noundef null)
  br label %2370

8737:                                             ; preds = %239
  %8738 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %8739 = icmp sgt i32 %8738, 0
  br i1 %8739, label %8743, label %8740

8740:                                             ; preds = %8737
  %8741 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8742 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br label %8779

8743:                                             ; preds = %8737
  %8744 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4
  %8745 = load i32, ptr @ip__Z8vm_entryv, align 4
  %8746 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %8747 = inttoptr i64 %8746 to ptr
  %8748 = sext i32 %8745 to i64
  %8749 = zext nneg i32 %8738 to i64
  br label %8750

8750:                                             ; preds = %8750, %8743
  %8751 = phi i64 [ %8748, %8743 ], [ %8755, %8750 ]
  %8752 = phi i64 [ 0, %8743 ], [ %8773, %8750 ]
  %8753 = phi i64 [ 0, %8743 ], [ %8772, %8750 ]
  %8754 = phi i32 [ %8744, %8743 ], [ %8764, %8750 ]
  %8755 = add nsw i64 %8751, 1
  %8756 = trunc nsw i64 %8755 to i32
  store i32 %8756, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8757 = getelementptr inbounds i8, ptr %8747, i64 %8751
  %8758 = load i8, ptr %8757, align 1, !tbaa !12
  %8759 = shl i32 %8754, 13
  %8760 = xor i32 %8759, %8754
  %8761 = lshr i32 %8760, 17
  %8762 = xor i32 %8761, %8760
  %8763 = shl i32 %8762, 5
  %8764 = xor i32 %8763, %8762
  store i32 %8764, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8765 = trunc i32 %8764 to i8
  %8766 = xor i8 %8758, %8765
  %8767 = icmp samesign ult i64 %8752, 8
  %8768 = zext i8 %8766 to i64
  %8769 = shl nuw nsw i64 %8752, 3
  %8770 = shl nuw i64 %8768, %8769
  %8771 = select i1 %8767, i64 %8770, i64 0
  %8772 = or i64 %8771, %8753
  %8773 = add nuw nsw i64 %8752, 1
  %8774 = icmp eq i64 %8773, %8749
  br i1 %8774, label %8775, label %8750, !llvm.loop !33

8775:                                             ; preds = %8750
  %8776 = phi i32 [ %8756, %8750 ]
  %8777 = phi i32 [ %8764, %8750 ]
  %8778 = phi i64 [ %8772, %8750 ]
  br label %8779

8779:                                             ; preds = %8775, %8740
  %8780 = phi i32 [ %8742, %8740 ], [ %8777, %8775 ]
  %8781 = phi i32 [ %8741, %8740 ], [ %8776, %8775 ]
  %8782 = phi i64 [ 0, %8740 ], [ %8778, %8775 ]
  %8783 = shl i32 %8780, 13
  %8784 = xor i32 %8783, %8780
  %8785 = lshr i32 %8784, 17
  %8786 = xor i32 %8785, %8784
  %8787 = shl i32 %8786, 5
  %8788 = xor i32 %8787, %8786
  %8789 = add nsw i32 %8781, 2
  store i32 %8789, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8790 = shl i32 %8788, 13
  %8791 = xor i32 %8790, %8788
  %8792 = lshr i32 %8791, 17
  %8793 = xor i32 %8792, %8791
  %8794 = shl i32 %8793, 5
  %8795 = xor i32 %8794, %8793
  store i32 %8795, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br i1 %8739, label %8796, label %8811

8796:                                             ; preds = %8779
  br label %8797

8797:                                             ; preds = %8796, %8797
  %8798 = phi i32 [ %8806, %8797 ], [ 0, %8796 ]
  %8799 = phi i32 [ %8805, %8797 ], [ %8795, %8796 ]
  %8800 = shl i32 %8799, 13
  %8801 = xor i32 %8800, %8799
  %8802 = lshr i32 %8801, 17
  %8803 = xor i32 %8802, %8801
  %8804 = shl i32 %8803, 5
  %8805 = xor i32 %8804, %8803
  %8806 = add nuw nsw i32 %8798, 1
  %8807 = icmp eq i32 %8806, %8738
  br i1 %8807, label %8808, label %8797, !llvm.loop !33

8808:                                             ; preds = %8797
  %8809 = phi i32 [ %8805, %8797 ]
  %8810 = add i32 %8738, %8789
  store i32 %8810, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  store i32 %8809, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br label %8811

8811:                                             ; preds = %8808, %8779
  %8812 = phi i32 [ %8809, %8808 ], [ %8795, %8779 ]
  %8813 = phi i32 [ %8810, %8808 ], [ %8789, %8779 ]
  %8814 = load i32, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  tail call void @vm_interpreter_callinst_dispatch__Z8vm_entryv(i64 noundef %8782) #11
  store i32 %8813, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  store i32 %8814, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  store i32 %8812, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br label %2370

8815:                                             ; preds = %239
  %8816 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8817 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8818 = shl i32 %8817, 13
  %8819 = xor i32 %8818, %8817
  %8820 = lshr i32 %8819, 17
  %8821 = xor i32 %8820, %8819
  %8822 = shl i32 %8821, 5
  %8823 = xor i32 %8822, %8821
  %8824 = shl i32 %8823, 13
  %8825 = xor i32 %8824, %8823
  %8826 = lshr i32 %8825, 17
  %8827 = xor i32 %8826, %8825
  %8828 = shl i32 %8827, 5
  %8829 = xor i32 %8828, %8827
  %8830 = shl i32 %8829, 13
  %8831 = xor i32 %8830, %8829
  %8832 = lshr i32 %8831, 17
  %8833 = xor i32 %8832, %8831
  %8834 = shl i32 %8833, 5
  %8835 = xor i32 %8834, %8833
  %8836 = shl i32 %8835, 13
  %8837 = xor i32 %8836, %8835
  %8838 = lshr i32 %8837, 17
  %8839 = xor i32 %8838, %8837
  %8840 = shl i32 %8839, 5
  %8841 = xor i32 %8840, %8839
  %8842 = shl i32 %8841, 13
  %8843 = xor i32 %8842, %8841
  %8844 = lshr i32 %8843, 17
  %8845 = xor i32 %8844, %8843
  %8846 = shl i32 %8845, 5
  %8847 = xor i32 %8846, %8845
  %8848 = add i32 %8816, 5
  store i32 %8848, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  store i32 %8847, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  fence seq_cst
  br label %2370

8849:                                             ; preds = %239
  %8850 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8851 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8852 = shl i32 %8851, 13
  %8853 = xor i32 %8852, %8851
  %8854 = lshr i32 %8853, 17
  %8855 = xor i32 %8854, %8853
  %8856 = shl i32 %8855, 5
  %8857 = xor i32 %8856, %8855
  %8858 = add nsw i32 %8850, 2
  %8859 = shl i32 %8857, 13
  %8860 = xor i32 %8859, %8857
  %8861 = lshr i32 %8860, 17
  %8862 = xor i32 %8861, %8860
  %8863 = shl i32 %8862, 5
  %8864 = xor i32 %8863, %8862
  store i32 %8864, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8865 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %8866 = icmp sgt i32 %8865, 0
  %8867 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  br i1 %8866, label %8868, label %8901

8868:                                             ; preds = %8849
  %8869 = inttoptr i64 %8867 to ptr
  %8870 = sext i32 %8858 to i64
  %8871 = zext nneg i32 %8865 to i64
  br label %8872

8872:                                             ; preds = %8872, %8868
  %8873 = phi i64 [ %8870, %8868 ], [ %8877, %8872 ]
  %8874 = phi i64 [ 0, %8868 ], [ %8895, %8872 ]
  %8875 = phi i64 [ 0, %8868 ], [ %8894, %8872 ]
  %8876 = phi i32 [ %8864, %8868 ], [ %8886, %8872 ]
  %8877 = add nsw i64 %8873, 1
  %8878 = trunc nsw i64 %8877 to i32
  store i32 %8878, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8879 = getelementptr inbounds i8, ptr %8869, i64 %8873
  %8880 = load i8, ptr %8879, align 1, !tbaa !12
  %8881 = shl i32 %8876, 13
  %8882 = xor i32 %8881, %8876
  %8883 = lshr i32 %8882, 17
  %8884 = xor i32 %8883, %8882
  %8885 = shl i32 %8884, 5
  %8886 = xor i32 %8885, %8884
  store i32 %8886, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8887 = trunc i32 %8886 to i8
  %8888 = xor i8 %8880, %8887
  %8889 = icmp samesign ult i64 %8874, 8
  %8890 = zext i8 %8888 to i64
  %8891 = shl nuw nsw i64 %8874, 3
  %8892 = shl nuw i64 %8890, %8891
  %8893 = select i1 %8889, i64 %8892, i64 0
  %8894 = or i64 %8893, %8875
  %8895 = add nuw nsw i64 %8874, 1
  %8896 = icmp eq i64 %8895, %8871
  br i1 %8896, label %8897, label %8872, !llvm.loop !33

8897:                                             ; preds = %8872
  %8898 = phi i32 [ %8878, %8872 ]
  %8899 = phi i32 [ %8886, %8872 ]
  %8900 = phi i64 [ %8894, %8872 ]
  br label %8901

8901:                                             ; preds = %8897, %8849
  %8902 = phi i32 [ %8864, %8849 ], [ %8899, %8897 ]
  %8903 = phi i32 [ %8858, %8849 ], [ %8898, %8897 ]
  %8904 = phi i64 [ 0, %8849 ], [ %8900, %8897 ]
  %8905 = shl i32 %8902, 13
  %8906 = xor i32 %8905, %8902
  %8907 = lshr i32 %8906, 17
  %8908 = xor i32 %8907, %8906
  %8909 = shl i32 %8908, 5
  %8910 = xor i32 %8909, %8908
  %8911 = add nsw i32 %8903, 2
  %8912 = shl i32 %8910, 13
  %8913 = xor i32 %8912, %8910
  %8914 = lshr i32 %8913, 17
  %8915 = xor i32 %8914, %8913
  %8916 = shl i32 %8915, 5
  %8917 = xor i32 %8916, %8915
  store i32 %8917, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br i1 %8866, label %8918, label %8951

8918:                                             ; preds = %8901
  %8919 = inttoptr i64 %8867 to ptr
  %8920 = sext i32 %8911 to i64
  %8921 = zext nneg i32 %8865 to i64
  br label %8922

8922:                                             ; preds = %8922, %8918
  %8923 = phi i64 [ %8920, %8918 ], [ %8927, %8922 ]
  %8924 = phi i64 [ 0, %8918 ], [ %8945, %8922 ]
  %8925 = phi i64 [ 0, %8918 ], [ %8944, %8922 ]
  %8926 = phi i32 [ %8917, %8918 ], [ %8936, %8922 ]
  %8927 = add nsw i64 %8923, 1
  %8928 = trunc nsw i64 %8927 to i32
  store i32 %8928, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8929 = getelementptr inbounds i8, ptr %8919, i64 %8923
  %8930 = load i8, ptr %8929, align 1, !tbaa !12
  %8931 = shl i32 %8926, 13
  %8932 = xor i32 %8931, %8926
  %8933 = lshr i32 %8932, 17
  %8934 = xor i32 %8933, %8932
  %8935 = shl i32 %8934, 5
  %8936 = xor i32 %8935, %8934
  store i32 %8936, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8937 = trunc i32 %8936 to i8
  %8938 = xor i8 %8930, %8937
  %8939 = icmp samesign ult i64 %8924, 8
  %8940 = zext i8 %8938 to i64
  %8941 = shl nuw nsw i64 %8924, 3
  %8942 = shl nuw i64 %8940, %8941
  %8943 = select i1 %8939, i64 %8942, i64 0
  %8944 = or i64 %8943, %8925
  %8945 = add nuw nsw i64 %8924, 1
  %8946 = icmp eq i64 %8945, %8921
  br i1 %8946, label %8947, label %8922, !llvm.loop !33

8947:                                             ; preds = %8922
  %8948 = phi i32 [ %8928, %8922 ]
  %8949 = phi i32 [ %8936, %8922 ]
  %8950 = phi i64 [ %8944, %8922 ]
  br label %8951

8951:                                             ; preds = %8947, %8901
  %8952 = phi i32 [ %8917, %8901 ], [ %8949, %8947 ]
  %8953 = phi i32 [ %8911, %8901 ], [ %8948, %8947 ]
  %8954 = phi i64 [ 0, %8901 ], [ %8950, %8947 ]
  %8955 = shl i32 %8952, 13
  %8956 = xor i32 %8955, %8952
  %8957 = lshr i32 %8956, 17
  %8958 = xor i32 %8957, %8956
  %8959 = shl i32 %8958, 5
  %8960 = xor i32 %8959, %8958
  %8961 = add nsw i32 %8953, 2
  %8962 = shl i32 %8960, 13
  %8963 = xor i32 %8962, %8960
  %8964 = lshr i32 %8963, 17
  %8965 = xor i32 %8964, %8963
  %8966 = shl i32 %8965, 5
  %8967 = xor i32 %8966, %8965
  store i32 %8967, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br i1 %8866, label %8968, label %9001

8968:                                             ; preds = %8951
  %8969 = inttoptr i64 %8867 to ptr
  %8970 = sext i32 %8961 to i64
  %8971 = zext nneg i32 %8865 to i64
  br label %8972

8972:                                             ; preds = %8972, %8968
  %8973 = phi i64 [ %8970, %8968 ], [ %8977, %8972 ]
  %8974 = phi i64 [ 0, %8968 ], [ %8995, %8972 ]
  %8975 = phi i64 [ 0, %8968 ], [ %8994, %8972 ]
  %8976 = phi i32 [ %8967, %8968 ], [ %8986, %8972 ]
  %8977 = add nsw i64 %8973, 1
  %8978 = trunc nsw i64 %8977 to i32
  store i32 %8978, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %8979 = getelementptr inbounds i8, ptr %8969, i64 %8973
  %8980 = load i8, ptr %8979, align 1, !tbaa !12
  %8981 = shl i32 %8976, 13
  %8982 = xor i32 %8981, %8976
  %8983 = lshr i32 %8982, 17
  %8984 = xor i32 %8983, %8982
  %8985 = shl i32 %8984, 5
  %8986 = xor i32 %8985, %8984
  store i32 %8986, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %8987 = trunc i32 %8986 to i8
  %8988 = xor i8 %8980, %8987
  %8989 = icmp samesign ult i64 %8974, 8
  %8990 = zext i8 %8988 to i64
  %8991 = shl nuw nsw i64 %8974, 3
  %8992 = shl nuw i64 %8990, %8991
  %8993 = select i1 %8989, i64 %8992, i64 0
  %8994 = or i64 %8993, %8975
  %8995 = add nuw nsw i64 %8974, 1
  %8996 = icmp eq i64 %8995, %8971
  br i1 %8996, label %8997, label %8972, !llvm.loop !33

8997:                                             ; preds = %8972
  %8998 = phi i32 [ %8978, %8972 ]
  %8999 = phi i32 [ %8986, %8972 ]
  %9000 = phi i64 [ %8994, %8972 ]
  br label %9001

9001:                                             ; preds = %8997, %8951
  %9002 = phi i32 [ %8967, %8951 ], [ %8999, %8997 ]
  %9003 = phi i32 [ %8961, %8951 ], [ %8998, %8997 ]
  %9004 = phi i64 [ 0, %8951 ], [ %9000, %8997 ]
  %9005 = shl i32 %9002, 13
  %9006 = xor i32 %9005, %9002
  %9007 = lshr i32 %9006, 17
  %9008 = xor i32 %9007, %9006
  %9009 = shl i32 %9008, 5
  %9010 = xor i32 %9009, %9008
  %9011 = add nsw i32 %9003, 2
  %9012 = shl i32 %9010, 13
  %9013 = xor i32 %9012, %9010
  %9014 = lshr i32 %9013, 17
  %9015 = xor i32 %9014, %9013
  %9016 = shl i32 %9015, 5
  %9017 = xor i32 %9016, %9015
  store i32 %9017, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9018 = inttoptr i64 %8867 to ptr
  br i1 %8866, label %9019, label %9051

9019:                                             ; preds = %9001
  %9020 = sext i32 %9011 to i64
  %9021 = zext nneg i32 %8865 to i64
  br label %9022

9022:                                             ; preds = %9022, %9019
  %9023 = phi i64 [ %9020, %9019 ], [ %9027, %9022 ]
  %9024 = phi i64 [ 0, %9019 ], [ %9045, %9022 ]
  %9025 = phi i64 [ 0, %9019 ], [ %9044, %9022 ]
  %9026 = phi i32 [ %9017, %9019 ], [ %9036, %9022 ]
  %9027 = add nsw i64 %9023, 1
  %9028 = trunc nsw i64 %9027 to i32
  store i32 %9028, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9029 = getelementptr inbounds i8, ptr %9018, i64 %9023
  %9030 = load i8, ptr %9029, align 1, !tbaa !12
  %9031 = shl i32 %9026, 13
  %9032 = xor i32 %9031, %9026
  %9033 = lshr i32 %9032, 17
  %9034 = xor i32 %9033, %9032
  %9035 = shl i32 %9034, 5
  %9036 = xor i32 %9035, %9034
  store i32 %9036, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9037 = trunc i32 %9036 to i8
  %9038 = xor i8 %9030, %9037
  %9039 = icmp samesign ult i64 %9024, 8
  %9040 = zext i8 %9038 to i64
  %9041 = shl nuw nsw i64 %9024, 3
  %9042 = shl nuw i64 %9040, %9041
  %9043 = select i1 %9039, i64 %9042, i64 0
  %9044 = or i64 %9043, %9025
  %9045 = add nuw nsw i64 %9024, 1
  %9046 = icmp eq i64 %9045, %9021
  br i1 %9046, label %9047, label %9022, !llvm.loop !33

9047:                                             ; preds = %9022
  %9048 = phi i32 [ %9028, %9022 ]
  %9049 = phi i32 [ %9036, %9022 ]
  %9050 = phi i64 [ %9044, %9022 ]
  br label %9051

9051:                                             ; preds = %9047, %9001
  %9052 = phi i32 [ %9017, %9001 ], [ %9049, %9047 ]
  %9053 = phi i32 [ %9011, %9001 ], [ %9048, %9047 ]
  %9054 = phi i64 [ 0, %9001 ], [ %9050, %9047 ]
  %9055 = shl i32 %9052, 13
  %9056 = xor i32 %9055, %9052
  %9057 = lshr i32 %9056, 17
  %9058 = xor i32 %9057, %9056
  %9059 = shl i32 %9058, 5
  %9060 = xor i32 %9059, %9058
  %9061 = add nsw i32 %9053, 2
  %9062 = shl i32 %9060, 13
  %9063 = xor i32 %9062, %9060
  %9064 = lshr i32 %9063, 17
  %9065 = xor i32 %9064, %9063
  %9066 = shl i32 %9065, 5
  %9067 = xor i32 %9066, %9065
  store i32 %9067, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9068 = sext i32 %9061 to i64
  %9069 = add nsw i64 %9068, 1
  %9070 = trunc nsw i64 %9069 to i32
  store i32 %9070, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9071 = getelementptr inbounds i8, ptr %9018, i64 %9068
  %9072 = load i8, ptr %9071, align 1, !tbaa !12
  %9073 = shl i32 %9067, 13
  %9074 = xor i32 %9073, %9067
  %9075 = lshr i32 %9074, 17
  %9076 = xor i32 %9075, %9074
  %9077 = shl i32 %9076, 5
  %9078 = xor i32 %9077, %9076
  store i32 %9078, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9079 = trunc i32 %9078 to i8
  %9080 = xor i8 %9072, %9079
  %9081 = zext i8 %9080 to i64
  %9082 = add nsw i64 %9068, 2
  %9083 = trunc nsw i64 %9082 to i32
  store i32 %9083, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9084 = getelementptr inbounds i8, ptr %9018, i64 %9069
  %9085 = load i8, ptr %9084, align 1, !tbaa !12
  %9086 = shl i32 %9078, 13
  %9087 = xor i32 %9086, %9078
  %9088 = lshr i32 %9087, 17
  %9089 = xor i32 %9088, %9087
  %9090 = shl i32 %9089, 5
  %9091 = xor i32 %9090, %9089
  store i32 %9091, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9092 = trunc i32 %9091 to i8
  %9093 = xor i8 %9085, %9092
  %9094 = zext i8 %9093 to i64
  %9095 = shl nuw nsw i64 %9094, 8
  %9096 = or disjoint i64 %9095, %9081
  %9097 = add nsw i64 %9068, 3
  %9098 = trunc nsw i64 %9097 to i32
  store i32 %9098, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9099 = getelementptr inbounds i8, ptr %9018, i64 %9082
  %9100 = load i8, ptr %9099, align 1, !tbaa !12
  %9101 = shl i32 %9091, 13
  %9102 = xor i32 %9101, %9091
  %9103 = lshr i32 %9102, 17
  %9104 = xor i32 %9103, %9102
  %9105 = shl i32 %9104, 5
  %9106 = xor i32 %9105, %9104
  store i32 %9106, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9107 = trunc i32 %9106 to i8
  %9108 = xor i8 %9100, %9107
  %9109 = zext i8 %9108 to i64
  %9110 = shl nuw nsw i64 %9109, 16
  %9111 = or disjoint i64 %9110, %9096
  %9112 = add i32 %9053, 6
  store i32 %9112, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9113 = getelementptr inbounds i8, ptr %9018, i64 %9097
  %9114 = load i8, ptr %9113, align 1, !tbaa !12
  %9115 = shl i32 %9106, 13
  %9116 = xor i32 %9115, %9106
  %9117 = lshr i32 %9116, 17
  %9118 = xor i32 %9117, %9116
  %9119 = shl i32 %9118, 5
  %9120 = xor i32 %9119, %9118
  store i32 %9120, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9121 = trunc i32 %9120 to i8
  %9122 = xor i8 %9114, %9121
  %9123 = zext i8 %9122 to i64
  %9124 = shl nuw nsw i64 %9123, 24
  %9125 = or disjoint i64 %9124, %9111
  %9126 = trunc nuw i64 %9125 to i32
  %9127 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %9128 = inttoptr i64 %9127 to ptr
  br i1 %8866, label %9129, label %9151

9129:                                             ; preds = %9051
  %9130 = zext nneg i32 %8865 to i64
  br label %9131

9131:                                             ; preds = %9144, %9129
  %9132 = phi i64 [ 0, %9129 ], [ %9146, %9144 ]
  %9133 = phi i64 [ %8954, %9129 ], [ %9135, %9144 ]
  %9134 = phi i64 [ 0, %9129 ], [ %9145, %9144 ]
  %9135 = add i64 %9133, 1
  %9136 = icmp samesign ult i64 %9132, 8
  br i1 %9136, label %9137, label %9144

9137:                                             ; preds = %9131
  %9138 = getelementptr inbounds nuw i8, ptr %9128, i64 %9133
  %9139 = load i8, ptr %9138, align 1, !tbaa !12
  %9140 = zext i8 %9139 to i64
  %9141 = shl nuw nsw i64 %9132, 3
  %9142 = shl nuw i64 %9140, %9141
  %9143 = or i64 %9142, %9134
  br label %9144

9144:                                             ; preds = %9137, %9131
  %9145 = phi i64 [ %9143, %9137 ], [ %9134, %9131 ]
  %9146 = add nuw nsw i64 %9132, 1
  %9147 = icmp eq i64 %9146, %9130
  br i1 %9147, label %9148, label %9131, !llvm.loop !35

9148:                                             ; preds = %9144
  %9149 = phi i64 [ %9145, %9144 ]
  %9150 = inttoptr i64 %9149 to ptr
  br label %9151

9151:                                             ; preds = %9148, %9051
  %9152 = phi ptr [ %9150, %9148 ], [ null, %9051 ]
  %9153 = getelementptr inbounds nuw i8, ptr %9128, i64 %9004
  %9154 = getelementptr inbounds nuw i8, ptr %9128, i64 %9054
  call void @llvm.lifetime.start.p0(i64 16, ptr nonnull %2)
  switch i32 %9126, label %9179 [
    i32 1, label %9155
    i32 2, label %9161
    i32 4, label %9167
    i32 8, label %9173
  ]

9155:                                             ; preds = %9151
  %9156 = load i8, ptr %9153, align 1, !tbaa !12
  %9157 = load i8, ptr %9154, align 1, !tbaa !12
  %9158 = cmpxchg ptr %9152, i8 %9156, i8 %9157 seq_cst seq_cst, align 1
  %9159 = extractvalue { i8, i1 } %9158, 0
  %9160 = extractvalue { i8, i1 } %9158, 1
  store i8 %9159, ptr %2, align 8, !tbaa !12
  br label %9179

9161:                                             ; preds = %9151
  %9162 = load i16, ptr %9153, align 2, !tbaa !77
  %9163 = load i16, ptr %9154, align 2, !tbaa !77
  %9164 = cmpxchg ptr %9152, i16 %9162, i16 %9163 seq_cst seq_cst, align 2
  %9165 = extractvalue { i16, i1 } %9164, 0
  %9166 = extractvalue { i16, i1 } %9164, 1
  store i16 %9165, ptr %2, align 8, !tbaa !77
  br label %9179

9167:                                             ; preds = %9151
  %9168 = load i32, ptr %9153, align 4, !tbaa !10
  %9169 = load i32, ptr %9154, align 4, !tbaa !10
  %9170 = cmpxchg ptr %9152, i32 %9168, i32 %9169 seq_cst seq_cst, align 4
  %9171 = extractvalue { i32, i1 } %9170, 0
  %9172 = extractvalue { i32, i1 } %9170, 1
  store i32 %9171, ptr %2, align 8, !tbaa !10
  br label %9179

9173:                                             ; preds = %9151
  %9174 = load i64, ptr %9153, align 8, !tbaa !6
  %9175 = load i64, ptr %9154, align 8, !tbaa !6
  %9176 = cmpxchg ptr %9152, i64 %9174, i64 %9175 seq_cst seq_cst, align 8
  %9177 = extractvalue { i64, i1 } %9176, 0
  %9178 = extractvalue { i64, i1 } %9176, 1
  store i64 %9177, ptr %2, align 8, !tbaa !6
  br label %9179

9179:                                             ; preds = %9173, %9167, %9161, %9155, %9151
  %9180 = phi i1 [ %9160, %9155 ], [ %9166, %9161 ], [ %9172, %9167 ], [ %9178, %9173 ], [ false, %9151 ]
  %9181 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %9182 = inttoptr i64 %9181 to ptr
  %9183 = getelementptr inbounds nuw i8, ptr %9182, i64 %8904
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %9183, ptr nonnull align 8 %2, i64 %9125, i1 false)
  %9184 = zext i1 %9180 to i8
  %9185 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %9186 = add i64 %9125, %8904
  %9187 = add i64 %9186, %9185
  %9188 = inttoptr i64 %9187 to ptr
  store i8 %9184, ptr %9188, align 1, !tbaa !12
  call void @llvm.lifetime.end.p0(i64 16, ptr nonnull %2)
  br label %2370

9189:                                             ; preds = %239
  %9190 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %9191 = inttoptr i64 %9190 to ptr
  %9192 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9193 = add nsw i32 %9192, 1
  store i32 %9193, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9194 = sext i32 %9192 to i64
  %9195 = getelementptr inbounds i8, ptr %9191, i64 %9194
  %9196 = load i8, ptr %9195, align 1, !tbaa !12
  %9197 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9198 = shl i32 %9197, 13
  %9199 = xor i32 %9198, %9197
  %9200 = lshr i32 %9199, 17
  %9201 = xor i32 %9200, %9199
  %9202 = shl i32 %9201, 5
  %9203 = xor i32 %9202, %9201
  %9204 = trunc i32 %9203 to i8
  %9205 = xor i8 %9196, %9204
  %9206 = shl i32 %9203, 13
  %9207 = xor i32 %9206, %9203
  %9208 = lshr i32 %9207, 17
  %9209 = xor i32 %9208, %9207
  %9210 = shl i32 %9209, 5
  %9211 = xor i32 %9210, %9209
  %9212 = add nsw i32 %9192, 3
  %9213 = shl i32 %9211, 13
  %9214 = xor i32 %9213, %9211
  %9215 = lshr i32 %9214, 17
  %9216 = xor i32 %9215, %9214
  %9217 = shl i32 %9216, 5
  %9218 = xor i32 %9217, %9216
  store i32 %9218, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9219 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %9220 = icmp sgt i32 %9219, 0
  br i1 %9220, label %9221, label %9253

9221:                                             ; preds = %9189
  %9222 = sext i32 %9212 to i64
  %9223 = zext nneg i32 %9219 to i64
  br label %9224

9224:                                             ; preds = %9224, %9221
  %9225 = phi i64 [ %9222, %9221 ], [ %9229, %9224 ]
  %9226 = phi i64 [ 0, %9221 ], [ %9247, %9224 ]
  %9227 = phi i64 [ 0, %9221 ], [ %9246, %9224 ]
  %9228 = phi i32 [ %9218, %9221 ], [ %9238, %9224 ]
  %9229 = add nsw i64 %9225, 1
  %9230 = trunc nsw i64 %9229 to i32
  store i32 %9230, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9231 = getelementptr inbounds i8, ptr %9191, i64 %9225
  %9232 = load i8, ptr %9231, align 1, !tbaa !12
  %9233 = shl i32 %9228, 13
  %9234 = xor i32 %9233, %9228
  %9235 = lshr i32 %9234, 17
  %9236 = xor i32 %9235, %9234
  %9237 = shl i32 %9236, 5
  %9238 = xor i32 %9237, %9236
  store i32 %9238, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9239 = trunc i32 %9238 to i8
  %9240 = xor i8 %9232, %9239
  %9241 = icmp samesign ult i64 %9226, 8
  %9242 = zext i8 %9240 to i64
  %9243 = shl nuw nsw i64 %9226, 3
  %9244 = shl nuw i64 %9242, %9243
  %9245 = select i1 %9241, i64 %9244, i64 0
  %9246 = or i64 %9245, %9227
  %9247 = add nuw nsw i64 %9226, 1
  %9248 = icmp eq i64 %9247, %9223
  br i1 %9248, label %9249, label %9224, !llvm.loop !33

9249:                                             ; preds = %9224
  %9250 = phi i32 [ %9230, %9224 ]
  %9251 = phi i32 [ %9238, %9224 ]
  %9252 = phi i64 [ %9246, %9224 ]
  br label %9253

9253:                                             ; preds = %9249, %9189
  %9254 = phi i32 [ %9218, %9189 ], [ %9251, %9249 ]
  %9255 = phi i32 [ %9212, %9189 ], [ %9250, %9249 ]
  %9256 = phi i64 [ 0, %9189 ], [ %9252, %9249 ]
  %9257 = shl i32 %9254, 13
  %9258 = xor i32 %9257, %9254
  %9259 = lshr i32 %9258, 17
  %9260 = xor i32 %9259, %9258
  %9261 = shl i32 %9260, 5
  %9262 = xor i32 %9261, %9260
  %9263 = add nsw i32 %9255, 2
  %9264 = shl i32 %9262, 13
  %9265 = xor i32 %9264, %9262
  %9266 = lshr i32 %9265, 17
  %9267 = xor i32 %9266, %9265
  %9268 = shl i32 %9267, 5
  %9269 = xor i32 %9268, %9267
  store i32 %9269, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br i1 %9220, label %9270, label %9302

9270:                                             ; preds = %9253
  %9271 = sext i32 %9263 to i64
  %9272 = zext nneg i32 %9219 to i64
  br label %9273

9273:                                             ; preds = %9273, %9270
  %9274 = phi i64 [ %9271, %9270 ], [ %9278, %9273 ]
  %9275 = phi i64 [ 0, %9270 ], [ %9296, %9273 ]
  %9276 = phi i64 [ 0, %9270 ], [ %9295, %9273 ]
  %9277 = phi i32 [ %9269, %9270 ], [ %9287, %9273 ]
  %9278 = add nsw i64 %9274, 1
  %9279 = trunc nsw i64 %9278 to i32
  store i32 %9279, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9280 = getelementptr inbounds i8, ptr %9191, i64 %9274
  %9281 = load i8, ptr %9280, align 1, !tbaa !12
  %9282 = shl i32 %9277, 13
  %9283 = xor i32 %9282, %9277
  %9284 = lshr i32 %9283, 17
  %9285 = xor i32 %9284, %9283
  %9286 = shl i32 %9285, 5
  %9287 = xor i32 %9286, %9285
  store i32 %9287, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9288 = trunc i32 %9287 to i8
  %9289 = xor i8 %9281, %9288
  %9290 = icmp samesign ult i64 %9275, 8
  %9291 = zext i8 %9289 to i64
  %9292 = shl nuw nsw i64 %9275, 3
  %9293 = shl nuw i64 %9291, %9292
  %9294 = select i1 %9290, i64 %9293, i64 0
  %9295 = or i64 %9294, %9276
  %9296 = add nuw nsw i64 %9275, 1
  %9297 = icmp eq i64 %9296, %9272
  br i1 %9297, label %9298, label %9273, !llvm.loop !33

9298:                                             ; preds = %9273
  %9299 = phi i32 [ %9279, %9273 ]
  %9300 = phi i32 [ %9287, %9273 ]
  %9301 = phi i64 [ %9295, %9273 ]
  br label %9302

9302:                                             ; preds = %9298, %9253
  %9303 = phi i32 [ %9269, %9253 ], [ %9300, %9298 ]
  %9304 = phi i32 [ %9263, %9253 ], [ %9299, %9298 ]
  %9305 = phi i64 [ 0, %9253 ], [ %9301, %9298 ]
  %9306 = add nsw i32 %9304, 1
  store i32 %9306, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9307 = sext i32 %9304 to i64
  %9308 = getelementptr inbounds i8, ptr %9191, i64 %9307
  %9309 = load i8, ptr %9308, align 1, !tbaa !12
  %9310 = shl i32 %9303, 13
  %9311 = xor i32 %9310, %9303
  %9312 = lshr i32 %9311, 17
  %9313 = xor i32 %9312, %9311
  %9314 = shl i32 %9313, 5
  %9315 = xor i32 %9314, %9313
  store i32 %9315, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9316 = trunc i32 %9315 to i8
  %9317 = xor i8 %9309, %9316
  %9318 = add nsw i32 %9304, 2
  store i32 %9318, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9319 = sext i32 %9306 to i64
  %9320 = getelementptr inbounds i8, ptr %9191, i64 %9319
  %9321 = load i8, ptr %9320, align 1, !tbaa !12
  %9322 = shl i32 %9315, 13
  %9323 = xor i32 %9322, %9315
  %9324 = lshr i32 %9323, 17
  %9325 = xor i32 %9324, %9323
  %9326 = shl i32 %9325, 5
  %9327 = xor i32 %9326, %9325
  store i32 %9327, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9328 = trunc i32 %9327 to i8
  %9329 = icmp eq i8 %9321, %9328
  br i1 %9329, label %9330, label %9389

9330:                                             ; preds = %9302
  br i1 %9220, label %9331, label %9363

9331:                                             ; preds = %9330
  %9332 = sext i32 %9318 to i64
  %9333 = zext nneg i32 %9219 to i64
  br label %9334

9334:                                             ; preds = %9334, %9331
  %9335 = phi i64 [ %9332, %9331 ], [ %9339, %9334 ]
  %9336 = phi i64 [ 0, %9331 ], [ %9357, %9334 ]
  %9337 = phi i64 [ 0, %9331 ], [ %9356, %9334 ]
  %9338 = phi i32 [ %9327, %9331 ], [ %9348, %9334 ]
  %9339 = add nsw i64 %9335, 1
  %9340 = trunc nsw i64 %9339 to i32
  store i32 %9340, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9341 = getelementptr inbounds i8, ptr %9191, i64 %9335
  %9342 = load i8, ptr %9341, align 1, !tbaa !12
  %9343 = shl i32 %9338, 13
  %9344 = xor i32 %9343, %9338
  %9345 = lshr i32 %9344, 17
  %9346 = xor i32 %9345, %9344
  %9347 = shl i32 %9346, 5
  %9348 = xor i32 %9347, %9346
  store i32 %9348, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9349 = trunc i32 %9348 to i8
  %9350 = xor i8 %9342, %9349
  %9351 = icmp samesign ult i64 %9336, 8
  %9352 = zext i8 %9350 to i64
  %9353 = shl nuw nsw i64 %9336, 3
  %9354 = shl nuw i64 %9352, %9353
  %9355 = select i1 %9351, i64 %9354, i64 0
  %9356 = or i64 %9355, %9337
  %9357 = add nuw nsw i64 %9336, 1
  %9358 = icmp eq i64 %9357, %9333
  br i1 %9358, label %9359, label %9334, !llvm.loop !33

9359:                                             ; preds = %9334
  %9360 = phi i32 [ %9340, %9334 ]
  %9361 = phi i32 [ %9348, %9334 ]
  %9362 = phi i64 [ %9356, %9334 ]
  br label %9363

9363:                                             ; preds = %9359, %9330
  %9364 = phi i32 [ %9327, %9330 ], [ %9361, %9359 ]
  %9365 = phi i32 [ %9318, %9330 ], [ %9360, %9359 ]
  %9366 = phi i64 [ 0, %9330 ], [ %9362, %9359 ]
  %9367 = icmp eq i8 %9309, %9316
  br i1 %9367, label %9425, label %9368

9368:                                             ; preds = %9363
  %9369 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %9370 = inttoptr i64 %9369 to ptr
  %9371 = zext i8 %9317 to i64
  br label %9372

9372:                                             ; preds = %9385, %9368
  %9373 = phi i64 [ 0, %9368 ], [ %9387, %9385 ]
  %9374 = phi i64 [ %9366, %9368 ], [ %9376, %9385 ]
  %9375 = phi i64 [ 0, %9368 ], [ %9386, %9385 ]
  %9376 = add i64 %9374, 1
  %9377 = icmp samesign ult i64 %9373, 8
  br i1 %9377, label %9378, label %9385

9378:                                             ; preds = %9372
  %9379 = getelementptr inbounds nuw i8, ptr %9370, i64 %9374
  %9380 = load i8, ptr %9379, align 1, !tbaa !12
  %9381 = zext i8 %9380 to i64
  %9382 = shl nuw nsw i64 %9373, 3
  %9383 = shl nuw i64 %9381, %9382
  %9384 = or i64 %9383, %9375
  br label %9385

9385:                                             ; preds = %9378, %9372
  %9386 = phi i64 [ %9384, %9378 ], [ %9375, %9372 ]
  %9387 = add nuw nsw i64 %9373, 1
  %9388 = icmp eq i64 %9387, %9371
  br i1 %9388, label %9419, label %9372, !llvm.loop !35

9389:                                             ; preds = %9302
  %9390 = icmp eq i8 %9309, %9316
  br i1 %9390, label %9425, label %9391

9391:                                             ; preds = %9389
  %9392 = sext i32 %9318 to i64
  %9393 = zext i8 %9317 to i64
  br label %9394

9394:                                             ; preds = %9394, %9391
  %9395 = phi i64 [ %9392, %9391 ], [ %9399, %9394 ]
  %9396 = phi i64 [ 0, %9391 ], [ %9417, %9394 ]
  %9397 = phi i64 [ 0, %9391 ], [ %9416, %9394 ]
  %9398 = phi i32 [ %9327, %9391 ], [ %9408, %9394 ]
  %9399 = add nsw i64 %9395, 1
  %9400 = trunc nsw i64 %9399 to i32
  store i32 %9400, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9401 = getelementptr inbounds i8, ptr %9191, i64 %9395
  %9402 = load i8, ptr %9401, align 1, !tbaa !12
  %9403 = shl i32 %9398, 13
  %9404 = xor i32 %9403, %9398
  %9405 = lshr i32 %9404, 17
  %9406 = xor i32 %9405, %9404
  %9407 = shl i32 %9406, 5
  %9408 = xor i32 %9407, %9406
  store i32 %9408, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9409 = trunc i32 %9408 to i8
  %9410 = xor i8 %9402, %9409
  %9411 = icmp samesign ult i64 %9396, 8
  %9412 = zext i8 %9410 to i64
  %9413 = shl nuw nsw i64 %9396, 3
  %9414 = shl nuw i64 %9412, %9413
  %9415 = select i1 %9411, i64 %9414, i64 0
  %9416 = or i64 %9415, %9397
  %9417 = add nuw nsw i64 %9396, 1
  %9418 = icmp eq i64 %9417, %9393
  br i1 %9418, label %9421, label %9394, !llvm.loop !33

9419:                                             ; preds = %9385
  %9420 = phi i64 [ %9386, %9385 ]
  br label %9425

9421:                                             ; preds = %9394
  %9422 = phi i32 [ %9400, %9394 ]
  %9423 = phi i32 [ %9408, %9394 ]
  %9424 = phi i64 [ %9416, %9394 ]
  br label %9425

9425:                                             ; preds = %9421, %9419, %9389, %9363
  %9426 = phi i32 [ %9364, %9363 ], [ %9327, %9389 ], [ %9364, %9419 ], [ %9423, %9421 ]
  %9427 = phi i32 [ %9365, %9363 ], [ %9318, %9389 ], [ %9365, %9419 ], [ %9422, %9421 ]
  %9428 = phi i64 [ 0, %9363 ], [ 0, %9389 ], [ %9420, %9419 ], [ %9424, %9421 ]
  %9429 = add nsw i32 %9427, 1
  %9430 = shl i32 %9426, 13
  %9431 = xor i32 %9430, %9426
  %9432 = lshr i32 %9431, 17
  %9433 = xor i32 %9432, %9431
  %9434 = shl i32 %9433, 5
  %9435 = xor i32 %9434, %9433
  store i32 %9435, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9436 = sext i32 %9429 to i64
  %9437 = add nsw i64 %9436, 1
  %9438 = trunc nsw i64 %9437 to i32
  store i32 %9438, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9439 = getelementptr inbounds i8, ptr %9191, i64 %9436
  %9440 = load i8, ptr %9439, align 1, !tbaa !12
  %9441 = shl i32 %9435, 13
  %9442 = xor i32 %9441, %9435
  %9443 = lshr i32 %9442, 17
  %9444 = xor i32 %9443, %9442
  %9445 = shl i32 %9444, 5
  %9446 = xor i32 %9445, %9444
  store i32 %9446, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9447 = trunc i32 %9446 to i8
  %9448 = xor i8 %9440, %9447
  %9449 = zext i8 %9448 to i64
  %9450 = add nsw i64 %9436, 2
  %9451 = trunc nsw i64 %9450 to i32
  store i32 %9451, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9452 = getelementptr inbounds i8, ptr %9191, i64 %9437
  %9453 = load i8, ptr %9452, align 1, !tbaa !12
  %9454 = shl i32 %9446, 13
  %9455 = xor i32 %9454, %9446
  %9456 = lshr i32 %9455, 17
  %9457 = xor i32 %9456, %9455
  %9458 = shl i32 %9457, 5
  %9459 = xor i32 %9458, %9457
  store i32 %9459, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9460 = trunc i32 %9459 to i8
  %9461 = xor i8 %9453, %9460
  %9462 = zext i8 %9461 to i64
  %9463 = shl nuw nsw i64 %9462, 8
  %9464 = or disjoint i64 %9463, %9449
  %9465 = add nsw i64 %9436, 3
  %9466 = trunc nsw i64 %9465 to i32
  store i32 %9466, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9467 = getelementptr inbounds i8, ptr %9191, i64 %9450
  %9468 = load i8, ptr %9467, align 1, !tbaa !12
  %9469 = shl i32 %9459, 13
  %9470 = xor i32 %9469, %9459
  %9471 = lshr i32 %9470, 17
  %9472 = xor i32 %9471, %9470
  %9473 = shl i32 %9472, 5
  %9474 = xor i32 %9473, %9472
  store i32 %9474, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9475 = trunc i32 %9474 to i8
  %9476 = xor i8 %9468, %9475
  %9477 = zext i8 %9476 to i64
  %9478 = shl nuw nsw i64 %9477, 16
  %9479 = or disjoint i64 %9478, %9464
  %9480 = add i32 %9427, 5
  store i32 %9480, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9481 = getelementptr inbounds i8, ptr %9191, i64 %9465
  %9482 = load i8, ptr %9481, align 1, !tbaa !12
  %9483 = shl i32 %9474, 13
  %9484 = xor i32 %9483, %9474
  %9485 = lshr i32 %9484, 17
  %9486 = xor i32 %9485, %9484
  %9487 = shl i32 %9486, 5
  %9488 = xor i32 %9487, %9486
  store i32 %9488, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9489 = trunc i32 %9488 to i8
  %9490 = xor i8 %9482, %9489
  %9491 = zext i8 %9490 to i64
  %9492 = shl nuw nsw i64 %9491, 24
  %9493 = or disjoint i64 %9492, %9479
  %9494 = trunc nuw i64 %9493 to i32
  br i1 %9220, label %9495, label %9519

9495:                                             ; preds = %9425
  %9496 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8
  %9497 = inttoptr i64 %9496 to ptr
  %9498 = zext nneg i32 %9219 to i64
  br label %9499

9499:                                             ; preds = %9512, %9495
  %9500 = phi i64 [ 0, %9495 ], [ %9514, %9512 ]
  %9501 = phi i64 [ %9305, %9495 ], [ %9503, %9512 ]
  %9502 = phi i64 [ 0, %9495 ], [ %9513, %9512 ]
  %9503 = add i64 %9501, 1
  %9504 = icmp samesign ult i64 %9500, 8
  br i1 %9504, label %9505, label %9512

9505:                                             ; preds = %9499
  %9506 = getelementptr inbounds nuw i8, ptr %9497, i64 %9501
  %9507 = load i8, ptr %9506, align 1, !tbaa !12
  %9508 = zext i8 %9507 to i64
  %9509 = shl nuw nsw i64 %9500, 3
  %9510 = shl nuw i64 %9508, %9509
  %9511 = or i64 %9510, %9502
  br label %9512

9512:                                             ; preds = %9505, %9499
  %9513 = phi i64 [ %9511, %9505 ], [ %9502, %9499 ]
  %9514 = add nuw nsw i64 %9500, 1
  %9515 = icmp eq i64 %9514, %9498
  br i1 %9515, label %9516, label %9499, !llvm.loop !35

9516:                                             ; preds = %9512
  %9517 = phi i64 [ %9513, %9512 ]
  %9518 = inttoptr i64 %9517 to ptr
  br label %9519

9519:                                             ; preds = %9516, %9425
  %9520 = phi ptr [ null, %9425 ], [ %9518, %9516 ]
  call void @llvm.lifetime.start.p0(i64 16, ptr nonnull %3)
  switch i32 %9494, label %9556 [
    i32 4, label %9521
    i32 8, label %9539
  ]

9521:                                             ; preds = %9519
  %9522 = trunc i64 %9428 to i32
  switch i8 %9205, label %9535 [
    i8 0, label %9523
    i8 1, label %9525
    i8 2, label %9527
    i8 3, label %9529
    i8 5, label %9531
    i8 6, label %9533
  ]

9523:                                             ; preds = %9521
  %9524 = atomicrmw xchg ptr %9520, i32 %9522 seq_cst, align 4
  br label %9537

9525:                                             ; preds = %9521
  %9526 = atomicrmw add ptr %9520, i32 %9522 seq_cst, align 4
  br label %9537

9527:                                             ; preds = %9521
  %9528 = atomicrmw sub ptr %9520, i32 %9522 seq_cst, align 4
  br label %9537

9529:                                             ; preds = %9521
  %9530 = atomicrmw and ptr %9520, i32 %9522 seq_cst, align 4
  br label %9537

9531:                                             ; preds = %9521
  %9532 = atomicrmw or ptr %9520, i32 %9522 seq_cst, align 4
  br label %9537

9533:                                             ; preds = %9521
  %9534 = atomicrmw xor ptr %9520, i32 %9522 seq_cst, align 4
  br label %9537

9535:                                             ; preds = %9521
  %9536 = atomicrmw add ptr %9520, i32 %9522 seq_cst, align 4
  br label %9537

9537:                                             ; preds = %9535, %9533, %9531, %9529, %9527, %9525, %9523
  %9538 = phi i32 [ %9536, %9535 ], [ %9524, %9523 ], [ %9526, %9525 ], [ %9528, %9527 ], [ %9530, %9529 ], [ %9532, %9531 ], [ %9534, %9533 ]
  store i32 %9538, ptr %3, align 8, !tbaa !10
  br label %9556

9539:                                             ; preds = %9519
  switch i8 %9205, label %9552 [
    i8 0, label %9540
    i8 1, label %9542
    i8 2, label %9544
    i8 3, label %9546
    i8 5, label %9548
    i8 6, label %9550
  ]

9540:                                             ; preds = %9539
  %9541 = atomicrmw xchg ptr %9520, i64 %9428 seq_cst, align 8
  br label %9554

9542:                                             ; preds = %9539
  %9543 = atomicrmw add ptr %9520, i64 %9428 seq_cst, align 8
  br label %9554

9544:                                             ; preds = %9539
  %9545 = atomicrmw sub ptr %9520, i64 %9428 seq_cst, align 8
  br label %9554

9546:                                             ; preds = %9539
  %9547 = atomicrmw and ptr %9520, i64 %9428 seq_cst, align 8
  br label %9554

9548:                                             ; preds = %9539
  %9549 = atomicrmw or ptr %9520, i64 %9428 seq_cst, align 8
  br label %9554

9550:                                             ; preds = %9539
  %9551 = atomicrmw xor ptr %9520, i64 %9428 seq_cst, align 8
  br label %9554

9552:                                             ; preds = %9539
  %9553 = atomicrmw add ptr %9520, i64 %9428 seq_cst, align 8
  br label %9554

9554:                                             ; preds = %9552, %9550, %9548, %9546, %9544, %9542, %9540
  %9555 = phi i64 [ %9553, %9552 ], [ %9541, %9540 ], [ %9543, %9542 ], [ %9545, %9544 ], [ %9547, %9546 ], [ %9549, %9548 ], [ %9551, %9550 ]
  store i64 %9555, ptr %3, align 8, !tbaa !6
  br label %9556

9556:                                             ; preds = %9554, %9537, %9519
  %9557 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %9558 = inttoptr i64 %9557 to ptr
  %9559 = getelementptr inbounds nuw i8, ptr %9558, i64 %9256
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %9559, ptr nonnull align 8 %3, i64 %9493, i1 false)
  call void @llvm.lifetime.end.p0(i64 16, ptr nonnull %3)
  br label %2370

9560:                                             ; preds = %239
  %9561 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9562 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9563 = shl i32 %9562, 13
  %9564 = xor i32 %9563, %9562
  %9565 = lshr i32 %9564, 17
  %9566 = xor i32 %9565, %9564
  %9567 = shl i32 %9566, 5
  %9568 = xor i32 %9567, %9566
  %9569 = add nsw i32 %9561, 2
  %9570 = shl i32 %9568, 13
  %9571 = xor i32 %9570, %9568
  %9572 = lshr i32 %9571, 17
  %9573 = xor i32 %9572, %9571
  %9574 = shl i32 %9573, 5
  %9575 = xor i32 %9574, %9573
  store i32 %9575, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9576 = load i32, ptr @pointer_size__Z8vm_entryv, align 4, !tbaa !10
  %9577 = icmp sgt i32 %9576, 0
  %9578 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  br i1 %9577, label %9579, label %9612

9579:                                             ; preds = %9560
  %9580 = inttoptr i64 %9578 to ptr
  %9581 = sext i32 %9569 to i64
  %9582 = zext nneg i32 %9576 to i64
  br label %9583

9583:                                             ; preds = %9583, %9579
  %9584 = phi i64 [ %9581, %9579 ], [ %9588, %9583 ]
  %9585 = phi i64 [ 0, %9579 ], [ %9606, %9583 ]
  %9586 = phi i64 [ 0, %9579 ], [ %9605, %9583 ]
  %9587 = phi i32 [ %9575, %9579 ], [ %9597, %9583 ]
  %9588 = add nsw i64 %9584, 1
  %9589 = trunc nsw i64 %9588 to i32
  store i32 %9589, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9590 = getelementptr inbounds i8, ptr %9580, i64 %9584
  %9591 = load i8, ptr %9590, align 1, !tbaa !12
  %9592 = shl i32 %9587, 13
  %9593 = xor i32 %9592, %9587
  %9594 = lshr i32 %9593, 17
  %9595 = xor i32 %9594, %9593
  %9596 = shl i32 %9595, 5
  %9597 = xor i32 %9596, %9595
  store i32 %9597, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9598 = trunc i32 %9597 to i8
  %9599 = xor i8 %9591, %9598
  %9600 = icmp samesign ult i64 %9585, 8
  %9601 = zext i8 %9599 to i64
  %9602 = shl nuw nsw i64 %9585, 3
  %9603 = shl nuw i64 %9601, %9602
  %9604 = select i1 %9600, i64 %9603, i64 0
  %9605 = or i64 %9604, %9586
  %9606 = add nuw nsw i64 %9585, 1
  %9607 = icmp eq i64 %9606, %9582
  br i1 %9607, label %9608, label %9583, !llvm.loop !33

9608:                                             ; preds = %9583
  %9609 = phi i32 [ %9589, %9583 ]
  %9610 = phi i32 [ %9597, %9583 ]
  %9611 = phi i64 [ %9605, %9583 ]
  br label %9612

9612:                                             ; preds = %9608, %9560
  %9613 = phi i32 [ %9575, %9560 ], [ %9610, %9608 ]
  %9614 = phi i32 [ %9569, %9560 ], [ %9609, %9608 ]
  %9615 = phi i64 [ 0, %9560 ], [ %9611, %9608 ]
  %9616 = shl i32 %9613, 13
  %9617 = xor i32 %9616, %9613
  %9618 = lshr i32 %9617, 17
  %9619 = xor i32 %9618, %9617
  %9620 = shl i32 %9619, 5
  %9621 = xor i32 %9620, %9619
  %9622 = add nsw i32 %9614, 2
  %9623 = shl i32 %9621, 13
  %9624 = xor i32 %9623, %9621
  %9625 = lshr i32 %9624, 17
  %9626 = xor i32 %9625, %9624
  %9627 = shl i32 %9626, 5
  %9628 = xor i32 %9627, %9626
  store i32 %9628, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9629 = inttoptr i64 %9578 to ptr
  br i1 %9577, label %9630, label %9637

9630:                                             ; preds = %9612
  %9631 = sext i32 %9622 to i64
  %9632 = zext nneg i32 %9576 to i64
  br label %9726

9633:                                             ; preds = %9726
  %9634 = phi i32 [ %9732, %9726 ]
  %9635 = phi i32 [ %9740, %9726 ]
  %9636 = phi i64 [ %9748, %9726 ]
  br label %9637

9637:                                             ; preds = %9633, %9612
  %9638 = phi i32 [ %9628, %9612 ], [ %9635, %9633 ]
  %9639 = phi i32 [ %9622, %9612 ], [ %9634, %9633 ]
  %9640 = phi i64 [ 0, %9612 ], [ %9636, %9633 ]
  %9641 = sext i32 %9639 to i64
  %9642 = add nsw i64 %9641, 1
  %9643 = trunc nsw i64 %9642 to i32
  store i32 %9643, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9644 = getelementptr inbounds i8, ptr %9629, i64 %9641
  %9645 = load i8, ptr %9644, align 1, !tbaa !12
  %9646 = shl i32 %9638, 13
  %9647 = xor i32 %9646, %9638
  %9648 = lshr i32 %9647, 17
  %9649 = xor i32 %9648, %9647
  %9650 = shl i32 %9649, 5
  %9651 = xor i32 %9650, %9649
  store i32 %9651, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9652 = trunc i32 %9651 to i8
  %9653 = xor i8 %9645, %9652
  %9654 = zext i8 %9653 to i64
  %9655 = add nsw i64 %9641, 2
  %9656 = trunc nsw i64 %9655 to i32
  store i32 %9656, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9657 = getelementptr inbounds i8, ptr %9629, i64 %9642
  %9658 = load i8, ptr %9657, align 1, !tbaa !12
  %9659 = shl i32 %9651, 13
  %9660 = xor i32 %9659, %9651
  %9661 = lshr i32 %9660, 17
  %9662 = xor i32 %9661, %9660
  %9663 = shl i32 %9662, 5
  %9664 = xor i32 %9663, %9662
  store i32 %9664, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9665 = trunc i32 %9664 to i8
  %9666 = xor i8 %9658, %9665
  %9667 = zext i8 %9666 to i64
  %9668 = shl nuw nsw i64 %9667, 8
  %9669 = or disjoint i64 %9668, %9654
  %9670 = add nsw i64 %9641, 3
  %9671 = trunc nsw i64 %9670 to i32
  store i32 %9671, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9672 = getelementptr inbounds i8, ptr %9629, i64 %9655
  %9673 = load i8, ptr %9672, align 1, !tbaa !12
  %9674 = shl i32 %9664, 13
  %9675 = xor i32 %9674, %9664
  %9676 = lshr i32 %9675, 17
  %9677 = xor i32 %9676, %9675
  %9678 = shl i32 %9677, 5
  %9679 = xor i32 %9678, %9677
  store i32 %9679, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9680 = trunc i32 %9679 to i8
  %9681 = xor i8 %9673, %9680
  %9682 = zext i8 %9681 to i64
  %9683 = shl nuw nsw i64 %9682, 16
  %9684 = or disjoint i64 %9683, %9669
  %9685 = add i32 %9639, 4
  store i32 %9685, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9686 = getelementptr inbounds i8, ptr %9629, i64 %9670
  %9687 = load i8, ptr %9686, align 1, !tbaa !12
  %9688 = shl i32 %9679, 13
  %9689 = xor i32 %9688, %9679
  %9690 = lshr i32 %9689, 17
  %9691 = xor i32 %9690, %9689
  %9692 = shl i32 %9691, 5
  %9693 = xor i32 %9692, %9691
  store i32 %9693, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9694 = trunc i32 %9693 to i8
  %9695 = xor i8 %9687, %9694
  %9696 = zext i8 %9695 to i64
  %9697 = shl nuw nsw i64 %9696, 24
  %9698 = or disjoint i64 %9697, %9684
  %9699 = trunc nuw i64 %9698 to i32
  %9700 = add i32 %9639, 5
  store i32 %9700, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9701 = sext i32 %9685 to i64
  %9702 = getelementptr inbounds i8, ptr %9629, i64 %9701
  %9703 = load i8, ptr %9702, align 1, !tbaa !12
  %9704 = shl i32 %9693, 13
  %9705 = xor i32 %9704, %9693
  %9706 = lshr i32 %9705, 17
  %9707 = xor i32 %9706, %9705
  %9708 = shl i32 %9707, 5
  %9709 = xor i32 %9708, %9707
  store i32 %9709, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9710 = trunc i32 %9709 to i8
  %9711 = xor i8 %9703, %9710
  %9712 = add i32 %9639, 6
  store i32 %9712, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9713 = sext i32 %9700 to i64
  %9714 = getelementptr inbounds i8, ptr %9629, i64 %9713
  %9715 = load i8, ptr %9714, align 1, !tbaa !12
  %9716 = shl i32 %9709, 13
  %9717 = xor i32 %9716, %9709
  %9718 = lshr i32 %9717, 17
  %9719 = xor i32 %9718, %9717
  %9720 = shl i32 %9719, 5
  %9721 = xor i32 %9720, %9719
  store i32 %9721, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9722 = trunc i32 %9721 to i8
  %9723 = xor i8 %9715, %9722
  %9724 = zext i8 %9711 to i32
  %9725 = icmp ugt i8 %9711, 2
  br i1 %9725, label %9751, label %9756

9726:                                             ; preds = %9726, %9630
  %9727 = phi i64 [ %9631, %9630 ], [ %9731, %9726 ]
  %9728 = phi i64 [ 0, %9630 ], [ %9749, %9726 ]
  %9729 = phi i64 [ 0, %9630 ], [ %9748, %9726 ]
  %9730 = phi i32 [ %9628, %9630 ], [ %9740, %9726 ]
  %9731 = add nsw i64 %9727, 1
  %9732 = trunc nsw i64 %9731 to i32
  store i32 %9732, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9733 = getelementptr inbounds i8, ptr %9629, i64 %9727
  %9734 = load i8, ptr %9733, align 1, !tbaa !12
  %9735 = shl i32 %9730, 13
  %9736 = xor i32 %9735, %9730
  %9737 = lshr i32 %9736, 17
  %9738 = xor i32 %9737, %9736
  %9739 = shl i32 %9738, 5
  %9740 = xor i32 %9739, %9738
  store i32 %9740, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9741 = trunc i32 %9740 to i8
  %9742 = xor i8 %9734, %9741
  %9743 = icmp samesign ult i64 %9728, 8
  %9744 = zext i8 %9742 to i64
  %9745 = shl nuw nsw i64 %9728, 3
  %9746 = shl nuw i64 %9744, %9745
  %9747 = select i1 %9743, i64 %9746, i64 0
  %9748 = or i64 %9747, %9729
  %9749 = add nuw nsw i64 %9728, 1
  %9750 = icmp eq i64 %9749, %9632
  br i1 %9750, label %9633, label %9726, !llvm.loop !33

9751:                                             ; preds = %9637
  %9752 = add nsw i32 %9724, -3
  br label %9761

9753:                                             ; preds = %9761
  %9754 = phi i32 [ %9769, %9761 ]
  %9755 = add i32 %9685, %9724
  store i32 %9755, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  store i32 %9754, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  br label %9756

9756:                                             ; preds = %9753, %9637
  %9757 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %9758 = add i64 %9757, %9640
  %9759 = inttoptr i64 %9758 to ptr
  %9760 = load ptr, ptr %9759, align 8, !tbaa !54
  switch i32 %9699, label %9902 [
    i32 1, label %9772
    i32 2, label %9795
    i32 4, label %9818
    i32 8, label %9840
    i32 16, label %9878
  ]

9761:                                             ; preds = %9761, %9751
  %9762 = phi i32 [ %9770, %9761 ], [ 0, %9751 ]
  %9763 = phi i32 [ %9769, %9761 ], [ %9721, %9751 ]
  %9764 = shl i32 %9763, 13
  %9765 = xor i32 %9764, %9763
  %9766 = lshr i32 %9765, 17
  %9767 = xor i32 %9766, %9765
  %9768 = shl i32 %9767, 5
  %9769 = xor i32 %9768, %9767
  %9770 = add nuw nsw i32 %9762, 1
  %9771 = icmp eq i32 %9762, %9752
  br i1 %9771, label %9753, label %9761, !llvm.loop !79

9772:                                             ; preds = %9756
  %9773 = getelementptr inbounds nuw i8, ptr %9760, i64 24
  %9774 = load i32, ptr %9773, align 8
  %9775 = icmp sgt i32 %9774, -1
  br i1 %9775, label %9784, label %9776

9776:                                             ; preds = %9772
  %9777 = add nsw i32 %9774, 8
  store i32 %9777, ptr %9773, align 8
  %9778 = icmp samesign ult i32 %9774, -7
  br i1 %9778, label %9779, label %9784

9779:                                             ; preds = %9776
  %9780 = getelementptr inbounds nuw i8, ptr %9760, i64 8
  %9781 = load ptr, ptr %9780, align 8
  %9782 = sext i32 %9774 to i64
  %9783 = getelementptr inbounds i8, ptr %9781, i64 %9782
  br label %9788

9784:                                             ; preds = %9776, %9772
  %9785 = load ptr, ptr %9760, align 8
  %9786 = getelementptr inbounds nuw i8, ptr %9785, i64 8
  store ptr %9786, ptr %9760, align 8
  %9787 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  br label %9788

9788:                                             ; preds = %9784, %9779
  %9789 = phi i64 [ %9757, %9779 ], [ %9787, %9784 ]
  %9790 = phi ptr [ %9783, %9779 ], [ %9785, %9784 ]
  %9791 = load i32, ptr %9790, align 8, !tbaa !10
  %9792 = trunc i32 %9791 to i8
  %9793 = inttoptr i64 %9789 to ptr
  %9794 = getelementptr inbounds nuw i8, ptr %9793, i64 %9615
  store i8 %9792, ptr %9794, align 1
  br label %9924

9795:                                             ; preds = %9756
  %9796 = getelementptr inbounds nuw i8, ptr %9760, i64 24
  %9797 = load i32, ptr %9796, align 8
  %9798 = icmp sgt i32 %9797, -1
  br i1 %9798, label %9807, label %9799

9799:                                             ; preds = %9795
  %9800 = add nsw i32 %9797, 8
  store i32 %9800, ptr %9796, align 8
  %9801 = icmp samesign ult i32 %9797, -7
  br i1 %9801, label %9802, label %9807

9802:                                             ; preds = %9799
  %9803 = getelementptr inbounds nuw i8, ptr %9760, i64 8
  %9804 = load ptr, ptr %9803, align 8
  %9805 = sext i32 %9797 to i64
  %9806 = getelementptr inbounds i8, ptr %9804, i64 %9805
  br label %9811

9807:                                             ; preds = %9799, %9795
  %9808 = load ptr, ptr %9760, align 8
  %9809 = getelementptr inbounds nuw i8, ptr %9808, i64 8
  store ptr %9809, ptr %9760, align 8
  %9810 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  br label %9811

9811:                                             ; preds = %9807, %9802
  %9812 = phi i64 [ %9757, %9802 ], [ %9810, %9807 ]
  %9813 = phi ptr [ %9806, %9802 ], [ %9808, %9807 ]
  %9814 = load i32, ptr %9813, align 8, !tbaa !10
  %9815 = trunc i32 %9814 to i16
  %9816 = inttoptr i64 %9812 to ptr
  %9817 = getelementptr inbounds nuw i8, ptr %9816, i64 %9615
  store i16 %9815, ptr %9817, align 1
  br label %9924

9818:                                             ; preds = %9756
  %9819 = getelementptr inbounds nuw i8, ptr %9760, i64 24
  %9820 = load i32, ptr %9819, align 8
  %9821 = icmp sgt i32 %9820, -1
  br i1 %9821, label %9830, label %9822

9822:                                             ; preds = %9818
  %9823 = add nsw i32 %9820, 8
  store i32 %9823, ptr %9819, align 8
  %9824 = icmp samesign ult i32 %9820, -7
  br i1 %9824, label %9825, label %9830

9825:                                             ; preds = %9822
  %9826 = getelementptr inbounds nuw i8, ptr %9760, i64 8
  %9827 = load ptr, ptr %9826, align 8
  %9828 = sext i32 %9820 to i64
  %9829 = getelementptr inbounds i8, ptr %9827, i64 %9828
  br label %9834

9830:                                             ; preds = %9822, %9818
  %9831 = load ptr, ptr %9760, align 8
  %9832 = getelementptr inbounds nuw i8, ptr %9831, i64 8
  store ptr %9832, ptr %9760, align 8
  %9833 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  br label %9834

9834:                                             ; preds = %9830, %9825
  %9835 = phi i64 [ %9757, %9825 ], [ %9833, %9830 ]
  %9836 = phi ptr [ %9829, %9825 ], [ %9831, %9830 ]
  %9837 = load i32, ptr %9836, align 8, !tbaa !10
  %9838 = inttoptr i64 %9835 to ptr
  %9839 = getelementptr inbounds nuw i8, ptr %9838, i64 %9615
  store i32 %9837, ptr %9839, align 1
  br label %9924

9840:                                             ; preds = %9756
  %9841 = icmp eq i8 %9723, 4
  br i1 %9841, label %9842, label %9857

9842:                                             ; preds = %9840
  %9843 = getelementptr inbounds nuw i8, ptr %9760, i64 28
  %9844 = load i32, ptr %9843, align 4
  %9845 = icmp sgt i32 %9844, -1
  br i1 %9845, label %9854, label %9846

9846:                                             ; preds = %9842
  %9847 = add nsw i32 %9844, 16
  store i32 %9847, ptr %9843, align 4
  %9848 = icmp samesign ult i32 %9844, -15
  br i1 %9848, label %9849, label %9854

9849:                                             ; preds = %9846
  %9850 = getelementptr inbounds nuw i8, ptr %9760, i64 16
  %9851 = load ptr, ptr %9850, align 8
  %9852 = sext i32 %9844 to i64
  %9853 = getelementptr inbounds i8, ptr %9851, i64 %9852
  br label %9872

9854:                                             ; preds = %9846, %9842
  %9855 = load ptr, ptr %9760, align 8
  %9856 = getelementptr inbounds nuw i8, ptr %9855, i64 8
  store ptr %9856, ptr %9760, align 8
  br label %9872

9857:                                             ; preds = %9840
  %9858 = getelementptr inbounds nuw i8, ptr %9760, i64 24
  %9859 = load i32, ptr %9858, align 8
  %9860 = icmp sgt i32 %9859, -1
  br i1 %9860, label %9869, label %9861

9861:                                             ; preds = %9857
  %9862 = add nsw i32 %9859, 8
  store i32 %9862, ptr %9858, align 8
  %9863 = icmp samesign ult i32 %9859, -7
  br i1 %9863, label %9864, label %9869

9864:                                             ; preds = %9861
  %9865 = getelementptr inbounds nuw i8, ptr %9760, i64 8
  %9866 = load ptr, ptr %9865, align 8
  %9867 = sext i32 %9859 to i64
  %9868 = getelementptr inbounds i8, ptr %9866, i64 %9867
  br label %9872

9869:                                             ; preds = %9861, %9857
  %9870 = load ptr, ptr %9760, align 8
  %9871 = getelementptr inbounds nuw i8, ptr %9870, i64 8
  store ptr %9871, ptr %9760, align 8
  br label %9872

9872:                                             ; preds = %9869, %9864, %9854, %9849
  %9873 = phi ptr [ %9853, %9849 ], [ %9855, %9854 ], [ %9868, %9864 ], [ %9870, %9869 ]
  %9874 = load i64, ptr %9873, align 8, !tbaa !12
  %9875 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %9876 = inttoptr i64 %9875 to ptr
  %9877 = getelementptr inbounds nuw i8, ptr %9876, i64 %9615
  store i64 %9874, ptr %9877, align 1
  br label %9924

9878:                                             ; preds = %9756
  %9879 = getelementptr inbounds nuw i8, ptr %9760, i64 28
  %9880 = load i32, ptr %9879, align 4
  %9881 = icmp sgt i32 %9880, -1
  br i1 %9881, label %9890, label %9882

9882:                                             ; preds = %9878
  %9883 = add nsw i32 %9880, 16
  store i32 %9883, ptr %9879, align 4
  %9884 = icmp samesign ult i32 %9880, -15
  br i1 %9884, label %9885, label %9890

9885:                                             ; preds = %9882
  %9886 = getelementptr inbounds nuw i8, ptr %9760, i64 16
  %9887 = load ptr, ptr %9886, align 8
  %9888 = sext i32 %9880 to i64
  %9889 = getelementptr inbounds i8, ptr %9887, i64 %9888
  br label %9896

9890:                                             ; preds = %9882, %9878
  %9891 = load ptr, ptr %9760, align 8
  %9892 = getelementptr inbounds nuw i8, ptr %9891, i64 15
  %9893 = tail call align 16 ptr @llvm.ptrmask.p0.i64(ptr nonnull %9892, i64 -16)
  %9894 = getelementptr inbounds nuw i8, ptr %9893, i64 16
  store ptr %9894, ptr %9760, align 8
  %9895 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  br label %9896

9896:                                             ; preds = %9890, %9885
  %9897 = phi i64 [ %9757, %9885 ], [ %9895, %9890 ]
  %9898 = phi ptr [ %9889, %9885 ], [ %9893, %9890 ]
  %9899 = load fp128, ptr %9898, align 16, !tbaa !80
  %9900 = inttoptr i64 %9897 to ptr
  %9901 = getelementptr inbounds nuw i8, ptr %9900, i64 %9615
  store fp128 %9899, ptr %9901, align 1
  br label %9924

9902:                                             ; preds = %9756
  %9903 = getelementptr inbounds nuw i8, ptr %9760, i64 24
  %9904 = load i32, ptr %9903, align 8
  %9905 = icmp sgt i32 %9904, -1
  br i1 %9905, label %9914, label %9906

9906:                                             ; preds = %9902
  %9907 = add nsw i32 %9904, 8
  store i32 %9907, ptr %9903, align 8
  %9908 = icmp samesign ult i32 %9904, -7
  br i1 %9908, label %9909, label %9914

9909:                                             ; preds = %9906
  %9910 = getelementptr inbounds nuw i8, ptr %9760, i64 8
  %9911 = load ptr, ptr %9910, align 8
  %9912 = sext i32 %9904 to i64
  %9913 = getelementptr inbounds i8, ptr %9911, i64 %9912
  br label %9918

9914:                                             ; preds = %9906, %9902
  %9915 = load ptr, ptr %9760, align 8
  %9916 = getelementptr inbounds nuw i8, ptr %9915, i64 8
  store ptr %9916, ptr %9760, align 8
  %9917 = load i64, ptr @data_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  br label %9918

9918:                                             ; preds = %9914, %9909
  %9919 = phi i64 [ %9757, %9909 ], [ %9917, %9914 ]
  %9920 = phi ptr [ %9913, %9909 ], [ %9915, %9914 ]
  %9921 = load ptr, ptr %9920, align 8, !tbaa !54
  %9922 = inttoptr i64 %9919 to ptr
  %9923 = getelementptr inbounds nuw i8, ptr %9922, i64 %9615
  tail call void @llvm.memcpy.p0.p0.i64(ptr align 1 %9923, ptr align 1 %9921, i64 %9698, i1 false)
  br label %9924

9924:                                             ; preds = %9918, %9896, %9872, %9834, %9811, %9788
  store ptr %9760, ptr %9759, align 8, !tbaa !54
  br label %2370

9925:                                             ; preds = %239
  %9926 = phi i8 [ %215, %239 ]
  %9927 = load i64, ptr @vm_trace_total__Z8vm_entryv.29, align 8, !tbaa !6
  %9928 = load i32, ptr @vm_trace_next__Z8vm_entryv.28, align 4
  %9929 = load i32, ptr @ip__Z8vm_entryv, align 4, !tbaa !10
  %9930 = tail call i32 @llvm.smax.i32(i32 %9929, i32 1)
  %9931 = add nsw i32 %9930, -1
  %9932 = zext i8 %9926 to i64
  %9933 = load i64, ptr @current_bb_id__Z8vm_entryv.3, align 8, !tbaa !6
  %9934 = load i64, ptr @last_br_from_bb_id__Z8vm_entryv.2, align 8, !tbaa !6
  %9935 = load i8, ptr @exception_thrown__Z8vm_entryv, align 4, !tbaa !12
  %9936 = zext i8 %9935 to i32
  %9937 = load i32, ptr @exception_selector__Z8vm_entryv, align 4, !tbaa !10
  %9938 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.20__Z8vm_entryv.49, i64 noundef %9932, i64 noundef %9933, i32 noundef %9931, i64 noundef %9933, i64 noundef %9934, i32 noundef %9936, i32 noundef %9937)
  %9939 = load i32, ptr @opcode_xorshift32_state__Z8vm_entryv, align 4, !tbaa !10
  %9940 = load i32, ptr @vm_code_state__Z8vm_entryv, align 4, !tbaa !10
  %9941 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.1__Z8vm_entryv.31, i32 noundef %9939, i32 noundef %9940)
  %9942 = icmp sgt i32 %9929, 9
  %9943 = add nsw i32 %9930, -9
  %9944 = select i1 %9942, i32 %9943, i32 0
  br label %9962

9945:                                             ; preds = %9962
  %9946 = add i32 %9944, 23
  %9947 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %9948 = inttoptr i64 %9947 to ptr
  %9949 = zext i32 %9946 to i64
  %9950 = getelementptr inbounds nuw i8, ptr %9948, i64 %9949
  %9951 = load i8, ptr %9950, align 1, !tbaa !12
  %9952 = zext i8 %9951 to i32
  %9953 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.2__Z8vm_entryv.32, i32 noundef %9952)
  %9954 = zext i8 %9926 to i32
  %9955 = tail call i64 @llvm.umin.i64(i64 %9927, i64 32)
  %9956 = icmp ugt i64 %9927, 32
  %9957 = zext i32 %9928 to i64
  %9958 = select i1 %9956, i64 %9957, i64 32
  %9959 = tail call i32 @putchar(i32 10)
  %9960 = icmp eq i64 %9927, 0
  br i1 %9960, label %9977, label %9961

9961:                                             ; preds = %9945
  br label %9979

9962:                                             ; preds = %9962, %9925
  %9963 = phi i64 [ 0, %9925 ], [ %9973, %9962 ]
  %9964 = trunc nuw nsw i64 %9963 to i32
  %9965 = add i32 %9944, %9964
  %9966 = load i64, ptr @code_seg_addr__Z8vm_entryv, align 8, !tbaa !6
  %9967 = inttoptr i64 %9966 to ptr
  %9968 = zext i32 %9965 to i64
  %9969 = getelementptr inbounds nuw i8, ptr %9967, i64 %9968
  %9970 = load i8, ptr %9969, align 1, !tbaa !12
  %9971 = zext i8 %9970 to i32
  %9972 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.2__Z8vm_entryv.32, i32 noundef %9971)
  %9973 = add nuw nsw i64 %9963, 1
  %9974 = tail call i32 @putchar(i32 32)
  %9975 = icmp eq i64 %9973, 23
  br i1 %9975, label %9945, label %9962, !llvm.loop !82

9976:                                             ; preds = %10041
  br label %9977

9977:                                             ; preds = %9976, %9945
  %9978 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.21__Z8vm_entryv.50, i32 noundef %9954)
  br label %10044

9979:                                             ; preds = %9961, %10041
  %9980 = phi i64 [ %10042, %10041 ], [ 0, %9961 ]
  %9981 = add nuw nsw i64 %9980, %9958
  %9982 = and i64 %9981, 31
  %9983 = getelementptr inbounds nuw [32 x %struct.VMTraceEntry], ptr @vm_trace_ring__Z8vm_entryv.33, i64 0, i64 %9982
  %9984 = load i8, ptr %9983, align 8, !tbaa !16
  switch i8 %9984, label %10041 [
    i8 1, label %9985
    i8 2, label %9993
    i8 3, label %10008
    i8 4, label %10024
  ]

9985:                                             ; preds = %9979
  %9986 = getelementptr inbounds nuw i8, ptr %9983, i64 4
  %9987 = load i32, ptr %9986, align 4, !tbaa !18
  %9988 = getelementptr inbounds nuw i8, ptr %9983, i64 8
  %9989 = load i64, ptr %9988, align 8, !tbaa !19
  %9990 = getelementptr inbounds nuw i8, ptr %9983, i64 16
  %9991 = load i64, ptr %9990, align 8, !tbaa !20
  %9992 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.5__Z8vm_entryv.34, i32 noundef %9987, i64 noundef %9989, i64 noundef %9991)
  br label %10041

9993:                                             ; preds = %9979
  %9994 = getelementptr inbounds nuw i8, ptr %9983, i64 4
  %9995 = load i32, ptr %9994, align 4, !tbaa !18
  %9996 = getelementptr inbounds nuw i8, ptr %9983, i64 1
  %9997 = load i8, ptr %9996, align 1, !tbaa !21
  %9998 = zext i8 %9997 to i32
  %9999 = getelementptr inbounds nuw i8, ptr %9983, i64 8
  %10000 = load i64, ptr %9999, align 8, !tbaa !19
  %10001 = getelementptr inbounds nuw i8, ptr %9983, i64 32
  %10002 = load i64, ptr %10001, align 8, !tbaa !22
  %10003 = getelementptr inbounds nuw i8, ptr %9983, i64 16
  %10004 = load i64, ptr %10003, align 8, !tbaa !20
  %10005 = getelementptr inbounds nuw i8, ptr %9983, i64 24
  %10006 = load i64, ptr %10005, align 8, !tbaa !23
  %10007 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.6__Z8vm_entryv.35, i32 noundef %9995, i32 noundef %9998, i64 noundef %10000, i64 noundef %10002, i64 noundef %10004, i64 noundef %10006)
  br label %10041

10008:                                            ; preds = %9979
  %10009 = getelementptr inbounds nuw i8, ptr %9983, i64 8
  %10010 = load i64, ptr %10009, align 8, !tbaa !19
  %10011 = getelementptr inbounds nuw i8, ptr %9983, i64 16
  %10012 = load i64, ptr %10011, align 8, !tbaa !20
  %10013 = getelementptr inbounds nuw i8, ptr %9983, i64 1
  %10014 = load i8, ptr %10013, align 1, !tbaa !21
  %10015 = zext i8 %10014 to i32
  %10016 = getelementptr inbounds nuw i8, ptr %9983, i64 2
  %10017 = load i8, ptr %10016, align 2, !tbaa !24
  %10018 = zext i8 %10017 to i32
  %10019 = getelementptr inbounds nuw i8, ptr %9983, i64 24
  %10020 = load i64, ptr %10019, align 8, !tbaa !23
  %10021 = getelementptr inbounds nuw i8, ptr %9983, i64 32
  %10022 = load i64, ptr %10021, align 8, !tbaa !22
  %10023 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.7__Z8vm_entryv.36, i64 noundef %10010, i64 noundef %10012, i32 noundef %10015, i32 noundef %10018, i64 noundef %10020, i64 noundef %10022)
  br label %10041

10024:                                            ; preds = %9979
  %10025 = getelementptr inbounds nuw i8, ptr %9983, i64 1
  %10026 = load i8, ptr %10025, align 1, !tbaa !21
  %10027 = icmp eq i8 %10026, 0
  %10028 = select i1 %10027, ptr @.str.10__Z8vm_entryv.39, ptr @.str.9__Z8vm_entryv.38
  %10029 = getelementptr inbounds nuw i8, ptr %9983, i64 8
  %10030 = load i64, ptr %10029, align 8, !tbaa !19
  %10031 = getelementptr inbounds nuw i8, ptr %9983, i64 16
  %10032 = load i64, ptr %10031, align 8, !tbaa !20
  %10033 = getelementptr inbounds nuw i8, ptr %9983, i64 24
  %10034 = load i64, ptr %10033, align 8, !tbaa !23
  %10035 = getelementptr inbounds nuw i8, ptr %9983, i64 2
  %10036 = load i8, ptr %10035, align 2, !tbaa !24
  %10037 = zext i8 %10036 to i32
  %10038 = getelementptr inbounds nuw i8, ptr %9983, i64 32
  %10039 = load i64, ptr %10038, align 8, !tbaa !22
  %10040 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.8__Z8vm_entryv.37, ptr noundef nonnull %10028, i64 noundef %10030, i64 noundef %10032, i64 noundef %10034, i32 noundef %10037, i64 noundef %10039)
  br label %10041

10041:                                            ; preds = %10024, %10008, %9993, %9985, %9979
  %10042 = add nuw nsw i64 %9980, 1
  %10043 = icmp eq i64 %10042, %9955
  br i1 %10043, label %9976, label %9979, !llvm.loop !83

10044:                                            ; preds = %9977, %5897
  %10045 = tail call i32 @fflush(ptr noundef null)
  br label %10047

10046:                                            ; preds = %8561
  br label %10047

10047:                                            ; preds = %10046, %10044, %8551, %5890
  ret void
}

attributes #0 = { mustprogress noinline optnone uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #1 = { noreturn "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #2 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #3 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #5 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #6 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #7 = { noreturn nounwind uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #8 = { nofree nounwind uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #9 = { nounwind uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fix-cortex-a53-835769,+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
attributes #10 = { noreturn nounwind }
attributes #11 = { nounwind }
attributes #12 = { allocsize(0) }
attributes #13 = { nounwind willreturn memory(read) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}
!llvm.dbg.cu = !{}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"\0AA-protector\0ABy abcdefgjh.\0A\E5\AF\92\E5\A4\9C\E6\BC\AB\E6\BC\AB\EF\BC\8C\E5\AD\A4\E7\81\AF\E7\85\A7\E5\BD\B1\E6\97\A0\E7\9C\A0\E3\80\82\E5\BE\80\E4\BA\8B\E5\8E\86\E5\8E\86\EF\BC\8C\E6\95\85\E4\BA\BA\E5\A6\82\E6\A2\A6\EF\BC\8C\E9\86\92\E6\9D\A5\E6\88\90\E7\A9\BA\E3\80\82\0A"}
!6 = !{!7, !7, i64 0}
!7 = !{!"long", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C/C++ TBAA"}
!10 = !{!11, !11, i64 0}
!11 = !{!"int", !8, i64 0}
!12 = !{!8, !8, i64 0}
!13 = distinct !{!13, !14, !15}
!14 = !{!"llvm.loop.mustprogress"}
!15 = !{!"llvm.loop.peeled.count", i32 1}
!16 = !{!17, !8, i64 0}
!17 = !{!"", !8, i64 0, !8, i64 1, !8, i64 2, !8, i64 3, !11, i64 4, !7, i64 8, !7, i64 16, !7, i64 24, !7, i64 32}
!18 = !{!17, !11, i64 4}
!19 = !{!17, !7, i64 8}
!20 = !{!17, !7, i64 16}
!21 = !{!17, !8, i64 1}
!22 = !{!17, !7, i64 32}
!23 = !{!17, !7, i64 24}
!24 = !{!17, !8, i64 2}
!25 = distinct !{!25, !14}
!26 = distinct !{!26, !14, !27, !28}
!27 = !{!"llvm.loop.isvectorized", i32 1}
!28 = !{!"llvm.loop.unroll.runtime.disable"}
!29 = distinct !{!29, !14, !27, !28}
!30 = distinct !{!30, !14, !28, !27}
!31 = distinct !{!31, !14}
!32 = !{!17, !8, i64 3}
!33 = distinct !{!33, !14}
!34 = distinct !{!34, !14}
!35 = distinct !{!35, !14}
!36 = distinct !{!36, !14}
!37 = distinct !{!37, !14}
!38 = distinct !{!38, !14}
!39 = distinct !{!39, !14, !27, !28}
!40 = distinct !{!40, !14, !28, !27}
!41 = distinct !{!41, !14, !15, !27, !28}
!42 = distinct !{!42, !14, !15, !27, !28}
!43 = distinct !{!43, !14, !15, !28, !27}
!44 = distinct !{!44, !14}
!45 = distinct !{!45, !14}
!46 = distinct !{!46, !14}
!47 = distinct !{!47, !14}
!48 = distinct !{!48, !14, !27, !28}
!49 = distinct !{!49, !14, !27, !28}
!50 = distinct !{!50, !14, !27}
!51 = distinct !{!51, !14, !28, !27}
!52 = distinct !{!52, !14}
!53 = distinct !{!53, !14}
!54 = !{!55, !55, i64 0}
!55 = !{!"any pointer", !8, i64 0}
!56 = distinct !{!56, !14}
!57 = distinct !{!57, !14}
!58 = distinct !{!58, !14}
!59 = distinct !{!59, !14, !27, !28}
!60 = distinct !{!60, !14, !27, !28}
!61 = distinct !{!61, !14, !27}
!62 = distinct !{!62, !14, !27, !28}
!63 = distinct !{!63, !14, !27, !28}
!64 = distinct !{!64, !14, !27, !28}
!65 = distinct !{!65, !14, !27, !28}
!66 = distinct !{!66, !14, !27}
!67 = distinct !{!67, !14, !27}
!68 = distinct !{!68, !14}
!69 = !{!70, !55, i64 16}
!70 = !{!"", !55, i64 0, !7, i64 8, !55, i64 16, !55, i64 24, !55, i64 32, !55, i64 40, !55, i64 48, !11, i64 56, !11, i64 60, !71, i64 64, !71, i64 72, !55, i64 80, !55, i64 88, !8, i64 96}
!71 = !{!"p1 omnipotent char", !55, i64 0}
!72 = !{!73, !71, i64 8}
!73 = !{!"", !55, i64 0, !71, i64 8}
!74 = !{!75, !55, i64 16}
!75 = !{!"", !55, i64 0, !71, i64 8, !55, i64 16}
!76 = distinct !{!76, !14}
!77 = !{!78, !78, i64 0}
!78 = !{!"short", !8, i64 0}
!79 = distinct !{!79, !14}
!80 = !{!81, !81, i64 0}
!81 = !{!"long double", !8, i64 0}
!82 = distinct !{!82, !14, !15}
!83 = distinct !{!83, !14}
