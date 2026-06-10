// Enable debug output unconditionally for testing
// #define GOVM_CPP_DEBUG 1

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <setjmp.h>
#include "aVMPInterpreter.h"

// C++ 异常处理支持
#ifdef __cplusplus
#include <exception>
#include <typeinfo>
#endif

// 异常处理相关的全局变量
static jmp_buf exception_jmp_buf;
static void* caught_exception_ptr = NULL;
static int caught_exception_selector = 0;

#ifdef __cplusplus
#include <exception>
static std::exception_ptr current_exception_ptr;
#endif

// C++ 异常捕获包装函数
#ifdef __cplusplus
extern "C" {
#endif

// 包装 call_handler 以捕获 C++ 异常
void call_handler_with_exception_handling(uint64_t targetfunc_id) {
#ifdef __cplusplus
	try {
		call_handler(targetfunc_id);
	} catch (...) {
		// 捕获所有异常
		current_exception_ptr = std::current_exception();

		try {
			std::rethrow_exception(current_exception_ptr);
		} catch (const std::exception& e) {
			// 标准异常
			caught_exception_ptr = (void*)&e;
			caught_exception_selector = 1;
			exception_thrown = 1;

#ifdef GOVM_CPP_DEBUG
			printf("[CALL_HANDLER] Caught std::exception: %s\n", e.what());
			fflush(NULL);
#endif
		} catch (...) {
			// 其他异常
			caught_exception_ptr = (void*)0x1;
			caught_exception_selector = 2;
			exception_thrown = 1;

#ifdef GOVM_CPP_DEBUG
			printf("[CALL_HANDLER] Caught unknown exception\n");
			fflush(NULL);
#endif
		}
	}
#else
	// C 环境，直接调用
	call_handler(targetfunc_id);
#endif
}

#ifdef __cplusplus
}
#endif

// Debug functions - implemented by VMP pass via IR injection
// Controlled by -irobf-debug flag
extern void vmp_debug_id(int id, uint64_t val);

// Debug mode control - set by VMP pass when -irobf-debug is enabled
// 0 = debug disabled (default), 1 = debug enabled
extern uint8_t vmp_debug_enabled;

// Debug IDs
#define DEBUG_ID_NEW_BB     1
#define DEBUG_ID_OPCODE     2
#define DEBUG_ID_CMP        3
#define DEBUG_ID_CMP_PRED   4
#define DEBUG_ID_CMP_OP1    5
#define DEBUG_ID_CMP_OP2    6
#define DEBUG_ID_CMP_RES    7
#define DEBUG_ID_SEED_OP    8
#define DEBUG_ID_SEED_VM    9
#define DEBUG_ID_IP         10

// DEBUG macro: only outputs when vmp_debug_enabled is set
#define DEBUG(id, val) do { if (vmp_debug_enabled) vmp_debug_id(id, val); } while(0)

#define SEG_SIZE 5000

#define IS_INLINE_FUNC

// #define TEST_GOVM_C

uint8_t gv_code_seg[SEG_SIZE] = {
#ifdef TEST_GOVM_C
	//  0, 8, 0, 12, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 28, 0, 0, 0, 0, 0, 0, 0, 36, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 48, 0, 0, 0, 0, 0, 0, 0, 17, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 12, 0, 0, 0, 0, 0, 0, 0, 17, 4, 0, 8, 0, 0, 0, 0, 0, 0, 0, 8, 0, 28, 0, 0, 0, 0, 0, 0, 0, 17, 4, 11, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 96, 0, 126, 0, 0, 0, 0, 0, 0, 0, 16, 4, 0, 52, 0, 0, 0, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 16, 4, 0, 56, 0, 0, 0, 0, 0, 0, 0, 8, 0, 28, 0, 0, 0, 0, 0, 0, 0, 64, 40, 1, 0, 60, 0, 0, 0, 0, 0, 0, 0, 4, 0, 52, 0, 0, 0, 0, 0, 0, 0, 4, 0, 56, 0, 0, 0, 0, 0, 0, 0, 96, 1, 1, 0, 60, 0, 0, 0, 0, 0, 0, 0, 228, 0, 0, 0, 0, 0, 0, 0, 14, 2, 0, 0, 0, 0, 0, 0, 16, 8, 0, 61, 0, 0, 0, 0, 0, 0, 0, 8, 0, 12, 0, 0, 0, 0, 0, 0, 0, 16, 4, 0, 69, 0, 0, 0, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 80, 8, 0, 73, 0, 0, 0, 0, 0, 0, 0, 4, 0, 69, 0, 0, 0, 0, 0, 0, 0, 48, 1, 11, 8, 0, 81, 0, 0, 0, 0, 0, 0, 0, 8, 0, 61, 0, 0, 0, 0, 0, 0, 0, 8, 0, 73, 0, 0, 0, 0, 0, 0, 0, 16, 1, 0, 89, 0, 0, 0, 0, 0, 0, 0, 8, 0, 81, 0, 0, 0, 0, 0, 0, 0, 80, 4, 0, 90, 0, 0, 0, 0, 0, 0, 0, 1, 0, 89, 0, 0, 0, 0, 0, 0, 0, 32, 29, 4, 0, 94, 0, 0, 0, 0, 0, 0, 0, 4, 0, 90, 0, 0, 0, 0, 0, 0, 0, 4, 11, 58, 0, 0, 0, 80, 1, 0, 98, 0, 0, 0, 0, 0, 0, 0, 4, 0, 94, 0, 0, 0, 0, 0, 0, 0, 17, 1, 0, 98, 0, 0, 0, 0, 0, 0, 0, 8, 0, 81, 0, 0, 0, 0, 0, 0, 0, 96, 0, 190, 1, 0, 0, 0, 0, 0, 0, 16, 4, 0, 99, 0, 0, 0, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 32, 12, 4, 0, 103, 0, 0, 0, 0, 0, 0, 0, 4, 0, 99, 0, 0, 0, 0, 0, 0, 0, 4, 11, 1, 0, 0, 0, 17, 4, 0, 103, 0, 0, 0, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 96, 0, 126, 0, 0, 0, 0, 0, 0, 0, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	243, 149, 17, 126, 14, 92, 244, 62, 16, 249, 39, 46, 15, 215, 218, 109, 247, 148, 139, 153, 184, 179, 1, 197, 71, 203, 253, 79, 245, 251, 238, 205, 163, 80, 118, 201, 130, 155, 240, 35, 189, 175, 219, 162, 206, 47, 50, 86, 244, 152, 43, 206, 127, 7, 96, 119, 37, 102, 184, 254, 124, 155, 77, 58, 139, 170, 132, 121, 240, 116, 126, 214, 205, 100, 252, 152, 52, 140, 120, 219, 59, 193, 209, 118, 220, 192, 214, 152, 143, 83, 100, 77, 57, 188, 93, 250, 68, 63, 108, 155, 22, 167, 228, 220, 170, 140, 10, 45, 104, 130, 211, 251, 19, 57, 102, 108, 126, 85, 231, 187, 100, 93, 150, 50, 253, 3, 33, 213, 60, 239, 3, 11, 161, 97, 25, 34, 219, 88, 79, 248, 180, 38, 6, 212, 162, 154, 2, 134, 179, 143, 126, 212, 140, 97, 130, 46, 230, 127, 234, 144, 237, 169, 186, 228, 86, 179, 113, 217, 52, 175, 203, 137, 105, 211, 180, 119, 188, 10, 32, 95, 209, 108, 233, 12, 99, 194, 144, 72, 35, 10, 121, 124, 8, 71, 170, 33, 90, 158, 233, 11, 65, 73, 142, 49, 188, 210, 200, 250, 146, 117, 24, 16, 129, 114, 246, 70, 178, 106, 216, 100, 147, 138, 111, 235, 36, 159, 86, 130, 47, 136, 128, 246, 187, 98, 226, 242, 240, 148, 170, 161, 171, 34, 24, 131, 73, 13, 7, 7, 156, 237, 190, 53, 75, 168, 231, 138, 0, 39, 130, 190, 155, 242, 130, 173, 20, 151, 199, 112, 211, 116, 113, 172, 189, 150, 228, 122, 78, 191, 177, 161, 119, 63, 74, 187, 121, 199, 165, 0, 146, 203, 253, 189, 85, 144, 24, 162, 117, 130, 200, 223, 110, 92, 116, 98, 240, 209, 246, 12, 19, 236, 6, 242, 36, 76, 32, 26, 101, 82, 176, 68, 218, 125, 48, 20, 14, 221, 234, 50, 141, 216, 17, 57, 243, 191, 56, 145, 204, 213, 193, 162, 89, 21, 143, 170, 184, 238, 62, 92, 62, 19, 43, 160, 171, 223, 23, 187, 144, 35, 19, 116, 64, 11, 27, 212, 249, 236, 34, 77, 191, 45, 58, 139, 156, 39, 255, 15, 163, 196, 154, 151, 74, 102, 211, 135, 198, 225, 185, 139, 98, 149, 71, 200, 109, 47, 161, 181, 161, 215, 84, 82, 173, 45, 160, 55, 116, 159, 30, 180, 46, 45, 84, 25, 120, 245, 51, 197, 195, 11, 19, 184, 239, 242, 155, 31, 94, 131, 55, 68, 43, 82, 55, 27, 86, 9, 255, 0, 0, 0
#endif
};
uint8_t gv_data_seg[SEG_SIZE] = {};

