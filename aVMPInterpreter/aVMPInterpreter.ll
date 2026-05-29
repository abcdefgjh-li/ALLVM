; ModuleID = '.\aVMPInterpreter\aVMPInterpreter.bc'
source_filename = "D:\\cpp\\obfuscator-ollvm-21.x\\aVMPInterpreter\\aVMPInterpreter.c"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.50.35728"

@gv_code_seg = dso_local local_unnamed_addr global [5000 x i8] zeroinitializer, align 16
@gv_data_seg = dso_local local_unnamed_addr global [5000 x i8] zeroinitializer, align 16
@pointer_size = external dso_local local_unnamed_addr global i32, align 4
@ip = external dso_local local_unnamed_addr global i32, align 4
@opcode_xorshift32_state = external dso_local local_unnamed_addr global i32, align 4
@vm_code_state = external dso_local local_unnamed_addr global i32, align 4
@current_bb_id = dso_local local_unnamed_addr global i64 0, align 8
@exception_thrown = dso_local local_unnamed_addr global i8 0, align 1
@exception_ptr = dso_local local_unnamed_addr global ptr null, align 8
@exception_selector = dso_local local_unnamed_addr global i32 0, align 4
@last_br_from_bb_id = dso_local local_unnamed_addr global i64 0, align 8
@code_seg_addr = external dso_local local_unnamed_addr global i64, align 8
@data_seg_addr = external dso_local local_unnamed_addr global i64, align 8

; Function Attrs: nounwind uwtable
define dso_local void @vm_interpreter() local_unnamed_addr #0 {
  %1 = alloca [23 x i8], align 16
  store i32 8, ptr @pointer_size, align 4
  br label %2

2:                                                ; preds = %8784, %0
  %3 = phi i32 [ 0, %0 ], [ %8786, %8784 ]
  %4 = phi i8 [ 1, %0 ], [ %8785, %8784 ]
  %5 = icmp eq i8 %4, 0
  %6 = load i64, ptr @code_seg_addr, align 8
  br i1 %5, label %7, label %12

7:                                                ; preds = %2
  %8 = load i32, ptr @vm_code_state, align 4
  %9 = load i32, ptr @opcode_xorshift32_state, align 4
  %10 = inttoptr i64 %6 to ptr
  %11 = sext i32 %3 to i64
  br label %67

12:                                               ; preds = %2
  %13 = inttoptr i64 %6 to ptr
  %14 = sext i32 %3 to i64
  %15 = add nsw i64 %14, 1
  %16 = trunc nsw i64 %15 to i32
  store i32 %16, ptr @ip, align 4
  %17 = getelementptr inbounds i8, ptr %13, i64 %14
  %18 = load i8, ptr %17, align 1
  %19 = zext i8 %18 to i32
  %20 = add nsw i64 %14, 2
  %21 = trunc nsw i64 %20 to i32
  store i32 %21, ptr @ip, align 4
  %22 = getelementptr inbounds i8, ptr %13, i64 %15
  %23 = load i8, ptr %22, align 1
  %24 = zext i8 %23 to i32
  %25 = shl nuw nsw i32 %24, 8
  %26 = or disjoint i32 %25, %19
  %27 = add nsw i64 %14, 3
  %28 = trunc nsw i64 %27 to i32
  store i32 %28, ptr @ip, align 4
  %29 = getelementptr inbounds i8, ptr %13, i64 %20
  %30 = load i8, ptr %29, align 1
  %31 = zext i8 %30 to i32
  %32 = shl nuw nsw i32 %31, 16
  %33 = or disjoint i32 %32, %26
  %34 = add i32 %3, 4
  store i32 %34, ptr @ip, align 4
  %35 = getelementptr inbounds i8, ptr %13, i64 %27
  %36 = load i8, ptr %35, align 1
  %37 = zext i8 %36 to i32
  %38 = shl nuw i32 %37, 24
  %39 = or disjoint i32 %38, %33
  store i32 %39, ptr @opcode_xorshift32_state, align 4
  %40 = sext i32 %34 to i64
  %41 = add nsw i64 %40, 1
  %42 = trunc nsw i64 %41 to i32
  store i32 %42, ptr @ip, align 4
  %43 = getelementptr inbounds i8, ptr %13, i64 %40
  %44 = load i8, ptr %43, align 1
  %45 = zext i8 %44 to i32
  %46 = add nsw i64 %40, 2
  %47 = trunc nsw i64 %46 to i32
  store i32 %47, ptr @ip, align 4
  %48 = getelementptr inbounds i8, ptr %13, i64 %41
  %49 = load i8, ptr %48, align 1
  %50 = zext i8 %49 to i32
  %51 = shl nuw nsw i32 %50, 8
  %52 = or disjoint i32 %51, %45
  %53 = add nsw i64 %40, 3
  %54 = trunc nsw i64 %53 to i32
  store i32 %54, ptr @ip, align 4
  %55 = getelementptr inbounds i8, ptr %13, i64 %46
  %56 = load i8, ptr %55, align 1
  %57 = zext i8 %56 to i32
  %58 = shl nuw nsw i32 %57, 16
  %59 = or disjoint i32 %58, %52
  %60 = add i32 %3, 8
  store i32 %60, ptr @ip, align 4
  %61 = getelementptr inbounds i8, ptr %13, i64 %53
  %62 = load i8, ptr %61, align 1
  %63 = zext i8 %62 to i32
  %64 = shl nuw i32 %63, 24
  %65 = or disjoint i32 %64, %59
  store i32 %65, ptr @vm_code_state, align 4
  %66 = sext i32 %60 to i64
  store i64 %66, ptr @current_bb_id, align 8
  br label %67

67:                                               ; preds = %7, %12
  %68 = phi i64 [ %11, %7 ], [ %66, %12 ]
  %69 = phi ptr [ %10, %7 ], [ %13, %12 ]
  %70 = phi i32 [ %9, %7 ], [ %39, %12 ]
  %71 = phi i32 [ %8, %7 ], [ %65, %12 ]
  %72 = phi i32 [ %3, %7 ], [ %60, %12 ]
  call void @llvm.lifetime.start.p0(i64 23, ptr nonnull %1) #4
  %73 = add nsw i32 %72, 1
  store i32 %73, ptr @ip, align 4
  %74 = getelementptr inbounds i8, ptr %69, i64 %68
  %75 = load i8, ptr %74, align 1
  %76 = shl i32 %71, 13
  %77 = xor i32 %76, %71
  %78 = lshr i32 %77, 17
  %79 = xor i32 %78, %77
  %80 = shl i32 %79, 5
  %81 = xor i32 %80, %79
  store i32 %81, ptr @vm_code_state, align 4
  %82 = trunc i32 %81 to i8
  %83 = xor i8 %75, %82
  br label %85

84:                                               ; preds = %176
  store i32 %93, ptr @opcode_xorshift32_state, align 4
  call void @llvm.lifetime.end.p0(i64 23, ptr nonnull %1) #4
  br label %8800

85:                                               ; preds = %67, %176
  %86 = phi i32 [ 0, %67 ], [ %177, %176 ]
  %87 = phi i32 [ %70, %67 ], [ %93, %176 ]
  %88 = shl i32 %87, 13
  %89 = xor i32 %88, %87
  %90 = lshr i32 %89, 17
  %91 = xor i32 %90, %89
  %92 = shl i32 %91, 5
  %93 = xor i32 %92, %91
  %94 = trunc i32 %93 to i8
  %95 = icmp eq i8 %83, %94
  br i1 %95, label %158, label %96

96:                                               ; preds = %85
  %97 = icmp sgt i32 %86, 0
  br i1 %97, label %98, label %172

98:                                               ; preds = %96
  %99 = zext nneg i32 %86 to i64
  %100 = icmp ult i32 %86, 4
  br i1 %100, label %101, label %104

101:                                              ; preds = %131, %153, %98
  %102 = phi i64 [ 0, %98 ], [ %107, %131 ], [ %138, %153 ]
  %103 = phi i8 [ 1, %98 ], [ %129, %131 ], [ %156, %153 ]
  br label %163

104:                                              ; preds = %98
  %105 = icmp ult i32 %86, 32
  br i1 %105, label %134, label %106

106:                                              ; preds = %104
  %107 = and i64 %99, 2147483616
  %108 = insertelement <16 x i8> poison, i8 %94, i64 0
  %109 = shufflevector <16 x i8> %108, <16 x i8> poison, <16 x i32> zeroinitializer
  br label %110

110:                                              ; preds = %110, %106
  %111 = phi i64 [ 0, %106 ], [ %122, %110 ]
  %112 = phi <16 x i1> [ zeroinitializer, %106 ], [ %120, %110 ]
  %113 = phi <16 x i1> [ zeroinitializer, %106 ], [ %121, %110 ]
  %114 = getelementptr inbounds nuw [23 x i8], ptr %1, i64 0, i64 %111
  %115 = getelementptr inbounds nuw i8, ptr %114, i64 16
  %116 = load <16 x i8>, ptr %114, align 16
  %117 = load <16 x i8>, ptr %115, align 16
  %118 = icmp eq <16 x i8> %116, %109
  %119 = icmp eq <16 x i8> %117, %109
  %120 = or <16 x i1> %112, %118
  %121 = or <16 x i1> %113, %119
  %122 = add nuw i64 %111, 32
  %123 = icmp eq i64 %122, %107
  br i1 %123, label %124, label %110, !llvm.loop !8

124:                                              ; preds = %110
  %125 = or <16 x i1> %121, %120
  %126 = freeze <16 x i1> %125
  %127 = bitcast <16 x i1> %126 to i16
  %128 = icmp eq i16 %127, 0
  %129 = zext i1 %128 to i8
  %130 = icmp eq i64 %107, %99
  br i1 %130, label %160, label %131

131:                                              ; preds = %124
  %132 = and i64 %99, 28
  %133 = icmp eq i64 %132, 0
  br i1 %133, label %101, label %134

134:                                              ; preds = %131, %104
  %135 = phi i64 [ %107, %131 ], [ 0, %104 ]
  %136 = phi i8 [ %129, %131 ], [ 1, %104 ]
  %137 = icmp eq i8 %136, 0
  %138 = and i64 %99, 2147483644
  %139 = insertelement <4 x i8> poison, i8 %94, i64 0
  %140 = shufflevector <4 x i8> %139, <4 x i8> poison, <4 x i32> zeroinitializer
  %141 = insertelement <4 x i1> poison, i1 %137, i64 0
  %142 = shufflevector <4 x i1> %141, <4 x i1> poison, <4 x i32> zeroinitializer
  br label %143

143:                                              ; preds = %143, %134
  %144 = phi i64 [ %135, %134 ], [ %151, %143 ]
  %145 = phi <4 x i1> [ %142, %134 ], [ %150, %143 ]
  %146 = getelementptr inbounds nuw [23 x i8], ptr %1, i64 0, i64 %144
  %147 = load <4 x i8>, ptr %146, align 4
  %148 = icmp eq <4 x i8> %147, %140
  %149 = or <4 x i1> %145, %148
  %150 = freeze <4 x i1> %149
  %151 = add nuw i64 %144, 4
  %152 = icmp eq i64 %151, %138
  br i1 %152, label %153, label %143, !llvm.loop !12

153:                                              ; preds = %143
  %154 = bitcast <4 x i1> %150 to i4
  %155 = icmp eq i4 %154, 0
  %156 = zext i1 %155 to i8
  %157 = icmp eq i64 %138, %99
  br i1 %157, label %160, label %101

158:                                              ; preds = %85
  store i32 %93, ptr @opcode_xorshift32_state, align 4
  %159 = trunc i32 %86 to i8
  call void @llvm.lifetime.end.p0(i64 23, ptr nonnull %1) #4
  switch i8 %159, label %8800 [
    i8 -1, label %8784
    i8 0, label %179
    i8 1, label %297
    i8 2, label %561
    i8 3, label %869
    i8 4, label %1533
    i8 5, label %1962
    i8 6, label %2527
    i8 7, label %2856
    i8 10, label %3147
    i8 11, label %3622
    i8 12, label %4356
    i8 13, label %4881
    i8 14, label %5229
    i8 15, label %5786
    i8 8, label %8584
    i8 17, label %5903
    i8 18, label %6059
    i8 19, label %6591
    i8 20, label %7340
    i8 21, label %8144
    i8 9, label %8387
  ]

160:                                              ; preds = %163, %153, %124
  %161 = phi i8 [ %129, %124 ], [ %156, %153 ], [ %169, %163 ]
  %162 = icmp eq i8 %161, 0
  br i1 %162, label %176, label %172

163:                                              ; preds = %101, %163
  %164 = phi i64 [ %170, %163 ], [ %102, %101 ]
  %165 = phi i8 [ %169, %163 ], [ %103, %101 ]
  %166 = getelementptr inbounds nuw [23 x i8], ptr %1, i64 0, i64 %164
  %167 = load i8, ptr %166, align 1
  %168 = icmp eq i8 %167, %94
  %169 = select i1 %168, i8 0, i8 %165
  %170 = add nuw nsw i64 %164, 1
  %171 = icmp eq i64 %170, %99
  br i1 %171, label %160, label %163, !llvm.loop !13

172:                                              ; preds = %96, %160
  %173 = sext i32 %86 to i64
  %174 = getelementptr inbounds [23 x i8], ptr %1, i64 0, i64 %173
  store i8 %94, ptr %174, align 1
  %175 = add nsw i32 %86, 1
  br label %176

176:                                              ; preds = %172, %160
  %177 = phi i32 [ %175, %172 ], [ %86, %160 ]
  %178 = icmp sgt i32 %177, 22
  br i1 %178, label %84, label %85, !llvm.loop !14

179:                                              ; preds = %158
  %180 = add nsw i32 %72, 2
  store i32 %180, ptr @ip, align 4
  %181 = sext i32 %73 to i64
  %182 = getelementptr inbounds i8, ptr %69, i64 %181
  %183 = load i8, ptr %182, align 1
  %184 = shl i32 %81, 13
  %185 = xor i32 %184, %81
  %186 = lshr i32 %185, 17
  %187 = xor i32 %186, %185
  %188 = shl i32 %187, 5
  %189 = xor i32 %188, %187
  %190 = trunc i32 %189 to i8
  %191 = xor i8 %183, %190
  %192 = add nsw i32 %72, 3
  store i32 %192, ptr @ip, align 4
  %193 = shl i32 %189, 13
  %194 = xor i32 %193, %189
  %195 = lshr i32 %194, 17
  %196 = xor i32 %195, %194
  %197 = shl i32 %196, 5
  %198 = xor i32 %197, %196
  store i32 %198, ptr @vm_code_state, align 4
  %199 = load i32, ptr @pointer_size, align 4
  %200 = icmp sgt i32 %199, 0
  br i1 %200, label %201, label %254

201:                                              ; preds = %179
  %202 = sext i32 %192 to i64
  %203 = zext nneg i32 %199 to i64
  br label %208

204:                                              ; preds = %208
  %205 = shl i64 %213, 32
  %206 = ashr exact i64 %205, 32
  %207 = zext nneg i32 %199 to i64
  br label %231

208:                                              ; preds = %201, %208
  %209 = phi i64 [ %202, %201 ], [ %213, %208 ]
  %210 = phi i64 [ 0, %201 ], [ %229, %208 ]
  %211 = phi i64 [ 0, %201 ], [ %228, %208 ]
  %212 = phi i32 [ %198, %201 ], [ %222, %208 ]
  %213 = add nsw i64 %209, 1
  %214 = trunc nsw i64 %213 to i32
  store i32 %214, ptr @ip, align 4
  %215 = getelementptr inbounds i8, ptr %69, i64 %209
  %216 = load i8, ptr %215, align 1
  %217 = shl i32 %212, 13
  %218 = xor i32 %217, %212
  %219 = lshr i32 %218, 17
  %220 = xor i32 %219, %218
  %221 = shl i32 %220, 5
  %222 = xor i32 %221, %220
  store i32 %222, ptr @vm_code_state, align 4
  %223 = trunc i32 %222 to i8
  %224 = xor i8 %216, %223
  %225 = zext i8 %224 to i64
  %226 = shl nsw i64 %210, 3
  %227 = shl i64 %225, %226
  %228 = or i64 %227, %211
  %229 = add nuw nsw i64 %210, 1
  %230 = icmp eq i64 %229, %203
  br i1 %230, label %204, label %208, !llvm.loop !15

231:                                              ; preds = %204, %231
  %232 = phi i64 [ %206, %204 ], [ %236, %231 ]
  %233 = phi i64 [ 0, %204 ], [ %252, %231 ]
  %234 = phi i64 [ 0, %204 ], [ %251, %231 ]
  %235 = phi i32 [ %222, %204 ], [ %245, %231 ]
  %236 = add nsw i64 %232, 1
  %237 = trunc nsw i64 %236 to i32
  store i32 %237, ptr @ip, align 4
  %238 = getelementptr inbounds i8, ptr %69, i64 %232
  %239 = load i8, ptr %238, align 1
  %240 = shl i32 %235, 13
  %241 = xor i32 %240, %235
  %242 = lshr i32 %241, 17
  %243 = xor i32 %242, %241
  %244 = shl i32 %243, 5
  %245 = xor i32 %244, %243
  store i32 %245, ptr @vm_code_state, align 4
  %246 = trunc i32 %245 to i8
  %247 = xor i8 %239, %246
  %248 = zext i8 %247 to i64
  %249 = shl nsw i64 %233, 3
  %250 = shl i64 %248, %249
  %251 = or i64 %250, %234
  %252 = add nuw nsw i64 %233, 1
  %253 = icmp eq i64 %252, %207
  br i1 %253, label %254, label %231, !llvm.loop !15

254:                                              ; preds = %231, %179
  %255 = phi i64 [ 0, %179 ], [ %228, %231 ]
  %256 = phi i64 [ 0, %179 ], [ %251, %231 ]
  %257 = icmp eq i8 %183, %190
  br i1 %257, label %8784, label %258

258:                                              ; preds = %254
  %259 = load i64, ptr @data_seg_addr, align 8
  %260 = add i64 %259, %255
  %261 = inttoptr i64 %260 to ptr
  %262 = add i64 %259, %256
  %263 = zext i8 %191 to i32
  %264 = and i32 %263, 7
  %265 = icmp ult i8 %191, 8
  br i1 %265, label %8615, label %266

266:                                              ; preds = %258
  %267 = and i32 %263, 248
  br label %268

268:                                              ; preds = %268, %266
  %269 = phi ptr [ %261, %266 ], [ %294, %268 ]
  %270 = phi i64 [ %262, %266 ], [ 0, %268 ]
  %271 = phi i32 [ 0, %266 ], [ %295, %268 ]
  %272 = trunc i64 %270 to i8
  store i8 %272, ptr %269, align 1
  %273 = getelementptr inbounds nuw i8, ptr %269, i64 1
  %274 = lshr i64 %270, 8
  %275 = trunc i64 %274 to i8
  store i8 %275, ptr %273, align 1
  %276 = getelementptr inbounds nuw i8, ptr %269, i64 2
  %277 = lshr i64 %270, 16
  %278 = trunc i64 %277 to i8
  store i8 %278, ptr %276, align 1
  %279 = getelementptr inbounds nuw i8, ptr %269, i64 3
  %280 = lshr i64 %270, 24
  %281 = trunc i64 %280 to i8
  store i8 %281, ptr %279, align 1
  %282 = getelementptr inbounds nuw i8, ptr %269, i64 4
  %283 = lshr i64 %270, 32
  %284 = trunc i64 %283 to i8
  store i8 %284, ptr %282, align 1
  %285 = getelementptr inbounds nuw i8, ptr %269, i64 5
  %286 = lshr i64 %270, 40
  %287 = trunc i64 %286 to i8
  store i8 %287, ptr %285, align 1
  %288 = getelementptr inbounds nuw i8, ptr %269, i64 6
  %289 = lshr i64 %270, 48
  %290 = trunc i64 %289 to i8
  store i8 %290, ptr %288, align 1
  %291 = getelementptr inbounds nuw i8, ptr %269, i64 7
  %292 = lshr i64 %270, 56
  %293 = trunc nuw i64 %292 to i8
  store i8 %293, ptr %291, align 1
  %294 = getelementptr inbounds nuw i8, ptr %269, i64 8
  %295 = add i32 %271, 8
  %296 = icmp eq i32 %295, %267
  br i1 %296, label %8615, label %268, !llvm.loop !16

297:                                              ; preds = %158
  %298 = add nsw i32 %72, 2
  store i32 %298, ptr @ip, align 4
  %299 = sext i32 %73 to i64
  %300 = getelementptr inbounds i8, ptr %69, i64 %299
  %301 = load i8, ptr %300, align 1
  %302 = shl i32 %81, 13
  %303 = xor i32 %302, %81
  %304 = lshr i32 %303, 17
  %305 = xor i32 %304, %303
  %306 = shl i32 %305, 5
  %307 = xor i32 %306, %305
  %308 = trunc i32 %307 to i8
  %309 = xor i8 %301, %308
  %310 = add nsw i32 %72, 3
  %311 = shl i32 %307, 13
  %312 = xor i32 %311, %307
  %313 = lshr i32 %312, 17
  %314 = xor i32 %313, %312
  %315 = shl i32 %314, 5
  %316 = xor i32 %315, %314
  store i32 %316, ptr @vm_code_state, align 4
  %317 = load i32, ptr @pointer_size, align 4
  %318 = icmp sgt i32 %317, 0
  br i1 %318, label %319, label %345

319:                                              ; preds = %297
  %320 = sext i32 %310 to i64
  %321 = zext nneg i32 %317 to i64
  br label %322

322:                                              ; preds = %319, %322
  %323 = phi i64 [ %320, %319 ], [ %327, %322 ]
  %324 = phi i64 [ 0, %319 ], [ %343, %322 ]
  %325 = phi i64 [ 0, %319 ], [ %342, %322 ]
  %326 = phi i32 [ %316, %319 ], [ %336, %322 ]
  %327 = add nsw i64 %323, 1
  %328 = trunc nsw i64 %327 to i32
  store i32 %328, ptr @ip, align 4
  %329 = getelementptr inbounds i8, ptr %69, i64 %323
  %330 = load i8, ptr %329, align 1
  %331 = shl i32 %326, 13
  %332 = xor i32 %331, %326
  %333 = lshr i32 %332, 17
  %334 = xor i32 %333, %332
  %335 = shl i32 %334, 5
  %336 = xor i32 %335, %334
  store i32 %336, ptr @vm_code_state, align 4
  %337 = trunc i32 %336 to i8
  %338 = xor i8 %330, %337
  %339 = zext i8 %338 to i64
  %340 = shl nsw i64 %324, 3
  %341 = shl i64 %339, %340
  %342 = or i64 %341, %325
  %343 = add nuw nsw i64 %324, 1
  %344 = icmp eq i64 %343, %321
  br i1 %344, label %345, label %322, !llvm.loop !15

345:                                              ; preds = %322, %297
  %346 = phi i32 [ %316, %297 ], [ %336, %322 ]
  %347 = phi i32 [ %310, %297 ], [ %328, %322 ]
  %348 = phi i64 [ 0, %297 ], [ %342, %322 ]
  %349 = shl i32 %346, 13
  %350 = xor i32 %349, %346
  %351 = lshr i32 %350, 17
  %352 = xor i32 %351, %350
  %353 = shl i32 %352, 5
  %354 = xor i32 %353, %352
  %355 = add nsw i32 %347, 2
  store i32 %355, ptr @ip, align 4
  %356 = shl i32 %354, 13
  %357 = xor i32 %356, %354
  %358 = lshr i32 %357, 17
  %359 = xor i32 %358, %357
  %360 = shl i32 %359, 5
  %361 = xor i32 %360, %359
  store i32 %361, ptr @vm_code_state, align 4
  br i1 %318, label %362, label %458

362:                                              ; preds = %345
  %363 = sext i32 %355 to i64
  %364 = zext nneg i32 %317 to i64
  br label %375

365:                                              ; preds = %375
  %366 = load i64, ptr @data_seg_addr, align 8
  %367 = inttoptr i64 %366 to ptr
  %368 = and i64 %364, 3
  %369 = icmp ult i32 %317, 4
  br i1 %369, label %434, label %370

370:                                              ; preds = %365
  %371 = and i64 %364, 2147483644
  %372 = getelementptr i8, ptr %367, i64 1
  %373 = getelementptr i8, ptr %367, i64 2
  %374 = getelementptr i8, ptr %367, i64 3
  br label %398

375:                                              ; preds = %362, %375
  %376 = phi i64 [ %363, %362 ], [ %380, %375 ]
  %377 = phi i64 [ 0, %362 ], [ %396, %375 ]
  %378 = phi i64 [ 0, %362 ], [ %395, %375 ]
  %379 = phi i32 [ %361, %362 ], [ %389, %375 ]
  %380 = add nsw i64 %376, 1
  %381 = trunc nsw i64 %380 to i32
  store i32 %381, ptr @ip, align 4
  %382 = getelementptr inbounds i8, ptr %69, i64 %376
  %383 = load i8, ptr %382, align 1
  %384 = shl i32 %379, 13
  %385 = xor i32 %384, %379
  %386 = lshr i32 %385, 17
  %387 = xor i32 %386, %385
  %388 = shl i32 %387, 5
  %389 = xor i32 %388, %387
  store i32 %389, ptr @vm_code_state, align 4
  %390 = trunc i32 %389 to i8
  %391 = xor i8 %383, %390
  %392 = zext i8 %391 to i64
  %393 = shl nsw i64 %377, 3
  %394 = shl i64 %392, %393
  %395 = or i64 %394, %378
  %396 = add nuw nsw i64 %377, 1
  %397 = icmp eq i64 %396, %364
  br i1 %397, label %365, label %375, !llvm.loop !15

398:                                              ; preds = %398, %370
  %399 = phi i64 [ 0, %370 ], [ %431, %398 ]
  %400 = phi i64 [ 0, %370 ], [ %430, %398 ]
  %401 = phi i64 [ %395, %370 ], [ %423, %398 ]
  %402 = phi i64 [ 0, %370 ], [ %432, %398 ]
  %403 = getelementptr inbounds nuw i8, ptr %367, i64 %401
  %404 = load i8, ptr %403, align 1
  %405 = zext i8 %404 to i64
  %406 = shl nsw i64 %399, 3
  %407 = shl i64 %405, %406
  %408 = or i64 %407, %400
  %409 = getelementptr i8, ptr %372, i64 %401
  %410 = load i8, ptr %409, align 1
  %411 = zext i8 %410 to i64
  %412 = shl i64 %399, 3
  %413 = or disjoint i64 %412, 8
  %414 = shl i64 %411, %413
  %415 = or i64 %414, %408
  %416 = getelementptr i8, ptr %373, i64 %401
  %417 = load i8, ptr %416, align 1
  %418 = zext i8 %417 to i64
  %419 = shl i64 %399, 3
  %420 = or disjoint i64 %419, 16
  %421 = shl i64 %418, %420
  %422 = or i64 %421, %415
  %423 = add i64 %401, 4
  %424 = getelementptr i8, ptr %374, i64 %401
  %425 = load i8, ptr %424, align 1
  %426 = zext i8 %425 to i64
  %427 = shl i64 %399, 3
  %428 = or disjoint i64 %427, 24
  %429 = shl i64 %426, %428
  %430 = or i64 %429, %422
  %431 = add nuw nsw i64 %399, 4
  %432 = add i64 %402, 4
  %433 = icmp eq i64 %432, %371
  br i1 %433, label %434, label %398, !llvm.loop !17

434:                                              ; preds = %398, %365
  %435 = phi i64 [ poison, %365 ], [ %430, %398 ]
  %436 = phi i64 [ 0, %365 ], [ %431, %398 ]
  %437 = phi i64 [ 0, %365 ], [ %430, %398 ]
  %438 = phi i64 [ %395, %365 ], [ %423, %398 ]
  %439 = icmp eq i64 %368, 0
  br i1 %439, label %455, label %440

440:                                              ; preds = %434, %440
  %441 = phi i64 [ %452, %440 ], [ %436, %434 ]
  %442 = phi i64 [ %451, %440 ], [ %437, %434 ]
  %443 = phi i64 [ %445, %440 ], [ %438, %434 ]
  %444 = phi i64 [ %453, %440 ], [ 0, %434 ]
  %445 = add i64 %443, 1
  %446 = getelementptr inbounds nuw i8, ptr %367, i64 %443
  %447 = load i8, ptr %446, align 1
  %448 = zext i8 %447 to i64
  %449 = shl nsw i64 %441, 3
  %450 = shl i64 %448, %449
  %451 = or i64 %450, %442
  %452 = add nuw nsw i64 %441, 1
  %453 = add i64 %444, 1
  %454 = icmp eq i64 %453, %368
  br i1 %454, label %455, label %440, !llvm.loop !18

455:                                              ; preds = %440, %434
  %456 = phi i64 [ %435, %434 ], [ %451, %440 ]
  %457 = inttoptr i64 %456 to ptr
  br label %458

458:                                              ; preds = %345, %455
  %459 = phi ptr [ %457, %455 ], [ null, %345 ]
  %460 = zext i8 %309 to i32
  %461 = icmp eq i8 %301, %308
  br i1 %461, label %8784, label %462

462:                                              ; preds = %458
  %463 = zext i8 %309 to i64
  %464 = and i64 %463, 3
  %465 = icmp ult i8 %309, 4
  br i1 %465, label %503, label %466

466:                                              ; preds = %462
  %467 = and i64 %463, 252
  br label %468

468:                                              ; preds = %468, %466
  %469 = phi i64 [ 0, %466 ], [ %500, %468 ]
  %470 = phi i64 [ 0, %466 ], [ %498, %468 ]
  %471 = phi ptr [ %459, %466 ], [ %499, %468 ]
  %472 = phi i64 [ 0, %466 ], [ %501, %468 ]
  %473 = load i8, ptr %471, align 1
  %474 = zext i8 %473 to i64
  %475 = shl nsw i64 %469, 3
  %476 = shl i64 %474, %475
  %477 = or i64 %476, %470
  %478 = getelementptr inbounds nuw i8, ptr %471, i64 1
  %479 = load i8, ptr %478, align 1
  %480 = zext i8 %479 to i64
  %481 = shl i64 %469, 3
  %482 = or disjoint i64 %481, 8
  %483 = shl i64 %480, %482
  %484 = or i64 %483, %477
  %485 = getelementptr inbounds nuw i8, ptr %471, i64 2
  %486 = load i8, ptr %485, align 1
  %487 = zext i8 %486 to i64
  %488 = shl i64 %469, 3
  %489 = or disjoint i64 %488, 16
  %490 = shl i64 %487, %489
  %491 = or i64 %490, %484
  %492 = getelementptr inbounds nuw i8, ptr %471, i64 3
  %493 = load i8, ptr %492, align 1
  %494 = zext i8 %493 to i64
  %495 = shl i64 %469, 3
  %496 = or disjoint i64 %495, 24
  %497 = shl i64 %494, %496
  %498 = or i64 %497, %491
  %499 = getelementptr inbounds nuw i8, ptr %471, i64 4
  %500 = add nuw nsw i64 %469, 4
  %501 = add i64 %472, 4
  %502 = icmp eq i64 %501, %467
  br i1 %502, label %503, label %468, !llvm.loop !20

503:                                              ; preds = %468, %462
  %504 = phi i64 [ poison, %462 ], [ %498, %468 ]
  %505 = phi i64 [ 0, %462 ], [ %500, %468 ]
  %506 = phi i64 [ 0, %462 ], [ %498, %468 ]
  %507 = phi ptr [ %459, %462 ], [ %499, %468 ]
  %508 = icmp eq i64 %464, 0
  br i1 %508, label %523, label %509

509:                                              ; preds = %503, %509
  %510 = phi i64 [ %520, %509 ], [ %505, %503 ]
  %511 = phi i64 [ %518, %509 ], [ %506, %503 ]
  %512 = phi ptr [ %519, %509 ], [ %507, %503 ]
  %513 = phi i64 [ %521, %509 ], [ 0, %503 ]
  %514 = load i8, ptr %512, align 1
  %515 = zext i8 %514 to i64
  %516 = shl nsw i64 %510, 3
  %517 = shl i64 %515, %516
  %518 = or i64 %517, %511
  %519 = getelementptr inbounds nuw i8, ptr %512, i64 1
  %520 = add nuw nsw i64 %510, 1
  %521 = add i64 %513, 1
  %522 = icmp eq i64 %521, %464
  br i1 %522, label %523, label %509, !llvm.loop !21

523:                                              ; preds = %509, %503
  %524 = phi i64 [ %504, %503 ], [ %518, %509 ]
  %525 = load i64, ptr @data_seg_addr, align 8
  %526 = add i64 %525, %348
  %527 = inttoptr i64 %526 to ptr
  %528 = and i32 %460, 7
  %529 = icmp ult i8 %309, 8
  br i1 %529, label %8628, label %530

530:                                              ; preds = %523
  %531 = and i32 %460, 248
  br label %532

532:                                              ; preds = %532, %530
  %533 = phi ptr [ %527, %530 ], [ %558, %532 ]
  %534 = phi i64 [ %524, %530 ], [ 0, %532 ]
  %535 = phi i32 [ 0, %530 ], [ %559, %532 ]
  %536 = trunc i64 %534 to i8
  store i8 %536, ptr %533, align 1
  %537 = getelementptr inbounds nuw i8, ptr %533, i64 1
  %538 = lshr i64 %534, 8
  %539 = trunc i64 %538 to i8
  store i8 %539, ptr %537, align 1
  %540 = getelementptr inbounds nuw i8, ptr %533, i64 2
  %541 = lshr i64 %534, 16
  %542 = trunc i64 %541 to i8
  store i8 %542, ptr %540, align 1
  %543 = getelementptr inbounds nuw i8, ptr %533, i64 3
  %544 = lshr i64 %534, 24
  %545 = trunc i64 %544 to i8
  store i8 %545, ptr %543, align 1
  %546 = getelementptr inbounds nuw i8, ptr %533, i64 4
  %547 = lshr i64 %534, 32
  %548 = trunc i64 %547 to i8
  store i8 %548, ptr %546, align 1
  %549 = getelementptr inbounds nuw i8, ptr %533, i64 5
  %550 = lshr i64 %534, 40
  %551 = trunc i64 %550 to i8
  store i8 %551, ptr %549, align 1
  %552 = getelementptr inbounds nuw i8, ptr %533, i64 6
  %553 = lshr i64 %534, 48
  %554 = trunc i64 %553 to i8
  store i8 %554, ptr %552, align 1
  %555 = getelementptr inbounds nuw i8, ptr %533, i64 7
  %556 = lshr i64 %534, 56
  %557 = trunc nuw i64 %556 to i8
  store i8 %557, ptr %555, align 1
  %558 = getelementptr inbounds nuw i8, ptr %533, i64 8
  %559 = add i32 %535, 8
  %560 = icmp eq i32 %559, %531
  br i1 %560, label %8628, label %532, !llvm.loop !16

561:                                              ; preds = %158
  %562 = add nsw i32 %72, 2
  store i32 %562, ptr @ip, align 4
  %563 = sext i32 %73 to i64
  %564 = getelementptr inbounds i8, ptr %69, i64 %563
  %565 = load i8, ptr %564, align 1
  %566 = shl i32 %81, 13
  %567 = xor i32 %566, %81
  %568 = lshr i32 %567, 17
  %569 = xor i32 %568, %567
  %570 = shl i32 %569, 5
  %571 = xor i32 %570, %569
  store i32 %571, ptr @vm_code_state, align 4
  %572 = trunc i32 %571 to i8
  %573 = xor i8 %565, %572
  %574 = add nsw i32 %72, 3
  store i32 %574, ptr @ip, align 4
  %575 = sext i32 %562 to i64
  %576 = getelementptr inbounds i8, ptr %69, i64 %575
  %577 = load i8, ptr %576, align 1
  %578 = shl i32 %571, 13
  %579 = xor i32 %578, %571
  %580 = lshr i32 %579, 17
  %581 = xor i32 %580, %579
  %582 = shl i32 %581, 5
  %583 = xor i32 %582, %581
  store i32 %583, ptr @vm_code_state, align 4
  %584 = trunc i32 %583 to i8
  %585 = icmp eq i8 %577, %584
  br i1 %585, label %586, label %667

586:                                              ; preds = %561
  %587 = load i32, ptr @pointer_size, align 4
  %588 = icmp sgt i32 %587, 0
  br i1 %588, label %589, label %615

589:                                              ; preds = %586
  %590 = sext i32 %574 to i64
  %591 = zext nneg i32 %587 to i64
  br label %592

592:                                              ; preds = %589, %592
  %593 = phi i64 [ %590, %589 ], [ %597, %592 ]
  %594 = phi i64 [ 0, %589 ], [ %613, %592 ]
  %595 = phi i64 [ 0, %589 ], [ %612, %592 ]
  %596 = phi i32 [ %583, %589 ], [ %606, %592 ]
  %597 = add nsw i64 %593, 1
  %598 = trunc nsw i64 %597 to i32
  store i32 %598, ptr @ip, align 4
  %599 = getelementptr inbounds i8, ptr %69, i64 %593
  %600 = load i8, ptr %599, align 1
  %601 = shl i32 %596, 13
  %602 = xor i32 %601, %596
  %603 = lshr i32 %602, 17
  %604 = xor i32 %603, %602
  %605 = shl i32 %604, 5
  %606 = xor i32 %605, %604
  store i32 %606, ptr @vm_code_state, align 4
  %607 = trunc i32 %606 to i8
  %608 = xor i8 %600, %607
  %609 = zext i8 %608 to i64
  %610 = shl nsw i64 %594, 3
  %611 = shl i64 %609, %610
  %612 = or i64 %611, %595
  %613 = add nuw nsw i64 %594, 1
  %614 = icmp eq i64 %613, %591
  br i1 %614, label %615, label %592, !llvm.loop !15

615:                                              ; preds = %592, %586
  %616 = phi i32 [ %583, %586 ], [ %606, %592 ]
  %617 = phi i32 [ %574, %586 ], [ %598, %592 ]
  %618 = phi i64 [ 0, %586 ], [ %612, %592 ]
  %619 = icmp eq i8 %565, %572
  br i1 %619, label %716, label %620

620:                                              ; preds = %615
  %621 = load i64, ptr @data_seg_addr, align 8
  %622 = inttoptr i64 %621 to ptr
  %623 = zext i8 %573 to i64
  %624 = and i64 %623, 3
  %625 = icmp ult i8 %573, 4
  br i1 %625, label %695, label %626

626:                                              ; preds = %620
  %627 = and i64 %623, 252
  %628 = getelementptr i8, ptr %622, i64 1
  %629 = getelementptr i8, ptr %622, i64 2
  %630 = getelementptr i8, ptr %622, i64 3
  br label %631

631:                                              ; preds = %631, %626
  %632 = phi i64 [ 0, %626 ], [ %664, %631 ]
  %633 = phi i64 [ 0, %626 ], [ %663, %631 ]
  %634 = phi i64 [ %618, %626 ], [ %656, %631 ]
  %635 = phi i64 [ 0, %626 ], [ %665, %631 ]
  %636 = getelementptr inbounds nuw i8, ptr %622, i64 %634
  %637 = load i8, ptr %636, align 1
  %638 = zext i8 %637 to i64
  %639 = shl nsw i64 %632, 3
  %640 = shl i64 %638, %639
  %641 = or i64 %640, %633
  %642 = getelementptr i8, ptr %628, i64 %634
  %643 = load i8, ptr %642, align 1
  %644 = zext i8 %643 to i64
  %645 = shl i64 %632, 3
  %646 = or disjoint i64 %645, 8
  %647 = shl i64 %644, %646
  %648 = or i64 %647, %641
  %649 = getelementptr i8, ptr %629, i64 %634
  %650 = load i8, ptr %649, align 1
  %651 = zext i8 %650 to i64
  %652 = shl i64 %632, 3
  %653 = or disjoint i64 %652, 16
  %654 = shl i64 %651, %653
  %655 = or i64 %654, %648
  %656 = add i64 %634, 4
  %657 = getelementptr i8, ptr %630, i64 %634
  %658 = load i8, ptr %657, align 1
  %659 = zext i8 %658 to i64
  %660 = shl i64 %632, 3
  %661 = or disjoint i64 %660, 24
  %662 = shl i64 %659, %661
  %663 = or i64 %662, %655
  %664 = add nuw nsw i64 %632, 4
  %665 = add i64 %635, 4
  %666 = icmp eq i64 %665, %627
  br i1 %666, label %695, label %631, !llvm.loop !17

667:                                              ; preds = %561
  %668 = icmp eq i8 %565, %572
  br i1 %668, label %716, label %669

669:                                              ; preds = %667
  %670 = sext i32 %574 to i64
  %671 = zext i8 %573 to i64
  br label %672

672:                                              ; preds = %669, %672
  %673 = phi i64 [ %670, %669 ], [ %677, %672 ]
  %674 = phi i64 [ 0, %669 ], [ %693, %672 ]
  %675 = phi i64 [ 0, %669 ], [ %692, %672 ]
  %676 = phi i32 [ %583, %669 ], [ %686, %672 ]
  %677 = add nsw i64 %673, 1
  %678 = trunc nsw i64 %677 to i32
  store i32 %678, ptr @ip, align 4
  %679 = getelementptr inbounds i8, ptr %69, i64 %673
  %680 = load i8, ptr %679, align 1
  %681 = shl i32 %676, 13
  %682 = xor i32 %681, %676
  %683 = lshr i32 %682, 17
  %684 = xor i32 %683, %682
  %685 = shl i32 %684, 5
  %686 = xor i32 %685, %684
  store i32 %686, ptr @vm_code_state, align 4
  %687 = trunc i32 %686 to i8
  %688 = xor i8 %680, %687
  %689 = zext i8 %688 to i64
  %690 = shl nsw i64 %674, 3
  %691 = shl i64 %689, %690
  %692 = or i64 %691, %675
  %693 = add nuw nsw i64 %674, 1
  %694 = icmp eq i64 %693, %671
  br i1 %694, label %716, label %672, !llvm.loop !15

695:                                              ; preds = %631, %620
  %696 = phi i64 [ poison, %620 ], [ %663, %631 ]
  %697 = phi i64 [ 0, %620 ], [ %664, %631 ]
  %698 = phi i64 [ 0, %620 ], [ %663, %631 ]
  %699 = phi i64 [ %618, %620 ], [ %656, %631 ]
  %700 = icmp eq i64 %624, 0
  br i1 %700, label %716, label %701

701:                                              ; preds = %695, %701
  %702 = phi i64 [ %713, %701 ], [ %697, %695 ]
  %703 = phi i64 [ %712, %701 ], [ %698, %695 ]
  %704 = phi i64 [ %706, %701 ], [ %699, %695 ]
  %705 = phi i64 [ %714, %701 ], [ 0, %695 ]
  %706 = add i64 %704, 1
  %707 = getelementptr inbounds nuw i8, ptr %622, i64 %704
  %708 = load i8, ptr %707, align 1
  %709 = zext i8 %708 to i64
  %710 = shl nsw i64 %702, 3
  %711 = shl i64 %709, %710
  %712 = or i64 %711, %703
  %713 = add nuw nsw i64 %702, 1
  %714 = add i64 %705, 1
  %715 = icmp eq i64 %714, %624
  br i1 %715, label %716, label %701, !llvm.loop !22

716:                                              ; preds = %672, %695, %701, %667, %615
  %717 = phi i32 [ %616, %615 ], [ %583, %667 ], [ %616, %701 ], [ %616, %695 ], [ %686, %672 ]
  %718 = phi i32 [ %617, %615 ], [ %574, %667 ], [ %617, %701 ], [ %617, %695 ], [ %678, %672 ]
  %719 = phi i64 [ 0, %615 ], [ 0, %667 ], [ %696, %695 ], [ %712, %701 ], [ %692, %672 ]
  %720 = shl i32 %717, 13
  %721 = xor i32 %720, %717
  %722 = lshr i32 %721, 17
  %723 = xor i32 %722, %721
  %724 = shl i32 %723, 5
  %725 = xor i32 %724, %723
  %726 = add nsw i32 %718, 2
  store i32 %726, ptr @ip, align 4
  %727 = shl i32 %725, 13
  %728 = xor i32 %727, %725
  %729 = lshr i32 %728, 17
  %730 = xor i32 %729, %728
  %731 = shl i32 %730, 5
  %732 = xor i32 %731, %730
  store i32 %732, ptr @vm_code_state, align 4
  %733 = load i32, ptr @pointer_size, align 4
  %734 = icmp sgt i32 %733, 0
  br i1 %734, label %735, label %831

735:                                              ; preds = %716
  %736 = sext i32 %726 to i64
  %737 = zext nneg i32 %733 to i64
  br label %748

738:                                              ; preds = %748
  %739 = load i64, ptr @data_seg_addr, align 8
  %740 = inttoptr i64 %739 to ptr
  %741 = and i64 %737, 3
  %742 = icmp ult i32 %733, 4
  br i1 %742, label %807, label %743

743:                                              ; preds = %738
  %744 = and i64 %737, 2147483644
  %745 = getelementptr i8, ptr %740, i64 1
  %746 = getelementptr i8, ptr %740, i64 2
  %747 = getelementptr i8, ptr %740, i64 3
  br label %771

748:                                              ; preds = %735, %748
  %749 = phi i64 [ %736, %735 ], [ %753, %748 ]
  %750 = phi i64 [ 0, %735 ], [ %769, %748 ]
  %751 = phi i64 [ 0, %735 ], [ %768, %748 ]
  %752 = phi i32 [ %732, %735 ], [ %762, %748 ]
  %753 = add nsw i64 %749, 1
  %754 = trunc nsw i64 %753 to i32
  store i32 %754, ptr @ip, align 4
  %755 = getelementptr inbounds i8, ptr %69, i64 %749
  %756 = load i8, ptr %755, align 1
  %757 = shl i32 %752, 13
  %758 = xor i32 %757, %752
  %759 = lshr i32 %758, 17
  %760 = xor i32 %759, %758
  %761 = shl i32 %760, 5
  %762 = xor i32 %761, %760
  store i32 %762, ptr @vm_code_state, align 4
  %763 = trunc i32 %762 to i8
  %764 = xor i8 %756, %763
  %765 = zext i8 %764 to i64
  %766 = shl nsw i64 %750, 3
  %767 = shl i64 %765, %766
  %768 = or i64 %767, %751
  %769 = add nuw nsw i64 %750, 1
  %770 = icmp eq i64 %769, %737
  br i1 %770, label %738, label %748, !llvm.loop !15

771:                                              ; preds = %771, %743
  %772 = phi i64 [ 0, %743 ], [ %804, %771 ]
  %773 = phi i64 [ 0, %743 ], [ %803, %771 ]
  %774 = phi i64 [ %768, %743 ], [ %796, %771 ]
  %775 = phi i64 [ 0, %743 ], [ %805, %771 ]
  %776 = getelementptr inbounds nuw i8, ptr %740, i64 %774
  %777 = load i8, ptr %776, align 1
  %778 = zext i8 %777 to i64
  %779 = shl nsw i64 %772, 3
  %780 = shl i64 %778, %779
  %781 = or i64 %780, %773
  %782 = getelementptr i8, ptr %745, i64 %774
  %783 = load i8, ptr %782, align 1
  %784 = zext i8 %783 to i64
  %785 = shl i64 %772, 3
  %786 = or disjoint i64 %785, 8
  %787 = shl i64 %784, %786
  %788 = or i64 %787, %781
  %789 = getelementptr i8, ptr %746, i64 %774
  %790 = load i8, ptr %789, align 1
  %791 = zext i8 %790 to i64
  %792 = shl i64 %772, 3
  %793 = or disjoint i64 %792, 16
  %794 = shl i64 %791, %793
  %795 = or i64 %794, %788
  %796 = add i64 %774, 4
  %797 = getelementptr i8, ptr %747, i64 %774
  %798 = load i8, ptr %797, align 1
  %799 = zext i8 %798 to i64
  %800 = shl i64 %772, 3
  %801 = or disjoint i64 %800, 24
  %802 = shl i64 %799, %801
  %803 = or i64 %802, %795
  %804 = add nuw nsw i64 %772, 4
  %805 = add i64 %775, 4
  %806 = icmp eq i64 %805, %744
  br i1 %806, label %807, label %771, !llvm.loop !17

807:                                              ; preds = %771, %738
  %808 = phi i64 [ poison, %738 ], [ %803, %771 ]
  %809 = phi i64 [ 0, %738 ], [ %804, %771 ]
  %810 = phi i64 [ 0, %738 ], [ %803, %771 ]
  %811 = phi i64 [ %768, %738 ], [ %796, %771 ]
  %812 = icmp eq i64 %741, 0
  br i1 %812, label %828, label %813

813:                                              ; preds = %807, %813
  %814 = phi i64 [ %825, %813 ], [ %809, %807 ]
  %815 = phi i64 [ %824, %813 ], [ %810, %807 ]
  %816 = phi i64 [ %818, %813 ], [ %811, %807 ]
  %817 = phi i64 [ %826, %813 ], [ 0, %807 ]
  %818 = add i64 %816, 1
  %819 = getelementptr inbounds nuw i8, ptr %740, i64 %816
  %820 = load i8, ptr %819, align 1
  %821 = zext i8 %820 to i64
  %822 = shl nsw i64 %814, 3
  %823 = shl i64 %821, %822
  %824 = or i64 %823, %815
  %825 = add nuw nsw i64 %814, 1
  %826 = add i64 %817, 1
  %827 = icmp eq i64 %826, %741
  br i1 %827, label %828, label %813, !llvm.loop !23

828:                                              ; preds = %813, %807
  %829 = phi i64 [ %808, %807 ], [ %824, %813 ]
  %830 = inttoptr i64 %829 to ptr
  br label %831

831:                                              ; preds = %716, %828
  %832 = phi ptr [ %830, %828 ], [ null, %716 ]
  %833 = icmp eq i8 %565, %572
  br i1 %833, label %8784, label %834

834:                                              ; preds = %831
  %835 = zext i8 %573 to i32
  %836 = and i32 %835, 7
  %837 = icmp ult i8 %573, 8
  br i1 %837, label %8641, label %838

838:                                              ; preds = %834
  %839 = and i32 %835, 248
  br label %840

840:                                              ; preds = %840, %838
  %841 = phi ptr [ %832, %838 ], [ %866, %840 ]
  %842 = phi i64 [ %719, %838 ], [ 0, %840 ]
  %843 = phi i32 [ 0, %838 ], [ %867, %840 ]
  %844 = trunc i64 %842 to i8
  store i8 %844, ptr %841, align 1
  %845 = getelementptr inbounds nuw i8, ptr %841, i64 1
  %846 = lshr i64 %842, 8
  %847 = trunc i64 %846 to i8
  store i8 %847, ptr %845, align 1
  %848 = getelementptr inbounds nuw i8, ptr %841, i64 2
  %849 = lshr i64 %842, 16
  %850 = trunc i64 %849 to i8
  store i8 %850, ptr %848, align 1
  %851 = getelementptr inbounds nuw i8, ptr %841, i64 3
  %852 = lshr i64 %842, 24
  %853 = trunc i64 %852 to i8
  store i8 %853, ptr %851, align 1
  %854 = getelementptr inbounds nuw i8, ptr %841, i64 4
  %855 = lshr i64 %842, 32
  %856 = trunc i64 %855 to i8
  store i8 %856, ptr %854, align 1
  %857 = getelementptr inbounds nuw i8, ptr %841, i64 5
  %858 = lshr i64 %842, 40
  %859 = trunc i64 %858 to i8
  store i8 %859, ptr %857, align 1
  %860 = getelementptr inbounds nuw i8, ptr %841, i64 6
  %861 = lshr i64 %842, 48
  %862 = trunc i64 %861 to i8
  store i8 %862, ptr %860, align 1
  %863 = getelementptr inbounds nuw i8, ptr %841, i64 7
  %864 = lshr i64 %842, 56
  %865 = trunc nuw i64 %864 to i8
  store i8 %865, ptr %863, align 1
  %866 = getelementptr inbounds nuw i8, ptr %841, i64 8
  %867 = add i32 %843, 8
  %868 = icmp eq i32 %867, %839
  br i1 %868, label %8641, label %840, !llvm.loop !16

869:                                              ; preds = %158
  %870 = add nsw i32 %72, 2
  store i32 %870, ptr @ip, align 4
  %871 = sext i32 %73 to i64
  %872 = getelementptr inbounds i8, ptr %69, i64 %871
  %873 = load i8, ptr %872, align 1
  %874 = shl i32 %81, 13
  %875 = xor i32 %874, %81
  %876 = lshr i32 %875, 17
  %877 = xor i32 %876, %875
  %878 = shl i32 %877, 5
  %879 = xor i32 %878, %877
  store i32 %879, ptr @vm_code_state, align 4
  %880 = trunc i32 %879 to i8
  %881 = xor i8 %873, %880
  %882 = add nsw i32 %72, 3
  store i32 %882, ptr @ip, align 4
  %883 = sext i32 %870 to i64
  %884 = getelementptr inbounds i8, ptr %69, i64 %883
  %885 = load i8, ptr %884, align 1
  %886 = shl i32 %879, 13
  %887 = xor i32 %886, %879
  %888 = lshr i32 %887, 17
  %889 = xor i32 %888, %887
  %890 = shl i32 %889, 5
  %891 = xor i32 %890, %889
  %892 = trunc i32 %891 to i8
  %893 = xor i8 %885, %892
  %894 = add nsw i32 %72, 4
  store i32 %894, ptr @ip, align 4
  %895 = shl i32 %891, 13
  %896 = xor i32 %895, %891
  %897 = lshr i32 %896, 17
  %898 = xor i32 %897, %896
  %899 = shl i32 %898, 5
  %900 = xor i32 %899, %898
  store i32 %900, ptr @vm_code_state, align 4
  %901 = load i32, ptr @pointer_size, align 4
  %902 = icmp sgt i32 %901, 0
  br i1 %902, label %903, label %929

903:                                              ; preds = %869
  %904 = sext i32 %894 to i64
  %905 = zext nneg i32 %901 to i64
  br label %906

906:                                              ; preds = %903, %906
  %907 = phi i64 [ %904, %903 ], [ %911, %906 ]
  %908 = phi i64 [ 0, %903 ], [ %927, %906 ]
  %909 = phi i64 [ 0, %903 ], [ %926, %906 ]
  %910 = phi i32 [ %900, %903 ], [ %920, %906 ]
  %911 = add nsw i64 %907, 1
  %912 = trunc nsw i64 %911 to i32
  store i32 %912, ptr @ip, align 4
  %913 = getelementptr inbounds i8, ptr %69, i64 %907
  %914 = load i8, ptr %913, align 1
  %915 = shl i32 %910, 13
  %916 = xor i32 %915, %910
  %917 = lshr i32 %916, 17
  %918 = xor i32 %917, %916
  %919 = shl i32 %918, 5
  %920 = xor i32 %919, %918
  store i32 %920, ptr @vm_code_state, align 4
  %921 = trunc i32 %920 to i8
  %922 = xor i8 %914, %921
  %923 = zext i8 %922 to i64
  %924 = shl nsw i64 %908, 3
  %925 = shl i64 %923, %924
  %926 = or i64 %925, %909
  %927 = add nuw nsw i64 %908, 1
  %928 = icmp eq i64 %927, %905
  br i1 %928, label %929, label %906, !llvm.loop !15

929:                                              ; preds = %906, %869
  %930 = phi i32 [ %900, %869 ], [ %920, %906 ]
  %931 = phi i32 [ %894, %869 ], [ %912, %906 ]
  %932 = phi i64 [ 0, %869 ], [ %926, %906 ]
  %933 = icmp eq i8 %881, 31
  %934 = add nsw i32 %931, 1
  store i32 %934, ptr @ip, align 4
  %935 = sext i32 %931 to i64
  %936 = getelementptr inbounds i8, ptr %69, i64 %935
  %937 = load i8, ptr %936, align 1
  %938 = shl i32 %930, 13
  %939 = xor i32 %938, %930
  %940 = lshr i32 %939, 17
  %941 = xor i32 %940, %939
  %942 = shl i32 %941, 5
  %943 = xor i32 %942, %941
  store i32 %943, ptr @vm_code_state, align 4
  %944 = trunc i32 %943 to i8
  %945 = xor i8 %937, %944
  %946 = add nsw i32 %931, 2
  store i32 %946, ptr @ip, align 4
  %947 = sext i32 %934 to i64
  %948 = getelementptr inbounds i8, ptr %69, i64 %947
  %949 = load i8, ptr %948, align 1
  %950 = shl i32 %943, 13
  %951 = xor i32 %950, %943
  %952 = lshr i32 %951, 17
  %953 = xor i32 %952, %951
  %954 = shl i32 %953, 5
  %955 = xor i32 %954, %953
  store i32 %955, ptr @vm_code_state, align 4
  %956 = trunc i32 %955 to i8
  %957 = icmp eq i8 %949, %956
  br i1 %933, label %958, label %1098

958:                                              ; preds = %929
  br i1 %957, label %959, label %1036

959:                                              ; preds = %958
  br i1 %902, label %960, label %986

960:                                              ; preds = %959
  %961 = sext i32 %946 to i64
  %962 = zext nneg i32 %901 to i64
  br label %963

963:                                              ; preds = %960, %963
  %964 = phi i64 [ %961, %960 ], [ %968, %963 ]
  %965 = phi i64 [ 0, %960 ], [ %984, %963 ]
  %966 = phi i64 [ 0, %960 ], [ %983, %963 ]
  %967 = phi i32 [ %955, %960 ], [ %977, %963 ]
  %968 = add nsw i64 %964, 1
  %969 = trunc nsw i64 %968 to i32
  store i32 %969, ptr @ip, align 4
  %970 = getelementptr inbounds i8, ptr %69, i64 %964
  %971 = load i8, ptr %970, align 1
  %972 = shl i32 %967, 13
  %973 = xor i32 %972, %967
  %974 = lshr i32 %973, 17
  %975 = xor i32 %974, %973
  %976 = shl i32 %975, 5
  %977 = xor i32 %976, %975
  store i32 %977, ptr @vm_code_state, align 4
  %978 = trunc i32 %977 to i8
  %979 = xor i8 %971, %978
  %980 = zext i8 %979 to i64
  %981 = shl nsw i64 %965, 3
  %982 = shl i64 %980, %981
  %983 = or i64 %982, %966
  %984 = add nuw nsw i64 %965, 1
  %985 = icmp eq i64 %984, %962
  br i1 %985, label %986, label %963, !llvm.loop !15

986:                                              ; preds = %963, %959
  %987 = phi i64 [ 0, %959 ], [ %983, %963 ]
  %988 = icmp eq i8 %937, %944
  br i1 %988, label %1085, label %989

989:                                              ; preds = %986
  %990 = load i64, ptr @data_seg_addr, align 8
  %991 = inttoptr i64 %990 to ptr
  %992 = zext i8 %945 to i64
  %993 = and i64 %992, 3
  %994 = icmp ult i8 %945, 4
  br i1 %994, label %1064, label %995

995:                                              ; preds = %989
  %996 = and i64 %992, 252
  %997 = getelementptr i8, ptr %991, i64 1
  %998 = getelementptr i8, ptr %991, i64 2
  %999 = getelementptr i8, ptr %991, i64 3
  br label %1000

1000:                                             ; preds = %1000, %995
  %1001 = phi i64 [ 0, %995 ], [ %1033, %1000 ]
  %1002 = phi i64 [ 0, %995 ], [ %1032, %1000 ]
  %1003 = phi i64 [ %987, %995 ], [ %1025, %1000 ]
  %1004 = phi i64 [ 0, %995 ], [ %1034, %1000 ]
  %1005 = getelementptr inbounds nuw i8, ptr %991, i64 %1003
  %1006 = load i8, ptr %1005, align 1
  %1007 = zext i8 %1006 to i64
  %1008 = shl nsw i64 %1001, 3
  %1009 = shl i64 %1007, %1008
  %1010 = or i64 %1009, %1002
  %1011 = getelementptr i8, ptr %997, i64 %1003
  %1012 = load i8, ptr %1011, align 1
  %1013 = zext i8 %1012 to i64
  %1014 = shl i64 %1001, 3
  %1015 = or disjoint i64 %1014, 8
  %1016 = shl i64 %1013, %1015
  %1017 = or i64 %1016, %1010
  %1018 = getelementptr i8, ptr %998, i64 %1003
  %1019 = load i8, ptr %1018, align 1
  %1020 = zext i8 %1019 to i64
  %1021 = shl i64 %1001, 3
  %1022 = or disjoint i64 %1021, 16
  %1023 = shl i64 %1020, %1022
  %1024 = or i64 %1023, %1017
  %1025 = add i64 %1003, 4
  %1026 = getelementptr i8, ptr %999, i64 %1003
  %1027 = load i8, ptr %1026, align 1
  %1028 = zext i8 %1027 to i64
  %1029 = shl i64 %1001, 3
  %1030 = or disjoint i64 %1029, 24
  %1031 = shl i64 %1028, %1030
  %1032 = or i64 %1031, %1024
  %1033 = add nuw nsw i64 %1001, 4
  %1034 = add i64 %1004, 4
  %1035 = icmp eq i64 %1034, %996
  br i1 %1035, label %1064, label %1000, !llvm.loop !17

1036:                                             ; preds = %958
  %1037 = icmp eq i8 %937, %944
  br i1 %1037, label %1085, label %1038

1038:                                             ; preds = %1036
  %1039 = sext i32 %946 to i64
  %1040 = zext i8 %945 to i64
  br label %1041

1041:                                             ; preds = %1038, %1041
  %1042 = phi i64 [ %1039, %1038 ], [ %1046, %1041 ]
  %1043 = phi i64 [ 0, %1038 ], [ %1062, %1041 ]
  %1044 = phi i64 [ 0, %1038 ], [ %1061, %1041 ]
  %1045 = phi i32 [ %955, %1038 ], [ %1055, %1041 ]
  %1046 = add nsw i64 %1042, 1
  %1047 = trunc nsw i64 %1046 to i32
  store i32 %1047, ptr @ip, align 4
  %1048 = getelementptr inbounds i8, ptr %69, i64 %1042
  %1049 = load i8, ptr %1048, align 1
  %1050 = shl i32 %1045, 13
  %1051 = xor i32 %1050, %1045
  %1052 = lshr i32 %1051, 17
  %1053 = xor i32 %1052, %1051
  %1054 = shl i32 %1053, 5
  %1055 = xor i32 %1054, %1053
  store i32 %1055, ptr @vm_code_state, align 4
  %1056 = trunc i32 %1055 to i8
  %1057 = xor i8 %1049, %1056
  %1058 = zext i8 %1057 to i64
  %1059 = shl nsw i64 %1043, 3
  %1060 = shl i64 %1058, %1059
  %1061 = or i64 %1060, %1044
  %1062 = add nuw nsw i64 %1043, 1
  %1063 = icmp eq i64 %1062, %1040
  br i1 %1063, label %1085, label %1041, !llvm.loop !15

1064:                                             ; preds = %1000, %989
  %1065 = phi i64 [ poison, %989 ], [ %1032, %1000 ]
  %1066 = phi i64 [ 0, %989 ], [ %1033, %1000 ]
  %1067 = phi i64 [ 0, %989 ], [ %1032, %1000 ]
  %1068 = phi i64 [ %987, %989 ], [ %1025, %1000 ]
  %1069 = icmp eq i64 %993, 0
  br i1 %1069, label %1085, label %1070

1070:                                             ; preds = %1064, %1070
  %1071 = phi i64 [ %1082, %1070 ], [ %1066, %1064 ]
  %1072 = phi i64 [ %1081, %1070 ], [ %1067, %1064 ]
  %1073 = phi i64 [ %1075, %1070 ], [ %1068, %1064 ]
  %1074 = phi i64 [ %1083, %1070 ], [ 0, %1064 ]
  %1075 = add i64 %1073, 1
  %1076 = getelementptr inbounds nuw i8, ptr %991, i64 %1073
  %1077 = load i8, ptr %1076, align 1
  %1078 = zext i8 %1077 to i64
  %1079 = shl nsw i64 %1071, 3
  %1080 = shl i64 %1078, %1079
  %1081 = or i64 %1080, %1072
  %1082 = add nuw nsw i64 %1071, 1
  %1083 = add i64 %1074, 1
  %1084 = icmp eq i64 %1083, %993
  br i1 %1084, label %1085, label %1070, !llvm.loop !24

1085:                                             ; preds = %1041, %1064, %1070, %1036, %986
  %1086 = phi i64 [ 0, %986 ], [ 0, %1036 ], [ %1065, %1064 ], [ %1081, %1070 ], [ %1061, %1041 ]
  %1087 = icmp ult i8 %893, 5
  br i1 %1087, label %1088, label %1094

1088:                                             ; preds = %1085
  %1089 = trunc i64 %1086 to i32
  %1090 = bitcast i32 %1089 to float
  %1091 = fneg float %1090
  %1092 = bitcast float %1091 to i32
  %1093 = zext i32 %1092 to i64
  br label %1492

1094:                                             ; preds = %1085
  %1095 = bitcast i64 %1086 to double
  %1096 = fneg double %1095
  %1097 = bitcast double %1096 to i64
  br label %1492

1098:                                             ; preds = %929
  br i1 %957, label %1099, label %1178

1099:                                             ; preds = %1098
  br i1 %902, label %1100, label %1126

1100:                                             ; preds = %1099
  %1101 = sext i32 %946 to i64
  %1102 = zext nneg i32 %901 to i64
  br label %1103

1103:                                             ; preds = %1100, %1103
  %1104 = phi i64 [ %1101, %1100 ], [ %1108, %1103 ]
  %1105 = phi i64 [ 0, %1100 ], [ %1124, %1103 ]
  %1106 = phi i64 [ 0, %1100 ], [ %1123, %1103 ]
  %1107 = phi i32 [ %955, %1100 ], [ %1117, %1103 ]
  %1108 = add nsw i64 %1104, 1
  %1109 = trunc nsw i64 %1108 to i32
  store i32 %1109, ptr @ip, align 4
  %1110 = getelementptr inbounds i8, ptr %69, i64 %1104
  %1111 = load i8, ptr %1110, align 1
  %1112 = shl i32 %1107, 13
  %1113 = xor i32 %1112, %1107
  %1114 = lshr i32 %1113, 17
  %1115 = xor i32 %1114, %1113
  %1116 = shl i32 %1115, 5
  %1117 = xor i32 %1116, %1115
  store i32 %1117, ptr @vm_code_state, align 4
  %1118 = trunc i32 %1117 to i8
  %1119 = xor i8 %1111, %1118
  %1120 = zext i8 %1119 to i64
  %1121 = shl nsw i64 %1105, 3
  %1122 = shl i64 %1120, %1121
  %1123 = or i64 %1122, %1106
  %1124 = add nuw nsw i64 %1105, 1
  %1125 = icmp eq i64 %1124, %1102
  br i1 %1125, label %1126, label %1103, !llvm.loop !15

1126:                                             ; preds = %1103, %1099
  %1127 = phi i32 [ %955, %1099 ], [ %1117, %1103 ]
  %1128 = phi i32 [ %946, %1099 ], [ %1109, %1103 ]
  %1129 = phi i64 [ 0, %1099 ], [ %1123, %1103 ]
  %1130 = icmp eq i8 %937, %944
  br i1 %1130, label %1227, label %1131

1131:                                             ; preds = %1126
  %1132 = load i64, ptr @data_seg_addr, align 8
  %1133 = inttoptr i64 %1132 to ptr
  %1134 = zext i8 %945 to i64
  %1135 = and i64 %1134, 3
  %1136 = icmp ult i8 %945, 4
  br i1 %1136, label %1206, label %1137

1137:                                             ; preds = %1131
  %1138 = and i64 %1134, 252
  %1139 = getelementptr i8, ptr %1133, i64 1
  %1140 = getelementptr i8, ptr %1133, i64 2
  %1141 = getelementptr i8, ptr %1133, i64 3
  br label %1142

1142:                                             ; preds = %1142, %1137
  %1143 = phi i64 [ 0, %1137 ], [ %1175, %1142 ]
  %1144 = phi i64 [ 0, %1137 ], [ %1174, %1142 ]
  %1145 = phi i64 [ %1129, %1137 ], [ %1167, %1142 ]
  %1146 = phi i64 [ 0, %1137 ], [ %1176, %1142 ]
  %1147 = getelementptr inbounds nuw i8, ptr %1133, i64 %1145
  %1148 = load i8, ptr %1147, align 1
  %1149 = zext i8 %1148 to i64
  %1150 = shl nsw i64 %1143, 3
  %1151 = shl i64 %1149, %1150
  %1152 = or i64 %1151, %1144
  %1153 = getelementptr i8, ptr %1139, i64 %1145
  %1154 = load i8, ptr %1153, align 1
  %1155 = zext i8 %1154 to i64
  %1156 = shl i64 %1143, 3
  %1157 = or disjoint i64 %1156, 8
  %1158 = shl i64 %1155, %1157
  %1159 = or i64 %1158, %1152
  %1160 = getelementptr i8, ptr %1140, i64 %1145
  %1161 = load i8, ptr %1160, align 1
  %1162 = zext i8 %1161 to i64
  %1163 = shl i64 %1143, 3
  %1164 = or disjoint i64 %1163, 16
  %1165 = shl i64 %1162, %1164
  %1166 = or i64 %1165, %1159
  %1167 = add i64 %1145, 4
  %1168 = getelementptr i8, ptr %1141, i64 %1145
  %1169 = load i8, ptr %1168, align 1
  %1170 = zext i8 %1169 to i64
  %1171 = shl i64 %1143, 3
  %1172 = or disjoint i64 %1171, 24
  %1173 = shl i64 %1170, %1172
  %1174 = or i64 %1173, %1166
  %1175 = add nuw nsw i64 %1143, 4
  %1176 = add i64 %1146, 4
  %1177 = icmp eq i64 %1176, %1138
  br i1 %1177, label %1206, label %1142, !llvm.loop !17

1178:                                             ; preds = %1098
  %1179 = icmp eq i8 %937, %944
  br i1 %1179, label %1227, label %1180

1180:                                             ; preds = %1178
  %1181 = sext i32 %946 to i64
  %1182 = zext i8 %945 to i64
  br label %1183

1183:                                             ; preds = %1180, %1183
  %1184 = phi i64 [ %1181, %1180 ], [ %1188, %1183 ]
  %1185 = phi i64 [ 0, %1180 ], [ %1204, %1183 ]
  %1186 = phi i64 [ 0, %1180 ], [ %1203, %1183 ]
  %1187 = phi i32 [ %955, %1180 ], [ %1197, %1183 ]
  %1188 = add nsw i64 %1184, 1
  %1189 = trunc nsw i64 %1188 to i32
  store i32 %1189, ptr @ip, align 4
  %1190 = getelementptr inbounds i8, ptr %69, i64 %1184
  %1191 = load i8, ptr %1190, align 1
  %1192 = shl i32 %1187, 13
  %1193 = xor i32 %1192, %1187
  %1194 = lshr i32 %1193, 17
  %1195 = xor i32 %1194, %1193
  %1196 = shl i32 %1195, 5
  %1197 = xor i32 %1196, %1195
  store i32 %1197, ptr @vm_code_state, align 4
  %1198 = trunc i32 %1197 to i8
  %1199 = xor i8 %1191, %1198
  %1200 = zext i8 %1199 to i64
  %1201 = shl nsw i64 %1185, 3
  %1202 = shl i64 %1200, %1201
  %1203 = or i64 %1202, %1186
  %1204 = add nuw nsw i64 %1185, 1
  %1205 = icmp eq i64 %1204, %1182
  br i1 %1205, label %1227, label %1183, !llvm.loop !15

1206:                                             ; preds = %1142, %1131
  %1207 = phi i64 [ poison, %1131 ], [ %1174, %1142 ]
  %1208 = phi i64 [ 0, %1131 ], [ %1175, %1142 ]
  %1209 = phi i64 [ 0, %1131 ], [ %1174, %1142 ]
  %1210 = phi i64 [ %1129, %1131 ], [ %1167, %1142 ]
  %1211 = icmp eq i64 %1135, 0
  br i1 %1211, label %1227, label %1212

1212:                                             ; preds = %1206, %1212
  %1213 = phi i64 [ %1224, %1212 ], [ %1208, %1206 ]
  %1214 = phi i64 [ %1223, %1212 ], [ %1209, %1206 ]
  %1215 = phi i64 [ %1217, %1212 ], [ %1210, %1206 ]
  %1216 = phi i64 [ %1225, %1212 ], [ 0, %1206 ]
  %1217 = add i64 %1215, 1
  %1218 = getelementptr inbounds nuw i8, ptr %1133, i64 %1215
  %1219 = load i8, ptr %1218, align 1
  %1220 = zext i8 %1219 to i64
  %1221 = shl nsw i64 %1213, 3
  %1222 = shl i64 %1220, %1221
  %1223 = or i64 %1222, %1214
  %1224 = add nuw nsw i64 %1213, 1
  %1225 = add i64 %1216, 1
  %1226 = icmp eq i64 %1225, %1135
  br i1 %1226, label %1227, label %1212, !llvm.loop !25

1227:                                             ; preds = %1183, %1206, %1212, %1178, %1126
  %1228 = phi i32 [ %1127, %1126 ], [ %955, %1178 ], [ %1127, %1212 ], [ %1127, %1206 ], [ %1197, %1183 ]
  %1229 = phi i32 [ %1128, %1126 ], [ %946, %1178 ], [ %1128, %1212 ], [ %1128, %1206 ], [ %1189, %1183 ]
  %1230 = phi i64 [ 0, %1126 ], [ 0, %1178 ], [ %1207, %1206 ], [ %1223, %1212 ], [ %1203, %1183 ]
  %1231 = add nsw i32 %1229, 1
  store i32 %1231, ptr @ip, align 4
  %1232 = sext i32 %1229 to i64
  %1233 = getelementptr inbounds i8, ptr %69, i64 %1232
  %1234 = load i8, ptr %1233, align 1
  %1235 = shl i32 %1228, 13
  %1236 = xor i32 %1235, %1228
  %1237 = lshr i32 %1236, 17
  %1238 = xor i32 %1237, %1236
  %1239 = shl i32 %1238, 5
  %1240 = xor i32 %1239, %1238
  store i32 %1240, ptr @vm_code_state, align 4
  %1241 = trunc i32 %1240 to i8
  %1242 = xor i8 %1234, %1241
  %1243 = add nsw i32 %1229, 2
  store i32 %1243, ptr @ip, align 4
  %1244 = sext i32 %1231 to i64
  %1245 = getelementptr inbounds i8, ptr %69, i64 %1244
  %1246 = load i8, ptr %1245, align 1
  %1247 = shl i32 %1240, 13
  %1248 = xor i32 %1247, %1240
  %1249 = lshr i32 %1248, 17
  %1250 = xor i32 %1249, %1248
  %1251 = shl i32 %1250, 5
  %1252 = xor i32 %1251, %1250
  store i32 %1252, ptr @vm_code_state, align 4
  %1253 = trunc i32 %1252 to i8
  %1254 = icmp eq i8 %1246, %1253
  br i1 %1254, label %1255, label %1332

1255:                                             ; preds = %1227
  br i1 %902, label %1256, label %1282

1256:                                             ; preds = %1255
  %1257 = sext i32 %1243 to i64
  %1258 = zext nneg i32 %901 to i64
  br label %1259

1259:                                             ; preds = %1256, %1259
  %1260 = phi i64 [ %1257, %1256 ], [ %1264, %1259 ]
  %1261 = phi i64 [ 0, %1256 ], [ %1280, %1259 ]
  %1262 = phi i64 [ 0, %1256 ], [ %1279, %1259 ]
  %1263 = phi i32 [ %1252, %1256 ], [ %1273, %1259 ]
  %1264 = add nsw i64 %1260, 1
  %1265 = trunc nsw i64 %1264 to i32
  store i32 %1265, ptr @ip, align 4
  %1266 = getelementptr inbounds i8, ptr %69, i64 %1260
  %1267 = load i8, ptr %1266, align 1
  %1268 = shl i32 %1263, 13
  %1269 = xor i32 %1268, %1263
  %1270 = lshr i32 %1269, 17
  %1271 = xor i32 %1270, %1269
  %1272 = shl i32 %1271, 5
  %1273 = xor i32 %1272, %1271
  store i32 %1273, ptr @vm_code_state, align 4
  %1274 = trunc i32 %1273 to i8
  %1275 = xor i8 %1267, %1274
  %1276 = zext i8 %1275 to i64
  %1277 = shl nsw i64 %1261, 3
  %1278 = shl i64 %1276, %1277
  %1279 = or i64 %1278, %1262
  %1280 = add nuw nsw i64 %1261, 1
  %1281 = icmp eq i64 %1280, %1258
  br i1 %1281, label %1282, label %1259, !llvm.loop !15

1282:                                             ; preds = %1259, %1255
  %1283 = phi i64 [ 0, %1255 ], [ %1279, %1259 ]
  %1284 = icmp eq i8 %1234, %1241
  br i1 %1284, label %1381, label %1285

1285:                                             ; preds = %1282
  %1286 = load i64, ptr @data_seg_addr, align 8
  %1287 = inttoptr i64 %1286 to ptr
  %1288 = zext i8 %1242 to i64
  %1289 = and i64 %1288, 3
  %1290 = icmp ult i8 %1242, 4
  br i1 %1290, label %1360, label %1291

1291:                                             ; preds = %1285
  %1292 = and i64 %1288, 252
  %1293 = getelementptr i8, ptr %1287, i64 1
  %1294 = getelementptr i8, ptr %1287, i64 2
  %1295 = getelementptr i8, ptr %1287, i64 3
  br label %1296

1296:                                             ; preds = %1296, %1291
  %1297 = phi i64 [ 0, %1291 ], [ %1329, %1296 ]
  %1298 = phi i64 [ 0, %1291 ], [ %1328, %1296 ]
  %1299 = phi i64 [ %1283, %1291 ], [ %1321, %1296 ]
  %1300 = phi i64 [ 0, %1291 ], [ %1330, %1296 ]
  %1301 = getelementptr inbounds nuw i8, ptr %1287, i64 %1299
  %1302 = load i8, ptr %1301, align 1
  %1303 = zext i8 %1302 to i64
  %1304 = shl nsw i64 %1297, 3
  %1305 = shl i64 %1303, %1304
  %1306 = or i64 %1305, %1298
  %1307 = getelementptr i8, ptr %1293, i64 %1299
  %1308 = load i8, ptr %1307, align 1
  %1309 = zext i8 %1308 to i64
  %1310 = shl i64 %1297, 3
  %1311 = or disjoint i64 %1310, 8
  %1312 = shl i64 %1309, %1311
  %1313 = or i64 %1312, %1306
  %1314 = getelementptr i8, ptr %1294, i64 %1299
  %1315 = load i8, ptr %1314, align 1
  %1316 = zext i8 %1315 to i64
  %1317 = shl i64 %1297, 3
  %1318 = or disjoint i64 %1317, 16
  %1319 = shl i64 %1316, %1318
  %1320 = or i64 %1319, %1313
  %1321 = add i64 %1299, 4
  %1322 = getelementptr i8, ptr %1295, i64 %1299
  %1323 = load i8, ptr %1322, align 1
  %1324 = zext i8 %1323 to i64
  %1325 = shl i64 %1297, 3
  %1326 = or disjoint i64 %1325, 24
  %1327 = shl i64 %1324, %1326
  %1328 = or i64 %1327, %1320
  %1329 = add nuw nsw i64 %1297, 4
  %1330 = add i64 %1300, 4
  %1331 = icmp eq i64 %1330, %1292
  br i1 %1331, label %1360, label %1296, !llvm.loop !17

1332:                                             ; preds = %1227
  %1333 = icmp eq i8 %1234, %1241
  br i1 %1333, label %1381, label %1334

1334:                                             ; preds = %1332
  %1335 = sext i32 %1243 to i64
  %1336 = zext i8 %1242 to i64
  br label %1337

1337:                                             ; preds = %1334, %1337
  %1338 = phi i64 [ %1335, %1334 ], [ %1342, %1337 ]
  %1339 = phi i64 [ 0, %1334 ], [ %1358, %1337 ]
  %1340 = phi i64 [ 0, %1334 ], [ %1357, %1337 ]
  %1341 = phi i32 [ %1252, %1334 ], [ %1351, %1337 ]
  %1342 = add nsw i64 %1338, 1
  %1343 = trunc nsw i64 %1342 to i32
  store i32 %1343, ptr @ip, align 4
  %1344 = getelementptr inbounds i8, ptr %69, i64 %1338
  %1345 = load i8, ptr %1344, align 1
  %1346 = shl i32 %1341, 13
  %1347 = xor i32 %1346, %1341
  %1348 = lshr i32 %1347, 17
  %1349 = xor i32 %1348, %1347
  %1350 = shl i32 %1349, 5
  %1351 = xor i32 %1350, %1349
  store i32 %1351, ptr @vm_code_state, align 4
  %1352 = trunc i32 %1351 to i8
  %1353 = xor i8 %1345, %1352
  %1354 = zext i8 %1353 to i64
  %1355 = shl nsw i64 %1339, 3
  %1356 = shl i64 %1354, %1355
  %1357 = or i64 %1356, %1340
  %1358 = add nuw nsw i64 %1339, 1
  %1359 = icmp eq i64 %1358, %1336
  br i1 %1359, label %1381, label %1337, !llvm.loop !15

1360:                                             ; preds = %1296, %1285
  %1361 = phi i64 [ poison, %1285 ], [ %1328, %1296 ]
  %1362 = phi i64 [ 0, %1285 ], [ %1329, %1296 ]
  %1363 = phi i64 [ 0, %1285 ], [ %1328, %1296 ]
  %1364 = phi i64 [ %1283, %1285 ], [ %1321, %1296 ]
  %1365 = icmp eq i64 %1289, 0
  br i1 %1365, label %1381, label %1366

1366:                                             ; preds = %1360, %1366
  %1367 = phi i64 [ %1378, %1366 ], [ %1362, %1360 ]
  %1368 = phi i64 [ %1377, %1366 ], [ %1363, %1360 ]
  %1369 = phi i64 [ %1371, %1366 ], [ %1364, %1360 ]
  %1370 = phi i64 [ %1379, %1366 ], [ 0, %1360 ]
  %1371 = add i64 %1369, 1
  %1372 = getelementptr inbounds nuw i8, ptr %1287, i64 %1369
  %1373 = load i8, ptr %1372, align 1
  %1374 = zext i8 %1373 to i64
  %1375 = shl nsw i64 %1367, 3
  %1376 = shl i64 %1374, %1375
  %1377 = or i64 %1376, %1368
  %1378 = add nuw nsw i64 %1367, 1
  %1379 = add i64 %1370, 1
  %1380 = icmp eq i64 %1379, %1289
  br i1 %1380, label %1381, label %1366, !llvm.loop !26

1381:                                             ; preds = %1337, %1360, %1366, %1332, %1282
  %1382 = phi i64 [ 0, %1282 ], [ 0, %1332 ], [ %1361, %1360 ], [ %1377, %1366 ], [ %1357, %1337 ]
  switch i8 %881, label %1492 [
    i8 13, label %1383
    i8 14, label %1385
    i8 15, label %1400
    i8 16, label %1402
    i8 17, label %1417
    i8 18, label %1419
    i8 19, label %1434
    i8 20, label %1436
    i8 21, label %1438
    i8 22, label %1453
    i8 23, label %1455
    i8 24, label %1457
    i8 25, label %1480
    i8 26, label %1482
    i8 27, label %1484
    i8 28, label %1486
    i8 29, label %1488
    i8 30, label %1490
  ]

1383:                                             ; preds = %1381
  %1384 = add i64 %1382, %1230
  br label %1492

1385:                                             ; preds = %1381
  %1386 = icmp ult i8 %893, 5
  br i1 %1386, label %1387, label %1395

1387:                                             ; preds = %1385
  %1388 = trunc i64 %1230 to i32
  %1389 = trunc i64 %1382 to i32
  %1390 = bitcast i32 %1388 to float
  %1391 = bitcast i32 %1389 to float
  %1392 = fadd float %1390, %1391
  %1393 = bitcast float %1392 to i32
  %1394 = zext i32 %1393 to i64
  br label %1492

1395:                                             ; preds = %1385
  %1396 = bitcast i64 %1230 to double
  %1397 = bitcast i64 %1382 to double
  %1398 = fadd double %1396, %1397
  %1399 = bitcast double %1398 to i64
  br label %1492

1400:                                             ; preds = %1381
  %1401 = sub i64 %1230, %1382
  br label %1492

1402:                                             ; preds = %1381
  %1403 = icmp ult i8 %893, 5
  br i1 %1403, label %1404, label %1412

1404:                                             ; preds = %1402
  %1405 = trunc i64 %1230 to i32
  %1406 = trunc i64 %1382 to i32
  %1407 = bitcast i32 %1405 to float
  %1408 = bitcast i32 %1406 to float
  %1409 = fsub float %1407, %1408
  %1410 = bitcast float %1409 to i32
  %1411 = zext i32 %1410 to i64
  br label %1492

1412:                                             ; preds = %1402
  %1413 = bitcast i64 %1230 to double
  %1414 = bitcast i64 %1382 to double
  %1415 = fsub double %1413, %1414
  %1416 = bitcast double %1415 to i64
  br label %1492

1417:                                             ; preds = %1381
  %1418 = mul i64 %1382, %1230
  br label %1492

1419:                                             ; preds = %1381
  %1420 = icmp ult i8 %893, 5
  br i1 %1420, label %1421, label %1429

1421:                                             ; preds = %1419
  %1422 = trunc i64 %1230 to i32
  %1423 = trunc i64 %1382 to i32
  %1424 = bitcast i32 %1422 to float
  %1425 = bitcast i32 %1423 to float
  %1426 = fmul float %1424, %1425
  %1427 = bitcast float %1426 to i32
  %1428 = zext i32 %1427 to i64
  br label %1492

1429:                                             ; preds = %1419
  %1430 = bitcast i64 %1230 to double
  %1431 = bitcast i64 %1382 to double
  %1432 = fmul double %1430, %1431
  %1433 = bitcast double %1432 to i64
  br label %1492

1434:                                             ; preds = %1381
  %1435 = udiv i64 %1230, %1382
  br label %1492

1436:                                             ; preds = %1381
  %1437 = udiv i64 %1230, %1382
  br label %1492

1438:                                             ; preds = %1381
  %1439 = icmp ult i8 %893, 5
  br i1 %1439, label %1440, label %1448

1440:                                             ; preds = %1438
  %1441 = trunc i64 %1230 to i32
  %1442 = trunc i64 %1382 to i32
  %1443 = bitcast i32 %1441 to float
  %1444 = bitcast i32 %1442 to float
  %1445 = fdiv float %1443, %1444
  %1446 = bitcast float %1445 to i32
  %1447 = zext i32 %1446 to i64
  br label %1492

1448:                                             ; preds = %1438
  %1449 = bitcast i64 %1230 to double
  %1450 = bitcast i64 %1382 to double
  %1451 = fdiv double %1449, %1450
  %1452 = bitcast double %1451 to i64
  br label %1492

1453:                                             ; preds = %1381
  %1454 = urem i64 %1230, %1382
  br label %1492

1455:                                             ; preds = %1381
  %1456 = urem i64 %1230, %1382
  br label %1492

1457:                                             ; preds = %1381
  %1458 = icmp ult i8 %893, 5
  br i1 %1458, label %1459, label %1471

1459:                                             ; preds = %1457
  %1460 = trunc i64 %1230 to i32
  %1461 = trunc i64 %1382 to i32
  %1462 = bitcast i32 %1460 to float
  %1463 = bitcast i32 %1461 to float
  %1464 = fdiv float %1462, %1463
  %1465 = fptosi float %1464 to i64
  %1466 = sitofp i64 %1465 to float
  %1467 = fneg float %1463
  %1468 = tail call float @llvm.fmuladd.f32(float %1467, float %1466, float %1462)
  %1469 = bitcast float %1468 to i32
  %1470 = zext i32 %1469 to i64
  br label %1492

1471:                                             ; preds = %1457
  %1472 = bitcast i64 %1230 to double
  %1473 = bitcast i64 %1382 to double
  %1474 = fdiv double %1472, %1473
  %1475 = fptosi double %1474 to i64
  %1476 = sitofp i64 %1475 to double
  %1477 = fneg double %1473
  %1478 = tail call double @llvm.fmuladd.f64(double %1477, double %1476, double %1472)
  %1479 = bitcast double %1478 to i64
  br label %1492

1480:                                             ; preds = %1381
  %1481 = shl i64 %1230, %1382
  br label %1492

1482:                                             ; preds = %1381
  %1483 = lshr i64 %1230, %1382
  br label %1492

1484:                                             ; preds = %1381
  %1485 = lshr i64 %1230, %1382
  br label %1492

1486:                                             ; preds = %1381
  %1487 = and i64 %1382, %1230
  br label %1492

1488:                                             ; preds = %1381
  %1489 = or i64 %1382, %1230
  br label %1492

1490:                                             ; preds = %1381
  %1491 = xor i64 %1382, %1230
  br label %1492

1492:                                             ; preds = %1490, %1488, %1486, %1484, %1482, %1480, %1471, %1459, %1455, %1453, %1448, %1440, %1436, %1434, %1429, %1421, %1417, %1412, %1404, %1400, %1395, %1387, %1383, %1381, %1094, %1088
  %1493 = phi i64 [ %1093, %1088 ], [ %1097, %1094 ], [ 0, %1381 ], [ %1384, %1383 ], [ %1394, %1387 ], [ %1399, %1395 ], [ %1401, %1400 ], [ %1411, %1404 ], [ %1416, %1412 ], [ %1418, %1417 ], [ %1428, %1421 ], [ %1433, %1429 ], [ %1435, %1434 ], [ %1437, %1436 ], [ %1447, %1440 ], [ %1452, %1448 ], [ %1454, %1453 ], [ %1456, %1455 ], [ %1470, %1459 ], [ %1479, %1471 ], [ %1481, %1480 ], [ %1483, %1482 ], [ %1485, %1484 ], [ %1487, %1486 ], [ %1489, %1488 ], [ %1491, %1490 ]
  %1494 = icmp eq i8 %885, %892
  br i1 %1494, label %8784, label %1495

1495:                                             ; preds = %1492
  %1496 = load i64, ptr @data_seg_addr, align 8
  %1497 = add i64 %1496, %932
  %1498 = inttoptr i64 %1497 to ptr
  %1499 = zext i8 %893 to i32
  %1500 = and i32 %1499, 7
  %1501 = icmp ult i8 %893, 8
  br i1 %1501, label %8654, label %1502

1502:                                             ; preds = %1495
  %1503 = and i32 %1499, 248
  br label %1504

1504:                                             ; preds = %1504, %1502
  %1505 = phi ptr [ %1498, %1502 ], [ %1530, %1504 ]
  %1506 = phi i64 [ %1493, %1502 ], [ 0, %1504 ]
  %1507 = phi i32 [ 0, %1502 ], [ %1531, %1504 ]
  %1508 = trunc i64 %1506 to i8
  store i8 %1508, ptr %1505, align 1
  %1509 = getelementptr inbounds nuw i8, ptr %1505, i64 1
  %1510 = lshr i64 %1506, 8
  %1511 = trunc i64 %1510 to i8
  store i8 %1511, ptr %1509, align 1
  %1512 = getelementptr inbounds nuw i8, ptr %1505, i64 2
  %1513 = lshr i64 %1506, 16
  %1514 = trunc i64 %1513 to i8
  store i8 %1514, ptr %1512, align 1
  %1515 = getelementptr inbounds nuw i8, ptr %1505, i64 3
  %1516 = lshr i64 %1506, 24
  %1517 = trunc i64 %1516 to i8
  store i8 %1517, ptr %1515, align 1
  %1518 = getelementptr inbounds nuw i8, ptr %1505, i64 4
  %1519 = lshr i64 %1506, 32
  %1520 = trunc i64 %1519 to i8
  store i8 %1520, ptr %1518, align 1
  %1521 = getelementptr inbounds nuw i8, ptr %1505, i64 5
  %1522 = lshr i64 %1506, 40
  %1523 = trunc i64 %1522 to i8
  store i8 %1523, ptr %1521, align 1
  %1524 = getelementptr inbounds nuw i8, ptr %1505, i64 6
  %1525 = lshr i64 %1506, 48
  %1526 = trunc i64 %1525 to i8
  store i8 %1526, ptr %1524, align 1
  %1527 = getelementptr inbounds nuw i8, ptr %1505, i64 7
  %1528 = lshr i64 %1506, 56
  %1529 = trunc nuw i64 %1528 to i8
  store i8 %1529, ptr %1527, align 1
  %1530 = getelementptr inbounds nuw i8, ptr %1505, i64 8
  %1531 = add i32 %1507, 8
  %1532 = icmp eq i32 %1531, %1503
  br i1 %1532, label %8654, label %1504, !llvm.loop !16

1533:                                             ; preds = %158
  %1534 = add nsw i32 %72, 2
  store i32 %1534, ptr @ip, align 4
  %1535 = sext i32 %73 to i64
  %1536 = getelementptr inbounds i8, ptr %69, i64 %1535
  %1537 = load i8, ptr %1536, align 1
  %1538 = shl i32 %81, 13
  %1539 = xor i32 %1538, %81
  %1540 = lshr i32 %1539, 17
  %1541 = xor i32 %1540, %1539
  %1542 = shl i32 %1541, 5
  %1543 = xor i32 %1542, %1541
  store i32 %1543, ptr @vm_code_state, align 4
  %1544 = trunc i32 %1543 to i8
  %1545 = xor i8 %1537, %1544
  %1546 = add nsw i32 %72, 3
  store i32 %1546, ptr @ip, align 4
  %1547 = sext i32 %1534 to i64
  %1548 = getelementptr inbounds i8, ptr %69, i64 %1547
  %1549 = load i8, ptr %1548, align 1
  %1550 = shl i32 %1543, 13
  %1551 = xor i32 %1550, %1543
  %1552 = lshr i32 %1551, 17
  %1553 = xor i32 %1552, %1551
  %1554 = shl i32 %1553, 5
  %1555 = xor i32 %1554, %1553
  store i32 %1555, ptr @vm_code_state, align 4
  %1556 = trunc i32 %1555 to i8
  %1557 = add nsw i32 %72, 4
  store i32 %1557, ptr @ip, align 4
  %1558 = sext i32 %1546 to i64
  %1559 = getelementptr inbounds i8, ptr %69, i64 %1558
  %1560 = load i8, ptr %1559, align 1
  %1561 = shl i32 %1555, 13
  %1562 = xor i32 %1561, %1555
  %1563 = lshr i32 %1562, 17
  %1564 = xor i32 %1563, %1562
  %1565 = shl i32 %1564, 5
  %1566 = xor i32 %1565, %1564
  %1567 = trunc i32 %1566 to i8
  %1568 = xor i8 %1560, %1567
  %1569 = add nsw i32 %72, 5
  %1570 = shl i32 %1566, 13
  %1571 = xor i32 %1570, %1566
  %1572 = lshr i32 %1571, 17
  %1573 = xor i32 %1572, %1571
  %1574 = shl i32 %1573, 5
  %1575 = xor i32 %1574, %1573
  store i32 %1575, ptr @vm_code_state, align 4
  %1576 = load i32, ptr @pointer_size, align 4
  %1577 = icmp sgt i32 %1576, 0
  br i1 %1577, label %1578, label %1604

1578:                                             ; preds = %1533
  %1579 = sext i32 %1569 to i64
  %1580 = zext nneg i32 %1576 to i64
  br label %1581

1581:                                             ; preds = %1578, %1581
  %1582 = phi i64 [ %1579, %1578 ], [ %1586, %1581 ]
  %1583 = phi i64 [ 0, %1578 ], [ %1602, %1581 ]
  %1584 = phi i64 [ 0, %1578 ], [ %1601, %1581 ]
  %1585 = phi i32 [ %1575, %1578 ], [ %1595, %1581 ]
  %1586 = add nsw i64 %1582, 1
  %1587 = trunc nsw i64 %1586 to i32
  store i32 %1587, ptr @ip, align 4
  %1588 = getelementptr inbounds i8, ptr %69, i64 %1582
  %1589 = load i8, ptr %1588, align 1
  %1590 = shl i32 %1585, 13
  %1591 = xor i32 %1590, %1585
  %1592 = lshr i32 %1591, 17
  %1593 = xor i32 %1592, %1591
  %1594 = shl i32 %1593, 5
  %1595 = xor i32 %1594, %1593
  store i32 %1595, ptr @vm_code_state, align 4
  %1596 = trunc i32 %1595 to i8
  %1597 = xor i8 %1589, %1596
  %1598 = zext i8 %1597 to i64
  %1599 = shl nsw i64 %1583, 3
  %1600 = shl i64 %1598, %1599
  %1601 = or i64 %1600, %1584
  %1602 = add nuw nsw i64 %1583, 1
  %1603 = icmp eq i64 %1602, %1580
  br i1 %1603, label %1604, label %1581, !llvm.loop !15

1604:                                             ; preds = %1581, %1533
  %1605 = phi i32 [ %1575, %1533 ], [ %1595, %1581 ]
  %1606 = phi i32 [ %1569, %1533 ], [ %1587, %1581 ]
  %1607 = phi i64 [ 0, %1533 ], [ %1601, %1581 ]
  %1608 = add nsw i32 %1606, 1
  store i32 %1608, ptr @ip, align 4
  %1609 = sext i32 %1606 to i64
  %1610 = getelementptr inbounds i8, ptr %69, i64 %1609
  %1611 = load i8, ptr %1610, align 1
  %1612 = shl i32 %1605, 13
  %1613 = xor i32 %1612, %1605
  %1614 = lshr i32 %1613, 17
  %1615 = xor i32 %1614, %1613
  %1616 = shl i32 %1615, 5
  %1617 = xor i32 %1616, %1615
  store i32 %1617, ptr @vm_code_state, align 4
  %1618 = trunc i32 %1617 to i8
  %1619 = xor i8 %1611, %1618
  %1620 = add nsw i32 %1606, 2
  store i32 %1620, ptr @ip, align 4
  %1621 = sext i32 %1608 to i64
  %1622 = getelementptr inbounds i8, ptr %69, i64 %1621
  %1623 = load i8, ptr %1622, align 1
  %1624 = shl i32 %1617, 13
  %1625 = xor i32 %1624, %1617
  %1626 = lshr i32 %1625, 17
  %1627 = xor i32 %1626, %1625
  %1628 = shl i32 %1627, 5
  %1629 = xor i32 %1628, %1627
  store i32 %1629, ptr @vm_code_state, align 4
  %1630 = trunc i32 %1629 to i8
  %1631 = icmp eq i8 %1623, %1630
  br i1 %1631, label %1632, label %1711

1632:                                             ; preds = %1604
  br i1 %1577, label %1633, label %1659

1633:                                             ; preds = %1632
  %1634 = sext i32 %1620 to i64
  %1635 = zext nneg i32 %1576 to i64
  br label %1636

1636:                                             ; preds = %1633, %1636
  %1637 = phi i64 [ %1634, %1633 ], [ %1641, %1636 ]
  %1638 = phi i64 [ 0, %1633 ], [ %1657, %1636 ]
  %1639 = phi i64 [ 0, %1633 ], [ %1656, %1636 ]
  %1640 = phi i32 [ %1629, %1633 ], [ %1650, %1636 ]
  %1641 = add nsw i64 %1637, 1
  %1642 = trunc nsw i64 %1641 to i32
  store i32 %1642, ptr @ip, align 4
  %1643 = getelementptr inbounds i8, ptr %69, i64 %1637
  %1644 = load i8, ptr %1643, align 1
  %1645 = shl i32 %1640, 13
  %1646 = xor i32 %1645, %1640
  %1647 = lshr i32 %1646, 17
  %1648 = xor i32 %1647, %1646
  %1649 = shl i32 %1648, 5
  %1650 = xor i32 %1649, %1648
  store i32 %1650, ptr @vm_code_state, align 4
  %1651 = trunc i32 %1650 to i8
  %1652 = xor i8 %1644, %1651
  %1653 = zext i8 %1652 to i64
  %1654 = shl nsw i64 %1638, 3
  %1655 = shl i64 %1653, %1654
  %1656 = or i64 %1655, %1639
  %1657 = add nuw nsw i64 %1638, 1
  %1658 = icmp eq i64 %1657, %1635
  br i1 %1658, label %1659, label %1636, !llvm.loop !15

1659:                                             ; preds = %1636, %1632
  %1660 = phi i32 [ %1629, %1632 ], [ %1650, %1636 ]
  %1661 = phi i32 [ %1620, %1632 ], [ %1642, %1636 ]
  %1662 = phi i64 [ 0, %1632 ], [ %1656, %1636 ]
  %1663 = icmp eq i8 %1611, %1618
  br i1 %1663, label %1760, label %1664

1664:                                             ; preds = %1659
  %1665 = load i64, ptr @data_seg_addr, align 8
  %1666 = inttoptr i64 %1665 to ptr
  %1667 = zext i8 %1619 to i64
  %1668 = and i64 %1667, 3
  %1669 = icmp ult i8 %1619, 4
  br i1 %1669, label %1739, label %1670

1670:                                             ; preds = %1664
  %1671 = and i64 %1667, 252
  %1672 = getelementptr i8, ptr %1666, i64 1
  %1673 = getelementptr i8, ptr %1666, i64 2
  %1674 = getelementptr i8, ptr %1666, i64 3
  br label %1675

1675:                                             ; preds = %1675, %1670
  %1676 = phi i64 [ 0, %1670 ], [ %1708, %1675 ]
  %1677 = phi i64 [ 0, %1670 ], [ %1707, %1675 ]
  %1678 = phi i64 [ %1662, %1670 ], [ %1700, %1675 ]
  %1679 = phi i64 [ 0, %1670 ], [ %1709, %1675 ]
  %1680 = getelementptr inbounds nuw i8, ptr %1666, i64 %1678
  %1681 = load i8, ptr %1680, align 1
  %1682 = zext i8 %1681 to i64
  %1683 = shl nsw i64 %1676, 3
  %1684 = shl i64 %1682, %1683
  %1685 = or i64 %1684, %1677
  %1686 = getelementptr i8, ptr %1672, i64 %1678
  %1687 = load i8, ptr %1686, align 1
  %1688 = zext i8 %1687 to i64
  %1689 = shl i64 %1676, 3
  %1690 = or disjoint i64 %1689, 8
  %1691 = shl i64 %1688, %1690
  %1692 = or i64 %1691, %1685
  %1693 = getelementptr i8, ptr %1673, i64 %1678
  %1694 = load i8, ptr %1693, align 1
  %1695 = zext i8 %1694 to i64
  %1696 = shl i64 %1676, 3
  %1697 = or disjoint i64 %1696, 16
  %1698 = shl i64 %1695, %1697
  %1699 = or i64 %1698, %1692
  %1700 = add i64 %1678, 4
  %1701 = getelementptr i8, ptr %1674, i64 %1678
  %1702 = load i8, ptr %1701, align 1
  %1703 = zext i8 %1702 to i64
  %1704 = shl i64 %1676, 3
  %1705 = or disjoint i64 %1704, 24
  %1706 = shl i64 %1703, %1705
  %1707 = or i64 %1706, %1699
  %1708 = add nuw nsw i64 %1676, 4
  %1709 = add i64 %1679, 4
  %1710 = icmp eq i64 %1709, %1671
  br i1 %1710, label %1739, label %1675, !llvm.loop !17

1711:                                             ; preds = %1604
  %1712 = icmp eq i8 %1611, %1618
  br i1 %1712, label %1760, label %1713

1713:                                             ; preds = %1711
  %1714 = sext i32 %1620 to i64
  %1715 = zext i8 %1619 to i64
  br label %1716

1716:                                             ; preds = %1713, %1716
  %1717 = phi i64 [ %1714, %1713 ], [ %1721, %1716 ]
  %1718 = phi i64 [ 0, %1713 ], [ %1737, %1716 ]
  %1719 = phi i64 [ 0, %1713 ], [ %1736, %1716 ]
  %1720 = phi i32 [ %1629, %1713 ], [ %1730, %1716 ]
  %1721 = add nsw i64 %1717, 1
  %1722 = trunc nsw i64 %1721 to i32
  store i32 %1722, ptr @ip, align 4
  %1723 = getelementptr inbounds i8, ptr %69, i64 %1717
  %1724 = load i8, ptr %1723, align 1
  %1725 = shl i32 %1720, 13
  %1726 = xor i32 %1725, %1720
  %1727 = lshr i32 %1726, 17
  %1728 = xor i32 %1727, %1726
  %1729 = shl i32 %1728, 5
  %1730 = xor i32 %1729, %1728
  store i32 %1730, ptr @vm_code_state, align 4
  %1731 = trunc i32 %1730 to i8
  %1732 = xor i8 %1724, %1731
  %1733 = zext i8 %1732 to i64
  %1734 = shl nsw i64 %1718, 3
  %1735 = shl i64 %1733, %1734
  %1736 = or i64 %1735, %1719
  %1737 = add nuw nsw i64 %1718, 1
  %1738 = icmp eq i64 %1737, %1715
  br i1 %1738, label %1760, label %1716, !llvm.loop !15

1739:                                             ; preds = %1675, %1664
  %1740 = phi i64 [ poison, %1664 ], [ %1707, %1675 ]
  %1741 = phi i64 [ 0, %1664 ], [ %1708, %1675 ]
  %1742 = phi i64 [ 0, %1664 ], [ %1707, %1675 ]
  %1743 = phi i64 [ %1662, %1664 ], [ %1700, %1675 ]
  %1744 = icmp eq i64 %1668, 0
  br i1 %1744, label %1760, label %1745

1745:                                             ; preds = %1739, %1745
  %1746 = phi i64 [ %1757, %1745 ], [ %1741, %1739 ]
  %1747 = phi i64 [ %1756, %1745 ], [ %1742, %1739 ]
  %1748 = phi i64 [ %1750, %1745 ], [ %1743, %1739 ]
  %1749 = phi i64 [ %1758, %1745 ], [ 0, %1739 ]
  %1750 = add i64 %1748, 1
  %1751 = getelementptr inbounds nuw i8, ptr %1666, i64 %1748
  %1752 = load i8, ptr %1751, align 1
  %1753 = zext i8 %1752 to i64
  %1754 = shl nsw i64 %1746, 3
  %1755 = shl i64 %1753, %1754
  %1756 = or i64 %1755, %1747
  %1757 = add nuw nsw i64 %1746, 1
  %1758 = add i64 %1749, 1
  %1759 = icmp eq i64 %1758, %1668
  br i1 %1759, label %1760, label %1745, !llvm.loop !27

1760:                                             ; preds = %1716, %1739, %1745, %1711, %1659
  %1761 = phi i32 [ %1660, %1659 ], [ %1629, %1711 ], [ %1660, %1745 ], [ %1660, %1739 ], [ %1730, %1716 ]
  %1762 = phi i32 [ %1661, %1659 ], [ %1620, %1711 ], [ %1661, %1745 ], [ %1661, %1739 ], [ %1722, %1716 ]
  %1763 = phi i64 [ 0, %1659 ], [ 0, %1711 ], [ %1740, %1739 ], [ %1756, %1745 ], [ %1736, %1716 ]
  %1764 = add nsw i32 %1762, 1
  store i32 %1764, ptr @ip, align 4
  %1765 = sext i32 %1762 to i64
  %1766 = getelementptr inbounds i8, ptr %69, i64 %1765
  %1767 = load i8, ptr %1766, align 1
  %1768 = shl i32 %1761, 13
  %1769 = xor i32 %1768, %1761
  %1770 = lshr i32 %1769, 17
  %1771 = xor i32 %1770, %1769
  %1772 = shl i32 %1771, 5
  %1773 = xor i32 %1772, %1771
  store i32 %1773, ptr @vm_code_state, align 4
  %1774 = trunc i32 %1773 to i8
  %1775 = xor i8 %1767, %1774
  %1776 = add nsw i32 %1762, 2
  store i32 %1776, ptr @ip, align 4
  %1777 = sext i32 %1764 to i64
  %1778 = getelementptr inbounds i8, ptr %69, i64 %1777
  %1779 = load i8, ptr %1778, align 1
  %1780 = shl i32 %1773, 13
  %1781 = xor i32 %1780, %1773
  %1782 = lshr i32 %1781, 17
  %1783 = xor i32 %1782, %1781
  %1784 = shl i32 %1783, 5
  %1785 = xor i32 %1784, %1783
  store i32 %1785, ptr @vm_code_state, align 4
  %1786 = trunc i32 %1785 to i8
  %1787 = icmp eq i8 %1779, %1786
  br i1 %1787, label %1788, label %1865

1788:                                             ; preds = %1760
  br i1 %1577, label %1789, label %1815

1789:                                             ; preds = %1788
  %1790 = sext i32 %1776 to i64
  %1791 = zext nneg i32 %1576 to i64
  br label %1792

1792:                                             ; preds = %1789, %1792
  %1793 = phi i64 [ %1790, %1789 ], [ %1797, %1792 ]
  %1794 = phi i64 [ 0, %1789 ], [ %1813, %1792 ]
  %1795 = phi i64 [ 0, %1789 ], [ %1812, %1792 ]
  %1796 = phi i32 [ %1785, %1789 ], [ %1806, %1792 ]
  %1797 = add nsw i64 %1793, 1
  %1798 = trunc nsw i64 %1797 to i32
  store i32 %1798, ptr @ip, align 4
  %1799 = getelementptr inbounds i8, ptr %69, i64 %1793
  %1800 = load i8, ptr %1799, align 1
  %1801 = shl i32 %1796, 13
  %1802 = xor i32 %1801, %1796
  %1803 = lshr i32 %1802, 17
  %1804 = xor i32 %1803, %1802
  %1805 = shl i32 %1804, 5
  %1806 = xor i32 %1805, %1804
  store i32 %1806, ptr @vm_code_state, align 4
  %1807 = trunc i32 %1806 to i8
  %1808 = xor i8 %1800, %1807
  %1809 = zext i8 %1808 to i64
  %1810 = shl nsw i64 %1794, 3
  %1811 = shl i64 %1809, %1810
  %1812 = or i64 %1811, %1795
  %1813 = add nuw nsw i64 %1794, 1
  %1814 = icmp eq i64 %1813, %1791
  br i1 %1814, label %1815, label %1792, !llvm.loop !15

1815:                                             ; preds = %1792, %1788
  %1816 = phi i64 [ 0, %1788 ], [ %1812, %1792 ]
  %1817 = icmp eq i8 %1767, %1774
  br i1 %1817, label %1914, label %1818

1818:                                             ; preds = %1815
  %1819 = load i64, ptr @data_seg_addr, align 8
  %1820 = inttoptr i64 %1819 to ptr
  %1821 = zext i8 %1775 to i64
  %1822 = and i64 %1821, 3
  %1823 = icmp ult i8 %1775, 4
  br i1 %1823, label %1893, label %1824

1824:                                             ; preds = %1818
  %1825 = and i64 %1821, 252
  %1826 = getelementptr i8, ptr %1820, i64 1
  %1827 = getelementptr i8, ptr %1820, i64 2
  %1828 = getelementptr i8, ptr %1820, i64 3
  br label %1829

1829:                                             ; preds = %1829, %1824
  %1830 = phi i64 [ 0, %1824 ], [ %1862, %1829 ]
  %1831 = phi i64 [ 0, %1824 ], [ %1861, %1829 ]
  %1832 = phi i64 [ %1816, %1824 ], [ %1854, %1829 ]
  %1833 = phi i64 [ 0, %1824 ], [ %1863, %1829 ]
  %1834 = getelementptr inbounds nuw i8, ptr %1820, i64 %1832
  %1835 = load i8, ptr %1834, align 1
  %1836 = zext i8 %1835 to i64
  %1837 = shl nsw i64 %1830, 3
  %1838 = shl i64 %1836, %1837
  %1839 = or i64 %1838, %1831
  %1840 = getelementptr i8, ptr %1826, i64 %1832
  %1841 = load i8, ptr %1840, align 1
  %1842 = zext i8 %1841 to i64
  %1843 = shl i64 %1830, 3
  %1844 = or disjoint i64 %1843, 8
  %1845 = shl i64 %1842, %1844
  %1846 = or i64 %1845, %1839
  %1847 = getelementptr i8, ptr %1827, i64 %1832
  %1848 = load i8, ptr %1847, align 1
  %1849 = zext i8 %1848 to i64
  %1850 = shl i64 %1830, 3
  %1851 = or disjoint i64 %1850, 16
  %1852 = shl i64 %1849, %1851
  %1853 = or i64 %1852, %1846
  %1854 = add i64 %1832, 4
  %1855 = getelementptr i8, ptr %1828, i64 %1832
  %1856 = load i8, ptr %1855, align 1
  %1857 = zext i8 %1856 to i64
  %1858 = shl i64 %1830, 3
  %1859 = or disjoint i64 %1858, 24
  %1860 = shl i64 %1857, %1859
  %1861 = or i64 %1860, %1853
  %1862 = add nuw nsw i64 %1830, 4
  %1863 = add i64 %1833, 4
  %1864 = icmp eq i64 %1863, %1825
  br i1 %1864, label %1893, label %1829, !llvm.loop !17

1865:                                             ; preds = %1760
  %1866 = icmp eq i8 %1767, %1774
  br i1 %1866, label %1914, label %1867

1867:                                             ; preds = %1865
  %1868 = sext i32 %1776 to i64
  %1869 = zext i8 %1775 to i64
  br label %1870

1870:                                             ; preds = %1867, %1870
  %1871 = phi i64 [ %1868, %1867 ], [ %1875, %1870 ]
  %1872 = phi i64 [ 0, %1867 ], [ %1891, %1870 ]
  %1873 = phi i64 [ 0, %1867 ], [ %1890, %1870 ]
  %1874 = phi i32 [ %1785, %1867 ], [ %1884, %1870 ]
  %1875 = add nsw i64 %1871, 1
  %1876 = trunc nsw i64 %1875 to i32
  store i32 %1876, ptr @ip, align 4
  %1877 = getelementptr inbounds i8, ptr %69, i64 %1871
  %1878 = load i8, ptr %1877, align 1
  %1879 = shl i32 %1874, 13
  %1880 = xor i32 %1879, %1874
  %1881 = lshr i32 %1880, 17
  %1882 = xor i32 %1881, %1880
  %1883 = shl i32 %1882, 5
  %1884 = xor i32 %1883, %1882
  store i32 %1884, ptr @vm_code_state, align 4
  %1885 = trunc i32 %1884 to i8
  %1886 = xor i8 %1878, %1885
  %1887 = zext i8 %1886 to i64
  %1888 = shl nsw i64 %1872, 3
  %1889 = shl i64 %1887, %1888
  %1890 = or i64 %1889, %1873
  %1891 = add nuw nsw i64 %1872, 1
  %1892 = icmp eq i64 %1891, %1869
  br i1 %1892, label %1914, label %1870, !llvm.loop !15

1893:                                             ; preds = %1829, %1818
  %1894 = phi i64 [ poison, %1818 ], [ %1861, %1829 ]
  %1895 = phi i64 [ 0, %1818 ], [ %1862, %1829 ]
  %1896 = phi i64 [ 0, %1818 ], [ %1861, %1829 ]
  %1897 = phi i64 [ %1816, %1818 ], [ %1854, %1829 ]
  %1898 = icmp eq i64 %1822, 0
  br i1 %1898, label %1914, label %1899

1899:                                             ; preds = %1893, %1899
  %1900 = phi i64 [ %1911, %1899 ], [ %1895, %1893 ]
  %1901 = phi i64 [ %1910, %1899 ], [ %1896, %1893 ]
  %1902 = phi i64 [ %1904, %1899 ], [ %1897, %1893 ]
  %1903 = phi i64 [ %1912, %1899 ], [ 0, %1893 ]
  %1904 = add i64 %1902, 1
  %1905 = getelementptr inbounds nuw i8, ptr %1820, i64 %1902
  %1906 = load i8, ptr %1905, align 1
  %1907 = zext i8 %1906 to i64
  %1908 = shl nsw i64 %1900, 3
  %1909 = shl i64 %1907, %1908
  %1910 = or i64 %1909, %1901
  %1911 = add nuw nsw i64 %1900, 1
  %1912 = add i64 %1903, 1
  %1913 = icmp eq i64 %1912, %1822
  br i1 %1913, label %1914, label %1899, !llvm.loop !28

1914:                                             ; preds = %1870, %1893, %1899, %1865, %1815
  %1915 = phi i64 [ 0, %1815 ], [ 0, %1865 ], [ %1894, %1893 ], [ %1910, %1899 ], [ %1890, %1870 ]
  %1916 = icmp eq i8 %1560, %1567
  br i1 %1916, label %8784, label %1917

1917:                                             ; preds = %1914
  %1918 = load i64, ptr @data_seg_addr, align 8
  %1919 = add i64 %1918, %1607
  %1920 = inttoptr i64 %1919 to ptr
  %1921 = zext i8 %1568 to i32
  %1922 = icmp ne i8 %1537, %1544
  %1923 = icmp ne i8 %1549, %1556
  %1924 = select i1 %1922, i1 %1923, i1 false
  %1925 = zext i8 %1545 to i64
  %1926 = select i1 %1924, i64 %1925, i64 1
  %1927 = mul i64 %1915, %1926
  %1928 = add i64 %1927, %1763
  %1929 = and i32 %1921, 7
  %1930 = icmp ult i8 %1568, 8
  br i1 %1930, label %8667, label %1931

1931:                                             ; preds = %1917
  %1932 = and i32 %1921, 248
  br label %1933

1933:                                             ; preds = %1933, %1931
  %1934 = phi ptr [ %1920, %1931 ], [ %1959, %1933 ]
  %1935 = phi i64 [ %1928, %1931 ], [ 0, %1933 ]
  %1936 = phi i32 [ 0, %1931 ], [ %1960, %1933 ]
  %1937 = trunc i64 %1935 to i8
  store i8 %1937, ptr %1934, align 1
  %1938 = getelementptr inbounds nuw i8, ptr %1934, i64 1
  %1939 = lshr i64 %1935, 8
  %1940 = trunc i64 %1939 to i8
  store i8 %1940, ptr %1938, align 1
  %1941 = getelementptr inbounds nuw i8, ptr %1934, i64 2
  %1942 = lshr i64 %1935, 16
  %1943 = trunc i64 %1942 to i8
  store i8 %1943, ptr %1941, align 1
  %1944 = getelementptr inbounds nuw i8, ptr %1934, i64 3
  %1945 = lshr i64 %1935, 24
  %1946 = trunc i64 %1945 to i8
  store i8 %1946, ptr %1944, align 1
  %1947 = getelementptr inbounds nuw i8, ptr %1934, i64 4
  %1948 = lshr i64 %1935, 32
  %1949 = trunc i64 %1948 to i8
  store i8 %1949, ptr %1947, align 1
  %1950 = getelementptr inbounds nuw i8, ptr %1934, i64 5
  %1951 = lshr i64 %1935, 40
  %1952 = trunc i64 %1951 to i8
  store i8 %1952, ptr %1950, align 1
  %1953 = getelementptr inbounds nuw i8, ptr %1934, i64 6
  %1954 = lshr i64 %1935, 48
  %1955 = trunc i64 %1954 to i8
  store i8 %1955, ptr %1953, align 1
  %1956 = getelementptr inbounds nuw i8, ptr %1934, i64 7
  %1957 = lshr i64 %1935, 56
  %1958 = trunc nuw i64 %1957 to i8
  store i8 %1958, ptr %1956, align 1
  %1959 = getelementptr inbounds nuw i8, ptr %1934, i64 8
  %1960 = add i32 %1936, 8
  %1961 = icmp eq i32 %1960, %1932
  br i1 %1961, label %8667, label %1933, !llvm.loop !16

1962:                                             ; preds = %158
  %1963 = add nsw i32 %72, 2
  store i32 %1963, ptr @ip, align 4
  %1964 = sext i32 %73 to i64
  %1965 = getelementptr inbounds i8, ptr %69, i64 %1964
  %1966 = load i8, ptr %1965, align 1
  %1967 = shl i32 %81, 13
  %1968 = xor i32 %1967, %81
  %1969 = lshr i32 %1968, 17
  %1970 = xor i32 %1969, %1968
  %1971 = shl i32 %1970, 5
  %1972 = xor i32 %1971, %1970
  store i32 %1972, ptr @vm_code_state, align 4
  %1973 = trunc i32 %1972 to i8
  %1974 = xor i8 %1966, %1973
  %1975 = add nsw i32 %72, 3
  store i32 %1975, ptr @ip, align 4
  %1976 = sext i32 %1963 to i64
  %1977 = getelementptr inbounds i8, ptr %69, i64 %1976
  %1978 = load i8, ptr %1977, align 1
  %1979 = shl i32 %1972, 13
  %1980 = xor i32 %1979, %1972
  %1981 = lshr i32 %1980, 17
  %1982 = xor i32 %1981, %1980
  %1983 = shl i32 %1982, 5
  %1984 = xor i32 %1983, %1982
  store i32 %1984, ptr @vm_code_state, align 4
  %1985 = trunc i32 %1984 to i8
  %1986 = xor i8 %1978, %1985
  %1987 = add nsw i32 %72, 4
  store i32 %1987, ptr @ip, align 4
  %1988 = sext i32 %1975 to i64
  %1989 = getelementptr inbounds i8, ptr %69, i64 %1988
  %1990 = load i8, ptr %1989, align 1
  %1991 = shl i32 %1984, 13
  %1992 = xor i32 %1991, %1984
  %1993 = lshr i32 %1992, 17
  %1994 = xor i32 %1993, %1992
  %1995 = shl i32 %1994, 5
  %1996 = xor i32 %1995, %1994
  %1997 = trunc i32 %1996 to i8
  %1998 = xor i8 %1990, %1997
  %1999 = add nsw i32 %72, 5
  %2000 = shl i32 %1996, 13
  %2001 = xor i32 %2000, %1996
  %2002 = lshr i32 %2001, 17
  %2003 = xor i32 %2002, %2001
  %2004 = shl i32 %2003, 5
  %2005 = xor i32 %2004, %2003
  store i32 %2005, ptr @vm_code_state, align 4
  %2006 = load i32, ptr @pointer_size, align 4
  %2007 = icmp sgt i32 %2006, 0
  br i1 %2007, label %2008, label %2034

2008:                                             ; preds = %1962
  %2009 = sext i32 %1999 to i64
  %2010 = zext nneg i32 %2006 to i64
  br label %2011

2011:                                             ; preds = %2008, %2011
  %2012 = phi i64 [ %2009, %2008 ], [ %2016, %2011 ]
  %2013 = phi i64 [ 0, %2008 ], [ %2032, %2011 ]
  %2014 = phi i64 [ 0, %2008 ], [ %2031, %2011 ]
  %2015 = phi i32 [ %2005, %2008 ], [ %2025, %2011 ]
  %2016 = add nsw i64 %2012, 1
  %2017 = trunc nsw i64 %2016 to i32
  store i32 %2017, ptr @ip, align 4
  %2018 = getelementptr inbounds i8, ptr %69, i64 %2012
  %2019 = load i8, ptr %2018, align 1
  %2020 = shl i32 %2015, 13
  %2021 = xor i32 %2020, %2015
  %2022 = lshr i32 %2021, 17
  %2023 = xor i32 %2022, %2021
  %2024 = shl i32 %2023, 5
  %2025 = xor i32 %2024, %2023
  store i32 %2025, ptr @vm_code_state, align 4
  %2026 = trunc i32 %2025 to i8
  %2027 = xor i8 %2019, %2026
  %2028 = zext i8 %2027 to i64
  %2029 = shl nsw i64 %2013, 3
  %2030 = shl i64 %2028, %2029
  %2031 = or i64 %2030, %2014
  %2032 = add nuw nsw i64 %2013, 1
  %2033 = icmp eq i64 %2032, %2010
  br i1 %2033, label %2034, label %2011, !llvm.loop !15

2034:                                             ; preds = %2011, %1962
  %2035 = phi i32 [ %2005, %1962 ], [ %2025, %2011 ]
  %2036 = phi i32 [ %1999, %1962 ], [ %2017, %2011 ]
  %2037 = phi i64 [ 0, %1962 ], [ %2031, %2011 ]
  %2038 = add nsw i32 %2036, 1
  store i32 %2038, ptr @ip, align 4
  %2039 = sext i32 %2036 to i64
  %2040 = getelementptr inbounds i8, ptr %69, i64 %2039
  %2041 = load i8, ptr %2040, align 1
  %2042 = shl i32 %2035, 13
  %2043 = xor i32 %2042, %2035
  %2044 = lshr i32 %2043, 17
  %2045 = xor i32 %2044, %2043
  %2046 = shl i32 %2045, 5
  %2047 = xor i32 %2046, %2045
  store i32 %2047, ptr @vm_code_state, align 4
  %2048 = trunc i32 %2047 to i8
  %2049 = xor i8 %2041, %2048
  %2050 = add nsw i32 %2036, 2
  store i32 %2050, ptr @ip, align 4
  %2051 = sext i32 %2038 to i64
  %2052 = getelementptr inbounds i8, ptr %69, i64 %2051
  %2053 = load i8, ptr %2052, align 1
  %2054 = shl i32 %2047, 13
  %2055 = xor i32 %2054, %2047
  %2056 = lshr i32 %2055, 17
  %2057 = xor i32 %2056, %2055
  %2058 = shl i32 %2057, 5
  %2059 = xor i32 %2058, %2057
  store i32 %2059, ptr @vm_code_state, align 4
  %2060 = trunc i32 %2059 to i8
  %2061 = icmp eq i8 %2053, %2060
  br i1 %2061, label %2062, label %2141

2062:                                             ; preds = %2034
  br i1 %2007, label %2063, label %2089

2063:                                             ; preds = %2062
  %2064 = sext i32 %2050 to i64
  %2065 = zext nneg i32 %2006 to i64
  br label %2066

2066:                                             ; preds = %2063, %2066
  %2067 = phi i64 [ %2064, %2063 ], [ %2071, %2066 ]
  %2068 = phi i64 [ 0, %2063 ], [ %2087, %2066 ]
  %2069 = phi i64 [ 0, %2063 ], [ %2086, %2066 ]
  %2070 = phi i32 [ %2059, %2063 ], [ %2080, %2066 ]
  %2071 = add nsw i64 %2067, 1
  %2072 = trunc nsw i64 %2071 to i32
  store i32 %2072, ptr @ip, align 4
  %2073 = getelementptr inbounds i8, ptr %69, i64 %2067
  %2074 = load i8, ptr %2073, align 1
  %2075 = shl i32 %2070, 13
  %2076 = xor i32 %2075, %2070
  %2077 = lshr i32 %2076, 17
  %2078 = xor i32 %2077, %2076
  %2079 = shl i32 %2078, 5
  %2080 = xor i32 %2079, %2078
  store i32 %2080, ptr @vm_code_state, align 4
  %2081 = trunc i32 %2080 to i8
  %2082 = xor i8 %2074, %2081
  %2083 = zext i8 %2082 to i64
  %2084 = shl nsw i64 %2068, 3
  %2085 = shl i64 %2083, %2084
  %2086 = or i64 %2085, %2069
  %2087 = add nuw nsw i64 %2068, 1
  %2088 = icmp eq i64 %2087, %2065
  br i1 %2088, label %2089, label %2066, !llvm.loop !15

2089:                                             ; preds = %2066, %2062
  %2090 = phi i32 [ %2059, %2062 ], [ %2080, %2066 ]
  %2091 = phi i32 [ %2050, %2062 ], [ %2072, %2066 ]
  %2092 = phi i64 [ 0, %2062 ], [ %2086, %2066 ]
  %2093 = icmp eq i8 %2041, %2048
  br i1 %2093, label %2190, label %2094

2094:                                             ; preds = %2089
  %2095 = load i64, ptr @data_seg_addr, align 8
  %2096 = inttoptr i64 %2095 to ptr
  %2097 = zext i8 %2049 to i64
  %2098 = and i64 %2097, 3
  %2099 = icmp ult i8 %2049, 4
  br i1 %2099, label %2169, label %2100

2100:                                             ; preds = %2094
  %2101 = and i64 %2097, 252
  %2102 = getelementptr i8, ptr %2096, i64 1
  %2103 = getelementptr i8, ptr %2096, i64 2
  %2104 = getelementptr i8, ptr %2096, i64 3
  br label %2105

2105:                                             ; preds = %2105, %2100
  %2106 = phi i64 [ 0, %2100 ], [ %2138, %2105 ]
  %2107 = phi i64 [ 0, %2100 ], [ %2137, %2105 ]
  %2108 = phi i64 [ %2092, %2100 ], [ %2130, %2105 ]
  %2109 = phi i64 [ 0, %2100 ], [ %2139, %2105 ]
  %2110 = getelementptr inbounds nuw i8, ptr %2096, i64 %2108
  %2111 = load i8, ptr %2110, align 1
  %2112 = zext i8 %2111 to i64
  %2113 = shl nsw i64 %2106, 3
  %2114 = shl i64 %2112, %2113
  %2115 = or i64 %2114, %2107
  %2116 = getelementptr i8, ptr %2102, i64 %2108
  %2117 = load i8, ptr %2116, align 1
  %2118 = zext i8 %2117 to i64
  %2119 = shl i64 %2106, 3
  %2120 = or disjoint i64 %2119, 8
  %2121 = shl i64 %2118, %2120
  %2122 = or i64 %2121, %2115
  %2123 = getelementptr i8, ptr %2103, i64 %2108
  %2124 = load i8, ptr %2123, align 1
  %2125 = zext i8 %2124 to i64
  %2126 = shl i64 %2106, 3
  %2127 = or disjoint i64 %2126, 16
  %2128 = shl i64 %2125, %2127
  %2129 = or i64 %2128, %2122
  %2130 = add i64 %2108, 4
  %2131 = getelementptr i8, ptr %2104, i64 %2108
  %2132 = load i8, ptr %2131, align 1
  %2133 = zext i8 %2132 to i64
  %2134 = shl i64 %2106, 3
  %2135 = or disjoint i64 %2134, 24
  %2136 = shl i64 %2133, %2135
  %2137 = or i64 %2136, %2129
  %2138 = add nuw nsw i64 %2106, 4
  %2139 = add i64 %2109, 4
  %2140 = icmp eq i64 %2139, %2101
  br i1 %2140, label %2169, label %2105, !llvm.loop !17

2141:                                             ; preds = %2034
  %2142 = icmp eq i8 %2041, %2048
  br i1 %2142, label %2190, label %2143

2143:                                             ; preds = %2141
  %2144 = sext i32 %2050 to i64
  %2145 = zext i8 %2049 to i64
  br label %2146

2146:                                             ; preds = %2143, %2146
  %2147 = phi i64 [ %2144, %2143 ], [ %2151, %2146 ]
  %2148 = phi i64 [ 0, %2143 ], [ %2167, %2146 ]
  %2149 = phi i64 [ 0, %2143 ], [ %2166, %2146 ]
  %2150 = phi i32 [ %2059, %2143 ], [ %2160, %2146 ]
  %2151 = add nsw i64 %2147, 1
  %2152 = trunc nsw i64 %2151 to i32
  store i32 %2152, ptr @ip, align 4
  %2153 = getelementptr inbounds i8, ptr %69, i64 %2147
  %2154 = load i8, ptr %2153, align 1
  %2155 = shl i32 %2150, 13
  %2156 = xor i32 %2155, %2150
  %2157 = lshr i32 %2156, 17
  %2158 = xor i32 %2157, %2156
  %2159 = shl i32 %2158, 5
  %2160 = xor i32 %2159, %2158
  store i32 %2160, ptr @vm_code_state, align 4
  %2161 = trunc i32 %2160 to i8
  %2162 = xor i8 %2154, %2161
  %2163 = zext i8 %2162 to i64
  %2164 = shl nsw i64 %2148, 3
  %2165 = shl i64 %2163, %2164
  %2166 = or i64 %2165, %2149
  %2167 = add nuw nsw i64 %2148, 1
  %2168 = icmp eq i64 %2167, %2145
  br i1 %2168, label %2190, label %2146, !llvm.loop !15

2169:                                             ; preds = %2105, %2094
  %2170 = phi i64 [ poison, %2094 ], [ %2137, %2105 ]
  %2171 = phi i64 [ 0, %2094 ], [ %2138, %2105 ]
  %2172 = phi i64 [ 0, %2094 ], [ %2137, %2105 ]
  %2173 = phi i64 [ %2092, %2094 ], [ %2130, %2105 ]
  %2174 = icmp eq i64 %2098, 0
  br i1 %2174, label %2190, label %2175

2175:                                             ; preds = %2169, %2175
  %2176 = phi i64 [ %2187, %2175 ], [ %2171, %2169 ]
  %2177 = phi i64 [ %2186, %2175 ], [ %2172, %2169 ]
  %2178 = phi i64 [ %2180, %2175 ], [ %2173, %2169 ]
  %2179 = phi i64 [ %2188, %2175 ], [ 0, %2169 ]
  %2180 = add i64 %2178, 1
  %2181 = getelementptr inbounds nuw i8, ptr %2096, i64 %2178
  %2182 = load i8, ptr %2181, align 1
  %2183 = zext i8 %2182 to i64
  %2184 = shl nsw i64 %2176, 3
  %2185 = shl i64 %2183, %2184
  %2186 = or i64 %2185, %2177
  %2187 = add nuw nsw i64 %2176, 1
  %2188 = add i64 %2179, 1
  %2189 = icmp eq i64 %2188, %2098
  br i1 %2189, label %2190, label %2175, !llvm.loop !29

2190:                                             ; preds = %2146, %2169, %2175, %2141, %2089
  %2191 = phi i32 [ %2090, %2089 ], [ %2059, %2141 ], [ %2090, %2175 ], [ %2090, %2169 ], [ %2160, %2146 ]
  %2192 = phi i32 [ %2091, %2089 ], [ %2050, %2141 ], [ %2091, %2175 ], [ %2091, %2169 ], [ %2152, %2146 ]
  %2193 = phi i64 [ 0, %2089 ], [ 0, %2141 ], [ %2170, %2169 ], [ %2186, %2175 ], [ %2166, %2146 ]
  %2194 = add nsw i32 %2192, 1
  store i32 %2194, ptr @ip, align 4
  %2195 = sext i32 %2192 to i64
  %2196 = getelementptr inbounds i8, ptr %69, i64 %2195
  %2197 = load i8, ptr %2196, align 1
  %2198 = shl i32 %2191, 13
  %2199 = xor i32 %2198, %2191
  %2200 = lshr i32 %2199, 17
  %2201 = xor i32 %2200, %2199
  %2202 = shl i32 %2201, 5
  %2203 = xor i32 %2202, %2201
  store i32 %2203, ptr @vm_code_state, align 4
  %2204 = trunc i32 %2203 to i8
  %2205 = xor i8 %2197, %2204
  %2206 = add nsw i32 %2192, 2
  store i32 %2206, ptr @ip, align 4
  %2207 = sext i32 %2194 to i64
  %2208 = getelementptr inbounds i8, ptr %69, i64 %2207
  %2209 = load i8, ptr %2208, align 1
  %2210 = shl i32 %2203, 13
  %2211 = xor i32 %2210, %2203
  %2212 = lshr i32 %2211, 17
  %2213 = xor i32 %2212, %2211
  %2214 = shl i32 %2213, 5
  %2215 = xor i32 %2214, %2213
  store i32 %2215, ptr @vm_code_state, align 4
  %2216 = trunc i32 %2215 to i8
  %2217 = icmp eq i8 %2209, %2216
  br i1 %2217, label %2218, label %2295

2218:                                             ; preds = %2190
  br i1 %2007, label %2219, label %2245

2219:                                             ; preds = %2218
  %2220 = sext i32 %2206 to i64
  %2221 = zext nneg i32 %2006 to i64
  br label %2222

2222:                                             ; preds = %2219, %2222
  %2223 = phi i64 [ %2220, %2219 ], [ %2227, %2222 ]
  %2224 = phi i64 [ 0, %2219 ], [ %2243, %2222 ]
  %2225 = phi i64 [ 0, %2219 ], [ %2242, %2222 ]
  %2226 = phi i32 [ %2215, %2219 ], [ %2236, %2222 ]
  %2227 = add nsw i64 %2223, 1
  %2228 = trunc nsw i64 %2227 to i32
  store i32 %2228, ptr @ip, align 4
  %2229 = getelementptr inbounds i8, ptr %69, i64 %2223
  %2230 = load i8, ptr %2229, align 1
  %2231 = shl i32 %2226, 13
  %2232 = xor i32 %2231, %2226
  %2233 = lshr i32 %2232, 17
  %2234 = xor i32 %2233, %2232
  %2235 = shl i32 %2234, 5
  %2236 = xor i32 %2235, %2234
  store i32 %2236, ptr @vm_code_state, align 4
  %2237 = trunc i32 %2236 to i8
  %2238 = xor i8 %2230, %2237
  %2239 = zext i8 %2238 to i64
  %2240 = shl nsw i64 %2224, 3
  %2241 = shl i64 %2239, %2240
  %2242 = or i64 %2241, %2225
  %2243 = add nuw nsw i64 %2224, 1
  %2244 = icmp eq i64 %2243, %2221
  br i1 %2244, label %2245, label %2222, !llvm.loop !15

2245:                                             ; preds = %2222, %2218
  %2246 = phi i64 [ 0, %2218 ], [ %2242, %2222 ]
  %2247 = icmp eq i8 %2197, %2204
  br i1 %2247, label %2344, label %2248

2248:                                             ; preds = %2245
  %2249 = load i64, ptr @data_seg_addr, align 8
  %2250 = inttoptr i64 %2249 to ptr
  %2251 = zext i8 %2205 to i64
  %2252 = and i64 %2251, 3
  %2253 = icmp ult i8 %2205, 4
  br i1 %2253, label %2323, label %2254

2254:                                             ; preds = %2248
  %2255 = and i64 %2251, 252
  %2256 = getelementptr i8, ptr %2250, i64 1
  %2257 = getelementptr i8, ptr %2250, i64 2
  %2258 = getelementptr i8, ptr %2250, i64 3
  br label %2259

2259:                                             ; preds = %2259, %2254
  %2260 = phi i64 [ 0, %2254 ], [ %2292, %2259 ]
  %2261 = phi i64 [ 0, %2254 ], [ %2291, %2259 ]
  %2262 = phi i64 [ %2246, %2254 ], [ %2284, %2259 ]
  %2263 = phi i64 [ 0, %2254 ], [ %2293, %2259 ]
  %2264 = getelementptr inbounds nuw i8, ptr %2250, i64 %2262
  %2265 = load i8, ptr %2264, align 1
  %2266 = zext i8 %2265 to i64
  %2267 = shl nsw i64 %2260, 3
  %2268 = shl i64 %2266, %2267
  %2269 = or i64 %2268, %2261
  %2270 = getelementptr i8, ptr %2256, i64 %2262
  %2271 = load i8, ptr %2270, align 1
  %2272 = zext i8 %2271 to i64
  %2273 = shl i64 %2260, 3
  %2274 = or disjoint i64 %2273, 8
  %2275 = shl i64 %2272, %2274
  %2276 = or i64 %2275, %2269
  %2277 = getelementptr i8, ptr %2257, i64 %2262
  %2278 = load i8, ptr %2277, align 1
  %2279 = zext i8 %2278 to i64
  %2280 = shl i64 %2260, 3
  %2281 = or disjoint i64 %2280, 16
  %2282 = shl i64 %2279, %2281
  %2283 = or i64 %2282, %2276
  %2284 = add i64 %2262, 4
  %2285 = getelementptr i8, ptr %2258, i64 %2262
  %2286 = load i8, ptr %2285, align 1
  %2287 = zext i8 %2286 to i64
  %2288 = shl i64 %2260, 3
  %2289 = or disjoint i64 %2288, 24
  %2290 = shl i64 %2287, %2289
  %2291 = or i64 %2290, %2283
  %2292 = add nuw nsw i64 %2260, 4
  %2293 = add i64 %2263, 4
  %2294 = icmp eq i64 %2293, %2255
  br i1 %2294, label %2323, label %2259, !llvm.loop !17

2295:                                             ; preds = %2190
  %2296 = icmp eq i8 %2197, %2204
  br i1 %2296, label %2344, label %2297

2297:                                             ; preds = %2295
  %2298 = sext i32 %2206 to i64
  %2299 = zext i8 %2205 to i64
  br label %2300

2300:                                             ; preds = %2297, %2300
  %2301 = phi i64 [ %2298, %2297 ], [ %2305, %2300 ]
  %2302 = phi i64 [ 0, %2297 ], [ %2321, %2300 ]
  %2303 = phi i64 [ 0, %2297 ], [ %2320, %2300 ]
  %2304 = phi i32 [ %2215, %2297 ], [ %2314, %2300 ]
  %2305 = add nsw i64 %2301, 1
  %2306 = trunc nsw i64 %2305 to i32
  store i32 %2306, ptr @ip, align 4
  %2307 = getelementptr inbounds i8, ptr %69, i64 %2301
  %2308 = load i8, ptr %2307, align 1
  %2309 = shl i32 %2304, 13
  %2310 = xor i32 %2309, %2304
  %2311 = lshr i32 %2310, 17
  %2312 = xor i32 %2311, %2310
  %2313 = shl i32 %2312, 5
  %2314 = xor i32 %2313, %2312
  store i32 %2314, ptr @vm_code_state, align 4
  %2315 = trunc i32 %2314 to i8
  %2316 = xor i8 %2308, %2315
  %2317 = zext i8 %2316 to i64
  %2318 = shl nsw i64 %2302, 3
  %2319 = shl i64 %2317, %2318
  %2320 = or i64 %2319, %2303
  %2321 = add nuw nsw i64 %2302, 1
  %2322 = icmp eq i64 %2321, %2299
  br i1 %2322, label %2344, label %2300, !llvm.loop !15

2323:                                             ; preds = %2259, %2248
  %2324 = phi i64 [ poison, %2248 ], [ %2291, %2259 ]
  %2325 = phi i64 [ 0, %2248 ], [ %2292, %2259 ]
  %2326 = phi i64 [ 0, %2248 ], [ %2291, %2259 ]
  %2327 = phi i64 [ %2246, %2248 ], [ %2284, %2259 ]
  %2328 = icmp eq i64 %2252, 0
  br i1 %2328, label %2344, label %2329

2329:                                             ; preds = %2323, %2329
  %2330 = phi i64 [ %2341, %2329 ], [ %2325, %2323 ]
  %2331 = phi i64 [ %2340, %2329 ], [ %2326, %2323 ]
  %2332 = phi i64 [ %2334, %2329 ], [ %2327, %2323 ]
  %2333 = phi i64 [ %2342, %2329 ], [ 0, %2323 ]
  %2334 = add i64 %2332, 1
  %2335 = getelementptr inbounds nuw i8, ptr %2250, i64 %2332
  %2336 = load i8, ptr %2335, align 1
  %2337 = zext i8 %2336 to i64
  %2338 = shl nsw i64 %2330, 3
  %2339 = shl i64 %2337, %2338
  %2340 = or i64 %2339, %2331
  %2341 = add nuw nsw i64 %2330, 1
  %2342 = add i64 %2333, 1
  %2343 = icmp eq i64 %2342, %2252
  br i1 %2343, label %2344, label %2329, !llvm.loop !30

2344:                                             ; preds = %2300, %2323, %2329, %2295, %2245
  %2345 = phi i64 [ 0, %2245 ], [ 0, %2295 ], [ %2324, %2323 ], [ %2340, %2329 ], [ %2320, %2300 ]
  %2346 = add i8 %1974, -42
  %2347 = icmp ult i8 %2346, 16
  %2348 = icmp ult i8 %1986, 5
  br i1 %2347, label %2349, label %2425

2349:                                             ; preds = %2344
  br i1 %2348, label %2350, label %2389

2350:                                             ; preds = %2349
  %2351 = trunc i64 %2193 to i32
  %2352 = trunc i64 %2345 to i32
  %2353 = bitcast i32 %2351 to float
  %2354 = bitcast i32 %2352 to float
  switch i8 %1974, label %2388 [
    i8 42, label %2452
    i8 43, label %2355
    i8 44, label %2357
    i8 45, label %2359
    i8 46, label %2361
    i8 47, label %2363
    i8 48, label %2365
    i8 49, label %2367
    i8 50, label %2371
    i8 51, label %2375
    i8 52, label %2377
    i8 53, label %2379
    i8 54, label %2381
    i8 55, label %2383
    i8 56, label %2385
    i8 57, label %2387
  ]

2355:                                             ; preds = %2350
  %2356 = fcmp oeq float %2353, %2354
  br label %2452

2357:                                             ; preds = %2350
  %2358 = fcmp ogt float %2353, %2354
  br label %2452

2359:                                             ; preds = %2350
  %2360 = fcmp oge float %2353, %2354
  br label %2452

2361:                                             ; preds = %2350
  %2362 = fcmp olt float %2353, %2354
  br label %2452

2363:                                             ; preds = %2350
  %2364 = fcmp ole float %2353, %2354
  br label %2452

2365:                                             ; preds = %2350
  %2366 = fcmp une float %2353, %2354
  br label %2452

2367:                                             ; preds = %2350
  %2368 = fcmp ord float %2353, 0.000000e+00
  %2369 = fcmp ord float %2354, 0.000000e+00
  %2370 = select i1 %2368, i1 %2369, i1 false
  br label %2452

2371:                                             ; preds = %2350
  %2372 = fcmp uno float %2353, 0.000000e+00
  %2373 = fcmp uno float %2354, 0.000000e+00
  %2374 = select i1 %2372, i1 true, i1 %2373
  br label %2452

2375:                                             ; preds = %2350
  %2376 = fcmp ueq float %2353, %2354
  br label %2452

2377:                                             ; preds = %2350
  %2378 = fcmp ugt float %2353, %2354
  br label %2452

2379:                                             ; preds = %2350
  %2380 = fcmp uge float %2353, %2354
  br label %2452

2381:                                             ; preds = %2350
  %2382 = fcmp ult float %2353, %2354
  br label %2452

2383:                                             ; preds = %2350
  %2384 = fcmp ule float %2353, %2354
  br label %2452

2385:                                             ; preds = %2350
  %2386 = fcmp une float %2353, %2354
  br label %2452

2387:                                             ; preds = %2350
  br label %2452

2388:                                             ; preds = %2389, %2350
  unreachable

2389:                                             ; preds = %2349
  %2390 = bitcast i64 %2193 to double
  %2391 = bitcast i64 %2345 to double
  switch i8 %1974, label %2388 [
    i8 42, label %2452
    i8 43, label %2392
    i8 44, label %2394
    i8 45, label %2396
    i8 46, label %2398
    i8 47, label %2400
    i8 48, label %2402
    i8 49, label %2404
    i8 50, label %2408
    i8 51, label %2412
    i8 52, label %2414
    i8 53, label %2416
    i8 54, label %2418
    i8 55, label %2420
    i8 56, label %2422
    i8 57, label %2424
  ]

2392:                                             ; preds = %2389
  %2393 = fcmp oeq double %2390, %2391
  br label %2452

2394:                                             ; preds = %2389
  %2395 = fcmp ogt double %2390, %2391
  br label %2452

2396:                                             ; preds = %2389
  %2397 = fcmp oge double %2390, %2391
  br label %2452

2398:                                             ; preds = %2389
  %2399 = fcmp olt double %2390, %2391
  br label %2452

2400:                                             ; preds = %2389
  %2401 = fcmp ole double %2390, %2391
  br label %2452

2402:                                             ; preds = %2389
  %2403 = fcmp une double %2390, %2391
  br label %2452

2404:                                             ; preds = %2389
  %2405 = fcmp ord double %2390, 0.000000e+00
  %2406 = fcmp ord double %2391, 0.000000e+00
  %2407 = select i1 %2405, i1 %2406, i1 false
  br label %2452

2408:                                             ; preds = %2389
  %2409 = fcmp uno double %2390, 0.000000e+00
  %2410 = fcmp uno double %2391, 0.000000e+00
  %2411 = select i1 %2409, i1 true, i1 %2410
  br label %2452

2412:                                             ; preds = %2389
  %2413 = fcmp ueq double %2390, %2391
  br label %2452

2414:                                             ; preds = %2389
  %2415 = fcmp ugt double %2390, %2391
  br label %2452

2416:                                             ; preds = %2389
  %2417 = fcmp uge double %2390, %2391
  br label %2452

2418:                                             ; preds = %2389
  %2419 = fcmp ult double %2390, %2391
  br label %2452

2420:                                             ; preds = %2389
  %2421 = fcmp ule double %2390, %2391
  br label %2452

2422:                                             ; preds = %2389
  %2423 = fcmp une double %2390, %2391
  br label %2452

2424:                                             ; preds = %2389
  br label %2452

2425:                                             ; preds = %2344
  %2426 = shl i64 %2193, 32
  %2427 = ashr exact i64 %2426, 32
  %2428 = shl i64 %2345, 32
  %2429 = ashr exact i64 %2428, 32
  %2430 = select i1 %2348, i64 %2427, i64 %2193
  %2431 = select i1 %2348, i64 %2429, i64 %2345
  switch i8 %1974, label %2452 [
    i8 32, label %2432
    i8 33, label %2434
    i8 34, label %2436
    i8 35, label %2438
    i8 36, label %2440
    i8 37, label %2442
    i8 38, label %2444
    i8 39, label %2446
    i8 40, label %2448
    i8 41, label %2450
  ]

2432:                                             ; preds = %2425
  %2433 = icmp eq i64 %2193, %2345
  br label %2452

2434:                                             ; preds = %2425
  %2435 = icmp ne i64 %2193, %2345
  br label %2452

2436:                                             ; preds = %2425
  %2437 = icmp ugt i64 %2193, %2345
  br label %2452

2438:                                             ; preds = %2425
  %2439 = icmp uge i64 %2193, %2345
  br label %2452

2440:                                             ; preds = %2425
  %2441 = icmp ult i64 %2193, %2345
  br label %2452

2442:                                             ; preds = %2425
  %2443 = icmp ule i64 %2193, %2345
  br label %2452

2444:                                             ; preds = %2425
  %2445 = icmp sgt i64 %2430, %2431
  br label %2452

2446:                                             ; preds = %2425
  %2447 = icmp sge i64 %2430, %2431
  br label %2452

2448:                                             ; preds = %2425
  %2449 = icmp slt i64 %2430, %2431
  br label %2452

2450:                                             ; preds = %2425
  %2451 = icmp sle i64 %2430, %2431
  br label %2452

2452:                                             ; preds = %2425, %2432, %2434, %2436, %2438, %2440, %2442, %2444, %2446, %2448, %2450, %2424, %2422, %2420, %2418, %2416, %2414, %2412, %2408, %2404, %2402, %2400, %2398, %2396, %2394, %2392, %2389, %2387, %2385, %2383, %2381, %2379, %2377, %2375, %2371, %2367, %2365, %2363, %2361, %2359, %2357, %2355, %2350
  %2453 = phi i1 [ %2356, %2355 ], [ %2358, %2357 ], [ %2360, %2359 ], [ %2362, %2361 ], [ %2364, %2363 ], [ %2366, %2365 ], [ %2370, %2367 ], [ %2374, %2371 ], [ %2376, %2375 ], [ %2378, %2377 ], [ %2380, %2379 ], [ %2382, %2381 ], [ %2384, %2383 ], [ %2386, %2385 ], [ true, %2387 ], [ false, %2350 ], [ %2393, %2392 ], [ %2395, %2394 ], [ %2397, %2396 ], [ %2399, %2398 ], [ %2401, %2400 ], [ %2403, %2402 ], [ %2407, %2404 ], [ %2411, %2408 ], [ %2413, %2412 ], [ %2415, %2414 ], [ %2417, %2416 ], [ %2419, %2418 ], [ %2421, %2420 ], [ %2423, %2422 ], [ true, %2424 ], [ false, %2389 ], [ false, %2425 ], [ %2433, %2432 ], [ %2435, %2434 ], [ %2437, %2436 ], [ %2439, %2438 ], [ %2441, %2440 ], [ %2443, %2442 ], [ %2445, %2444 ], [ %2447, %2446 ], [ %2449, %2448 ], [ %2451, %2450 ]
  %2454 = icmp eq i8 %1990, %1997
  br i1 %2454, label %8784, label %2455

2455:                                             ; preds = %2452
  %2456 = zext i1 %2453 to i8
  %2457 = load i64, ptr @data_seg_addr, align 8
  %2458 = add i64 %2457, %2037
  %2459 = inttoptr i64 %2458 to ptr
  %2460 = zext i8 %1998 to i32
  store i8 %2456, ptr %2459, align 1
  %2461 = icmp eq i8 %1998, 1
  br i1 %2461, label %8784, label %2462

2462:                                             ; preds = %2455
  %2463 = add nsw i32 %2460, -1
  %2464 = zext i32 %2463 to i64
  %2465 = icmp ult i8 %1998, 9
  br i1 %2465, label %2518, label %2466

2466:                                             ; preds = %2462
  %2467 = icmp ult i8 %1998, 33
  br i1 %2467, label %2504, label %2468

2468:                                             ; preds = %2466
  %2469 = and i64 %2464, 4294967264
  %2470 = getelementptr inbounds nuw i8, ptr %2459, i64 1
  %2471 = getelementptr inbounds nuw i8, ptr %2459, i64 17
  store <16 x i8> zeroinitializer, ptr %2470, align 1
  store <16 x i8> zeroinitializer, ptr %2471, align 1
  %2472 = icmp eq i64 %2469, 32
  br i1 %2472, label %2496, label %2473, !llvm.loop !31

2473:                                             ; preds = %2468
  %2474 = getelementptr i8, ptr %2459, i64 33
  %2475 = getelementptr i8, ptr %2459, i64 49
  store <16 x i8> zeroinitializer, ptr %2474, align 1
  store <16 x i8> zeroinitializer, ptr %2475, align 1
  %2476 = icmp eq i64 %2469, 64
  br i1 %2476, label %2496, label %2477, !llvm.loop !31

2477:                                             ; preds = %2473
  %2478 = getelementptr i8, ptr %2459, i64 65
  %2479 = getelementptr i8, ptr %2459, i64 81
  store <16 x i8> zeroinitializer, ptr %2478, align 1
  store <16 x i8> zeroinitializer, ptr %2479, align 1
  %2480 = icmp eq i64 %2469, 96
  br i1 %2480, label %2496, label %2481, !llvm.loop !31

2481:                                             ; preds = %2477
  %2482 = getelementptr i8, ptr %2459, i64 97
  %2483 = getelementptr i8, ptr %2459, i64 113
  store <16 x i8> zeroinitializer, ptr %2482, align 1
  store <16 x i8> zeroinitializer, ptr %2483, align 1
  %2484 = icmp eq i64 %2469, 128
  br i1 %2484, label %2496, label %2485, !llvm.loop !31

2485:                                             ; preds = %2481
  %2486 = getelementptr i8, ptr %2459, i64 129
  %2487 = getelementptr i8, ptr %2459, i64 145
  store <16 x i8> zeroinitializer, ptr %2486, align 1
  store <16 x i8> zeroinitializer, ptr %2487, align 1
  %2488 = icmp eq i64 %2469, 160
  br i1 %2488, label %2496, label %2489, !llvm.loop !31

2489:                                             ; preds = %2485
  %2490 = getelementptr i8, ptr %2459, i64 161
  %2491 = getelementptr i8, ptr %2459, i64 177
  store <16 x i8> zeroinitializer, ptr %2490, align 1
  store <16 x i8> zeroinitializer, ptr %2491, align 1
  %2492 = icmp eq i64 %2469, 192
  br i1 %2492, label %2496, label %2493, !llvm.loop !31

2493:                                             ; preds = %2489
  %2494 = getelementptr i8, ptr %2459, i64 193
  %2495 = getelementptr i8, ptr %2459, i64 209
  store <16 x i8> zeroinitializer, ptr %2494, align 1
  store <16 x i8> zeroinitializer, ptr %2495, align 1
  br label %2496

2496:                                             ; preds = %2493, %2489, %2485, %2481, %2477, %2473, %2468
  %2497 = icmp eq i64 %2469, %2464
  br i1 %2497, label %8784, label %2498

2498:                                             ; preds = %2496
  %2499 = trunc nuw i64 %2469 to i32
  %2500 = or disjoint i32 %2499, 1
  %2501 = getelementptr i8, ptr %2459, i64 %2469
  %2502 = and i64 %2464, 24
  %2503 = icmp eq i64 %2502, 0
  br i1 %2503, label %2518, label %2504

2504:                                             ; preds = %2498, %2466
  %2505 = phi i64 [ %2469, %2498 ], [ 0, %2466 ]
  %2506 = and i64 %2464, 4294967288
  %2507 = trunc nuw i64 %2506 to i32
  %2508 = or disjoint i32 %2507, 1
  %2509 = getelementptr i8, ptr %2459, i64 %2506
  %2510 = getelementptr i8, ptr %2459, i64 1
  br label %2511

2511:                                             ; preds = %2511, %2504
  %2512 = phi i64 [ %2505, %2504 ], [ %2514, %2511 ]
  %2513 = getelementptr i8, ptr %2510, i64 %2512
  store <8 x i8> zeroinitializer, ptr %2513, align 1
  %2514 = add nuw i64 %2512, 8
  %2515 = icmp eq i64 %2514, %2506
  br i1 %2515, label %2516, label %2511, !llvm.loop !33

2516:                                             ; preds = %2511
  %2517 = icmp eq i64 %2506, %2464
  br i1 %2517, label %8784, label %2518

2518:                                             ; preds = %2498, %2516, %2462
  %2519 = phi i32 [ 1, %2462 ], [ %2500, %2498 ], [ %2508, %2516 ]
  %2520 = phi ptr [ %2459, %2462 ], [ %2501, %2498 ], [ %2509, %2516 ]
  br label %2521

2521:                                             ; preds = %2518, %2521
  %2522 = phi i32 [ %2525, %2521 ], [ %2519, %2518 ]
  %2523 = phi ptr [ %2524, %2521 ], [ %2520, %2518 ]
  %2524 = getelementptr inbounds nuw i8, ptr %2523, i64 1
  store i8 0, ptr %2524, align 1
  %2525 = add nuw nsw i32 %2522, 1
  %2526 = icmp eq i32 %2525, %2460
  br i1 %2526, label %8784, label %2521, !llvm.loop !34

2527:                                             ; preds = %158
  %2528 = add nsw i32 %72, 2
  store i32 %2528, ptr @ip, align 4
  %2529 = sext i32 %73 to i64
  %2530 = getelementptr inbounds i8, ptr %69, i64 %2529
  %2531 = load i8, ptr %2530, align 1
  %2532 = shl i32 %81, 13
  %2533 = xor i32 %2532, %81
  %2534 = lshr i32 %2533, 17
  %2535 = xor i32 %2534, %2533
  %2536 = shl i32 %2535, 5
  %2537 = xor i32 %2536, %2535
  store i32 %2537, ptr @vm_code_state, align 4
  %2538 = trunc i32 %2537 to i8
  %2539 = xor i8 %2531, %2538
  %2540 = add nsw i32 %72, 3
  store i32 %2540, ptr @ip, align 4
  %2541 = sext i32 %2528 to i64
  %2542 = getelementptr inbounds i8, ptr %69, i64 %2541
  %2543 = load i8, ptr %2542, align 1
  %2544 = shl i32 %2537, 13
  %2545 = xor i32 %2544, %2537
  %2546 = lshr i32 %2545, 17
  %2547 = xor i32 %2546, %2545
  %2548 = shl i32 %2547, 5
  %2549 = xor i32 %2548, %2547
  store i32 %2549, ptr @vm_code_state, align 4
  %2550 = trunc i32 %2549 to i8
  %2551 = xor i8 %2543, %2550
  %2552 = add nsw i32 %72, 4
  store i32 %2552, ptr @ip, align 4
  %2553 = sext i32 %2540 to i64
  %2554 = getelementptr inbounds i8, ptr %69, i64 %2553
  %2555 = load i8, ptr %2554, align 1
  %2556 = shl i32 %2549, 13
  %2557 = xor i32 %2556, %2549
  %2558 = lshr i32 %2557, 17
  %2559 = xor i32 %2558, %2557
  %2560 = shl i32 %2559, 5
  %2561 = xor i32 %2560, %2559
  %2562 = trunc i32 %2561 to i8
  %2563 = xor i8 %2555, %2562
  %2564 = add nsw i32 %72, 5
  %2565 = shl i32 %2561, 13
  %2566 = xor i32 %2565, %2561
  %2567 = lshr i32 %2566, 17
  %2568 = xor i32 %2567, %2566
  %2569 = shl i32 %2568, 5
  %2570 = xor i32 %2569, %2568
  store i32 %2570, ptr @vm_code_state, align 4
  %2571 = load i32, ptr @pointer_size, align 4
  %2572 = icmp sgt i32 %2571, 0
  br i1 %2572, label %2573, label %2599

2573:                                             ; preds = %2527
  %2574 = sext i32 %2564 to i64
  %2575 = zext nneg i32 %2571 to i64
  br label %2576

2576:                                             ; preds = %2573, %2576
  %2577 = phi i64 [ %2574, %2573 ], [ %2581, %2576 ]
  %2578 = phi i64 [ 0, %2573 ], [ %2597, %2576 ]
  %2579 = phi i64 [ 0, %2573 ], [ %2596, %2576 ]
  %2580 = phi i32 [ %2570, %2573 ], [ %2590, %2576 ]
  %2581 = add nsw i64 %2577, 1
  %2582 = trunc nsw i64 %2581 to i32
  store i32 %2582, ptr @ip, align 4
  %2583 = getelementptr inbounds i8, ptr %69, i64 %2577
  %2584 = load i8, ptr %2583, align 1
  %2585 = shl i32 %2580, 13
  %2586 = xor i32 %2585, %2580
  %2587 = lshr i32 %2586, 17
  %2588 = xor i32 %2587, %2586
  %2589 = shl i32 %2588, 5
  %2590 = xor i32 %2589, %2588
  store i32 %2590, ptr @vm_code_state, align 4
  %2591 = trunc i32 %2590 to i8
  %2592 = xor i8 %2584, %2591
  %2593 = zext i8 %2592 to i64
  %2594 = shl nsw i64 %2578, 3
  %2595 = shl i64 %2593, %2594
  %2596 = or i64 %2595, %2579
  %2597 = add nuw nsw i64 %2578, 1
  %2598 = icmp eq i64 %2597, %2575
  br i1 %2598, label %2599, label %2576, !llvm.loop !15

2599:                                             ; preds = %2576, %2527
  %2600 = phi i32 [ %2570, %2527 ], [ %2590, %2576 ]
  %2601 = phi i32 [ %2564, %2527 ], [ %2582, %2576 ]
  %2602 = phi i64 [ 0, %2527 ], [ %2596, %2576 ]
  %2603 = add nsw i32 %2601, 1
  store i32 %2603, ptr @ip, align 4
  %2604 = sext i32 %2601 to i64
  %2605 = getelementptr inbounds i8, ptr %69, i64 %2604
  %2606 = load i8, ptr %2605, align 1
  %2607 = shl i32 %2600, 13
  %2608 = xor i32 %2607, %2600
  %2609 = lshr i32 %2608, 17
  %2610 = xor i32 %2609, %2608
  %2611 = shl i32 %2610, 5
  %2612 = xor i32 %2611, %2610
  store i32 %2612, ptr @vm_code_state, align 4
  %2613 = trunc i32 %2612 to i8
  %2614 = xor i8 %2606, %2613
  %2615 = add nsw i32 %2601, 2
  store i32 %2615, ptr @ip, align 4
  %2616 = sext i32 %2603 to i64
  %2617 = getelementptr inbounds i8, ptr %69, i64 %2616
  %2618 = load i8, ptr %2617, align 1
  %2619 = shl i32 %2612, 13
  %2620 = xor i32 %2619, %2612
  %2621 = lshr i32 %2620, 17
  %2622 = xor i32 %2621, %2620
  %2623 = shl i32 %2622, 5
  %2624 = xor i32 %2623, %2622
  store i32 %2624, ptr @vm_code_state, align 4
  %2625 = trunc i32 %2624 to i8
  %2626 = icmp eq i8 %2618, %2625
  br i1 %2626, label %2627, label %2704

2627:                                             ; preds = %2599
  br i1 %2572, label %2628, label %2654

2628:                                             ; preds = %2627
  %2629 = sext i32 %2615 to i64
  %2630 = zext nneg i32 %2571 to i64
  br label %2631

2631:                                             ; preds = %2628, %2631
  %2632 = phi i64 [ %2629, %2628 ], [ %2636, %2631 ]
  %2633 = phi i64 [ 0, %2628 ], [ %2652, %2631 ]
  %2634 = phi i64 [ 0, %2628 ], [ %2651, %2631 ]
  %2635 = phi i32 [ %2624, %2628 ], [ %2645, %2631 ]
  %2636 = add nsw i64 %2632, 1
  %2637 = trunc nsw i64 %2636 to i32
  store i32 %2637, ptr @ip, align 4
  %2638 = getelementptr inbounds i8, ptr %69, i64 %2632
  %2639 = load i8, ptr %2638, align 1
  %2640 = shl i32 %2635, 13
  %2641 = xor i32 %2640, %2635
  %2642 = lshr i32 %2641, 17
  %2643 = xor i32 %2642, %2641
  %2644 = shl i32 %2643, 5
  %2645 = xor i32 %2644, %2643
  store i32 %2645, ptr @vm_code_state, align 4
  %2646 = trunc i32 %2645 to i8
  %2647 = xor i8 %2639, %2646
  %2648 = zext i8 %2647 to i64
  %2649 = shl nsw i64 %2633, 3
  %2650 = shl i64 %2648, %2649
  %2651 = or i64 %2650, %2634
  %2652 = add nuw nsw i64 %2633, 1
  %2653 = icmp eq i64 %2652, %2630
  br i1 %2653, label %2654, label %2631, !llvm.loop !15

2654:                                             ; preds = %2631, %2627
  %2655 = phi i64 [ 0, %2627 ], [ %2651, %2631 ]
  %2656 = icmp eq i8 %2606, %2613
  br i1 %2656, label %2753, label %2657

2657:                                             ; preds = %2654
  %2658 = load i64, ptr @data_seg_addr, align 8
  %2659 = inttoptr i64 %2658 to ptr
  %2660 = zext i8 %2614 to i64
  %2661 = and i64 %2660, 3
  %2662 = icmp ult i8 %2614, 4
  br i1 %2662, label %2732, label %2663

2663:                                             ; preds = %2657
  %2664 = and i64 %2660, 252
  %2665 = getelementptr i8, ptr %2659, i64 1
  %2666 = getelementptr i8, ptr %2659, i64 2
  %2667 = getelementptr i8, ptr %2659, i64 3
  br label %2668

2668:                                             ; preds = %2668, %2663
  %2669 = phi i64 [ 0, %2663 ], [ %2701, %2668 ]
  %2670 = phi i64 [ 0, %2663 ], [ %2700, %2668 ]
  %2671 = phi i64 [ %2655, %2663 ], [ %2693, %2668 ]
  %2672 = phi i64 [ 0, %2663 ], [ %2702, %2668 ]
  %2673 = getelementptr inbounds nuw i8, ptr %2659, i64 %2671
  %2674 = load i8, ptr %2673, align 1
  %2675 = zext i8 %2674 to i64
  %2676 = shl nsw i64 %2669, 3
  %2677 = shl i64 %2675, %2676
  %2678 = or i64 %2677, %2670
  %2679 = getelementptr i8, ptr %2665, i64 %2671
  %2680 = load i8, ptr %2679, align 1
  %2681 = zext i8 %2680 to i64
  %2682 = shl i64 %2669, 3
  %2683 = or disjoint i64 %2682, 8
  %2684 = shl i64 %2681, %2683
  %2685 = or i64 %2684, %2678
  %2686 = getelementptr i8, ptr %2666, i64 %2671
  %2687 = load i8, ptr %2686, align 1
  %2688 = zext i8 %2687 to i64
  %2689 = shl i64 %2669, 3
  %2690 = or disjoint i64 %2689, 16
  %2691 = shl i64 %2688, %2690
  %2692 = or i64 %2691, %2685
  %2693 = add i64 %2671, 4
  %2694 = getelementptr i8, ptr %2667, i64 %2671
  %2695 = load i8, ptr %2694, align 1
  %2696 = zext i8 %2695 to i64
  %2697 = shl i64 %2669, 3
  %2698 = or disjoint i64 %2697, 24
  %2699 = shl i64 %2696, %2698
  %2700 = or i64 %2699, %2692
  %2701 = add nuw nsw i64 %2669, 4
  %2702 = add i64 %2672, 4
  %2703 = icmp eq i64 %2702, %2664
  br i1 %2703, label %2732, label %2668, !llvm.loop !17

2704:                                             ; preds = %2599
  %2705 = icmp eq i8 %2606, %2613
  br i1 %2705, label %2753, label %2706

2706:                                             ; preds = %2704
  %2707 = sext i32 %2615 to i64
  %2708 = zext i8 %2614 to i64
  br label %2709

2709:                                             ; preds = %2706, %2709
  %2710 = phi i64 [ %2707, %2706 ], [ %2714, %2709 ]
  %2711 = phi i64 [ 0, %2706 ], [ %2730, %2709 ]
  %2712 = phi i64 [ 0, %2706 ], [ %2729, %2709 ]
  %2713 = phi i32 [ %2624, %2706 ], [ %2723, %2709 ]
  %2714 = add nsw i64 %2710, 1
  %2715 = trunc nsw i64 %2714 to i32
  store i32 %2715, ptr @ip, align 4
  %2716 = getelementptr inbounds i8, ptr %69, i64 %2710
  %2717 = load i8, ptr %2716, align 1
  %2718 = shl i32 %2713, 13
  %2719 = xor i32 %2718, %2713
  %2720 = lshr i32 %2719, 17
  %2721 = xor i32 %2720, %2719
  %2722 = shl i32 %2721, 5
  %2723 = xor i32 %2722, %2721
  store i32 %2723, ptr @vm_code_state, align 4
  %2724 = trunc i32 %2723 to i8
  %2725 = xor i8 %2717, %2724
  %2726 = zext i8 %2725 to i64
  %2727 = shl nsw i64 %2711, 3
  %2728 = shl i64 %2726, %2727
  %2729 = or i64 %2728, %2712
  %2730 = add nuw nsw i64 %2711, 1
  %2731 = icmp eq i64 %2730, %2708
  br i1 %2731, label %2753, label %2709, !llvm.loop !15

2732:                                             ; preds = %2668, %2657
  %2733 = phi i64 [ poison, %2657 ], [ %2700, %2668 ]
  %2734 = phi i64 [ 0, %2657 ], [ %2701, %2668 ]
  %2735 = phi i64 [ 0, %2657 ], [ %2700, %2668 ]
  %2736 = phi i64 [ %2655, %2657 ], [ %2693, %2668 ]
  %2737 = icmp eq i64 %2661, 0
  br i1 %2737, label %2753, label %2738

2738:                                             ; preds = %2732, %2738
  %2739 = phi i64 [ %2750, %2738 ], [ %2734, %2732 ]
  %2740 = phi i64 [ %2749, %2738 ], [ %2735, %2732 ]
  %2741 = phi i64 [ %2743, %2738 ], [ %2736, %2732 ]
  %2742 = phi i64 [ %2751, %2738 ], [ 0, %2732 ]
  %2743 = add i64 %2741, 1
  %2744 = getelementptr inbounds nuw i8, ptr %2659, i64 %2741
  %2745 = load i8, ptr %2744, align 1
  %2746 = zext i8 %2745 to i64
  %2747 = shl nsw i64 %2739, 3
  %2748 = shl i64 %2746, %2747
  %2749 = or i64 %2748, %2740
  %2750 = add nuw nsw i64 %2739, 1
  %2751 = add i64 %2742, 1
  %2752 = icmp eq i64 %2751, %2661
  br i1 %2752, label %2753, label %2738, !llvm.loop !35

2753:                                             ; preds = %2709, %2732, %2738, %2704, %2654
  %2754 = phi i64 [ 0, %2654 ], [ 0, %2704 ], [ %2733, %2732 ], [ %2749, %2738 ], [ %2729, %2709 ]
  switch i8 %2539, label %2815 [
    i8 8, label %2806
    i8 7, label %2797
    i8 2, label %2755
    i8 3, label %2765
    i8 4, label %2772
    i8 5, label %2779
    i8 6, label %2788
  ]

2755:                                             ; preds = %2753
  switch i8 %2551, label %2815 [
    i8 1, label %2756
    i8 2, label %2759
    i8 4, label %2762
  ]

2756:                                             ; preds = %2755
  %2757 = shl i64 %2754, 56
  %2758 = ashr exact i64 %2757, 56
  br label %2815

2759:                                             ; preds = %2755
  %2760 = shl i64 %2754, 48
  %2761 = ashr exact i64 %2760, 48
  br label %2815

2762:                                             ; preds = %2755
  %2763 = shl i64 %2754, 32
  %2764 = ashr exact i64 %2763, 32
  br label %2815

2765:                                             ; preds = %2753
  %2766 = icmp eq i8 %2551, 8
  br i1 %2766, label %2767, label %2815

2767:                                             ; preds = %2765
  %2768 = bitcast i64 %2754 to double
  %2769 = fptrunc double %2768 to float
  %2770 = bitcast float %2769 to i32
  %2771 = zext i32 %2770 to i64
  br label %2815

2772:                                             ; preds = %2753
  %2773 = icmp eq i8 %2551, 4
  br i1 %2773, label %2774, label %2815

2774:                                             ; preds = %2772
  %2775 = trunc i64 %2754 to i32
  %2776 = bitcast i32 %2775 to float
  %2777 = fpext float %2776 to double
  %2778 = bitcast double %2777 to i64
  br label %2815

2779:                                             ; preds = %2753
  %2780 = icmp eq i8 %2551, 4
  br i1 %2780, label %2781, label %2785

2781:                                             ; preds = %2779
  %2782 = trunc i64 %2754 to i32
  %2783 = bitcast i32 %2782 to float
  %2784 = fptoui float %2783 to i64
  br label %2815

2785:                                             ; preds = %2779
  %2786 = bitcast i64 %2754 to double
  %2787 = fptoui double %2786 to i64
  br label %2815

2788:                                             ; preds = %2753
  %2789 = icmp eq i8 %2551, 4
  br i1 %2789, label %2790, label %2794

2790:                                             ; preds = %2788
  %2791 = trunc i64 %2754 to i32
  %2792 = bitcast i32 %2791 to float
  %2793 = fptosi float %2792 to i64
  br label %2815

2794:                                             ; preds = %2788
  %2795 = bitcast i64 %2754 to double
  %2796 = fptosi double %2795 to i64
  br label %2815

2797:                                             ; preds = %2753
  %2798 = icmp ult i8 %2563, 5
  br i1 %2798, label %2799, label %2803

2799:                                             ; preds = %2797
  %2800 = uitofp i64 %2754 to float
  %2801 = bitcast float %2800 to i32
  %2802 = zext nneg i32 %2801 to i64
  br label %2815

2803:                                             ; preds = %2797
  %2804 = uitofp i64 %2754 to double
  %2805 = bitcast double %2804 to i64
  br label %2815

2806:                                             ; preds = %2753
  %2807 = icmp ult i8 %2563, 5
  br i1 %2807, label %2808, label %2812

2808:                                             ; preds = %2806
  %2809 = sitofp i64 %2754 to float
  %2810 = bitcast float %2809 to i32
  %2811 = zext i32 %2810 to i64
  br label %2815

2812:                                             ; preds = %2806
  %2813 = sitofp i64 %2754 to double
  %2814 = bitcast double %2813 to i64
  br label %2815

2815:                                             ; preds = %2812, %2808, %2803, %2799, %2794, %2790, %2785, %2781, %2774, %2772, %2767, %2765, %2762, %2759, %2756, %2755, %2753
  %2816 = phi i64 [ %2758, %2756 ], [ %2761, %2759 ], [ %2764, %2762 ], [ %2771, %2767 ], [ %2778, %2774 ], [ %2784, %2781 ], [ %2787, %2785 ], [ %2793, %2790 ], [ %2796, %2794 ], [ %2802, %2799 ], [ %2805, %2803 ], [ %2811, %2808 ], [ %2814, %2812 ], [ %2754, %2755 ], [ %2754, %2765 ], [ %2754, %2772 ], [ %2754, %2753 ]
  %2817 = icmp eq i8 %2555, %2562
  br i1 %2817, label %8784, label %2818

2818:                                             ; preds = %2815
  %2819 = load i64, ptr @data_seg_addr, align 8
  %2820 = add i64 %2819, %2602
  %2821 = inttoptr i64 %2820 to ptr
  %2822 = zext i8 %2563 to i32
  %2823 = and i32 %2822, 7
  %2824 = icmp ult i8 %2563, 8
  br i1 %2824, label %8680, label %2825

2825:                                             ; preds = %2818
  %2826 = and i32 %2822, 248
  br label %2827

2827:                                             ; preds = %2827, %2825
  %2828 = phi ptr [ %2821, %2825 ], [ %2853, %2827 ]
  %2829 = phi i64 [ %2816, %2825 ], [ 0, %2827 ]
  %2830 = phi i32 [ 0, %2825 ], [ %2854, %2827 ]
  %2831 = trunc i64 %2829 to i8
  store i8 %2831, ptr %2828, align 1
  %2832 = getelementptr inbounds nuw i8, ptr %2828, i64 1
  %2833 = lshr i64 %2829, 8
  %2834 = trunc i64 %2833 to i8
  store i8 %2834, ptr %2832, align 1
  %2835 = getelementptr inbounds nuw i8, ptr %2828, i64 2
  %2836 = lshr i64 %2829, 16
  %2837 = trunc i64 %2836 to i8
  store i8 %2837, ptr %2835, align 1
  %2838 = getelementptr inbounds nuw i8, ptr %2828, i64 3
  %2839 = lshr i64 %2829, 24
  %2840 = trunc i64 %2839 to i8
  store i8 %2840, ptr %2838, align 1
  %2841 = getelementptr inbounds nuw i8, ptr %2828, i64 4
  %2842 = lshr i64 %2829, 32
  %2843 = trunc i64 %2842 to i8
  store i8 %2843, ptr %2841, align 1
  %2844 = getelementptr inbounds nuw i8, ptr %2828, i64 5
  %2845 = lshr i64 %2829, 40
  %2846 = trunc i64 %2845 to i8
  store i8 %2846, ptr %2844, align 1
  %2847 = getelementptr inbounds nuw i8, ptr %2828, i64 6
  %2848 = lshr i64 %2829, 48
  %2849 = trunc i64 %2848 to i8
  store i8 %2849, ptr %2847, align 1
  %2850 = getelementptr inbounds nuw i8, ptr %2828, i64 7
  %2851 = lshr i64 %2829, 56
  %2852 = trunc nuw i64 %2851 to i8
  store i8 %2852, ptr %2850, align 1
  %2853 = getelementptr inbounds nuw i8, ptr %2828, i64 8
  %2854 = add i32 %2830, 8
  %2855 = icmp eq i32 %2854, %2826
  br i1 %2855, label %8680, label %2827, !llvm.loop !16

2856:                                             ; preds = %158
  %2857 = add nsw i32 %72, 2
  store i32 %2857, ptr @ip, align 4
  %2858 = sext i32 %73 to i64
  %2859 = getelementptr inbounds i8, ptr %69, i64 %2858
  %2860 = load i8, ptr %2859, align 1
  %2861 = shl i32 %81, 13
  %2862 = xor i32 %2861, %81
  %2863 = lshr i32 %2862, 17
  %2864 = xor i32 %2863, %2862
  %2865 = shl i32 %2864, 5
  %2866 = xor i32 %2865, %2864
  store i32 %2866, ptr @vm_code_state, align 4
  %2867 = trunc i32 %2866 to i8
  %2868 = load i32, ptr @pointer_size, align 4
  %2869 = icmp sgt i32 %2868, 0
  br i1 %2869, label %2870, label %2898

2870:                                             ; preds = %2856
  %2871 = sext i32 %2857 to i64
  %2872 = zext nneg i32 %2868 to i64
  br label %2873

2873:                                             ; preds = %2870, %2873
  %2874 = phi i64 [ %2871, %2870 ], [ %2878, %2873 ]
  %2875 = phi i64 [ 0, %2870 ], [ %2894, %2873 ]
  %2876 = phi i64 [ 0, %2870 ], [ %2893, %2873 ]
  %2877 = phi i32 [ %2866, %2870 ], [ %2887, %2873 ]
  %2878 = add nsw i64 %2874, 1
  %2879 = trunc nsw i64 %2878 to i32
  store i32 %2879, ptr @ip, align 4
  %2880 = getelementptr inbounds i8, ptr %69, i64 %2874
  %2881 = load i8, ptr %2880, align 1
  %2882 = shl i32 %2877, 13
  %2883 = xor i32 %2882, %2877
  %2884 = lshr i32 %2883, 17
  %2885 = xor i32 %2884, %2883
  %2886 = shl i32 %2885, 5
  %2887 = xor i32 %2886, %2885
  store i32 %2887, ptr @vm_code_state, align 4
  %2888 = trunc i32 %2887 to i8
  %2889 = xor i8 %2881, %2888
  %2890 = zext i8 %2889 to i64
  %2891 = shl nsw i64 %2875, 3
  %2892 = shl i64 %2890, %2891
  %2893 = or i64 %2892, %2876
  %2894 = add nuw nsw i64 %2875, 1
  %2895 = icmp eq i64 %2894, %2872
  br i1 %2895, label %2896, label %2873, !llvm.loop !15

2896:                                             ; preds = %2873
  store i64 %2893, ptr @last_br_from_bb_id, align 8
  %2897 = icmp eq i8 %2860, %2867
  br i1 %2897, label %2900, label %2927

2898:                                             ; preds = %2856
  store i64 0, ptr @last_br_from_bb_id, align 8
  %2899 = icmp eq i8 %2860, %2867
  br i1 %2899, label %3144, label %2927

2900:                                             ; preds = %2896
  %2901 = shl i64 %2878, 32
  %2902 = ashr exact i64 %2901, 32
  %2903 = zext nneg i32 %2868 to i64
  br label %2904

2904:                                             ; preds = %2900, %2904
  %2905 = phi i64 [ %2902, %2900 ], [ %2909, %2904 ]
  %2906 = phi i64 [ 0, %2900 ], [ %2925, %2904 ]
  %2907 = phi i64 [ 0, %2900 ], [ %2924, %2904 ]
  %2908 = phi i32 [ %2887, %2900 ], [ %2918, %2904 ]
  %2909 = add nsw i64 %2905, 1
  %2910 = trunc nsw i64 %2909 to i32
  store i32 %2910, ptr @ip, align 4
  %2911 = getelementptr inbounds i8, ptr %69, i64 %2905
  %2912 = load i8, ptr %2911, align 1
  %2913 = shl i32 %2908, 13
  %2914 = xor i32 %2913, %2908
  %2915 = lshr i32 %2914, 17
  %2916 = xor i32 %2915, %2914
  %2917 = shl i32 %2916, 5
  %2918 = xor i32 %2917, %2916
  store i32 %2918, ptr @vm_code_state, align 4
  %2919 = trunc i32 %2918 to i8
  %2920 = xor i8 %2912, %2919
  %2921 = zext i8 %2920 to i64
  %2922 = shl nsw i64 %2906, 3
  %2923 = shl i64 %2921, %2922
  %2924 = or i64 %2923, %2907
  %2925 = add nuw nsw i64 %2906, 1
  %2926 = icmp eq i64 %2925, %2903
  br i1 %2926, label %3144, label %2904, !llvm.loop !15

2927:                                             ; preds = %2898, %2896
  %2928 = phi i32 [ %2857, %2898 ], [ %2879, %2896 ]
  %2929 = phi i32 [ %2866, %2898 ], [ %2887, %2896 ]
  %2930 = add nsw i32 %2928, 1
  store i32 %2930, ptr @ip, align 4
  %2931 = sext i32 %2928 to i64
  %2932 = getelementptr inbounds i8, ptr %69, i64 %2931
  %2933 = load i8, ptr %2932, align 1
  %2934 = shl i32 %2929, 13
  %2935 = xor i32 %2934, %2929
  %2936 = lshr i32 %2935, 17
  %2937 = xor i32 %2936, %2935
  %2938 = shl i32 %2937, 5
  %2939 = xor i32 %2938, %2937
  store i32 %2939, ptr @vm_code_state, align 4
  %2940 = trunc i32 %2939 to i8
  %2941 = xor i8 %2933, %2940
  %2942 = add nsw i32 %2928, 2
  store i32 %2942, ptr @ip, align 4
  %2943 = sext i32 %2930 to i64
  %2944 = getelementptr inbounds i8, ptr %69, i64 %2943
  %2945 = load i8, ptr %2944, align 1
  %2946 = shl i32 %2939, 13
  %2947 = xor i32 %2946, %2939
  %2948 = lshr i32 %2947, 17
  %2949 = xor i32 %2948, %2947
  %2950 = shl i32 %2949, 5
  %2951 = xor i32 %2950, %2949
  store i32 %2951, ptr @vm_code_state, align 4
  %2952 = trunc i32 %2951 to i8
  %2953 = icmp eq i8 %2945, %2952
  br i1 %2953, label %2954, label %3033

2954:                                             ; preds = %2927
  br i1 %2869, label %2955, label %2981

2955:                                             ; preds = %2954
  %2956 = sext i32 %2942 to i64
  %2957 = zext nneg i32 %2868 to i64
  br label %2958

2958:                                             ; preds = %2955, %2958
  %2959 = phi i64 [ %2956, %2955 ], [ %2963, %2958 ]
  %2960 = phi i64 [ 0, %2955 ], [ %2979, %2958 ]
  %2961 = phi i64 [ 0, %2955 ], [ %2978, %2958 ]
  %2962 = phi i32 [ %2951, %2955 ], [ %2972, %2958 ]
  %2963 = add nsw i64 %2959, 1
  %2964 = trunc nsw i64 %2963 to i32
  store i32 %2964, ptr @ip, align 4
  %2965 = getelementptr inbounds i8, ptr %69, i64 %2959
  %2966 = load i8, ptr %2965, align 1
  %2967 = shl i32 %2962, 13
  %2968 = xor i32 %2967, %2962
  %2969 = lshr i32 %2968, 17
  %2970 = xor i32 %2969, %2968
  %2971 = shl i32 %2970, 5
  %2972 = xor i32 %2971, %2970
  store i32 %2972, ptr @vm_code_state, align 4
  %2973 = trunc i32 %2972 to i8
  %2974 = xor i8 %2966, %2973
  %2975 = zext i8 %2974 to i64
  %2976 = shl nsw i64 %2960, 3
  %2977 = shl i64 %2975, %2976
  %2978 = or i64 %2977, %2961
  %2979 = add nuw nsw i64 %2960, 1
  %2980 = icmp eq i64 %2979, %2957
  br i1 %2980, label %2981, label %2958, !llvm.loop !15

2981:                                             ; preds = %2958, %2954
  %2982 = phi i32 [ %2951, %2954 ], [ %2972, %2958 ]
  %2983 = phi i32 [ %2942, %2954 ], [ %2964, %2958 ]
  %2984 = phi i64 [ 0, %2954 ], [ %2978, %2958 ]
  %2985 = icmp eq i8 %2933, %2940
  br i1 %2985, label %3082, label %2986

2986:                                             ; preds = %2981
  %2987 = load i64, ptr @data_seg_addr, align 8
  %2988 = inttoptr i64 %2987 to ptr
  %2989 = zext i8 %2941 to i64
  %2990 = and i64 %2989, 3
  %2991 = icmp ult i8 %2941, 4
  br i1 %2991, label %3061, label %2992

2992:                                             ; preds = %2986
  %2993 = and i64 %2989, 252
  %2994 = getelementptr i8, ptr %2988, i64 1
  %2995 = getelementptr i8, ptr %2988, i64 2
  %2996 = getelementptr i8, ptr %2988, i64 3
  br label %2997

2997:                                             ; preds = %2997, %2992
  %2998 = phi i64 [ 0, %2992 ], [ %3030, %2997 ]
  %2999 = phi i64 [ 0, %2992 ], [ %3029, %2997 ]
  %3000 = phi i64 [ %2984, %2992 ], [ %3022, %2997 ]
  %3001 = phi i64 [ 0, %2992 ], [ %3031, %2997 ]
  %3002 = getelementptr inbounds nuw i8, ptr %2988, i64 %3000
  %3003 = load i8, ptr %3002, align 1
  %3004 = zext i8 %3003 to i64
  %3005 = shl nsw i64 %2998, 3
  %3006 = shl i64 %3004, %3005
  %3007 = or i64 %3006, %2999
  %3008 = getelementptr i8, ptr %2994, i64 %3000
  %3009 = load i8, ptr %3008, align 1
  %3010 = zext i8 %3009 to i64
  %3011 = shl i64 %2998, 3
  %3012 = or disjoint i64 %3011, 8
  %3013 = shl i64 %3010, %3012
  %3014 = or i64 %3013, %3007
  %3015 = getelementptr i8, ptr %2995, i64 %3000
  %3016 = load i8, ptr %3015, align 1
  %3017 = zext i8 %3016 to i64
  %3018 = shl i64 %2998, 3
  %3019 = or disjoint i64 %3018, 16
  %3020 = shl i64 %3017, %3019
  %3021 = or i64 %3020, %3014
  %3022 = add i64 %3000, 4
  %3023 = getelementptr i8, ptr %2996, i64 %3000
  %3024 = load i8, ptr %3023, align 1
  %3025 = zext i8 %3024 to i64
  %3026 = shl i64 %2998, 3
  %3027 = or disjoint i64 %3026, 24
  %3028 = shl i64 %3025, %3027
  %3029 = or i64 %3028, %3021
  %3030 = add nuw nsw i64 %2998, 4
  %3031 = add i64 %3001, 4
  %3032 = icmp eq i64 %3031, %2993
  br i1 %3032, label %3061, label %2997, !llvm.loop !17

3033:                                             ; preds = %2927
  %3034 = icmp eq i8 %2933, %2940
  br i1 %3034, label %3082, label %3035

3035:                                             ; preds = %3033
  %3036 = sext i32 %2942 to i64
  %3037 = zext i8 %2941 to i64
  br label %3038

3038:                                             ; preds = %3035, %3038
  %3039 = phi i64 [ %3036, %3035 ], [ %3043, %3038 ]
  %3040 = phi i64 [ 0, %3035 ], [ %3059, %3038 ]
  %3041 = phi i64 [ 0, %3035 ], [ %3058, %3038 ]
  %3042 = phi i32 [ %2951, %3035 ], [ %3052, %3038 ]
  %3043 = add nsw i64 %3039, 1
  %3044 = trunc nsw i64 %3043 to i32
  store i32 %3044, ptr @ip, align 4
  %3045 = getelementptr inbounds i8, ptr %69, i64 %3039
  %3046 = load i8, ptr %3045, align 1
  %3047 = shl i32 %3042, 13
  %3048 = xor i32 %3047, %3042
  %3049 = lshr i32 %3048, 17
  %3050 = xor i32 %3049, %3048
  %3051 = shl i32 %3050, 5
  %3052 = xor i32 %3051, %3050
  store i32 %3052, ptr @vm_code_state, align 4
  %3053 = trunc i32 %3052 to i8
  %3054 = xor i8 %3046, %3053
  %3055 = zext i8 %3054 to i64
  %3056 = shl nsw i64 %3040, 3
  %3057 = shl i64 %3055, %3056
  %3058 = or i64 %3057, %3041
  %3059 = add nuw nsw i64 %3040, 1
  %3060 = icmp eq i64 %3059, %3037
  br i1 %3060, label %3082, label %3038, !llvm.loop !15

3061:                                             ; preds = %2997, %2986
  %3062 = phi i64 [ poison, %2986 ], [ %3029, %2997 ]
  %3063 = phi i64 [ 0, %2986 ], [ %3030, %2997 ]
  %3064 = phi i64 [ 0, %2986 ], [ %3029, %2997 ]
  %3065 = phi i64 [ %2984, %2986 ], [ %3022, %2997 ]
  %3066 = icmp eq i64 %2990, 0
  br i1 %3066, label %3082, label %3067

3067:                                             ; preds = %3061, %3067
  %3068 = phi i64 [ %3079, %3067 ], [ %3063, %3061 ]
  %3069 = phi i64 [ %3078, %3067 ], [ %3064, %3061 ]
  %3070 = phi i64 [ %3072, %3067 ], [ %3065, %3061 ]
  %3071 = phi i64 [ %3080, %3067 ], [ 0, %3061 ]
  %3072 = add i64 %3070, 1
  %3073 = getelementptr inbounds nuw i8, ptr %2988, i64 %3070
  %3074 = load i8, ptr %3073, align 1
  %3075 = zext i8 %3074 to i64
  %3076 = shl nsw i64 %3068, 3
  %3077 = shl i64 %3075, %3076
  %3078 = or i64 %3077, %3069
  %3079 = add nuw nsw i64 %3068, 1
  %3080 = add i64 %3071, 1
  %3081 = icmp eq i64 %3080, %2990
  br i1 %3081, label %3082, label %3067, !llvm.loop !36

3082:                                             ; preds = %3038, %3061, %3067, %3033, %2981
  %3083 = phi i32 [ %2982, %2981 ], [ %2951, %3033 ], [ %2982, %3067 ], [ %2982, %3061 ], [ %3052, %3038 ]
  %3084 = phi i32 [ %2983, %2981 ], [ %2942, %3033 ], [ %2983, %3067 ], [ %2983, %3061 ], [ %3044, %3038 ]
  %3085 = phi i64 [ 0, %2981 ], [ 0, %3033 ], [ %3062, %3061 ], [ %3078, %3067 ], [ %3058, %3038 ]
  br i1 %2869, label %3086, label %3139

3086:                                             ; preds = %3082
  %3087 = sext i32 %3084 to i64
  %3088 = zext nneg i32 %2868 to i64
  br label %3093

3089:                                             ; preds = %3093
  %3090 = shl i64 %3098, 32
  %3091 = ashr exact i64 %3090, 32
  %3092 = zext nneg i32 %2868 to i64
  br label %3116

3093:                                             ; preds = %3086, %3093
  %3094 = phi i64 [ %3087, %3086 ], [ %3098, %3093 ]
  %3095 = phi i64 [ 0, %3086 ], [ %3114, %3093 ]
  %3096 = phi i64 [ 0, %3086 ], [ %3113, %3093 ]
  %3097 = phi i32 [ %3083, %3086 ], [ %3107, %3093 ]
  %3098 = add nsw i64 %3094, 1
  %3099 = trunc nsw i64 %3098 to i32
  store i32 %3099, ptr @ip, align 4
  %3100 = getelementptr inbounds i8, ptr %69, i64 %3094
  %3101 = load i8, ptr %3100, align 1
  %3102 = shl i32 %3097, 13
  %3103 = xor i32 %3102, %3097
  %3104 = lshr i32 %3103, 17
  %3105 = xor i32 %3104, %3103
  %3106 = shl i32 %3105, 5
  %3107 = xor i32 %3106, %3105
  store i32 %3107, ptr @vm_code_state, align 4
  %3108 = trunc i32 %3107 to i8
  %3109 = xor i8 %3101, %3108
  %3110 = zext i8 %3109 to i64
  %3111 = shl nsw i64 %3095, 3
  %3112 = shl i64 %3110, %3111
  %3113 = or i64 %3112, %3096
  %3114 = add nuw nsw i64 %3095, 1
  %3115 = icmp eq i64 %3114, %3088
  br i1 %3115, label %3089, label %3093, !llvm.loop !15

3116:                                             ; preds = %3089, %3116
  %3117 = phi i64 [ %3091, %3089 ], [ %3121, %3116 ]
  %3118 = phi i64 [ 0, %3089 ], [ %3137, %3116 ]
  %3119 = phi i64 [ 0, %3089 ], [ %3136, %3116 ]
  %3120 = phi i32 [ %3107, %3089 ], [ %3130, %3116 ]
  %3121 = add nsw i64 %3117, 1
  %3122 = trunc nsw i64 %3121 to i32
  store i32 %3122, ptr @ip, align 4
  %3123 = getelementptr inbounds i8, ptr %69, i64 %3117
  %3124 = load i8, ptr %3123, align 1
  %3125 = shl i32 %3120, 13
  %3126 = xor i32 %3125, %3120
  %3127 = lshr i32 %3126, 17
  %3128 = xor i32 %3127, %3126
  %3129 = shl i32 %3128, 5
  %3130 = xor i32 %3129, %3128
  store i32 %3130, ptr @vm_code_state, align 4
  %3131 = trunc i32 %3130 to i8
  %3132 = xor i8 %3124, %3131
  %3133 = zext i8 %3132 to i64
  %3134 = shl nsw i64 %3118, 3
  %3135 = shl i64 %3133, %3134
  %3136 = or i64 %3135, %3119
  %3137 = add nuw nsw i64 %3118, 1
  %3138 = icmp eq i64 %3137, %3092
  br i1 %3138, label %3139, label %3116, !llvm.loop !15

3139:                                             ; preds = %3116, %3082
  %3140 = phi i64 [ 0, %3082 ], [ %3113, %3116 ]
  %3141 = phi i64 [ 0, %3082 ], [ %3136, %3116 ]
  %3142 = icmp eq i64 %3085, 0
  %3143 = select i1 %3142, i64 %3141, i64 %3140
  br label %3144

3144:                                             ; preds = %2904, %2898, %3139
  %3145 = phi i64 [ %3143, %3139 ], [ 0, %2898 ], [ %2924, %2904 ]
  %3146 = trunc i64 %3145 to i32
  store i32 %3146, ptr @ip, align 4
  br label %8784

3147:                                             ; preds = %158
  %3148 = add nsw i32 %72, 2
  store i32 %3148, ptr @ip, align 4
  %3149 = sext i32 %73 to i64
  %3150 = getelementptr inbounds i8, ptr %69, i64 %3149
  %3151 = load i8, ptr %3150, align 1
  %3152 = shl i32 %81, 13
  %3153 = xor i32 %3152, %81
  %3154 = lshr i32 %3153, 17
  %3155 = xor i32 %3154, %3153
  %3156 = shl i32 %3155, 5
  %3157 = xor i32 %3156, %3155
  store i32 %3157, ptr @vm_code_state, align 4
  %3158 = trunc i32 %3157 to i8
  %3159 = xor i8 %3151, %3158
  %3160 = add nsw i32 %72, 3
  store i32 %3160, ptr @ip, align 4
  %3161 = sext i32 %3148 to i64
  %3162 = getelementptr inbounds i8, ptr %69, i64 %3161
  %3163 = load i8, ptr %3162, align 1
  %3164 = shl i32 %3157, 13
  %3165 = xor i32 %3164, %3157
  %3166 = lshr i32 %3165, 17
  %3167 = xor i32 %3166, %3165
  %3168 = shl i32 %3167, 5
  %3169 = xor i32 %3168, %3167
  store i32 %3169, ptr @vm_code_state, align 4
  %3170 = trunc i32 %3169 to i8
  %3171 = icmp eq i8 %3163, %3170
  br i1 %3171, label %3172, label %3253

3172:                                             ; preds = %3147
  %3173 = load i32, ptr @pointer_size, align 4
  %3174 = icmp sgt i32 %3173, 0
  br i1 %3174, label %3175, label %3201

3175:                                             ; preds = %3172
  %3176 = sext i32 %3160 to i64
  %3177 = zext nneg i32 %3173 to i64
  br label %3178

3178:                                             ; preds = %3175, %3178
  %3179 = phi i64 [ %3176, %3175 ], [ %3183, %3178 ]
  %3180 = phi i64 [ 0, %3175 ], [ %3199, %3178 ]
  %3181 = phi i64 [ 0, %3175 ], [ %3198, %3178 ]
  %3182 = phi i32 [ %3169, %3175 ], [ %3192, %3178 ]
  %3183 = add nsw i64 %3179, 1
  %3184 = trunc nsw i64 %3183 to i32
  store i32 %3184, ptr @ip, align 4
  %3185 = getelementptr inbounds i8, ptr %69, i64 %3179
  %3186 = load i8, ptr %3185, align 1
  %3187 = shl i32 %3182, 13
  %3188 = xor i32 %3187, %3182
  %3189 = lshr i32 %3188, 17
  %3190 = xor i32 %3189, %3188
  %3191 = shl i32 %3190, 5
  %3192 = xor i32 %3191, %3190
  store i32 %3192, ptr @vm_code_state, align 4
  %3193 = trunc i32 %3192 to i8
  %3194 = xor i8 %3186, %3193
  %3195 = zext i8 %3194 to i64
  %3196 = shl nsw i64 %3180, 3
  %3197 = shl i64 %3195, %3196
  %3198 = or i64 %3197, %3181
  %3199 = add nuw nsw i64 %3180, 1
  %3200 = icmp eq i64 %3199, %3177
  br i1 %3200, label %3201, label %3178, !llvm.loop !15

3201:                                             ; preds = %3178, %3172
  %3202 = phi i32 [ %3169, %3172 ], [ %3192, %3178 ]
  %3203 = phi i32 [ %3160, %3172 ], [ %3184, %3178 ]
  %3204 = phi i64 [ 0, %3172 ], [ %3198, %3178 ]
  %3205 = icmp eq i8 %3151, %3158
  br i1 %3205, label %3302, label %3206

3206:                                             ; preds = %3201
  %3207 = load i64, ptr @data_seg_addr, align 8
  %3208 = inttoptr i64 %3207 to ptr
  %3209 = zext i8 %3159 to i64
  %3210 = and i64 %3209, 3
  %3211 = icmp ult i8 %3159, 4
  br i1 %3211, label %3281, label %3212

3212:                                             ; preds = %3206
  %3213 = and i64 %3209, 252
  %3214 = getelementptr i8, ptr %3208, i64 1
  %3215 = getelementptr i8, ptr %3208, i64 2
  %3216 = getelementptr i8, ptr %3208, i64 3
  br label %3217

3217:                                             ; preds = %3217, %3212
  %3218 = phi i64 [ 0, %3212 ], [ %3250, %3217 ]
  %3219 = phi i64 [ 0, %3212 ], [ %3249, %3217 ]
  %3220 = phi i64 [ %3204, %3212 ], [ %3242, %3217 ]
  %3221 = phi i64 [ 0, %3212 ], [ %3251, %3217 ]
  %3222 = getelementptr inbounds nuw i8, ptr %3208, i64 %3220
  %3223 = load i8, ptr %3222, align 1
  %3224 = zext i8 %3223 to i64
  %3225 = shl nsw i64 %3218, 3
  %3226 = shl i64 %3224, %3225
  %3227 = or i64 %3226, %3219
  %3228 = getelementptr i8, ptr %3214, i64 %3220
  %3229 = load i8, ptr %3228, align 1
  %3230 = zext i8 %3229 to i64
  %3231 = shl i64 %3218, 3
  %3232 = or disjoint i64 %3231, 8
  %3233 = shl i64 %3230, %3232
  %3234 = or i64 %3233, %3227
  %3235 = getelementptr i8, ptr %3215, i64 %3220
  %3236 = load i8, ptr %3235, align 1
  %3237 = zext i8 %3236 to i64
  %3238 = shl i64 %3218, 3
  %3239 = or disjoint i64 %3238, 16
  %3240 = shl i64 %3237, %3239
  %3241 = or i64 %3240, %3234
  %3242 = add i64 %3220, 4
  %3243 = getelementptr i8, ptr %3216, i64 %3220
  %3244 = load i8, ptr %3243, align 1
  %3245 = zext i8 %3244 to i64
  %3246 = shl i64 %3218, 3
  %3247 = or disjoint i64 %3246, 24
  %3248 = shl i64 %3245, %3247
  %3249 = or i64 %3248, %3241
  %3250 = add nuw nsw i64 %3218, 4
  %3251 = add i64 %3221, 4
  %3252 = icmp eq i64 %3251, %3213
  br i1 %3252, label %3281, label %3217, !llvm.loop !17

3253:                                             ; preds = %3147
  %3254 = icmp eq i8 %3151, %3158
  br i1 %3254, label %3302, label %3255

3255:                                             ; preds = %3253
  %3256 = sext i32 %3160 to i64
  %3257 = zext i8 %3159 to i64
  br label %3258

3258:                                             ; preds = %3255, %3258
  %3259 = phi i64 [ %3256, %3255 ], [ %3263, %3258 ]
  %3260 = phi i64 [ 0, %3255 ], [ %3279, %3258 ]
  %3261 = phi i64 [ 0, %3255 ], [ %3278, %3258 ]
  %3262 = phi i32 [ %3169, %3255 ], [ %3272, %3258 ]
  %3263 = add nsw i64 %3259, 1
  %3264 = trunc nsw i64 %3263 to i32
  store i32 %3264, ptr @ip, align 4
  %3265 = getelementptr inbounds i8, ptr %69, i64 %3259
  %3266 = load i8, ptr %3265, align 1
  %3267 = shl i32 %3262, 13
  %3268 = xor i32 %3267, %3262
  %3269 = lshr i32 %3268, 17
  %3270 = xor i32 %3269, %3268
  %3271 = shl i32 %3270, 5
  %3272 = xor i32 %3271, %3270
  store i32 %3272, ptr @vm_code_state, align 4
  %3273 = trunc i32 %3272 to i8
  %3274 = xor i8 %3266, %3273
  %3275 = zext i8 %3274 to i64
  %3276 = shl nsw i64 %3260, 3
  %3277 = shl i64 %3275, %3276
  %3278 = or i64 %3277, %3261
  %3279 = add nuw nsw i64 %3260, 1
  %3280 = icmp eq i64 %3279, %3257
  br i1 %3280, label %3302, label %3258, !llvm.loop !15

3281:                                             ; preds = %3217, %3206
  %3282 = phi i64 [ poison, %3206 ], [ %3249, %3217 ]
  %3283 = phi i64 [ 0, %3206 ], [ %3250, %3217 ]
  %3284 = phi i64 [ 0, %3206 ], [ %3249, %3217 ]
  %3285 = phi i64 [ %3204, %3206 ], [ %3242, %3217 ]
  %3286 = icmp eq i64 %3210, 0
  br i1 %3286, label %3302, label %3287

3287:                                             ; preds = %3281, %3287
  %3288 = phi i64 [ %3299, %3287 ], [ %3283, %3281 ]
  %3289 = phi i64 [ %3298, %3287 ], [ %3284, %3281 ]
  %3290 = phi i64 [ %3292, %3287 ], [ %3285, %3281 ]
  %3291 = phi i64 [ %3300, %3287 ], [ 0, %3281 ]
  %3292 = add i64 %3290, 1
  %3293 = getelementptr inbounds nuw i8, ptr %3208, i64 %3290
  %3294 = load i8, ptr %3293, align 1
  %3295 = zext i8 %3294 to i64
  %3296 = shl nsw i64 %3288, 3
  %3297 = shl i64 %3295, %3296
  %3298 = or i64 %3297, %3289
  %3299 = add nuw nsw i64 %3288, 1
  %3300 = add i64 %3291, 1
  %3301 = icmp eq i64 %3300, %3210
  br i1 %3301, label %3302, label %3287, !llvm.loop !37

3302:                                             ; preds = %3258, %3281, %3287, %3253, %3201
  %3303 = phi i32 [ %3202, %3201 ], [ %3169, %3253 ], [ %3202, %3287 ], [ %3202, %3281 ], [ %3272, %3258 ]
  %3304 = phi i32 [ %3203, %3201 ], [ %3160, %3253 ], [ %3203, %3287 ], [ %3203, %3281 ], [ %3264, %3258 ]
  %3305 = phi i64 [ 0, %3201 ], [ 0, %3253 ], [ %3282, %3281 ], [ %3298, %3287 ], [ %3278, %3258 ]
  %3306 = sext i32 %3304 to i64
  %3307 = add nsw i64 %3306, 1
  %3308 = trunc nsw i64 %3307 to i32
  store i32 %3308, ptr @ip, align 4
  %3309 = getelementptr inbounds i8, ptr %69, i64 %3306
  %3310 = load i8, ptr %3309, align 1
  %3311 = shl i32 %3303, 13
  %3312 = xor i32 %3311, %3303
  %3313 = lshr i32 %3312, 17
  %3314 = xor i32 %3313, %3312
  %3315 = shl i32 %3314, 5
  %3316 = xor i32 %3315, %3314
  store i32 %3316, ptr @vm_code_state, align 4
  %3317 = add nsw i64 %3306, 2
  %3318 = trunc nsw i64 %3317 to i32
  store i32 %3318, ptr @ip, align 4
  %3319 = getelementptr inbounds i8, ptr %69, i64 %3307
  %3320 = load i8, ptr %3319, align 1
  %3321 = shl i32 %3316, 13
  %3322 = xor i32 %3321, %3316
  %3323 = lshr i32 %3322, 17
  %3324 = xor i32 %3323, %3322
  %3325 = shl i32 %3324, 5
  %3326 = xor i32 %3325, %3324
  store i32 %3326, ptr @vm_code_state, align 4
  %3327 = add nsw i64 %3306, 3
  %3328 = trunc nsw i64 %3327 to i32
  store i32 %3328, ptr @ip, align 4
  %3329 = getelementptr inbounds i8, ptr %69, i64 %3317
  %3330 = load i8, ptr %3329, align 1
  %3331 = shl i32 %3326, 13
  %3332 = xor i32 %3331, %3326
  %3333 = lshr i32 %3332, 17
  %3334 = xor i32 %3333, %3332
  %3335 = shl i32 %3334, 5
  %3336 = xor i32 %3335, %3334
  store i32 %3336, ptr @vm_code_state, align 4
  %3337 = add i32 %3304, 4
  store i32 %3337, ptr @ip, align 4
  %3338 = getelementptr inbounds i8, ptr %69, i64 %3327
  %3339 = load i8, ptr %3338, align 1
  %3340 = shl i32 %3336, 13
  %3341 = xor i32 %3340, %3336
  %3342 = lshr i32 %3341, 17
  %3343 = xor i32 %3342, %3341
  %3344 = shl i32 %3343, 5
  %3345 = xor i32 %3344, %3343
  store i32 %3345, ptr @vm_code_state, align 4
  %3346 = sext i32 %3337 to i64
  %3347 = add nsw i64 %3346, 1
  %3348 = trunc nsw i64 %3347 to i32
  store i32 %3348, ptr @ip, align 4
  %3349 = getelementptr inbounds i8, ptr %69, i64 %3346
  %3350 = load i8, ptr %3349, align 1
  %3351 = shl i32 %3345, 13
  %3352 = xor i32 %3351, %3345
  %3353 = lshr i32 %3352, 17
  %3354 = xor i32 %3353, %3352
  %3355 = shl i32 %3354, 5
  %3356 = xor i32 %3355, %3354
  store i32 %3356, ptr @vm_code_state, align 4
  %3357 = trunc i32 %3356 to i8
  %3358 = xor i8 %3350, %3357
  %3359 = zext i8 %3358 to i32
  %3360 = add nsw i64 %3346, 2
  %3361 = trunc nsw i64 %3360 to i32
  store i32 %3361, ptr @ip, align 4
  %3362 = getelementptr inbounds i8, ptr %69, i64 %3347
  %3363 = load i8, ptr %3362, align 1
  %3364 = shl i32 %3356, 13
  %3365 = xor i32 %3364, %3356
  %3366 = lshr i32 %3365, 17
  %3367 = xor i32 %3366, %3365
  %3368 = shl i32 %3367, 5
  %3369 = xor i32 %3368, %3367
  store i32 %3369, ptr @vm_code_state, align 4
  %3370 = trunc i32 %3369 to i8
  %3371 = xor i8 %3363, %3370
  %3372 = zext i8 %3371 to i32
  %3373 = shl nuw nsw i32 %3372, 8
  %3374 = or disjoint i32 %3373, %3359
  %3375 = add nsw i64 %3346, 3
  %3376 = trunc nsw i64 %3375 to i32
  store i32 %3376, ptr @ip, align 4
  %3377 = getelementptr inbounds i8, ptr %69, i64 %3360
  %3378 = load i8, ptr %3377, align 1
  %3379 = shl i32 %3369, 13
  %3380 = xor i32 %3379, %3369
  %3381 = lshr i32 %3380, 17
  %3382 = xor i32 %3381, %3380
  %3383 = shl i32 %3382, 5
  %3384 = xor i32 %3383, %3382
  store i32 %3384, ptr @vm_code_state, align 4
  %3385 = trunc i32 %3384 to i8
  %3386 = xor i8 %3378, %3385
  %3387 = zext i8 %3386 to i32
  %3388 = shl nuw nsw i32 %3387, 16
  %3389 = or disjoint i32 %3388, %3374
  %3390 = add i32 %3304, 8
  store i32 %3390, ptr @ip, align 4
  %3391 = getelementptr inbounds i8, ptr %69, i64 %3375
  %3392 = load i8, ptr %3391, align 1
  %3393 = shl i32 %3384, 13
  %3394 = xor i32 %3393, %3384
  %3395 = lshr i32 %3394, 17
  %3396 = xor i32 %3395, %3394
  %3397 = shl i32 %3396, 5
  %3398 = xor i32 %3397, %3396
  store i32 %3398, ptr @vm_code_state, align 4
  %3399 = trunc i32 %3398 to i8
  %3400 = xor i8 %3392, %3399
  %3401 = zext i8 %3400 to i32
  %3402 = shl nuw i32 %3401, 24
  %3403 = or disjoint i32 %3402, %3389
  %3404 = trunc i32 %3316 to i8
  %3405 = xor i8 %3310, %3404
  %3406 = zext i8 %3405 to i32
  %3407 = trunc i32 %3326 to i8
  %3408 = xor i8 %3320, %3407
  %3409 = zext i8 %3408 to i32
  %3410 = shl nuw nsw i32 %3409, 8
  %3411 = or disjoint i32 %3410, %3406
  %3412 = trunc i32 %3336 to i8
  %3413 = xor i8 %3330, %3412
  %3414 = zext i8 %3413 to i32
  %3415 = shl nuw nsw i32 %3414, 16
  %3416 = or disjoint i32 %3415, %3411
  %3417 = trunc i32 %3345 to i8
  %3418 = xor i8 %3339, %3417
  %3419 = zext i8 %3418 to i32
  %3420 = shl nuw i32 %3419, 24
  %3421 = or disjoint i32 %3420, %3416
  %3422 = load i32, ptr @pointer_size, align 4
  %3423 = icmp sgt i32 %3422, 0
  br i1 %3423, label %3424, label %3427

3424:                                             ; preds = %3302
  %3425 = sext i32 %3390 to i64
  %3426 = zext nneg i32 %3422 to i64
  br label %3436

3427:                                             ; preds = %3436, %3302
  %3428 = phi i32 [ %3390, %3302 ], [ %3442, %3436 ]
  %3429 = phi i32 [ %3398, %3302 ], [ %3450, %3436 ]
  %3430 = phi i64 [ 0, %3302 ], [ %3456, %3436 ]
  %3431 = icmp eq i32 %3421, 0
  br i1 %3431, label %3619, label %3432

3432:                                             ; preds = %3427
  %3433 = icmp sgt i32 %3403, 0
  %3434 = zext nneg i32 %3403 to i64
  %3435 = zext nneg i32 %3422 to i64
  br label %3459

3436:                                             ; preds = %3424, %3436
  %3437 = phi i64 [ %3425, %3424 ], [ %3441, %3436 ]
  %3438 = phi i64 [ 0, %3424 ], [ %3457, %3436 ]
  %3439 = phi i64 [ 0, %3424 ], [ %3456, %3436 ]
  %3440 = phi i32 [ %3398, %3424 ], [ %3450, %3436 ]
  %3441 = add nsw i64 %3437, 1
  %3442 = trunc nsw i64 %3441 to i32
  store i32 %3442, ptr @ip, align 4
  %3443 = getelementptr inbounds i8, ptr %69, i64 %3437
  %3444 = load i8, ptr %3443, align 1
  %3445 = shl i32 %3440, 13
  %3446 = xor i32 %3445, %3440
  %3447 = lshr i32 %3446, 17
  %3448 = xor i32 %3447, %3446
  %3449 = shl i32 %3448, 5
  %3450 = xor i32 %3449, %3448
  store i32 %3450, ptr @vm_code_state, align 4
  %3451 = trunc i32 %3450 to i8
  %3452 = xor i8 %3444, %3451
  %3453 = zext i8 %3452 to i64
  %3454 = shl nsw i64 %3438, 3
  %3455 = shl i64 %3453, %3454
  %3456 = or i64 %3455, %3439
  %3457 = add nuw nsw i64 %3438, 1
  %3458 = icmp eq i64 %3457, %3426
  br i1 %3458, label %3427, label %3436, !llvm.loop !15

3459:                                             ; preds = %3432, %3617
  %3460 = phi i32 [ 0, %3432 ], [ %3522, %3617 ]
  %3461 = phi i32 [ %3428, %3432 ], [ %3519, %3617 ]
  %3462 = phi i32 [ %3429, %3432 ], [ %3518, %3617 ]
  br i1 %3433, label %3463, label %3465

3463:                                             ; preds = %3459
  %3464 = sext i32 %3461 to i64
  br label %3471

3465:                                             ; preds = %3471, %3459
  %3466 = phi i32 [ %3462, %3459 ], [ %3485, %3471 ]
  %3467 = phi i32 [ %3461, %3459 ], [ %3477, %3471 ]
  %3468 = phi i64 [ 0, %3459 ], [ %3491, %3471 ]
  br i1 %3423, label %3469, label %3517

3469:                                             ; preds = %3465
  %3470 = sext i32 %3467 to i64
  br label %3494

3471:                                             ; preds = %3463, %3471
  %3472 = phi i64 [ %3464, %3463 ], [ %3476, %3471 ]
  %3473 = phi i64 [ 0, %3463 ], [ %3492, %3471 ]
  %3474 = phi i64 [ 0, %3463 ], [ %3491, %3471 ]
  %3475 = phi i32 [ %3462, %3463 ], [ %3485, %3471 ]
  %3476 = add nsw i64 %3472, 1
  %3477 = trunc nsw i64 %3476 to i32
  store i32 %3477, ptr @ip, align 4
  %3478 = getelementptr inbounds i8, ptr %69, i64 %3472
  %3479 = load i8, ptr %3478, align 1
  %3480 = shl i32 %3475, 13
  %3481 = xor i32 %3480, %3475
  %3482 = lshr i32 %3481, 17
  %3483 = xor i32 %3482, %3481
  %3484 = shl i32 %3483, 5
  %3485 = xor i32 %3484, %3483
  store i32 %3485, ptr @vm_code_state, align 4
  %3486 = trunc i32 %3485 to i8
  %3487 = xor i8 %3479, %3486
  %3488 = zext i8 %3487 to i64
  %3489 = shl nsw i64 %3473, 3
  %3490 = shl i64 %3488, %3489
  %3491 = or i64 %3490, %3474
  %3492 = add nuw nsw i64 %3473, 1
  %3493 = icmp eq i64 %3492, %3434
  br i1 %3493, label %3465, label %3471, !llvm.loop !15

3494:                                             ; preds = %3469, %3494
  %3495 = phi i64 [ %3470, %3469 ], [ %3499, %3494 ]
  %3496 = phi i64 [ 0, %3469 ], [ %3515, %3494 ]
  %3497 = phi i64 [ 0, %3469 ], [ %3514, %3494 ]
  %3498 = phi i32 [ %3466, %3469 ], [ %3508, %3494 ]
  %3499 = add nsw i64 %3495, 1
  %3500 = trunc nsw i64 %3499 to i32
  store i32 %3500, ptr @ip, align 4
  %3501 = getelementptr inbounds i8, ptr %69, i64 %3495
  %3502 = load i8, ptr %3501, align 1
  %3503 = shl i32 %3498, 13
  %3504 = xor i32 %3503, %3498
  %3505 = lshr i32 %3504, 17
  %3506 = xor i32 %3505, %3504
  %3507 = shl i32 %3506, 5
  %3508 = xor i32 %3507, %3506
  store i32 %3508, ptr @vm_code_state, align 4
  %3509 = trunc i32 %3508 to i8
  %3510 = xor i8 %3502, %3509
  %3511 = zext i8 %3510 to i64
  %3512 = shl nsw i64 %3496, 3
  %3513 = shl i64 %3511, %3512
  %3514 = or i64 %3513, %3497
  %3515 = add nuw nsw i64 %3496, 1
  %3516 = icmp eq i64 %3515, %3435
  br i1 %3516, label %3517, label %3494, !llvm.loop !15

3517:                                             ; preds = %3494, %3465
  %3518 = phi i32 [ %3466, %3465 ], [ %3508, %3494 ]
  %3519 = phi i32 [ %3467, %3465 ], [ %3500, %3494 ]
  %3520 = phi i64 [ 0, %3465 ], [ %3514, %3494 ]
  %3521 = icmp eq i64 %3305, %3468
  %3522 = add i32 %3460, 1
  br i1 %3521, label %3523, label %3617

3523:                                             ; preds = %3517
  %3524 = icmp ult i32 %3522, %3421
  br i1 %3524, label %3525, label %3619

3525:                                             ; preds = %3523
  %3526 = tail call i32 @llvm.smax.i32(i32 %3403, i32 0)
  %3527 = add i32 %3422, %3526
  %3528 = or disjoint i32 %3402, %3388
  %3529 = or disjoint i32 %3528, %3373
  %3530 = or disjoint i32 %3529, %3359
  %3531 = add i32 %3402, -1
  %3532 = add i32 %3531, %3388
  %3533 = add i32 %3532, %3373
  %3534 = and i32 %3359, 1
  %3535 = sub nsw i32 0, %3359
  %3536 = icmp eq i32 %3533, %3535
  %3537 = sub i32 %3530, %3534
  %3538 = icmp eq i32 %3534, 0
  %3539 = and i32 %3422, 1
  %3540 = icmp eq i32 %3422, 1
  %3541 = and i32 %3422, 2147483646
  %3542 = icmp eq i32 %3539, 0
  br label %3561

3543:                                             ; preds = %3600, %3582
  %3544 = phi i32 [ poison, %3582 ], [ %3614, %3600 ]
  %3545 = phi i32 [ %3580, %3582 ], [ %3614, %3600 ]
  br i1 %3542, label %3553, label %3546

3546:                                             ; preds = %3543
  %3547 = shl i32 %3545, 13
  %3548 = xor i32 %3547, %3545
  %3549 = lshr i32 %3548, 17
  %3550 = xor i32 %3549, %3548
  %3551 = shl i32 %3550, 5
  %3552 = xor i32 %3551, %3550
  br label %3553

3553:                                             ; preds = %3543, %3546
  %3554 = phi i32 [ %3544, %3543 ], [ %3552, %3546 ]
  %3555 = add i32 %3527, %3563
  store i32 %3555, ptr @ip, align 4
  store i32 %3554, ptr @vm_code_state, align 4
  br label %3556

3556:                                             ; preds = %3553, %3579
  %3557 = phi i32 [ %3554, %3553 ], [ %3580, %3579 ]
  %3558 = phi i32 [ %3555, %3553 ], [ %3581, %3579 ]
  %3559 = add nuw i32 %3562, 1
  %3560 = icmp eq i32 %3559, %3421
  br i1 %3560, label %3619, label %3561, !llvm.loop !38

3561:                                             ; preds = %3525, %3556
  %3562 = phi i32 [ %3522, %3525 ], [ %3559, %3556 ]
  %3563 = phi i32 [ %3519, %3525 ], [ %3558, %3556 ]
  %3564 = phi i32 [ %3518, %3525 ], [ %3557, %3556 ]
  br i1 %3433, label %3565, label %3579

3565:                                             ; preds = %3561
  br i1 %3536, label %3566, label %3583

3566:                                             ; preds = %3583, %3565
  %3567 = phi i32 [ poison, %3565 ], [ %3597, %3583 ]
  %3568 = phi i32 [ %3564, %3565 ], [ %3597, %3583 ]
  br i1 %3538, label %3576, label %3569

3569:                                             ; preds = %3566
  %3570 = shl i32 %3568, 13
  %3571 = xor i32 %3570, %3568
  %3572 = lshr i32 %3571, 17
  %3573 = xor i32 %3572, %3571
  %3574 = shl i32 %3573, 5
  %3575 = xor i32 %3574, %3573
  br label %3576

3576:                                             ; preds = %3566, %3569
  %3577 = phi i32 [ %3567, %3566 ], [ %3575, %3569 ]
  %3578 = add i32 %3403, %3563
  store i32 %3577, ptr @vm_code_state, align 4
  br label %3579

3579:                                             ; preds = %3576, %3561
  %3580 = phi i32 [ %3577, %3576 ], [ %3564, %3561 ]
  %3581 = phi i32 [ %3578, %3576 ], [ %3563, %3561 ]
  br i1 %3423, label %3582, label %3556

3582:                                             ; preds = %3579
  br i1 %3540, label %3543, label %3600

3583:                                             ; preds = %3565, %3583
  %3584 = phi i32 [ %3597, %3583 ], [ %3564, %3565 ]
  %3585 = phi i32 [ %3598, %3583 ], [ 0, %3565 ]
  %3586 = shl i32 %3584, 13
  %3587 = xor i32 %3586, %3584
  %3588 = lshr i32 %3587, 17
  %3589 = xor i32 %3588, %3587
  %3590 = shl i32 %3589, 5
  %3591 = xor i32 %3590, %3589
  %3592 = shl i32 %3591, 13
  %3593 = xor i32 %3592, %3591
  %3594 = lshr i32 %3593, 17
  %3595 = xor i32 %3594, %3593
  %3596 = shl i32 %3595, 5
  %3597 = xor i32 %3596, %3595
  %3598 = add i32 %3585, 2
  %3599 = icmp eq i32 %3598, %3537
  br i1 %3599, label %3566, label %3583, !llvm.loop !15

3600:                                             ; preds = %3582, %3600
  %3601 = phi i32 [ %3614, %3600 ], [ %3580, %3582 ]
  %3602 = phi i32 [ %3615, %3600 ], [ 0, %3582 ]
  %3603 = shl i32 %3601, 13
  %3604 = xor i32 %3603, %3601
  %3605 = lshr i32 %3604, 17
  %3606 = xor i32 %3605, %3604
  %3607 = shl i32 %3606, 5
  %3608 = xor i32 %3607, %3606
  %3609 = shl i32 %3608, 13
  %3610 = xor i32 %3609, %3608
  %3611 = lshr i32 %3610, 17
  %3612 = xor i32 %3611, %3610
  %3613 = shl i32 %3612, 5
  %3614 = xor i32 %3613, %3612
  %3615 = add i32 %3602, 2
  %3616 = icmp eq i32 %3615, %3541
  br i1 %3616, label %3543, label %3600, !llvm.loop !15

3617:                                             ; preds = %3517
  %3618 = icmp eq i32 %3522, %3421
  br i1 %3618, label %3619, label %3459, !llvm.loop !39

3619:                                             ; preds = %3617, %3556, %3427, %3523
  %3620 = phi i64 [ %3520, %3523 ], [ %3430, %3427 ], [ %3520, %3556 ], [ %3430, %3617 ]
  %3621 = trunc i64 %3620 to i32
  store i32 %3621, ptr @ip, align 4
  br label %8784

3622:                                             ; preds = %158
  %3623 = add nsw i32 %72, 2
  store i32 %3623, ptr @ip, align 4
  %3624 = sext i32 %73 to i64
  %3625 = getelementptr inbounds i8, ptr %69, i64 %3624
  %3626 = load i8, ptr %3625, align 1
  %3627 = shl i32 %81, 13
  %3628 = xor i32 %3627, %81
  %3629 = lshr i32 %3628, 17
  %3630 = xor i32 %3629, %3628
  %3631 = shl i32 %3630, 5
  %3632 = xor i32 %3631, %3630
  %3633 = trunc i32 %3632 to i8
  %3634 = xor i8 %3626, %3633
  %3635 = add nsw i32 %72, 3
  %3636 = shl i32 %3632, 13
  %3637 = xor i32 %3636, %3632
  %3638 = lshr i32 %3637, 17
  %3639 = xor i32 %3638, %3637
  %3640 = shl i32 %3639, 5
  %3641 = xor i32 %3640, %3639
  store i32 %3641, ptr @vm_code_state, align 4
  %3642 = load i32, ptr @pointer_size, align 4
  %3643 = icmp sgt i32 %3642, 0
  br i1 %3643, label %3644, label %3670

3644:                                             ; preds = %3622
  %3645 = sext i32 %3635 to i64
  %3646 = zext nneg i32 %3642 to i64
  br label %3647

3647:                                             ; preds = %3644, %3647
  %3648 = phi i64 [ %3645, %3644 ], [ %3652, %3647 ]
  %3649 = phi i64 [ 0, %3644 ], [ %3668, %3647 ]
  %3650 = phi i64 [ 0, %3644 ], [ %3667, %3647 ]
  %3651 = phi i32 [ %3641, %3644 ], [ %3661, %3647 ]
  %3652 = add nsw i64 %3648, 1
  %3653 = trunc nsw i64 %3652 to i32
  store i32 %3653, ptr @ip, align 4
  %3654 = getelementptr inbounds i8, ptr %69, i64 %3648
  %3655 = load i8, ptr %3654, align 1
  %3656 = shl i32 %3651, 13
  %3657 = xor i32 %3656, %3651
  %3658 = lshr i32 %3657, 17
  %3659 = xor i32 %3658, %3657
  %3660 = shl i32 %3659, 5
  %3661 = xor i32 %3660, %3659
  store i32 %3661, ptr @vm_code_state, align 4
  %3662 = trunc i32 %3661 to i8
  %3663 = xor i8 %3655, %3662
  %3664 = zext i8 %3663 to i64
  %3665 = shl nsw i64 %3649, 3
  %3666 = shl i64 %3664, %3665
  %3667 = or i64 %3666, %3650
  %3668 = add nuw nsw i64 %3649, 1
  %3669 = icmp eq i64 %3668, %3646
  br i1 %3669, label %3670, label %3647, !llvm.loop !15

3670:                                             ; preds = %3647, %3622
  %3671 = phi i32 [ %3641, %3622 ], [ %3661, %3647 ]
  %3672 = phi i32 [ %3635, %3622 ], [ %3653, %3647 ]
  %3673 = phi i64 [ 0, %3622 ], [ %3667, %3647 ]
  %3674 = add nsw i32 %3672, 1
  store i32 %3674, ptr @ip, align 4
  %3675 = sext i32 %3672 to i64
  %3676 = getelementptr inbounds i8, ptr %69, i64 %3675
  %3677 = load i8, ptr %3676, align 1
  %3678 = shl i32 %3671, 13
  %3679 = xor i32 %3678, %3671
  %3680 = lshr i32 %3679, 17
  %3681 = xor i32 %3680, %3679
  %3682 = shl i32 %3681, 5
  %3683 = xor i32 %3682, %3681
  store i32 %3683, ptr @vm_code_state, align 4
  %3684 = trunc i32 %3683 to i8
  %3685 = xor i8 %3677, %3684
  %3686 = add nsw i32 %3672, 2
  store i32 %3686, ptr @ip, align 4
  %3687 = sext i32 %3674 to i64
  %3688 = getelementptr inbounds i8, ptr %69, i64 %3687
  %3689 = load i8, ptr %3688, align 1
  %3690 = shl i32 %3683, 13
  %3691 = xor i32 %3690, %3683
  %3692 = lshr i32 %3691, 17
  %3693 = xor i32 %3692, %3691
  %3694 = shl i32 %3693, 5
  %3695 = xor i32 %3694, %3693
  store i32 %3695, ptr @vm_code_state, align 4
  %3696 = trunc i32 %3695 to i8
  %3697 = icmp eq i8 %3689, %3696
  br i1 %3697, label %3698, label %3777

3698:                                             ; preds = %3670
  br i1 %3643, label %3699, label %3725

3699:                                             ; preds = %3698
  %3700 = sext i32 %3686 to i64
  %3701 = zext nneg i32 %3642 to i64
  br label %3702

3702:                                             ; preds = %3699, %3702
  %3703 = phi i64 [ %3700, %3699 ], [ %3707, %3702 ]
  %3704 = phi i64 [ 0, %3699 ], [ %3723, %3702 ]
  %3705 = phi i64 [ 0, %3699 ], [ %3722, %3702 ]
  %3706 = phi i32 [ %3695, %3699 ], [ %3716, %3702 ]
  %3707 = add nsw i64 %3703, 1
  %3708 = trunc nsw i64 %3707 to i32
  store i32 %3708, ptr @ip, align 4
  %3709 = getelementptr inbounds i8, ptr %69, i64 %3703
  %3710 = load i8, ptr %3709, align 1
  %3711 = shl i32 %3706, 13
  %3712 = xor i32 %3711, %3706
  %3713 = lshr i32 %3712, 17
  %3714 = xor i32 %3713, %3712
  %3715 = shl i32 %3714, 5
  %3716 = xor i32 %3715, %3714
  store i32 %3716, ptr @vm_code_state, align 4
  %3717 = trunc i32 %3716 to i8
  %3718 = xor i8 %3710, %3717
  %3719 = zext i8 %3718 to i64
  %3720 = shl nsw i64 %3704, 3
  %3721 = shl i64 %3719, %3720
  %3722 = or i64 %3721, %3705
  %3723 = add nuw nsw i64 %3704, 1
  %3724 = icmp eq i64 %3723, %3701
  br i1 %3724, label %3725, label %3702, !llvm.loop !15

3725:                                             ; preds = %3702, %3698
  %3726 = phi i32 [ %3695, %3698 ], [ %3716, %3702 ]
  %3727 = phi i32 [ %3686, %3698 ], [ %3708, %3702 ]
  %3728 = phi i64 [ 0, %3698 ], [ %3722, %3702 ]
  %3729 = icmp eq i8 %3677, %3684
  br i1 %3729, label %3826, label %3730

3730:                                             ; preds = %3725
  %3731 = load i64, ptr @data_seg_addr, align 8
  %3732 = inttoptr i64 %3731 to ptr
  %3733 = zext i8 %3685 to i64
  %3734 = and i64 %3733, 3
  %3735 = icmp ult i8 %3685, 4
  br i1 %3735, label %3805, label %3736

3736:                                             ; preds = %3730
  %3737 = and i64 %3733, 252
  %3738 = getelementptr i8, ptr %3732, i64 1
  %3739 = getelementptr i8, ptr %3732, i64 2
  %3740 = getelementptr i8, ptr %3732, i64 3
  br label %3741

3741:                                             ; preds = %3741, %3736
  %3742 = phi i64 [ 0, %3736 ], [ %3774, %3741 ]
  %3743 = phi i64 [ 0, %3736 ], [ %3773, %3741 ]
  %3744 = phi i64 [ %3728, %3736 ], [ %3766, %3741 ]
  %3745 = phi i64 [ 0, %3736 ], [ %3775, %3741 ]
  %3746 = getelementptr inbounds nuw i8, ptr %3732, i64 %3744
  %3747 = load i8, ptr %3746, align 1
  %3748 = zext i8 %3747 to i64
  %3749 = shl nsw i64 %3742, 3
  %3750 = shl i64 %3748, %3749
  %3751 = or i64 %3750, %3743
  %3752 = getelementptr i8, ptr %3738, i64 %3744
  %3753 = load i8, ptr %3752, align 1
  %3754 = zext i8 %3753 to i64
  %3755 = shl i64 %3742, 3
  %3756 = or disjoint i64 %3755, 8
  %3757 = shl i64 %3754, %3756
  %3758 = or i64 %3757, %3751
  %3759 = getelementptr i8, ptr %3739, i64 %3744
  %3760 = load i8, ptr %3759, align 1
  %3761 = zext i8 %3760 to i64
  %3762 = shl i64 %3742, 3
  %3763 = or disjoint i64 %3762, 16
  %3764 = shl i64 %3761, %3763
  %3765 = or i64 %3764, %3758
  %3766 = add i64 %3744, 4
  %3767 = getelementptr i8, ptr %3740, i64 %3744
  %3768 = load i8, ptr %3767, align 1
  %3769 = zext i8 %3768 to i64
  %3770 = shl i64 %3742, 3
  %3771 = or disjoint i64 %3770, 24
  %3772 = shl i64 %3769, %3771
  %3773 = or i64 %3772, %3765
  %3774 = add nuw nsw i64 %3742, 4
  %3775 = add i64 %3745, 4
  %3776 = icmp eq i64 %3775, %3737
  br i1 %3776, label %3805, label %3741, !llvm.loop !17

3777:                                             ; preds = %3670
  %3778 = icmp eq i8 %3677, %3684
  br i1 %3778, label %3826, label %3779

3779:                                             ; preds = %3777
  %3780 = sext i32 %3686 to i64
  %3781 = zext i8 %3685 to i64
  br label %3782

3782:                                             ; preds = %3779, %3782
  %3783 = phi i64 [ %3780, %3779 ], [ %3787, %3782 ]
  %3784 = phi i64 [ 0, %3779 ], [ %3803, %3782 ]
  %3785 = phi i64 [ 0, %3779 ], [ %3802, %3782 ]
  %3786 = phi i32 [ %3695, %3779 ], [ %3796, %3782 ]
  %3787 = add nsw i64 %3783, 1
  %3788 = trunc nsw i64 %3787 to i32
  store i32 %3788, ptr @ip, align 4
  %3789 = getelementptr inbounds i8, ptr %69, i64 %3783
  %3790 = load i8, ptr %3789, align 1
  %3791 = shl i32 %3786, 13
  %3792 = xor i32 %3791, %3786
  %3793 = lshr i32 %3792, 17
  %3794 = xor i32 %3793, %3792
  %3795 = shl i32 %3794, 5
  %3796 = xor i32 %3795, %3794
  store i32 %3796, ptr @vm_code_state, align 4
  %3797 = trunc i32 %3796 to i8
  %3798 = xor i8 %3790, %3797
  %3799 = zext i8 %3798 to i64
  %3800 = shl nsw i64 %3784, 3
  %3801 = shl i64 %3799, %3800
  %3802 = or i64 %3801, %3785
  %3803 = add nuw nsw i64 %3784, 1
  %3804 = icmp eq i64 %3803, %3781
  br i1 %3804, label %3826, label %3782, !llvm.loop !15

3805:                                             ; preds = %3741, %3730
  %3806 = phi i64 [ poison, %3730 ], [ %3773, %3741 ]
  %3807 = phi i64 [ 0, %3730 ], [ %3774, %3741 ]
  %3808 = phi i64 [ 0, %3730 ], [ %3773, %3741 ]
  %3809 = phi i64 [ %3728, %3730 ], [ %3766, %3741 ]
  %3810 = icmp eq i64 %3734, 0
  br i1 %3810, label %3826, label %3811

3811:                                             ; preds = %3805, %3811
  %3812 = phi i64 [ %3823, %3811 ], [ %3807, %3805 ]
  %3813 = phi i64 [ %3822, %3811 ], [ %3808, %3805 ]
  %3814 = phi i64 [ %3816, %3811 ], [ %3809, %3805 ]
  %3815 = phi i64 [ %3824, %3811 ], [ 0, %3805 ]
  %3816 = add i64 %3814, 1
  %3817 = getelementptr inbounds nuw i8, ptr %3732, i64 %3814
  %3818 = load i8, ptr %3817, align 1
  %3819 = zext i8 %3818 to i64
  %3820 = shl nsw i64 %3812, 3
  %3821 = shl i64 %3819, %3820
  %3822 = or i64 %3821, %3813
  %3823 = add nuw nsw i64 %3812, 1
  %3824 = add i64 %3815, 1
  %3825 = icmp eq i64 %3824, %3734
  br i1 %3825, label %3826, label %3811, !llvm.loop !40

3826:                                             ; preds = %3782, %3805, %3811, %3777, %3725
  %3827 = phi i32 [ %3726, %3725 ], [ %3695, %3777 ], [ %3726, %3811 ], [ %3726, %3805 ], [ %3796, %3782 ]
  %3828 = phi i32 [ %3727, %3725 ], [ %3686, %3777 ], [ %3727, %3811 ], [ %3727, %3805 ], [ %3788, %3782 ]
  %3829 = phi i64 [ 0, %3725 ], [ 0, %3777 ], [ %3806, %3805 ], [ %3822, %3811 ], [ %3802, %3782 ]
  %3830 = add nsw i32 %3828, 1
  store i32 %3830, ptr @ip, align 4
  %3831 = sext i32 %3828 to i64
  %3832 = getelementptr inbounds i8, ptr %69, i64 %3831
  %3833 = load i8, ptr %3832, align 1
  %3834 = shl i32 %3827, 13
  %3835 = xor i32 %3834, %3827
  %3836 = lshr i32 %3835, 17
  %3837 = xor i32 %3836, %3835
  %3838 = shl i32 %3837, 5
  %3839 = xor i32 %3838, %3837
  store i32 %3839, ptr @vm_code_state, align 4
  %3840 = trunc i32 %3839 to i8
  %3841 = xor i8 %3833, %3840
  %3842 = add nsw i32 %3828, 2
  store i32 %3842, ptr @ip, align 4
  %3843 = sext i32 %3830 to i64
  %3844 = getelementptr inbounds i8, ptr %69, i64 %3843
  %3845 = load i8, ptr %3844, align 1
  %3846 = shl i32 %3839, 13
  %3847 = xor i32 %3846, %3839
  %3848 = lshr i32 %3847, 17
  %3849 = xor i32 %3848, %3847
  %3850 = shl i32 %3849, 5
  %3851 = xor i32 %3850, %3849
  store i32 %3851, ptr @vm_code_state, align 4
  %3852 = trunc i32 %3851 to i8
  %3853 = icmp eq i8 %3845, %3852
  br i1 %3853, label %3854, label %3933

3854:                                             ; preds = %3826
  br i1 %3643, label %3855, label %3881

3855:                                             ; preds = %3854
  %3856 = sext i32 %3842 to i64
  %3857 = zext nneg i32 %3642 to i64
  br label %3858

3858:                                             ; preds = %3855, %3858
  %3859 = phi i64 [ %3856, %3855 ], [ %3863, %3858 ]
  %3860 = phi i64 [ 0, %3855 ], [ %3879, %3858 ]
  %3861 = phi i64 [ 0, %3855 ], [ %3878, %3858 ]
  %3862 = phi i32 [ %3851, %3855 ], [ %3872, %3858 ]
  %3863 = add nsw i64 %3859, 1
  %3864 = trunc nsw i64 %3863 to i32
  store i32 %3864, ptr @ip, align 4
  %3865 = getelementptr inbounds i8, ptr %69, i64 %3859
  %3866 = load i8, ptr %3865, align 1
  %3867 = shl i32 %3862, 13
  %3868 = xor i32 %3867, %3862
  %3869 = lshr i32 %3868, 17
  %3870 = xor i32 %3869, %3868
  %3871 = shl i32 %3870, 5
  %3872 = xor i32 %3871, %3870
  store i32 %3872, ptr @vm_code_state, align 4
  %3873 = trunc i32 %3872 to i8
  %3874 = xor i8 %3866, %3873
  %3875 = zext i8 %3874 to i64
  %3876 = shl nsw i64 %3860, 3
  %3877 = shl i64 %3875, %3876
  %3878 = or i64 %3877, %3861
  %3879 = add nuw nsw i64 %3860, 1
  %3880 = icmp eq i64 %3879, %3857
  br i1 %3880, label %3881, label %3858, !llvm.loop !15

3881:                                             ; preds = %3858, %3854
  %3882 = phi i32 [ %3851, %3854 ], [ %3872, %3858 ]
  %3883 = phi i32 [ %3842, %3854 ], [ %3864, %3858 ]
  %3884 = phi i64 [ 0, %3854 ], [ %3878, %3858 ]
  %3885 = icmp eq i8 %3833, %3840
  br i1 %3885, label %3982, label %3886

3886:                                             ; preds = %3881
  %3887 = load i64, ptr @data_seg_addr, align 8
  %3888 = inttoptr i64 %3887 to ptr
  %3889 = zext i8 %3841 to i64
  %3890 = and i64 %3889, 3
  %3891 = icmp ult i8 %3841, 4
  br i1 %3891, label %3961, label %3892

3892:                                             ; preds = %3886
  %3893 = and i64 %3889, 252
  %3894 = getelementptr i8, ptr %3888, i64 1
  %3895 = getelementptr i8, ptr %3888, i64 2
  %3896 = getelementptr i8, ptr %3888, i64 3
  br label %3897

3897:                                             ; preds = %3897, %3892
  %3898 = phi i64 [ 0, %3892 ], [ %3930, %3897 ]
  %3899 = phi i64 [ 0, %3892 ], [ %3929, %3897 ]
  %3900 = phi i64 [ %3884, %3892 ], [ %3922, %3897 ]
  %3901 = phi i64 [ 0, %3892 ], [ %3931, %3897 ]
  %3902 = getelementptr inbounds nuw i8, ptr %3888, i64 %3900
  %3903 = load i8, ptr %3902, align 1
  %3904 = zext i8 %3903 to i64
  %3905 = shl nsw i64 %3898, 3
  %3906 = shl i64 %3904, %3905
  %3907 = or i64 %3906, %3899
  %3908 = getelementptr i8, ptr %3894, i64 %3900
  %3909 = load i8, ptr %3908, align 1
  %3910 = zext i8 %3909 to i64
  %3911 = shl i64 %3898, 3
  %3912 = or disjoint i64 %3911, 8
  %3913 = shl i64 %3910, %3912
  %3914 = or i64 %3913, %3907
  %3915 = getelementptr i8, ptr %3895, i64 %3900
  %3916 = load i8, ptr %3915, align 1
  %3917 = zext i8 %3916 to i64
  %3918 = shl i64 %3898, 3
  %3919 = or disjoint i64 %3918, 16
  %3920 = shl i64 %3917, %3919
  %3921 = or i64 %3920, %3914
  %3922 = add i64 %3900, 4
  %3923 = getelementptr i8, ptr %3896, i64 %3900
  %3924 = load i8, ptr %3923, align 1
  %3925 = zext i8 %3924 to i64
  %3926 = shl i64 %3898, 3
  %3927 = or disjoint i64 %3926, 24
  %3928 = shl i64 %3925, %3927
  %3929 = or i64 %3928, %3921
  %3930 = add nuw nsw i64 %3898, 4
  %3931 = add i64 %3901, 4
  %3932 = icmp eq i64 %3931, %3893
  br i1 %3932, label %3961, label %3897, !llvm.loop !17

3933:                                             ; preds = %3826
  %3934 = icmp eq i8 %3833, %3840
  br i1 %3934, label %3982, label %3935

3935:                                             ; preds = %3933
  %3936 = sext i32 %3842 to i64
  %3937 = zext i8 %3841 to i64
  br label %3938

3938:                                             ; preds = %3935, %3938
  %3939 = phi i64 [ %3936, %3935 ], [ %3943, %3938 ]
  %3940 = phi i64 [ 0, %3935 ], [ %3959, %3938 ]
  %3941 = phi i64 [ 0, %3935 ], [ %3958, %3938 ]
  %3942 = phi i32 [ %3851, %3935 ], [ %3952, %3938 ]
  %3943 = add nsw i64 %3939, 1
  %3944 = trunc nsw i64 %3943 to i32
  store i32 %3944, ptr @ip, align 4
  %3945 = getelementptr inbounds i8, ptr %69, i64 %3939
  %3946 = load i8, ptr %3945, align 1
  %3947 = shl i32 %3942, 13
  %3948 = xor i32 %3947, %3942
  %3949 = lshr i32 %3948, 17
  %3950 = xor i32 %3949, %3948
  %3951 = shl i32 %3950, 5
  %3952 = xor i32 %3951, %3950
  store i32 %3952, ptr @vm_code_state, align 4
  %3953 = trunc i32 %3952 to i8
  %3954 = xor i8 %3946, %3953
  %3955 = zext i8 %3954 to i64
  %3956 = shl nsw i64 %3940, 3
  %3957 = shl i64 %3955, %3956
  %3958 = or i64 %3957, %3941
  %3959 = add nuw nsw i64 %3940, 1
  %3960 = icmp eq i64 %3959, %3937
  br i1 %3960, label %3982, label %3938, !llvm.loop !15

3961:                                             ; preds = %3897, %3886
  %3962 = phi i64 [ poison, %3886 ], [ %3929, %3897 ]
  %3963 = phi i64 [ 0, %3886 ], [ %3930, %3897 ]
  %3964 = phi i64 [ 0, %3886 ], [ %3929, %3897 ]
  %3965 = phi i64 [ %3884, %3886 ], [ %3922, %3897 ]
  %3966 = icmp eq i64 %3890, 0
  br i1 %3966, label %3982, label %3967

3967:                                             ; preds = %3961, %3967
  %3968 = phi i64 [ %3979, %3967 ], [ %3963, %3961 ]
  %3969 = phi i64 [ %3978, %3967 ], [ %3964, %3961 ]
  %3970 = phi i64 [ %3972, %3967 ], [ %3965, %3961 ]
  %3971 = phi i64 [ %3980, %3967 ], [ 0, %3961 ]
  %3972 = add i64 %3970, 1
  %3973 = getelementptr inbounds nuw i8, ptr %3888, i64 %3970
  %3974 = load i8, ptr %3973, align 1
  %3975 = zext i8 %3974 to i64
  %3976 = shl nsw i64 %3968, 3
  %3977 = shl i64 %3975, %3976
  %3978 = or i64 %3977, %3969
  %3979 = add nuw nsw i64 %3968, 1
  %3980 = add i64 %3971, 1
  %3981 = icmp eq i64 %3980, %3890
  br i1 %3981, label %3982, label %3967, !llvm.loop !41

3982:                                             ; preds = %3938, %3961, %3967, %3933, %3881
  %3983 = phi i32 [ %3882, %3881 ], [ %3851, %3933 ], [ %3882, %3967 ], [ %3882, %3961 ], [ %3952, %3938 ]
  %3984 = phi i32 [ %3883, %3881 ], [ %3842, %3933 ], [ %3883, %3967 ], [ %3883, %3961 ], [ %3944, %3938 ]
  %3985 = phi i64 [ 0, %3881 ], [ 0, %3933 ], [ %3962, %3961 ], [ %3978, %3967 ], [ %3958, %3938 ]
  %3986 = add nsw i32 %3984, 1
  store i32 %3986, ptr @ip, align 4
  %3987 = sext i32 %3984 to i64
  %3988 = getelementptr inbounds i8, ptr %69, i64 %3987
  %3989 = load i8, ptr %3988, align 1
  %3990 = shl i32 %3983, 13
  %3991 = xor i32 %3990, %3983
  %3992 = lshr i32 %3991, 17
  %3993 = xor i32 %3992, %3991
  %3994 = shl i32 %3993, 5
  %3995 = xor i32 %3994, %3993
  store i32 %3995, ptr @vm_code_state, align 4
  %3996 = trunc i32 %3995 to i8
  %3997 = xor i8 %3989, %3996
  %3998 = add nsw i32 %3984, 2
  store i32 %3998, ptr @ip, align 4
  %3999 = sext i32 %3986 to i64
  %4000 = getelementptr inbounds i8, ptr %69, i64 %3999
  %4001 = load i8, ptr %4000, align 1
  %4002 = shl i32 %3995, 13
  %4003 = xor i32 %4002, %3995
  %4004 = lshr i32 %4003, 17
  %4005 = xor i32 %4004, %4003
  %4006 = shl i32 %4005, 5
  %4007 = xor i32 %4006, %4005
  store i32 %4007, ptr @vm_code_state, align 4
  %4008 = trunc i32 %4007 to i8
  %4009 = icmp eq i8 %4001, %4008
  br i1 %4009, label %4010, label %4089

4010:                                             ; preds = %3982
  br i1 %3643, label %4011, label %4037

4011:                                             ; preds = %4010
  %4012 = sext i32 %3998 to i64
  %4013 = zext nneg i32 %3642 to i64
  br label %4014

4014:                                             ; preds = %4011, %4014
  %4015 = phi i64 [ %4012, %4011 ], [ %4019, %4014 ]
  %4016 = phi i64 [ 0, %4011 ], [ %4035, %4014 ]
  %4017 = phi i64 [ 0, %4011 ], [ %4034, %4014 ]
  %4018 = phi i32 [ %4007, %4011 ], [ %4028, %4014 ]
  %4019 = add nsw i64 %4015, 1
  %4020 = trunc nsw i64 %4019 to i32
  store i32 %4020, ptr @ip, align 4
  %4021 = getelementptr inbounds i8, ptr %69, i64 %4015
  %4022 = load i8, ptr %4021, align 1
  %4023 = shl i32 %4018, 13
  %4024 = xor i32 %4023, %4018
  %4025 = lshr i32 %4024, 17
  %4026 = xor i32 %4025, %4024
  %4027 = shl i32 %4026, 5
  %4028 = xor i32 %4027, %4026
  store i32 %4028, ptr @vm_code_state, align 4
  %4029 = trunc i32 %4028 to i8
  %4030 = xor i8 %4022, %4029
  %4031 = zext i8 %4030 to i64
  %4032 = shl nsw i64 %4016, 3
  %4033 = shl i64 %4031, %4032
  %4034 = or i64 %4033, %4017
  %4035 = add nuw nsw i64 %4016, 1
  %4036 = icmp eq i64 %4035, %4013
  br i1 %4036, label %4037, label %4014, !llvm.loop !15

4037:                                             ; preds = %4014, %4010
  %4038 = phi i32 [ %4007, %4010 ], [ %4028, %4014 ]
  %4039 = phi i32 [ %3998, %4010 ], [ %4020, %4014 ]
  %4040 = phi i64 [ 0, %4010 ], [ %4034, %4014 ]
  %4041 = icmp eq i8 %3989, %3996
  br i1 %4041, label %4138, label %4042

4042:                                             ; preds = %4037
  %4043 = load i64, ptr @data_seg_addr, align 8
  %4044 = inttoptr i64 %4043 to ptr
  %4045 = zext i8 %3997 to i64
  %4046 = and i64 %4045, 3
  %4047 = icmp ult i8 %3997, 4
  br i1 %4047, label %4117, label %4048

4048:                                             ; preds = %4042
  %4049 = and i64 %4045, 252
  %4050 = getelementptr i8, ptr %4044, i64 1
  %4051 = getelementptr i8, ptr %4044, i64 2
  %4052 = getelementptr i8, ptr %4044, i64 3
  br label %4053

4053:                                             ; preds = %4053, %4048
  %4054 = phi i64 [ 0, %4048 ], [ %4086, %4053 ]
  %4055 = phi i64 [ 0, %4048 ], [ %4085, %4053 ]
  %4056 = phi i64 [ %4040, %4048 ], [ %4078, %4053 ]
  %4057 = phi i64 [ 0, %4048 ], [ %4087, %4053 ]
  %4058 = getelementptr inbounds nuw i8, ptr %4044, i64 %4056
  %4059 = load i8, ptr %4058, align 1
  %4060 = zext i8 %4059 to i64
  %4061 = shl nsw i64 %4054, 3
  %4062 = shl i64 %4060, %4061
  %4063 = or i64 %4062, %4055
  %4064 = getelementptr i8, ptr %4050, i64 %4056
  %4065 = load i8, ptr %4064, align 1
  %4066 = zext i8 %4065 to i64
  %4067 = shl i64 %4054, 3
  %4068 = or disjoint i64 %4067, 8
  %4069 = shl i64 %4066, %4068
  %4070 = or i64 %4069, %4063
  %4071 = getelementptr i8, ptr %4051, i64 %4056
  %4072 = load i8, ptr %4071, align 1
  %4073 = zext i8 %4072 to i64
  %4074 = shl i64 %4054, 3
  %4075 = or disjoint i64 %4074, 16
  %4076 = shl i64 %4073, %4075
  %4077 = or i64 %4076, %4070
  %4078 = add i64 %4056, 4
  %4079 = getelementptr i8, ptr %4052, i64 %4056
  %4080 = load i8, ptr %4079, align 1
  %4081 = zext i8 %4080 to i64
  %4082 = shl i64 %4054, 3
  %4083 = or disjoint i64 %4082, 24
  %4084 = shl i64 %4081, %4083
  %4085 = or i64 %4084, %4077
  %4086 = add nuw nsw i64 %4054, 4
  %4087 = add i64 %4057, 4
  %4088 = icmp eq i64 %4087, %4049
  br i1 %4088, label %4117, label %4053, !llvm.loop !17

4089:                                             ; preds = %3982
  %4090 = icmp eq i8 %3989, %3996
  br i1 %4090, label %4138, label %4091

4091:                                             ; preds = %4089
  %4092 = sext i32 %3998 to i64
  %4093 = zext i8 %3997 to i64
  br label %4094

4094:                                             ; preds = %4091, %4094
  %4095 = phi i64 [ %4092, %4091 ], [ %4099, %4094 ]
  %4096 = phi i64 [ 0, %4091 ], [ %4115, %4094 ]
  %4097 = phi i64 [ 0, %4091 ], [ %4114, %4094 ]
  %4098 = phi i32 [ %4007, %4091 ], [ %4108, %4094 ]
  %4099 = add nsw i64 %4095, 1
  %4100 = trunc nsw i64 %4099 to i32
  store i32 %4100, ptr @ip, align 4
  %4101 = getelementptr inbounds i8, ptr %69, i64 %4095
  %4102 = load i8, ptr %4101, align 1
  %4103 = shl i32 %4098, 13
  %4104 = xor i32 %4103, %4098
  %4105 = lshr i32 %4104, 17
  %4106 = xor i32 %4105, %4104
  %4107 = shl i32 %4106, 5
  %4108 = xor i32 %4107, %4106
  store i32 %4108, ptr @vm_code_state, align 4
  %4109 = trunc i32 %4108 to i8
  %4110 = xor i8 %4102, %4109
  %4111 = zext i8 %4110 to i64
  %4112 = shl nsw i64 %4096, 3
  %4113 = shl i64 %4111, %4112
  %4114 = or i64 %4113, %4097
  %4115 = add nuw nsw i64 %4096, 1
  %4116 = icmp eq i64 %4115, %4093
  br i1 %4116, label %4138, label %4094, !llvm.loop !15

4117:                                             ; preds = %4053, %4042
  %4118 = phi i64 [ poison, %4042 ], [ %4085, %4053 ]
  %4119 = phi i64 [ 0, %4042 ], [ %4086, %4053 ]
  %4120 = phi i64 [ 0, %4042 ], [ %4085, %4053 ]
  %4121 = phi i64 [ %4040, %4042 ], [ %4078, %4053 ]
  %4122 = icmp eq i64 %4046, 0
  br i1 %4122, label %4138, label %4123

4123:                                             ; preds = %4117, %4123
  %4124 = phi i64 [ %4135, %4123 ], [ %4119, %4117 ]
  %4125 = phi i64 [ %4134, %4123 ], [ %4120, %4117 ]
  %4126 = phi i64 [ %4128, %4123 ], [ %4121, %4117 ]
  %4127 = phi i64 [ %4136, %4123 ], [ 0, %4117 ]
  %4128 = add i64 %4126, 1
  %4129 = getelementptr inbounds nuw i8, ptr %4044, i64 %4126
  %4130 = load i8, ptr %4129, align 1
  %4131 = zext i8 %4130 to i64
  %4132 = shl nsw i64 %4124, 3
  %4133 = shl i64 %4131, %4132
  %4134 = or i64 %4133, %4125
  %4135 = add nuw nsw i64 %4124, 1
  %4136 = add i64 %4127, 1
  %4137 = icmp eq i64 %4136, %4046
  br i1 %4137, label %4138, label %4123, !llvm.loop !42

4138:                                             ; preds = %4094, %4117, %4123, %4089, %4037
  %4139 = phi i32 [ %4038, %4037 ], [ %4007, %4089 ], [ %4038, %4123 ], [ %4038, %4117 ], [ %4108, %4094 ]
  %4140 = phi i32 [ %4039, %4037 ], [ %3998, %4089 ], [ %4039, %4123 ], [ %4039, %4117 ], [ %4100, %4094 ]
  %4141 = phi i64 [ 0, %4037 ], [ 0, %4089 ], [ %4118, %4117 ], [ %4134, %4123 ], [ %4114, %4094 ]
  %4142 = sext i32 %4140 to i64
  %4143 = add nsw i64 %4142, 1
  %4144 = trunc nsw i64 %4143 to i32
  store i32 %4144, ptr @ip, align 4
  %4145 = getelementptr inbounds i8, ptr %69, i64 %4142
  %4146 = load i8, ptr %4145, align 1
  %4147 = shl i32 %4139, 13
  %4148 = xor i32 %4147, %4139
  %4149 = lshr i32 %4148, 17
  %4150 = xor i32 %4149, %4148
  %4151 = shl i32 %4150, 5
  %4152 = xor i32 %4151, %4150
  store i32 %4152, ptr @vm_code_state, align 4
  %4153 = trunc i32 %4152 to i8
  %4154 = xor i8 %4146, %4153
  %4155 = zext i8 %4154 to i32
  %4156 = add nsw i64 %4142, 2
  %4157 = trunc nsw i64 %4156 to i32
  store i32 %4157, ptr @ip, align 4
  %4158 = getelementptr inbounds i8, ptr %69, i64 %4143
  %4159 = load i8, ptr %4158, align 1
  %4160 = shl i32 %4152, 13
  %4161 = xor i32 %4160, %4152
  %4162 = lshr i32 %4161, 17
  %4163 = xor i32 %4162, %4161
  %4164 = shl i32 %4163, 5
  %4165 = xor i32 %4164, %4163
  store i32 %4165, ptr @vm_code_state, align 4
  %4166 = trunc i32 %4165 to i8
  %4167 = xor i8 %4159, %4166
  %4168 = zext i8 %4167 to i32
  %4169 = shl nuw nsw i32 %4168, 8
  %4170 = or disjoint i32 %4169, %4155
  %4171 = add nsw i64 %4142, 3
  %4172 = trunc nsw i64 %4171 to i32
  store i32 %4172, ptr @ip, align 4
  %4173 = getelementptr inbounds i8, ptr %69, i64 %4156
  %4174 = load i8, ptr %4173, align 1
  %4175 = shl i32 %4165, 13
  %4176 = xor i32 %4175, %4165
  %4177 = lshr i32 %4176, 17
  %4178 = xor i32 %4177, %4176
  %4179 = shl i32 %4178, 5
  %4180 = xor i32 %4179, %4178
  store i32 %4180, ptr @vm_code_state, align 4
  %4181 = trunc i32 %4180 to i8
  %4182 = xor i8 %4174, %4181
  %4183 = zext i8 %4182 to i32
  %4184 = shl nuw nsw i32 %4183, 16
  %4185 = or disjoint i32 %4184, %4170
  %4186 = add i32 %4140, 4
  store i32 %4186, ptr @ip, align 4
  %4187 = getelementptr inbounds i8, ptr %69, i64 %4171
  %4188 = load i8, ptr %4187, align 1
  %4189 = shl i32 %4180, 13
  %4190 = xor i32 %4189, %4180
  %4191 = lshr i32 %4190, 17
  %4192 = xor i32 %4191, %4190
  %4193 = shl i32 %4192, 5
  %4194 = xor i32 %4193, %4192
  store i32 %4194, ptr @vm_code_state, align 4
  %4195 = trunc i32 %4194 to i8
  %4196 = xor i8 %4188, %4195
  %4197 = zext i8 %4196 to i32
  %4198 = shl nuw i32 %4197, 24
  %4199 = or disjoint i32 %4198, %4185
  %4200 = load i64, ptr @data_seg_addr, align 8
  %4201 = add i64 %4200, %3673
  %4202 = icmp eq i8 %3626, %3633
  br i1 %4202, label %4301, label %4203

4203:                                             ; preds = %4138
  %4204 = inttoptr i64 %3829 to ptr
  %4205 = inttoptr i64 %4201 to ptr
  %4206 = zext i8 %3634 to i64
  %4207 = icmp ult i8 %3634, 4
  %4208 = sub i64 %4201, %3829
  %4209 = icmp ult i64 %4208, 32
  %4210 = select i1 %4207, i1 true, i1 %4209
  br i1 %4210, label %4211, label %4228

4211:                                             ; preds = %4286, %4299, %4203
  %4212 = phi i64 [ 0, %4203 ], [ %4231, %4286 ], [ %4291, %4299 ]
  %4213 = and i64 %4206, 3
  %4214 = icmp eq i64 %4213, 0
  br i1 %4214, label %4224, label %4215

4215:                                             ; preds = %4211, %4215
  %4216 = phi i64 [ %4221, %4215 ], [ %4212, %4211 ]
  %4217 = phi i64 [ %4222, %4215 ], [ 0, %4211 ]
  %4218 = getelementptr inbounds nuw i8, ptr %4204, i64 %4216
  %4219 = load i8, ptr %4218, align 1
  %4220 = getelementptr inbounds nuw i8, ptr %4205, i64 %4216
  store i8 %4219, ptr %4220, align 1
  %4221 = add nuw nsw i64 %4216, 1
  %4222 = add i64 %4217, 1
  %4223 = icmp eq i64 %4222, %4213
  br i1 %4223, label %4224, label %4215, !llvm.loop !43

4224:                                             ; preds = %4215, %4211
  %4225 = phi i64 [ %4212, %4211 ], [ %4221, %4215 ]
  %4226 = sub nsw i64 %4212, %4206
  %4227 = icmp ugt i64 %4226, -4
  br i1 %4227, label %4301, label %4312

4228:                                             ; preds = %4203
  %4229 = icmp ult i8 %3634, 32
  br i1 %4229, label %4289, label %4230

4230:                                             ; preds = %4228
  %4231 = and i64 %4206, 224
  %4232 = getelementptr inbounds nuw i8, ptr %4204, i64 16
  %4233 = load <16 x i8>, ptr %4204, align 1
  %4234 = load <16 x i8>, ptr %4232, align 1
  %4235 = getelementptr inbounds nuw i8, ptr %4205, i64 16
  store <16 x i8> %4233, ptr %4205, align 1
  store <16 x i8> %4234, ptr %4235, align 1
  %4236 = icmp eq i64 %4231, 32
  br i1 %4236, label %4284, label %4237, !llvm.loop !44

4237:                                             ; preds = %4230
  %4238 = getelementptr inbounds nuw i8, ptr %4204, i64 32
  %4239 = getelementptr inbounds nuw i8, ptr %4204, i64 48
  %4240 = load <16 x i8>, ptr %4238, align 1
  %4241 = load <16 x i8>, ptr %4239, align 1
  %4242 = getelementptr inbounds nuw i8, ptr %4205, i64 32
  %4243 = getelementptr inbounds nuw i8, ptr %4205, i64 48
  store <16 x i8> %4240, ptr %4242, align 1
  store <16 x i8> %4241, ptr %4243, align 1
  %4244 = icmp eq i64 %4231, 64
  br i1 %4244, label %4284, label %4245, !llvm.loop !44

4245:                                             ; preds = %4237
  %4246 = getelementptr inbounds nuw i8, ptr %4204, i64 64
  %4247 = getelementptr inbounds nuw i8, ptr %4204, i64 80
  %4248 = load <16 x i8>, ptr %4246, align 1
  %4249 = load <16 x i8>, ptr %4247, align 1
  %4250 = getelementptr inbounds nuw i8, ptr %4205, i64 64
  %4251 = getelementptr inbounds nuw i8, ptr %4205, i64 80
  store <16 x i8> %4248, ptr %4250, align 1
  store <16 x i8> %4249, ptr %4251, align 1
  %4252 = icmp eq i64 %4231, 96
  br i1 %4252, label %4284, label %4253, !llvm.loop !44

4253:                                             ; preds = %4245
  %4254 = getelementptr inbounds nuw i8, ptr %4204, i64 96
  %4255 = getelementptr inbounds nuw i8, ptr %4204, i64 112
  %4256 = load <16 x i8>, ptr %4254, align 1
  %4257 = load <16 x i8>, ptr %4255, align 1
  %4258 = getelementptr inbounds nuw i8, ptr %4205, i64 96
  %4259 = getelementptr inbounds nuw i8, ptr %4205, i64 112
  store <16 x i8> %4256, ptr %4258, align 1
  store <16 x i8> %4257, ptr %4259, align 1
  %4260 = icmp eq i64 %4231, 128
  br i1 %4260, label %4284, label %4261, !llvm.loop !44

4261:                                             ; preds = %4253
  %4262 = getelementptr inbounds nuw i8, ptr %4204, i64 128
  %4263 = getelementptr inbounds nuw i8, ptr %4204, i64 144
  %4264 = load <16 x i8>, ptr %4262, align 1
  %4265 = load <16 x i8>, ptr %4263, align 1
  %4266 = getelementptr inbounds nuw i8, ptr %4205, i64 128
  %4267 = getelementptr inbounds nuw i8, ptr %4205, i64 144
  store <16 x i8> %4264, ptr %4266, align 1
  store <16 x i8> %4265, ptr %4267, align 1
  %4268 = icmp eq i64 %4231, 160
  br i1 %4268, label %4284, label %4269, !llvm.loop !44

4269:                                             ; preds = %4261
  %4270 = getelementptr inbounds nuw i8, ptr %4204, i64 160
  %4271 = getelementptr inbounds nuw i8, ptr %4204, i64 176
  %4272 = load <16 x i8>, ptr %4270, align 1
  %4273 = load <16 x i8>, ptr %4271, align 1
  %4274 = getelementptr inbounds nuw i8, ptr %4205, i64 160
  %4275 = getelementptr inbounds nuw i8, ptr %4205, i64 176
  store <16 x i8> %4272, ptr %4274, align 1
  store <16 x i8> %4273, ptr %4275, align 1
  %4276 = icmp eq i64 %4231, 192
  br i1 %4276, label %4284, label %4277, !llvm.loop !44

4277:                                             ; preds = %4269
  %4278 = getelementptr inbounds nuw i8, ptr %4204, i64 192
  %4279 = getelementptr inbounds nuw i8, ptr %4204, i64 208
  %4280 = load <16 x i8>, ptr %4278, align 1
  %4281 = load <16 x i8>, ptr %4279, align 1
  %4282 = getelementptr inbounds nuw i8, ptr %4205, i64 192
  %4283 = getelementptr inbounds nuw i8, ptr %4205, i64 208
  store <16 x i8> %4280, ptr %4282, align 1
  store <16 x i8> %4281, ptr %4283, align 1
  br label %4284

4284:                                             ; preds = %4277, %4269, %4261, %4253, %4245, %4237, %4230
  %4285 = icmp eq i64 %4231, %4206
  br i1 %4285, label %4301, label %4286

4286:                                             ; preds = %4284
  %4287 = and i64 %4206, 28
  %4288 = icmp eq i64 %4287, 0
  br i1 %4288, label %4211, label %4289

4289:                                             ; preds = %4286, %4228
  %4290 = phi i64 [ %4231, %4286 ], [ 0, %4228 ]
  %4291 = and i64 %4206, 252
  br label %4292

4292:                                             ; preds = %4292, %4289
  %4293 = phi i64 [ %4290, %4289 ], [ %4297, %4292 ]
  %4294 = getelementptr inbounds nuw i8, ptr %4204, i64 %4293
  %4295 = load <4 x i8>, ptr %4294, align 1
  %4296 = getelementptr inbounds nuw i8, ptr %4205, i64 %4293
  store <4 x i8> %4295, ptr %4296, align 1
  %4297 = add nuw i64 %4293, 4
  %4298 = icmp eq i64 %4297, %4291
  br i1 %4298, label %4299, label %4292, !llvm.loop !45

4299:                                             ; preds = %4292
  %4300 = icmp eq i64 %4291, %4206
  br i1 %4300, label %4301, label %4211

4301:                                             ; preds = %4224, %4312, %4284, %4299, %4138
  %4302 = icmp eq i32 %4199, 0
  br i1 %4302, label %8784, label %4303

4303:                                             ; preds = %4301
  %4304 = tail call i32 @llvm.umin.i32(i32 %4199, i32 8)
  %4305 = inttoptr i64 %4201 to ptr
  %4306 = getelementptr i8, ptr %4305, i64 %4141
  %4307 = zext nneg i32 %4304 to i64
  %4308 = and i64 %4307, 3
  %4309 = icmp ult i32 %4199, 4
  br i1 %4309, label %8693, label %4310

4310:                                             ; preds = %4303
  %4311 = and i64 %4307, 12
  br label %4331

4312:                                             ; preds = %4224, %4312
  %4313 = phi i64 [ %4329, %4312 ], [ %4225, %4224 ]
  %4314 = getelementptr inbounds nuw i8, ptr %4204, i64 %4313
  %4315 = load i8, ptr %4314, align 1
  %4316 = getelementptr inbounds nuw i8, ptr %4205, i64 %4313
  store i8 %4315, ptr %4316, align 1
  %4317 = add nuw nsw i64 %4313, 1
  %4318 = getelementptr inbounds nuw i8, ptr %4204, i64 %4317
  %4319 = load i8, ptr %4318, align 1
  %4320 = getelementptr inbounds nuw i8, ptr %4205, i64 %4317
  store i8 %4319, ptr %4320, align 1
  %4321 = add nuw nsw i64 %4313, 2
  %4322 = getelementptr inbounds nuw i8, ptr %4204, i64 %4321
  %4323 = load i8, ptr %4322, align 1
  %4324 = getelementptr inbounds nuw i8, ptr %4205, i64 %4321
  store i8 %4323, ptr %4324, align 1
  %4325 = add nuw nsw i64 %4313, 3
  %4326 = getelementptr inbounds nuw i8, ptr %4204, i64 %4325
  %4327 = load i8, ptr %4326, align 1
  %4328 = getelementptr inbounds nuw i8, ptr %4205, i64 %4325
  store i8 %4327, ptr %4328, align 1
  %4329 = add nuw nsw i64 %4313, 4
  %4330 = icmp eq i64 %4329, %4206
  br i1 %4330, label %4301, label %4312, !llvm.loop !46

4331:                                             ; preds = %4331, %4310
  %4332 = phi i64 [ 0, %4310 ], [ %4353, %4331 ]
  %4333 = phi i64 [ 0, %4310 ], [ %4354, %4331 ]
  %4334 = shl nuw nsw i64 %4332, 3
  %4335 = lshr i64 %3985, %4334
  %4336 = trunc i64 %4335 to i8
  %4337 = getelementptr i8, ptr %4306, i64 %4332
  store i8 %4336, ptr %4337, align 1
  %4338 = or disjoint i64 %4332, 1
  %4339 = shl nuw nsw i64 %4338, 3
  %4340 = lshr i64 %3985, %4339
  %4341 = trunc i64 %4340 to i8
  %4342 = getelementptr i8, ptr %4306, i64 %4338
  store i8 %4341, ptr %4342, align 1
  %4343 = or disjoint i64 %4332, 2
  %4344 = shl nuw nsw i64 %4343, 3
  %4345 = lshr i64 %3985, %4344
  %4346 = trunc i64 %4345 to i8
  %4347 = getelementptr i8, ptr %4306, i64 %4343
  store i8 %4346, ptr %4347, align 1
  %4348 = or disjoint i64 %4332, 3
  %4349 = shl nuw nsw i64 %4348, 3
  %4350 = lshr i64 %3985, %4349
  %4351 = trunc i64 %4350 to i8
  %4352 = getelementptr i8, ptr %4306, i64 %4348
  store i8 %4351, ptr %4352, align 1
  %4353 = add nuw nsw i64 %4332, 4
  %4354 = add i64 %4333, 4
  %4355 = icmp eq i64 %4354, %4311
  br i1 %4355, label %8693, label %4331, !llvm.loop !47

4356:                                             ; preds = %158
  %4357 = add nsw i32 %72, 2
  store i32 %4357, ptr @ip, align 4
  %4358 = sext i32 %73 to i64
  %4359 = getelementptr inbounds i8, ptr %69, i64 %4358
  %4360 = load i8, ptr %4359, align 1
  %4361 = shl i32 %81, 13
  %4362 = xor i32 %4361, %81
  %4363 = lshr i32 %4362, 17
  %4364 = xor i32 %4363, %4362
  %4365 = shl i32 %4364, 5
  %4366 = xor i32 %4365, %4364
  %4367 = trunc i32 %4366 to i8
  %4368 = xor i8 %4360, %4367
  %4369 = add nsw i32 %72, 3
  %4370 = shl i32 %4366, 13
  %4371 = xor i32 %4370, %4366
  %4372 = lshr i32 %4371, 17
  %4373 = xor i32 %4372, %4371
  %4374 = shl i32 %4373, 5
  %4375 = xor i32 %4374, %4373
  store i32 %4375, ptr @vm_code_state, align 4
  %4376 = load i32, ptr @pointer_size, align 4
  %4377 = icmp sgt i32 %4376, 0
  br i1 %4377, label %4378, label %4404

4378:                                             ; preds = %4356
  %4379 = sext i32 %4369 to i64
  %4380 = zext nneg i32 %4376 to i64
  br label %4381

4381:                                             ; preds = %4378, %4381
  %4382 = phi i64 [ %4379, %4378 ], [ %4386, %4381 ]
  %4383 = phi i64 [ 0, %4378 ], [ %4402, %4381 ]
  %4384 = phi i64 [ 0, %4378 ], [ %4401, %4381 ]
  %4385 = phi i32 [ %4375, %4378 ], [ %4395, %4381 ]
  %4386 = add nsw i64 %4382, 1
  %4387 = trunc nsw i64 %4386 to i32
  store i32 %4387, ptr @ip, align 4
  %4388 = getelementptr inbounds i8, ptr %69, i64 %4382
  %4389 = load i8, ptr %4388, align 1
  %4390 = shl i32 %4385, 13
  %4391 = xor i32 %4390, %4385
  %4392 = lshr i32 %4391, 17
  %4393 = xor i32 %4392, %4391
  %4394 = shl i32 %4393, 5
  %4395 = xor i32 %4394, %4393
  store i32 %4395, ptr @vm_code_state, align 4
  %4396 = trunc i32 %4395 to i8
  %4397 = xor i8 %4389, %4396
  %4398 = zext i8 %4397 to i64
  %4399 = shl nsw i64 %4383, 3
  %4400 = shl i64 %4398, %4399
  %4401 = or i64 %4400, %4384
  %4402 = add nuw nsw i64 %4383, 1
  %4403 = icmp eq i64 %4402, %4380
  br i1 %4403, label %4404, label %4381, !llvm.loop !15

4404:                                             ; preds = %4381, %4356
  %4405 = phi i32 [ %4375, %4356 ], [ %4395, %4381 ]
  %4406 = phi i32 [ %4369, %4356 ], [ %4387, %4381 ]
  %4407 = phi i64 [ 0, %4356 ], [ %4401, %4381 ]
  %4408 = add nsw i32 %4406, 1
  store i32 %4408, ptr @ip, align 4
  %4409 = sext i32 %4406 to i64
  %4410 = getelementptr inbounds i8, ptr %69, i64 %4409
  %4411 = load i8, ptr %4410, align 1
  %4412 = shl i32 %4405, 13
  %4413 = xor i32 %4412, %4405
  %4414 = lshr i32 %4413, 17
  %4415 = xor i32 %4414, %4413
  %4416 = shl i32 %4415, 5
  %4417 = xor i32 %4416, %4415
  store i32 %4417, ptr @vm_code_state, align 4
  %4418 = trunc i32 %4417 to i8
  %4419 = xor i8 %4411, %4418
  %4420 = add nsw i32 %4406, 2
  store i32 %4420, ptr @ip, align 4
  %4421 = sext i32 %4408 to i64
  %4422 = getelementptr inbounds i8, ptr %69, i64 %4421
  %4423 = load i8, ptr %4422, align 1
  %4424 = shl i32 %4417, 13
  %4425 = xor i32 %4424, %4417
  %4426 = lshr i32 %4425, 17
  %4427 = xor i32 %4426, %4425
  %4428 = shl i32 %4427, 5
  %4429 = xor i32 %4428, %4427
  store i32 %4429, ptr @vm_code_state, align 4
  %4430 = trunc i32 %4429 to i8
  %4431 = icmp eq i8 %4423, %4430
  br i1 %4431, label %4432, label %4511

4432:                                             ; preds = %4404
  br i1 %4377, label %4433, label %4459

4433:                                             ; preds = %4432
  %4434 = sext i32 %4420 to i64
  %4435 = zext nneg i32 %4376 to i64
  br label %4436

4436:                                             ; preds = %4433, %4436
  %4437 = phi i64 [ %4434, %4433 ], [ %4441, %4436 ]
  %4438 = phi i64 [ 0, %4433 ], [ %4457, %4436 ]
  %4439 = phi i64 [ 0, %4433 ], [ %4456, %4436 ]
  %4440 = phi i32 [ %4429, %4433 ], [ %4450, %4436 ]
  %4441 = add nsw i64 %4437, 1
  %4442 = trunc nsw i64 %4441 to i32
  store i32 %4442, ptr @ip, align 4
  %4443 = getelementptr inbounds i8, ptr %69, i64 %4437
  %4444 = load i8, ptr %4443, align 1
  %4445 = shl i32 %4440, 13
  %4446 = xor i32 %4445, %4440
  %4447 = lshr i32 %4446, 17
  %4448 = xor i32 %4447, %4446
  %4449 = shl i32 %4448, 5
  %4450 = xor i32 %4449, %4448
  store i32 %4450, ptr @vm_code_state, align 4
  %4451 = trunc i32 %4450 to i8
  %4452 = xor i8 %4444, %4451
  %4453 = zext i8 %4452 to i64
  %4454 = shl nsw i64 %4438, 3
  %4455 = shl i64 %4453, %4454
  %4456 = or i64 %4455, %4439
  %4457 = add nuw nsw i64 %4438, 1
  %4458 = icmp eq i64 %4457, %4435
  br i1 %4458, label %4459, label %4436, !llvm.loop !15

4459:                                             ; preds = %4436, %4432
  %4460 = phi i32 [ %4429, %4432 ], [ %4450, %4436 ]
  %4461 = phi i32 [ %4420, %4432 ], [ %4442, %4436 ]
  %4462 = phi i64 [ 0, %4432 ], [ %4456, %4436 ]
  %4463 = icmp eq i8 %4411, %4418
  br i1 %4463, label %4560, label %4464

4464:                                             ; preds = %4459
  %4465 = load i64, ptr @data_seg_addr, align 8
  %4466 = inttoptr i64 %4465 to ptr
  %4467 = zext i8 %4419 to i64
  %4468 = and i64 %4467, 3
  %4469 = icmp ult i8 %4419, 4
  br i1 %4469, label %4539, label %4470

4470:                                             ; preds = %4464
  %4471 = and i64 %4467, 252
  %4472 = getelementptr i8, ptr %4466, i64 1
  %4473 = getelementptr i8, ptr %4466, i64 2
  %4474 = getelementptr i8, ptr %4466, i64 3
  br label %4475

4475:                                             ; preds = %4475, %4470
  %4476 = phi i64 [ 0, %4470 ], [ %4508, %4475 ]
  %4477 = phi i64 [ 0, %4470 ], [ %4507, %4475 ]
  %4478 = phi i64 [ %4462, %4470 ], [ %4500, %4475 ]
  %4479 = phi i64 [ 0, %4470 ], [ %4509, %4475 ]
  %4480 = getelementptr inbounds nuw i8, ptr %4466, i64 %4478
  %4481 = load i8, ptr %4480, align 1
  %4482 = zext i8 %4481 to i64
  %4483 = shl nsw i64 %4476, 3
  %4484 = shl i64 %4482, %4483
  %4485 = or i64 %4484, %4477
  %4486 = getelementptr i8, ptr %4472, i64 %4478
  %4487 = load i8, ptr %4486, align 1
  %4488 = zext i8 %4487 to i64
  %4489 = shl i64 %4476, 3
  %4490 = or disjoint i64 %4489, 8
  %4491 = shl i64 %4488, %4490
  %4492 = or i64 %4491, %4485
  %4493 = getelementptr i8, ptr %4473, i64 %4478
  %4494 = load i8, ptr %4493, align 1
  %4495 = zext i8 %4494 to i64
  %4496 = shl i64 %4476, 3
  %4497 = or disjoint i64 %4496, 16
  %4498 = shl i64 %4495, %4497
  %4499 = or i64 %4498, %4492
  %4500 = add i64 %4478, 4
  %4501 = getelementptr i8, ptr %4474, i64 %4478
  %4502 = load i8, ptr %4501, align 1
  %4503 = zext i8 %4502 to i64
  %4504 = shl i64 %4476, 3
  %4505 = or disjoint i64 %4504, 24
  %4506 = shl i64 %4503, %4505
  %4507 = or i64 %4506, %4499
  %4508 = add nuw nsw i64 %4476, 4
  %4509 = add i64 %4479, 4
  %4510 = icmp eq i64 %4509, %4471
  br i1 %4510, label %4539, label %4475, !llvm.loop !17

4511:                                             ; preds = %4404
  %4512 = icmp eq i8 %4411, %4418
  br i1 %4512, label %4560, label %4513

4513:                                             ; preds = %4511
  %4514 = sext i32 %4420 to i64
  %4515 = zext i8 %4419 to i64
  br label %4516

4516:                                             ; preds = %4513, %4516
  %4517 = phi i64 [ %4514, %4513 ], [ %4521, %4516 ]
  %4518 = phi i64 [ 0, %4513 ], [ %4537, %4516 ]
  %4519 = phi i64 [ 0, %4513 ], [ %4536, %4516 ]
  %4520 = phi i32 [ %4429, %4513 ], [ %4530, %4516 ]
  %4521 = add nsw i64 %4517, 1
  %4522 = trunc nsw i64 %4521 to i32
  store i32 %4522, ptr @ip, align 4
  %4523 = getelementptr inbounds i8, ptr %69, i64 %4517
  %4524 = load i8, ptr %4523, align 1
  %4525 = shl i32 %4520, 13
  %4526 = xor i32 %4525, %4520
  %4527 = lshr i32 %4526, 17
  %4528 = xor i32 %4527, %4526
  %4529 = shl i32 %4528, 5
  %4530 = xor i32 %4529, %4528
  store i32 %4530, ptr @vm_code_state, align 4
  %4531 = trunc i32 %4530 to i8
  %4532 = xor i8 %4524, %4531
  %4533 = zext i8 %4532 to i64
  %4534 = shl nsw i64 %4518, 3
  %4535 = shl i64 %4533, %4534
  %4536 = or i64 %4535, %4519
  %4537 = add nuw nsw i64 %4518, 1
  %4538 = icmp eq i64 %4537, %4515
  br i1 %4538, label %4560, label %4516, !llvm.loop !15

4539:                                             ; preds = %4475, %4464
  %4540 = phi i64 [ poison, %4464 ], [ %4507, %4475 ]
  %4541 = phi i64 [ 0, %4464 ], [ %4508, %4475 ]
  %4542 = phi i64 [ 0, %4464 ], [ %4507, %4475 ]
  %4543 = phi i64 [ %4462, %4464 ], [ %4500, %4475 ]
  %4544 = icmp eq i64 %4468, 0
  br i1 %4544, label %4560, label %4545

4545:                                             ; preds = %4539, %4545
  %4546 = phi i64 [ %4557, %4545 ], [ %4541, %4539 ]
  %4547 = phi i64 [ %4556, %4545 ], [ %4542, %4539 ]
  %4548 = phi i64 [ %4550, %4545 ], [ %4543, %4539 ]
  %4549 = phi i64 [ %4558, %4545 ], [ 0, %4539 ]
  %4550 = add i64 %4548, 1
  %4551 = getelementptr inbounds nuw i8, ptr %4466, i64 %4548
  %4552 = load i8, ptr %4551, align 1
  %4553 = zext i8 %4552 to i64
  %4554 = shl nsw i64 %4546, 3
  %4555 = shl i64 %4553, %4554
  %4556 = or i64 %4555, %4547
  %4557 = add nuw nsw i64 %4546, 1
  %4558 = add i64 %4549, 1
  %4559 = icmp eq i64 %4558, %4468
  br i1 %4559, label %4560, label %4545, !llvm.loop !48

4560:                                             ; preds = %4516, %4539, %4545, %4511, %4459
  %4561 = phi i32 [ %4460, %4459 ], [ %4429, %4511 ], [ %4460, %4545 ], [ %4460, %4539 ], [ %4530, %4516 ]
  %4562 = phi i32 [ %4461, %4459 ], [ %4420, %4511 ], [ %4461, %4545 ], [ %4461, %4539 ], [ %4522, %4516 ]
  %4563 = phi i64 [ 0, %4459 ], [ 0, %4511 ], [ %4540, %4539 ], [ %4556, %4545 ], [ %4536, %4516 ]
  %4564 = add nsw i32 %4562, 1
  store i32 %4564, ptr @ip, align 4
  %4565 = sext i32 %4562 to i64
  %4566 = getelementptr inbounds i8, ptr %69, i64 %4565
  %4567 = load i8, ptr %4566, align 1
  %4568 = shl i32 %4561, 13
  %4569 = xor i32 %4568, %4561
  %4570 = lshr i32 %4569, 17
  %4571 = xor i32 %4570, %4569
  %4572 = shl i32 %4571, 5
  %4573 = xor i32 %4572, %4571
  store i32 %4573, ptr @vm_code_state, align 4
  %4574 = trunc i32 %4573 to i8
  %4575 = xor i8 %4567, %4574
  %4576 = add nsw i32 %4562, 2
  store i32 %4576, ptr @ip, align 4
  %4577 = sext i32 %4564 to i64
  %4578 = getelementptr inbounds i8, ptr %69, i64 %4577
  %4579 = load i8, ptr %4578, align 1
  %4580 = shl i32 %4573, 13
  %4581 = xor i32 %4580, %4573
  %4582 = lshr i32 %4581, 17
  %4583 = xor i32 %4582, %4581
  %4584 = shl i32 %4583, 5
  %4585 = xor i32 %4584, %4583
  store i32 %4585, ptr @vm_code_state, align 4
  %4586 = trunc i32 %4585 to i8
  %4587 = icmp eq i8 %4579, %4586
  br i1 %4587, label %4588, label %4667

4588:                                             ; preds = %4560
  br i1 %4377, label %4589, label %4615

4589:                                             ; preds = %4588
  %4590 = sext i32 %4576 to i64
  %4591 = zext nneg i32 %4376 to i64
  br label %4592

4592:                                             ; preds = %4589, %4592
  %4593 = phi i64 [ %4590, %4589 ], [ %4597, %4592 ]
  %4594 = phi i64 [ 0, %4589 ], [ %4613, %4592 ]
  %4595 = phi i64 [ 0, %4589 ], [ %4612, %4592 ]
  %4596 = phi i32 [ %4585, %4589 ], [ %4606, %4592 ]
  %4597 = add nsw i64 %4593, 1
  %4598 = trunc nsw i64 %4597 to i32
  store i32 %4598, ptr @ip, align 4
  %4599 = getelementptr inbounds i8, ptr %69, i64 %4593
  %4600 = load i8, ptr %4599, align 1
  %4601 = shl i32 %4596, 13
  %4602 = xor i32 %4601, %4596
  %4603 = lshr i32 %4602, 17
  %4604 = xor i32 %4603, %4602
  %4605 = shl i32 %4604, 5
  %4606 = xor i32 %4605, %4604
  store i32 %4606, ptr @vm_code_state, align 4
  %4607 = trunc i32 %4606 to i8
  %4608 = xor i8 %4600, %4607
  %4609 = zext i8 %4608 to i64
  %4610 = shl nsw i64 %4594, 3
  %4611 = shl i64 %4609, %4610
  %4612 = or i64 %4611, %4595
  %4613 = add nuw nsw i64 %4594, 1
  %4614 = icmp eq i64 %4613, %4591
  br i1 %4614, label %4615, label %4592, !llvm.loop !15

4615:                                             ; preds = %4592, %4588
  %4616 = phi i32 [ %4585, %4588 ], [ %4606, %4592 ]
  %4617 = phi i32 [ %4576, %4588 ], [ %4598, %4592 ]
  %4618 = phi i64 [ 0, %4588 ], [ %4612, %4592 ]
  %4619 = icmp eq i8 %4567, %4574
  br i1 %4619, label %4716, label %4620

4620:                                             ; preds = %4615
  %4621 = load i64, ptr @data_seg_addr, align 8
  %4622 = inttoptr i64 %4621 to ptr
  %4623 = zext i8 %4575 to i64
  %4624 = and i64 %4623, 3
  %4625 = icmp ult i8 %4575, 4
  br i1 %4625, label %4695, label %4626

4626:                                             ; preds = %4620
  %4627 = and i64 %4623, 252
  %4628 = getelementptr i8, ptr %4622, i64 1
  %4629 = getelementptr i8, ptr %4622, i64 2
  %4630 = getelementptr i8, ptr %4622, i64 3
  br label %4631

4631:                                             ; preds = %4631, %4626
  %4632 = phi i64 [ 0, %4626 ], [ %4664, %4631 ]
  %4633 = phi i64 [ 0, %4626 ], [ %4663, %4631 ]
  %4634 = phi i64 [ %4618, %4626 ], [ %4656, %4631 ]
  %4635 = phi i64 [ 0, %4626 ], [ %4665, %4631 ]
  %4636 = getelementptr inbounds nuw i8, ptr %4622, i64 %4634
  %4637 = load i8, ptr %4636, align 1
  %4638 = zext i8 %4637 to i64
  %4639 = shl nsw i64 %4632, 3
  %4640 = shl i64 %4638, %4639
  %4641 = or i64 %4640, %4633
  %4642 = getelementptr i8, ptr %4628, i64 %4634
  %4643 = load i8, ptr %4642, align 1
  %4644 = zext i8 %4643 to i64
  %4645 = shl i64 %4632, 3
  %4646 = or disjoint i64 %4645, 8
  %4647 = shl i64 %4644, %4646
  %4648 = or i64 %4647, %4641
  %4649 = getelementptr i8, ptr %4629, i64 %4634
  %4650 = load i8, ptr %4649, align 1
  %4651 = zext i8 %4650 to i64
  %4652 = shl i64 %4632, 3
  %4653 = or disjoint i64 %4652, 16
  %4654 = shl i64 %4651, %4653
  %4655 = or i64 %4654, %4648
  %4656 = add i64 %4634, 4
  %4657 = getelementptr i8, ptr %4630, i64 %4634
  %4658 = load i8, ptr %4657, align 1
  %4659 = zext i8 %4658 to i64
  %4660 = shl i64 %4632, 3
  %4661 = or disjoint i64 %4660, 24
  %4662 = shl i64 %4659, %4661
  %4663 = or i64 %4662, %4655
  %4664 = add nuw nsw i64 %4632, 4
  %4665 = add i64 %4635, 4
  %4666 = icmp eq i64 %4665, %4627
  br i1 %4666, label %4695, label %4631, !llvm.loop !17

4667:                                             ; preds = %4560
  %4668 = icmp eq i8 %4567, %4574
  br i1 %4668, label %4716, label %4669

4669:                                             ; preds = %4667
  %4670 = sext i32 %4576 to i64
  %4671 = zext i8 %4575 to i64
  br label %4672

4672:                                             ; preds = %4669, %4672
  %4673 = phi i64 [ %4670, %4669 ], [ %4677, %4672 ]
  %4674 = phi i64 [ 0, %4669 ], [ %4693, %4672 ]
  %4675 = phi i64 [ 0, %4669 ], [ %4692, %4672 ]
  %4676 = phi i32 [ %4585, %4669 ], [ %4686, %4672 ]
  %4677 = add nsw i64 %4673, 1
  %4678 = trunc nsw i64 %4677 to i32
  store i32 %4678, ptr @ip, align 4
  %4679 = getelementptr inbounds i8, ptr %69, i64 %4673
  %4680 = load i8, ptr %4679, align 1
  %4681 = shl i32 %4676, 13
  %4682 = xor i32 %4681, %4676
  %4683 = lshr i32 %4682, 17
  %4684 = xor i32 %4683, %4682
  %4685 = shl i32 %4684, 5
  %4686 = xor i32 %4685, %4684
  store i32 %4686, ptr @vm_code_state, align 4
  %4687 = trunc i32 %4686 to i8
  %4688 = xor i8 %4680, %4687
  %4689 = zext i8 %4688 to i64
  %4690 = shl nsw i64 %4674, 3
  %4691 = shl i64 %4689, %4690
  %4692 = or i64 %4691, %4675
  %4693 = add nuw nsw i64 %4674, 1
  %4694 = icmp eq i64 %4693, %4671
  br i1 %4694, label %4716, label %4672, !llvm.loop !15

4695:                                             ; preds = %4631, %4620
  %4696 = phi i64 [ poison, %4620 ], [ %4663, %4631 ]
  %4697 = phi i64 [ 0, %4620 ], [ %4664, %4631 ]
  %4698 = phi i64 [ 0, %4620 ], [ %4663, %4631 ]
  %4699 = phi i64 [ %4618, %4620 ], [ %4656, %4631 ]
  %4700 = icmp eq i64 %4624, 0
  br i1 %4700, label %4716, label %4701

4701:                                             ; preds = %4695, %4701
  %4702 = phi i64 [ %4713, %4701 ], [ %4697, %4695 ]
  %4703 = phi i64 [ %4712, %4701 ], [ %4698, %4695 ]
  %4704 = phi i64 [ %4706, %4701 ], [ %4699, %4695 ]
  %4705 = phi i64 [ %4714, %4701 ], [ 0, %4695 ]
  %4706 = add i64 %4704, 1
  %4707 = getelementptr inbounds nuw i8, ptr %4622, i64 %4704
  %4708 = load i8, ptr %4707, align 1
  %4709 = zext i8 %4708 to i64
  %4710 = shl nsw i64 %4702, 3
  %4711 = shl i64 %4709, %4710
  %4712 = or i64 %4711, %4703
  %4713 = add nuw nsw i64 %4702, 1
  %4714 = add i64 %4705, 1
  %4715 = icmp eq i64 %4714, %4624
  br i1 %4715, label %4716, label %4701, !llvm.loop !49

4716:                                             ; preds = %4672, %4695, %4701, %4667, %4615
  %4717 = phi i32 [ %4616, %4615 ], [ %4585, %4667 ], [ %4616, %4701 ], [ %4616, %4695 ], [ %4686, %4672 ]
  %4718 = phi i32 [ %4617, %4615 ], [ %4576, %4667 ], [ %4617, %4701 ], [ %4617, %4695 ], [ %4678, %4672 ]
  %4719 = phi i64 [ 0, %4615 ], [ 0, %4667 ], [ %4696, %4695 ], [ %4712, %4701 ], [ %4692, %4672 ]
  %4720 = sext i32 %4718 to i64
  %4721 = add nsw i64 %4720, 1
  %4722 = trunc nsw i64 %4721 to i32
  store i32 %4722, ptr @ip, align 4
  %4723 = getelementptr inbounds i8, ptr %69, i64 %4720
  %4724 = load i8, ptr %4723, align 1
  %4725 = shl i32 %4717, 13
  %4726 = xor i32 %4725, %4717
  %4727 = lshr i32 %4726, 17
  %4728 = xor i32 %4727, %4726
  %4729 = shl i32 %4728, 5
  %4730 = xor i32 %4729, %4728
  store i32 %4730, ptr @vm_code_state, align 4
  %4731 = trunc i32 %4730 to i8
  %4732 = xor i8 %4724, %4731
  %4733 = zext i8 %4732 to i32
  %4734 = add nsw i64 %4720, 2
  %4735 = trunc nsw i64 %4734 to i32
  store i32 %4735, ptr @ip, align 4
  %4736 = getelementptr inbounds i8, ptr %69, i64 %4721
  %4737 = load i8, ptr %4736, align 1
  %4738 = shl i32 %4730, 13
  %4739 = xor i32 %4738, %4730
  %4740 = lshr i32 %4739, 17
  %4741 = xor i32 %4740, %4739
  %4742 = shl i32 %4741, 5
  %4743 = xor i32 %4742, %4741
  store i32 %4743, ptr @vm_code_state, align 4
  %4744 = trunc i32 %4743 to i8
  %4745 = xor i8 %4737, %4744
  %4746 = zext i8 %4745 to i32
  %4747 = shl nuw nsw i32 %4746, 8
  %4748 = or disjoint i32 %4747, %4733
  %4749 = add nsw i64 %4720, 3
  %4750 = trunc nsw i64 %4749 to i32
  store i32 %4750, ptr @ip, align 4
  %4751 = getelementptr inbounds i8, ptr %69, i64 %4734
  %4752 = load i8, ptr %4751, align 1
  %4753 = shl i32 %4743, 13
  %4754 = xor i32 %4753, %4743
  %4755 = lshr i32 %4754, 17
  %4756 = xor i32 %4755, %4754
  %4757 = shl i32 %4756, 5
  %4758 = xor i32 %4757, %4756
  store i32 %4758, ptr @vm_code_state, align 4
  %4759 = trunc i32 %4758 to i8
  %4760 = xor i8 %4752, %4759
  %4761 = zext i8 %4760 to i32
  %4762 = shl nuw nsw i32 %4761, 16
  %4763 = or disjoint i32 %4762, %4748
  %4764 = add i32 %4718, 4
  store i32 %4764, ptr @ip, align 4
  %4765 = getelementptr inbounds i8, ptr %69, i64 %4749
  %4766 = load i8, ptr %4765, align 1
  %4767 = shl i32 %4758, 13
  %4768 = xor i32 %4767, %4758
  %4769 = lshr i32 %4768, 17
  %4770 = xor i32 %4769, %4768
  %4771 = shl i32 %4770, 5
  %4772 = xor i32 %4771, %4770
  store i32 %4772, ptr @vm_code_state, align 4
  %4773 = trunc i32 %4772 to i8
  %4774 = xor i8 %4766, %4773
  %4775 = zext i8 %4774 to i32
  %4776 = shl nuw i32 %4775, 24
  %4777 = or disjoint i32 %4776, %4763
  %4778 = icmp eq i32 %4777, 0
  br i1 %4778, label %4806, label %4779

4779:                                             ; preds = %4716
  %4780 = tail call i32 @llvm.umin.i32(i32 %4777, i32 8)
  %4781 = add i64 %4719, %4563
  %4782 = inttoptr i64 %4781 to ptr
  %4783 = zext nneg i32 %4780 to i64
  %4784 = and i64 %4783, 3
  %4785 = icmp ult i32 %4777, 4
  br i1 %4785, label %4788, label %4786

4786:                                             ; preds = %4779
  %4787 = and i64 %4783, 12
  br label %4847

4788:                                             ; preds = %4847, %4779
  %4789 = phi i64 [ poison, %4779 ], [ %4877, %4847 ]
  %4790 = phi i64 [ 0, %4779 ], [ %4878, %4847 ]
  %4791 = phi i64 [ 0, %4779 ], [ %4877, %4847 ]
  %4792 = icmp eq i64 %4784, 0
  br i1 %4792, label %4806, label %4793

4793:                                             ; preds = %4788, %4793
  %4794 = phi i64 [ %4803, %4793 ], [ %4790, %4788 ]
  %4795 = phi i64 [ %4802, %4793 ], [ %4791, %4788 ]
  %4796 = phi i64 [ %4804, %4793 ], [ 0, %4788 ]
  %4797 = getelementptr inbounds nuw i8, ptr %4782, i64 %4794
  %4798 = load i8, ptr %4797, align 1
  %4799 = zext i8 %4798 to i64
  %4800 = shl nuw nsw i64 %4794, 3
  %4801 = shl nuw i64 %4799, %4800
  %4802 = or i64 %4801, %4795
  %4803 = add nuw nsw i64 %4794, 1
  %4804 = add i64 %4796, 1
  %4805 = icmp eq i64 %4804, %4784
  br i1 %4805, label %4806, label %4793, !llvm.loop !50

4806:                                             ; preds = %4788, %4793, %4716
  %4807 = phi i64 [ 0, %4716 ], [ %4789, %4788 ], [ %4802, %4793 ]
  %4808 = icmp eq i8 %4360, %4367
  br i1 %4808, label %8784, label %4809

4809:                                             ; preds = %4806
  %4810 = load i64, ptr @data_seg_addr, align 8
  %4811 = add i64 %4810, %4407
  %4812 = inttoptr i64 %4811 to ptr
  %4813 = zext i8 %4368 to i32
  %4814 = and i32 %4813, 7
  %4815 = icmp ult i8 %4368, 8
  br i1 %4815, label %8706, label %4816

4816:                                             ; preds = %4809
  %4817 = and i32 %4813, 248
  br label %4818

4818:                                             ; preds = %4818, %4816
  %4819 = phi ptr [ %4812, %4816 ], [ %4844, %4818 ]
  %4820 = phi i64 [ %4807, %4816 ], [ 0, %4818 ]
  %4821 = phi i32 [ 0, %4816 ], [ %4845, %4818 ]
  %4822 = trunc i64 %4820 to i8
  store i8 %4822, ptr %4819, align 1
  %4823 = getelementptr inbounds nuw i8, ptr %4819, i64 1
  %4824 = lshr i64 %4820, 8
  %4825 = trunc i64 %4824 to i8
  store i8 %4825, ptr %4823, align 1
  %4826 = getelementptr inbounds nuw i8, ptr %4819, i64 2
  %4827 = lshr i64 %4820, 16
  %4828 = trunc i64 %4827 to i8
  store i8 %4828, ptr %4826, align 1
  %4829 = getelementptr inbounds nuw i8, ptr %4819, i64 3
  %4830 = lshr i64 %4820, 24
  %4831 = trunc i64 %4830 to i8
  store i8 %4831, ptr %4829, align 1
  %4832 = getelementptr inbounds nuw i8, ptr %4819, i64 4
  %4833 = lshr i64 %4820, 32
  %4834 = trunc i64 %4833 to i8
  store i8 %4834, ptr %4832, align 1
  %4835 = getelementptr inbounds nuw i8, ptr %4819, i64 5
  %4836 = lshr i64 %4820, 40
  %4837 = trunc i64 %4836 to i8
  store i8 %4837, ptr %4835, align 1
  %4838 = getelementptr inbounds nuw i8, ptr %4819, i64 6
  %4839 = lshr i64 %4820, 48
  %4840 = trunc i64 %4839 to i8
  store i8 %4840, ptr %4838, align 1
  %4841 = getelementptr inbounds nuw i8, ptr %4819, i64 7
  %4842 = lshr i64 %4820, 56
  %4843 = trunc nuw i64 %4842 to i8
  store i8 %4843, ptr %4841, align 1
  %4844 = getelementptr inbounds nuw i8, ptr %4819, i64 8
  %4845 = add i32 %4821, 8
  %4846 = icmp eq i32 %4845, %4817
  br i1 %4846, label %8706, label %4818, !llvm.loop !16

4847:                                             ; preds = %4847, %4786
  %4848 = phi i64 [ 0, %4786 ], [ %4878, %4847 ]
  %4849 = phi i64 [ 0, %4786 ], [ %4877, %4847 ]
  %4850 = phi i64 [ 0, %4786 ], [ %4879, %4847 ]
  %4851 = getelementptr inbounds nuw i8, ptr %4782, i64 %4848
  %4852 = load i8, ptr %4851, align 1
  %4853 = zext i8 %4852 to i64
  %4854 = shl nuw nsw i64 %4848, 3
  %4855 = shl nuw i64 %4853, %4854
  %4856 = or i64 %4855, %4849
  %4857 = or disjoint i64 %4848, 1
  %4858 = getelementptr inbounds nuw i8, ptr %4782, i64 %4857
  %4859 = load i8, ptr %4858, align 1
  %4860 = zext i8 %4859 to i64
  %4861 = shl nuw nsw i64 %4857, 3
  %4862 = shl nuw i64 %4860, %4861
  %4863 = or i64 %4862, %4856
  %4864 = or disjoint i64 %4848, 2
  %4865 = getelementptr inbounds nuw i8, ptr %4782, i64 %4864
  %4866 = load i8, ptr %4865, align 1
  %4867 = zext i8 %4866 to i64
  %4868 = shl nuw nsw i64 %4864, 3
  %4869 = shl nuw i64 %4867, %4868
  %4870 = or i64 %4869, %4863
  %4871 = or disjoint i64 %4848, 3
  %4872 = getelementptr inbounds nuw i8, ptr %4782, i64 %4871
  %4873 = load i8, ptr %4872, align 1
  %4874 = zext i8 %4873 to i64
  %4875 = shl nuw nsw i64 %4871, 3
  %4876 = shl nuw i64 %4874, %4875
  %4877 = or i64 %4876, %4870
  %4878 = add nuw nsw i64 %4848, 4
  %4879 = add i64 %4850, 4
  %4880 = icmp eq i64 %4879, %4787
  br i1 %4880, label %4788, label %4847, !llvm.loop !51

4881:                                             ; preds = %158
  %4882 = add nsw i32 %72, 2
  store i32 %4882, ptr @ip, align 4
  %4883 = sext i32 %73 to i64
  %4884 = getelementptr inbounds i8, ptr %69, i64 %4883
  %4885 = load i8, ptr %4884, align 1
  %4886 = shl i32 %81, 13
  %4887 = xor i32 %4886, %81
  %4888 = lshr i32 %4887, 17
  %4889 = xor i32 %4888, %4887
  %4890 = shl i32 %4889, 5
  %4891 = xor i32 %4890, %4889
  %4892 = trunc i32 %4891 to i8
  %4893 = xor i8 %4885, %4892
  %4894 = add nsw i32 %72, 3
  %4895 = shl i32 %4891, 13
  %4896 = xor i32 %4895, %4891
  %4897 = lshr i32 %4896, 17
  %4898 = xor i32 %4897, %4896
  %4899 = shl i32 %4898, 5
  %4900 = xor i32 %4899, %4898
  store i32 %4900, ptr @vm_code_state, align 4
  %4901 = load i32, ptr @pointer_size, align 4
  %4902 = icmp sgt i32 %4901, 0
  br i1 %4902, label %4903, label %4906

4903:                                             ; preds = %4881
  %4904 = sext i32 %4894 to i64
  %4905 = zext nneg i32 %4901 to i64
  br label %4970

4906:                                             ; preds = %4970, %4881
  %4907 = phi i32 [ %4900, %4881 ], [ %4984, %4970 ]
  %4908 = phi i32 [ %4894, %4881 ], [ %4976, %4970 ]
  %4909 = phi i64 [ 0, %4881 ], [ %4990, %4970 ]
  %4910 = sext i32 %4908 to i64
  %4911 = add nsw i64 %4910, 1
  %4912 = trunc nsw i64 %4911 to i32
  store i32 %4912, ptr @ip, align 4
  %4913 = getelementptr inbounds i8, ptr %69, i64 %4910
  %4914 = load i8, ptr %4913, align 1
  %4915 = shl i32 %4907, 13
  %4916 = xor i32 %4915, %4907
  %4917 = lshr i32 %4916, 17
  %4918 = xor i32 %4917, %4916
  %4919 = shl i32 %4918, 5
  %4920 = xor i32 %4919, %4918
  store i32 %4920, ptr @vm_code_state, align 4
  %4921 = trunc i32 %4920 to i8
  %4922 = xor i8 %4914, %4921
  %4923 = zext i8 %4922 to i32
  %4924 = add nsw i64 %4910, 2
  %4925 = trunc nsw i64 %4924 to i32
  store i32 %4925, ptr @ip, align 4
  %4926 = getelementptr inbounds i8, ptr %69, i64 %4911
  %4927 = load i8, ptr %4926, align 1
  %4928 = shl i32 %4920, 13
  %4929 = xor i32 %4928, %4920
  %4930 = lshr i32 %4929, 17
  %4931 = xor i32 %4930, %4929
  %4932 = shl i32 %4931, 5
  %4933 = xor i32 %4932, %4931
  store i32 %4933, ptr @vm_code_state, align 4
  %4934 = trunc i32 %4933 to i8
  %4935 = xor i8 %4927, %4934
  %4936 = zext i8 %4935 to i32
  %4937 = shl nuw nsw i32 %4936, 8
  %4938 = or disjoint i32 %4937, %4923
  %4939 = add nsw i64 %4910, 3
  %4940 = trunc nsw i64 %4939 to i32
  store i32 %4940, ptr @ip, align 4
  %4941 = getelementptr inbounds i8, ptr %69, i64 %4924
  %4942 = load i8, ptr %4941, align 1
  %4943 = shl i32 %4933, 13
  %4944 = xor i32 %4943, %4933
  %4945 = lshr i32 %4944, 17
  %4946 = xor i32 %4945, %4944
  %4947 = shl i32 %4946, 5
  %4948 = xor i32 %4947, %4946
  store i32 %4948, ptr @vm_code_state, align 4
  %4949 = trunc i32 %4948 to i8
  %4950 = xor i8 %4942, %4949
  %4951 = zext i8 %4950 to i32
  %4952 = shl nuw nsw i32 %4951, 16
  %4953 = or disjoint i32 %4952, %4938
  %4954 = add i32 %4908, 4
  store i32 %4954, ptr @ip, align 4
  %4955 = getelementptr inbounds i8, ptr %69, i64 %4939
  %4956 = load i8, ptr %4955, align 1
  %4957 = shl i32 %4948, 13
  %4958 = xor i32 %4957, %4948
  %4959 = lshr i32 %4958, 17
  %4960 = xor i32 %4959, %4958
  %4961 = shl i32 %4960, 5
  %4962 = xor i32 %4961, %4960
  store i32 %4962, ptr @vm_code_state, align 4
  %4963 = trunc i32 %4962 to i8
  %4964 = xor i8 %4956, %4963
  %4965 = zext i8 %4964 to i32
  %4966 = shl nuw i32 %4965, 24
  %4967 = or disjoint i32 %4966, %4953
  %4968 = icmp eq i32 %4967, 0
  %4969 = load i64, ptr @data_seg_addr, align 8
  br i1 %4968, label %5008, label %4993

4970:                                             ; preds = %4903, %4970
  %4971 = phi i64 [ %4904, %4903 ], [ %4975, %4970 ]
  %4972 = phi i64 [ 0, %4903 ], [ %4991, %4970 ]
  %4973 = phi i64 [ 0, %4903 ], [ %4990, %4970 ]
  %4974 = phi i32 [ %4900, %4903 ], [ %4984, %4970 ]
  %4975 = add nsw i64 %4971, 1
  %4976 = trunc nsw i64 %4975 to i32
  store i32 %4976, ptr @ip, align 4
  %4977 = getelementptr inbounds i8, ptr %69, i64 %4971
  %4978 = load i8, ptr %4977, align 1
  %4979 = shl i32 %4974, 13
  %4980 = xor i32 %4979, %4974
  %4981 = lshr i32 %4980, 17
  %4982 = xor i32 %4981, %4980
  %4983 = shl i32 %4982, 5
  %4984 = xor i32 %4983, %4982
  store i32 %4984, ptr @vm_code_state, align 4
  %4985 = trunc i32 %4984 to i8
  %4986 = xor i8 %4978, %4985
  %4987 = zext i8 %4986 to i64
  %4988 = shl nsw i64 %4972, 3
  %4989 = shl i64 %4987, %4988
  %4990 = or i64 %4989, %4973
  %4991 = add nuw nsw i64 %4972, 1
  %4992 = icmp eq i64 %4991, %4905
  br i1 %4992, label %4906, label %4970, !llvm.loop !15

4993:                                             ; preds = %4906
  %4994 = inttoptr i64 %4969 to ptr
  %4995 = load i64, ptr @last_br_from_bb_id, align 8
  %4996 = zext nneg i32 %4901 to i64
  %4997 = zext nneg i32 %4901 to i64
  %4998 = getelementptr i8, ptr %4994, i64 1
  %4999 = getelementptr i8, ptr %4994, i64 2
  %5000 = getelementptr i8, ptr %4994, i64 3
  br label %5001

5001:                                             ; preds = %4993, %5221
  %5002 = phi i64 [ 0, %4993 ], [ %5226, %5221 ]
  %5003 = phi i32 [ 0, %4993 ], [ %5227, %5221 ]
  %5004 = phi i32 [ %4954, %4993 ], [ %5223, %5221 ]
  %5005 = phi i32 [ %4962, %4993 ], [ %5222, %5221 ]
  br i1 %4902, label %5006, label %5071

5006:                                             ; preds = %5001
  %5007 = sext i32 %5004 to i64
  br label %5048

5008:                                             ; preds = %5221, %4906
  %5009 = phi i64 [ 0, %4906 ], [ %5226, %5221 ]
  %5010 = icmp eq i8 %4885, %4892
  br i1 %5010, label %8784, label %5011

5011:                                             ; preds = %5008
  %5012 = add i64 %4969, %4909
  %5013 = inttoptr i64 %5012 to ptr
  %5014 = zext i8 %4893 to i32
  %5015 = and i32 %5014, 7
  %5016 = icmp ult i8 %4893, 8
  br i1 %5016, label %8719, label %5017

5017:                                             ; preds = %5011
  %5018 = and i32 %5014, 248
  br label %5019

5019:                                             ; preds = %5019, %5017
  %5020 = phi ptr [ %5013, %5017 ], [ %5045, %5019 ]
  %5021 = phi i64 [ %5009, %5017 ], [ 0, %5019 ]
  %5022 = phi i32 [ 0, %5017 ], [ %5046, %5019 ]
  %5023 = trunc i64 %5021 to i8
  store i8 %5023, ptr %5020, align 1
  %5024 = getelementptr inbounds nuw i8, ptr %5020, i64 1
  %5025 = lshr i64 %5021, 8
  %5026 = trunc i64 %5025 to i8
  store i8 %5026, ptr %5024, align 1
  %5027 = getelementptr inbounds nuw i8, ptr %5020, i64 2
  %5028 = lshr i64 %5021, 16
  %5029 = trunc i64 %5028 to i8
  store i8 %5029, ptr %5027, align 1
  %5030 = getelementptr inbounds nuw i8, ptr %5020, i64 3
  %5031 = lshr i64 %5021, 24
  %5032 = trunc i64 %5031 to i8
  store i8 %5032, ptr %5030, align 1
  %5033 = getelementptr inbounds nuw i8, ptr %5020, i64 4
  %5034 = lshr i64 %5021, 32
  %5035 = trunc i64 %5034 to i8
  store i8 %5035, ptr %5033, align 1
  %5036 = getelementptr inbounds nuw i8, ptr %5020, i64 5
  %5037 = lshr i64 %5021, 40
  %5038 = trunc i64 %5037 to i8
  store i8 %5038, ptr %5036, align 1
  %5039 = getelementptr inbounds nuw i8, ptr %5020, i64 6
  %5040 = lshr i64 %5021, 48
  %5041 = trunc i64 %5040 to i8
  store i8 %5041, ptr %5039, align 1
  %5042 = getelementptr inbounds nuw i8, ptr %5020, i64 7
  %5043 = lshr i64 %5021, 56
  %5044 = trunc nuw i64 %5043 to i8
  store i8 %5044, ptr %5042, align 1
  %5045 = getelementptr inbounds nuw i8, ptr %5020, i64 8
  %5046 = add i32 %5022, 8
  %5047 = icmp eq i32 %5046, %5018
  br i1 %5047, label %8719, label %5019, !llvm.loop !16

5048:                                             ; preds = %5006, %5048
  %5049 = phi i64 [ %5007, %5006 ], [ %5053, %5048 ]
  %5050 = phi i64 [ 0, %5006 ], [ %5069, %5048 ]
  %5051 = phi i64 [ 0, %5006 ], [ %5068, %5048 ]
  %5052 = phi i32 [ %5005, %5006 ], [ %5062, %5048 ]
  %5053 = add nsw i64 %5049, 1
  %5054 = trunc nsw i64 %5053 to i32
  store i32 %5054, ptr @ip, align 4
  %5055 = getelementptr inbounds i8, ptr %69, i64 %5049
  %5056 = load i8, ptr %5055, align 1
  %5057 = shl i32 %5052, 13
  %5058 = xor i32 %5057, %5052
  %5059 = lshr i32 %5058, 17
  %5060 = xor i32 %5059, %5058
  %5061 = shl i32 %5060, 5
  %5062 = xor i32 %5061, %5060
  store i32 %5062, ptr @vm_code_state, align 4
  %5063 = trunc i32 %5062 to i8
  %5064 = xor i8 %5056, %5063
  %5065 = zext i8 %5064 to i64
  %5066 = shl nsw i64 %5050, 3
  %5067 = shl i64 %5065, %5066
  %5068 = or i64 %5067, %5051
  %5069 = add nuw nsw i64 %5050, 1
  %5070 = icmp eq i64 %5069, %4996
  br i1 %5070, label %5071, label %5048, !llvm.loop !15

5071:                                             ; preds = %5048, %5001
  %5072 = phi i32 [ %5005, %5001 ], [ %5062, %5048 ]
  %5073 = phi i32 [ %5004, %5001 ], [ %5054, %5048 ]
  %5074 = phi i64 [ 0, %5001 ], [ %5068, %5048 ]
  %5075 = add nsw i32 %5073, 1
  store i32 %5075, ptr @ip, align 4
  %5076 = sext i32 %5073 to i64
  %5077 = getelementptr inbounds i8, ptr %69, i64 %5076
  %5078 = load i8, ptr %5077, align 1
  %5079 = shl i32 %5072, 13
  %5080 = xor i32 %5079, %5072
  %5081 = lshr i32 %5080, 17
  %5082 = xor i32 %5081, %5080
  %5083 = shl i32 %5082, 5
  %5084 = xor i32 %5083, %5082
  store i32 %5084, ptr @vm_code_state, align 4
  %5085 = trunc i32 %5084 to i8
  %5086 = xor i8 %5078, %5085
  %5087 = add i32 %5073, 2
  store i32 %5087, ptr @ip, align 4
  %5088 = sext i32 %5075 to i64
  %5089 = getelementptr inbounds i8, ptr %69, i64 %5088
  %5090 = load i8, ptr %5089, align 1
  %5091 = shl i32 %5084, 13
  %5092 = xor i32 %5091, %5084
  %5093 = lshr i32 %5092, 17
  %5094 = xor i32 %5093, %5092
  %5095 = shl i32 %5094, 5
  %5096 = xor i32 %5095, %5094
  store i32 %5096, ptr @vm_code_state, align 4
  %5097 = trunc i32 %5096 to i8
  %5098 = icmp eq i8 %5090, %5097
  br i1 %5098, label %5099, label %5172

5099:                                             ; preds = %5071
  br i1 %4902, label %5100, label %5125

5100:                                             ; preds = %5099
  %5101 = sext i32 %5087 to i64
  br label %5102

5102:                                             ; preds = %5100, %5102
  %5103 = phi i64 [ %5101, %5100 ], [ %5107, %5102 ]
  %5104 = phi i64 [ 0, %5100 ], [ %5123, %5102 ]
  %5105 = phi i64 [ 0, %5100 ], [ %5122, %5102 ]
  %5106 = phi i32 [ %5096, %5100 ], [ %5116, %5102 ]
  %5107 = add nsw i64 %5103, 1
  %5108 = trunc nsw i64 %5107 to i32
  store i32 %5108, ptr @ip, align 4
  %5109 = getelementptr inbounds i8, ptr %69, i64 %5103
  %5110 = load i8, ptr %5109, align 1
  %5111 = shl i32 %5106, 13
  %5112 = xor i32 %5111, %5106
  %5113 = lshr i32 %5112, 17
  %5114 = xor i32 %5113, %5112
  %5115 = shl i32 %5114, 5
  %5116 = xor i32 %5115, %5114
  store i32 %5116, ptr @vm_code_state, align 4
  %5117 = trunc i32 %5116 to i8
  %5118 = xor i8 %5110, %5117
  %5119 = zext i8 %5118 to i64
  %5120 = shl nsw i64 %5104, 3
  %5121 = shl i64 %5119, %5120
  %5122 = or i64 %5121, %5105
  %5123 = add nuw nsw i64 %5104, 1
  %5124 = icmp eq i64 %5123, %4997
  br i1 %5124, label %5125, label %5102, !llvm.loop !15

5125:                                             ; preds = %5102, %5099
  %5126 = phi i32 [ %5096, %5099 ], [ %5116, %5102 ]
  %5127 = phi i32 [ %5087, %5099 ], [ %5108, %5102 ]
  %5128 = phi i64 [ 0, %5099 ], [ %5122, %5102 ]
  %5129 = icmp eq i8 %5078, %5085
  br i1 %5129, label %5221, label %5130

5130:                                             ; preds = %5125
  %5131 = zext i8 %5086 to i64
  %5132 = and i64 %5131, 3
  %5133 = icmp ult i8 %5086, 4
  br i1 %5133, label %5200, label %5134

5134:                                             ; preds = %5130
  %5135 = and i64 %5131, 252
  br label %5136

5136:                                             ; preds = %5136, %5134
  %5137 = phi i64 [ 0, %5134 ], [ %5169, %5136 ]
  %5138 = phi i64 [ 0, %5134 ], [ %5168, %5136 ]
  %5139 = phi i64 [ %5128, %5134 ], [ %5161, %5136 ]
  %5140 = phi i64 [ 0, %5134 ], [ %5170, %5136 ]
  %5141 = getelementptr inbounds nuw i8, ptr %4994, i64 %5139
  %5142 = load i8, ptr %5141, align 1
  %5143 = zext i8 %5142 to i64
  %5144 = shl nsw i64 %5137, 3
  %5145 = shl i64 %5143, %5144
  %5146 = or i64 %5145, %5138
  %5147 = getelementptr i8, ptr %4998, i64 %5139
  %5148 = load i8, ptr %5147, align 1
  %5149 = zext i8 %5148 to i64
  %5150 = shl i64 %5137, 3
  %5151 = or disjoint i64 %5150, 8
  %5152 = shl i64 %5149, %5151
  %5153 = or i64 %5152, %5146
  %5154 = getelementptr i8, ptr %4999, i64 %5139
  %5155 = load i8, ptr %5154, align 1
  %5156 = zext i8 %5155 to i64
  %5157 = shl i64 %5137, 3
  %5158 = or disjoint i64 %5157, 16
  %5159 = shl i64 %5156, %5158
  %5160 = or i64 %5159, %5153
  %5161 = add i64 %5139, 4
  %5162 = getelementptr i8, ptr %5000, i64 %5139
  %5163 = load i8, ptr %5162, align 1
  %5164 = zext i8 %5163 to i64
  %5165 = shl i64 %5137, 3
  %5166 = or disjoint i64 %5165, 24
  %5167 = shl i64 %5164, %5166
  %5168 = or i64 %5167, %5160
  %5169 = add nuw nsw i64 %5137, 4
  %5170 = add i64 %5140, 4
  %5171 = icmp eq i64 %5170, %5135
  br i1 %5171, label %5200, label %5136, !llvm.loop !17

5172:                                             ; preds = %5071
  %5173 = icmp eq i8 %5078, %5085
  br i1 %5173, label %5221, label %5174

5174:                                             ; preds = %5172
  %5175 = sext i32 %5087 to i64
  %5176 = zext i8 %5086 to i64
  br label %5177

5177:                                             ; preds = %5174, %5177
  %5178 = phi i64 [ %5175, %5174 ], [ %5182, %5177 ]
  %5179 = phi i64 [ 0, %5174 ], [ %5198, %5177 ]
  %5180 = phi i64 [ 0, %5174 ], [ %5197, %5177 ]
  %5181 = phi i32 [ %5096, %5174 ], [ %5191, %5177 ]
  %5182 = add nsw i64 %5178, 1
  %5183 = trunc nsw i64 %5182 to i32
  store i32 %5183, ptr @ip, align 4
  %5184 = getelementptr inbounds i8, ptr %69, i64 %5178
  %5185 = load i8, ptr %5184, align 1
  %5186 = shl i32 %5181, 13
  %5187 = xor i32 %5186, %5181
  %5188 = lshr i32 %5187, 17
  %5189 = xor i32 %5188, %5187
  %5190 = shl i32 %5189, 5
  %5191 = xor i32 %5190, %5189
  store i32 %5191, ptr @vm_code_state, align 4
  %5192 = trunc i32 %5191 to i8
  %5193 = xor i8 %5185, %5192
  %5194 = zext i8 %5193 to i64
  %5195 = shl nsw i64 %5179, 3
  %5196 = shl i64 %5194, %5195
  %5197 = or i64 %5196, %5180
  %5198 = add nuw nsw i64 %5179, 1
  %5199 = icmp eq i64 %5198, %5176
  br i1 %5199, label %5221, label %5177, !llvm.loop !15

5200:                                             ; preds = %5136, %5130
  %5201 = phi i64 [ poison, %5130 ], [ %5168, %5136 ]
  %5202 = phi i64 [ 0, %5130 ], [ %5169, %5136 ]
  %5203 = phi i64 [ 0, %5130 ], [ %5168, %5136 ]
  %5204 = phi i64 [ %5128, %5130 ], [ %5161, %5136 ]
  %5205 = icmp eq i64 %5132, 0
  br i1 %5205, label %5221, label %5206

5206:                                             ; preds = %5200, %5206
  %5207 = phi i64 [ %5218, %5206 ], [ %5202, %5200 ]
  %5208 = phi i64 [ %5217, %5206 ], [ %5203, %5200 ]
  %5209 = phi i64 [ %5211, %5206 ], [ %5204, %5200 ]
  %5210 = phi i64 [ %5219, %5206 ], [ 0, %5200 ]
  %5211 = add i64 %5209, 1
  %5212 = getelementptr inbounds nuw i8, ptr %4994, i64 %5209
  %5213 = load i8, ptr %5212, align 1
  %5214 = zext i8 %5213 to i64
  %5215 = shl nsw i64 %5207, 3
  %5216 = shl i64 %5214, %5215
  %5217 = or i64 %5216, %5208
  %5218 = add nuw nsw i64 %5207, 1
  %5219 = add i64 %5210, 1
  %5220 = icmp eq i64 %5219, %5132
  br i1 %5220, label %5221, label %5206, !llvm.loop !52

5221:                                             ; preds = %5177, %5200, %5206, %5172, %5125
  %5222 = phi i32 [ %5126, %5125 ], [ %5096, %5172 ], [ %5126, %5206 ], [ %5126, %5200 ], [ %5191, %5177 ]
  %5223 = phi i32 [ %5127, %5125 ], [ %5087, %5172 ], [ %5127, %5206 ], [ %5127, %5200 ], [ %5183, %5177 ]
  %5224 = phi i64 [ 0, %5125 ], [ 0, %5172 ], [ %5201, %5200 ], [ %5217, %5206 ], [ %5197, %5177 ]
  %5225 = icmp eq i64 %5074, %4995
  %5226 = select i1 %5225, i64 %5224, i64 %5002
  %5227 = add nuw i32 %5003, 1
  %5228 = icmp eq i32 %5227, %4967
  br i1 %5228, label %5008, label %5001, !llvm.loop !53

5229:                                             ; preds = %158
  %5230 = add nsw i32 %72, 2
  store i32 %5230, ptr @ip, align 4
  %5231 = sext i32 %73 to i64
  %5232 = getelementptr inbounds i8, ptr %69, i64 %5231
  %5233 = load i8, ptr %5232, align 1
  %5234 = shl i32 %81, 13
  %5235 = xor i32 %5234, %81
  %5236 = lshr i32 %5235, 17
  %5237 = xor i32 %5236, %5235
  %5238 = shl i32 %5237, 5
  %5239 = xor i32 %5238, %5237
  %5240 = trunc i32 %5239 to i8
  %5241 = xor i8 %5233, %5240
  %5242 = add nsw i32 %72, 3
  %5243 = shl i32 %5239, 13
  %5244 = xor i32 %5243, %5239
  %5245 = lshr i32 %5244, 17
  %5246 = xor i32 %5245, %5244
  %5247 = shl i32 %5246, 5
  %5248 = xor i32 %5247, %5246
  store i32 %5248, ptr @vm_code_state, align 4
  %5249 = load i32, ptr @pointer_size, align 4
  %5250 = icmp sgt i32 %5249, 0
  br i1 %5250, label %5251, label %5277

5251:                                             ; preds = %5229
  %5252 = sext i32 %5242 to i64
  %5253 = zext nneg i32 %5249 to i64
  br label %5254

5254:                                             ; preds = %5251, %5254
  %5255 = phi i64 [ %5252, %5251 ], [ %5259, %5254 ]
  %5256 = phi i64 [ 0, %5251 ], [ %5275, %5254 ]
  %5257 = phi i64 [ 0, %5251 ], [ %5274, %5254 ]
  %5258 = phi i32 [ %5248, %5251 ], [ %5268, %5254 ]
  %5259 = add nsw i64 %5255, 1
  %5260 = trunc nsw i64 %5259 to i32
  store i32 %5260, ptr @ip, align 4
  %5261 = getelementptr inbounds i8, ptr %69, i64 %5255
  %5262 = load i8, ptr %5261, align 1
  %5263 = shl i32 %5258, 13
  %5264 = xor i32 %5263, %5258
  %5265 = lshr i32 %5264, 17
  %5266 = xor i32 %5265, %5264
  %5267 = shl i32 %5266, 5
  %5268 = xor i32 %5267, %5266
  store i32 %5268, ptr @vm_code_state, align 4
  %5269 = trunc i32 %5268 to i8
  %5270 = xor i8 %5262, %5269
  %5271 = zext i8 %5270 to i64
  %5272 = shl nsw i64 %5256, 3
  %5273 = shl i64 %5271, %5272
  %5274 = or i64 %5273, %5257
  %5275 = add nuw nsw i64 %5256, 1
  %5276 = icmp eq i64 %5275, %5253
  br i1 %5276, label %5277, label %5254, !llvm.loop !15

5277:                                             ; preds = %5254, %5229
  %5278 = phi i32 [ %5248, %5229 ], [ %5268, %5254 ]
  %5279 = phi i32 [ %5242, %5229 ], [ %5260, %5254 ]
  %5280 = phi i64 [ 0, %5229 ], [ %5274, %5254 ]
  %5281 = add nsw i32 %5279, 1
  store i32 %5281, ptr @ip, align 4
  %5282 = sext i32 %5279 to i64
  %5283 = getelementptr inbounds i8, ptr %69, i64 %5282
  %5284 = load i8, ptr %5283, align 1
  %5285 = shl i32 %5278, 13
  %5286 = xor i32 %5285, %5278
  %5287 = lshr i32 %5286, 17
  %5288 = xor i32 %5287, %5286
  %5289 = shl i32 %5288, 5
  %5290 = xor i32 %5289, %5288
  store i32 %5290, ptr @vm_code_state, align 4
  %5291 = trunc i32 %5290 to i8
  %5292 = xor i8 %5284, %5291
  %5293 = add nsw i32 %5279, 2
  store i32 %5293, ptr @ip, align 4
  %5294 = sext i32 %5281 to i64
  %5295 = getelementptr inbounds i8, ptr %69, i64 %5294
  %5296 = load i8, ptr %5295, align 1
  %5297 = shl i32 %5290, 13
  %5298 = xor i32 %5297, %5290
  %5299 = lshr i32 %5298, 17
  %5300 = xor i32 %5299, %5298
  %5301 = shl i32 %5300, 5
  %5302 = xor i32 %5301, %5300
  store i32 %5302, ptr @vm_code_state, align 4
  %5303 = trunc i32 %5302 to i8
  %5304 = icmp eq i8 %5296, %5303
  br i1 %5304, label %5305, label %5384

5305:                                             ; preds = %5277
  br i1 %5250, label %5306, label %5332

5306:                                             ; preds = %5305
  %5307 = sext i32 %5293 to i64
  %5308 = zext nneg i32 %5249 to i64
  br label %5309

5309:                                             ; preds = %5306, %5309
  %5310 = phi i64 [ %5307, %5306 ], [ %5314, %5309 ]
  %5311 = phi i64 [ 0, %5306 ], [ %5330, %5309 ]
  %5312 = phi i64 [ 0, %5306 ], [ %5329, %5309 ]
  %5313 = phi i32 [ %5302, %5306 ], [ %5323, %5309 ]
  %5314 = add nsw i64 %5310, 1
  %5315 = trunc nsw i64 %5314 to i32
  store i32 %5315, ptr @ip, align 4
  %5316 = getelementptr inbounds i8, ptr %69, i64 %5310
  %5317 = load i8, ptr %5316, align 1
  %5318 = shl i32 %5313, 13
  %5319 = xor i32 %5318, %5313
  %5320 = lshr i32 %5319, 17
  %5321 = xor i32 %5320, %5319
  %5322 = shl i32 %5321, 5
  %5323 = xor i32 %5322, %5321
  store i32 %5323, ptr @vm_code_state, align 4
  %5324 = trunc i32 %5323 to i8
  %5325 = xor i8 %5317, %5324
  %5326 = zext i8 %5325 to i64
  %5327 = shl nsw i64 %5311, 3
  %5328 = shl i64 %5326, %5327
  %5329 = or i64 %5328, %5312
  %5330 = add nuw nsw i64 %5311, 1
  %5331 = icmp eq i64 %5330, %5308
  br i1 %5331, label %5332, label %5309, !llvm.loop !15

5332:                                             ; preds = %5309, %5305
  %5333 = phi i32 [ %5302, %5305 ], [ %5323, %5309 ]
  %5334 = phi i32 [ %5293, %5305 ], [ %5315, %5309 ]
  %5335 = phi i64 [ 0, %5305 ], [ %5329, %5309 ]
  %5336 = icmp eq i8 %5284, %5291
  br i1 %5336, label %5433, label %5337

5337:                                             ; preds = %5332
  %5338 = load i64, ptr @data_seg_addr, align 8
  %5339 = inttoptr i64 %5338 to ptr
  %5340 = zext i8 %5292 to i64
  %5341 = and i64 %5340, 3
  %5342 = icmp ult i8 %5292, 4
  br i1 %5342, label %5412, label %5343

5343:                                             ; preds = %5337
  %5344 = and i64 %5340, 252
  %5345 = getelementptr i8, ptr %5339, i64 1
  %5346 = getelementptr i8, ptr %5339, i64 2
  %5347 = getelementptr i8, ptr %5339, i64 3
  br label %5348

5348:                                             ; preds = %5348, %5343
  %5349 = phi i64 [ 0, %5343 ], [ %5381, %5348 ]
  %5350 = phi i64 [ 0, %5343 ], [ %5380, %5348 ]
  %5351 = phi i64 [ %5335, %5343 ], [ %5373, %5348 ]
  %5352 = phi i64 [ 0, %5343 ], [ %5382, %5348 ]
  %5353 = getelementptr inbounds nuw i8, ptr %5339, i64 %5351
  %5354 = load i8, ptr %5353, align 1
  %5355 = zext i8 %5354 to i64
  %5356 = shl nsw i64 %5349, 3
  %5357 = shl i64 %5355, %5356
  %5358 = or i64 %5357, %5350
  %5359 = getelementptr i8, ptr %5345, i64 %5351
  %5360 = load i8, ptr %5359, align 1
  %5361 = zext i8 %5360 to i64
  %5362 = shl i64 %5349, 3
  %5363 = or disjoint i64 %5362, 8
  %5364 = shl i64 %5361, %5363
  %5365 = or i64 %5364, %5358
  %5366 = getelementptr i8, ptr %5346, i64 %5351
  %5367 = load i8, ptr %5366, align 1
  %5368 = zext i8 %5367 to i64
  %5369 = shl i64 %5349, 3
  %5370 = or disjoint i64 %5369, 16
  %5371 = shl i64 %5368, %5370
  %5372 = or i64 %5371, %5365
  %5373 = add i64 %5351, 4
  %5374 = getelementptr i8, ptr %5347, i64 %5351
  %5375 = load i8, ptr %5374, align 1
  %5376 = zext i8 %5375 to i64
  %5377 = shl i64 %5349, 3
  %5378 = or disjoint i64 %5377, 24
  %5379 = shl i64 %5376, %5378
  %5380 = or i64 %5379, %5372
  %5381 = add nuw nsw i64 %5349, 4
  %5382 = add i64 %5352, 4
  %5383 = icmp eq i64 %5382, %5344
  br i1 %5383, label %5412, label %5348, !llvm.loop !17

5384:                                             ; preds = %5277
  %5385 = icmp eq i8 %5284, %5291
  br i1 %5385, label %5433, label %5386

5386:                                             ; preds = %5384
  %5387 = sext i32 %5293 to i64
  %5388 = zext i8 %5292 to i64
  br label %5389

5389:                                             ; preds = %5386, %5389
  %5390 = phi i64 [ %5387, %5386 ], [ %5394, %5389 ]
  %5391 = phi i64 [ 0, %5386 ], [ %5410, %5389 ]
  %5392 = phi i64 [ 0, %5386 ], [ %5409, %5389 ]
  %5393 = phi i32 [ %5302, %5386 ], [ %5403, %5389 ]
  %5394 = add nsw i64 %5390, 1
  %5395 = trunc nsw i64 %5394 to i32
  store i32 %5395, ptr @ip, align 4
  %5396 = getelementptr inbounds i8, ptr %69, i64 %5390
  %5397 = load i8, ptr %5396, align 1
  %5398 = shl i32 %5393, 13
  %5399 = xor i32 %5398, %5393
  %5400 = lshr i32 %5399, 17
  %5401 = xor i32 %5400, %5399
  %5402 = shl i32 %5401, 5
  %5403 = xor i32 %5402, %5401
  store i32 %5403, ptr @vm_code_state, align 4
  %5404 = trunc i32 %5403 to i8
  %5405 = xor i8 %5397, %5404
  %5406 = zext i8 %5405 to i64
  %5407 = shl nsw i64 %5391, 3
  %5408 = shl i64 %5406, %5407
  %5409 = or i64 %5408, %5392
  %5410 = add nuw nsw i64 %5391, 1
  %5411 = icmp eq i64 %5410, %5388
  br i1 %5411, label %5433, label %5389, !llvm.loop !15

5412:                                             ; preds = %5348, %5337
  %5413 = phi i64 [ poison, %5337 ], [ %5380, %5348 ]
  %5414 = phi i64 [ 0, %5337 ], [ %5381, %5348 ]
  %5415 = phi i64 [ 0, %5337 ], [ %5380, %5348 ]
  %5416 = phi i64 [ %5335, %5337 ], [ %5373, %5348 ]
  %5417 = icmp eq i64 %5341, 0
  br i1 %5417, label %5433, label %5418

5418:                                             ; preds = %5412, %5418
  %5419 = phi i64 [ %5430, %5418 ], [ %5414, %5412 ]
  %5420 = phi i64 [ %5429, %5418 ], [ %5415, %5412 ]
  %5421 = phi i64 [ %5423, %5418 ], [ %5416, %5412 ]
  %5422 = phi i64 [ %5431, %5418 ], [ 0, %5412 ]
  %5423 = add i64 %5421, 1
  %5424 = getelementptr inbounds nuw i8, ptr %5339, i64 %5421
  %5425 = load i8, ptr %5424, align 1
  %5426 = zext i8 %5425 to i64
  %5427 = shl nsw i64 %5419, 3
  %5428 = shl i64 %5426, %5427
  %5429 = or i64 %5428, %5420
  %5430 = add nuw nsw i64 %5419, 1
  %5431 = add i64 %5422, 1
  %5432 = icmp eq i64 %5431, %5341
  br i1 %5432, label %5433, label %5418, !llvm.loop !54

5433:                                             ; preds = %5389, %5412, %5418, %5384, %5332
  %5434 = phi i32 [ %5333, %5332 ], [ %5302, %5384 ], [ %5333, %5418 ], [ %5333, %5412 ], [ %5403, %5389 ]
  %5435 = phi i32 [ %5334, %5332 ], [ %5293, %5384 ], [ %5334, %5418 ], [ %5334, %5412 ], [ %5395, %5389 ]
  %5436 = phi i64 [ 0, %5332 ], [ 0, %5384 ], [ %5413, %5412 ], [ %5429, %5418 ], [ %5409, %5389 ]
  %5437 = add nsw i32 %5435, 1
  store i32 %5437, ptr @ip, align 4
  %5438 = sext i32 %5435 to i64
  %5439 = getelementptr inbounds i8, ptr %69, i64 %5438
  %5440 = load i8, ptr %5439, align 1
  %5441 = shl i32 %5434, 13
  %5442 = xor i32 %5441, %5434
  %5443 = lshr i32 %5442, 17
  %5444 = xor i32 %5443, %5442
  %5445 = shl i32 %5444, 5
  %5446 = xor i32 %5445, %5444
  store i32 %5446, ptr @vm_code_state, align 4
  %5447 = trunc i32 %5446 to i8
  %5448 = xor i8 %5440, %5447
  %5449 = add nsw i32 %5435, 2
  store i32 %5449, ptr @ip, align 4
  %5450 = sext i32 %5437 to i64
  %5451 = getelementptr inbounds i8, ptr %69, i64 %5450
  %5452 = load i8, ptr %5451, align 1
  %5453 = shl i32 %5446, 13
  %5454 = xor i32 %5453, %5446
  %5455 = lshr i32 %5454, 17
  %5456 = xor i32 %5455, %5454
  %5457 = shl i32 %5456, 5
  %5458 = xor i32 %5457, %5456
  store i32 %5458, ptr @vm_code_state, align 4
  %5459 = trunc i32 %5458 to i8
  %5460 = icmp eq i8 %5452, %5459
  br i1 %5460, label %5461, label %5540

5461:                                             ; preds = %5433
  br i1 %5250, label %5462, label %5488

5462:                                             ; preds = %5461
  %5463 = sext i32 %5449 to i64
  %5464 = zext nneg i32 %5249 to i64
  br label %5465

5465:                                             ; preds = %5462, %5465
  %5466 = phi i64 [ %5463, %5462 ], [ %5470, %5465 ]
  %5467 = phi i64 [ 0, %5462 ], [ %5486, %5465 ]
  %5468 = phi i64 [ 0, %5462 ], [ %5485, %5465 ]
  %5469 = phi i32 [ %5458, %5462 ], [ %5479, %5465 ]
  %5470 = add nsw i64 %5466, 1
  %5471 = trunc nsw i64 %5470 to i32
  store i32 %5471, ptr @ip, align 4
  %5472 = getelementptr inbounds i8, ptr %69, i64 %5466
  %5473 = load i8, ptr %5472, align 1
  %5474 = shl i32 %5469, 13
  %5475 = xor i32 %5474, %5469
  %5476 = lshr i32 %5475, 17
  %5477 = xor i32 %5476, %5475
  %5478 = shl i32 %5477, 5
  %5479 = xor i32 %5478, %5477
  store i32 %5479, ptr @vm_code_state, align 4
  %5480 = trunc i32 %5479 to i8
  %5481 = xor i8 %5473, %5480
  %5482 = zext i8 %5481 to i64
  %5483 = shl nsw i64 %5467, 3
  %5484 = shl i64 %5482, %5483
  %5485 = or i64 %5484, %5468
  %5486 = add nuw nsw i64 %5467, 1
  %5487 = icmp eq i64 %5486, %5464
  br i1 %5487, label %5488, label %5465, !llvm.loop !15

5488:                                             ; preds = %5465, %5461
  %5489 = phi i32 [ %5458, %5461 ], [ %5479, %5465 ]
  %5490 = phi i32 [ %5449, %5461 ], [ %5471, %5465 ]
  %5491 = phi i64 [ 0, %5461 ], [ %5485, %5465 ]
  %5492 = icmp eq i8 %5440, %5447
  br i1 %5492, label %5589, label %5493

5493:                                             ; preds = %5488
  %5494 = load i64, ptr @data_seg_addr, align 8
  %5495 = inttoptr i64 %5494 to ptr
  %5496 = zext i8 %5448 to i64
  %5497 = and i64 %5496, 3
  %5498 = icmp ult i8 %5448, 4
  br i1 %5498, label %5568, label %5499

5499:                                             ; preds = %5493
  %5500 = and i64 %5496, 252
  %5501 = getelementptr i8, ptr %5495, i64 1
  %5502 = getelementptr i8, ptr %5495, i64 2
  %5503 = getelementptr i8, ptr %5495, i64 3
  br label %5504

5504:                                             ; preds = %5504, %5499
  %5505 = phi i64 [ 0, %5499 ], [ %5537, %5504 ]
  %5506 = phi i64 [ 0, %5499 ], [ %5536, %5504 ]
  %5507 = phi i64 [ %5491, %5499 ], [ %5529, %5504 ]
  %5508 = phi i64 [ 0, %5499 ], [ %5538, %5504 ]
  %5509 = getelementptr inbounds nuw i8, ptr %5495, i64 %5507
  %5510 = load i8, ptr %5509, align 1
  %5511 = zext i8 %5510 to i64
  %5512 = shl nsw i64 %5505, 3
  %5513 = shl i64 %5511, %5512
  %5514 = or i64 %5513, %5506
  %5515 = getelementptr i8, ptr %5501, i64 %5507
  %5516 = load i8, ptr %5515, align 1
  %5517 = zext i8 %5516 to i64
  %5518 = shl i64 %5505, 3
  %5519 = or disjoint i64 %5518, 8
  %5520 = shl i64 %5517, %5519
  %5521 = or i64 %5520, %5514
  %5522 = getelementptr i8, ptr %5502, i64 %5507
  %5523 = load i8, ptr %5522, align 1
  %5524 = zext i8 %5523 to i64
  %5525 = shl i64 %5505, 3
  %5526 = or disjoint i64 %5525, 16
  %5527 = shl i64 %5524, %5526
  %5528 = or i64 %5527, %5521
  %5529 = add i64 %5507, 4
  %5530 = getelementptr i8, ptr %5503, i64 %5507
  %5531 = load i8, ptr %5530, align 1
  %5532 = zext i8 %5531 to i64
  %5533 = shl i64 %5505, 3
  %5534 = or disjoint i64 %5533, 24
  %5535 = shl i64 %5532, %5534
  %5536 = or i64 %5535, %5528
  %5537 = add nuw nsw i64 %5505, 4
  %5538 = add i64 %5508, 4
  %5539 = icmp eq i64 %5538, %5500
  br i1 %5539, label %5568, label %5504, !llvm.loop !17

5540:                                             ; preds = %5433
  %5541 = icmp eq i8 %5440, %5447
  br i1 %5541, label %5589, label %5542

5542:                                             ; preds = %5540
  %5543 = sext i32 %5449 to i64
  %5544 = zext i8 %5448 to i64
  br label %5545

5545:                                             ; preds = %5542, %5545
  %5546 = phi i64 [ %5543, %5542 ], [ %5550, %5545 ]
  %5547 = phi i64 [ 0, %5542 ], [ %5566, %5545 ]
  %5548 = phi i64 [ 0, %5542 ], [ %5565, %5545 ]
  %5549 = phi i32 [ %5458, %5542 ], [ %5559, %5545 ]
  %5550 = add nsw i64 %5546, 1
  %5551 = trunc nsw i64 %5550 to i32
  store i32 %5551, ptr @ip, align 4
  %5552 = getelementptr inbounds i8, ptr %69, i64 %5546
  %5553 = load i8, ptr %5552, align 1
  %5554 = shl i32 %5549, 13
  %5555 = xor i32 %5554, %5549
  %5556 = lshr i32 %5555, 17
  %5557 = xor i32 %5556, %5555
  %5558 = shl i32 %5557, 5
  %5559 = xor i32 %5558, %5557
  store i32 %5559, ptr @vm_code_state, align 4
  %5560 = trunc i32 %5559 to i8
  %5561 = xor i8 %5553, %5560
  %5562 = zext i8 %5561 to i64
  %5563 = shl nsw i64 %5547, 3
  %5564 = shl i64 %5562, %5563
  %5565 = or i64 %5564, %5548
  %5566 = add nuw nsw i64 %5547, 1
  %5567 = icmp eq i64 %5566, %5544
  br i1 %5567, label %5589, label %5545, !llvm.loop !15

5568:                                             ; preds = %5504, %5493
  %5569 = phi i64 [ poison, %5493 ], [ %5536, %5504 ]
  %5570 = phi i64 [ 0, %5493 ], [ %5537, %5504 ]
  %5571 = phi i64 [ 0, %5493 ], [ %5536, %5504 ]
  %5572 = phi i64 [ %5491, %5493 ], [ %5529, %5504 ]
  %5573 = icmp eq i64 %5497, 0
  br i1 %5573, label %5589, label %5574

5574:                                             ; preds = %5568, %5574
  %5575 = phi i64 [ %5586, %5574 ], [ %5570, %5568 ]
  %5576 = phi i64 [ %5585, %5574 ], [ %5571, %5568 ]
  %5577 = phi i64 [ %5579, %5574 ], [ %5572, %5568 ]
  %5578 = phi i64 [ %5587, %5574 ], [ 0, %5568 ]
  %5579 = add i64 %5577, 1
  %5580 = getelementptr inbounds nuw i8, ptr %5495, i64 %5577
  %5581 = load i8, ptr %5580, align 1
  %5582 = zext i8 %5581 to i64
  %5583 = shl nsw i64 %5575, 3
  %5584 = shl i64 %5582, %5583
  %5585 = or i64 %5584, %5576
  %5586 = add nuw nsw i64 %5575, 1
  %5587 = add i64 %5578, 1
  %5588 = icmp eq i64 %5587, %5497
  br i1 %5588, label %5589, label %5574, !llvm.loop !55

5589:                                             ; preds = %5545, %5568, %5574, %5540, %5488
  %5590 = phi i32 [ %5489, %5488 ], [ %5458, %5540 ], [ %5489, %5574 ], [ %5489, %5568 ], [ %5559, %5545 ]
  %5591 = phi i32 [ %5490, %5488 ], [ %5449, %5540 ], [ %5490, %5574 ], [ %5490, %5568 ], [ %5551, %5545 ]
  %5592 = phi i64 [ 0, %5488 ], [ 0, %5540 ], [ %5569, %5568 ], [ %5585, %5574 ], [ %5565, %5545 ]
  %5593 = add nsw i32 %5591, 1
  store i32 %5593, ptr @ip, align 4
  %5594 = sext i32 %5591 to i64
  %5595 = getelementptr inbounds i8, ptr %69, i64 %5594
  %5596 = load i8, ptr %5595, align 1
  %5597 = shl i32 %5590, 13
  %5598 = xor i32 %5597, %5590
  %5599 = lshr i32 %5598, 17
  %5600 = xor i32 %5599, %5598
  %5601 = shl i32 %5600, 5
  %5602 = xor i32 %5601, %5600
  store i32 %5602, ptr @vm_code_state, align 4
  %5603 = trunc i32 %5602 to i8
  %5604 = xor i8 %5596, %5603
  %5605 = add nsw i32 %5591, 2
  store i32 %5605, ptr @ip, align 4
  %5606 = sext i32 %5593 to i64
  %5607 = getelementptr inbounds i8, ptr %69, i64 %5606
  %5608 = load i8, ptr %5607, align 1
  %5609 = shl i32 %5602, 13
  %5610 = xor i32 %5609, %5602
  %5611 = lshr i32 %5610, 17
  %5612 = xor i32 %5611, %5610
  %5613 = shl i32 %5612, 5
  %5614 = xor i32 %5613, %5612
  store i32 %5614, ptr @vm_code_state, align 4
  %5615 = trunc i32 %5614 to i8
  %5616 = icmp eq i8 %5608, %5615
  br i1 %5616, label %5617, label %5694

5617:                                             ; preds = %5589
  br i1 %5250, label %5618, label %5644

5618:                                             ; preds = %5617
  %5619 = sext i32 %5605 to i64
  %5620 = zext nneg i32 %5249 to i64
  br label %5621

5621:                                             ; preds = %5618, %5621
  %5622 = phi i64 [ %5619, %5618 ], [ %5626, %5621 ]
  %5623 = phi i64 [ 0, %5618 ], [ %5642, %5621 ]
  %5624 = phi i64 [ 0, %5618 ], [ %5641, %5621 ]
  %5625 = phi i32 [ %5614, %5618 ], [ %5635, %5621 ]
  %5626 = add nsw i64 %5622, 1
  %5627 = trunc nsw i64 %5626 to i32
  store i32 %5627, ptr @ip, align 4
  %5628 = getelementptr inbounds i8, ptr %69, i64 %5622
  %5629 = load i8, ptr %5628, align 1
  %5630 = shl i32 %5625, 13
  %5631 = xor i32 %5630, %5625
  %5632 = lshr i32 %5631, 17
  %5633 = xor i32 %5632, %5631
  %5634 = shl i32 %5633, 5
  %5635 = xor i32 %5634, %5633
  store i32 %5635, ptr @vm_code_state, align 4
  %5636 = trunc i32 %5635 to i8
  %5637 = xor i8 %5629, %5636
  %5638 = zext i8 %5637 to i64
  %5639 = shl nsw i64 %5623, 3
  %5640 = shl i64 %5638, %5639
  %5641 = or i64 %5640, %5624
  %5642 = add nuw nsw i64 %5623, 1
  %5643 = icmp eq i64 %5642, %5620
  br i1 %5643, label %5644, label %5621, !llvm.loop !15

5644:                                             ; preds = %5621, %5617
  %5645 = phi i64 [ 0, %5617 ], [ %5641, %5621 ]
  %5646 = icmp eq i8 %5596, %5603
  br i1 %5646, label %5743, label %5647

5647:                                             ; preds = %5644
  %5648 = load i64, ptr @data_seg_addr, align 8
  %5649 = inttoptr i64 %5648 to ptr
  %5650 = zext i8 %5604 to i64
  %5651 = and i64 %5650, 3
  %5652 = icmp ult i8 %5604, 4
  br i1 %5652, label %5722, label %5653

5653:                                             ; preds = %5647
  %5654 = and i64 %5650, 252
  %5655 = getelementptr i8, ptr %5649, i64 1
  %5656 = getelementptr i8, ptr %5649, i64 2
  %5657 = getelementptr i8, ptr %5649, i64 3
  br label %5658

5658:                                             ; preds = %5658, %5653
  %5659 = phi i64 [ 0, %5653 ], [ %5691, %5658 ]
  %5660 = phi i64 [ 0, %5653 ], [ %5690, %5658 ]
  %5661 = phi i64 [ %5645, %5653 ], [ %5683, %5658 ]
  %5662 = phi i64 [ 0, %5653 ], [ %5692, %5658 ]
  %5663 = getelementptr inbounds nuw i8, ptr %5649, i64 %5661
  %5664 = load i8, ptr %5663, align 1
  %5665 = zext i8 %5664 to i64
  %5666 = shl nsw i64 %5659, 3
  %5667 = shl i64 %5665, %5666
  %5668 = or i64 %5667, %5660
  %5669 = getelementptr i8, ptr %5655, i64 %5661
  %5670 = load i8, ptr %5669, align 1
  %5671 = zext i8 %5670 to i64
  %5672 = shl i64 %5659, 3
  %5673 = or disjoint i64 %5672, 8
  %5674 = shl i64 %5671, %5673
  %5675 = or i64 %5674, %5668
  %5676 = getelementptr i8, ptr %5656, i64 %5661
  %5677 = load i8, ptr %5676, align 1
  %5678 = zext i8 %5677 to i64
  %5679 = shl i64 %5659, 3
  %5680 = or disjoint i64 %5679, 16
  %5681 = shl i64 %5678, %5680
  %5682 = or i64 %5681, %5675
  %5683 = add i64 %5661, 4
  %5684 = getelementptr i8, ptr %5657, i64 %5661
  %5685 = load i8, ptr %5684, align 1
  %5686 = zext i8 %5685 to i64
  %5687 = shl i64 %5659, 3
  %5688 = or disjoint i64 %5687, 24
  %5689 = shl i64 %5686, %5688
  %5690 = or i64 %5689, %5682
  %5691 = add nuw nsw i64 %5659, 4
  %5692 = add i64 %5662, 4
  %5693 = icmp eq i64 %5692, %5654
  br i1 %5693, label %5722, label %5658, !llvm.loop !17

5694:                                             ; preds = %5589
  %5695 = icmp eq i8 %5596, %5603
  br i1 %5695, label %5743, label %5696

5696:                                             ; preds = %5694
  %5697 = sext i32 %5605 to i64
  %5698 = zext i8 %5604 to i64
  br label %5699

5699:                                             ; preds = %5696, %5699
  %5700 = phi i64 [ %5697, %5696 ], [ %5704, %5699 ]
  %5701 = phi i64 [ 0, %5696 ], [ %5720, %5699 ]
  %5702 = phi i64 [ 0, %5696 ], [ %5719, %5699 ]
  %5703 = phi i32 [ %5614, %5696 ], [ %5713, %5699 ]
  %5704 = add nsw i64 %5700, 1
  %5705 = trunc nsw i64 %5704 to i32
  store i32 %5705, ptr @ip, align 4
  %5706 = getelementptr inbounds i8, ptr %69, i64 %5700
  %5707 = load i8, ptr %5706, align 1
  %5708 = shl i32 %5703, 13
  %5709 = xor i32 %5708, %5703
  %5710 = lshr i32 %5709, 17
  %5711 = xor i32 %5710, %5709
  %5712 = shl i32 %5711, 5
  %5713 = xor i32 %5712, %5711
  store i32 %5713, ptr @vm_code_state, align 4
  %5714 = trunc i32 %5713 to i8
  %5715 = xor i8 %5707, %5714
  %5716 = zext i8 %5715 to i64
  %5717 = shl nsw i64 %5701, 3
  %5718 = shl i64 %5716, %5717
  %5719 = or i64 %5718, %5702
  %5720 = add nuw nsw i64 %5701, 1
  %5721 = icmp eq i64 %5720, %5698
  br i1 %5721, label %5743, label %5699, !llvm.loop !15

5722:                                             ; preds = %5658, %5647
  %5723 = phi i64 [ poison, %5647 ], [ %5690, %5658 ]
  %5724 = phi i64 [ 0, %5647 ], [ %5691, %5658 ]
  %5725 = phi i64 [ 0, %5647 ], [ %5690, %5658 ]
  %5726 = phi i64 [ %5645, %5647 ], [ %5683, %5658 ]
  %5727 = icmp eq i64 %5651, 0
  br i1 %5727, label %5743, label %5728

5728:                                             ; preds = %5722, %5728
  %5729 = phi i64 [ %5740, %5728 ], [ %5724, %5722 ]
  %5730 = phi i64 [ %5739, %5728 ], [ %5725, %5722 ]
  %5731 = phi i64 [ %5733, %5728 ], [ %5726, %5722 ]
  %5732 = phi i64 [ %5741, %5728 ], [ 0, %5722 ]
  %5733 = add i64 %5731, 1
  %5734 = getelementptr inbounds nuw i8, ptr %5649, i64 %5731
  %5735 = load i8, ptr %5734, align 1
  %5736 = zext i8 %5735 to i64
  %5737 = shl nsw i64 %5729, 3
  %5738 = shl i64 %5736, %5737
  %5739 = or i64 %5738, %5730
  %5740 = add nuw nsw i64 %5729, 1
  %5741 = add i64 %5732, 1
  %5742 = icmp eq i64 %5741, %5651
  br i1 %5742, label %5743, label %5728, !llvm.loop !56

5743:                                             ; preds = %5699, %5722, %5728, %5694, %5644
  %5744 = phi i64 [ 0, %5644 ], [ 0, %5694 ], [ %5723, %5722 ], [ %5739, %5728 ], [ %5719, %5699 ]
  %5745 = icmp eq i8 %5233, %5240
  br i1 %5745, label %8784, label %5746

5746:                                             ; preds = %5743
  %5747 = load i64, ptr @data_seg_addr, align 8
  %5748 = add i64 %5747, %5280
  %5749 = inttoptr i64 %5748 to ptr
  %5750 = zext i8 %5241 to i32
  %5751 = icmp eq i64 %5436, 0
  %5752 = select i1 %5751, i64 %5744, i64 %5592
  %5753 = and i32 %5750, 7
  %5754 = icmp ult i8 %5241, 8
  br i1 %5754, label %8732, label %5755

5755:                                             ; preds = %5746
  %5756 = and i32 %5750, 248
  br label %5757

5757:                                             ; preds = %5757, %5755
  %5758 = phi ptr [ %5749, %5755 ], [ %5783, %5757 ]
  %5759 = phi i64 [ %5752, %5755 ], [ 0, %5757 ]
  %5760 = phi i32 [ 0, %5755 ], [ %5784, %5757 ]
  %5761 = trunc i64 %5759 to i8
  store i8 %5761, ptr %5758, align 1
  %5762 = getelementptr inbounds nuw i8, ptr %5758, i64 1
  %5763 = lshr i64 %5759, 8
  %5764 = trunc i64 %5763 to i8
  store i8 %5764, ptr %5762, align 1
  %5765 = getelementptr inbounds nuw i8, ptr %5758, i64 2
  %5766 = lshr i64 %5759, 16
  %5767 = trunc i64 %5766 to i8
  store i8 %5767, ptr %5765, align 1
  %5768 = getelementptr inbounds nuw i8, ptr %5758, i64 3
  %5769 = lshr i64 %5759, 24
  %5770 = trunc i64 %5769 to i8
  store i8 %5770, ptr %5768, align 1
  %5771 = getelementptr inbounds nuw i8, ptr %5758, i64 4
  %5772 = lshr i64 %5759, 32
  %5773 = trunc i64 %5772 to i8
  store i8 %5773, ptr %5771, align 1
  %5774 = getelementptr inbounds nuw i8, ptr %5758, i64 5
  %5775 = lshr i64 %5759, 40
  %5776 = trunc i64 %5775 to i8
  store i8 %5776, ptr %5774, align 1
  %5777 = getelementptr inbounds nuw i8, ptr %5758, i64 6
  %5778 = lshr i64 %5759, 48
  %5779 = trunc i64 %5778 to i8
  store i8 %5779, ptr %5777, align 1
  %5780 = getelementptr inbounds nuw i8, ptr %5758, i64 7
  %5781 = lshr i64 %5759, 56
  %5782 = trunc nuw i64 %5781 to i8
  store i8 %5782, ptr %5780, align 1
  %5783 = getelementptr inbounds nuw i8, ptr %5758, i64 8
  %5784 = add i32 %5760, 8
  %5785 = icmp eq i32 %5784, %5756
  br i1 %5785, label %8732, label %5757, !llvm.loop !16

5786:                                             ; preds = %158
  %5787 = shl i32 %81, 13
  %5788 = xor i32 %5787, %81
  %5789 = lshr i32 %5788, 17
  %5790 = xor i32 %5789, %5788
  %5791 = shl i32 %5790, 5
  %5792 = xor i32 %5791, %5790
  %5793 = add nsw i32 %72, 3
  store i32 %5793, ptr @ip, align 4
  %5794 = shl i32 %5792, 13
  %5795 = xor i32 %5794, %5792
  %5796 = lshr i32 %5795, 17
  %5797 = xor i32 %5796, %5795
  %5798 = shl i32 %5797, 5
  %5799 = xor i32 %5798, %5797
  store i32 %5799, ptr @vm_code_state, align 4
  %5800 = load i32, ptr @pointer_size, align 4
  %5801 = icmp sgt i32 %5800, 0
  br i1 %5801, label %5802, label %5828

5802:                                             ; preds = %5786
  %5803 = sext i32 %5793 to i64
  %5804 = zext nneg i32 %5800 to i64
  br label %5805

5805:                                             ; preds = %5802, %5805
  %5806 = phi i64 [ %5803, %5802 ], [ %5810, %5805 ]
  %5807 = phi i64 [ 0, %5802 ], [ %5826, %5805 ]
  %5808 = phi i64 [ 0, %5802 ], [ %5825, %5805 ]
  %5809 = phi i32 [ %5799, %5802 ], [ %5819, %5805 ]
  %5810 = add nsw i64 %5806, 1
  %5811 = trunc nsw i64 %5810 to i32
  store i32 %5811, ptr @ip, align 4
  %5812 = getelementptr inbounds i8, ptr %69, i64 %5806
  %5813 = load i8, ptr %5812, align 1
  %5814 = shl i32 %5809, 13
  %5815 = xor i32 %5814, %5809
  %5816 = lshr i32 %5815, 17
  %5817 = xor i32 %5816, %5815
  %5818 = shl i32 %5817, 5
  %5819 = xor i32 %5818, %5817
  store i32 %5819, ptr @vm_code_state, align 4
  %5820 = trunc i32 %5819 to i8
  %5821 = xor i8 %5813, %5820
  %5822 = zext i8 %5821 to i64
  %5823 = shl nsw i64 %5807, 3
  %5824 = shl i64 %5822, %5823
  %5825 = or i64 %5824, %5808
  %5826 = add nuw nsw i64 %5807, 1
  %5827 = icmp eq i64 %5826, %5804
  br i1 %5827, label %5830, label %5805, !llvm.loop !15

5828:                                             ; preds = %5786
  %5829 = load i64, ptr @data_seg_addr, align 8
  br label %5886

5830:                                             ; preds = %5805
  %5831 = load i64, ptr @data_seg_addr, align 8
  %5832 = add i64 %5831, %5825
  %5833 = inttoptr i64 %5832 to ptr
  %5834 = load ptr, ptr @exception_ptr, align 8
  %5835 = ptrtoint ptr %5834 to i64
  %5836 = and i32 %5800, 7
  %5837 = icmp ult i32 %5800, 8
  br i1 %5837, label %5869, label %5838

5838:                                             ; preds = %5830
  %5839 = and i32 %5800, 2147483640
  br label %5840

5840:                                             ; preds = %5840, %5838
  %5841 = phi ptr [ %5833, %5838 ], [ %5866, %5840 ]
  %5842 = phi i64 [ %5835, %5838 ], [ 0, %5840 ]
  %5843 = phi i32 [ 0, %5838 ], [ %5867, %5840 ]
  %5844 = trunc i64 %5842 to i8
  store i8 %5844, ptr %5841, align 1
  %5845 = getelementptr inbounds nuw i8, ptr %5841, i64 1
  %5846 = lshr i64 %5842, 8
  %5847 = trunc i64 %5846 to i8
  store i8 %5847, ptr %5845, align 1
  %5848 = getelementptr inbounds nuw i8, ptr %5841, i64 2
  %5849 = lshr i64 %5842, 16
  %5850 = trunc i64 %5849 to i8
  store i8 %5850, ptr %5848, align 1
  %5851 = getelementptr inbounds nuw i8, ptr %5841, i64 3
  %5852 = lshr i64 %5842, 24
  %5853 = trunc i64 %5852 to i8
  store i8 %5853, ptr %5851, align 1
  %5854 = getelementptr inbounds nuw i8, ptr %5841, i64 4
  %5855 = lshr i64 %5842, 32
  %5856 = trunc i64 %5855 to i8
  store i8 %5856, ptr %5854, align 1
  %5857 = getelementptr inbounds nuw i8, ptr %5841, i64 5
  %5858 = lshr i64 %5842, 40
  %5859 = trunc i64 %5858 to i8
  store i8 %5859, ptr %5857, align 1
  %5860 = getelementptr inbounds nuw i8, ptr %5841, i64 6
  %5861 = lshr i64 %5842, 48
  %5862 = trunc i64 %5861 to i8
  store i8 %5862, ptr %5860, align 1
  %5863 = getelementptr inbounds nuw i8, ptr %5841, i64 7
  %5864 = lshr i64 %5842, 56
  %5865 = trunc nuw i64 %5864 to i8
  store i8 %5865, ptr %5863, align 1
  %5866 = getelementptr inbounds nuw i8, ptr %5841, i64 8
  %5867 = add i32 %5843, 8
  %5868 = icmp eq i32 %5867, %5839
  br i1 %5868, label %5869, label %5840, !llvm.loop !16

5869:                                             ; preds = %5840, %5830
  %5870 = phi ptr [ %5833, %5830 ], [ %5866, %5840 ]
  %5871 = phi i64 [ %5835, %5830 ], [ 0, %5840 ]
  %5872 = icmp eq i32 %5836, 0
  br i1 %5872, label %5882, label %5873

5873:                                             ; preds = %5869, %5873
  %5874 = phi ptr [ %5878, %5873 ], [ %5870, %5869 ]
  %5875 = phi i64 [ %5879, %5873 ], [ %5871, %5869 ]
  %5876 = phi i32 [ %5880, %5873 ], [ 0, %5869 ]
  %5877 = trunc i64 %5875 to i8
  store i8 %5877, ptr %5874, align 1
  %5878 = getelementptr inbounds nuw i8, ptr %5874, i64 1
  %5879 = lshr i64 %5875, 8
  %5880 = add i32 %5876, 1
  %5881 = icmp eq i32 %5880, %5836
  br i1 %5881, label %5882, label %5873, !llvm.loop !57

5882:                                             ; preds = %5873, %5869
  %5883 = load i64, ptr @data_seg_addr, align 8
  %5884 = load i32, ptr @pointer_size, align 4
  %5885 = add i64 %5883, %5825
  br label %5886

5886:                                             ; preds = %5828, %5882
  %5887 = phi i64 [ %5885, %5882 ], [ %5829, %5828 ]
  %5888 = phi i32 [ %5884, %5882 ], [ %5800, %5828 ]
  %5889 = load i32, ptr @exception_selector, align 4
  %5890 = zext i32 %5888 to i64
  %5891 = add i64 %5887, %5890
  %5892 = inttoptr i64 %5891 to ptr
  %5893 = trunc i32 %5889 to i8
  store i8 %5893, ptr %5892, align 1
  %5894 = getelementptr inbounds nuw i8, ptr %5892, i64 1
  %5895 = lshr i32 %5889, 8
  %5896 = trunc i32 %5895 to i8
  store i8 %5896, ptr %5894, align 1
  %5897 = getelementptr inbounds nuw i8, ptr %5892, i64 2
  %5898 = lshr i32 %5889, 16
  %5899 = trunc i32 %5898 to i8
  store i8 %5899, ptr %5897, align 1
  %5900 = getelementptr inbounds nuw i8, ptr %5892, i64 3
  %5901 = lshr i32 %5889, 24
  %5902 = trunc nuw i32 %5901 to i8
  store i8 %5902, ptr %5900, align 1
  store i8 0, ptr @exception_thrown, align 1
  br label %8784

5903:                                             ; preds = %158
  %5904 = add nsw i32 %72, 2
  store i32 %5904, ptr @ip, align 4
  %5905 = sext i32 %73 to i64
  %5906 = getelementptr inbounds i8, ptr %69, i64 %5905
  %5907 = load i8, ptr %5906, align 1
  %5908 = shl i32 %81, 13
  %5909 = xor i32 %5908, %81
  %5910 = lshr i32 %5909, 17
  %5911 = xor i32 %5910, %5909
  %5912 = shl i32 %5911, 5
  %5913 = xor i32 %5912, %5911
  store i32 %5913, ptr @vm_code_state, align 4
  %5914 = trunc i32 %5913 to i8
  %5915 = xor i8 %5907, %5914
  %5916 = add nsw i32 %72, 3
  store i32 %5916, ptr @ip, align 4
  %5917 = sext i32 %5904 to i64
  %5918 = getelementptr inbounds i8, ptr %69, i64 %5917
  %5919 = load i8, ptr %5918, align 1
  %5920 = shl i32 %5913, 13
  %5921 = xor i32 %5920, %5913
  %5922 = lshr i32 %5921, 17
  %5923 = xor i32 %5922, %5921
  %5924 = shl i32 %5923, 5
  %5925 = xor i32 %5924, %5923
  store i32 %5925, ptr @vm_code_state, align 4
  %5926 = trunc i32 %5925 to i8
  %5927 = icmp eq i8 %5919, %5926
  br i1 %5927, label %5928, label %6007

5928:                                             ; preds = %5903
  %5929 = load i32, ptr @pointer_size, align 4
  %5930 = icmp sgt i32 %5929, 0
  br i1 %5930, label %5931, label %5957

5931:                                             ; preds = %5928
  %5932 = sext i32 %5916 to i64
  %5933 = zext nneg i32 %5929 to i64
  br label %5934

5934:                                             ; preds = %5931, %5934
  %5935 = phi i64 [ %5932, %5931 ], [ %5939, %5934 ]
  %5936 = phi i64 [ 0, %5931 ], [ %5955, %5934 ]
  %5937 = phi i64 [ 0, %5931 ], [ %5954, %5934 ]
  %5938 = phi i32 [ %5925, %5931 ], [ %5948, %5934 ]
  %5939 = add nsw i64 %5935, 1
  %5940 = trunc nsw i64 %5939 to i32
  store i32 %5940, ptr @ip, align 4
  %5941 = getelementptr inbounds i8, ptr %69, i64 %5935
  %5942 = load i8, ptr %5941, align 1
  %5943 = shl i32 %5938, 13
  %5944 = xor i32 %5943, %5938
  %5945 = lshr i32 %5944, 17
  %5946 = xor i32 %5945, %5944
  %5947 = shl i32 %5946, 5
  %5948 = xor i32 %5947, %5946
  store i32 %5948, ptr @vm_code_state, align 4
  %5949 = trunc i32 %5948 to i8
  %5950 = xor i8 %5942, %5949
  %5951 = zext i8 %5950 to i64
  %5952 = shl nsw i64 %5936, 3
  %5953 = shl i64 %5951, %5952
  %5954 = or i64 %5953, %5937
  %5955 = add nuw nsw i64 %5936, 1
  %5956 = icmp eq i64 %5955, %5933
  br i1 %5956, label %5957, label %5934, !llvm.loop !15

5957:                                             ; preds = %5934, %5928
  %5958 = phi i64 [ 0, %5928 ], [ %5954, %5934 ]
  %5959 = icmp eq i8 %5907, %5914
  br i1 %5959, label %6056, label %5960

5960:                                             ; preds = %5957
  %5961 = load i64, ptr @data_seg_addr, align 8
  %5962 = inttoptr i64 %5961 to ptr
  %5963 = zext i8 %5915 to i64
  %5964 = and i64 %5963, 3
  %5965 = icmp ult i8 %5915, 4
  br i1 %5965, label %6035, label %5966

5966:                                             ; preds = %5960
  %5967 = and i64 %5963, 252
  %5968 = getelementptr i8, ptr %5962, i64 1
  %5969 = getelementptr i8, ptr %5962, i64 2
  %5970 = getelementptr i8, ptr %5962, i64 3
  br label %5971

5971:                                             ; preds = %5971, %5966
  %5972 = phi i64 [ 0, %5966 ], [ %6004, %5971 ]
  %5973 = phi i64 [ 0, %5966 ], [ %6003, %5971 ]
  %5974 = phi i64 [ %5958, %5966 ], [ %5996, %5971 ]
  %5975 = phi i64 [ 0, %5966 ], [ %6005, %5971 ]
  %5976 = getelementptr inbounds nuw i8, ptr %5962, i64 %5974
  %5977 = load i8, ptr %5976, align 1
  %5978 = zext i8 %5977 to i64
  %5979 = shl nsw i64 %5972, 3
  %5980 = shl i64 %5978, %5979
  %5981 = or i64 %5980, %5973
  %5982 = getelementptr i8, ptr %5968, i64 %5974
  %5983 = load i8, ptr %5982, align 1
  %5984 = zext i8 %5983 to i64
  %5985 = shl i64 %5972, 3
  %5986 = or disjoint i64 %5985, 8
  %5987 = shl i64 %5984, %5986
  %5988 = or i64 %5987, %5981
  %5989 = getelementptr i8, ptr %5969, i64 %5974
  %5990 = load i8, ptr %5989, align 1
  %5991 = zext i8 %5990 to i64
  %5992 = shl i64 %5972, 3
  %5993 = or disjoint i64 %5992, 16
  %5994 = shl i64 %5991, %5993
  %5995 = or i64 %5994, %5988
  %5996 = add i64 %5974, 4
  %5997 = getelementptr i8, ptr %5970, i64 %5974
  %5998 = load i8, ptr %5997, align 1
  %5999 = zext i8 %5998 to i64
  %6000 = shl i64 %5972, 3
  %6001 = or disjoint i64 %6000, 24
  %6002 = shl i64 %5999, %6001
  %6003 = or i64 %6002, %5995
  %6004 = add nuw nsw i64 %5972, 4
  %6005 = add i64 %5975, 4
  %6006 = icmp eq i64 %6005, %5967
  br i1 %6006, label %6035, label %5971, !llvm.loop !17

6007:                                             ; preds = %5903
  %6008 = icmp eq i8 %5907, %5914
  br i1 %6008, label %6056, label %6009

6009:                                             ; preds = %6007
  %6010 = sext i32 %5916 to i64
  %6011 = zext i8 %5915 to i64
  br label %6012

6012:                                             ; preds = %6009, %6012
  %6013 = phi i64 [ %6010, %6009 ], [ %6017, %6012 ]
  %6014 = phi i64 [ 0, %6009 ], [ %6033, %6012 ]
  %6015 = phi i64 [ 0, %6009 ], [ %6032, %6012 ]
  %6016 = phi i32 [ %5925, %6009 ], [ %6026, %6012 ]
  %6017 = add nsw i64 %6013, 1
  %6018 = trunc nsw i64 %6017 to i32
  store i32 %6018, ptr @ip, align 4
  %6019 = getelementptr inbounds i8, ptr %69, i64 %6013
  %6020 = load i8, ptr %6019, align 1
  %6021 = shl i32 %6016, 13
  %6022 = xor i32 %6021, %6016
  %6023 = lshr i32 %6022, 17
  %6024 = xor i32 %6023, %6022
  %6025 = shl i32 %6024, 5
  %6026 = xor i32 %6025, %6024
  store i32 %6026, ptr @vm_code_state, align 4
  %6027 = trunc i32 %6026 to i8
  %6028 = xor i8 %6020, %6027
  %6029 = zext i8 %6028 to i64
  %6030 = shl nsw i64 %6014, 3
  %6031 = shl i64 %6029, %6030
  %6032 = or i64 %6031, %6015
  %6033 = add nuw nsw i64 %6014, 1
  %6034 = icmp eq i64 %6033, %6011
  br i1 %6034, label %6056, label %6012, !llvm.loop !15

6035:                                             ; preds = %5971, %5960
  %6036 = phi i64 [ poison, %5960 ], [ %6003, %5971 ]
  %6037 = phi i64 [ 0, %5960 ], [ %6004, %5971 ]
  %6038 = phi i64 [ 0, %5960 ], [ %6003, %5971 ]
  %6039 = phi i64 [ %5958, %5960 ], [ %5996, %5971 ]
  %6040 = icmp eq i64 %5964, 0
  br i1 %6040, label %6056, label %6041

6041:                                             ; preds = %6035, %6041
  %6042 = phi i64 [ %6053, %6041 ], [ %6037, %6035 ]
  %6043 = phi i64 [ %6052, %6041 ], [ %6038, %6035 ]
  %6044 = phi i64 [ %6046, %6041 ], [ %6039, %6035 ]
  %6045 = phi i64 [ %6054, %6041 ], [ 0, %6035 ]
  %6046 = add i64 %6044, 1
  %6047 = getelementptr inbounds nuw i8, ptr %5962, i64 %6044
  %6048 = load i8, ptr %6047, align 1
  %6049 = zext i8 %6048 to i64
  %6050 = shl nsw i64 %6042, 3
  %6051 = shl i64 %6049, %6050
  %6052 = or i64 %6051, %6043
  %6053 = add nuw nsw i64 %6042, 1
  %6054 = add i64 %6045, 1
  %6055 = icmp eq i64 %6054, %5964
  br i1 %6055, label %6056, label %6041, !llvm.loop !58

6056:                                             ; preds = %6012, %6035, %6041, %6007, %5957
  %6057 = phi i64 [ 0, %5957 ], [ 0, %6007 ], [ %6036, %6035 ], [ %6052, %6041 ], [ %6032, %6012 ]
  %6058 = trunc i64 %6057 to i32
  store i32 %6058, ptr @ip, align 4
  br label %8784

6059:                                             ; preds = %158
  %6060 = add nsw i32 %72, 2
  store i32 %6060, ptr @ip, align 4
  %6061 = sext i32 %73 to i64
  %6062 = getelementptr inbounds i8, ptr %69, i64 %6061
  %6063 = load i8, ptr %6062, align 1
  %6064 = shl i32 %81, 13
  %6065 = xor i32 %6064, %81
  %6066 = lshr i32 %6065, 17
  %6067 = xor i32 %6066, %6065
  %6068 = shl i32 %6067, 5
  %6069 = xor i32 %6068, %6067
  %6070 = trunc i32 %6069 to i8
  %6071 = xor i8 %6063, %6070
  %6072 = add nsw i32 %72, 3
  %6073 = shl i32 %6069, 13
  %6074 = xor i32 %6073, %6069
  %6075 = lshr i32 %6074, 17
  %6076 = xor i32 %6075, %6074
  %6077 = shl i32 %6076, 5
  %6078 = xor i32 %6077, %6076
  store i32 %6078, ptr @vm_code_state, align 4
  %6079 = load i32, ptr @pointer_size, align 4
  %6080 = icmp sgt i32 %6079, 0
  br i1 %6080, label %6081, label %6107

6081:                                             ; preds = %6059
  %6082 = sext i32 %6072 to i64
  %6083 = zext nneg i32 %6079 to i64
  br label %6084

6084:                                             ; preds = %6081, %6084
  %6085 = phi i64 [ %6082, %6081 ], [ %6089, %6084 ]
  %6086 = phi i64 [ 0, %6081 ], [ %6105, %6084 ]
  %6087 = phi i64 [ 0, %6081 ], [ %6104, %6084 ]
  %6088 = phi i32 [ %6078, %6081 ], [ %6098, %6084 ]
  %6089 = add nsw i64 %6085, 1
  %6090 = trunc nsw i64 %6089 to i32
  store i32 %6090, ptr @ip, align 4
  %6091 = getelementptr inbounds i8, ptr %69, i64 %6085
  %6092 = load i8, ptr %6091, align 1
  %6093 = shl i32 %6088, 13
  %6094 = xor i32 %6093, %6088
  %6095 = lshr i32 %6094, 17
  %6096 = xor i32 %6095, %6094
  %6097 = shl i32 %6096, 5
  %6098 = xor i32 %6097, %6096
  store i32 %6098, ptr @vm_code_state, align 4
  %6099 = trunc i32 %6098 to i8
  %6100 = xor i8 %6092, %6099
  %6101 = zext i8 %6100 to i64
  %6102 = shl nsw i64 %6086, 3
  %6103 = shl i64 %6101, %6102
  %6104 = or i64 %6103, %6087
  %6105 = add nuw nsw i64 %6086, 1
  %6106 = icmp eq i64 %6105, %6083
  br i1 %6106, label %6107, label %6084, !llvm.loop !15

6107:                                             ; preds = %6084, %6059
  %6108 = phi i32 [ %6078, %6059 ], [ %6098, %6084 ]
  %6109 = phi i32 [ %6072, %6059 ], [ %6090, %6084 ]
  %6110 = phi i64 [ 0, %6059 ], [ %6104, %6084 ]
  %6111 = add nsw i32 %6109, 1
  store i32 %6111, ptr @ip, align 4
  %6112 = sext i32 %6109 to i64
  %6113 = getelementptr inbounds i8, ptr %69, i64 %6112
  %6114 = load i8, ptr %6113, align 1
  %6115 = shl i32 %6108, 13
  %6116 = xor i32 %6115, %6108
  %6117 = lshr i32 %6116, 17
  %6118 = xor i32 %6117, %6116
  %6119 = shl i32 %6118, 5
  %6120 = xor i32 %6119, %6118
  store i32 %6120, ptr @vm_code_state, align 4
  %6121 = trunc i32 %6120 to i8
  %6122 = xor i8 %6114, %6121
  %6123 = add nsw i32 %6109, 2
  store i32 %6123, ptr @ip, align 4
  %6124 = sext i32 %6111 to i64
  %6125 = getelementptr inbounds i8, ptr %69, i64 %6124
  %6126 = load i8, ptr %6125, align 1
  %6127 = shl i32 %6120, 13
  %6128 = xor i32 %6127, %6120
  %6129 = lshr i32 %6128, 17
  %6130 = xor i32 %6129, %6128
  %6131 = shl i32 %6130, 5
  %6132 = xor i32 %6131, %6130
  store i32 %6132, ptr @vm_code_state, align 4
  %6133 = trunc i32 %6132 to i8
  %6134 = icmp eq i8 %6126, %6133
  br i1 %6134, label %6135, label %6214

6135:                                             ; preds = %6107
  br i1 %6080, label %6136, label %6162

6136:                                             ; preds = %6135
  %6137 = sext i32 %6123 to i64
  %6138 = zext nneg i32 %6079 to i64
  br label %6139

6139:                                             ; preds = %6136, %6139
  %6140 = phi i64 [ %6137, %6136 ], [ %6144, %6139 ]
  %6141 = phi i64 [ 0, %6136 ], [ %6160, %6139 ]
  %6142 = phi i64 [ 0, %6136 ], [ %6159, %6139 ]
  %6143 = phi i32 [ %6132, %6136 ], [ %6153, %6139 ]
  %6144 = add nsw i64 %6140, 1
  %6145 = trunc nsw i64 %6144 to i32
  store i32 %6145, ptr @ip, align 4
  %6146 = getelementptr inbounds i8, ptr %69, i64 %6140
  %6147 = load i8, ptr %6146, align 1
  %6148 = shl i32 %6143, 13
  %6149 = xor i32 %6148, %6143
  %6150 = lshr i32 %6149, 17
  %6151 = xor i32 %6150, %6149
  %6152 = shl i32 %6151, 5
  %6153 = xor i32 %6152, %6151
  store i32 %6153, ptr @vm_code_state, align 4
  %6154 = trunc i32 %6153 to i8
  %6155 = xor i8 %6147, %6154
  %6156 = zext i8 %6155 to i64
  %6157 = shl nsw i64 %6141, 3
  %6158 = shl i64 %6156, %6157
  %6159 = or i64 %6158, %6142
  %6160 = add nuw nsw i64 %6141, 1
  %6161 = icmp eq i64 %6160, %6138
  br i1 %6161, label %6162, label %6139, !llvm.loop !15

6162:                                             ; preds = %6139, %6135
  %6163 = phi i32 [ %6132, %6135 ], [ %6153, %6139 ]
  %6164 = phi i32 [ %6123, %6135 ], [ %6145, %6139 ]
  %6165 = phi i64 [ 0, %6135 ], [ %6159, %6139 ]
  %6166 = icmp eq i8 %6114, %6121
  br i1 %6166, label %6263, label %6167

6167:                                             ; preds = %6162
  %6168 = load i64, ptr @data_seg_addr, align 8
  %6169 = inttoptr i64 %6168 to ptr
  %6170 = zext i8 %6122 to i64
  %6171 = and i64 %6170, 3
  %6172 = icmp ult i8 %6122, 4
  br i1 %6172, label %6242, label %6173

6173:                                             ; preds = %6167
  %6174 = and i64 %6170, 252
  %6175 = getelementptr i8, ptr %6169, i64 1
  %6176 = getelementptr i8, ptr %6169, i64 2
  %6177 = getelementptr i8, ptr %6169, i64 3
  br label %6178

6178:                                             ; preds = %6178, %6173
  %6179 = phi i64 [ 0, %6173 ], [ %6211, %6178 ]
  %6180 = phi i64 [ 0, %6173 ], [ %6210, %6178 ]
  %6181 = phi i64 [ %6165, %6173 ], [ %6203, %6178 ]
  %6182 = phi i64 [ 0, %6173 ], [ %6212, %6178 ]
  %6183 = getelementptr inbounds nuw i8, ptr %6169, i64 %6181
  %6184 = load i8, ptr %6183, align 1
  %6185 = zext i8 %6184 to i64
  %6186 = shl nsw i64 %6179, 3
  %6187 = shl i64 %6185, %6186
  %6188 = or i64 %6187, %6180
  %6189 = getelementptr i8, ptr %6175, i64 %6181
  %6190 = load i8, ptr %6189, align 1
  %6191 = zext i8 %6190 to i64
  %6192 = shl i64 %6179, 3
  %6193 = or disjoint i64 %6192, 8
  %6194 = shl i64 %6191, %6193
  %6195 = or i64 %6194, %6188
  %6196 = getelementptr i8, ptr %6176, i64 %6181
  %6197 = load i8, ptr %6196, align 1
  %6198 = zext i8 %6197 to i64
  %6199 = shl i64 %6179, 3
  %6200 = or disjoint i64 %6199, 16
  %6201 = shl i64 %6198, %6200
  %6202 = or i64 %6201, %6195
  %6203 = add i64 %6181, 4
  %6204 = getelementptr i8, ptr %6177, i64 %6181
  %6205 = load i8, ptr %6204, align 1
  %6206 = zext i8 %6205 to i64
  %6207 = shl i64 %6179, 3
  %6208 = or disjoint i64 %6207, 24
  %6209 = shl i64 %6206, %6208
  %6210 = or i64 %6209, %6202
  %6211 = add nuw nsw i64 %6179, 4
  %6212 = add i64 %6182, 4
  %6213 = icmp eq i64 %6212, %6174
  br i1 %6213, label %6242, label %6178, !llvm.loop !17

6214:                                             ; preds = %6107
  %6215 = icmp eq i8 %6114, %6121
  br i1 %6215, label %6263, label %6216

6216:                                             ; preds = %6214
  %6217 = sext i32 %6123 to i64
  %6218 = zext i8 %6122 to i64
  br label %6219

6219:                                             ; preds = %6216, %6219
  %6220 = phi i64 [ %6217, %6216 ], [ %6224, %6219 ]
  %6221 = phi i64 [ 0, %6216 ], [ %6240, %6219 ]
  %6222 = phi i64 [ 0, %6216 ], [ %6239, %6219 ]
  %6223 = phi i32 [ %6132, %6216 ], [ %6233, %6219 ]
  %6224 = add nsw i64 %6220, 1
  %6225 = trunc nsw i64 %6224 to i32
  store i32 %6225, ptr @ip, align 4
  %6226 = getelementptr inbounds i8, ptr %69, i64 %6220
  %6227 = load i8, ptr %6226, align 1
  %6228 = shl i32 %6223, 13
  %6229 = xor i32 %6228, %6223
  %6230 = lshr i32 %6229, 17
  %6231 = xor i32 %6230, %6229
  %6232 = shl i32 %6231, 5
  %6233 = xor i32 %6232, %6231
  store i32 %6233, ptr @vm_code_state, align 4
  %6234 = trunc i32 %6233 to i8
  %6235 = xor i8 %6227, %6234
  %6236 = zext i8 %6235 to i64
  %6237 = shl nsw i64 %6221, 3
  %6238 = shl i64 %6236, %6237
  %6239 = or i64 %6238, %6222
  %6240 = add nuw nsw i64 %6221, 1
  %6241 = icmp eq i64 %6240, %6218
  br i1 %6241, label %6263, label %6219, !llvm.loop !15

6242:                                             ; preds = %6178, %6167
  %6243 = phi i64 [ poison, %6167 ], [ %6210, %6178 ]
  %6244 = phi i64 [ 0, %6167 ], [ %6211, %6178 ]
  %6245 = phi i64 [ 0, %6167 ], [ %6210, %6178 ]
  %6246 = phi i64 [ %6165, %6167 ], [ %6203, %6178 ]
  %6247 = icmp eq i64 %6171, 0
  br i1 %6247, label %6263, label %6248

6248:                                             ; preds = %6242, %6248
  %6249 = phi i64 [ %6260, %6248 ], [ %6244, %6242 ]
  %6250 = phi i64 [ %6259, %6248 ], [ %6245, %6242 ]
  %6251 = phi i64 [ %6253, %6248 ], [ %6246, %6242 ]
  %6252 = phi i64 [ %6261, %6248 ], [ 0, %6242 ]
  %6253 = add i64 %6251, 1
  %6254 = getelementptr inbounds nuw i8, ptr %6169, i64 %6251
  %6255 = load i8, ptr %6254, align 1
  %6256 = zext i8 %6255 to i64
  %6257 = shl nsw i64 %6249, 3
  %6258 = shl i64 %6256, %6257
  %6259 = or i64 %6258, %6250
  %6260 = add nuw nsw i64 %6249, 1
  %6261 = add i64 %6252, 1
  %6262 = icmp eq i64 %6261, %6171
  br i1 %6262, label %6263, label %6248, !llvm.loop !59

6263:                                             ; preds = %6219, %6242, %6248, %6214, %6162
  %6264 = phi i32 [ %6163, %6162 ], [ %6132, %6214 ], [ %6163, %6248 ], [ %6163, %6242 ], [ %6233, %6219 ]
  %6265 = phi i32 [ %6164, %6162 ], [ %6123, %6214 ], [ %6164, %6248 ], [ %6164, %6242 ], [ %6225, %6219 ]
  %6266 = phi i64 [ 0, %6162 ], [ 0, %6214 ], [ %6243, %6242 ], [ %6259, %6248 ], [ %6239, %6219 ]
  %6267 = add nsw i32 %6265, 1
  store i32 %6267, ptr @ip, align 4
  %6268 = sext i32 %6265 to i64
  %6269 = getelementptr inbounds i8, ptr %69, i64 %6268
  %6270 = load i8, ptr %6269, align 1
  %6271 = shl i32 %6264, 13
  %6272 = xor i32 %6271, %6264
  %6273 = lshr i32 %6272, 17
  %6274 = xor i32 %6273, %6272
  %6275 = shl i32 %6274, 5
  %6276 = xor i32 %6275, %6274
  store i32 %6276, ptr @vm_code_state, align 4
  %6277 = trunc i32 %6276 to i8
  %6278 = xor i8 %6270, %6277
  %6279 = add nsw i32 %6265, 2
  store i32 %6279, ptr @ip, align 4
  %6280 = sext i32 %6267 to i64
  %6281 = getelementptr inbounds i8, ptr %69, i64 %6280
  %6282 = load i8, ptr %6281, align 1
  %6283 = shl i32 %6276, 13
  %6284 = xor i32 %6283, %6276
  %6285 = lshr i32 %6284, 17
  %6286 = xor i32 %6285, %6284
  %6287 = shl i32 %6286, 5
  %6288 = xor i32 %6287, %6286
  store i32 %6288, ptr @vm_code_state, align 4
  %6289 = trunc i32 %6288 to i8
  %6290 = icmp eq i8 %6282, %6289
  br i1 %6290, label %6291, label %6370

6291:                                             ; preds = %6263
  br i1 %6080, label %6292, label %6318

6292:                                             ; preds = %6291
  %6293 = sext i32 %6279 to i64
  %6294 = zext nneg i32 %6079 to i64
  br label %6295

6295:                                             ; preds = %6292, %6295
  %6296 = phi i64 [ %6293, %6292 ], [ %6300, %6295 ]
  %6297 = phi i64 [ 0, %6292 ], [ %6316, %6295 ]
  %6298 = phi i64 [ 0, %6292 ], [ %6315, %6295 ]
  %6299 = phi i32 [ %6288, %6292 ], [ %6309, %6295 ]
  %6300 = add nsw i64 %6296, 1
  %6301 = trunc nsw i64 %6300 to i32
  store i32 %6301, ptr @ip, align 4
  %6302 = getelementptr inbounds i8, ptr %69, i64 %6296
  %6303 = load i8, ptr %6302, align 1
  %6304 = shl i32 %6299, 13
  %6305 = xor i32 %6304, %6299
  %6306 = lshr i32 %6305, 17
  %6307 = xor i32 %6306, %6305
  %6308 = shl i32 %6307, 5
  %6309 = xor i32 %6308, %6307
  store i32 %6309, ptr @vm_code_state, align 4
  %6310 = trunc i32 %6309 to i8
  %6311 = xor i8 %6303, %6310
  %6312 = zext i8 %6311 to i64
  %6313 = shl nsw i64 %6297, 3
  %6314 = shl i64 %6312, %6313
  %6315 = or i64 %6314, %6298
  %6316 = add nuw nsw i64 %6297, 1
  %6317 = icmp eq i64 %6316, %6294
  br i1 %6317, label %6318, label %6295, !llvm.loop !15

6318:                                             ; preds = %6295, %6291
  %6319 = phi i32 [ %6288, %6291 ], [ %6309, %6295 ]
  %6320 = phi i32 [ %6279, %6291 ], [ %6301, %6295 ]
  %6321 = phi i64 [ 0, %6291 ], [ %6315, %6295 ]
  %6322 = icmp eq i8 %6270, %6277
  br i1 %6322, label %6419, label %6323

6323:                                             ; preds = %6318
  %6324 = load i64, ptr @data_seg_addr, align 8
  %6325 = inttoptr i64 %6324 to ptr
  %6326 = zext i8 %6278 to i64
  %6327 = and i64 %6326, 3
  %6328 = icmp ult i8 %6278, 4
  br i1 %6328, label %6398, label %6329

6329:                                             ; preds = %6323
  %6330 = and i64 %6326, 252
  %6331 = getelementptr i8, ptr %6325, i64 1
  %6332 = getelementptr i8, ptr %6325, i64 2
  %6333 = getelementptr i8, ptr %6325, i64 3
  br label %6334

6334:                                             ; preds = %6334, %6329
  %6335 = phi i64 [ 0, %6329 ], [ %6367, %6334 ]
  %6336 = phi i64 [ 0, %6329 ], [ %6366, %6334 ]
  %6337 = phi i64 [ %6321, %6329 ], [ %6359, %6334 ]
  %6338 = phi i64 [ 0, %6329 ], [ %6368, %6334 ]
  %6339 = getelementptr inbounds nuw i8, ptr %6325, i64 %6337
  %6340 = load i8, ptr %6339, align 1
  %6341 = zext i8 %6340 to i64
  %6342 = shl nsw i64 %6335, 3
  %6343 = shl i64 %6341, %6342
  %6344 = or i64 %6343, %6336
  %6345 = getelementptr i8, ptr %6331, i64 %6337
  %6346 = load i8, ptr %6345, align 1
  %6347 = zext i8 %6346 to i64
  %6348 = shl i64 %6335, 3
  %6349 = or disjoint i64 %6348, 8
  %6350 = shl i64 %6347, %6349
  %6351 = or i64 %6350, %6344
  %6352 = getelementptr i8, ptr %6332, i64 %6337
  %6353 = load i8, ptr %6352, align 1
  %6354 = zext i8 %6353 to i64
  %6355 = shl i64 %6335, 3
  %6356 = or disjoint i64 %6355, 16
  %6357 = shl i64 %6354, %6356
  %6358 = or i64 %6357, %6351
  %6359 = add i64 %6337, 4
  %6360 = getelementptr i8, ptr %6333, i64 %6337
  %6361 = load i8, ptr %6360, align 1
  %6362 = zext i8 %6361 to i64
  %6363 = shl i64 %6335, 3
  %6364 = or disjoint i64 %6363, 24
  %6365 = shl i64 %6362, %6364
  %6366 = or i64 %6365, %6358
  %6367 = add nuw nsw i64 %6335, 4
  %6368 = add i64 %6338, 4
  %6369 = icmp eq i64 %6368, %6330
  br i1 %6369, label %6398, label %6334, !llvm.loop !17

6370:                                             ; preds = %6263
  %6371 = icmp eq i8 %6270, %6277
  br i1 %6371, label %6419, label %6372

6372:                                             ; preds = %6370
  %6373 = sext i32 %6279 to i64
  %6374 = zext i8 %6278 to i64
  br label %6375

6375:                                             ; preds = %6372, %6375
  %6376 = phi i64 [ %6373, %6372 ], [ %6380, %6375 ]
  %6377 = phi i64 [ 0, %6372 ], [ %6396, %6375 ]
  %6378 = phi i64 [ 0, %6372 ], [ %6395, %6375 ]
  %6379 = phi i32 [ %6288, %6372 ], [ %6389, %6375 ]
  %6380 = add nsw i64 %6376, 1
  %6381 = trunc nsw i64 %6380 to i32
  store i32 %6381, ptr @ip, align 4
  %6382 = getelementptr inbounds i8, ptr %69, i64 %6376
  %6383 = load i8, ptr %6382, align 1
  %6384 = shl i32 %6379, 13
  %6385 = xor i32 %6384, %6379
  %6386 = lshr i32 %6385, 17
  %6387 = xor i32 %6386, %6385
  %6388 = shl i32 %6387, 5
  %6389 = xor i32 %6388, %6387
  store i32 %6389, ptr @vm_code_state, align 4
  %6390 = trunc i32 %6389 to i8
  %6391 = xor i8 %6383, %6390
  %6392 = zext i8 %6391 to i64
  %6393 = shl nsw i64 %6377, 3
  %6394 = shl i64 %6392, %6393
  %6395 = or i64 %6394, %6378
  %6396 = add nuw nsw i64 %6377, 1
  %6397 = icmp eq i64 %6396, %6374
  br i1 %6397, label %6419, label %6375, !llvm.loop !15

6398:                                             ; preds = %6334, %6323
  %6399 = phi i64 [ poison, %6323 ], [ %6366, %6334 ]
  %6400 = phi i64 [ 0, %6323 ], [ %6367, %6334 ]
  %6401 = phi i64 [ 0, %6323 ], [ %6366, %6334 ]
  %6402 = phi i64 [ %6321, %6323 ], [ %6359, %6334 ]
  %6403 = icmp eq i64 %6327, 0
  br i1 %6403, label %6419, label %6404

6404:                                             ; preds = %6398, %6404
  %6405 = phi i64 [ %6416, %6404 ], [ %6400, %6398 ]
  %6406 = phi i64 [ %6415, %6404 ], [ %6401, %6398 ]
  %6407 = phi i64 [ %6409, %6404 ], [ %6402, %6398 ]
  %6408 = phi i64 [ %6417, %6404 ], [ 0, %6398 ]
  %6409 = add i64 %6407, 1
  %6410 = getelementptr inbounds nuw i8, ptr %6325, i64 %6407
  %6411 = load i8, ptr %6410, align 1
  %6412 = zext i8 %6411 to i64
  %6413 = shl nsw i64 %6405, 3
  %6414 = shl i64 %6412, %6413
  %6415 = or i64 %6414, %6406
  %6416 = add nuw nsw i64 %6405, 1
  %6417 = add i64 %6408, 1
  %6418 = icmp eq i64 %6417, %6327
  br i1 %6418, label %6419, label %6404, !llvm.loop !60

6419:                                             ; preds = %6375, %6398, %6404, %6370, %6318
  %6420 = phi i32 [ %6319, %6318 ], [ %6288, %6370 ], [ %6319, %6404 ], [ %6319, %6398 ], [ %6389, %6375 ]
  %6421 = phi i32 [ %6320, %6318 ], [ %6279, %6370 ], [ %6320, %6404 ], [ %6320, %6398 ], [ %6381, %6375 ]
  %6422 = phi i64 [ 0, %6318 ], [ 0, %6370 ], [ %6399, %6398 ], [ %6415, %6404 ], [ %6395, %6375 ]
  %6423 = sext i32 %6421 to i64
  %6424 = add nsw i64 %6423, 1
  %6425 = trunc nsw i64 %6424 to i32
  store i32 %6425, ptr @ip, align 4
  %6426 = getelementptr inbounds i8, ptr %69, i64 %6423
  %6427 = load i8, ptr %6426, align 1
  %6428 = shl i32 %6420, 13
  %6429 = xor i32 %6428, %6420
  %6430 = lshr i32 %6429, 17
  %6431 = xor i32 %6430, %6429
  %6432 = shl i32 %6431, 5
  %6433 = xor i32 %6432, %6431
  store i32 %6433, ptr @vm_code_state, align 4
  %6434 = trunc i32 %6433 to i8
  %6435 = xor i8 %6427, %6434
  %6436 = zext i8 %6435 to i64
  %6437 = add nsw i64 %6423, 2
  %6438 = trunc nsw i64 %6437 to i32
  store i32 %6438, ptr @ip, align 4
  %6439 = getelementptr inbounds i8, ptr %69, i64 %6424
  %6440 = load i8, ptr %6439, align 1
  %6441 = shl i32 %6433, 13
  %6442 = xor i32 %6441, %6433
  %6443 = lshr i32 %6442, 17
  %6444 = xor i32 %6443, %6442
  %6445 = shl i32 %6444, 5
  %6446 = xor i32 %6445, %6444
  store i32 %6446, ptr @vm_code_state, align 4
  %6447 = trunc i32 %6446 to i8
  %6448 = xor i8 %6440, %6447
  %6449 = zext i8 %6448 to i64
  %6450 = shl nuw nsw i64 %6449, 8
  %6451 = or disjoint i64 %6450, %6436
  %6452 = add nsw i64 %6423, 3
  %6453 = trunc nsw i64 %6452 to i32
  store i32 %6453, ptr @ip, align 4
  %6454 = getelementptr inbounds i8, ptr %69, i64 %6437
  %6455 = load i8, ptr %6454, align 1
  %6456 = shl i32 %6446, 13
  %6457 = xor i32 %6456, %6446
  %6458 = lshr i32 %6457, 17
  %6459 = xor i32 %6458, %6457
  %6460 = shl i32 %6459, 5
  %6461 = xor i32 %6460, %6459
  store i32 %6461, ptr @vm_code_state, align 4
  %6462 = trunc i32 %6461 to i8
  %6463 = xor i8 %6455, %6462
  %6464 = zext i8 %6463 to i64
  %6465 = shl nuw nsw i64 %6464, 16
  %6466 = or disjoint i64 %6465, %6451
  %6467 = add i32 %6421, 4
  store i32 %6467, ptr @ip, align 4
  %6468 = getelementptr inbounds i8, ptr %69, i64 %6452
  %6469 = load i8, ptr %6468, align 1
  %6470 = shl i32 %6461, 13
  %6471 = xor i32 %6470, %6461
  %6472 = lshr i32 %6471, 17
  %6473 = xor i32 %6472, %6471
  %6474 = shl i32 %6473, 5
  %6475 = xor i32 %6474, %6473
  store i32 %6475, ptr @vm_code_state, align 4
  %6476 = trunc i32 %6475 to i8
  %6477 = xor i8 %6469, %6476
  %6478 = zext i8 %6477 to i64
  %6479 = shl nuw nsw i64 %6478, 24
  %6480 = or disjoint i64 %6479, %6466
  %6481 = trunc nuw i64 %6480 to i32
  %6482 = icmp sgt i32 %6481, 0
  br i1 %6482, label %6483, label %6550

6483:                                             ; preds = %6419
  %6484 = mul i64 %6480, %6422
  %6485 = add i64 %6484, %6266
  %6486 = inttoptr i64 %6485 to ptr
  %6487 = add nsw i64 %6479, -1
  %6488 = add nsw i64 %6487, %6465
  %6489 = add nsw i64 %6488, %6450
  %6490 = add nsw i64 %6489, %6436
  %6491 = and i64 %6436, 3
  %6492 = icmp ult i64 %6490, 3
  br i1 %6492, label %6530, label %6493

6493:                                             ; preds = %6483
  %6494 = sub nsw i64 %6480, %6491
  br label %6495

6495:                                             ; preds = %6495, %6493
  %6496 = phi i64 [ 0, %6493 ], [ %6527, %6495 ]
  %6497 = phi i64 [ 0, %6493 ], [ %6525, %6495 ]
  %6498 = phi ptr [ %6486, %6493 ], [ %6526, %6495 ]
  %6499 = phi i64 [ 0, %6493 ], [ %6528, %6495 ]
  %6500 = load i8, ptr %6498, align 1
  %6501 = zext i8 %6500 to i64
  %6502 = shl nsw i64 %6496, 3
  %6503 = shl i64 %6501, %6502
  %6504 = or i64 %6503, %6497
  %6505 = getelementptr inbounds nuw i8, ptr %6498, i64 1
  %6506 = load i8, ptr %6505, align 1
  %6507 = zext i8 %6506 to i64
  %6508 = shl i64 %6496, 3
  %6509 = or disjoint i64 %6508, 8
  %6510 = shl i64 %6507, %6509
  %6511 = or i64 %6510, %6504
  %6512 = getelementptr inbounds nuw i8, ptr %6498, i64 2
  %6513 = load i8, ptr %6512, align 1
  %6514 = zext i8 %6513 to i64
  %6515 = shl i64 %6496, 3
  %6516 = or disjoint i64 %6515, 16
  %6517 = shl i64 %6514, %6516
  %6518 = or i64 %6517, %6511
  %6519 = getelementptr inbounds nuw i8, ptr %6498, i64 3
  %6520 = load i8, ptr %6519, align 1
  %6521 = zext i8 %6520 to i64
  %6522 = shl i64 %6496, 3
  %6523 = or disjoint i64 %6522, 24
  %6524 = shl i64 %6521, %6523
  %6525 = or i64 %6524, %6518
  %6526 = getelementptr inbounds nuw i8, ptr %6498, i64 4
  %6527 = add nuw nsw i64 %6496, 4
  %6528 = add i64 %6499, 4
  %6529 = icmp eq i64 %6528, %6494
  br i1 %6529, label %6530, label %6495, !llvm.loop !20

6530:                                             ; preds = %6495, %6483
  %6531 = phi i64 [ poison, %6483 ], [ %6525, %6495 ]
  %6532 = phi i64 [ 0, %6483 ], [ %6527, %6495 ]
  %6533 = phi i64 [ 0, %6483 ], [ %6525, %6495 ]
  %6534 = phi ptr [ %6486, %6483 ], [ %6526, %6495 ]
  %6535 = icmp eq i64 %6491, 0
  br i1 %6535, label %6550, label %6536

6536:                                             ; preds = %6530, %6536
  %6537 = phi i64 [ %6547, %6536 ], [ %6532, %6530 ]
  %6538 = phi i64 [ %6545, %6536 ], [ %6533, %6530 ]
  %6539 = phi ptr [ %6546, %6536 ], [ %6534, %6530 ]
  %6540 = phi i64 [ %6548, %6536 ], [ 0, %6530 ]
  %6541 = load i8, ptr %6539, align 1
  %6542 = zext i8 %6541 to i64
  %6543 = shl nsw i64 %6537, 3
  %6544 = shl i64 %6542, %6543
  %6545 = or i64 %6544, %6538
  %6546 = getelementptr inbounds nuw i8, ptr %6539, i64 1
  %6547 = add nuw nsw i64 %6537, 1
  %6548 = add i64 %6540, 1
  %6549 = icmp eq i64 %6548, %6491
  br i1 %6549, label %6550, label %6536, !llvm.loop !61

6550:                                             ; preds = %6530, %6536, %6419
  %6551 = phi i64 [ 0, %6419 ], [ %6531, %6530 ], [ %6545, %6536 ]
  %6552 = icmp eq i8 %6063, %6070
  br i1 %6552, label %8784, label %6553

6553:                                             ; preds = %6550
  %6554 = load i64, ptr @data_seg_addr, align 8
  %6555 = add i64 %6554, %6110
  %6556 = inttoptr i64 %6555 to ptr
  %6557 = zext i8 %6071 to i32
  %6558 = and i32 %6557, 7
  %6559 = icmp ult i8 %6071, 8
  br i1 %6559, label %8745, label %6560

6560:                                             ; preds = %6553
  %6561 = and i32 %6557, 248
  br label %6562

6562:                                             ; preds = %6562, %6560
  %6563 = phi ptr [ %6556, %6560 ], [ %6588, %6562 ]
  %6564 = phi i64 [ %6551, %6560 ], [ 0, %6562 ]
  %6565 = phi i32 [ 0, %6560 ], [ %6589, %6562 ]
  %6566 = trunc i64 %6564 to i8
  store i8 %6566, ptr %6563, align 1
  %6567 = getelementptr inbounds nuw i8, ptr %6563, i64 1
  %6568 = lshr i64 %6564, 8
  %6569 = trunc i64 %6568 to i8
  store i8 %6569, ptr %6567, align 1
  %6570 = getelementptr inbounds nuw i8, ptr %6563, i64 2
  %6571 = lshr i64 %6564, 16
  %6572 = trunc i64 %6571 to i8
  store i8 %6572, ptr %6570, align 1
  %6573 = getelementptr inbounds nuw i8, ptr %6563, i64 3
  %6574 = lshr i64 %6564, 24
  %6575 = trunc i64 %6574 to i8
  store i8 %6575, ptr %6573, align 1
  %6576 = getelementptr inbounds nuw i8, ptr %6563, i64 4
  %6577 = lshr i64 %6564, 32
  %6578 = trunc i64 %6577 to i8
  store i8 %6578, ptr %6576, align 1
  %6579 = getelementptr inbounds nuw i8, ptr %6563, i64 5
  %6580 = lshr i64 %6564, 40
  %6581 = trunc i64 %6580 to i8
  store i8 %6581, ptr %6579, align 1
  %6582 = getelementptr inbounds nuw i8, ptr %6563, i64 6
  %6583 = lshr i64 %6564, 48
  %6584 = trunc i64 %6583 to i8
  store i8 %6584, ptr %6582, align 1
  %6585 = getelementptr inbounds nuw i8, ptr %6563, i64 7
  %6586 = lshr i64 %6564, 56
  %6587 = trunc nuw i64 %6586 to i8
  store i8 %6587, ptr %6585, align 1
  %6588 = getelementptr inbounds nuw i8, ptr %6563, i64 8
  %6589 = add i32 %6565, 8
  %6590 = icmp eq i32 %6589, %6561
  br i1 %6590, label %8745, label %6562, !llvm.loop !16

6591:                                             ; preds = %158
  %6592 = add nsw i32 %72, 2
  store i32 %6592, ptr @ip, align 4
  %6593 = sext i32 %73 to i64
  %6594 = getelementptr inbounds i8, ptr %69, i64 %6593
  %6595 = load i8, ptr %6594, align 1
  %6596 = shl i32 %81, 13
  %6597 = xor i32 %6596, %81
  %6598 = lshr i32 %6597, 17
  %6599 = xor i32 %6598, %6597
  %6600 = shl i32 %6599, 5
  %6601 = xor i32 %6600, %6599
  %6602 = trunc i32 %6601 to i8
  %6603 = xor i8 %6595, %6602
  %6604 = add nsw i32 %72, 3
  %6605 = shl i32 %6601, 13
  %6606 = xor i32 %6605, %6601
  %6607 = lshr i32 %6606, 17
  %6608 = xor i32 %6607, %6606
  %6609 = shl i32 %6608, 5
  %6610 = xor i32 %6609, %6608
  store i32 %6610, ptr @vm_code_state, align 4
  %6611 = load i32, ptr @pointer_size, align 4
  %6612 = icmp sgt i32 %6611, 0
  br i1 %6612, label %6613, label %6639

6613:                                             ; preds = %6591
  %6614 = sext i32 %6604 to i64
  %6615 = zext nneg i32 %6611 to i64
  br label %6616

6616:                                             ; preds = %6613, %6616
  %6617 = phi i64 [ %6614, %6613 ], [ %6621, %6616 ]
  %6618 = phi i64 [ 0, %6613 ], [ %6637, %6616 ]
  %6619 = phi i64 [ 0, %6613 ], [ %6636, %6616 ]
  %6620 = phi i32 [ %6610, %6613 ], [ %6630, %6616 ]
  %6621 = add nsw i64 %6617, 1
  %6622 = trunc nsw i64 %6621 to i32
  store i32 %6622, ptr @ip, align 4
  %6623 = getelementptr inbounds i8, ptr %69, i64 %6617
  %6624 = load i8, ptr %6623, align 1
  %6625 = shl i32 %6620, 13
  %6626 = xor i32 %6625, %6620
  %6627 = lshr i32 %6626, 17
  %6628 = xor i32 %6627, %6626
  %6629 = shl i32 %6628, 5
  %6630 = xor i32 %6629, %6628
  store i32 %6630, ptr @vm_code_state, align 4
  %6631 = trunc i32 %6630 to i8
  %6632 = xor i8 %6624, %6631
  %6633 = zext i8 %6632 to i64
  %6634 = shl nsw i64 %6618, 3
  %6635 = shl i64 %6633, %6634
  %6636 = or i64 %6635, %6619
  %6637 = add nuw nsw i64 %6618, 1
  %6638 = icmp eq i64 %6637, %6615
  br i1 %6638, label %6639, label %6616, !llvm.loop !15

6639:                                             ; preds = %6616, %6591
  %6640 = phi i32 [ %6610, %6591 ], [ %6630, %6616 ]
  %6641 = phi i32 [ %6604, %6591 ], [ %6622, %6616 ]
  %6642 = phi i64 [ 0, %6591 ], [ %6636, %6616 ]
  %6643 = add nsw i32 %6641, 1
  store i32 %6643, ptr @ip, align 4
  %6644 = sext i32 %6641 to i64
  %6645 = getelementptr inbounds i8, ptr %69, i64 %6644
  %6646 = load i8, ptr %6645, align 1
  %6647 = shl i32 %6640, 13
  %6648 = xor i32 %6647, %6640
  %6649 = lshr i32 %6648, 17
  %6650 = xor i32 %6649, %6648
  %6651 = shl i32 %6650, 5
  %6652 = xor i32 %6651, %6650
  store i32 %6652, ptr @vm_code_state, align 4
  %6653 = trunc i32 %6652 to i8
  %6654 = xor i8 %6646, %6653
  %6655 = add nsw i32 %6641, 2
  store i32 %6655, ptr @ip, align 4
  %6656 = sext i32 %6643 to i64
  %6657 = getelementptr inbounds i8, ptr %69, i64 %6656
  %6658 = load i8, ptr %6657, align 1
  %6659 = shl i32 %6652, 13
  %6660 = xor i32 %6659, %6652
  %6661 = lshr i32 %6660, 17
  %6662 = xor i32 %6661, %6660
  %6663 = shl i32 %6662, 5
  %6664 = xor i32 %6663, %6662
  store i32 %6664, ptr @vm_code_state, align 4
  %6665 = trunc i32 %6664 to i8
  %6666 = icmp eq i8 %6658, %6665
  br i1 %6666, label %6667, label %6746

6667:                                             ; preds = %6639
  br i1 %6612, label %6668, label %6694

6668:                                             ; preds = %6667
  %6669 = sext i32 %6655 to i64
  %6670 = zext nneg i32 %6611 to i64
  br label %6671

6671:                                             ; preds = %6668, %6671
  %6672 = phi i64 [ %6669, %6668 ], [ %6676, %6671 ]
  %6673 = phi i64 [ 0, %6668 ], [ %6692, %6671 ]
  %6674 = phi i64 [ 0, %6668 ], [ %6691, %6671 ]
  %6675 = phi i32 [ %6664, %6668 ], [ %6685, %6671 ]
  %6676 = add nsw i64 %6672, 1
  %6677 = trunc nsw i64 %6676 to i32
  store i32 %6677, ptr @ip, align 4
  %6678 = getelementptr inbounds i8, ptr %69, i64 %6672
  %6679 = load i8, ptr %6678, align 1
  %6680 = shl i32 %6675, 13
  %6681 = xor i32 %6680, %6675
  %6682 = lshr i32 %6681, 17
  %6683 = xor i32 %6682, %6681
  %6684 = shl i32 %6683, 5
  %6685 = xor i32 %6684, %6683
  store i32 %6685, ptr @vm_code_state, align 4
  %6686 = trunc i32 %6685 to i8
  %6687 = xor i8 %6679, %6686
  %6688 = zext i8 %6687 to i64
  %6689 = shl nsw i64 %6673, 3
  %6690 = shl i64 %6688, %6689
  %6691 = or i64 %6690, %6674
  %6692 = add nuw nsw i64 %6673, 1
  %6693 = icmp eq i64 %6692, %6670
  br i1 %6693, label %6694, label %6671, !llvm.loop !15

6694:                                             ; preds = %6671, %6667
  %6695 = phi i32 [ %6664, %6667 ], [ %6685, %6671 ]
  %6696 = phi i32 [ %6655, %6667 ], [ %6677, %6671 ]
  %6697 = phi i64 [ 0, %6667 ], [ %6691, %6671 ]
  %6698 = icmp eq i8 %6646, %6653
  br i1 %6698, label %6795, label %6699

6699:                                             ; preds = %6694
  %6700 = load i64, ptr @data_seg_addr, align 8
  %6701 = inttoptr i64 %6700 to ptr
  %6702 = zext i8 %6654 to i64
  %6703 = and i64 %6702, 3
  %6704 = icmp ult i8 %6654, 4
  br i1 %6704, label %6774, label %6705

6705:                                             ; preds = %6699
  %6706 = and i64 %6702, 252
  %6707 = getelementptr i8, ptr %6701, i64 1
  %6708 = getelementptr i8, ptr %6701, i64 2
  %6709 = getelementptr i8, ptr %6701, i64 3
  br label %6710

6710:                                             ; preds = %6710, %6705
  %6711 = phi i64 [ 0, %6705 ], [ %6743, %6710 ]
  %6712 = phi i64 [ 0, %6705 ], [ %6742, %6710 ]
  %6713 = phi i64 [ %6697, %6705 ], [ %6735, %6710 ]
  %6714 = phi i64 [ 0, %6705 ], [ %6744, %6710 ]
  %6715 = getelementptr inbounds nuw i8, ptr %6701, i64 %6713
  %6716 = load i8, ptr %6715, align 1
  %6717 = zext i8 %6716 to i64
  %6718 = shl nsw i64 %6711, 3
  %6719 = shl i64 %6717, %6718
  %6720 = or i64 %6719, %6712
  %6721 = getelementptr i8, ptr %6707, i64 %6713
  %6722 = load i8, ptr %6721, align 1
  %6723 = zext i8 %6722 to i64
  %6724 = shl i64 %6711, 3
  %6725 = or disjoint i64 %6724, 8
  %6726 = shl i64 %6723, %6725
  %6727 = or i64 %6726, %6720
  %6728 = getelementptr i8, ptr %6708, i64 %6713
  %6729 = load i8, ptr %6728, align 1
  %6730 = zext i8 %6729 to i64
  %6731 = shl i64 %6711, 3
  %6732 = or disjoint i64 %6731, 16
  %6733 = shl i64 %6730, %6732
  %6734 = or i64 %6733, %6727
  %6735 = add i64 %6713, 4
  %6736 = getelementptr i8, ptr %6709, i64 %6713
  %6737 = load i8, ptr %6736, align 1
  %6738 = zext i8 %6737 to i64
  %6739 = shl i64 %6711, 3
  %6740 = or disjoint i64 %6739, 24
  %6741 = shl i64 %6738, %6740
  %6742 = or i64 %6741, %6734
  %6743 = add nuw nsw i64 %6711, 4
  %6744 = add i64 %6714, 4
  %6745 = icmp eq i64 %6744, %6706
  br i1 %6745, label %6774, label %6710, !llvm.loop !17

6746:                                             ; preds = %6639
  %6747 = icmp eq i8 %6646, %6653
  br i1 %6747, label %6795, label %6748

6748:                                             ; preds = %6746
  %6749 = sext i32 %6655 to i64
  %6750 = zext i8 %6654 to i64
  br label %6751

6751:                                             ; preds = %6748, %6751
  %6752 = phi i64 [ %6749, %6748 ], [ %6756, %6751 ]
  %6753 = phi i64 [ 0, %6748 ], [ %6772, %6751 ]
  %6754 = phi i64 [ 0, %6748 ], [ %6771, %6751 ]
  %6755 = phi i32 [ %6664, %6748 ], [ %6765, %6751 ]
  %6756 = add nsw i64 %6752, 1
  %6757 = trunc nsw i64 %6756 to i32
  store i32 %6757, ptr @ip, align 4
  %6758 = getelementptr inbounds i8, ptr %69, i64 %6752
  %6759 = load i8, ptr %6758, align 1
  %6760 = shl i32 %6755, 13
  %6761 = xor i32 %6760, %6755
  %6762 = lshr i32 %6761, 17
  %6763 = xor i32 %6762, %6761
  %6764 = shl i32 %6763, 5
  %6765 = xor i32 %6764, %6763
  store i32 %6765, ptr @vm_code_state, align 4
  %6766 = trunc i32 %6765 to i8
  %6767 = xor i8 %6759, %6766
  %6768 = zext i8 %6767 to i64
  %6769 = shl nsw i64 %6753, 3
  %6770 = shl i64 %6768, %6769
  %6771 = or i64 %6770, %6754
  %6772 = add nuw nsw i64 %6753, 1
  %6773 = icmp eq i64 %6772, %6750
  br i1 %6773, label %6795, label %6751, !llvm.loop !15

6774:                                             ; preds = %6710, %6699
  %6775 = phi i64 [ poison, %6699 ], [ %6742, %6710 ]
  %6776 = phi i64 [ 0, %6699 ], [ %6743, %6710 ]
  %6777 = phi i64 [ 0, %6699 ], [ %6742, %6710 ]
  %6778 = phi i64 [ %6697, %6699 ], [ %6735, %6710 ]
  %6779 = icmp eq i64 %6703, 0
  br i1 %6779, label %6795, label %6780

6780:                                             ; preds = %6774, %6780
  %6781 = phi i64 [ %6792, %6780 ], [ %6776, %6774 ]
  %6782 = phi i64 [ %6791, %6780 ], [ %6777, %6774 ]
  %6783 = phi i64 [ %6785, %6780 ], [ %6778, %6774 ]
  %6784 = phi i64 [ %6793, %6780 ], [ 0, %6774 ]
  %6785 = add i64 %6783, 1
  %6786 = getelementptr inbounds nuw i8, ptr %6701, i64 %6783
  %6787 = load i8, ptr %6786, align 1
  %6788 = zext i8 %6787 to i64
  %6789 = shl nsw i64 %6781, 3
  %6790 = shl i64 %6788, %6789
  %6791 = or i64 %6790, %6782
  %6792 = add nuw nsw i64 %6781, 1
  %6793 = add i64 %6784, 1
  %6794 = icmp eq i64 %6793, %6703
  br i1 %6794, label %6795, label %6780, !llvm.loop !62

6795:                                             ; preds = %6751, %6774, %6780, %6746, %6694
  %6796 = phi i32 [ %6695, %6694 ], [ %6664, %6746 ], [ %6695, %6780 ], [ %6695, %6774 ], [ %6765, %6751 ]
  %6797 = phi i32 [ %6696, %6694 ], [ %6655, %6746 ], [ %6696, %6780 ], [ %6696, %6774 ], [ %6757, %6751 ]
  %6798 = phi i64 [ 0, %6694 ], [ 0, %6746 ], [ %6775, %6774 ], [ %6791, %6780 ], [ %6771, %6751 ]
  %6799 = add nsw i32 %6797, 1
  store i32 %6799, ptr @ip, align 4
  %6800 = sext i32 %6797 to i64
  %6801 = getelementptr inbounds i8, ptr %69, i64 %6800
  %6802 = load i8, ptr %6801, align 1
  %6803 = shl i32 %6796, 13
  %6804 = xor i32 %6803, %6796
  %6805 = lshr i32 %6804, 17
  %6806 = xor i32 %6805, %6804
  %6807 = shl i32 %6806, 5
  %6808 = xor i32 %6807, %6806
  store i32 %6808, ptr @vm_code_state, align 4
  %6809 = trunc i32 %6808 to i8
  %6810 = xor i8 %6802, %6809
  %6811 = add nsw i32 %6797, 2
  store i32 %6811, ptr @ip, align 4
  %6812 = sext i32 %6799 to i64
  %6813 = getelementptr inbounds i8, ptr %69, i64 %6812
  %6814 = load i8, ptr %6813, align 1
  %6815 = shl i32 %6808, 13
  %6816 = xor i32 %6815, %6808
  %6817 = lshr i32 %6816, 17
  %6818 = xor i32 %6817, %6816
  %6819 = shl i32 %6818, 5
  %6820 = xor i32 %6819, %6818
  store i32 %6820, ptr @vm_code_state, align 4
  %6821 = trunc i32 %6820 to i8
  %6822 = icmp eq i8 %6814, %6821
  br i1 %6822, label %6823, label %6902

6823:                                             ; preds = %6795
  br i1 %6612, label %6824, label %6850

6824:                                             ; preds = %6823
  %6825 = sext i32 %6811 to i64
  %6826 = zext nneg i32 %6611 to i64
  br label %6827

6827:                                             ; preds = %6824, %6827
  %6828 = phi i64 [ %6825, %6824 ], [ %6832, %6827 ]
  %6829 = phi i64 [ 0, %6824 ], [ %6848, %6827 ]
  %6830 = phi i64 [ 0, %6824 ], [ %6847, %6827 ]
  %6831 = phi i32 [ %6820, %6824 ], [ %6841, %6827 ]
  %6832 = add nsw i64 %6828, 1
  %6833 = trunc nsw i64 %6832 to i32
  store i32 %6833, ptr @ip, align 4
  %6834 = getelementptr inbounds i8, ptr %69, i64 %6828
  %6835 = load i8, ptr %6834, align 1
  %6836 = shl i32 %6831, 13
  %6837 = xor i32 %6836, %6831
  %6838 = lshr i32 %6837, 17
  %6839 = xor i32 %6838, %6837
  %6840 = shl i32 %6839, 5
  %6841 = xor i32 %6840, %6839
  store i32 %6841, ptr @vm_code_state, align 4
  %6842 = trunc i32 %6841 to i8
  %6843 = xor i8 %6835, %6842
  %6844 = zext i8 %6843 to i64
  %6845 = shl nsw i64 %6829, 3
  %6846 = shl i64 %6844, %6845
  %6847 = or i64 %6846, %6830
  %6848 = add nuw nsw i64 %6829, 1
  %6849 = icmp eq i64 %6848, %6826
  br i1 %6849, label %6850, label %6827, !llvm.loop !15

6850:                                             ; preds = %6827, %6823
  %6851 = phi i32 [ %6820, %6823 ], [ %6841, %6827 ]
  %6852 = phi i32 [ %6811, %6823 ], [ %6833, %6827 ]
  %6853 = phi i64 [ 0, %6823 ], [ %6847, %6827 ]
  %6854 = icmp eq i8 %6802, %6809
  br i1 %6854, label %6951, label %6855

6855:                                             ; preds = %6850
  %6856 = load i64, ptr @data_seg_addr, align 8
  %6857 = inttoptr i64 %6856 to ptr
  %6858 = zext i8 %6810 to i64
  %6859 = and i64 %6858, 3
  %6860 = icmp ult i8 %6810, 4
  br i1 %6860, label %6930, label %6861

6861:                                             ; preds = %6855
  %6862 = and i64 %6858, 252
  %6863 = getelementptr i8, ptr %6857, i64 1
  %6864 = getelementptr i8, ptr %6857, i64 2
  %6865 = getelementptr i8, ptr %6857, i64 3
  br label %6866

6866:                                             ; preds = %6866, %6861
  %6867 = phi i64 [ 0, %6861 ], [ %6899, %6866 ]
  %6868 = phi i64 [ 0, %6861 ], [ %6898, %6866 ]
  %6869 = phi i64 [ %6853, %6861 ], [ %6891, %6866 ]
  %6870 = phi i64 [ 0, %6861 ], [ %6900, %6866 ]
  %6871 = getelementptr inbounds nuw i8, ptr %6857, i64 %6869
  %6872 = load i8, ptr %6871, align 1
  %6873 = zext i8 %6872 to i64
  %6874 = shl nsw i64 %6867, 3
  %6875 = shl i64 %6873, %6874
  %6876 = or i64 %6875, %6868
  %6877 = getelementptr i8, ptr %6863, i64 %6869
  %6878 = load i8, ptr %6877, align 1
  %6879 = zext i8 %6878 to i64
  %6880 = shl i64 %6867, 3
  %6881 = or disjoint i64 %6880, 8
  %6882 = shl i64 %6879, %6881
  %6883 = or i64 %6882, %6876
  %6884 = getelementptr i8, ptr %6864, i64 %6869
  %6885 = load i8, ptr %6884, align 1
  %6886 = zext i8 %6885 to i64
  %6887 = shl i64 %6867, 3
  %6888 = or disjoint i64 %6887, 16
  %6889 = shl i64 %6886, %6888
  %6890 = or i64 %6889, %6883
  %6891 = add i64 %6869, 4
  %6892 = getelementptr i8, ptr %6865, i64 %6869
  %6893 = load i8, ptr %6892, align 1
  %6894 = zext i8 %6893 to i64
  %6895 = shl i64 %6867, 3
  %6896 = or disjoint i64 %6895, 24
  %6897 = shl i64 %6894, %6896
  %6898 = or i64 %6897, %6890
  %6899 = add nuw nsw i64 %6867, 4
  %6900 = add i64 %6870, 4
  %6901 = icmp eq i64 %6900, %6862
  br i1 %6901, label %6930, label %6866, !llvm.loop !17

6902:                                             ; preds = %6795
  %6903 = icmp eq i8 %6802, %6809
  br i1 %6903, label %6951, label %6904

6904:                                             ; preds = %6902
  %6905 = sext i32 %6811 to i64
  %6906 = zext i8 %6810 to i64
  br label %6907

6907:                                             ; preds = %6904, %6907
  %6908 = phi i64 [ %6905, %6904 ], [ %6912, %6907 ]
  %6909 = phi i64 [ 0, %6904 ], [ %6928, %6907 ]
  %6910 = phi i64 [ 0, %6904 ], [ %6927, %6907 ]
  %6911 = phi i32 [ %6820, %6904 ], [ %6921, %6907 ]
  %6912 = add nsw i64 %6908, 1
  %6913 = trunc nsw i64 %6912 to i32
  store i32 %6913, ptr @ip, align 4
  %6914 = getelementptr inbounds i8, ptr %69, i64 %6908
  %6915 = load i8, ptr %6914, align 1
  %6916 = shl i32 %6911, 13
  %6917 = xor i32 %6916, %6911
  %6918 = lshr i32 %6917, 17
  %6919 = xor i32 %6918, %6917
  %6920 = shl i32 %6919, 5
  %6921 = xor i32 %6920, %6919
  store i32 %6921, ptr @vm_code_state, align 4
  %6922 = trunc i32 %6921 to i8
  %6923 = xor i8 %6915, %6922
  %6924 = zext i8 %6923 to i64
  %6925 = shl nsw i64 %6909, 3
  %6926 = shl i64 %6924, %6925
  %6927 = or i64 %6926, %6910
  %6928 = add nuw nsw i64 %6909, 1
  %6929 = icmp eq i64 %6928, %6906
  br i1 %6929, label %6951, label %6907, !llvm.loop !15

6930:                                             ; preds = %6866, %6855
  %6931 = phi i64 [ poison, %6855 ], [ %6898, %6866 ]
  %6932 = phi i64 [ 0, %6855 ], [ %6899, %6866 ]
  %6933 = phi i64 [ 0, %6855 ], [ %6898, %6866 ]
  %6934 = phi i64 [ %6853, %6855 ], [ %6891, %6866 ]
  %6935 = icmp eq i64 %6859, 0
  br i1 %6935, label %6951, label %6936

6936:                                             ; preds = %6930, %6936
  %6937 = phi i64 [ %6948, %6936 ], [ %6932, %6930 ]
  %6938 = phi i64 [ %6947, %6936 ], [ %6933, %6930 ]
  %6939 = phi i64 [ %6941, %6936 ], [ %6934, %6930 ]
  %6940 = phi i64 [ %6949, %6936 ], [ 0, %6930 ]
  %6941 = add i64 %6939, 1
  %6942 = getelementptr inbounds nuw i8, ptr %6857, i64 %6939
  %6943 = load i8, ptr %6942, align 1
  %6944 = zext i8 %6943 to i64
  %6945 = shl nsw i64 %6937, 3
  %6946 = shl i64 %6944, %6945
  %6947 = or i64 %6946, %6938
  %6948 = add nuw nsw i64 %6937, 1
  %6949 = add i64 %6940, 1
  %6950 = icmp eq i64 %6949, %6859
  br i1 %6950, label %6951, label %6936, !llvm.loop !63

6951:                                             ; preds = %6907, %6930, %6936, %6902, %6850
  %6952 = phi i32 [ %6851, %6850 ], [ %6820, %6902 ], [ %6851, %6936 ], [ %6851, %6930 ], [ %6921, %6907 ]
  %6953 = phi i32 [ %6852, %6850 ], [ %6811, %6902 ], [ %6852, %6936 ], [ %6852, %6930 ], [ %6913, %6907 ]
  %6954 = phi i64 [ 0, %6850 ], [ 0, %6902 ], [ %6931, %6930 ], [ %6947, %6936 ], [ %6927, %6907 ]
  %6955 = add nsw i32 %6953, 1
  store i32 %6955, ptr @ip, align 4
  %6956 = sext i32 %6953 to i64
  %6957 = getelementptr inbounds i8, ptr %69, i64 %6956
  %6958 = load i8, ptr %6957, align 1
  %6959 = shl i32 %6952, 13
  %6960 = xor i32 %6959, %6952
  %6961 = lshr i32 %6960, 17
  %6962 = xor i32 %6961, %6960
  %6963 = shl i32 %6962, 5
  %6964 = xor i32 %6963, %6962
  store i32 %6964, ptr @vm_code_state, align 4
  %6965 = trunc i32 %6964 to i8
  %6966 = xor i8 %6958, %6965
  %6967 = add nsw i32 %6953, 2
  store i32 %6967, ptr @ip, align 4
  %6968 = sext i32 %6955 to i64
  %6969 = getelementptr inbounds i8, ptr %69, i64 %6968
  %6970 = load i8, ptr %6969, align 1
  %6971 = shl i32 %6964, 13
  %6972 = xor i32 %6971, %6964
  %6973 = lshr i32 %6972, 17
  %6974 = xor i32 %6973, %6972
  %6975 = shl i32 %6974, 5
  %6976 = xor i32 %6975, %6974
  store i32 %6976, ptr @vm_code_state, align 4
  %6977 = trunc i32 %6976 to i8
  %6978 = icmp eq i8 %6970, %6977
  br i1 %6978, label %6979, label %7058

6979:                                             ; preds = %6951
  br i1 %6612, label %6980, label %7006

6980:                                             ; preds = %6979
  %6981 = sext i32 %6967 to i64
  %6982 = zext nneg i32 %6611 to i64
  br label %6983

6983:                                             ; preds = %6980, %6983
  %6984 = phi i64 [ %6981, %6980 ], [ %6988, %6983 ]
  %6985 = phi i64 [ 0, %6980 ], [ %7004, %6983 ]
  %6986 = phi i64 [ 0, %6980 ], [ %7003, %6983 ]
  %6987 = phi i32 [ %6976, %6980 ], [ %6997, %6983 ]
  %6988 = add nsw i64 %6984, 1
  %6989 = trunc nsw i64 %6988 to i32
  store i32 %6989, ptr @ip, align 4
  %6990 = getelementptr inbounds i8, ptr %69, i64 %6984
  %6991 = load i8, ptr %6990, align 1
  %6992 = shl i32 %6987, 13
  %6993 = xor i32 %6992, %6987
  %6994 = lshr i32 %6993, 17
  %6995 = xor i32 %6994, %6993
  %6996 = shl i32 %6995, 5
  %6997 = xor i32 %6996, %6995
  store i32 %6997, ptr @vm_code_state, align 4
  %6998 = trunc i32 %6997 to i8
  %6999 = xor i8 %6991, %6998
  %7000 = zext i8 %6999 to i64
  %7001 = shl nsw i64 %6985, 3
  %7002 = shl i64 %7000, %7001
  %7003 = or i64 %7002, %6986
  %7004 = add nuw nsw i64 %6985, 1
  %7005 = icmp eq i64 %7004, %6982
  br i1 %7005, label %7006, label %6983, !llvm.loop !15

7006:                                             ; preds = %6983, %6979
  %7007 = phi i32 [ %6976, %6979 ], [ %6997, %6983 ]
  %7008 = phi i32 [ %6967, %6979 ], [ %6989, %6983 ]
  %7009 = phi i64 [ 0, %6979 ], [ %7003, %6983 ]
  %7010 = icmp eq i8 %6958, %6965
  br i1 %7010, label %7107, label %7011

7011:                                             ; preds = %7006
  %7012 = load i64, ptr @data_seg_addr, align 8
  %7013 = inttoptr i64 %7012 to ptr
  %7014 = zext i8 %6966 to i64
  %7015 = and i64 %7014, 3
  %7016 = icmp ult i8 %6966, 4
  br i1 %7016, label %7086, label %7017

7017:                                             ; preds = %7011
  %7018 = and i64 %7014, 252
  %7019 = getelementptr i8, ptr %7013, i64 1
  %7020 = getelementptr i8, ptr %7013, i64 2
  %7021 = getelementptr i8, ptr %7013, i64 3
  br label %7022

7022:                                             ; preds = %7022, %7017
  %7023 = phi i64 [ 0, %7017 ], [ %7055, %7022 ]
  %7024 = phi i64 [ 0, %7017 ], [ %7054, %7022 ]
  %7025 = phi i64 [ %7009, %7017 ], [ %7047, %7022 ]
  %7026 = phi i64 [ 0, %7017 ], [ %7056, %7022 ]
  %7027 = getelementptr inbounds nuw i8, ptr %7013, i64 %7025
  %7028 = load i8, ptr %7027, align 1
  %7029 = zext i8 %7028 to i64
  %7030 = shl nsw i64 %7023, 3
  %7031 = shl i64 %7029, %7030
  %7032 = or i64 %7031, %7024
  %7033 = getelementptr i8, ptr %7019, i64 %7025
  %7034 = load i8, ptr %7033, align 1
  %7035 = zext i8 %7034 to i64
  %7036 = shl i64 %7023, 3
  %7037 = or disjoint i64 %7036, 8
  %7038 = shl i64 %7035, %7037
  %7039 = or i64 %7038, %7032
  %7040 = getelementptr i8, ptr %7020, i64 %7025
  %7041 = load i8, ptr %7040, align 1
  %7042 = zext i8 %7041 to i64
  %7043 = shl i64 %7023, 3
  %7044 = or disjoint i64 %7043, 16
  %7045 = shl i64 %7042, %7044
  %7046 = or i64 %7045, %7039
  %7047 = add i64 %7025, 4
  %7048 = getelementptr i8, ptr %7021, i64 %7025
  %7049 = load i8, ptr %7048, align 1
  %7050 = zext i8 %7049 to i64
  %7051 = shl i64 %7023, 3
  %7052 = or disjoint i64 %7051, 24
  %7053 = shl i64 %7050, %7052
  %7054 = or i64 %7053, %7046
  %7055 = add nuw nsw i64 %7023, 4
  %7056 = add i64 %7026, 4
  %7057 = icmp eq i64 %7056, %7018
  br i1 %7057, label %7086, label %7022, !llvm.loop !17

7058:                                             ; preds = %6951
  %7059 = icmp eq i8 %6958, %6965
  br i1 %7059, label %7107, label %7060

7060:                                             ; preds = %7058
  %7061 = sext i32 %6967 to i64
  %7062 = zext i8 %6966 to i64
  br label %7063

7063:                                             ; preds = %7060, %7063
  %7064 = phi i64 [ %7061, %7060 ], [ %7068, %7063 ]
  %7065 = phi i64 [ 0, %7060 ], [ %7084, %7063 ]
  %7066 = phi i64 [ 0, %7060 ], [ %7083, %7063 ]
  %7067 = phi i32 [ %6976, %7060 ], [ %7077, %7063 ]
  %7068 = add nsw i64 %7064, 1
  %7069 = trunc nsw i64 %7068 to i32
  store i32 %7069, ptr @ip, align 4
  %7070 = getelementptr inbounds i8, ptr %69, i64 %7064
  %7071 = load i8, ptr %7070, align 1
  %7072 = shl i32 %7067, 13
  %7073 = xor i32 %7072, %7067
  %7074 = lshr i32 %7073, 17
  %7075 = xor i32 %7074, %7073
  %7076 = shl i32 %7075, 5
  %7077 = xor i32 %7076, %7075
  store i32 %7077, ptr @vm_code_state, align 4
  %7078 = trunc i32 %7077 to i8
  %7079 = xor i8 %7071, %7078
  %7080 = zext i8 %7079 to i64
  %7081 = shl nsw i64 %7065, 3
  %7082 = shl i64 %7080, %7081
  %7083 = or i64 %7082, %7066
  %7084 = add nuw nsw i64 %7065, 1
  %7085 = icmp eq i64 %7084, %7062
  br i1 %7085, label %7107, label %7063, !llvm.loop !15

7086:                                             ; preds = %7022, %7011
  %7087 = phi i64 [ poison, %7011 ], [ %7054, %7022 ]
  %7088 = phi i64 [ 0, %7011 ], [ %7055, %7022 ]
  %7089 = phi i64 [ 0, %7011 ], [ %7054, %7022 ]
  %7090 = phi i64 [ %7009, %7011 ], [ %7047, %7022 ]
  %7091 = icmp eq i64 %7015, 0
  br i1 %7091, label %7107, label %7092

7092:                                             ; preds = %7086, %7092
  %7093 = phi i64 [ %7104, %7092 ], [ %7088, %7086 ]
  %7094 = phi i64 [ %7103, %7092 ], [ %7089, %7086 ]
  %7095 = phi i64 [ %7097, %7092 ], [ %7090, %7086 ]
  %7096 = phi i64 [ %7105, %7092 ], [ 0, %7086 ]
  %7097 = add i64 %7095, 1
  %7098 = getelementptr inbounds nuw i8, ptr %7013, i64 %7095
  %7099 = load i8, ptr %7098, align 1
  %7100 = zext i8 %7099 to i64
  %7101 = shl nsw i64 %7093, 3
  %7102 = shl i64 %7100, %7101
  %7103 = or i64 %7102, %7094
  %7104 = add nuw nsw i64 %7093, 1
  %7105 = add i64 %7096, 1
  %7106 = icmp eq i64 %7105, %7015
  br i1 %7106, label %7107, label %7092, !llvm.loop !64

7107:                                             ; preds = %7063, %7086, %7092, %7058, %7006
  %7108 = phi i32 [ %7007, %7006 ], [ %6976, %7058 ], [ %7007, %7092 ], [ %7007, %7086 ], [ %7077, %7063 ]
  %7109 = phi i32 [ %7008, %7006 ], [ %6967, %7058 ], [ %7008, %7092 ], [ %7008, %7086 ], [ %7069, %7063 ]
  %7110 = phi i64 [ 0, %7006 ], [ 0, %7058 ], [ %7087, %7086 ], [ %7103, %7092 ], [ %7083, %7063 ]
  %7111 = sext i32 %7109 to i64
  %7112 = add nsw i64 %7111, 1
  %7113 = trunc nsw i64 %7112 to i32
  store i32 %7113, ptr @ip, align 4
  %7114 = getelementptr inbounds i8, ptr %69, i64 %7111
  %7115 = load i8, ptr %7114, align 1
  %7116 = shl i32 %7108, 13
  %7117 = xor i32 %7116, %7108
  %7118 = lshr i32 %7117, 17
  %7119 = xor i32 %7118, %7117
  %7120 = shl i32 %7119, 5
  %7121 = xor i32 %7120, %7119
  store i32 %7121, ptr @vm_code_state, align 4
  %7122 = trunc i32 %7121 to i8
  %7123 = xor i8 %7115, %7122
  %7124 = zext i8 %7123 to i64
  %7125 = add nsw i64 %7111, 2
  %7126 = trunc nsw i64 %7125 to i32
  store i32 %7126, ptr @ip, align 4
  %7127 = getelementptr inbounds i8, ptr %69, i64 %7112
  %7128 = load i8, ptr %7127, align 1
  %7129 = shl i32 %7121, 13
  %7130 = xor i32 %7129, %7121
  %7131 = lshr i32 %7130, 17
  %7132 = xor i32 %7131, %7130
  %7133 = shl i32 %7132, 5
  %7134 = xor i32 %7133, %7132
  store i32 %7134, ptr @vm_code_state, align 4
  %7135 = trunc i32 %7134 to i8
  %7136 = xor i8 %7128, %7135
  %7137 = zext i8 %7136 to i64
  %7138 = shl nuw nsw i64 %7137, 8
  %7139 = or disjoint i64 %7138, %7124
  %7140 = add nsw i64 %7111, 3
  %7141 = trunc nsw i64 %7140 to i32
  store i32 %7141, ptr @ip, align 4
  %7142 = getelementptr inbounds i8, ptr %69, i64 %7125
  %7143 = load i8, ptr %7142, align 1
  %7144 = shl i32 %7134, 13
  %7145 = xor i32 %7144, %7134
  %7146 = lshr i32 %7145, 17
  %7147 = xor i32 %7146, %7145
  %7148 = shl i32 %7147, 5
  %7149 = xor i32 %7148, %7147
  store i32 %7149, ptr @vm_code_state, align 4
  %7150 = trunc i32 %7149 to i8
  %7151 = xor i8 %7143, %7150
  %7152 = zext i8 %7151 to i64
  %7153 = shl nuw nsw i64 %7152, 16
  %7154 = or disjoint i64 %7153, %7139
  %7155 = add i32 %7109, 4
  store i32 %7155, ptr @ip, align 4
  %7156 = getelementptr inbounds i8, ptr %69, i64 %7140
  %7157 = load i8, ptr %7156, align 1
  %7158 = shl i32 %7149, 13
  %7159 = xor i32 %7158, %7149
  %7160 = lshr i32 %7159, 17
  %7161 = xor i32 %7160, %7159
  %7162 = shl i32 %7161, 5
  %7163 = xor i32 %7162, %7161
  store i32 %7163, ptr @vm_code_state, align 4
  %7164 = trunc i32 %7163 to i8
  %7165 = xor i8 %7157, %7164
  %7166 = zext i8 %7165 to i64
  %7167 = shl nuw nsw i64 %7166, 24
  %7168 = or disjoint i64 %7167, %7154
  %7169 = load i64, ptr @data_seg_addr, align 8
  %7170 = add i64 %7169, %6642
  %7171 = icmp eq i8 %6595, %6602
  br i1 %7171, label %7270, label %7172

7172:                                             ; preds = %7107
  %7173 = inttoptr i64 %6798 to ptr
  %7174 = inttoptr i64 %7170 to ptr
  %7175 = zext i8 %6603 to i64
  %7176 = icmp ult i8 %6603, 4
  %7177 = sub i64 %7170, %6798
  %7178 = icmp ult i64 %7177, 32
  %7179 = select i1 %7176, i1 true, i1 %7178
  br i1 %7179, label %7180, label %7197

7180:                                             ; preds = %7255, %7268, %7172
  %7181 = phi i64 [ 0, %7172 ], [ %7200, %7255 ], [ %7260, %7268 ]
  %7182 = and i64 %7175, 3
  %7183 = icmp eq i64 %7182, 0
  br i1 %7183, label %7193, label %7184

7184:                                             ; preds = %7180, %7184
  %7185 = phi i64 [ %7190, %7184 ], [ %7181, %7180 ]
  %7186 = phi i64 [ %7191, %7184 ], [ 0, %7180 ]
  %7187 = getelementptr inbounds nuw i8, ptr %7173, i64 %7185
  %7188 = load i8, ptr %7187, align 1
  %7189 = getelementptr inbounds nuw i8, ptr %7174, i64 %7185
  store i8 %7188, ptr %7189, align 1
  %7190 = add nuw nsw i64 %7185, 1
  %7191 = add i64 %7186, 1
  %7192 = icmp eq i64 %7191, %7182
  br i1 %7192, label %7193, label %7184, !llvm.loop !65

7193:                                             ; preds = %7184, %7180
  %7194 = phi i64 [ %7181, %7180 ], [ %7190, %7184 ]
  %7195 = sub nsw i64 %7181, %7175
  %7196 = icmp ugt i64 %7195, -4
  br i1 %7196, label %7270, label %7321

7197:                                             ; preds = %7172
  %7198 = icmp ult i8 %6603, 32
  br i1 %7198, label %7258, label %7199

7199:                                             ; preds = %7197
  %7200 = and i64 %7175, 224
  %7201 = getelementptr inbounds nuw i8, ptr %7173, i64 16
  %7202 = load <16 x i8>, ptr %7173, align 1
  %7203 = load <16 x i8>, ptr %7201, align 1
  %7204 = getelementptr inbounds nuw i8, ptr %7174, i64 16
  store <16 x i8> %7202, ptr %7174, align 1
  store <16 x i8> %7203, ptr %7204, align 1
  %7205 = icmp eq i64 %7200, 32
  br i1 %7205, label %7253, label %7206, !llvm.loop !66

7206:                                             ; preds = %7199
  %7207 = getelementptr inbounds nuw i8, ptr %7173, i64 32
  %7208 = getelementptr inbounds nuw i8, ptr %7173, i64 48
  %7209 = load <16 x i8>, ptr %7207, align 1
  %7210 = load <16 x i8>, ptr %7208, align 1
  %7211 = getelementptr inbounds nuw i8, ptr %7174, i64 32
  %7212 = getelementptr inbounds nuw i8, ptr %7174, i64 48
  store <16 x i8> %7209, ptr %7211, align 1
  store <16 x i8> %7210, ptr %7212, align 1
  %7213 = icmp eq i64 %7200, 64
  br i1 %7213, label %7253, label %7214, !llvm.loop !66

7214:                                             ; preds = %7206
  %7215 = getelementptr inbounds nuw i8, ptr %7173, i64 64
  %7216 = getelementptr inbounds nuw i8, ptr %7173, i64 80
  %7217 = load <16 x i8>, ptr %7215, align 1
  %7218 = load <16 x i8>, ptr %7216, align 1
  %7219 = getelementptr inbounds nuw i8, ptr %7174, i64 64
  %7220 = getelementptr inbounds nuw i8, ptr %7174, i64 80
  store <16 x i8> %7217, ptr %7219, align 1
  store <16 x i8> %7218, ptr %7220, align 1
  %7221 = icmp eq i64 %7200, 96
  br i1 %7221, label %7253, label %7222, !llvm.loop !66

7222:                                             ; preds = %7214
  %7223 = getelementptr inbounds nuw i8, ptr %7173, i64 96
  %7224 = getelementptr inbounds nuw i8, ptr %7173, i64 112
  %7225 = load <16 x i8>, ptr %7223, align 1
  %7226 = load <16 x i8>, ptr %7224, align 1
  %7227 = getelementptr inbounds nuw i8, ptr %7174, i64 96
  %7228 = getelementptr inbounds nuw i8, ptr %7174, i64 112
  store <16 x i8> %7225, ptr %7227, align 1
  store <16 x i8> %7226, ptr %7228, align 1
  %7229 = icmp eq i64 %7200, 128
  br i1 %7229, label %7253, label %7230, !llvm.loop !66

7230:                                             ; preds = %7222
  %7231 = getelementptr inbounds nuw i8, ptr %7173, i64 128
  %7232 = getelementptr inbounds nuw i8, ptr %7173, i64 144
  %7233 = load <16 x i8>, ptr %7231, align 1
  %7234 = load <16 x i8>, ptr %7232, align 1
  %7235 = getelementptr inbounds nuw i8, ptr %7174, i64 128
  %7236 = getelementptr inbounds nuw i8, ptr %7174, i64 144
  store <16 x i8> %7233, ptr %7235, align 1
  store <16 x i8> %7234, ptr %7236, align 1
  %7237 = icmp eq i64 %7200, 160
  br i1 %7237, label %7253, label %7238, !llvm.loop !66

7238:                                             ; preds = %7230
  %7239 = getelementptr inbounds nuw i8, ptr %7173, i64 160
  %7240 = getelementptr inbounds nuw i8, ptr %7173, i64 176
  %7241 = load <16 x i8>, ptr %7239, align 1
  %7242 = load <16 x i8>, ptr %7240, align 1
  %7243 = getelementptr inbounds nuw i8, ptr %7174, i64 160
  %7244 = getelementptr inbounds nuw i8, ptr %7174, i64 176
  store <16 x i8> %7241, ptr %7243, align 1
  store <16 x i8> %7242, ptr %7244, align 1
  %7245 = icmp eq i64 %7200, 192
  br i1 %7245, label %7253, label %7246, !llvm.loop !66

7246:                                             ; preds = %7238
  %7247 = getelementptr inbounds nuw i8, ptr %7173, i64 192
  %7248 = getelementptr inbounds nuw i8, ptr %7173, i64 208
  %7249 = load <16 x i8>, ptr %7247, align 1
  %7250 = load <16 x i8>, ptr %7248, align 1
  %7251 = getelementptr inbounds nuw i8, ptr %7174, i64 192
  %7252 = getelementptr inbounds nuw i8, ptr %7174, i64 208
  store <16 x i8> %7249, ptr %7251, align 1
  store <16 x i8> %7250, ptr %7252, align 1
  br label %7253

7253:                                             ; preds = %7246, %7238, %7230, %7222, %7214, %7206, %7199
  %7254 = icmp eq i64 %7200, %7175
  br i1 %7254, label %7270, label %7255

7255:                                             ; preds = %7253
  %7256 = and i64 %7175, 28
  %7257 = icmp eq i64 %7256, 0
  br i1 %7257, label %7180, label %7258

7258:                                             ; preds = %7255, %7197
  %7259 = phi i64 [ %7200, %7255 ], [ 0, %7197 ]
  %7260 = and i64 %7175, 252
  br label %7261

7261:                                             ; preds = %7261, %7258
  %7262 = phi i64 [ %7259, %7258 ], [ %7266, %7261 ]
  %7263 = getelementptr inbounds nuw i8, ptr %7173, i64 %7262
  %7264 = load <4 x i8>, ptr %7263, align 1
  %7265 = getelementptr inbounds nuw i8, ptr %7174, i64 %7262
  store <4 x i8> %7264, ptr %7265, align 1
  %7266 = add nuw i64 %7262, 4
  %7267 = icmp eq i64 %7266, %7260
  br i1 %7267, label %7268, label %7261, !llvm.loop !67

7268:                                             ; preds = %7261
  %7269 = icmp eq i64 %7260, %7175
  br i1 %7269, label %7270, label %7180

7270:                                             ; preds = %7193, %7321, %7253, %7268, %7107
  %7271 = trunc nuw i64 %7168 to i32
  %7272 = icmp sgt i32 %7271, 0
  br i1 %7272, label %7273, label %8784

7273:                                             ; preds = %7270
  %7274 = mul i64 %7168, %7110
  %7275 = add i64 %7170, %7274
  %7276 = inttoptr i64 %7275 to ptr
  %7277 = zext i8 %7165 to i32
  %7278 = shl nuw i32 %7277, 24
  %7279 = add i32 %7278, -1
  %7280 = zext i8 %7151 to i32
  %7281 = shl nuw nsw i32 %7280, 16
  %7282 = add i32 %7279, %7281
  %7283 = zext i8 %7136 to i32
  %7284 = shl nuw nsw i32 %7283, 8
  %7285 = add i32 %7282, %7284
  %7286 = zext i8 %7123 to i32
  %7287 = add i32 %7285, %7286
  %7288 = and i32 %7271, 7
  %7289 = icmp ult i32 %7287, 7
  br i1 %7289, label %8758, label %7290

7290:                                             ; preds = %7273
  %7291 = and i32 %7271, 2147483640
  br label %7292

7292:                                             ; preds = %7292, %7290
  %7293 = phi ptr [ %7276, %7290 ], [ %7318, %7292 ]
  %7294 = phi i64 [ %6954, %7290 ], [ 0, %7292 ]
  %7295 = phi i32 [ 0, %7290 ], [ %7319, %7292 ]
  %7296 = trunc i64 %7294 to i8
  store i8 %7296, ptr %7293, align 1
  %7297 = getelementptr inbounds nuw i8, ptr %7293, i64 1
  %7298 = lshr i64 %7294, 8
  %7299 = trunc i64 %7298 to i8
  store i8 %7299, ptr %7297, align 1
  %7300 = getelementptr inbounds nuw i8, ptr %7293, i64 2
  %7301 = lshr i64 %7294, 16
  %7302 = trunc i64 %7301 to i8
  store i8 %7302, ptr %7300, align 1
  %7303 = getelementptr inbounds nuw i8, ptr %7293, i64 3
  %7304 = lshr i64 %7294, 24
  %7305 = trunc i64 %7304 to i8
  store i8 %7305, ptr %7303, align 1
  %7306 = getelementptr inbounds nuw i8, ptr %7293, i64 4
  %7307 = lshr i64 %7294, 32
  %7308 = trunc i64 %7307 to i8
  store i8 %7308, ptr %7306, align 1
  %7309 = getelementptr inbounds nuw i8, ptr %7293, i64 5
  %7310 = lshr i64 %7294, 40
  %7311 = trunc i64 %7310 to i8
  store i8 %7311, ptr %7309, align 1
  %7312 = getelementptr inbounds nuw i8, ptr %7293, i64 6
  %7313 = lshr i64 %7294, 48
  %7314 = trunc i64 %7313 to i8
  store i8 %7314, ptr %7312, align 1
  %7315 = getelementptr inbounds nuw i8, ptr %7293, i64 7
  %7316 = lshr i64 %7294, 56
  %7317 = trunc nuw i64 %7316 to i8
  store i8 %7317, ptr %7315, align 1
  %7318 = getelementptr inbounds nuw i8, ptr %7293, i64 8
  %7319 = add i32 %7295, 8
  %7320 = icmp eq i32 %7319, %7291
  br i1 %7320, label %8758, label %7292, !llvm.loop !16

7321:                                             ; preds = %7193, %7321
  %7322 = phi i64 [ %7338, %7321 ], [ %7194, %7193 ]
  %7323 = getelementptr inbounds nuw i8, ptr %7173, i64 %7322
  %7324 = load i8, ptr %7323, align 1
  %7325 = getelementptr inbounds nuw i8, ptr %7174, i64 %7322
  store i8 %7324, ptr %7325, align 1
  %7326 = add nuw nsw i64 %7322, 1
  %7327 = getelementptr inbounds nuw i8, ptr %7173, i64 %7326
  %7328 = load i8, ptr %7327, align 1
  %7329 = getelementptr inbounds nuw i8, ptr %7174, i64 %7326
  store i8 %7328, ptr %7329, align 1
  %7330 = add nuw nsw i64 %7322, 2
  %7331 = getelementptr inbounds nuw i8, ptr %7173, i64 %7330
  %7332 = load i8, ptr %7331, align 1
  %7333 = getelementptr inbounds nuw i8, ptr %7174, i64 %7330
  store i8 %7332, ptr %7333, align 1
  %7334 = add nuw nsw i64 %7322, 3
  %7335 = getelementptr inbounds nuw i8, ptr %7173, i64 %7334
  %7336 = load i8, ptr %7335, align 1
  %7337 = getelementptr inbounds nuw i8, ptr %7174, i64 %7334
  store i8 %7336, ptr %7337, align 1
  %7338 = add nuw nsw i64 %7322, 4
  %7339 = icmp eq i64 %7338, %7175
  br i1 %7339, label %7270, label %7321, !llvm.loop !68

7340:                                             ; preds = %158
  %7341 = shl i32 %81, 13
  %7342 = xor i32 %7341, %81
  %7343 = lshr i32 %7342, 17
  %7344 = xor i32 %7343, %7342
  %7345 = shl i32 %7344, 5
  %7346 = xor i32 %7345, %7344
  %7347 = add nsw i32 %72, 3
  %7348 = shl i32 %7346, 13
  %7349 = xor i32 %7348, %7346
  %7350 = lshr i32 %7349, 17
  %7351 = xor i32 %7350, %7349
  %7352 = shl i32 %7351, 5
  %7353 = xor i32 %7352, %7351
  store i32 %7353, ptr @vm_code_state, align 4
  %7354 = load i32, ptr @pointer_size, align 4
  %7355 = icmp sgt i32 %7354, 0
  br i1 %7355, label %7356, label %7382

7356:                                             ; preds = %7340
  %7357 = sext i32 %7347 to i64
  %7358 = zext nneg i32 %7354 to i64
  br label %7359

7359:                                             ; preds = %7356, %7359
  %7360 = phi i64 [ %7357, %7356 ], [ %7364, %7359 ]
  %7361 = phi i64 [ 0, %7356 ], [ %7380, %7359 ]
  %7362 = phi i64 [ 0, %7356 ], [ %7379, %7359 ]
  %7363 = phi i32 [ %7353, %7356 ], [ %7373, %7359 ]
  %7364 = add nsw i64 %7360, 1
  %7365 = trunc nsw i64 %7364 to i32
  store i32 %7365, ptr @ip, align 4
  %7366 = getelementptr inbounds i8, ptr %69, i64 %7360
  %7367 = load i8, ptr %7366, align 1
  %7368 = shl i32 %7363, 13
  %7369 = xor i32 %7368, %7363
  %7370 = lshr i32 %7369, 17
  %7371 = xor i32 %7370, %7369
  %7372 = shl i32 %7371, 5
  %7373 = xor i32 %7372, %7371
  store i32 %7373, ptr @vm_code_state, align 4
  %7374 = trunc i32 %7373 to i8
  %7375 = xor i8 %7367, %7374
  %7376 = zext i8 %7375 to i64
  %7377 = shl nsw i64 %7361, 3
  %7378 = shl i64 %7376, %7377
  %7379 = or i64 %7378, %7362
  %7380 = add nuw nsw i64 %7361, 1
  %7381 = icmp eq i64 %7380, %7358
  br i1 %7381, label %7382, label %7359, !llvm.loop !15

7382:                                             ; preds = %7359, %7340
  %7383 = phi i32 [ %7353, %7340 ], [ %7373, %7359 ]
  %7384 = phi i32 [ %7347, %7340 ], [ %7365, %7359 ]
  %7385 = phi i64 [ 0, %7340 ], [ %7379, %7359 ]
  %7386 = add nsw i32 %7384, 1
  store i32 %7386, ptr @ip, align 4
  %7387 = sext i32 %7384 to i64
  %7388 = getelementptr inbounds i8, ptr %69, i64 %7387
  %7389 = load i8, ptr %7388, align 1
  %7390 = shl i32 %7383, 13
  %7391 = xor i32 %7390, %7383
  %7392 = lshr i32 %7391, 17
  %7393 = xor i32 %7392, %7391
  %7394 = shl i32 %7393, 5
  %7395 = xor i32 %7394, %7393
  store i32 %7395, ptr @vm_code_state, align 4
  %7396 = trunc i32 %7395 to i8
  %7397 = xor i8 %7389, %7396
  %7398 = add nsw i32 %7384, 2
  store i32 %7398, ptr @ip, align 4
  %7399 = sext i32 %7386 to i64
  %7400 = getelementptr inbounds i8, ptr %69, i64 %7399
  %7401 = load i8, ptr %7400, align 1
  %7402 = shl i32 %7395, 13
  %7403 = xor i32 %7402, %7395
  %7404 = lshr i32 %7403, 17
  %7405 = xor i32 %7404, %7403
  %7406 = shl i32 %7405, 5
  %7407 = xor i32 %7406, %7405
  store i32 %7407, ptr @vm_code_state, align 4
  %7408 = trunc i32 %7407 to i8
  %7409 = icmp eq i8 %7401, %7408
  br i1 %7409, label %7410, label %7489

7410:                                             ; preds = %7382
  br i1 %7355, label %7411, label %7437

7411:                                             ; preds = %7410
  %7412 = sext i32 %7398 to i64
  %7413 = zext nneg i32 %7354 to i64
  br label %7414

7414:                                             ; preds = %7411, %7414
  %7415 = phi i64 [ %7412, %7411 ], [ %7419, %7414 ]
  %7416 = phi i64 [ 0, %7411 ], [ %7435, %7414 ]
  %7417 = phi i64 [ 0, %7411 ], [ %7434, %7414 ]
  %7418 = phi i32 [ %7407, %7411 ], [ %7428, %7414 ]
  %7419 = add nsw i64 %7415, 1
  %7420 = trunc nsw i64 %7419 to i32
  store i32 %7420, ptr @ip, align 4
  %7421 = getelementptr inbounds i8, ptr %69, i64 %7415
  %7422 = load i8, ptr %7421, align 1
  %7423 = shl i32 %7418, 13
  %7424 = xor i32 %7423, %7418
  %7425 = lshr i32 %7424, 17
  %7426 = xor i32 %7425, %7424
  %7427 = shl i32 %7426, 5
  %7428 = xor i32 %7427, %7426
  store i32 %7428, ptr @vm_code_state, align 4
  %7429 = trunc i32 %7428 to i8
  %7430 = xor i8 %7422, %7429
  %7431 = zext i8 %7430 to i64
  %7432 = shl nsw i64 %7416, 3
  %7433 = shl i64 %7431, %7432
  %7434 = or i64 %7433, %7417
  %7435 = add nuw nsw i64 %7416, 1
  %7436 = icmp eq i64 %7435, %7413
  br i1 %7436, label %7437, label %7414, !llvm.loop !15

7437:                                             ; preds = %7414, %7410
  %7438 = phi i32 [ %7407, %7410 ], [ %7428, %7414 ]
  %7439 = phi i32 [ %7398, %7410 ], [ %7420, %7414 ]
  %7440 = phi i64 [ 0, %7410 ], [ %7434, %7414 ]
  %7441 = icmp eq i8 %7389, %7396
  br i1 %7441, label %7538, label %7442

7442:                                             ; preds = %7437
  %7443 = load i64, ptr @data_seg_addr, align 8
  %7444 = inttoptr i64 %7443 to ptr
  %7445 = zext i8 %7397 to i64
  %7446 = and i64 %7445, 3
  %7447 = icmp ult i8 %7397, 4
  br i1 %7447, label %7517, label %7448

7448:                                             ; preds = %7442
  %7449 = and i64 %7445, 252
  %7450 = getelementptr i8, ptr %7444, i64 1
  %7451 = getelementptr i8, ptr %7444, i64 2
  %7452 = getelementptr i8, ptr %7444, i64 3
  br label %7453

7453:                                             ; preds = %7453, %7448
  %7454 = phi i64 [ 0, %7448 ], [ %7486, %7453 ]
  %7455 = phi i64 [ 0, %7448 ], [ %7485, %7453 ]
  %7456 = phi i64 [ %7440, %7448 ], [ %7478, %7453 ]
  %7457 = phi i64 [ 0, %7448 ], [ %7487, %7453 ]
  %7458 = getelementptr inbounds nuw i8, ptr %7444, i64 %7456
  %7459 = load i8, ptr %7458, align 1
  %7460 = zext i8 %7459 to i64
  %7461 = shl nsw i64 %7454, 3
  %7462 = shl i64 %7460, %7461
  %7463 = or i64 %7462, %7455
  %7464 = getelementptr i8, ptr %7450, i64 %7456
  %7465 = load i8, ptr %7464, align 1
  %7466 = zext i8 %7465 to i64
  %7467 = shl i64 %7454, 3
  %7468 = or disjoint i64 %7467, 8
  %7469 = shl i64 %7466, %7468
  %7470 = or i64 %7469, %7463
  %7471 = getelementptr i8, ptr %7451, i64 %7456
  %7472 = load i8, ptr %7471, align 1
  %7473 = zext i8 %7472 to i64
  %7474 = shl i64 %7454, 3
  %7475 = or disjoint i64 %7474, 16
  %7476 = shl i64 %7473, %7475
  %7477 = or i64 %7476, %7470
  %7478 = add i64 %7456, 4
  %7479 = getelementptr i8, ptr %7452, i64 %7456
  %7480 = load i8, ptr %7479, align 1
  %7481 = zext i8 %7480 to i64
  %7482 = shl i64 %7454, 3
  %7483 = or disjoint i64 %7482, 24
  %7484 = shl i64 %7481, %7483
  %7485 = or i64 %7484, %7477
  %7486 = add nuw nsw i64 %7454, 4
  %7487 = add i64 %7457, 4
  %7488 = icmp eq i64 %7487, %7449
  br i1 %7488, label %7517, label %7453, !llvm.loop !17

7489:                                             ; preds = %7382
  %7490 = icmp eq i8 %7389, %7396
  br i1 %7490, label %7538, label %7491

7491:                                             ; preds = %7489
  %7492 = sext i32 %7398 to i64
  %7493 = zext i8 %7397 to i64
  br label %7494

7494:                                             ; preds = %7491, %7494
  %7495 = phi i64 [ %7492, %7491 ], [ %7499, %7494 ]
  %7496 = phi i64 [ 0, %7491 ], [ %7515, %7494 ]
  %7497 = phi i64 [ 0, %7491 ], [ %7514, %7494 ]
  %7498 = phi i32 [ %7407, %7491 ], [ %7508, %7494 ]
  %7499 = add nsw i64 %7495, 1
  %7500 = trunc nsw i64 %7499 to i32
  store i32 %7500, ptr @ip, align 4
  %7501 = getelementptr inbounds i8, ptr %69, i64 %7495
  %7502 = load i8, ptr %7501, align 1
  %7503 = shl i32 %7498, 13
  %7504 = xor i32 %7503, %7498
  %7505 = lshr i32 %7504, 17
  %7506 = xor i32 %7505, %7504
  %7507 = shl i32 %7506, 5
  %7508 = xor i32 %7507, %7506
  store i32 %7508, ptr @vm_code_state, align 4
  %7509 = trunc i32 %7508 to i8
  %7510 = xor i8 %7502, %7509
  %7511 = zext i8 %7510 to i64
  %7512 = shl nsw i64 %7496, 3
  %7513 = shl i64 %7511, %7512
  %7514 = or i64 %7513, %7497
  %7515 = add nuw nsw i64 %7496, 1
  %7516 = icmp eq i64 %7515, %7493
  br i1 %7516, label %7538, label %7494, !llvm.loop !15

7517:                                             ; preds = %7453, %7442
  %7518 = phi i64 [ poison, %7442 ], [ %7485, %7453 ]
  %7519 = phi i64 [ 0, %7442 ], [ %7486, %7453 ]
  %7520 = phi i64 [ 0, %7442 ], [ %7485, %7453 ]
  %7521 = phi i64 [ %7440, %7442 ], [ %7478, %7453 ]
  %7522 = icmp eq i64 %7446, 0
  br i1 %7522, label %7538, label %7523

7523:                                             ; preds = %7517, %7523
  %7524 = phi i64 [ %7535, %7523 ], [ %7519, %7517 ]
  %7525 = phi i64 [ %7534, %7523 ], [ %7520, %7517 ]
  %7526 = phi i64 [ %7528, %7523 ], [ %7521, %7517 ]
  %7527 = phi i64 [ %7536, %7523 ], [ 0, %7517 ]
  %7528 = add i64 %7526, 1
  %7529 = getelementptr inbounds nuw i8, ptr %7444, i64 %7526
  %7530 = load i8, ptr %7529, align 1
  %7531 = zext i8 %7530 to i64
  %7532 = shl nsw i64 %7524, 3
  %7533 = shl i64 %7531, %7532
  %7534 = or i64 %7533, %7525
  %7535 = add nuw nsw i64 %7524, 1
  %7536 = add i64 %7527, 1
  %7537 = icmp eq i64 %7536, %7446
  br i1 %7537, label %7538, label %7523, !llvm.loop !69

7538:                                             ; preds = %7494, %7517, %7523, %7489, %7437
  %7539 = phi i32 [ %7438, %7437 ], [ %7407, %7489 ], [ %7438, %7523 ], [ %7438, %7517 ], [ %7508, %7494 ]
  %7540 = phi i32 [ %7439, %7437 ], [ %7398, %7489 ], [ %7439, %7523 ], [ %7439, %7517 ], [ %7500, %7494 ]
  %7541 = phi i64 [ 0, %7437 ], [ 0, %7489 ], [ %7518, %7517 ], [ %7534, %7523 ], [ %7514, %7494 ]
  %7542 = add nsw i32 %7540, 1
  store i32 %7542, ptr @ip, align 4
  %7543 = sext i32 %7540 to i64
  %7544 = getelementptr inbounds i8, ptr %69, i64 %7543
  %7545 = load i8, ptr %7544, align 1
  %7546 = shl i32 %7539, 13
  %7547 = xor i32 %7546, %7539
  %7548 = lshr i32 %7547, 17
  %7549 = xor i32 %7548, %7547
  %7550 = shl i32 %7549, 5
  %7551 = xor i32 %7550, %7549
  store i32 %7551, ptr @vm_code_state, align 4
  %7552 = trunc i32 %7551 to i8
  %7553 = xor i8 %7545, %7552
  %7554 = add nsw i32 %7540, 2
  store i32 %7554, ptr @ip, align 4
  %7555 = sext i32 %7542 to i64
  %7556 = getelementptr inbounds i8, ptr %69, i64 %7555
  %7557 = load i8, ptr %7556, align 1
  %7558 = shl i32 %7551, 13
  %7559 = xor i32 %7558, %7551
  %7560 = lshr i32 %7559, 17
  %7561 = xor i32 %7560, %7559
  %7562 = shl i32 %7561, 5
  %7563 = xor i32 %7562, %7561
  store i32 %7563, ptr @vm_code_state, align 4
  %7564 = trunc i32 %7563 to i8
  %7565 = icmp eq i8 %7557, %7564
  br i1 %7565, label %7566, label %7645

7566:                                             ; preds = %7538
  br i1 %7355, label %7567, label %7593

7567:                                             ; preds = %7566
  %7568 = sext i32 %7554 to i64
  %7569 = zext nneg i32 %7354 to i64
  br label %7570

7570:                                             ; preds = %7567, %7570
  %7571 = phi i64 [ %7568, %7567 ], [ %7575, %7570 ]
  %7572 = phi i64 [ 0, %7567 ], [ %7591, %7570 ]
  %7573 = phi i64 [ 0, %7567 ], [ %7590, %7570 ]
  %7574 = phi i32 [ %7563, %7567 ], [ %7584, %7570 ]
  %7575 = add nsw i64 %7571, 1
  %7576 = trunc nsw i64 %7575 to i32
  store i32 %7576, ptr @ip, align 4
  %7577 = getelementptr inbounds i8, ptr %69, i64 %7571
  %7578 = load i8, ptr %7577, align 1
  %7579 = shl i32 %7574, 13
  %7580 = xor i32 %7579, %7574
  %7581 = lshr i32 %7580, 17
  %7582 = xor i32 %7581, %7580
  %7583 = shl i32 %7582, 5
  %7584 = xor i32 %7583, %7582
  store i32 %7584, ptr @vm_code_state, align 4
  %7585 = trunc i32 %7584 to i8
  %7586 = xor i8 %7578, %7585
  %7587 = zext i8 %7586 to i64
  %7588 = shl nsw i64 %7572, 3
  %7589 = shl i64 %7587, %7588
  %7590 = or i64 %7589, %7573
  %7591 = add nuw nsw i64 %7572, 1
  %7592 = icmp eq i64 %7591, %7569
  br i1 %7592, label %7593, label %7570, !llvm.loop !15

7593:                                             ; preds = %7570, %7566
  %7594 = phi i32 [ %7563, %7566 ], [ %7584, %7570 ]
  %7595 = phi i32 [ %7554, %7566 ], [ %7576, %7570 ]
  %7596 = phi i64 [ 0, %7566 ], [ %7590, %7570 ]
  %7597 = icmp eq i8 %7545, %7552
  br i1 %7597, label %7694, label %7598

7598:                                             ; preds = %7593
  %7599 = load i64, ptr @data_seg_addr, align 8
  %7600 = inttoptr i64 %7599 to ptr
  %7601 = zext i8 %7553 to i64
  %7602 = and i64 %7601, 3
  %7603 = icmp ult i8 %7553, 4
  br i1 %7603, label %7673, label %7604

7604:                                             ; preds = %7598
  %7605 = and i64 %7601, 252
  %7606 = getelementptr i8, ptr %7600, i64 1
  %7607 = getelementptr i8, ptr %7600, i64 2
  %7608 = getelementptr i8, ptr %7600, i64 3
  br label %7609

7609:                                             ; preds = %7609, %7604
  %7610 = phi i64 [ 0, %7604 ], [ %7642, %7609 ]
  %7611 = phi i64 [ 0, %7604 ], [ %7641, %7609 ]
  %7612 = phi i64 [ %7596, %7604 ], [ %7634, %7609 ]
  %7613 = phi i64 [ 0, %7604 ], [ %7643, %7609 ]
  %7614 = getelementptr inbounds nuw i8, ptr %7600, i64 %7612
  %7615 = load i8, ptr %7614, align 1
  %7616 = zext i8 %7615 to i64
  %7617 = shl nsw i64 %7610, 3
  %7618 = shl i64 %7616, %7617
  %7619 = or i64 %7618, %7611
  %7620 = getelementptr i8, ptr %7606, i64 %7612
  %7621 = load i8, ptr %7620, align 1
  %7622 = zext i8 %7621 to i64
  %7623 = shl i64 %7610, 3
  %7624 = or disjoint i64 %7623, 8
  %7625 = shl i64 %7622, %7624
  %7626 = or i64 %7625, %7619
  %7627 = getelementptr i8, ptr %7607, i64 %7612
  %7628 = load i8, ptr %7627, align 1
  %7629 = zext i8 %7628 to i64
  %7630 = shl i64 %7610, 3
  %7631 = or disjoint i64 %7630, 16
  %7632 = shl i64 %7629, %7631
  %7633 = or i64 %7632, %7626
  %7634 = add i64 %7612, 4
  %7635 = getelementptr i8, ptr %7608, i64 %7612
  %7636 = load i8, ptr %7635, align 1
  %7637 = zext i8 %7636 to i64
  %7638 = shl i64 %7610, 3
  %7639 = or disjoint i64 %7638, 24
  %7640 = shl i64 %7637, %7639
  %7641 = or i64 %7640, %7633
  %7642 = add nuw nsw i64 %7610, 4
  %7643 = add i64 %7613, 4
  %7644 = icmp eq i64 %7643, %7605
  br i1 %7644, label %7673, label %7609, !llvm.loop !17

7645:                                             ; preds = %7538
  %7646 = icmp eq i8 %7545, %7552
  br i1 %7646, label %7694, label %7647

7647:                                             ; preds = %7645
  %7648 = sext i32 %7554 to i64
  %7649 = zext i8 %7553 to i64
  br label %7650

7650:                                             ; preds = %7647, %7650
  %7651 = phi i64 [ %7648, %7647 ], [ %7655, %7650 ]
  %7652 = phi i64 [ 0, %7647 ], [ %7671, %7650 ]
  %7653 = phi i64 [ 0, %7647 ], [ %7670, %7650 ]
  %7654 = phi i32 [ %7563, %7647 ], [ %7664, %7650 ]
  %7655 = add nsw i64 %7651, 1
  %7656 = trunc nsw i64 %7655 to i32
  store i32 %7656, ptr @ip, align 4
  %7657 = getelementptr inbounds i8, ptr %69, i64 %7651
  %7658 = load i8, ptr %7657, align 1
  %7659 = shl i32 %7654, 13
  %7660 = xor i32 %7659, %7654
  %7661 = lshr i32 %7660, 17
  %7662 = xor i32 %7661, %7660
  %7663 = shl i32 %7662, 5
  %7664 = xor i32 %7663, %7662
  store i32 %7664, ptr @vm_code_state, align 4
  %7665 = trunc i32 %7664 to i8
  %7666 = xor i8 %7658, %7665
  %7667 = zext i8 %7666 to i64
  %7668 = shl nsw i64 %7652, 3
  %7669 = shl i64 %7667, %7668
  %7670 = or i64 %7669, %7653
  %7671 = add nuw nsw i64 %7652, 1
  %7672 = icmp eq i64 %7671, %7649
  br i1 %7672, label %7694, label %7650, !llvm.loop !15

7673:                                             ; preds = %7609, %7598
  %7674 = phi i64 [ poison, %7598 ], [ %7641, %7609 ]
  %7675 = phi i64 [ 0, %7598 ], [ %7642, %7609 ]
  %7676 = phi i64 [ 0, %7598 ], [ %7641, %7609 ]
  %7677 = phi i64 [ %7596, %7598 ], [ %7634, %7609 ]
  %7678 = icmp eq i64 %7602, 0
  br i1 %7678, label %7694, label %7679

7679:                                             ; preds = %7673, %7679
  %7680 = phi i64 [ %7691, %7679 ], [ %7675, %7673 ]
  %7681 = phi i64 [ %7690, %7679 ], [ %7676, %7673 ]
  %7682 = phi i64 [ %7684, %7679 ], [ %7677, %7673 ]
  %7683 = phi i64 [ %7692, %7679 ], [ 0, %7673 ]
  %7684 = add i64 %7682, 1
  %7685 = getelementptr inbounds nuw i8, ptr %7600, i64 %7682
  %7686 = load i8, ptr %7685, align 1
  %7687 = zext i8 %7686 to i64
  %7688 = shl nsw i64 %7680, 3
  %7689 = shl i64 %7687, %7688
  %7690 = or i64 %7689, %7681
  %7691 = add nuw nsw i64 %7680, 1
  %7692 = add i64 %7683, 1
  %7693 = icmp eq i64 %7692, %7602
  br i1 %7693, label %7694, label %7679, !llvm.loop !70

7694:                                             ; preds = %7650, %7673, %7679, %7645, %7593
  %7695 = phi i32 [ %7594, %7593 ], [ %7563, %7645 ], [ %7594, %7679 ], [ %7594, %7673 ], [ %7664, %7650 ]
  %7696 = phi i32 [ %7595, %7593 ], [ %7554, %7645 ], [ %7595, %7679 ], [ %7595, %7673 ], [ %7656, %7650 ]
  %7697 = phi i64 [ 0, %7593 ], [ 0, %7645 ], [ %7674, %7673 ], [ %7690, %7679 ], [ %7670, %7650 ]
  %7698 = sext i32 %7696 to i64
  %7699 = add nsw i64 %7698, 1
  %7700 = trunc nsw i64 %7699 to i32
  store i32 %7700, ptr @ip, align 4
  %7701 = getelementptr inbounds i8, ptr %69, i64 %7698
  %7702 = load i8, ptr %7701, align 1
  %7703 = shl i32 %7695, 13
  %7704 = xor i32 %7703, %7695
  %7705 = lshr i32 %7704, 17
  %7706 = xor i32 %7705, %7704
  %7707 = shl i32 %7706, 5
  %7708 = xor i32 %7707, %7706
  store i32 %7708, ptr @vm_code_state, align 4
  %7709 = add nsw i64 %7698, 2
  %7710 = trunc nsw i64 %7709 to i32
  store i32 %7710, ptr @ip, align 4
  %7711 = getelementptr inbounds i8, ptr %69, i64 %7699
  %7712 = load i8, ptr %7711, align 1
  %7713 = shl i32 %7708, 13
  %7714 = xor i32 %7713, %7708
  %7715 = lshr i32 %7714, 17
  %7716 = xor i32 %7715, %7714
  %7717 = shl i32 %7716, 5
  %7718 = xor i32 %7717, %7716
  store i32 %7718, ptr @vm_code_state, align 4
  %7719 = add nsw i64 %7698, 3
  %7720 = trunc nsw i64 %7719 to i32
  store i32 %7720, ptr @ip, align 4
  %7721 = getelementptr inbounds i8, ptr %69, i64 %7709
  %7722 = load i8, ptr %7721, align 1
  %7723 = shl i32 %7718, 13
  %7724 = xor i32 %7723, %7718
  %7725 = lshr i32 %7724, 17
  %7726 = xor i32 %7725, %7724
  %7727 = shl i32 %7726, 5
  %7728 = xor i32 %7727, %7726
  store i32 %7728, ptr @vm_code_state, align 4
  %7729 = add i32 %7696, 4
  store i32 %7729, ptr @ip, align 4
  %7730 = getelementptr inbounds i8, ptr %69, i64 %7719
  %7731 = load i8, ptr %7730, align 1
  %7732 = shl i32 %7728, 13
  %7733 = xor i32 %7732, %7728
  %7734 = lshr i32 %7733, 17
  %7735 = xor i32 %7734, %7733
  %7736 = shl i32 %7735, 5
  %7737 = xor i32 %7736, %7735
  store i32 %7737, ptr @vm_code_state, align 4
  %7738 = sext i32 %7729 to i64
  %7739 = add nsw i64 %7738, 1
  %7740 = trunc nsw i64 %7739 to i32
  store i32 %7740, ptr @ip, align 4
  %7741 = getelementptr inbounds i8, ptr %69, i64 %7738
  %7742 = load i8, ptr %7741, align 1
  %7743 = shl i32 %7737, 13
  %7744 = xor i32 %7743, %7737
  %7745 = lshr i32 %7744, 17
  %7746 = xor i32 %7745, %7744
  %7747 = shl i32 %7746, 5
  %7748 = xor i32 %7747, %7746
  store i32 %7748, ptr @vm_code_state, align 4
  %7749 = add nsw i64 %7738, 2
  %7750 = trunc nsw i64 %7749 to i32
  store i32 %7750, ptr @ip, align 4
  %7751 = getelementptr inbounds i8, ptr %69, i64 %7739
  %7752 = load i8, ptr %7751, align 1
  %7753 = shl i32 %7748, 13
  %7754 = xor i32 %7753, %7748
  %7755 = lshr i32 %7754, 17
  %7756 = xor i32 %7755, %7754
  %7757 = shl i32 %7756, 5
  %7758 = xor i32 %7757, %7756
  store i32 %7758, ptr @vm_code_state, align 4
  %7759 = add nsw i64 %7738, 3
  %7760 = trunc nsw i64 %7759 to i32
  store i32 %7760, ptr @ip, align 4
  %7761 = getelementptr inbounds i8, ptr %69, i64 %7749
  %7762 = load i8, ptr %7761, align 1
  %7763 = shl i32 %7758, 13
  %7764 = xor i32 %7763, %7758
  %7765 = lshr i32 %7764, 17
  %7766 = xor i32 %7765, %7764
  %7767 = shl i32 %7766, 5
  %7768 = xor i32 %7767, %7766
  store i32 %7768, ptr @vm_code_state, align 4
  %7769 = add i32 %7696, 8
  store i32 %7769, ptr @ip, align 4
  %7770 = getelementptr inbounds i8, ptr %69, i64 %7759
  %7771 = load i8, ptr %7770, align 1
  %7772 = shl i32 %7768, 13
  %7773 = xor i32 %7772, %7768
  %7774 = lshr i32 %7773, 17
  %7775 = xor i32 %7774, %7773
  %7776 = shl i32 %7775, 5
  %7777 = xor i32 %7776, %7775
  %7778 = shl i32 %7777, 13
  %7779 = xor i32 %7778, %7777
  %7780 = lshr i32 %7779, 17
  %7781 = xor i32 %7780, %7779
  %7782 = shl i32 %7781, 5
  %7783 = xor i32 %7782, %7781
  %7784 = shl i32 %7783, 13
  %7785 = xor i32 %7784, %7783
  %7786 = lshr i32 %7785, 17
  %7787 = xor i32 %7786, %7785
  %7788 = shl i32 %7787, 5
  %7789 = xor i32 %7788, %7787
  %7790 = shl i32 %7789, 13
  %7791 = xor i32 %7790, %7789
  %7792 = lshr i32 %7791, 17
  %7793 = xor i32 %7792, %7791
  %7794 = shl i32 %7793, 5
  %7795 = xor i32 %7794, %7793
  %7796 = shl i32 %7795, 13
  %7797 = xor i32 %7796, %7795
  %7798 = lshr i32 %7797, 17
  %7799 = xor i32 %7798, %7797
  %7800 = shl i32 %7799, 5
  %7801 = xor i32 %7800, %7799
  %7802 = add i32 %7696, 12
  store i32 %7801, ptr @vm_code_state, align 4
  %7803 = load i64, ptr @code_seg_addr, align 8
  %7804 = inttoptr i64 %7803 to ptr
  %7805 = sext i32 %7802 to i64
  %7806 = add nsw i64 %7805, 1
  %7807 = trunc nsw i64 %7806 to i32
  store i32 %7807, ptr @ip, align 4
  %7808 = getelementptr inbounds i8, ptr %7804, i64 %7805
  %7809 = load i8, ptr %7808, align 1
  %7810 = shl i32 %7801, 13
  %7811 = xor i32 %7810, %7801
  %7812 = lshr i32 %7811, 17
  %7813 = xor i32 %7812, %7811
  %7814 = shl i32 %7813, 5
  %7815 = xor i32 %7814, %7813
  store i32 %7815, ptr @vm_code_state, align 4
  %7816 = trunc i32 %7815 to i8
  %7817 = xor i8 %7809, %7816
  %7818 = zext i8 %7817 to i32
  %7819 = add nsw i64 %7805, 2
  %7820 = trunc nsw i64 %7819 to i32
  store i32 %7820, ptr @ip, align 4
  %7821 = getelementptr inbounds i8, ptr %7804, i64 %7806
  %7822 = load i8, ptr %7821, align 1
  %7823 = shl i32 %7815, 13
  %7824 = xor i32 %7823, %7815
  %7825 = lshr i32 %7824, 17
  %7826 = xor i32 %7825, %7824
  %7827 = shl i32 %7826, 5
  %7828 = xor i32 %7827, %7826
  store i32 %7828, ptr @vm_code_state, align 4
  %7829 = trunc i32 %7828 to i8
  %7830 = xor i8 %7822, %7829
  %7831 = zext i8 %7830 to i32
  %7832 = shl nuw nsw i32 %7831, 8
  %7833 = or disjoint i32 %7832, %7818
  %7834 = add nsw i64 %7805, 3
  %7835 = trunc nsw i64 %7834 to i32
  store i32 %7835, ptr @ip, align 4
  %7836 = getelementptr inbounds i8, ptr %7804, i64 %7819
  %7837 = load i8, ptr %7836, align 1
  %7838 = shl i32 %7828, 13
  %7839 = xor i32 %7838, %7828
  %7840 = lshr i32 %7839, 17
  %7841 = xor i32 %7840, %7839
  %7842 = shl i32 %7841, 5
  %7843 = xor i32 %7842, %7841
  store i32 %7843, ptr @vm_code_state, align 4
  %7844 = trunc i32 %7843 to i8
  %7845 = xor i8 %7837, %7844
  %7846 = zext i8 %7845 to i32
  %7847 = shl nuw nsw i32 %7846, 16
  %7848 = or disjoint i32 %7847, %7833
  %7849 = add i32 %7696, 16
  store i32 %7849, ptr @ip, align 4
  %7850 = getelementptr inbounds i8, ptr %7804, i64 %7834
  %7851 = load i8, ptr %7850, align 1
  %7852 = shl i32 %7843, 13
  %7853 = xor i32 %7852, %7843
  %7854 = lshr i32 %7853, 17
  %7855 = xor i32 %7854, %7853
  %7856 = shl i32 %7855, 5
  %7857 = xor i32 %7856, %7855
  store i32 %7857, ptr @vm_code_state, align 4
  %7858 = trunc i32 %7857 to i8
  %7859 = xor i8 %7851, %7858
  %7860 = zext i8 %7859 to i32
  %7861 = shl nuw i32 %7860, 24
  %7862 = or disjoint i32 %7861, %7848
  %7863 = trunc i32 %7748 to i8
  %7864 = xor i8 %7742, %7863
  %7865 = zext i8 %7864 to i64
  %7866 = trunc i32 %7758 to i8
  %7867 = xor i8 %7752, %7866
  %7868 = zext i8 %7867 to i64
  %7869 = shl nuw nsw i64 %7868, 8
  %7870 = or disjoint i64 %7869, %7865
  %7871 = trunc i32 %7768 to i8
  %7872 = xor i8 %7762, %7871
  %7873 = zext i8 %7872 to i64
  %7874 = shl nuw nsw i64 %7873, 16
  %7875 = or disjoint i64 %7874, %7870
  %7876 = trunc i32 %7777 to i8
  %7877 = xor i8 %7771, %7876
  %7878 = zext i8 %7877 to i64
  %7879 = shl nuw nsw i64 %7878, 24
  %7880 = or disjoint i64 %7879, %7875
  %7881 = trunc i32 %7708 to i8
  %7882 = xor i8 %7702, %7881
  %7883 = zext i8 %7882 to i64
  %7884 = trunc i32 %7718 to i8
  %7885 = xor i8 %7712, %7884
  %7886 = zext i8 %7885 to i64
  %7887 = shl nuw nsw i64 %7886, 8
  %7888 = or disjoint i64 %7887, %7883
  %7889 = trunc i32 %7728 to i8
  %7890 = xor i8 %7722, %7889
  %7891 = zext i8 %7890 to i64
  %7892 = shl nuw nsw i64 %7891, 16
  %7893 = or disjoint i64 %7892, %7888
  %7894 = trunc i32 %7737 to i8
  %7895 = xor i8 %7731, %7894
  %7896 = zext i8 %7895 to i64
  %7897 = shl nuw nsw i64 %7896, 24
  %7898 = or disjoint i64 %7897, %7893
  %7899 = trunc nuw i64 %7898 to i32
  %7900 = icmp eq i32 %7862, 0
  br i1 %7900, label %8784, label %7901

7901:                                             ; preds = %7694
  %7902 = load i64, ptr @data_seg_addr, align 8
  %7903 = add i64 %7902, %7385
  %7904 = icmp eq i64 %7898, 0
  %7905 = inttoptr i64 %7903 to ptr
  %7906 = add nsw i64 %7897, -1
  %7907 = add nsw i64 %7906, %7892
  %7908 = add nsw i64 %7907, %7887
  %7909 = add nsw i64 %7908, %7883
  %7910 = zext i8 %7895 to i32
  %7911 = shl nuw i32 %7910, 24
  %7912 = zext i8 %7890 to i32
  %7913 = shl nuw nsw i32 %7912, 16
  %7914 = or disjoint i32 %7911, %7913
  %7915 = zext i8 %7885 to i32
  %7916 = shl nuw nsw i32 %7915, 8
  %7917 = or disjoint i32 %7914, %7916
  %7918 = zext i8 %7882 to i32
  %7919 = or disjoint i32 %7917, %7918
  %7920 = zext i8 %7895 to i32
  %7921 = shl nuw i32 %7920, 24
  %7922 = zext i8 %7890 to i32
  %7923 = shl nuw nsw i32 %7922, 16
  %7924 = or disjoint i32 %7921, %7923
  %7925 = zext i8 %7885 to i32
  %7926 = shl nuw nsw i32 %7925, 8
  %7927 = or disjoint i32 %7924, %7926
  %7928 = zext i8 %7882 to i32
  %7929 = or disjoint i32 %7927, %7928
  %7930 = or disjoint i64 %7897, %7892
  %7931 = or disjoint i64 %7930, %7887
  %7932 = or disjoint i64 %7931, %7883
  %7933 = add nsw i64 %7897, -1
  %7934 = add nsw i64 %7933, %7892
  %7935 = add nsw i64 %7934, %7887
  %7936 = add nsw i64 %7935, %7883
  %7937 = zext i8 %7895 to i32
  %7938 = shl nuw i32 %7937, 24
  %7939 = zext i8 %7890 to i32
  %7940 = shl nuw nsw i32 %7939, 16
  %7941 = or disjoint i32 %7938, %7940
  %7942 = zext i8 %7885 to i32
  %7943 = shl nuw nsw i32 %7942, 8
  %7944 = or disjoint i32 %7941, %7943
  %7945 = zext i8 %7882 to i32
  %7946 = or disjoint i32 %7944, %7945
  %7947 = or disjoint i64 %7897, %7892
  %7948 = or disjoint i64 %7947, %7887
  %7949 = or disjoint i64 %7948, %7883
  %7950 = icmp samesign ult i64 %7949, 8
  %7951 = trunc i64 %7936 to i32
  %7952 = icmp ugt i64 %7936, 4294967295
  %7953 = icmp samesign ult i64 %7949, 32
  %7954 = and i64 %7883, 31
  %7955 = sub nuw nsw i64 %7949, %7954
  %7956 = icmp eq i64 %7954, 0
  %7957 = icmp samesign ult i64 %7954, 8
  %7958 = and i64 %7883, 7
  %7959 = sub nsw i64 %7949, %7958
  %7960 = icmp eq i64 %7958, 0
  %7961 = icmp samesign ult i64 %7932, 4
  %7962 = trunc i64 %7909 to i32
  %7963 = icmp ugt i64 %7909, 4294967295
  %7964 = icmp samesign ult i64 %7932, 32
  %7965 = and i64 %7883, 31
  %7966 = sub nuw nsw i64 %7932, %7965
  %7967 = icmp eq i64 %7965, 0
  %7968 = icmp samesign ult i64 %7965, 4
  %7969 = and i64 %7883, 3
  %7970 = sub nsw i64 %7932, %7969
  %7971 = icmp eq i64 %7969, 0
  br label %7972

7972:                                             ; preds = %7901, %8141
  %7973 = phi i32 [ 0, %7901 ], [ %8142, %8141 ]
  %7974 = mul i32 %7946, %7973
  %7975 = mul i32 %7929, %7973
  %7976 = zext i32 %7975 to i64
  %7977 = mul i32 %7919, %7973
  %7978 = load i64, ptr @code_seg_addr, align 8
  %7979 = inttoptr i64 %7978 to ptr
  %7980 = load i32, ptr @ip, align 4
  %7981 = load i32, ptr @vm_code_state, align 4
  %7982 = sext i32 %7980 to i64
  %7983 = add nsw i64 %7982, 1
  %7984 = trunc nsw i64 %7983 to i32
  store i32 %7984, ptr @ip, align 4
  %7985 = getelementptr inbounds i8, ptr %7979, i64 %7982
  %7986 = load i8, ptr %7985, align 1
  %7987 = shl i32 %7981, 13
  %7988 = xor i32 %7987, %7981
  %7989 = lshr i32 %7988, 17
  %7990 = xor i32 %7989, %7988
  %7991 = shl i32 %7990, 5
  %7992 = xor i32 %7991, %7990
  store i32 %7992, ptr @vm_code_state, align 4
  %7993 = add nsw i64 %7982, 2
  %7994 = trunc nsw i64 %7993 to i32
  store i32 %7994, ptr @ip, align 4
  %7995 = getelementptr inbounds i8, ptr %7979, i64 %7983
  %7996 = load i8, ptr %7995, align 1
  %7997 = shl i32 %7992, 13
  %7998 = xor i32 %7997, %7992
  %7999 = lshr i32 %7998, 17
  %8000 = xor i32 %7999, %7998
  %8001 = shl i32 %8000, 5
  %8002 = xor i32 %8001, %8000
  store i32 %8002, ptr @vm_code_state, align 4
  %8003 = add nsw i64 %7982, 3
  %8004 = trunc nsw i64 %8003 to i32
  store i32 %8004, ptr @ip, align 4
  %8005 = getelementptr inbounds i8, ptr %7979, i64 %7993
  %8006 = load i8, ptr %8005, align 1
  %8007 = shl i32 %8002, 13
  %8008 = xor i32 %8007, %8002
  %8009 = lshr i32 %8008, 17
  %8010 = xor i32 %8009, %8008
  %8011 = shl i32 %8010, 5
  %8012 = xor i32 %8011, %8010
  store i32 %8012, ptr @vm_code_state, align 4
  %8013 = add i32 %7980, 4
  store i32 %8013, ptr @ip, align 4
  %8014 = getelementptr inbounds i8, ptr %7979, i64 %8003
  %8015 = load i8, ptr %8014, align 1
  %8016 = shl i32 %8012, 13
  %8017 = xor i32 %8016, %8012
  %8018 = lshr i32 %8017, 17
  %8019 = xor i32 %8018, %8017
  %8020 = shl i32 %8019, 5
  %8021 = xor i32 %8020, %8019
  store i32 %8021, ptr @vm_code_state, align 4
  %8022 = trunc i32 %8021 to i8
  %8023 = xor i8 %8015, %8022
  %8024 = icmp sgt i8 %8023, -1
  br i1 %8024, label %8057, label %8025

8025:                                             ; preds = %7972
  br i1 %7904, label %8141, label %8026

8026:                                             ; preds = %8025
  %8027 = mul i32 %7973, %7899
  br i1 %7950, label %8028, label %8030

8028:                                             ; preds = %8045, %8056, %8030, %8026
  %8029 = phi i64 [ 0, %8026 ], [ 0, %8030 ], [ %7955, %8045 ], [ %7959, %8056 ]
  br label %8133

8030:                                             ; preds = %8026
  %8031 = xor i32 %7974, -1
  %8032 = icmp ult i32 %8031, %7951
  %8033 = or i1 %8032, %7952
  br i1 %8033, label %8028, label %8034

8034:                                             ; preds = %8030
  br i1 %7953, label %8046, label %8035

8035:                                             ; preds = %8034, %8035
  %8036 = phi i64 [ %8042, %8035 ], [ 0, %8034 ]
  %8037 = trunc i64 %8036 to i32
  %8038 = add i32 %8027, %8037
  %8039 = zext i32 %8038 to i64
  %8040 = getelementptr inbounds nuw i8, ptr %7905, i64 %8039
  %8041 = getelementptr inbounds nuw i8, ptr %8040, i64 16
  store <16 x i8> zeroinitializer, ptr %8040, align 1
  store <16 x i8> zeroinitializer, ptr %8041, align 1
  %8042 = add nuw i64 %8036, 32
  %8043 = icmp eq i64 %8042, %7955
  br i1 %8043, label %8044, label %8035, !llvm.loop !71

8044:                                             ; preds = %8035
  br i1 %7956, label %8141, label %8045

8045:                                             ; preds = %8044
  br i1 %7957, label %8028, label %8046

8046:                                             ; preds = %8045, %8034
  %8047 = phi i64 [ %7955, %8045 ], [ 0, %8034 ]
  br label %8048

8048:                                             ; preds = %8048, %8046
  %8049 = phi i64 [ %8047, %8046 ], [ %8054, %8048 ]
  %8050 = trunc i64 %8049 to i32
  %8051 = add i32 %8027, %8050
  %8052 = zext i32 %8051 to i64
  %8053 = getelementptr inbounds nuw i8, ptr %7905, i64 %8052
  store <8 x i8> zeroinitializer, ptr %8053, align 1
  %8054 = add nuw i64 %8049, 8
  %8055 = icmp eq i64 %8054, %7959
  br i1 %8055, label %8056, label %8048, !llvm.loop !72

8056:                                             ; preds = %8048
  br i1 %7960, label %8141, label %8028

8057:                                             ; preds = %7972
  br i1 %7904, label %8141, label %8058

8058:                                             ; preds = %8057
  %8059 = zext nneg i8 %8023 to i64
  %8060 = shl nuw nsw i64 %8059, 24
  %8061 = trunc i32 %8012 to i8
  %8062 = xor i8 %8006, %8061
  %8063 = zext i8 %8062 to i64
  %8064 = shl nuw nsw i64 %8063, 16
  %8065 = trunc i32 %8002 to i8
  %8066 = xor i8 %7996, %8065
  %8067 = zext i8 %8066 to i64
  %8068 = shl nuw nsw i64 %8067, 8
  %8069 = trunc i32 %7992 to i8
  %8070 = xor i8 %7986, %8069
  %8071 = zext i8 %8070 to i64
  %8072 = or disjoint i64 %8068, %8071
  %8073 = or disjoint i64 %8064, %8072
  %8074 = or disjoint i64 %8060, %8073
  %8075 = icmp samesign ult i64 %8074, %7880
  %8076 = select i1 %8075, i64 0, i64 %7880
  %8077 = sub nuw nsw i64 %8074, %8076
  %8078 = mul nuw nsw i64 %8077, %7898
  %8079 = and i64 %8078, 4294967295
  %8080 = select i1 %8075, i64 %7541, i64 %7697
  %8081 = add i64 %8079, %8080
  %8082 = inttoptr i64 %8081 to ptr
  %8083 = mul i32 %7973, %7899
  br i1 %7961, label %8121, label %8084

8084:                                             ; preds = %8058
  %8085 = xor i32 %7977, -1
  %8086 = icmp ult i32 %8085, %7962
  %8087 = or i1 %8086, %7963
  br i1 %8087, label %8121, label %8088

8088:                                             ; preds = %8084
  %8089 = add i64 %7903, %7976
  %8090 = sub i64 %8089, %8081
  %8091 = icmp ult i64 %8090, 32
  br i1 %8091, label %8121, label %8092

8092:                                             ; preds = %8088
  br i1 %7964, label %8108, label %8093

8093:                                             ; preds = %8092, %8093
  %8094 = phi i64 [ %8104, %8093 ], [ 0, %8092 ]
  %8095 = trunc i64 %8094 to i32
  %8096 = getelementptr inbounds nuw i8, ptr %8082, i64 %8094
  %8097 = getelementptr inbounds nuw i8, ptr %8096, i64 16
  %8098 = load <16 x i8>, ptr %8096, align 1
  %8099 = load <16 x i8>, ptr %8097, align 1
  %8100 = add i32 %8083, %8095
  %8101 = zext i32 %8100 to i64
  %8102 = getelementptr inbounds nuw i8, ptr %7905, i64 %8101
  %8103 = getelementptr inbounds nuw i8, ptr %8102, i64 16
  store <16 x i8> %8098, ptr %8102, align 1
  store <16 x i8> %8099, ptr %8103, align 1
  %8104 = add nuw i64 %8094, 32
  %8105 = icmp eq i64 %8104, %7966
  br i1 %8105, label %8106, label %8093, !llvm.loop !73

8106:                                             ; preds = %8093
  br i1 %7967, label %8141, label %8107

8107:                                             ; preds = %8106
  br i1 %7968, label %8121, label %8108

8108:                                             ; preds = %8107, %8092
  %8109 = phi i64 [ %7966, %8107 ], [ 0, %8092 ]
  br label %8110

8110:                                             ; preds = %8110, %8108
  %8111 = phi i64 [ %8109, %8108 ], [ %8118, %8110 ]
  %8112 = trunc i64 %8111 to i32
  %8113 = getelementptr inbounds nuw i8, ptr %8082, i64 %8111
  %8114 = load <4 x i8>, ptr %8113, align 1
  %8115 = add i32 %8083, %8112
  %8116 = zext i32 %8115 to i64
  %8117 = getelementptr inbounds nuw i8, ptr %7905, i64 %8116
  store <4 x i8> %8114, ptr %8117, align 1
  %8118 = add nuw i64 %8111, 4
  %8119 = icmp eq i64 %8118, %7970
  br i1 %8119, label %8120, label %8110, !llvm.loop !74

8120:                                             ; preds = %8110
  br i1 %7971, label %8141, label %8121

8121:                                             ; preds = %8107, %8120, %8088, %8084, %8058
  %8122 = phi i64 [ 0, %8058 ], [ 0, %8084 ], [ 0, %8088 ], [ %7966, %8107 ], [ %7970, %8120 ]
  br label %8123

8123:                                             ; preds = %8121, %8123
  %8124 = phi i64 [ %8131, %8123 ], [ %8122, %8121 ]
  %8125 = getelementptr inbounds nuw i8, ptr %8082, i64 %8124
  %8126 = load i8, ptr %8125, align 1
  %8127 = trunc nuw i64 %8124 to i32
  %8128 = add i32 %8083, %8127
  %8129 = zext i32 %8128 to i64
  %8130 = getelementptr inbounds nuw i8, ptr %7905, i64 %8129
  store i8 %8126, ptr %8130, align 1
  %8131 = add nuw nsw i64 %8124, 1
  %8132 = icmp eq i64 %8131, %7898
  br i1 %8132, label %8141, label %8123, !llvm.loop !75

8133:                                             ; preds = %8028, %8133
  %8134 = phi i64 [ %8139, %8133 ], [ %8029, %8028 ]
  %8135 = trunc nuw i64 %8134 to i32
  %8136 = add i32 %8027, %8135
  %8137 = zext i32 %8136 to i64
  %8138 = getelementptr inbounds nuw i8, ptr %7905, i64 %8137
  store i8 0, ptr %8138, align 1
  %8139 = add nuw nsw i64 %8134, 1
  %8140 = icmp eq i64 %8139, %7898
  br i1 %8140, label %8141, label %8133, !llvm.loop !76

8141:                                             ; preds = %8133, %8123, %8044, %8056, %8106, %8120, %8025, %8057
  %8142 = add nuw i32 %7973, 1
  %8143 = icmp eq i32 %8142, %7862
  br i1 %8143, label %8784, label %7972, !llvm.loop !77

8144:                                             ; preds = %158
  %8145 = add nsw i32 %72, 2
  store i32 %8145, ptr @ip, align 4
  %8146 = sext i32 %73 to i64
  %8147 = getelementptr inbounds i8, ptr %69, i64 %8146
  %8148 = load i8, ptr %8147, align 1
  %8149 = shl i32 %81, 13
  %8150 = xor i32 %8149, %81
  %8151 = lshr i32 %8150, 17
  %8152 = xor i32 %8151, %8150
  %8153 = shl i32 %8152, 5
  %8154 = xor i32 %8153, %8152
  %8155 = trunc i32 %8154 to i8
  %8156 = xor i8 %8148, %8155
  %8157 = add nsw i32 %72, 3
  %8158 = shl i32 %8154, 13
  %8159 = xor i32 %8158, %8154
  %8160 = lshr i32 %8159, 17
  %8161 = xor i32 %8160, %8159
  %8162 = shl i32 %8161, 5
  %8163 = xor i32 %8162, %8161
  store i32 %8163, ptr @vm_code_state, align 4
  %8164 = load i32, ptr @pointer_size, align 4
  %8165 = icmp sgt i32 %8164, 0
  br i1 %8165, label %8166, label %8192

8166:                                             ; preds = %8144
  %8167 = sext i32 %8157 to i64
  %8168 = zext nneg i32 %8164 to i64
  br label %8169

8169:                                             ; preds = %8166, %8169
  %8170 = phi i64 [ %8167, %8166 ], [ %8174, %8169 ]
  %8171 = phi i64 [ 0, %8166 ], [ %8190, %8169 ]
  %8172 = phi i64 [ 0, %8166 ], [ %8189, %8169 ]
  %8173 = phi i32 [ %8163, %8166 ], [ %8183, %8169 ]
  %8174 = add nsw i64 %8170, 1
  %8175 = trunc nsw i64 %8174 to i32
  store i32 %8175, ptr @ip, align 4
  %8176 = getelementptr inbounds i8, ptr %69, i64 %8170
  %8177 = load i8, ptr %8176, align 1
  %8178 = shl i32 %8173, 13
  %8179 = xor i32 %8178, %8173
  %8180 = lshr i32 %8179, 17
  %8181 = xor i32 %8180, %8179
  %8182 = shl i32 %8181, 5
  %8183 = xor i32 %8182, %8181
  store i32 %8183, ptr @vm_code_state, align 4
  %8184 = trunc i32 %8183 to i8
  %8185 = xor i8 %8177, %8184
  %8186 = zext i8 %8185 to i64
  %8187 = shl nsw i64 %8171, 3
  %8188 = shl i64 %8186, %8187
  %8189 = or i64 %8188, %8172
  %8190 = add nuw nsw i64 %8171, 1
  %8191 = icmp eq i64 %8190, %8168
  br i1 %8191, label %8192, label %8169, !llvm.loop !15

8192:                                             ; preds = %8169, %8144
  %8193 = phi i32 [ %8163, %8144 ], [ %8183, %8169 ]
  %8194 = phi i32 [ %8157, %8144 ], [ %8175, %8169 ]
  %8195 = phi i64 [ 0, %8144 ], [ %8189, %8169 ]
  %8196 = add nsw i32 %8194, 1
  store i32 %8196, ptr @ip, align 4
  %8197 = sext i32 %8194 to i64
  %8198 = getelementptr inbounds i8, ptr %69, i64 %8197
  %8199 = load i8, ptr %8198, align 1
  %8200 = shl i32 %8193, 13
  %8201 = xor i32 %8200, %8193
  %8202 = lshr i32 %8201, 17
  %8203 = xor i32 %8202, %8201
  %8204 = shl i32 %8203, 5
  %8205 = xor i32 %8204, %8203
  store i32 %8205, ptr @vm_code_state, align 4
  %8206 = trunc i32 %8205 to i8
  %8207 = xor i8 %8199, %8206
  %8208 = add nsw i32 %8194, 2
  store i32 %8208, ptr @ip, align 4
  %8209 = sext i32 %8196 to i64
  %8210 = getelementptr inbounds i8, ptr %69, i64 %8209
  %8211 = load i8, ptr %8210, align 1
  %8212 = shl i32 %8205, 13
  %8213 = xor i32 %8212, %8205
  %8214 = lshr i32 %8213, 17
  %8215 = xor i32 %8214, %8213
  %8216 = shl i32 %8215, 5
  %8217 = xor i32 %8216, %8215
  store i32 %8217, ptr @vm_code_state, align 4
  %8218 = trunc i32 %8217 to i8
  %8219 = icmp eq i8 %8211, %8218
  br i1 %8219, label %8220, label %8297

8220:                                             ; preds = %8192
  br i1 %8165, label %8221, label %8247

8221:                                             ; preds = %8220
  %8222 = sext i32 %8208 to i64
  %8223 = zext nneg i32 %8164 to i64
  br label %8224

8224:                                             ; preds = %8221, %8224
  %8225 = phi i64 [ %8222, %8221 ], [ %8229, %8224 ]
  %8226 = phi i64 [ 0, %8221 ], [ %8245, %8224 ]
  %8227 = phi i64 [ 0, %8221 ], [ %8244, %8224 ]
  %8228 = phi i32 [ %8217, %8221 ], [ %8238, %8224 ]
  %8229 = add nsw i64 %8225, 1
  %8230 = trunc nsw i64 %8229 to i32
  store i32 %8230, ptr @ip, align 4
  %8231 = getelementptr inbounds i8, ptr %69, i64 %8225
  %8232 = load i8, ptr %8231, align 1
  %8233 = shl i32 %8228, 13
  %8234 = xor i32 %8233, %8228
  %8235 = lshr i32 %8234, 17
  %8236 = xor i32 %8235, %8234
  %8237 = shl i32 %8236, 5
  %8238 = xor i32 %8237, %8236
  store i32 %8238, ptr @vm_code_state, align 4
  %8239 = trunc i32 %8238 to i8
  %8240 = xor i8 %8232, %8239
  %8241 = zext i8 %8240 to i64
  %8242 = shl nsw i64 %8226, 3
  %8243 = shl i64 %8241, %8242
  %8244 = or i64 %8243, %8227
  %8245 = add nuw nsw i64 %8226, 1
  %8246 = icmp eq i64 %8245, %8223
  br i1 %8246, label %8247, label %8224, !llvm.loop !15

8247:                                             ; preds = %8224, %8220
  %8248 = phi i64 [ 0, %8220 ], [ %8244, %8224 ]
  %8249 = icmp eq i8 %8199, %8206
  br i1 %8249, label %8346, label %8250

8250:                                             ; preds = %8247
  %8251 = load i64, ptr @data_seg_addr, align 8
  %8252 = inttoptr i64 %8251 to ptr
  %8253 = zext i8 %8207 to i64
  %8254 = and i64 %8253, 3
  %8255 = icmp ult i8 %8207, 4
  br i1 %8255, label %8325, label %8256

8256:                                             ; preds = %8250
  %8257 = and i64 %8253, 252
  %8258 = getelementptr i8, ptr %8252, i64 1
  %8259 = getelementptr i8, ptr %8252, i64 2
  %8260 = getelementptr i8, ptr %8252, i64 3
  br label %8261

8261:                                             ; preds = %8261, %8256
  %8262 = phi i64 [ 0, %8256 ], [ %8294, %8261 ]
  %8263 = phi i64 [ 0, %8256 ], [ %8293, %8261 ]
  %8264 = phi i64 [ %8248, %8256 ], [ %8286, %8261 ]
  %8265 = phi i64 [ 0, %8256 ], [ %8295, %8261 ]
  %8266 = getelementptr inbounds nuw i8, ptr %8252, i64 %8264
  %8267 = load i8, ptr %8266, align 1
  %8268 = zext i8 %8267 to i64
  %8269 = shl nsw i64 %8262, 3
  %8270 = shl i64 %8268, %8269
  %8271 = or i64 %8270, %8263
  %8272 = getelementptr i8, ptr %8258, i64 %8264
  %8273 = load i8, ptr %8272, align 1
  %8274 = zext i8 %8273 to i64
  %8275 = shl i64 %8262, 3
  %8276 = or disjoint i64 %8275, 8
  %8277 = shl i64 %8274, %8276
  %8278 = or i64 %8277, %8271
  %8279 = getelementptr i8, ptr %8259, i64 %8264
  %8280 = load i8, ptr %8279, align 1
  %8281 = zext i8 %8280 to i64
  %8282 = shl i64 %8262, 3
  %8283 = or disjoint i64 %8282, 16
  %8284 = shl i64 %8281, %8283
  %8285 = or i64 %8284, %8278
  %8286 = add i64 %8264, 4
  %8287 = getelementptr i8, ptr %8260, i64 %8264
  %8288 = load i8, ptr %8287, align 1
  %8289 = zext i8 %8288 to i64
  %8290 = shl i64 %8262, 3
  %8291 = or disjoint i64 %8290, 24
  %8292 = shl i64 %8289, %8291
  %8293 = or i64 %8292, %8285
  %8294 = add nuw nsw i64 %8262, 4
  %8295 = add i64 %8265, 4
  %8296 = icmp eq i64 %8295, %8257
  br i1 %8296, label %8325, label %8261, !llvm.loop !17

8297:                                             ; preds = %8192
  %8298 = icmp eq i8 %8199, %8206
  br i1 %8298, label %8346, label %8299

8299:                                             ; preds = %8297
  %8300 = sext i32 %8208 to i64
  %8301 = zext i8 %8207 to i64
  br label %8302

8302:                                             ; preds = %8299, %8302
  %8303 = phi i64 [ %8300, %8299 ], [ %8307, %8302 ]
  %8304 = phi i64 [ 0, %8299 ], [ %8323, %8302 ]
  %8305 = phi i64 [ 0, %8299 ], [ %8322, %8302 ]
  %8306 = phi i32 [ %8217, %8299 ], [ %8316, %8302 ]
  %8307 = add nsw i64 %8303, 1
  %8308 = trunc nsw i64 %8307 to i32
  store i32 %8308, ptr @ip, align 4
  %8309 = getelementptr inbounds i8, ptr %69, i64 %8303
  %8310 = load i8, ptr %8309, align 1
  %8311 = shl i32 %8306, 13
  %8312 = xor i32 %8311, %8306
  %8313 = lshr i32 %8312, 17
  %8314 = xor i32 %8313, %8312
  %8315 = shl i32 %8314, 5
  %8316 = xor i32 %8315, %8314
  store i32 %8316, ptr @vm_code_state, align 4
  %8317 = trunc i32 %8316 to i8
  %8318 = xor i8 %8310, %8317
  %8319 = zext i8 %8318 to i64
  %8320 = shl nsw i64 %8304, 3
  %8321 = shl i64 %8319, %8320
  %8322 = or i64 %8321, %8305
  %8323 = add nuw nsw i64 %8304, 1
  %8324 = icmp eq i64 %8323, %8301
  br i1 %8324, label %8346, label %8302, !llvm.loop !15

8325:                                             ; preds = %8261, %8250
  %8326 = phi i64 [ poison, %8250 ], [ %8293, %8261 ]
  %8327 = phi i64 [ 0, %8250 ], [ %8294, %8261 ]
  %8328 = phi i64 [ 0, %8250 ], [ %8293, %8261 ]
  %8329 = phi i64 [ %8248, %8250 ], [ %8286, %8261 ]
  %8330 = icmp eq i64 %8254, 0
  br i1 %8330, label %8346, label %8331

8331:                                             ; preds = %8325, %8331
  %8332 = phi i64 [ %8343, %8331 ], [ %8327, %8325 ]
  %8333 = phi i64 [ %8342, %8331 ], [ %8328, %8325 ]
  %8334 = phi i64 [ %8336, %8331 ], [ %8329, %8325 ]
  %8335 = phi i64 [ %8344, %8331 ], [ 0, %8325 ]
  %8336 = add i64 %8334, 1
  %8337 = getelementptr inbounds nuw i8, ptr %8252, i64 %8334
  %8338 = load i8, ptr %8337, align 1
  %8339 = zext i8 %8338 to i64
  %8340 = shl nsw i64 %8332, 3
  %8341 = shl i64 %8339, %8340
  %8342 = or i64 %8341, %8333
  %8343 = add nuw nsw i64 %8332, 1
  %8344 = add i64 %8335, 1
  %8345 = icmp eq i64 %8344, %8254
  br i1 %8345, label %8346, label %8331, !llvm.loop !78

8346:                                             ; preds = %8302, %8325, %8331, %8297, %8247
  %8347 = phi i64 [ 0, %8247 ], [ 0, %8297 ], [ %8326, %8325 ], [ %8342, %8331 ], [ %8322, %8302 ]
  %8348 = icmp eq i8 %8148, %8155
  br i1 %8348, label %8784, label %8349

8349:                                             ; preds = %8346
  %8350 = load i64, ptr @data_seg_addr, align 8
  %8351 = add i64 %8350, %8195
  %8352 = inttoptr i64 %8351 to ptr
  %8353 = zext i8 %8156 to i32
  %8354 = and i32 %8353, 7
  %8355 = icmp ult i8 %8156, 8
  br i1 %8355, label %8771, label %8356

8356:                                             ; preds = %8349
  %8357 = and i32 %8353, 248
  br label %8358

8358:                                             ; preds = %8358, %8356
  %8359 = phi ptr [ %8352, %8356 ], [ %8384, %8358 ]
  %8360 = phi i64 [ %8347, %8356 ], [ 0, %8358 ]
  %8361 = phi i32 [ 0, %8356 ], [ %8385, %8358 ]
  %8362 = trunc i64 %8360 to i8
  store i8 %8362, ptr %8359, align 1
  %8363 = getelementptr inbounds nuw i8, ptr %8359, i64 1
  %8364 = lshr i64 %8360, 8
  %8365 = trunc i64 %8364 to i8
  store i8 %8365, ptr %8363, align 1
  %8366 = getelementptr inbounds nuw i8, ptr %8359, i64 2
  %8367 = lshr i64 %8360, 16
  %8368 = trunc i64 %8367 to i8
  store i8 %8368, ptr %8366, align 1
  %8369 = getelementptr inbounds nuw i8, ptr %8359, i64 3
  %8370 = lshr i64 %8360, 24
  %8371 = trunc i64 %8370 to i8
  store i8 %8371, ptr %8369, align 1
  %8372 = getelementptr inbounds nuw i8, ptr %8359, i64 4
  %8373 = lshr i64 %8360, 32
  %8374 = trunc i64 %8373 to i8
  store i8 %8374, ptr %8372, align 1
  %8375 = getelementptr inbounds nuw i8, ptr %8359, i64 5
  %8376 = lshr i64 %8360, 40
  %8377 = trunc i64 %8376 to i8
  store i8 %8377, ptr %8375, align 1
  %8378 = getelementptr inbounds nuw i8, ptr %8359, i64 6
  %8379 = lshr i64 %8360, 48
  %8380 = trunc i64 %8379 to i8
  store i8 %8380, ptr %8378, align 1
  %8381 = getelementptr inbounds nuw i8, ptr %8359, i64 7
  %8382 = lshr i64 %8360, 56
  %8383 = trunc nuw i64 %8382 to i8
  store i8 %8383, ptr %8381, align 1
  %8384 = getelementptr inbounds nuw i8, ptr %8359, i64 8
  %8385 = add i32 %8361, 8
  %8386 = icmp eq i32 %8385, %8357
  br i1 %8386, label %8771, label %8358, !llvm.loop !16

8387:                                             ; preds = %158
  %8388 = add nsw i32 %72, 2
  store i32 %8388, ptr @ip, align 4
  %8389 = sext i32 %73 to i64
  %8390 = getelementptr inbounds i8, ptr %69, i64 %8389
  %8391 = load i8, ptr %8390, align 1
  %8392 = shl i32 %81, 13
  %8393 = xor i32 %8392, %81
  %8394 = lshr i32 %8393, 17
  %8395 = xor i32 %8394, %8393
  %8396 = shl i32 %8395, 5
  %8397 = xor i32 %8396, %8395
  store i32 %8397, ptr @vm_code_state, align 4
  %8398 = trunc i32 %8397 to i8
  %8399 = xor i8 %8391, %8398
  %8400 = add nsw i32 %72, 3
  store i32 %8400, ptr @ip, align 4
  %8401 = sext i32 %8388 to i64
  %8402 = getelementptr inbounds i8, ptr %69, i64 %8401
  %8403 = load i8, ptr %8402, align 1
  %8404 = shl i32 %8397, 13
  %8405 = xor i32 %8404, %8397
  %8406 = lshr i32 %8405, 17
  %8407 = xor i32 %8406, %8405
  %8408 = shl i32 %8407, 5
  %8409 = xor i32 %8408, %8407
  store i32 %8409, ptr @vm_code_state, align 4
  %8410 = trunc i32 %8409 to i8
  %8411 = xor i8 %8403, %8410
  %8412 = icmp eq i8 %8403, %8410
  br i1 %8412, label %8413, label %8492

8413:                                             ; preds = %8387
  %8414 = load i32, ptr @pointer_size, align 4
  %8415 = icmp sgt i32 %8414, 0
  br i1 %8415, label %8416, label %8442

8416:                                             ; preds = %8413
  %8417 = sext i32 %8400 to i64
  %8418 = zext nneg i32 %8414 to i64
  br label %8419

8419:                                             ; preds = %8416, %8419
  %8420 = phi i64 [ %8417, %8416 ], [ %8424, %8419 ]
  %8421 = phi i64 [ 0, %8416 ], [ %8440, %8419 ]
  %8422 = phi i64 [ 0, %8416 ], [ %8439, %8419 ]
  %8423 = phi i32 [ %8409, %8416 ], [ %8433, %8419 ]
  %8424 = add nsw i64 %8420, 1
  %8425 = trunc nsw i64 %8424 to i32
  store i32 %8425, ptr @ip, align 4
  %8426 = getelementptr inbounds i8, ptr %69, i64 %8420
  %8427 = load i8, ptr %8426, align 1
  %8428 = shl i32 %8423, 13
  %8429 = xor i32 %8428, %8423
  %8430 = lshr i32 %8429, 17
  %8431 = xor i32 %8430, %8429
  %8432 = shl i32 %8431, 5
  %8433 = xor i32 %8432, %8431
  store i32 %8433, ptr @vm_code_state, align 4
  %8434 = trunc i32 %8433 to i8
  %8435 = xor i8 %8427, %8434
  %8436 = zext i8 %8435 to i64
  %8437 = shl nsw i64 %8421, 3
  %8438 = shl i64 %8436, %8437
  %8439 = or i64 %8438, %8422
  %8440 = add nuw nsw i64 %8421, 1
  %8441 = icmp eq i64 %8440, %8418
  br i1 %8441, label %8442, label %8419, !llvm.loop !15

8442:                                             ; preds = %8419, %8413
  %8443 = phi i64 [ 0, %8413 ], [ %8439, %8419 ]
  %8444 = icmp eq i8 %8391, %8398
  br i1 %8444, label %8541, label %8445

8445:                                             ; preds = %8442
  %8446 = load i64, ptr @data_seg_addr, align 8
  %8447 = inttoptr i64 %8446 to ptr
  %8448 = zext i8 %8399 to i64
  %8449 = and i64 %8448, 3
  %8450 = icmp ult i8 %8399, 4
  br i1 %8450, label %8520, label %8451

8451:                                             ; preds = %8445
  %8452 = and i64 %8448, 252
  %8453 = getelementptr i8, ptr %8447, i64 1
  %8454 = getelementptr i8, ptr %8447, i64 2
  %8455 = getelementptr i8, ptr %8447, i64 3
  br label %8456

8456:                                             ; preds = %8456, %8451
  %8457 = phi i64 [ 0, %8451 ], [ %8489, %8456 ]
  %8458 = phi i64 [ 0, %8451 ], [ %8488, %8456 ]
  %8459 = phi i64 [ %8443, %8451 ], [ %8481, %8456 ]
  %8460 = phi i64 [ 0, %8451 ], [ %8490, %8456 ]
  %8461 = getelementptr inbounds nuw i8, ptr %8447, i64 %8459
  %8462 = load i8, ptr %8461, align 1
  %8463 = zext i8 %8462 to i64
  %8464 = shl nsw i64 %8457, 3
  %8465 = shl i64 %8463, %8464
  %8466 = or i64 %8465, %8458
  %8467 = getelementptr i8, ptr %8453, i64 %8459
  %8468 = load i8, ptr %8467, align 1
  %8469 = zext i8 %8468 to i64
  %8470 = shl i64 %8457, 3
  %8471 = or disjoint i64 %8470, 8
  %8472 = shl i64 %8469, %8471
  %8473 = or i64 %8472, %8466
  %8474 = getelementptr i8, ptr %8454, i64 %8459
  %8475 = load i8, ptr %8474, align 1
  %8476 = zext i8 %8475 to i64
  %8477 = shl i64 %8457, 3
  %8478 = or disjoint i64 %8477, 16
  %8479 = shl i64 %8476, %8478
  %8480 = or i64 %8479, %8473
  %8481 = add i64 %8459, 4
  %8482 = getelementptr i8, ptr %8455, i64 %8459
  %8483 = load i8, ptr %8482, align 1
  %8484 = zext i8 %8483 to i64
  %8485 = shl i64 %8457, 3
  %8486 = or disjoint i64 %8485, 24
  %8487 = shl i64 %8484, %8486
  %8488 = or i64 %8487, %8480
  %8489 = add nuw nsw i64 %8457, 4
  %8490 = add i64 %8460, 4
  %8491 = icmp eq i64 %8490, %8452
  br i1 %8491, label %8520, label %8456, !llvm.loop !17

8492:                                             ; preds = %8387
  %8493 = icmp eq i8 %8391, %8398
  br i1 %8493, label %8541, label %8494

8494:                                             ; preds = %8492
  %8495 = sext i32 %8400 to i64
  %8496 = zext i8 %8399 to i64
  br label %8497

8497:                                             ; preds = %8494, %8497
  %8498 = phi i64 [ %8495, %8494 ], [ %8502, %8497 ]
  %8499 = phi i64 [ 0, %8494 ], [ %8518, %8497 ]
  %8500 = phi i64 [ 0, %8494 ], [ %8517, %8497 ]
  %8501 = phi i32 [ %8409, %8494 ], [ %8511, %8497 ]
  %8502 = add nsw i64 %8498, 1
  %8503 = trunc nsw i64 %8502 to i32
  store i32 %8503, ptr @ip, align 4
  %8504 = getelementptr inbounds i8, ptr %69, i64 %8498
  %8505 = load i8, ptr %8504, align 1
  %8506 = shl i32 %8501, 13
  %8507 = xor i32 %8506, %8501
  %8508 = lshr i32 %8507, 17
  %8509 = xor i32 %8508, %8507
  %8510 = shl i32 %8509, 5
  %8511 = xor i32 %8510, %8509
  store i32 %8511, ptr @vm_code_state, align 4
  %8512 = trunc i32 %8511 to i8
  %8513 = xor i8 %8505, %8512
  %8514 = zext i8 %8513 to i64
  %8515 = shl nsw i64 %8499, 3
  %8516 = shl i64 %8514, %8515
  %8517 = or i64 %8516, %8500
  %8518 = add nuw nsw i64 %8499, 1
  %8519 = icmp eq i64 %8518, %8496
  br i1 %8519, label %8541, label %8497, !llvm.loop !15

8520:                                             ; preds = %8456, %8445
  %8521 = phi i64 [ poison, %8445 ], [ %8488, %8456 ]
  %8522 = phi i64 [ 0, %8445 ], [ %8489, %8456 ]
  %8523 = phi i64 [ 0, %8445 ], [ %8488, %8456 ]
  %8524 = phi i64 [ %8443, %8445 ], [ %8481, %8456 ]
  %8525 = icmp eq i64 %8449, 0
  br i1 %8525, label %8541, label %8526

8526:                                             ; preds = %8520, %8526
  %8527 = phi i64 [ %8538, %8526 ], [ %8522, %8520 ]
  %8528 = phi i64 [ %8537, %8526 ], [ %8523, %8520 ]
  %8529 = phi i64 [ %8531, %8526 ], [ %8524, %8520 ]
  %8530 = phi i64 [ %8539, %8526 ], [ 0, %8520 ]
  %8531 = add i64 %8529, 1
  %8532 = getelementptr inbounds nuw i8, ptr %8447, i64 %8529
  %8533 = load i8, ptr %8532, align 1
  %8534 = zext i8 %8533 to i64
  %8535 = shl nsw i64 %8527, 3
  %8536 = shl i64 %8534, %8535
  %8537 = or i64 %8536, %8528
  %8538 = add nuw nsw i64 %8527, 1
  %8539 = add i64 %8530, 1
  %8540 = icmp eq i64 %8539, %8449
  br i1 %8540, label %8541, label %8526, !llvm.loop !79

8541:                                             ; preds = %8497, %8520, %8526, %8492, %8442
  %8542 = phi i64 [ 0, %8442 ], [ 0, %8492 ], [ %8521, %8520 ], [ %8537, %8526 ], [ %8517, %8497 ]
  %8543 = or i8 %8411, %8399
  %8544 = icmp eq i8 %8543, 0
  %8545 = icmp eq i8 %8391, %8398
  %8546 = select i1 %8544, i1 true, i1 %8545
  br i1 %8546, label %8800, label %8547

8547:                                             ; preds = %8541
  %8548 = load i64, ptr @data_seg_addr, align 8
  %8549 = inttoptr i64 %8548 to ptr
  %8550 = zext i8 %8399 to i32
  %8551 = and i32 %8550, 7
  %8552 = icmp ult i8 %8399, 8
  br i1 %8552, label %8787, label %8553

8553:                                             ; preds = %8547
  %8554 = and i32 %8550, 248
  br label %8555

8555:                                             ; preds = %8555, %8553
  %8556 = phi ptr [ %8549, %8553 ], [ %8581, %8555 ]
  %8557 = phi i64 [ %8542, %8553 ], [ 0, %8555 ]
  %8558 = phi i32 [ 0, %8553 ], [ %8582, %8555 ]
  %8559 = trunc i64 %8557 to i8
  store i8 %8559, ptr %8556, align 1
  %8560 = getelementptr inbounds nuw i8, ptr %8556, i64 1
  %8561 = lshr i64 %8557, 8
  %8562 = trunc i64 %8561 to i8
  store i8 %8562, ptr %8560, align 1
  %8563 = getelementptr inbounds nuw i8, ptr %8556, i64 2
  %8564 = lshr i64 %8557, 16
  %8565 = trunc i64 %8564 to i8
  store i8 %8565, ptr %8563, align 1
  %8566 = getelementptr inbounds nuw i8, ptr %8556, i64 3
  %8567 = lshr i64 %8557, 24
  %8568 = trunc i64 %8567 to i8
  store i8 %8568, ptr %8566, align 1
  %8569 = getelementptr inbounds nuw i8, ptr %8556, i64 4
  %8570 = lshr i64 %8557, 32
  %8571 = trunc i64 %8570 to i8
  store i8 %8571, ptr %8569, align 1
  %8572 = getelementptr inbounds nuw i8, ptr %8556, i64 5
  %8573 = lshr i64 %8557, 40
  %8574 = trunc i64 %8573 to i8
  store i8 %8574, ptr %8572, align 1
  %8575 = getelementptr inbounds nuw i8, ptr %8556, i64 6
  %8576 = lshr i64 %8557, 48
  %8577 = trunc i64 %8576 to i8
  store i8 %8577, ptr %8575, align 1
  %8578 = getelementptr inbounds nuw i8, ptr %8556, i64 7
  %8579 = lshr i64 %8557, 56
  %8580 = trunc nuw i64 %8579 to i8
  store i8 %8580, ptr %8578, align 1
  %8581 = getelementptr inbounds nuw i8, ptr %8556, i64 8
  %8582 = add i32 %8558, 8
  %8583 = icmp eq i32 %8582, %8554
  br i1 %8583, label %8787, label %8555, !llvm.loop !16

8584:                                             ; preds = %158
  %8585 = load i32, ptr @pointer_size, align 4
  %8586 = icmp sgt i32 %8585, 0
  br i1 %8586, label %8587, label %8613

8587:                                             ; preds = %8584
  %8588 = sext i32 %73 to i64
  %8589 = zext nneg i32 %8585 to i64
  br label %8590

8590:                                             ; preds = %8587, %8590
  %8591 = phi i64 [ %8588, %8587 ], [ %8595, %8590 ]
  %8592 = phi i64 [ 0, %8587 ], [ %8611, %8590 ]
  %8593 = phi i64 [ 0, %8587 ], [ %8610, %8590 ]
  %8594 = phi i32 [ %81, %8587 ], [ %8604, %8590 ]
  %8595 = add nsw i64 %8591, 1
  %8596 = trunc nsw i64 %8595 to i32
  store i32 %8596, ptr @ip, align 4
  %8597 = getelementptr inbounds i8, ptr %69, i64 %8591
  %8598 = load i8, ptr %8597, align 1
  %8599 = shl i32 %8594, 13
  %8600 = xor i32 %8599, %8594
  %8601 = lshr i32 %8600, 17
  %8602 = xor i32 %8601, %8600
  %8603 = shl i32 %8602, 5
  %8604 = xor i32 %8603, %8602
  store i32 %8604, ptr @vm_code_state, align 4
  %8605 = trunc i32 %8604 to i8
  %8606 = xor i8 %8598, %8605
  %8607 = zext i8 %8606 to i64
  %8608 = shl nsw i64 %8592, 3
  %8609 = shl i64 %8607, %8608
  %8610 = or i64 %8609, %8593
  %8611 = add nuw nsw i64 %8592, 1
  %8612 = icmp eq i64 %8611, %8589
  br i1 %8612, label %8613, label %8590, !llvm.loop !15

8613:                                             ; preds = %8590, %8584
  %8614 = phi i64 [ 0, %8584 ], [ %8610, %8590 ]
  tail call void @call_handler(i64 noundef %8614) #4
  br label %8784

8615:                                             ; preds = %268, %258
  %8616 = phi ptr [ %261, %258 ], [ %294, %268 ]
  %8617 = phi i64 [ %262, %258 ], [ 0, %268 ]
  %8618 = icmp eq i32 %264, 0
  br i1 %8618, label %8784, label %8619

8619:                                             ; preds = %8615, %8619
  %8620 = phi ptr [ %8624, %8619 ], [ %8616, %8615 ]
  %8621 = phi i64 [ %8625, %8619 ], [ %8617, %8615 ]
  %8622 = phi i32 [ %8626, %8619 ], [ 0, %8615 ]
  %8623 = trunc i64 %8621 to i8
  store i8 %8623, ptr %8620, align 1
  %8624 = getelementptr inbounds nuw i8, ptr %8620, i64 1
  %8625 = lshr i64 %8621, 8
  %8626 = add i32 %8622, 1
  %8627 = icmp eq i32 %8626, %264
  br i1 %8627, label %8784, label %8619, !llvm.loop !80

8628:                                             ; preds = %532, %523
  %8629 = phi ptr [ %527, %523 ], [ %558, %532 ]
  %8630 = phi i64 [ %524, %523 ], [ 0, %532 ]
  %8631 = icmp eq i32 %528, 0
  br i1 %8631, label %8784, label %8632

8632:                                             ; preds = %8628, %8632
  %8633 = phi ptr [ %8637, %8632 ], [ %8629, %8628 ]
  %8634 = phi i64 [ %8638, %8632 ], [ %8630, %8628 ]
  %8635 = phi i32 [ %8639, %8632 ], [ 0, %8628 ]
  %8636 = trunc i64 %8634 to i8
  store i8 %8636, ptr %8633, align 1
  %8637 = getelementptr inbounds nuw i8, ptr %8633, i64 1
  %8638 = lshr i64 %8634, 8
  %8639 = add i32 %8635, 1
  %8640 = icmp eq i32 %8639, %528
  br i1 %8640, label %8784, label %8632, !llvm.loop !81

8641:                                             ; preds = %840, %834
  %8642 = phi ptr [ %832, %834 ], [ %866, %840 ]
  %8643 = phi i64 [ %719, %834 ], [ 0, %840 ]
  %8644 = icmp eq i32 %836, 0
  br i1 %8644, label %8784, label %8645

8645:                                             ; preds = %8641, %8645
  %8646 = phi ptr [ %8650, %8645 ], [ %8642, %8641 ]
  %8647 = phi i64 [ %8651, %8645 ], [ %8643, %8641 ]
  %8648 = phi i32 [ %8652, %8645 ], [ 0, %8641 ]
  %8649 = trunc i64 %8647 to i8
  store i8 %8649, ptr %8646, align 1
  %8650 = getelementptr inbounds nuw i8, ptr %8646, i64 1
  %8651 = lshr i64 %8647, 8
  %8652 = add i32 %8648, 1
  %8653 = icmp eq i32 %8652, %836
  br i1 %8653, label %8784, label %8645, !llvm.loop !82

8654:                                             ; preds = %1504, %1495
  %8655 = phi ptr [ %1498, %1495 ], [ %1530, %1504 ]
  %8656 = phi i64 [ %1493, %1495 ], [ 0, %1504 ]
  %8657 = icmp eq i32 %1500, 0
  br i1 %8657, label %8784, label %8658

8658:                                             ; preds = %8654, %8658
  %8659 = phi ptr [ %8663, %8658 ], [ %8655, %8654 ]
  %8660 = phi i64 [ %8664, %8658 ], [ %8656, %8654 ]
  %8661 = phi i32 [ %8665, %8658 ], [ 0, %8654 ]
  %8662 = trunc i64 %8660 to i8
  store i8 %8662, ptr %8659, align 1
  %8663 = getelementptr inbounds nuw i8, ptr %8659, i64 1
  %8664 = lshr i64 %8660, 8
  %8665 = add i32 %8661, 1
  %8666 = icmp eq i32 %8665, %1500
  br i1 %8666, label %8784, label %8658, !llvm.loop !83

8667:                                             ; preds = %1933, %1917
  %8668 = phi ptr [ %1920, %1917 ], [ %1959, %1933 ]
  %8669 = phi i64 [ %1928, %1917 ], [ 0, %1933 ]
  %8670 = icmp eq i32 %1929, 0
  br i1 %8670, label %8784, label %8671

8671:                                             ; preds = %8667, %8671
  %8672 = phi ptr [ %8676, %8671 ], [ %8668, %8667 ]
  %8673 = phi i64 [ %8677, %8671 ], [ %8669, %8667 ]
  %8674 = phi i32 [ %8678, %8671 ], [ 0, %8667 ]
  %8675 = trunc i64 %8673 to i8
  store i8 %8675, ptr %8672, align 1
  %8676 = getelementptr inbounds nuw i8, ptr %8672, i64 1
  %8677 = lshr i64 %8673, 8
  %8678 = add i32 %8674, 1
  %8679 = icmp eq i32 %8678, %1929
  br i1 %8679, label %8784, label %8671, !llvm.loop !84

8680:                                             ; preds = %2827, %2818
  %8681 = phi ptr [ %2821, %2818 ], [ %2853, %2827 ]
  %8682 = phi i64 [ %2816, %2818 ], [ 0, %2827 ]
  %8683 = icmp eq i32 %2823, 0
  br i1 %8683, label %8784, label %8684

8684:                                             ; preds = %8680, %8684
  %8685 = phi ptr [ %8689, %8684 ], [ %8681, %8680 ]
  %8686 = phi i64 [ %8690, %8684 ], [ %8682, %8680 ]
  %8687 = phi i32 [ %8691, %8684 ], [ 0, %8680 ]
  %8688 = trunc i64 %8686 to i8
  store i8 %8688, ptr %8685, align 1
  %8689 = getelementptr inbounds nuw i8, ptr %8685, i64 1
  %8690 = lshr i64 %8686, 8
  %8691 = add i32 %8687, 1
  %8692 = icmp eq i32 %8691, %2823
  br i1 %8692, label %8784, label %8684, !llvm.loop !85

8693:                                             ; preds = %4331, %4303
  %8694 = phi i64 [ 0, %4303 ], [ %4353, %4331 ]
  %8695 = icmp eq i64 %4308, 0
  br i1 %8695, label %8784, label %8696

8696:                                             ; preds = %8693, %8696
  %8697 = phi i64 [ %8703, %8696 ], [ %8694, %8693 ]
  %8698 = phi i64 [ %8704, %8696 ], [ 0, %8693 ]
  %8699 = shl nuw nsw i64 %8697, 3
  %8700 = lshr i64 %3985, %8699
  %8701 = trunc i64 %8700 to i8
  %8702 = getelementptr i8, ptr %4306, i64 %8697
  store i8 %8701, ptr %8702, align 1
  %8703 = add nuw nsw i64 %8697, 1
  %8704 = add i64 %8698, 1
  %8705 = icmp eq i64 %8704, %4308
  br i1 %8705, label %8784, label %8696, !llvm.loop !86

8706:                                             ; preds = %4818, %4809
  %8707 = phi ptr [ %4812, %4809 ], [ %4844, %4818 ]
  %8708 = phi i64 [ %4807, %4809 ], [ 0, %4818 ]
  %8709 = icmp eq i32 %4814, 0
  br i1 %8709, label %8784, label %8710

8710:                                             ; preds = %8706, %8710
  %8711 = phi ptr [ %8715, %8710 ], [ %8707, %8706 ]
  %8712 = phi i64 [ %8716, %8710 ], [ %8708, %8706 ]
  %8713 = phi i32 [ %8717, %8710 ], [ 0, %8706 ]
  %8714 = trunc i64 %8712 to i8
  store i8 %8714, ptr %8711, align 1
  %8715 = getelementptr inbounds nuw i8, ptr %8711, i64 1
  %8716 = lshr i64 %8712, 8
  %8717 = add i32 %8713, 1
  %8718 = icmp eq i32 %8717, %4814
  br i1 %8718, label %8784, label %8710, !llvm.loop !87

8719:                                             ; preds = %5019, %5011
  %8720 = phi ptr [ %5013, %5011 ], [ %5045, %5019 ]
  %8721 = phi i64 [ %5009, %5011 ], [ 0, %5019 ]
  %8722 = icmp eq i32 %5015, 0
  br i1 %8722, label %8784, label %8723

8723:                                             ; preds = %8719, %8723
  %8724 = phi ptr [ %8728, %8723 ], [ %8720, %8719 ]
  %8725 = phi i64 [ %8729, %8723 ], [ %8721, %8719 ]
  %8726 = phi i32 [ %8730, %8723 ], [ 0, %8719 ]
  %8727 = trunc i64 %8725 to i8
  store i8 %8727, ptr %8724, align 1
  %8728 = getelementptr inbounds nuw i8, ptr %8724, i64 1
  %8729 = lshr i64 %8725, 8
  %8730 = add i32 %8726, 1
  %8731 = icmp eq i32 %8730, %5015
  br i1 %8731, label %8784, label %8723, !llvm.loop !88

8732:                                             ; preds = %5757, %5746
  %8733 = phi ptr [ %5749, %5746 ], [ %5783, %5757 ]
  %8734 = phi i64 [ %5752, %5746 ], [ 0, %5757 ]
  %8735 = icmp eq i32 %5753, 0
  br i1 %8735, label %8784, label %8736

8736:                                             ; preds = %8732, %8736
  %8737 = phi ptr [ %8741, %8736 ], [ %8733, %8732 ]
  %8738 = phi i64 [ %8742, %8736 ], [ %8734, %8732 ]
  %8739 = phi i32 [ %8743, %8736 ], [ 0, %8732 ]
  %8740 = trunc i64 %8738 to i8
  store i8 %8740, ptr %8737, align 1
  %8741 = getelementptr inbounds nuw i8, ptr %8737, i64 1
  %8742 = lshr i64 %8738, 8
  %8743 = add i32 %8739, 1
  %8744 = icmp eq i32 %8743, %5753
  br i1 %8744, label %8784, label %8736, !llvm.loop !89

8745:                                             ; preds = %6562, %6553
  %8746 = phi ptr [ %6556, %6553 ], [ %6588, %6562 ]
  %8747 = phi i64 [ %6551, %6553 ], [ 0, %6562 ]
  %8748 = icmp eq i32 %6558, 0
  br i1 %8748, label %8784, label %8749

8749:                                             ; preds = %8745, %8749
  %8750 = phi ptr [ %8754, %8749 ], [ %8746, %8745 ]
  %8751 = phi i64 [ %8755, %8749 ], [ %8747, %8745 ]
  %8752 = phi i32 [ %8756, %8749 ], [ 0, %8745 ]
  %8753 = trunc i64 %8751 to i8
  store i8 %8753, ptr %8750, align 1
  %8754 = getelementptr inbounds nuw i8, ptr %8750, i64 1
  %8755 = lshr i64 %8751, 8
  %8756 = add i32 %8752, 1
  %8757 = icmp eq i32 %8756, %6558
  br i1 %8757, label %8784, label %8749, !llvm.loop !90

8758:                                             ; preds = %7292, %7273
  %8759 = phi ptr [ %7276, %7273 ], [ %7318, %7292 ]
  %8760 = phi i64 [ %6954, %7273 ], [ 0, %7292 ]
  %8761 = icmp eq i32 %7288, 0
  br i1 %8761, label %8784, label %8762

8762:                                             ; preds = %8758, %8762
  %8763 = phi ptr [ %8767, %8762 ], [ %8759, %8758 ]
  %8764 = phi i64 [ %8768, %8762 ], [ %8760, %8758 ]
  %8765 = phi i32 [ %8769, %8762 ], [ 0, %8758 ]
  %8766 = trunc i64 %8764 to i8
  store i8 %8766, ptr %8763, align 1
  %8767 = getelementptr inbounds nuw i8, ptr %8763, i64 1
  %8768 = lshr i64 %8764, 8
  %8769 = add i32 %8765, 1
  %8770 = icmp eq i32 %8769, %7288
  br i1 %8770, label %8784, label %8762, !llvm.loop !91

8771:                                             ; preds = %8358, %8349
  %8772 = phi ptr [ %8352, %8349 ], [ %8384, %8358 ]
  %8773 = phi i64 [ %8347, %8349 ], [ 0, %8358 ]
  %8774 = icmp eq i32 %8354, 0
  br i1 %8774, label %8784, label %8775

8775:                                             ; preds = %8771, %8775
  %8776 = phi ptr [ %8780, %8775 ], [ %8772, %8771 ]
  %8777 = phi i64 [ %8781, %8775 ], [ %8773, %8771 ]
  %8778 = phi i32 [ %8782, %8775 ], [ 0, %8771 ]
  %8779 = trunc i64 %8777 to i8
  store i8 %8779, ptr %8776, align 1
  %8780 = getelementptr inbounds nuw i8, ptr %8776, i64 1
  %8781 = lshr i64 %8777, 8
  %8782 = add i32 %8778, 1
  %8783 = icmp eq i32 %8782, %8354
  br i1 %8783, label %8784, label %8775, !llvm.loop !92

8784:                                             ; preds = %8771, %8775, %8141, %8758, %8762, %8745, %8749, %8732, %8736, %8719, %8723, %8706, %8710, %8693, %8696, %8680, %8684, %2521, %8667, %8671, %8654, %8658, %8641, %8645, %8628, %8632, %8615, %8619, %2496, %2516, %458, %2455, %8346, %7694, %7270, %6550, %5743, %5008, %4806, %4301, %2815, %2452, %1914, %1492, %831, %254, %158, %3144, %3619, %5886, %6056, %8613
  %8785 = phi i8 [ 1, %3144 ], [ 1, %3619 ], [ 0, %5886 ], [ 1, %6056 ], [ 0, %8613 ], [ 0, %158 ], [ 0, %254 ], [ 0, %831 ], [ 0, %1492 ], [ 0, %1914 ], [ 0, %2452 ], [ 0, %2815 ], [ 0, %4301 ], [ 0, %4806 ], [ 0, %5008 ], [ 0, %5743 ], [ 0, %6550 ], [ 0, %7270 ], [ 0, %7694 ], [ 0, %8346 ], [ 0, %2455 ], [ 0, %458 ], [ 0, %2516 ], [ 0, %2496 ], [ 0, %8619 ], [ 0, %8615 ], [ 0, %8632 ], [ 0, %8628 ], [ 0, %8645 ], [ 0, %8641 ], [ 0, %8658 ], [ 0, %8654 ], [ 0, %8671 ], [ 0, %8667 ], [ 0, %2521 ], [ 0, %8684 ], [ 0, %8680 ], [ 0, %8696 ], [ 0, %8693 ], [ 0, %8710 ], [ 0, %8706 ], [ 0, %8723 ], [ 0, %8719 ], [ 0, %8736 ], [ 0, %8732 ], [ 0, %8749 ], [ 0, %8745 ], [ 0, %8762 ], [ 0, %8758 ], [ 0, %8141 ], [ 0, %8775 ], [ 0, %8771 ]
  %8786 = load i32, ptr @ip, align 4
  br label %2

8787:                                             ; preds = %8555, %8547
  %8788 = phi ptr [ %8549, %8547 ], [ %8581, %8555 ]
  %8789 = phi i64 [ %8542, %8547 ], [ 0, %8555 ]
  %8790 = icmp eq i32 %8551, 0
  br i1 %8790, label %8800, label %8791

8791:                                             ; preds = %8787, %8791
  %8792 = phi ptr [ %8796, %8791 ], [ %8788, %8787 ]
  %8793 = phi i64 [ %8797, %8791 ], [ %8789, %8787 ]
  %8794 = phi i32 [ %8798, %8791 ], [ 0, %8787 ]
  %8795 = trunc i64 %8793 to i8
  store i8 %8795, ptr %8792, align 1
  %8796 = getelementptr inbounds nuw i8, ptr %8792, i64 1
  %8797 = lshr i64 %8793, 8
  %8798 = add i32 %8794, 1
  %8799 = icmp eq i32 %8798, %8551
  br i1 %8799, label %8800, label %8791, !llvm.loop !93

8800:                                             ; preds = %158, %8787, %8791, %84, %8541
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #1

declare dso_local void @call_handler(i64 noundef) local_unnamed_addr #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.fmuladd.f32(float, float, float) #3

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.fmuladd.f64(double, double, double) #3

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.umin.i32(i32, i32) #3

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #3

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "clang version 21.1.8 (https://github.com/abcdefgjh-li/ALLVM.git b1a2de6eace7f3ea4520c9515975ea298647a978)\0AA-protect https://github.com/abcdefgjh-li/ALLVM\0ABy abcdefgjh.\0A\E9\A3\8E\E6\AD\A2\E9\9B\BE\E6\95\A3\EF\BC\8C\E4\BD\A0\E6\88\91\E7\BB\88\E6\88\90\E9\99\8C\E8\B7\AF\E3\80\82\E6\94\BE\E4\B8\8B\E8\BF\87\E5\BE\80\EF\BC\8C\E5\A5\94\E8\B5\B4\E5\89\8D\E8\B7\AF\EF\BC\8C\E6\96\B9\E6\98\AF\E5\BD\92\E9\80\94\E3\80\82\0A", isOptimized: true, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "D:\\cpp\\obfuscator-ollvm-21.x\\aVMPInterpreter\\aVMPInterpreter.c", directory: "D:\\cpp\\obfuscator-ollvm-21.x")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 2}
!4 = !{i32 8, !"PIC Level", i32 2}
!5 = !{i32 7, !"uwtable", i32 2}
!6 = !{i32 1, !"MaxTLSAlign", i32 65536}
!7 = !{!"clang version 21.1.8 (https://github.com/abcdefgjh-li/ALLVM.git b1a2de6eace7f3ea4520c9515975ea298647a978)\0AA-protect https://github.com/abcdefgjh-li/ALLVM\0ABy abcdefgjh.\0A\E9\A3\8E\E6\AD\A2\E9\9B\BE\E6\95\A3\EF\BC\8C\E4\BD\A0\E6\88\91\E7\BB\88\E6\88\90\E9\99\8C\E8\B7\AF\E3\80\82\E6\94\BE\E4\B8\8B\E8\BF\87\E5\BE\80\EF\BC\8C\E5\A5\94\E8\B5\B4\E5\89\8D\E8\B7\AF\EF\BC\8C\E6\96\B9\E6\98\AF\E5\BD\92\E9\80\94\E3\80\82\0A"}
!8 = distinct !{!8, !9, !10, !11}
!9 = !{!"llvm.loop.mustprogress"}
!10 = !{!"llvm.loop.isvectorized", i32 1}
!11 = !{!"llvm.loop.unroll.runtime.disable"}
!12 = distinct !{!12, !9, !10, !11}
!13 = distinct !{!13, !9, !11, !10}
!14 = distinct !{!14, !9}
!15 = distinct !{!15, !9}
!16 = distinct !{!16, !9}
!17 = distinct !{!17, !9}
!18 = distinct !{!18, !19}
!19 = !{!"llvm.loop.unroll.disable"}
!20 = distinct !{!20, !9}
!21 = distinct !{!21, !19}
!22 = distinct !{!22, !19}
!23 = distinct !{!23, !19}
!24 = distinct !{!24, !19}
!25 = distinct !{!25, !19}
!26 = distinct !{!26, !19}
!27 = distinct !{!27, !19}
!28 = distinct !{!28, !19}
!29 = distinct !{!29, !19}
!30 = distinct !{!30, !19}
!31 = distinct !{!31, !9, !32, !10, !11}
!32 = !{!"llvm.loop.peeled.count", i32 1}
!33 = distinct !{!33, !9, !32, !10, !11}
!34 = distinct !{!34, !9, !32, !11, !10}
!35 = distinct !{!35, !19}
!36 = distinct !{!36, !19}
!37 = distinct !{!37, !19}
!38 = distinct !{!38, !9}
!39 = distinct !{!39, !9}
!40 = distinct !{!40, !19}
!41 = distinct !{!41, !19}
!42 = distinct !{!42, !19}
!43 = distinct !{!43, !19}
!44 = distinct !{!44, !9, !10, !11}
!45 = distinct !{!45, !9, !10, !11}
!46 = distinct !{!46, !9, !10}
!47 = distinct !{!47, !9}
!48 = distinct !{!48, !19}
!49 = distinct !{!49, !19}
!50 = distinct !{!50, !19}
!51 = distinct !{!51, !9}
!52 = distinct !{!52, !19}
!53 = distinct !{!53, !9}
!54 = distinct !{!54, !19}
!55 = distinct !{!55, !19}
!56 = distinct !{!56, !19}
!57 = distinct !{!57, !19}
!58 = distinct !{!58, !19}
!59 = distinct !{!59, !19}
!60 = distinct !{!60, !19}
!61 = distinct !{!61, !19}
!62 = distinct !{!62, !19}
!63 = distinct !{!63, !19}
!64 = distinct !{!64, !19}
!65 = distinct !{!65, !19}
!66 = distinct !{!66, !9, !10, !11}
!67 = distinct !{!67, !9, !10, !11}
!68 = distinct !{!68, !9, !10}
!69 = distinct !{!69, !19}
!70 = distinct !{!70, !19}
!71 = distinct !{!71, !9, !10, !11}
!72 = distinct !{!72, !9, !10, !11}
!73 = distinct !{!73, !9, !10, !11}
!74 = distinct !{!74, !9, !10, !11}
!75 = distinct !{!75, !9, !10}
!76 = distinct !{!76, !9, !10}
!77 = distinct !{!77, !9}
!78 = distinct !{!78, !19}
!79 = distinct !{!79, !19}
!80 = distinct !{!80, !19}
!81 = distinct !{!81, !19}
!82 = distinct !{!82, !19}
!83 = distinct !{!83, !19}
!84 = distinct !{!84, !19}
!85 = distinct !{!85, !19}
!86 = distinct !{!86, !19}
!87 = distinct !{!87, !19}
!88 = distinct !{!88, !19}
!89 = distinct !{!89, !19}
!90 = distinct !{!90, !19}
!91 = distinct !{!91, !19}
!92 = distinct !{!92, !19}
!93 = distinct !{!93, !19}