//
uintptr_t data_seg_addr = 0;
uintptr_t code_seg_addr = 0;

extern int ip;

extern unsigned pointer_size;

// Opcode encrypt by xorshift32
extern uint32_t opcode_xorshift32_state;
extern uint32_t vm_code_state;

uint8_t exception_thrown;
void *exception_ptr;
int exception_selector;
uint64_t last_br_from_bb_id;
uint64_t current_bb_id;

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
/* The state word must be initialized to non-zero */
uint32_t xorshift32(uint32_t *state) {
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	uint32_t x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return *state = x;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
uint8_t get_byte_code() {
	uint8_t tmp = ((uint8_t *)code_seg_addr)[ip++];
	tmp ^= (xorshift32(&vm_code_state) & 0xFF);
	return tmp;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
// unpack data from code_seg directly(without xorshift32)
uint32_t get_xorshift_seed() {
	uint32_t res = 0;

	for (int i = 0; i < 4; i++) {
		res |= (uint32_t)((uint8_t *)code_seg_addr)[ip++] << (8 * i);
	}

	return res;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
// unpack data from code_seg
uint64_t unpack_code(int size) {
	uint64_t res = 0;

	for (int i = 0; i < size; i++) {
		res |= (uint64_t)get_byte_code() << (8 * i);
	}

	return res;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
// unpack data from (uint8_t*)data_seg_addr
uint64_t unpack_data(uint64_t offset, int size) {
	uint64_t res = 0;

	for (int i = 0; i < size; i++) {
		// must add (uint64_t), or overflow int32
		res |= (uint64_t)((uint8_t *)(uint8_t*)data_seg_addr)[offset++] << (8 * i);
	}

	return res;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
uint64_t unpack_addr(uint64_t address, int size) {
	uint8_t * ptr = (uint8_t *) address;

	uint64_t res = 0;

	for (int i = 0; i < size; i++) {
		res |= (uint64_t) * ptr << (8 * i);
		ptr ++;
	}

	return res;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void pack_store_addr(uint64_t address, uint64_t value, int size) {
	uint8_t * ptr = (uint8_t *) address;

	for (int i = 0; i < size; i++) {
		*ptr = value & 0xFF;
		ptr ++;
		value = value >> 8;
	}
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
// get a var or const directly
uint64_t get_value_with_size(uint8_t value_size, uint8_t value_type) {

	uint64_t res = 0;
	if (value_type == 0) {
		// is a var

		// get var_offset of (uint8_t*)data_seg_addr
		uint64_t var_offset = unpack_code(pointer_size);

		// fetch data from (uint8_t*)data_seg_addr
		res = unpack_data(var_offset, value_size);
	} else {
		// const

		// unpack const from code
		res = unpack_code(value_size);
	}

	return res;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
// get a var or const directly, also returns size
uint64_t get_value_ex(uint8_t *out_size) {

	uint8_t value_size = get_byte_code();
	uint8_t value_type = get_byte_code();

	if (out_size) *out_size = value_size;

	uint64_t res = 0;
	if (value_type == 0) {
		// is a var

		// get var_offset of (uint8_t*)data_seg_addr
		uint64_t var_offset = unpack_code(pointer_size);

		// fetch data from (uint8_t*)data_seg_addr
		res = unpack_data(var_offset, value_size);
	} else {
		// const

		// unpack const from code
		res = unpack_code(value_size);
	}

	return res;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
uint64_t get_value() {

	uint8_t value_size = get_byte_code();
	uint8_t value_type = get_byte_code();

	uint64_t res = 0;
	if (value_type == 0) {
		// is a var

		// get var_offset of (uint8_t*)data_seg_addr
		uint64_t var_offset = unpack_code(pointer_size);

		// fetch data from (uint8_t*)data_seg_addr
		res = unpack_data(var_offset, value_size);
	} else {
		// const

		// unpack const from code
		res = unpack_code(value_size);
	}

	return res;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void alloca_handler() {
	// size and type of pointer is useless
	uint8_t var_size = get_byte_code();
	uint8_t var_type = get_byte_code();

	// get pointer var offset
	uint64_t var_offset = unpack_code(pointer_size);

	// get alloca area offset
	uint64_t area_offset = unpack_code(pointer_size);

	// store area virtual address to var
	// set_var(var_offset, pointer_size, (uint8_t*)data_seg_addr+area_offset);
	pack_store_addr(data_seg_addr + var_offset, data_seg_addr + area_offset, var_size);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void load_handler() {
	uint8_t var_size = get_byte_code();
	uint8_t var_type = get_byte_code();
	uint64_t var_offset = unpack_code(pointer_size);


	uint8_t ptr_size = get_byte_code();
	uint8_t ptr_type = get_byte_code();
	uint64_t ptr_offset = unpack_code(pointer_size);

	// load virtual address
	uint64_t ptr = unpack_data(ptr_offset, pointer_size);

	// load value from address
	uint64_t load_value = unpack_addr(ptr, var_size);

	// printf("load  ptr: %lx, load_value: %lx, var_size: %lx\n", ptr, load_value, var_size);
	// store value to var
	pack_store_addr(data_seg_addr + var_offset, load_value, var_size);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void store_handler() {
	uint8_t var_size = get_byte_code();
	uint8_t var_type = get_byte_code();

	if (var_size > 8 && var_type == 0) {
		// 大尺寸值（如16字节数组）：直接从源地址复制内存到目标地址
		uint64_t src_offset = unpack_code(pointer_size);

		// 读取指针信息
		uint8_t ptr_size = get_byte_code();
		uint8_t ptr_type = get_byte_code();
		uint64_t ptr_offset = unpack_code(pointer_size);
		uint64_t ptr = unpack_data(ptr_offset, pointer_size);

		uint64_t src_addr = data_seg_addr + src_offset;

#ifdef GOVM_CPP_DEBUG
		printf("[store_handler] Large value: size=%d, src_offset=%lu, ptr=%lx\n", var_size, src_offset, ptr);
		fflush(NULL);
#endif

		// 直接内存复制
		memcpy((void*)ptr, (void*)src_addr, var_size);
	} else {
		// 小尺寸值（<=8字节）或常量：使用原有逻辑
		uint64_t store_value = get_value_with_size(var_size, var_type);

		uint8_t ptr_size = get_byte_code();
		uint8_t ptr_type = get_byte_code();
		uint64_t ptr_offset = unpack_code(pointer_size);

		uint64_t ptr = unpack_data(ptr_offset, pointer_size);

		// printf("store ptr: %lx, store_value: %lx, var_size: %lx\n", ptr, store_value, var_size);
		pack_store_addr(ptr, store_value, var_size);
	}
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void binaryOperator_handler() {
	uint8_t op_code = get_byte_code();

	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	uint64_t res_value = 0;

	if (op_code == BINOP_FNEG) {
		uint8_t op1_size;
		uint64_t op1_value = get_value_ex(&op1_size);

		if (op1_size <= 4) {
			uint32_t op1_32 = (uint32_t)op1_value;
			float f1 = *(float*)&op1_32;
			float fr = -f1;
			res_value = (uint64_t) * (uint32_t*)&fr;
		} else {
			double d1 = *(double*)&op1_value;
			double dr = -d1;
			res_value = *(uint64_t*)&dr;
		}
	} else {
		uint8_t op1_size, op2_size;
		uint64_t op1_value = get_value_ex(&op1_size);
		uint64_t op2_value = get_value_ex(&op2_size);

		switch (op_code) {
			case BINOP_ADD:
				res_value = op1_value + op2_value;
				break;
			case BINOP_FADD: {
				if (op1_size <= 4) {
					uint32_t op1_32 = (uint32_t)op1_value;
					uint32_t op2_32 = (uint32_t)op2_value;
					float f1 = *(float*)&op1_32;
					float f2 = *(float*)&op2_32;
					float fr = f1 + f2;
					res_value = (uint64_t) * (uint32_t*)&fr;
				} else {
					double d1 = *(double*)&op1_value;
					double d2 = *(double*)&op2_value;
					double dr = d1 + d2;
					res_value = *(uint64_t*)&dr;
				}
				break;
			}
			case BINOP_SUB:
				res_value = op1_value - op2_value;
				break;
			case BINOP_FSUB: {
				if (op1_size <= 4) {
					uint32_t op1_32 = (uint32_t)op1_value;
					uint32_t op2_32 = (uint32_t)op2_value;
					float f1 = *(float*)&op1_32;
					float f2 = *(float*)&op2_32;
					float fr = f1 - f2;
					res_value = (uint64_t) * (uint32_t*)&fr;
				} else {
					double d1 = *(double*)&op1_value;
					double d2 = *(double*)&op2_value;
					double dr = d1 - d2;
					res_value = *(uint64_t*)&dr;
				}
				break;
			}
			case BINOP_MUL:
				res_value = op1_value * op2_value;
				break;
			case BINOP_FMUL: {
				if (op1_size <= 4) {
					uint32_t op1_32 = (uint32_t)op1_value;
					uint32_t op2_32 = (uint32_t)op2_value;
					float f1 = *(float*)&op1_32;
					float f2 = *(float*)&op2_32;
					float fr = f1 * f2;
					res_value = (uint64_t) * (uint32_t*)&fr;
				} else {
					double d1 = *(double*)&op1_value;
					double d2 = *(double*)&op2_value;
					double dr = d1 * d2;
					res_value = *(uint64_t*)&dr;
				}
				break;
			}
			case BINOP_UDIV:
				res_value = op1_value / op2_value;
				break;
			case BINOP_SDIV:
				if (op1_size <= 4) {
					int32_t s1 = (int32_t)op1_value;
					int32_t s2 = (int32_t)op2_value;
					res_value = (uint64_t)(s1 / s2);
				} else {
					int64_t s1 = (int64_t)op1_value;
					int64_t s2 = (int64_t)op2_value;
					res_value = (uint64_t)(s1 / s2);
				}
				break;
			case BINOP_FDIV: {
				if (op1_size <= 4) {
					uint32_t op1_32 = (uint32_t)op1_value;
					uint32_t op2_32 = (uint32_t)op2_value;
					float f1 = *(float*)&op1_32;
					float f2 = *(float*)&op2_32;
					float fr = f1 / f2;
					res_value = (uint64_t) * (uint32_t*)&fr;
				} else {
					double d1 = *(double*)&op1_value;
					double d2 = *(double*)&op2_value;
					double dr = d1 / d2;
					res_value = *(uint64_t*)&dr;
				}
				break;
			}
			case BINOP_UREM:
				res_value = op1_value % op2_value;
				break;
			case BINOP_SREM:
				if (op1_size <= 4) {
					int32_t s1 = (int32_t)op1_value;
					int32_t s2 = (int32_t)op2_value;
					res_value = (uint64_t)(s1 % s2);
				} else {
					int64_t s1 = (int64_t)op1_value;
					int64_t s2 = (int64_t)op2_value;
					res_value = (uint64_t)(s1 % s2);
				}
				break;
			case BINOP_FREM: {
				if (op1_size <= 4) {
					uint32_t op1_32 = (uint32_t)op1_value;
					uint32_t op2_32 = (uint32_t)op2_value;
					float f1 = *(float*)&op1_32;
					float f2 = *(float*)&op2_32;
					float fr = (float)(f1 - f2 * (int64_t)(f1 / f2));
					res_value = (uint64_t) * (uint32_t*)&fr;
				} else {
					double d1 = *(double*)&op1_value;
					double d2 = *(double*)&op2_value;
					double dr = d1 - d2 * (int64_t)(d1 / d2);
					res_value = *(uint64_t*)&dr;
				}
				break;
			}
			case BINOP_SHL:
				res_value = op1_value << op2_value;
				break;
			case BINOP_LSHR:
				res_value = op1_value >> op2_value;
				break;
			case BINOP_ASHR:
				if (op1_size <= 4) {
					int32_t s1 = (int32_t)op1_value;
					res_value = (uint64_t)(s1 >> op2_value);
				} else {
					int64_t s1 = (int64_t)op1_value;
					res_value = (uint64_t)(s1 >> op2_value);
				}
				break;
			case BINOP_AND:
				res_value = op1_value & op2_value;
				break;
			case BINOP_OR:
				res_value = op1_value | op2_value;
				break;
			case BINOP_XOR:
				res_value = op1_value ^ op2_value;
				break;
			default:
				break;
		}
	}

	pack_store_addr(data_seg_addr + res_offset, res_value, res_size);

}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void gep_handler() {
	// get gep size and type
	uint8_t gep_size = get_byte_code();
	uint8_t gep_type = get_byte_code();

	// get return value
	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	uint64_t ptr_value = get_value();

	uint64_t idx_value = get_value();

	uint64_t res_value = 0;

	if (gep_size != 0 && gep_type != 0) {
		// array type
		res_value = ptr_value + gep_size * idx_value;
	} else {
		// struct type - idx_value是成员偏移量（常量）
		res_value = ptr_value + idx_value;
	}

	pack_store_addr(data_seg_addr + res_offset, res_value, res_size);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void cmp_handler() {
	// get Predicate
	uint8_t predicate = get_byte_code();

	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	// get operands with size
	uint8_t op1_size, op2_size;
	uint64_t op1_value = get_value_ex(&op1_size);
	uint64_t op2_value = get_value_ex(&op2_size);

	uint64_t res_value = 0;

	DEBUG(DEBUG_ID_CMP, predicate);

	if (predicate >= FCMP_FALSE && predicate <= FCMP_TRUE) {
		// FCMP predicates - use operand size, not result size
		if (op1_size <= 4) {
			uint32_t op1_32 = (uint32_t)op1_value;
			uint32_t op2_32 = (uint32_t)op2_value;
			float a = *(float*)&op1_32;
			float b = *(float*)&op2_32;
			switch (predicate) {
				case FCMP_FALSE:
					res_value = 0;
					break;
				case FCMP_OEQ:
					res_value = (a == b);
					break;
				case FCMP_OGT:
					res_value = (a > b);
					break;
				case FCMP_OGE:
					res_value = (a >= b);
					break;
				case FCMP_OLT:
					res_value = (a < b);
					break;
				case FCMP_OLE:
					res_value = (a <= b);
					break;
				case FCMP_ONE:
					res_value = (a != b);
					break;
				case FCMP_ORD:
					res_value = !((a != a) || (b != b));
					break;
				case FCMP_UNO:
					res_value = ((a != a) || (b != b));
					break;
				case FCMP_UEQ:
					res_value = ((a == b) || (a != a) || (b != b));
					break;
				case FCMP_UGT:
					res_value = ((a > b) || (a != a) || (b != b));
					break;
				case FCMP_UGE:
					res_value = ((a >= b) || (a != a) || (b != b));
					break;
				case FCMP_ULT:
					res_value = ((a < b) || (a != a) || (b != b));
					break;
				case FCMP_ULE:
					res_value = ((a <= b) || (a != a) || (b != b));
					break;
				case FCMP_UNE:
					res_value = ((a != b) || (a != a) || (b != b));
					break;
				case FCMP_TRUE:
					res_value = 1;
					break;
				default:
					break;
			}
		} else {
			double a = *(double*)&op1_value;
			double b = *(double*)&op2_value;
			switch (predicate) {
				case FCMP_FALSE:
					res_value = 0;
					break;
				case FCMP_OEQ:
					res_value = (a == b);
					break;
				case FCMP_OGT:
					res_value = (a > b);
					break;
				case FCMP_OGE:
					res_value = (a >= b);
					break;
				case FCMP_OLT:
					res_value = (a < b);
					break;
				case FCMP_OLE:
					res_value = (a <= b);
					break;
				case FCMP_ONE:
					res_value = (a != b);
					break;
				case FCMP_ORD:
					res_value = !((a != a) || (b != b));
					break;
				case FCMP_UNO:
					res_value = ((a != a) || (b != b));
					break;
				case FCMP_UEQ:
					res_value = ((a == b) || (a != a) || (b != b));
					break;
				case FCMP_UGT:
					res_value = ((a > b) || (a != a) || (b != b));
					break;
				case FCMP_UGE:
					res_value = ((a >= b) || (a != a) || (b != b));
					break;
				case FCMP_ULT:
					res_value = ((a < b) || (a != a) || (b != b));
					break;
				case FCMP_ULE:
					res_value = ((a <= b) || (a != a) || (b != b));
					break;
				case FCMP_UNE:
					res_value = ((a != b) || (a != a) || (b != b));
					break;
				case FCMP_TRUE:
					res_value = 1;
					break;
				default:
					break;
			}
		}
	} else {
		// ICMP predicates
		int64_t s1, s2;
		if (op1_size <= 4) {
			s1 = (int64_t)(int32_t)(uint32_t)op1_value;
			s2 = (int64_t)(int32_t)(uint32_t)op2_value;
		} else {
			s1 = (int64_t)op1_value;
			s2 = (int64_t)op2_value;
		}
		switch (predicate) {
			case ICMP_EQ:
				res_value = op1_value == op2_value;
				break;
			case ICMP_NE:
				res_value = op1_value != op2_value;
				break;
			case ICMP_UGT:
				res_value = op1_value >  op2_value;
				break;
			case ICMP_UGE:
				res_value = op1_value >= op2_value;
				break;
			case ICMP_ULT:
				res_value = op1_value <  op2_value;
				break;
			case ICMP_ULE:
				res_value = op1_value <= op2_value;
				break;
			case ICMP_SGT:
				res_value = s1 >  s2;
				break;
			case ICMP_SGE:
				res_value = s1 >= s2;
				break;
			case ICMP_SLT:
				res_value = s1 <  s2;
				break;
			case ICMP_SLE:
				res_value = s1 <= s2;
				break;
			default:
				break;
		}
	}

	pack_store_addr(data_seg_addr + res_offset, res_value, res_size);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void cast_handler() {
	uint8_t cast_op = get_byte_code();
	uint8_t op_size = get_byte_code();

	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	uint64_t op_value = get_value();

	uint64_t res_value = 0;

	switch (cast_op) {
		case CAST_TRUNC:
			res_value = op_value & (((uint64_t)1 << (res_size * 8)) - 1);
			break;
		case CAST_ZEXT:
			res_value = op_value;
			break;
		case CAST_SEXT: {
			int64_t sval;
			if (op_size == 1) {
				sval = (int64_t)(int8_t)(uint8_t)op_value;
			} else if (op_size == 2) {
				sval = (int64_t)(int16_t)(uint16_t)op_value;
			} else if (op_size == 4) {
				sval = (int64_t)(int32_t)(uint32_t)op_value;
			} else {
				sval = (int64_t)op_value;
			}
			res_value = (uint64_t)sval;
			break;
		}
		case CAST_FPTRUNC: {
			if (op_size == 8) {
				double dval = *(double*)&op_value;
				float fval = (float)dval;
				res_value = (uint64_t) * (uint32_t*)&fval;
			} else {
				res_value = op_value;
			}
			break;
		}
		case CAST_FPEXT: {
			if (op_size == 4) {
				uint32_t op_32 = (uint32_t)op_value;
				float fval = *(float*)&op_32;
				double dval = (double)fval;
				res_value = *(uint64_t*)&dval;
			} else {
				res_value = op_value;
			}
			break;
		}
		case CAST_FPTOUI: {
			if (op_size == 4) {
				uint32_t op_32 = (uint32_t)op_value;
				float fval = *(float*)&op_32;
				res_value = (uint64_t)(uint64_t)fval;
			} else {
				double dval = *(double*)&op_value;
				res_value = (uint64_t)dval;
			}
			break;
		}
		case CAST_FPTOSI: {
			int64_t sval;
			if (op_size == 4) {
				uint32_t op_32 = (uint32_t)op_value;
				float fval = *(float*)&op_32;
				sval = (int64_t)fval;
			} else {
				double dval = *(double*)&op_value;
				sval = (int64_t)dval;
			}
			res_value = (uint64_t)sval;
			break;
		}
		case CAST_UITOFP: {
			if (res_size <= 4) {
				float fval = (float)op_value;
				res_value = (uint64_t) * (uint32_t*)&fval;
			} else {
				double dval = (double)op_value;
				res_value = *(uint64_t*)&dval;
			}
			break;
		}
		case CAST_SITOFP: {
			int64_t sval = (int64_t)op_value;
			if (res_size <= 4) {
				float fval = (float)sval;
				res_value = (uint64_t) * (uint32_t*)&fval;
			} else {
				double dval = (double)sval;
				res_value = *(uint64_t*)&dval;
			}
			break;
		}
		case CAST_PTRTOINT:
		case CAST_INTTOPTR:
		case CAST_BITCAST:
		default:
			res_value = op_value;
			break;
	}

	pack_store_addr(data_seg_addr + res_offset, res_value, res_size);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void br_handler() {
	// get br type
	uint8_t br_type = get_byte_code();

	uint64_t source_bb_offset = unpack_code(pointer_size);
	last_br_from_bb_id = source_bb_offset;

	uint64_t target_addr = 0;

	if (br_type == 0) {
		// uncondition br
		target_addr = unpack_code(pointer_size);
	} else {
		// condition
		uint64_t condition_value = get_value();
		uint64_t true_br = unpack_code(pointer_size);
		uint64_t false_br = unpack_code(pointer_size);

		if (condition_value) {
			target_addr = true_br;
		} else {
			target_addr = false_br;
		}
	}

	// set ip
	ip = target_addr;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void switch_handler() {
	// unpack condition value (size+type+offset/value)
	uint64_t condition_value = get_value();

	// number of cases (4 bytes)
	uint32_t num_cases = (uint32_t)unpack_code(4);

	// case value size (4 bytes)
	uint32_t case_val_size = (uint32_t)unpack_code(4);

	// default target
	uint64_t default_target = unpack_code(pointer_size);

	uint64_t matched_target = default_target;

	for (uint32_t i = 0; i < num_cases; i++) {
		// read case value (raw from code, already encrypted)
		uint64_t case_val = unpack_code(case_val_size);

		// read case target
		uint64_t case_target = unpack_code(pointer_size);

		if (condition_value == case_val) {
			matched_target = case_target;
			// consume remaining cases but don't evaluate
			for (uint32_t j = i + 1; j < num_cases; j++) {
				unpack_code(case_val_size);
				unpack_code(pointer_size);
			}
			break;
		}
	}

	ip = matched_target;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void insertvalue_handler() {
	// 结果值位置
	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	// 聚合操作数的值（地址）
	uint64_t agg_value = get_value();

	// 要插入的值
	uint64_t insert_value = get_value();

	// 偏移量
	uint64_t offset = get_value();

	// 值大小
	uint32_t value_size = (uint32_t)unpack_code(4);

	// 复制聚合值到结果位置
	uint64_t dst_addr = data_seg_addr + res_offset;
	uint64_t src_addr = agg_value;

	// 先复制整个聚合值
	for (uint32_t i = 0; i < res_size; i++) {
		((uint8_t *)dst_addr)[i] = ((uint8_t *)src_addr)[i];
	}

	// 然后在指定偏移处插入新值
	for (uint32_t i = 0; i < value_size && i < 8; i++) {
		((uint8_t *)dst_addr)[offset + i] = (uint8_t)(insert_value >> (i * 8));
	}
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void extractvalue_handler() {
	// 结果值位置
	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	// 聚合操作数的值（地址）
	uint64_t agg_value = get_value();

	// 偏移量
	uint64_t offset = get_value();

	// 结果类型大小
	uint32_t value_size = (uint32_t)unpack_code(4);

	// 从聚合值地址+offset处读取数据，存储到结果位置
	uint64_t src_addr = agg_value + offset;
	uint64_t result_value = 0;

	// 读取value_size字节的数据
	for (uint32_t i = 0; i < value_size && i < 8; i++) {
		result_value |= ((uint64_t)((uint8_t *)src_addr)[i]) << (i * 8);
	}

	// 存储到结果位置
	pack_store_addr(data_seg_addr + res_offset, result_value, res_size);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void phi_handler() {
	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	uint32_t num_incoming = (uint32_t)unpack_code(4);

	uint64_t matched_value = 0;

	for (uint32_t i = 0; i < num_incoming; i++) {
		uint64_t incoming_bb_id = unpack_code(pointer_size);
		uint64_t incoming_value = get_value();

		if (incoming_bb_id == last_br_from_bb_id) {
			matched_value = incoming_value;
		}
	}

	pack_store_addr(data_seg_addr + res_offset, matched_value, res_size);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void select_handler() {
	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	uint64_t condition = get_value();
	uint64_t true_val = get_value();
	uint64_t false_val = get_value();

	uint64_t result = condition ? true_val : false_val;

	pack_store_addr(data_seg_addr + res_offset, result, res_size);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void landingpad_handler() {
	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	// 读取clauses数量
	uint32_t num_clauses = (uint32_t)unpack_code(4);

#ifdef GOVM_CPP_DEBUG
	printf("[LANDINGPAD] res_offset=%lu, num_clauses=%u, exception_thrown=%d\n",
	       res_offset, num_clauses, exception_thrown);
	fflush(NULL);
#endif

	// 使用捕获的异常对象（如果有）
	void* exc_ptr = caught_exception_ptr;
	int exc_selector = caught_exception_selector;

	// 如果没有捕获的异常，使用全局异常变量
	if (exc_ptr == NULL && exception_thrown) {
		exc_ptr = exception_ptr;
		exc_selector = exception_selector;
	}

	// 存储异常对象指针和选择器
	pack_store_addr(data_seg_addr + res_offset, (uint64_t)(uintptr_t)exc_ptr, pointer_size);
	pack_store_addr(data_seg_addr + res_offset + pointer_size, (uint64_t)exc_selector, 4);

	// 解析并存储所有clauses的类型信息（用于后续类型匹配）
	// 在(uint8_t*)data_seg_addr中分配空间存储类型信息表
	uint64_t clauses_offset = res_offset + pointer_size + 4;  // 异常对象 + selector之后
	pack_store_addr(data_seg_addr + clauses_offset, num_clauses, 4);  // 存储clauses数量

	for (uint32_t i = 0; i < num_clauses; i++) {
		uint8_t is_catch = get_byte_code();  // 1=catch, 0=filter
		uint64_t clause_offset = clauses_offset + 4 + i * (1 + pointer_size);  // 每个clause: type(1) + info(pointer_size)

		pack_store_addr(data_seg_addr + clause_offset, is_catch, 1);  // 存储类型

		if (is_catch) {
			// Catch clause: 读取类型信息
			uint64_t type_info = unpack_code(pointer_size);
			pack_store_addr(data_seg_addr + clause_offset + 1, type_info, pointer_size);

#ifdef GOVM_CPP_DEBUG
			printf("[LANDINGPAD]   Clause %u: catch, type_info=0x%lx\n", i, type_info);
			fflush(NULL);
#endif
		} else {
			// Filter clause: 读取类型数组
			uint32_t num_types = (uint32_t)unpack_code(4);
			pack_store_addr(data_seg_addr + clause_offset + 1, num_types, 4);  // 存储类型数量

#ifdef GOVM_CPP_DEBUG
			printf("[LANDINGPAD]   Clause %u: filter, num_types=%u\n", i, num_types);
			fflush(NULL);
#endif

			for (uint32_t j = 0; j < num_types; j++) {
				uint64_t type_info = unpack_code(pointer_size);
				uint64_t type_offset = clause_offset + 1 + 4 + j * pointer_size;
				pack_store_addr(data_seg_addr + type_offset, type_info, pointer_size);

#ifdef GOVM_CPP_DEBUG
				printf("[LANDINGPAD]     Filter type %u: 0x%lx\n", j, type_info);
				fflush(NULL);
#endif
			}
		}
	}

	// 清除异常标志
	exception_thrown = 0;
	caught_exception_ptr = NULL;
	caught_exception_selector = 0;

#ifdef GOVM_CPP_DEBUG
	printf("[LANDINGPAD] Exception caught, ptr=%p, selector=%d\n", exc_ptr, exc_selector);
	fflush(NULL);
#endif
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void resume_handler() {
	// 获取异常对象
	uint8_t exc_size = get_byte_code();
	uint8_t exc_type = get_byte_code();
	uint64_t exc_value = 0;

	if (exc_type == 0) {
		// 变量
		uint64_t exc_offset = unpack_code(pointer_size);
		exc_value = unpack_data(exc_offset, exc_size);
	} else {
		// 常量
		exc_value = unpack_code(exc_size);
	}

	// 重新抛出异常
	exception_thrown = 1;
	exception_ptr = (void*)(uintptr_t)exc_value;

#ifdef GOVM_CPP_DEBUG
	printf("[RESUME] Re-throwing exception, ptr=%p\n", exception_ptr);
	fflush(NULL);
#endif

	// 返回，让调用者处理异常传播
	return;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void indirectbr_handler() {
	uint64_t target_addr = get_value();

	ip = (uint32_t)(target_addr & 0xFFFFFFFF);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void extractelement_handler() {
	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	uint64_t vector_ptr = get_value();
	uint64_t index = get_value();
	uint32_t elem_size = (uint32_t)unpack_code(4);

	uint64_t src = vector_ptr + index * elem_size;
	uint64_t res_value = unpack_addr(src, elem_size);

	pack_store_addr(data_seg_addr + res_offset, res_value, res_size);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void insertelement_handler() {
	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	uint64_t vector_ptr = get_value();
	uint64_t element_val = get_value();
	uint64_t index = get_value();
	uint32_t elem_size = (uint32_t)unpack_code(4);

	uint64_t dst_addr = data_seg_addr + res_offset;

	for (uint32_t i = 0; i < res_size; i++) {
		((uint8_t *)dst_addr)[i] = ((uint8_t *)vector_ptr)[i];
	}

	pack_store_addr(dst_addr + index * elem_size, element_val, elem_size);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void shufflevector_handler() {
	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	uint64_t v1_ptr = get_value();
	uint64_t v2_ptr = get_value();
	uint32_t elem_size = (uint32_t)unpack_code(4);
	uint32_t v1_num_elements = (uint32_t)unpack_code(4);
	uint32_t v2_num_elements = (uint32_t)unpack_code(4);
	uint32_t mask_size = (uint32_t)unpack_code(4);

	uint64_t dst_addr = data_seg_addr + res_offset;

	for (uint32_t i = 0; i < mask_size; i++) {
		int32_t mask_val = (int32_t)unpack_code(4);

		if (mask_val >= 0) {
			uint64_t src;
			if ((uint32_t)mask_val < v1_num_elements) {
				src = v1_ptr + mask_val * elem_size;
			} else {
				src = v2_ptr + (mask_val - v1_num_elements) * elem_size;
			}
			for (uint32_t j = 0; j < elem_size; j++) {
				((uint8_t *)dst_addr)[i * elem_size + j] = ((uint8_t *)src)[j];
			}
		} else {
			for (uint32_t j = 0; j < elem_size; j++) {
				((uint8_t *)dst_addr)[i * elem_size + j] = 0;
			}
		}
	}
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void freeze_handler() {
	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	uint64_t value = get_value();

	pack_store_addr(data_seg_addr + res_offset, value, res_size);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void catchswitch_handler() {
	// 读取handler数量
	uint32_t num_handlers = (uint32_t)unpack_code(4);

	// 读取unwind目标
	uint64_t unwind_target = unpack_code(pointer_size);

#ifdef GOVM_CPP_DEBUG
	printf("[CATCHSWITCH] num_handlers=%u, unwind_target=%lu, exception_thrown=%d\n",
	       num_handlers, unwind_target, exception_thrown);
	fflush(NULL);
#endif

	// 如果没有异常抛出，跳转到unwind目标
	if (!exception_thrown) {
		if (unwind_target != 0) {
			ip = (uint32_t)unwind_target;
		}
		return;
	}

	// 获取异常对象的类型信息
	// 在C++ ABI中，异常对象包含类型信息
	// exception_selector通常包含类型信息的索引或指针
	uint64_t exception_type_info = (uint64_t)exception_selector;

#ifdef GOVM_CPP_DEBUG
	printf("[CATCHSWITCH] Exception type_info=0x%lx, exception_ptr=%p\n",
	       exception_type_info, exception_ptr);
	fflush(NULL);
#endif

	// 遍历每个handler，进行类型匹配
	for (uint32_t i = 0; i < num_handlers; i++) {
		uint64_t handler_target = unpack_code(pointer_size);
		uint64_t catch_type_info = unpack_code(pointer_size);

#ifdef GOVM_CPP_DEBUG
		printf("[CATCHSWITCH] handler[%u]: target=%lu, catch_type_info=0x%lx\n",
		       i, handler_target, catch_type_info);
		fflush(NULL);
#endif

		// 完整的RTTI类型匹配逻辑：
		// 1. catch_type_info == 0 表示 catch-all (...)，匹配所有异常
		// 2. 否则需要比较类型信息

		int type_matches = 0;  // 0 = false, 1 = true

		if (catch_type_info == 0) {
			// catch-all (...) - 匹配所有异常
			type_matches = 1;
#ifdef GOVM_CPP_DEBUG
			printf("[CATCHSWITCH]   catch-all matched!\n");
			fflush(NULL);
#endif
		} else {
			// 完整的RTTI类型匹配
			// 在实际的C++ ABI中，需要：
			// 1. 比较类型信息指针
			// 2. 检查继承关系（基类匹配）
			// 3. 处理指针和引用类型

			// 简化实现：直接比较类型信息地址
			// 在实际实现中，应该调用__cxa_allocate_exception等函数获取完整类型信息
			if (exception_type_info == catch_type_info) {
				type_matches = 1;
#ifdef GOVM_CPP_DEBUG
				printf("[CATCHSWITCH]   Exact type match!\n");
				fflush(NULL);
#endif
			} else {
				// 检查继承关系
				// 在实际实现中，需要遍历类型层次结构
				// 这里简化处理：检查异常对象是否包含类型信息表

				// 假设exception_ptr指向异常对象，其第一个字段是类型信息指针
				if (exception_ptr != NULL) {
					uint64_t *exc_obj = (uint64_t *)exception_ptr;
					// 异常对象通常包含指向类型信息的指针
					// 这里简化处理，实际需要根据C++ ABI规范解析

#ifdef GOVM_CPP_DEBUG
					printf("[CATCHSWITCH]   Checking inheritance: exc_obj[0]=0x%lx\n", exc_obj[0]);
					fflush(NULL);
#endif

					// 检查异常对象的类型信息是否匹配
					if (exc_obj[0] == catch_type_info) {
						type_matches = 1;
#ifdef GOVM_CPP_DEBUG
						printf("[CATCHSWITCH]   Inheritance match!\n");
						fflush(NULL);
#endif
					}
				}
			}
		}

		if (type_matches) {
#ifdef GOVM_CPP_DEBUG
			printf("[CATCHSWITCH] Matched! Jumping to handler %lu\n", handler_target);
			fflush(NULL);
#endif

			// 跳转到匹配的handler
			ip = (uint32_t)handler_target;
			return;
		}
	}

	// 没有匹配的handler，跳转到unwind目标
#ifdef GOVM_CPP_DEBUG
	printf("[CATCHSWITCH] No match, jumping to unwind %lu\n", unwind_target);
	fflush(NULL);
#endif

	if (unwind_target != 0) {
		ip = (uint32_t)unwind_target;
	}
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void data_seg_clean(int return_value_off) {
	// clean data seg, from the end of return value
	for (unsigned i = return_value_off; i < SEG_SIZE; i++) {
		((uint8_t *)data_seg_addr)[i] = 0;
	}
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void return_handler() {
	uint8_t var_size = get_byte_code();
	uint8_t var_type = get_byte_code();
	uint64_t ret_value = get_value_with_size(var_size, var_type);

	if (var_size != 0 || var_type != 0) {
		pack_store_addr((uint64_t)(uint8_t*)data_seg_addr, ret_value, var_size);
	}
	// we dont know (uint8_t*)data_seg_addr size, may segmentfault
	// (uint8_t*)data_seg_addr_clean(var_size);
	// for (unsigned i=var_size; i<SEG_SIZE; i++) {
	//     ((uint8_t *)(uint8_t*)data_seg_addr)[i] = 0;
	// }
}

// call_handler is declared extern in the header and replaced at link time


#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
/* Get Opcode, Opcode encrypt by xorshift32*/
uint8_t get_opcode() {
	uint8_t cnt = 0;
	uint8_t his[OP_TOTAL + 1];

	uint8_t curr_byte = ((uint8_t *)code_seg_addr)[ip++];
	curr_byte ^= (xorshift32(&vm_code_state) & 0xFF);

	for (int i = 0; i < OP_TOTAL + 1; i++) {
		uint8_t tmp = xorshift32(&opcode_xorshift32_state);
		if (tmp == curr_byte) {
			return i + 1;
		}

		uint8_t flag = 1;
		for (int j = 0; j < i; j++) {
			if (his[j] == tmp) {
				flag = 0;
			}
		}

		if (flag == 1) {
			his[i] = tmp;
		} else {
			i--;
		}
	}

	return 0xFF;
}


void vm_interpreter() {

	// DEBUG: Print entry
	DEBUG(DEBUG_ID_NEW_BB, 999);

	// init pointer size based on architecture
	pointer_size = sizeof(void*);

	// init
	ip = 0;

	// when step into a new basicblock, we need to fetch opcode_seed and vm_code_seed
	uint8_t is_a_new_bb = 1;

	int instruction_count = 0;

	while (1) {
		instruction_count++;

		DEBUG(DEBUG_ID_IP, instruction_count);

		if (is_a_new_bb) {
			opcode_xorshift32_state = get_xorshift_seed();
			vm_code_state = get_xorshift_seed();
			is_a_new_bb = 0;
			current_bb_id = ip;

			DEBUG(DEBUG_ID_NEW_BB, ip);
		}

		// switch op_code and add ip
		uint8_t opcode = get_opcode();
		DEBUG(DEBUG_ID_OPCODE, opcode);

		switch (opcode) {

			case NOP_OP:
				break;
			case ALLOCA_OP:
				alloca_handler();
				break;
			case LOAD_OP:
				load_handler();
				break;
			case STORE_OP:
				store_handler();
				break;
			case BinaryOperator_OP:
				binaryOperator_handler();
				break;
			case GEP_OP:
				gep_handler();
				break;
			case CMP_OP:
				cmp_handler();
				break;
			case CAST_OP:
				cast_handler();
				break;
			case BR_OP:
				br_handler();
				is_a_new_bb = 1;
				break;
			case SWITCH_OP:
				switch_handler();
				is_a_new_bb = 1;
				break;
			case INSERTVALUE_OP:
				insertvalue_handler();
				break;
			case EXTRACTVALUE_OP:
				extractvalue_handler();
				break;
			case PHI_OP:
				phi_handler();
				break;
			case SELECT_OP:
				select_handler();
				break;
			case LANDINGPAD_OP:
				landingpad_handler();
				break;
			case RESUME_OP:
				resume_handler();
				return;
			case INDIRECTBR_OP:
				indirectbr_handler();
				is_a_new_bb = 1;
				break;
			case EXTRACTELEMENT_OP:
				extractelement_handler();
				break;
			case INSERTELEMENT_OP:
				insertelement_handler();
				break;
			case SHUFFLEVECTOR_OP:
				shufflevector_handler();
				break;
			case FREEZE_OP:
				freeze_handler();
				break;
			case CATCHSWITCH_OP:
				catchswitch_handler();
				is_a_new_bb = 1;
				break;
			case Ret_OP:
#ifdef GOVM_CPP_DEBUG
				printf("[VM] Ret_OP encountered, returning\n");
				fflush(NULL);
#endif
				return_handler();
				return;
				break;
			case Call_OP: {
				uint64_t packed_funcid = unpack_code(pointer_size);
				// packed_res format: {type_size(1), type_id(1), offset(pointer_size)}
				// Total: 2 + pointer_size bytes
				uint8_t type_size = get_byte_code();
				uint8_t type_id = get_byte_code();
				uint64_t offset = unpack_code(pointer_size);
				// Use DEBUG_ID_NEW_BB (1) for funcid, DEBUG_ID_OPCODE (2) for offset
				DEBUG(DEBUG_ID_NEW_BB, packed_funcid);
				DEBUG(DEBUG_ID_OPCODE, offset);
				// 使用异常捕获包装函数
				call_handler_with_exception_handling(packed_funcid);
			}
			break;

			// ==================== 新增操作码处理 ====================

			case CALLBR_OP: {
#ifdef GOVM_CPP_DEBUG
				printf("[VM] CALLBR_OP\n");
				fflush(NULL);
#endif

				// 解析参数
				uint64_t packed_funcid = unpack_code(pointer_size);
				uint8_t type_size = get_byte_code();
				uint8_t type_id = get_byte_code();
				uint64_t offset = unpack_code(pointer_size);

				// 调用函数（与 Call_OP 相同）
				// 使用异常捕获包装函数
				call_handler_with_exception_handling(packed_funcid);

				// CallBr 的分支处理在翻译器中已经生成，这里不需要额外处理
			}
			break;

			case FENCE_OP: {
#ifdef GOVM_CPP_DEBUG
				printf("[VM] FENCE_OP\n");
				fflush(NULL);
#endif

				// 解析内存序和作用域
				uint8_t ordering = get_byte_code();
				uint32_t scope = (uint32_t)unpack_code(4);

				// 执行内存屏障
				// 在实际实现中，这需要映射到目标平台的屏障指令
				// 这里使用编译器屏障
				__sync_synchronize();

#ifdef GOVM_CPP_DEBUG
				printf("[FENCE] ordering=%d scope=%u\n", ordering, scope);
				fflush(NULL);
#endif
			}
			break;

			case ATOMIC_CMPXCHG_OP: {
#ifdef GOVM_CPP_DEBUG
				printf("[VM] ATOMIC_CMPXCHG_OP\n");
				fflush(NULL);
#endif

				// 解析参数
				// packed_res: 结果结构体 {原始值, 比较结果}
				uint8_t res_type_size = get_byte_code();
				uint8_t res_type_id = get_byte_code();
				uint64_t res_offset = unpack_code(pointer_size);

				// packed_ptr: 指针操作数
				uint8_t ptr_type_size = get_byte_code();
				uint8_t ptr_type_id = get_byte_code();
				uint64_t ptr_offset = unpack_code(pointer_size);

				// packed_cmp: 比较值
				uint8_t cmp_type_size = get_byte_code();
				uint8_t cmp_type_id = get_byte_code();
				uint64_t cmp_offset = unpack_code(pointer_size);

				// packed_new: 新值
				uint8_t new_type_size = get_byte_code();
				uint8_t new_type_id = get_byte_code();
				uint64_t new_offset = unpack_code(pointer_size);

				// 内存序
				uint8_t success_ordering = get_byte_code();
				uint8_t failure_ordering = get_byte_code();

				// 值类型大小
				uint32_t val_size = (uint32_t)unpack_code(4);

				// 获取指针 - 修复类型转换
				uint8_t *ptr = (uint8_t*)unpack_addr(data_seg_addr + ptr_offset, pointer_size);

				// 获取比较值和新值
				uint8_t *cmp_val = (uint8_t*)data_seg_addr + cmp_offset;
				uint8_t *new_val = (uint8_t*)data_seg_addr + new_offset;

				// 执行原子比较交换
				bool success = false;
				uint8_t old_val[16]; // 假设最大16字节

				// 根据值大小选择合适的原子操作
				if (val_size == 1) {
					uint8_t expected = *(uint8_t*)cmp_val;
					uint8_t desired = *(uint8_t*)new_val;
					uint8_t old = __sync_val_compare_and_swap((uint8_t*)ptr, expected, desired);
					success = (old == expected);
					*(uint8_t*)old_val = old;
				} else if (val_size == 2) {
					uint16_t expected = *(uint16_t*)cmp_val;
					uint16_t desired = *(uint16_t*)new_val;
					uint16_t old = __sync_val_compare_and_swap((uint16_t*)ptr, expected, desired);
					success = (old == expected);
					*(uint16_t*)old_val = old;
				} else if (val_size == 4) {
					uint32_t expected = *(uint32_t*)cmp_val;
					uint32_t desired = *(uint32_t*)new_val;
					uint32_t old = __sync_val_compare_and_swap((uint32_t*)ptr, expected, desired);
					success = (old == expected);
					*(uint32_t*)old_val = old;
				} else if (val_size == 8) {
					uint64_t expected = *(uint64_t*)cmp_val;
					uint64_t desired = *(uint64_t*)new_val;
					uint64_t old = __sync_val_compare_and_swap((uint64_t*)ptr, expected, desired);
					success = (old == expected);
					*(uint64_t*)old_val = old;
				}

				// 存储结果结构体 {原始值, 比较结果}
				memcpy((uint8_t*)data_seg_addr + res_offset, old_val, val_size);
				*(uint8_t*)(data_seg_addr + res_offset + val_size) = success ? 1 : 0;

#ifdef GOVM_CPP_DEBUG
				printf("[ATOMIC_CMPXCHG] val_size=%u success=%d\n", val_size, success);
				fflush(NULL);
#endif
			}
			break;

			case ATOMIC_RMW_OP: {
#ifdef GOVM_CPP_DEBUG
				printf("[VM] ATOMIC_RMW_OP\n");
				fflush(NULL);
#endif

				// 解析操作类型
				uint8_t op_val = get_byte_code();

				// packed_res: 结果值（原始值）
				uint8_t res_type_size = get_byte_code();
				uint8_t res_type_id = get_byte_code();
				uint64_t res_offset = unpack_code(pointer_size);

				// packed_ptr: 指针操作数
				uint8_t ptr_type_size = get_byte_code();
				uint8_t ptr_type_id = get_byte_code();
				uint64_t ptr_offset = unpack_code(pointer_size);

				// packed_val: 值操作数
				uint8_t val_type_size = get_byte_code();
				uint8_t val_type_id = get_byte_code();
				uint64_t val_offset = unpack_code(pointer_size);

				// 内存序
				uint8_t ordering = get_byte_code();

				// 值类型大小
				uint32_t val_size = (uint32_t)unpack_code(4);

				// 获取指针和值 - 修复类型转换
				uint8_t *ptr = (uint8_t*)unpack_addr(data_seg_addr + ptr_offset, pointer_size);
				uint8_t *val = (uint8_t*)data_seg_addr + val_offset;

				// 执行原子读-修改-写操作
				uint8_t old_val[16];

				// 根据操作类型和值大小执行相应的原子操作
				if (val_size == 4) {
					uint32_t operand = *(uint32_t*)val;
					uint32_t old;

					switch (op_val) {
						case 0: // Xchg
							old = __sync_lock_test_and_set((uint32_t*)ptr, operand);
							break;
						case 1: // Add
							old = __sync_fetch_and_add((uint32_t*)ptr, operand);
							break;
						case 2: // Sub
							old = __sync_fetch_and_sub((uint32_t*)ptr, operand);
							break;
						case 3: // And
							old = __sync_fetch_and_and((uint32_t*)ptr, operand);
							break;
						case 5: // Or
							old = __sync_fetch_and_or((uint32_t*)ptr, operand);
							break;
						case 6: // Xor
							old = __sync_fetch_and_xor((uint32_t*)ptr, operand);
							break;
						default:
							old = __sync_fetch_and_add((uint32_t*)ptr, operand);
							break;
					}
					*(uint32_t*)old_val = old;
				} else if (val_size == 8) {
					uint64_t operand = *(uint64_t*)val;
					uint64_t old;

					switch (op_val) {
						case 0: // Xchg
							old = __sync_lock_test_and_set((uint64_t*)ptr, operand);
							break;
						case 1: // Add
							old = __sync_fetch_and_add((uint64_t*)ptr, operand);
							break;
						case 2: // Sub
							old = __sync_fetch_and_sub((uint64_t*)ptr, operand);
							break;
						case 3: // And
							old = __sync_fetch_and_and((uint64_t*)ptr, operand);
							break;
						case 5: // Or
							old = __sync_fetch_and_or((uint64_t*)ptr, operand);
							break;
						case 6: // Xor
							old = __sync_fetch_and_xor((uint64_t*)ptr, operand);
							break;
						default:
							old = __sync_fetch_and_add((uint64_t*)ptr, operand);
							break;
					}
					*(uint64_t*)old_val = old;
				}

				// 存储结果（原始值）
				memcpy((uint8_t*)data_seg_addr + res_offset, old_val, val_size);

#ifdef GOVM_CPP_DEBUG
				printf("[ATOMIC_RMW] op=%d val_size=%u\n", op_val, val_size);
				fflush(NULL);
#endif
			}
			break;

			case VAARG_OP: {
#ifdef GOVM_CPP_DEBUG
				printf("[VM] VAARG_OP\n");
				fflush(NULL);
#endif

				// 解析参数
				// packed_res: 结果值
				uint8_t res_type_size = get_byte_code();
				uint8_t res_type_id = get_byte_code();
				uint64_t res_offset = unpack_code(pointer_size);

				// packed_va_list: va_list 指针
				uint8_t va_list_type_size = get_byte_code();
				uint8_t va_list_type_id = get_byte_code();
				uint64_t va_list_offset = unpack_code(pointer_size);

				// 参数类型大小
				uint32_t arg_size = (uint32_t)unpack_code(4);

				// 参数类型信息
				uint8_t arg_type_size = get_byte_code();
				uint8_t arg_type_id = get_byte_code();
				// 跳过类型信息的其余部分
				for (int i = 0; i < arg_type_size - 2; i++) {
					get_byte_code();
				}

				// 获取 va_list 指针的地址
				void **va_list_ptr_ptr = (void**)(data_seg_addr + va_list_offset);
				va_list *va = (va_list*)(*va_list_ptr_ptr);

				// 完整实现：使用 va_arg 宏获取参数
				// 由于我们在运行时不知道参数类型，需要根据 arg_size 来处理
				void *result = NULL;

				// 在 AArch64 Android 上，va_list 是一个结构体指针
				// 我们需要正确处理不同的参数类型和大小

				// 根据参数大小调用相应的 va_arg
				if (arg_size == 1) {
					// 对于小于 int 的类型，C 标准规定会提升为 int
					int val = va_arg(*va, int);
					result = &val;
					// 只复制需要的字节数
					memcpy((uint8_t*)data_seg_addr + res_offset, result, arg_size);
				} else if (arg_size == 2) {
					// 对于小于 int 的类型，C 标准规定会提升为 int
					int val = va_arg(*va, int);
					result = &val;
					memcpy((uint8_t*)data_seg_addr + res_offset, result, arg_size);
				} else if (arg_size == 4) {
					int val = va_arg(*va, int);
					result = &val;
					memcpy((uint8_t*)data_seg_addr + res_offset, result, arg_size);
				} else if (arg_size == 8) {
					// 8字节可能是 long long 或 double
					// 根据 type_id 判断
					if (arg_type_id == 4) { // FloatTyID
						double val = va_arg(*va, double);
						result = &val;
					} else {
						long long val = va_arg(*va, long long);
						result = &val;
					}
					memcpy((uint8_t*)data_seg_addr + res_offset, result, arg_size);
				} else if (arg_size == 16) {
					// 16字节可能是 long double 或 __int128
					// 使用 long double 作为通用类型
					long double val = va_arg(*va, long double);
					result = &val;
					memcpy((uint8_t*)data_seg_addr + res_offset, result, arg_size);
				} else {
					// 对于更大的类型（结构体等），通过指针传递
					void *val = va_arg(*va, void*);
					memcpy((uint8_t*)data_seg_addr + res_offset, val, arg_size);
				}

				// 更新 va_list 指针
				*va_list_ptr_ptr = (void*)va;

#ifdef GOVM_CPP_DEBUG
				printf("[VAARG] arg_size=%u type_id=%u\n", arg_size, arg_type_id);
				fflush(NULL);
#endif
			}
			break;

			default:
#ifdef GOVM_CPP_DEBUG
				printf("[VM] Unknown opcode 0x%02x, returning\n", opcode);
				fflush(NULL);
#endif
				return;
				// cannot recognize opcode

		}
	}
}

// Main function removed - VM interpreter should be linked, not executed directly
// int main() {
//     char test[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//     uint32_t len = 10;
//     setbuf(stdout, 0);
//     setbuf(stderr, 0);
//     ((uintptr_t *)gv_(uint8_t*)data_seg_addr)[0] = (uintptr_t) test;
//     ((uint32_t *)gv_(uint8_t*)data_seg_addr)[2] = len;
//     (uint8_t*)data_seg_addr = (uintptr_t) gv_(uint8_t*)data_seg_addr;
//     code_seg_addr = (uintptr_t) gv_code_seg;
//     vm_interpreter();
//     for(int i=0; i < len; i++) {
//         printf("%d, ", test[i]);
//     }
//     printf("\n");
// }
