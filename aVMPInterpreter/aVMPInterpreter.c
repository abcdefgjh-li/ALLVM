// Verbose VM tracing is disabled by default.
// Crash context is dumped independently and does not rely on this macro.
#ifdef GOVM_CPP_DEBUG
#undef GOVM_CPP_DEBUG
#endif

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <unistd.h>
#include "aVMPInterpreter.h"

extern void _Unwind_Resume(void *exc) __attribute__((noreturn));

__attribute__((noreturn)) void vmp_resume_unwind(void *exc) {
	_Unwind_Resume(exc);
	__builtin_unreachable();
}

// C++ RTTI 类型信息结构（简化版，基于 Itanium C++ ABI）
// 参考：https://itanium-cxx-abi.github.io/cxx-abi/abi.html#rtti

// type_info 基类
typedef struct {
	void *vtable;  // 虚表指针
	const char *name;  // 类型名称
} __type_info_t;

// __class_type_info - 普通类（无基类）
typedef struct {
	void *vtable;
	const char *name;
} __class_type_info_t;

// __si_class_type_info - 单继承类
typedef struct {
	void *vtable;
	const char *name;
	const __class_type_info_t *base_type;
} __si_class_type_info_t;

// __base_class_type_info - 基类信息
typedef struct {
	const __class_type_info_t *base_type;
	long offset_flags;  // 低8位是标志，高位是偏移量
} __base_class_type_info_t;

// __vmi_class_type_info - 多重继承/虚拟继承类
typedef struct {
	void *vtable;
	const char *name;
	unsigned int flags;
	unsigned int base_count;
	__base_class_type_info_t base_info[1];  // 可变长度数组
} __vmi_class_type_info_t;

// RTTI 类型标志
#define RTTI_CLASS_TYPE        0
#define RTTI_SI_CLASS_TYPE     1
#define RTTI_VMI_CLASS_TYPE    2

// 基类标志
#define BASE_VIRTUAL_MASK   0x1
#define BASE_PUBLIC_MASK    0x2
#define BASE_OFFSET_SHIFT   8

// 动态转换信息
typedef struct {
	const __class_type_info_t *dst_type;
	const void *static_ptr;
	const __class_type_info_t *static_type;
	const void *dst_ptr_leading_to_static_ptr;
	int path_dst_ptr_to_static_ptr;  // 0=unknown, 1=public, 2=not_public
	int number_to_static_ptr;
	bool search_done;
} __dynamic_cast_info_t;

// 异常对象结构（基于 __cxa_exception）
typedef struct {
	// __cxa_exception 头部
	void *reserve;  // 64位系统的填充
	size_t referenceCount;  // C++11 exception_ptr 支持

	// 异常类型信息
	const void *exceptionType;  // 指向 type_info 的指针
	void (*exceptionDestructor)(void *);

	// 异常处理器
	void *unexpectedHandler;
	void *terminateHandler;

	// 异常链
	void *nextException;
	int handlerCount;

	// 异常处理上下文
	int handlerSwitchValue;
	const unsigned char *actionRecord;
	const unsigned char *languageSpecificData;
	void *catchTemp;
	void *adjustedPtr;

	// Unwind 头部
	void *unwindHeader[6];  // _Unwind_Exception
} __cxa_exception_t;

// 判断两个 type_info 是否相等
static bool is_type_info_equal(const void *type1, const void *type2) {
	if (type1 == type2) return true;
	if (!type1 || !type2) return false;

	// 比较类型名称（简化实现）
	const __type_info_t *t1 = (const __type_info_t *)type1;
	const __type_info_t *t2 = (const __type_info_t *)type2;

	if (t1->name && t2->name) {
		return strcmp(t1->name, t2->name) == 0;
	}

	return false;
}

// 获取 RTTI 类型（通过 vtable 或结构判断）
static int get_rtti_type(const void *type_info) {
	if (!type_info) return RTTI_CLASS_TYPE;

	// 在实际实现中，应该通过 vtable 判断
	// 这里简化处理：尝试判断结构类型
	const __type_info_t *ti = (const __type_info_t *)type_info;

	// 尝试访问 __si_class_type_info 的 base_type 字段
	// 如果地址有效且不是 NULL，可能是单继承
	const __si_class_type_info_t *si_ti = (const __si_class_type_info_t *)type_info;
	if (si_ti->base_type != NULL && si_ti->base_type != type_info) {
		// 可能是单继承，但需要进一步验证
		// 这里简化处理，假设所有有 base_type 的都是单继承
		return RTTI_SI_CLASS_TYPE;
	}

	return RTTI_CLASS_TYPE;
}

// 从异常对象中提取类型信息
static const void* get_exception_type_info(void *exception_ptr) {
	if (!exception_ptr) return NULL;

	// 异常对象布局：__cxa_exception 头部 + 实际异常对象
	// 异常对象的第一个字段通常是指向 type_info 的指针
	__cxa_exception_t *exc_header = (__cxa_exception_t *)exception_ptr - 1;

	// 从 __cxa_exception 中获取类型信息
	if (exc_header->exceptionType) {
		return exc_header->exceptionType;
	}

	// 备用方案：从异常对象本身读取
	void **exc_obj = (void **)exception_ptr;
	if (exc_obj[0]) {
		// 第一个字段可能是指向 type_info 的指针
		return exc_obj[0];
	}

	return NULL;
}

// 完整的类型层次遍历（支持所有继承类型）
static bool traverse_type_hierarchy(
	const __class_type_info_t *type,
	bool (*visitor)(const __class_type_info_t *, void *, int, void *),
	void *visitor_data,
	void *object_ptr,
	int depth,
	void *vbase_cookie
) {
	if (!type || !visitor) return false;

	// 访问当前类型
	if (visitor(type, object_ptr, depth, visitor_data)) {
		return true;  // 访问者返回 true 表示停止遍历
	}

	// 检查基类
	int rtti_type = get_rtti_type(type);

	if (rtti_type == RTTI_SI_CLASS_TYPE) {
		// 单继承：递归遍历基类
		const __si_class_type_info_t *si_type = (const __si_class_type_info_t *)type;
		if (si_type->base_type) {
			return traverse_type_hierarchy(
				si_type->base_type,
				visitor,
				visitor_data,
				object_ptr,  // 单继承，偏移量为 0
				depth + 1,
				vbase_cookie
			);
		}
	} else if (rtti_type == RTTI_VMI_CLASS_TYPE) {
		// 多重继承/虚拟继承：遍历所有基类
		const __vmi_class_type_info_t *vmi_type = (const __vmi_class_type_info_t *)type;

		for (unsigned int i = 0; i < vmi_type->base_count; i++) {
			const __base_class_type_info_t *base_info = &vmi_type->base_info[i];

			void *base_ptr = object_ptr;
			void *new_vbase_cookie = vbase_cookie;

			// 检查是否为虚拟基类
			bool is_virtual = (base_info->offset_flags & BASE_VIRTUAL_MASK) != 0;

			if (is_virtual) {
				// 虚拟基类处理
				const __type_info_t *base_ti = (const __type_info_t *)base_info->base_type;
				if (base_ti && base_ti->name) {
					// 检查是否已经访问过
					if (vbase_cookie != NULL && vbase_cookie == (void *)base_ti->name) {
						continue;  // 跳过已访问的虚拟基类
					}
					new_vbase_cookie = (void *)base_ti->name;
				}

				// 计算虚拟基类偏移量
				long offset = base_info->offset_flags >> BASE_OFFSET_SHIFT;
				if (object_ptr != NULL) {
					base_ptr = (void *)((uintptr_t)object_ptr + offset);
				} else {
					base_ptr = NULL;
				}
			} else {
				// 非虚拟基类
				long offset = base_info->offset_flags >> BASE_OFFSET_SHIFT;
				base_ptr = (void *)((uintptr_t)object_ptr + offset);
			}

			// 递归遍历基类
			if (traverse_type_hierarchy(
				base_info->base_type,
				visitor,
				visitor_data,
				base_ptr,
				depth + 1,
				new_vbase_cookie
			)) {
				return true;  // 找到目标，停止遍历
			}
		}
	}

	return false;
}

// 类型匹配访问者函数
typedef struct {
	const __class_type_info_t *target_type;
	void **result_ptr;
	bool found;
} type_match_visitor_data_t;

static bool type_match_visitor(
	const __class_type_info_t *type,
	void *object_ptr,
	int depth,
	void *data
) {
	type_match_visitor_data_t *visitor_data = (type_match_visitor_data_t *)data;

	if (is_type_info_equal(type, visitor_data->target_type)) {
		if (visitor_data->result_ptr) {
			*visitor_data->result_ptr = object_ptr;
		}
		visitor_data->found = true;
		return true;  // 找到匹配，停止遍历
	}

	return false;  // 继续遍历
}

// 检查类型层次中的基类匹配（递归）
// 支持：单继承、多重继承、虚拟继承
// 使用完整的类型层次遍历
static bool has_unambiguous_public_base(
	const __class_type_info_t *thrown_type,
	const __class_type_info_t *catch_type,
	void *adjusted_ptr,
	void **result_ptr,
	void *vbase_cookie  // 用于跟踪虚拟基类
) {
	if (!thrown_type || !catch_type) return false;

	// 使用访问者模式进行类型匹配
	type_match_visitor_data_t visitor_data = {
		.target_type = catch_type,
		.result_ptr = result_ptr,
		.found = false
	};

	// 遍历类型层次
	traverse_type_hierarchy(
		thrown_type,
		type_match_visitor,
		&visitor_data,
		adjusted_ptr,
		0,
		vbase_cookie
	);

	return visitor_data.found;
}

// 检查异常类型是否匹配 catch 类型
// 支持动态类型信息解析
static bool can_catch_exception(
	const void *thrown_type_info,
	const void *catch_type_info,
	void *exception_ptr,
	void **adjusted_ptr
) {
	// catch-all (...) 匹配所有异常
	if (!catch_type_info) {
		if (adjusted_ptr) *adjusted_ptr = exception_ptr;
		return true;
	}

	// 没有抛出类型信息，尝试从异常对象中提取
	if (!thrown_type_info && exception_ptr) {
		thrown_type_info = get_exception_type_info(exception_ptr);
	}

	// 仍然没有类型信息，无法匹配
	if (!thrown_type_info) {
		return false;
	}

	// 直接类型匹配
	if (is_type_info_equal(thrown_type_info, catch_type_info)) {
		if (adjusted_ptr) *adjusted_ptr = exception_ptr;
		return true;
	}

	// 检查基类匹配（需要遍历类型层次）
	// 支持：单继承、多重继承、虚拟继承
	return has_unambiguous_public_base(
		(const __class_type_info_t *)thrown_type_info,
		(const __class_type_info_t *)catch_type_info,
		exception_ptr,
		adjusted_ptr,
		NULL  // 初始 vbase_cookie 为 NULL
	);
}

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

#define EH_TRACE(fmt, ...) do { \
	if (vmp_debug_enabled) { \
		printf(fmt, ##__VA_ARGS__); \
		fflush(NULL); \
	} \
} while (0)

// C++ 异常捕获包装函数
#ifdef __cplusplus
extern "C" {
#endif

// 包装 call_handler 以捕获 C++ 异常
void call_handler_with_exception_handling(uint64_t targetfunc_id) {
#ifdef __cplusplus
	EH_TRACE("[EH_CALL] enter funcid=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\n",
	         (unsigned long long)targetfunc_id,
	         (unsigned)exception_thrown,
	         exception_ptr,
	         exception_selector,
	         caught_exception_ptr,
	         caught_exception_selector,
	         ip);
	try {
		call_handler(targetfunc_id);
		EH_TRACE("[EH_CALL] return funcid=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\n",
		         (unsigned long long)targetfunc_id,
		         (unsigned)exception_thrown,
		         exception_ptr,
		         exception_selector,
		         caught_exception_ptr,
		         caught_exception_selector,
		         ip);
	} catch (...) {
		// 捕获所有异常
		current_exception_ptr = std::current_exception();
		EH_TRACE("[EH_CALL] catch-all funcid=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\n",
		         (unsigned long long)targetfunc_id,
		         (unsigned)exception_thrown,
		         exception_ptr,
		         exception_selector,
		         caught_exception_ptr,
		         caught_exception_selector,
		         ip);

		try {
			std::rethrow_exception(current_exception_ptr);
		} catch (const std::exception& e) {
			// 标准异常
			caught_exception_ptr = (void*)&e;
			caught_exception_selector = 1;
			exception_thrown = 1;
			EH_TRACE("[EH_CALL] std-exception funcid=%llu what=%s exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\n",
			         (unsigned long long)targetfunc_id,
			         e.what(),
			         (unsigned)exception_thrown,
			         exception_ptr,
			         exception_selector,
			         caught_exception_ptr,
			         caught_exception_selector,
			         ip);

#ifdef GOVM_CPP_DEBUG
			printf("[CALL_HANDLER] Caught std::exception: %s\n", e.what());
			fflush(NULL);
#endif
		} catch (...) {
			// 其他异常
			caught_exception_ptr = (void*)0x1;
			caught_exception_selector = 2;
			exception_thrown = 1;
			EH_TRACE("[EH_CALL] unknown-exception funcid=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\n",
			         (unsigned long long)targetfunc_id,
			         (unsigned)exception_thrown,
			         exception_ptr,
			         exception_selector,
			         caught_exception_ptr,
			         caught_exception_selector,
			         ip);

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
uintptr_t dispatch_code_seg_addr = 0;

extern int ip;

extern unsigned pointer_size;

// Opcode encrypt by xorshift32
extern uint64_t opcode_xorshift32_state;
extern uint64_t vm_code_state;
extern uint64_t vm_function_key;
extern uint64_t vm_block_chain_state;
extern uint64_t expected_bb_token;

uint8_t exception_thrown;
void *exception_ptr;
int exception_selector;
uint64_t last_br_from_bb_id;
uint64_t current_bb_id;

#define VM_CRASH_TRACE_DEPTH 32
#define VM_TRACE_KIND_BB 1
#define VM_TRACE_KIND_OPCODE 2
#define VM_TRACE_KIND_BRANCH 3
#define VM_TRACE_KIND_CALL 4

typedef struct {
	uint8_t kind;
	uint8_t tag0;
	uint8_t tag1;
	uint8_t reserved;
	uint32_t ip_value;
	uint64_t a;
	uint64_t b;
	uint64_t c;
	uint64_t d;
} VMTraceEntry;

VMTraceEntry vm_trace_ring[VM_CRASH_TRACE_DEPTH];
uint32_t vm_trace_next = 0;
uint64_t vm_trace_total = 0;

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void vm_trace_push(uint8_t kind, uint32_t ip_value, uint8_t tag0, uint8_t tag1,
	               uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	VMTraceEntry *entry = &vm_trace_ring[vm_trace_next];
	entry->kind = kind;
	entry->tag0 = tag0;
	entry->tag1 = tag1;
	entry->reserved = 0;
	entry->ip_value = ip_value;
	entry->a = a;
	entry->b = b;
	entry->c = c;
	entry->d = d;
	vm_trace_next = (vm_trace_next + 1) % VM_CRASH_TRACE_DEPTH;
	vm_trace_total++;
}

void vm_dump_fault_context(const char *reason, uint64_t detail0, uint64_t detail1) {
	uint64_t available = vm_trace_total < VM_CRASH_TRACE_DEPTH ? vm_trace_total : VM_CRASH_TRACE_DEPTH;
	uint64_t start = vm_trace_total > VM_CRASH_TRACE_DEPTH ? vm_trace_next : 0;
	uint32_t fault_ip = ip > 0 ? (uint32_t)(ip - 1) : 0;

	printf("\n[VM_CRASH] reason=%s detail0=%llu detail1=%llu ip=%u current_bb=%llu last_br_from=%llu exception=%u selector=%d\n",
	       reason,
	       (unsigned long long)detail0,
	       (unsigned long long)detail1,
	       (unsigned)fault_ip,
	       (unsigned long long)current_bb_id,
	       (unsigned long long)last_br_from_bb_id,
	       (unsigned)exception_thrown,
	       exception_selector);

	printf("[VM_CRASH] opcode_state=0x%08x vm_state=0x%08x chain_state=0x%08llx expected_token=0x%08llx code_bytes=",
	       (unsigned)opcode_xorshift32_state,
	       (unsigned)vm_code_state,
	       (unsigned long long)vm_block_chain_state,
	       (unsigned long long)expected_bb_token);
	{
		uint32_t raw_start = fault_ip > 8 ? fault_ip - 8 : 0;
		for (uint32_t i = 0; i < 24; ++i) {
			uint32_t pos = raw_start + i;
			printf("%02x", ((uint8_t *)code_seg_addr)[pos]);
			if (i + 1 != 24) printf(" ");
		}
	}
	printf("\n");

	for (uint64_t i = 0; i < available; ++i) {
		VMTraceEntry *entry = &vm_trace_ring[(start + i) % VM_CRASH_TRACE_DEPTH];
		switch (entry->kind) {
			case VM_TRACE_KIND_BB:
				printf("[VM_CRASH][TRACE] bb ip=%u opcode_seed=0x%08llx vm_seed=0x%08llx bb_token=0x%08llx chain_seed=0x%08llx\n",
				       entry->ip_value,
				       (unsigned long long)entry->a,
				       (unsigned long long)entry->b,
				       (unsigned long long)entry->c,
				       (unsigned long long)entry->d);
				break;
			case VM_TRACE_KIND_OPCODE:
				printf("[VM_CRASH][TRACE] opcode ip=%u op=%u raw=0x%02llx bb=%llu opcode_state=0x%08llx vm_state=0x%08llx\n",
				       entry->ip_value,
				       (unsigned)entry->tag0,
				       (unsigned long long)entry->a,
				       (unsigned long long)entry->d,
				       (unsigned long long)entry->b,
				       (unsigned long long)entry->c);
				break;
			case VM_TRACE_KIND_BRANCH:
				printf("[VM_CRASH][TRACE] br source=%llu target=%llu type=%u flag=%u aux0=%llu aux1=%llu\n",
				       (unsigned long long)entry->a,
				       (unsigned long long)entry->b,
				       (unsigned)entry->tag0,
				       (unsigned)entry->tag1,
				       (unsigned long long)entry->c,
				       (unsigned long long)entry->d);
				break;
			case VM_TRACE_KIND_CALL:
				printf("[VM_CRASH][TRACE] call stage=%s funcid=%llu saved_ip=%llu res_offset=%llu exc=%u selector=%llu\n",
				       entry->tag0 ? "leave" : "enter",
				       (unsigned long long)entry->a,
				       (unsigned long long)entry->b,
				       (unsigned long long)entry->c,
				       (unsigned)entry->tag1,
				       (unsigned long long)entry->d);
				break;
			default:
				break;
		}
	}
	fflush(NULL);
}

// #region debug-point vm-stdio-entry
static void vm_debug_log_stdio_entry(const char *stage) {
	if (!vmp_debug_enabled) {
		return;
	}
	char buffer[96];
	int len = snprintf(buffer, sizeof(buffer),
	                  "[vm-debug] stage=%s\n", stage);
	if (len > 0) {
		size_t to_write = (size_t)len < sizeof(buffer) ? (size_t)len : sizeof(buffer);
		(void)write(2, buffer, to_write);
	}
}

// #region debug-point vm-stage-log
static void vm_debug_log_ip_stage(const char *stage, uint64_t ip_value) {
	if (!vmp_debug_enabled) {
		return;
	}
	char buffer[128];
	int len = snprintf(buffer, sizeof(buffer),
	                  "[vm-debug] stage=%s ip=%llu\n",
	                  stage, (unsigned long long)ip_value);
	if (len > 0) {
		size_t to_write = (size_t)len < sizeof(buffer) ? (size_t)len : sizeof(buffer);
		(void)write(2, buffer, to_write);
	}
}
// #endregion debug-point vm-stage-log

// #region debug-point vm-value-log
static void vm_debug_log_u32_stage(const char *stage, uint32_t a, uint32_t b) {
	if (!vmp_debug_enabled) {
		return;
	}
	char buffer[160];
	int len = snprintf(buffer, sizeof(buffer),
	                  "[vm-debug] stage=%s a=%u b=%u\n",
	                  stage, a, b);
	if (len > 0) {
		size_t to_write = (size_t)len < sizeof(buffer) ? (size_t)len : sizeof(buffer);
		(void)write(2, buffer, to_write);
	}
}
// #endregion debug-point vm-value-log
// #endregion debug-point vm-stdio-entry

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
uint64_t xorshift32(uint64_t *state) {
	*state += 0x9e3779b97f4a7c15ULL;
	uint64_t z = *state;
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
	z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
	z ^= z >> 31;
	return z;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint32_t rotl32(uint32_t value, unsigned shift) {
	return (value << shift) | (value >> (32U - shift));
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint32_t vm_nonzero32(uint32_t value, uint32_t fallback) {
	return value ? value : fallback;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint32_t vm_mix32(uint32_t value) {
	value ^= value >> 16;
	value *= 0x7feb352dU;
	value ^= value >> 15;
	value *= 0x846ca68bU;
	value ^= value >> 16;
	return value;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint64_t rotl64(uint64_t value, unsigned shift) {
	return (value << shift) | (value >> (64U - shift));
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint64_t vm_nonzero64(uint64_t value, uint64_t fallback) {
	return value ? value : fallback;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint64_t vm_mix64(uint64_t value) {
	value ^= value >> 30;
	value *= 0xbf58476d1ce4e5b9ULL;
	value ^= value >> 27;
	value *= 0x94d049bb133111ebULL;
	value ^= value >> 31;
	return value;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint64_t derive_bb_token(uint64_t function_key, uint32_t bb_offset) {
	uint64_t mixed = function_key ^ ((uint64_t)bb_offset * 0x45d9f3b45d9f3bULL) ^
	                 0x9e3779b97f4a7c15ULL;
	return vm_nonzero64(vm_mix64(mixed), 0x13579bdf2468ace1ULL);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint32_t derive_bb_tag(uint64_t function_key, uint32_t bb_offset) {
	uint64_t token = derive_bb_token(function_key, bb_offset);
	uint64_t mixed = token ^ rotl64(function_key ^ 0x85ebca6b27d4eb2dULL, 11) ^
	                 ((uint64_t)bb_offset * 0x27d4eb2f165667c5ULL) ^
	                 0xc2b2ae3d27d4eb4fULL;
	return vm_nonzero32((uint32_t)vm_mix64(mixed), 0x2468ace1u);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint64_t derive_opcode_seed(uint64_t bb_token) {
	return vm_nonzero64(vm_mix64(bb_token ^ 0xa5a5f00d1badb002ULL),
	                    0xa5a5f00d1badb002ULL);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint64_t derive_vm_seed(uint64_t function_key, uint64_t bb_token) {
	uint64_t mixed = bb_token ^ rotl64(function_key, 7) ^ 0x3c6ef372fe94f82bULL;
	return vm_nonzero64(vm_mix64(mixed), 0x3c6ef372fe94f82bULL);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint64_t derive_chain_seed(uint64_t function_key, uint64_t bb_token,
	                              uint32_t bb_offset) {
	uint64_t mixed = rotl64(bb_token, 13) ^ function_key ^
	                 ((uint64_t)bb_offset * 0x165667b19e3779f9ULL) ^
	                 0xd1b54a32d192ed03ULL;
	return vm_nonzero64(vm_mix64(mixed), 0xd1b54a32d192ed03ULL);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint32_t load32_le(const uint8_t *src) {
	return ((uint32_t)src[0]) |
	       ((uint32_t)src[1] << 8) |
	       ((uint32_t)src[2] << 16) |
	       ((uint32_t)src[3] << 24);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static void store32_le(uint8_t *dst, uint32_t value) {
	dst[0] = (uint8_t)(value & 0xFF);
	dst[1] = (uint8_t)((value >> 8) & 0xFF);
	dst[2] = (uint8_t)((value >> 16) & 0xFF);
	dst[3] = (uint8_t)((value >> 24) & 0xFF);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint32_t chacha_rotl32(uint32_t value, unsigned shift) {
	return (value << shift) | (value >> (32U - shift));
}

#define CHACHA_QR(a, b, c, d) \
	a += b; d ^= a; d = chacha_rotl32(d, 16); \
	c += d; b ^= c; b = chacha_rotl32(b, 12); \
	a += b; d ^= a; d = chacha_rotl32(d, 8); \
	c += d; b ^= c; b = chacha_rotl32(b, 7)

static void chacha20_block(const uint32_t key_words[8], uint32_t counter,
	                      const uint32_t nonce_words[3], uint8_t out[64]) {
	static const uint32_t constants[4] = {
		0x61707865U, 0x3320646eU, 0x79622d32U, 0x6b206574U
	};
	uint32_t state[16];
	uint32_t working[16];
	state[0] = constants[0];
	state[1] = constants[1];
	state[2] = constants[2];
	state[3] = constants[3];
	for (int i = 0; i < 8; ++i) {
		state[4 + i] = key_words[i];
	}
	state[12] = counter;
	state[13] = nonce_words[0];
	state[14] = nonce_words[1];
	state[15] = nonce_words[2];
	for (int i = 0; i < 16; ++i) {
		working[i] = state[i];
	}
	for (int round = 0; round < 10; ++round) {
		CHACHA_QR(working[0], working[4], working[8], working[12]);
		CHACHA_QR(working[1], working[5], working[9], working[13]);
		CHACHA_QR(working[2], working[6], working[10], working[14]);
		CHACHA_QR(working[3], working[7], working[11], working[15]);
		CHACHA_QR(working[0], working[5], working[10], working[15]);
		CHACHA_QR(working[1], working[6], working[11], working[12]);
		CHACHA_QR(working[2], working[7], working[8], working[13]);
		CHACHA_QR(working[3], working[4], working[9], working[14]);
	}
	for (int i = 0; i < 16; ++i) {
		working[i] += state[i];
		store32_le(out + i * 4, working[i]);
	}
}

#undef CHACHA_QR

static void derive_chacha_material(uint64_t function_key, uint64_t payload_seed,
	                              uint64_t chain_seed, uint32_t bb_offset,
	                              uint32_t key_words[8], uint32_t nonce_words[3]) {
	uint64_t state = function_key ^ rotl64(payload_seed, 17) ^
	                 rotl64(chain_seed, 29) ^
	                 ((uint64_t)bb_offset * 0x9e3779b97f4a7c15ULL) ^
	                 0x6a09e667f3bcc909ULL;
	for (int i = 0; i < 4; ++i) {
		uint64_t word = xorshift32(&state);
		key_words[i * 2] = (uint32_t)(word & 0xFFFFFFFFU);
		key_words[i * 2 + 1] = (uint32_t)(word >> 32);
	}
	for (int i = 0; i < 3; ++i) {
		nonce_words[i] = (uint32_t)xorshift32(&state);
	}
}

static uint8_t chacha20_byte_at(uint64_t function_key, uint64_t payload_seed,
	                           uint64_t chain_seed, uint32_t bb_offset,
	                           uint32_t payload_index) {
	uint32_t key_words[8];
	uint32_t nonce_words[3];
	uint8_t block[64];
	uint32_t block_index = payload_index / 64U;
	uint32_t block_offset = payload_index % 64U;
	derive_chacha_material(function_key, payload_seed, chain_seed, bb_offset,
	                      key_words, nonce_words);
	chacha20_block(key_words, block_index, nonce_words, block);
	return block[block_offset];
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static uint32_t read_code_u32_raw(void) {
	uint32_t res = 0;
	for (int i = 0; i < 4; i++) {
		res |= (uint32_t)((uint8_t *)code_seg_addr)[ip++] << (8 * i);
	}
	return res;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
static void sync_expected_bb_token(uint64_t target_addr) {
	if (target_addr == 0) {
		expected_bb_token = 0;
		return;
	}
	expected_bb_token = derive_bb_token(vm_function_key, (uint32_t)target_addr);
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
uint8_t get_byte_code() {
	uint32_t raw_index = (uint32_t)ip;
	uint32_t payload_index = raw_index - (uint32_t)current_bb_id - VM_BB_HEADER_SIZE;
	if (payload_index == 0) {
		vm_debug_log_u32_stage("get-byte-enter", raw_index, payload_index);
	}
	uint8_t tmp = ((uint8_t *)code_seg_addr)[ip++];
	{
		uint32_t key_words[8];
		uint32_t nonce_words[3];
		uint8_t block[64];
		uint32_t block_index = payload_index / 64U;
		uint32_t block_offset = payload_index % 64U;
		derive_chacha_material(vm_function_key, vm_code_state, vm_block_chain_state,
		                      (uint32_t)current_bb_id, key_words, nonce_words);
		chacha20_block(key_words, block_index, nonce_words, block);
		tmp ^= block[block_offset];
	}
	if (payload_index == 0) {
		vm_debug_log_u32_stage("get-byte-after-chacha", raw_index, tmp);
	}
	return tmp;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
// unpack data from code_seg directly(without xorshift32)
uint32_t get_xorshift_seed() {
	return read_code_u32_raw();
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
// unpack data from code_seg
uint64_t unpack_code(int size) {
	uint64_t res = 0;

	for (int i = 0; i < size; i++) {
		uint64_t byte_value = (uint64_t)get_byte_code();
		if (i < 8) {
			res |= byte_value << (8 * i);
		}
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
		uint64_t byte_value = (uint64_t)((uint8_t *)(uint8_t*)data_seg_addr)[offset++];
		if (i < 8) {
			res |= byte_value << (8 * i);
		}
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
		uint64_t byte_value = (uint64_t)*ptr;
		if (i < 8) {
			res |= byte_value << (8 * i);
		}
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
static uint64_t get_aggregate_addr(uint8_t *scratch, uint32_t scratch_cap,
	                               uint8_t *out_size) {
	uint8_t value_size = get_byte_code();
	uint8_t value_type = get_byte_code();

	if (out_size) *out_size = value_size;

	if (value_type == 0) {
		uint64_t var_offset = unpack_code(pointer_size);
		return data_seg_addr + var_offset;
	}

	uint32_t copy_size = value_size < scratch_cap ? value_size : scratch_cap;
	for (uint32_t i = 0; i < copy_size; ++i) {
		scratch[i] = get_byte_code();
	}
	for (uint32_t i = copy_size; i < value_size; ++i) {
		(void)get_byte_code();
	}
	return (uint64_t)(uintptr_t)scratch;
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
void read_inline_const_bytes(uint8_t *dst, uint32_t size) {
	for (uint32_t i = 0; i < size; ++i) {
		dst[i] = get_byte_code();
	}
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

	if (var_size > 8) {
		uint8_t *src_addr = 0;
		uint8_t *tmp_inline = 0;

		if (var_type == 0) {
			// 大尺寸变量值：直接从 data_seg 中复制到目标地址
			uint64_t src_offset = unpack_code(pointer_size);
			src_addr = (uint8_t *)data_seg_addr + src_offset;
		} else {
			// 大尺寸常量值：必须按 VM 字节流解密读取，否则会破坏后续 ip/vm_code_state
			tmp_inline = (uint8_t *)malloc(var_size);
			if (tmp_inline) {
				read_inline_const_bytes(tmp_inline, var_size);
				src_addr = tmp_inline;
			} else {
				for (uint32_t i = 0; i < var_size; ++i) {
					(void)get_byte_code();
				}
			}
		}

		// 读取指针信息
		uint8_t ptr_size = get_byte_code();
		uint8_t ptr_type = get_byte_code();
		uint64_t ptr_offset = unpack_code(pointer_size);
		uint64_t ptr = unpack_data(ptr_offset, pointer_size);

		if (src_addr) {
			memcpy((void*)ptr, (void*)src_addr, var_size);
		} else {
			memset((void*)ptr, 0, var_size);
		}

		if (tmp_inline) {
			free(tmp_inline);
		}
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

		// DEBUG: 打印加法操作的操作数
#ifdef GOVM_CPP_DEBUG
		if (op_code == BINOP_ADD) {
			printf("[BINOP_DEBUG] ADD: op1=%lld, op2=%lld, res_offset=%llu\n", (long long)op1_value, (long long)op2_value, (unsigned long long)res_offset);
		}
#endif

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
#ifdef GOVM_CPP_DEBUG
				// DEBUG: 打印乘法操作
				printf("[BINOP_DEBUG] MUL: op1=%lld, op2=%lld, res_offset=%llu\n", (long long)op1_value, (long long)op2_value, (unsigned long long)res_offset);
#endif
				res_value = op1_value * op2_value;
#ifdef GOVM_CPP_DEBUG
				printf("[BINOP_DEBUG] MUL result: %lld\n", (long long)res_value);
#endif
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
	uint64_t trace_aux0 = 0;
	uint64_t trace_aux1 = 0;
	uint8_t trace_flag = 0;

	if (br_type == 0) {
		// uncondition br
		target_addr = unpack_code(pointer_size);
		trace_aux0 = target_addr;
	} else if (br_type == 2) {
		// invoke branch: dispatch on exception_thrown directly
		uint64_t true_br = unpack_code(pointer_size);
		uint64_t false_br = unpack_code(pointer_size);
		target_addr = exception_thrown ? true_br : false_br;
		trace_flag = (uint8_t)(exception_thrown ? 1 : 0);
		trace_aux0 = true_br;
		trace_aux1 = false_br;
#ifdef GOVM_CPP_DEBUG
		printf("[BR_HANDLER] invoke source=%llu exception_thrown=%u true=%llu false=%llu target=%llu\n",
		       (unsigned long long)source_bb_offset,
		       (unsigned)exception_thrown,
		       (unsigned long long)true_br,
		       (unsigned long long)false_br,
		       (unsigned long long)target_addr);
		fflush(NULL);
#endif
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
		trace_flag = (uint8_t)(condition_value ? 1 : 0);
		trace_aux0 = true_br;
		trace_aux1 = false_br;
#ifdef GOVM_CPP_DEBUG
		printf("[BR_HANDLER] cond source=%llu cond=%llu true=%llu false=%llu target=%llu\n",
		       (unsigned long long)source_bb_offset,
		       (unsigned long long)condition_value,
		       (unsigned long long)true_br,
		       (unsigned long long)false_br,
		       (unsigned long long)target_addr);
		fflush(NULL);
#endif
	}

	// set ip
	vm_trace_push(VM_TRACE_KIND_BRANCH, (uint32_t)source_bb_offset, br_type, trace_flag,
	             source_bb_offset, target_addr, trace_aux0, trace_aux1);
	sync_expected_bb_token(target_addr);
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

	sync_expected_bb_token(matched_target);
	ip = matched_target;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void insertvalue_handler() {
	uint64_t debug_ip_before = ip;
#ifdef GOVM_CPP_DEBUG
	printf("[INSERTVALUE_RAW] ip=%llu bytes=", (unsigned long long)debug_ip_before);
	for (int dbg_i = 0; dbg_i < 40; ++dbg_i) {
		printf("%02x", ((uint8_t *)code_seg_addr)[debug_ip_before + dbg_i]);
		if (dbg_i + 1 != 40) printf(" ");
	}
	printf("\n");
	fflush(NULL);
#endif

	// 结果值位置
	uint8_t res_size = get_byte_code();
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	// 聚合操作数描述
	uint8_t agg_size = get_byte_code();
	uint8_t agg_type = get_byte_code();
	uint64_t agg_offset = 0;
	uint8_t *agg_src = 0;
	uint8_t *agg_tmp = 0;
	if (agg_type == 0) {
		agg_offset = unpack_code(pointer_size);
		agg_src = (uint8_t *)data_seg_addr + agg_offset;
	} else {
		agg_tmp = (uint8_t *)malloc(agg_size);
		if (agg_tmp) {
			read_inline_const_bytes(agg_tmp, agg_size);
			agg_src = agg_tmp;
		} else {
			for (uint32_t i = 0; i < agg_size; ++i) {
				(void)get_byte_code();
			}
		}
	}

	// 要插入的值
	uint8_t insert_size = get_byte_code();
	uint8_t insert_type = get_byte_code();
	uint64_t insert_payload = 0;
	uint64_t insert_value = 0;
	if (insert_type == 0) {
		insert_payload = unpack_code(pointer_size);
		insert_value = unpack_data(insert_payload, insert_size);
	} else {
		if (insert_size <= 8) {
			insert_payload = unpack_code(insert_size);
			insert_value = insert_payload;
		} else {
			insert_payload = ip;
			insert_value = unpack_code(8);
			for (uint32_t i = 8; i < insert_size; ++i) {
				(void)get_byte_code();
			}
		}
	}

	// 偏移量
	uint8_t offset_size = get_byte_code();
	uint8_t offset_type = get_byte_code();
	uint64_t offset_payload = 0;
	uint64_t offset = 0;
	if (offset_type == 0) {
		offset_payload = unpack_code(pointer_size);
		offset = unpack_data(offset_payload, offset_size);
	} else {
		offset_payload = unpack_code(offset_size);
		offset = offset_payload;
	}

	// 值大小
	uint32_t value_size = (uint32_t)unpack_code(4);

	// #region debug-point interpreter-insertvalue
#ifdef GOVM_CPP_DEBUG
	printf("[INSERTVALUE_HANDLER] res_size=%u res_offset=%llu agg_size=%u agg_type=%u agg_offset=%llu insert_size=%u insert_type=%u insert_payload=%llu insert_value=0x%llx offset_size=%u offset_type=%u offset_payload=%llu offset=%llu value_size=%u\n",
	       (unsigned)res_size,
	       (unsigned long long)res_offset,
	       (unsigned)agg_size,
	       (unsigned)agg_type,
	       (unsigned long long)agg_offset,
	       (unsigned)insert_size,
	       (unsigned)insert_type,
	       (unsigned long long)insert_payload,
	       (unsigned long long)insert_value,
	       (unsigned)offset_size,
	       (unsigned)offset_type,
	       (unsigned long long)offset_payload,
	       (unsigned long long)offset,
	       (unsigned)value_size);
	fflush(NULL);
#endif
	// #endregion

	// 复制聚合值到结果位置
	uint64_t dst_addr = data_seg_addr + res_offset;

	// 先复制整个聚合值
	if (agg_src) {
		for (uint32_t i = 0; i < res_size; i++) {
			((uint8_t *)dst_addr)[i] = agg_src[i];
		}
	} else {
		memset((void *)dst_addr, 0, res_size);
	}

	// 然后在指定偏移处插入新值
	for (uint32_t i = 0; i < value_size && i < 8; i++) {
		((uint8_t *)dst_addr)[offset + i] = (uint8_t)(insert_value >> (i * 8));
	}

	if (agg_tmp) {
		free(agg_tmp);
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

	// 聚合操作数的值（data_seg 中的偏移量）
	uint8_t agg_size = get_byte_code();
	uint8_t agg_type = get_byte_code();
	uint64_t agg_offset = 0;
	uint8_t *agg_src = 0;
	uint8_t *agg_tmp = 0;
	if (agg_type == 0) {
		agg_offset = unpack_code(pointer_size);
		agg_src = (uint8_t *)data_seg_addr + agg_offset;
	} else {
		agg_tmp = (uint8_t *)malloc(agg_size);
		if (agg_tmp) {
			read_inline_const_bytes(agg_tmp, agg_size);
			agg_src = agg_tmp;
		} else {
			for (uint32_t i = 0; i < agg_size; ++i) {
				(void)get_byte_code();
			}
		}
	}

	// 偏移量（常量）
	uint8_t offset_size = get_byte_code();
	uint8_t offset_type = get_byte_code();
	uint64_t offset = unpack_code(pointer_size);

	// 结果类型大小
	uint32_t value_size = (uint32_t)unpack_code(4);

	// #region debug-point interpreter-extractvalue
#ifdef GOVM_CPP_DEBUG
	printf("[EXTRACTVALUE_HANDLER] res_size=%u res_offset=%llu agg_size=%u agg_type=%u agg_offset=%llu offset_size=%u offset_type=%u offset=%llu value_size=%u\n",
	       (unsigned)res_size,
	       (unsigned long long)res_offset,
	       (unsigned)agg_size,
	       (unsigned)agg_type,
	       (unsigned long long)agg_offset,
	       (unsigned)offset_size,
	       (unsigned)offset_type,
	       (unsigned long long)offset,
	       (unsigned)value_size);
	fflush(NULL);
#endif
	// #endregion

	// 从 data_seg 中读取聚合操作数的数据
	// agg_offset 是 data_seg 中的偏移量，offset 是结构体中的偏移量
	uint8_t *src_addr = agg_src + offset;

	// 读取 value_size 字节的数据，存储到结果位置
	if (agg_src) {
		memcpy((uint8_t*)data_seg_addr + res_offset, src_addr, value_size);
	} else {
		memset((uint8_t*)data_seg_addr + res_offset, 0, value_size);
	}

	if (agg_tmp) {
		free(agg_tmp);
	}
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

#ifdef GOVM_CPP_DEBUG
		printf("[PHI_HANDLER] res_offset=%llu last_br_from=%llu incoming_bb_id=%llu incoming_value=0x%llx\n",
		       (unsigned long long)res_offset,
		       (unsigned long long)last_br_from_bb_id,
		       (unsigned long long)incoming_bb_id,
		       (unsigned long long)incoming_value);
		fflush(NULL);
#endif

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
	EH_TRACE("[EH_LPAD] begin res_offset=%llu clauses=%u exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\n",
	         (unsigned long long)res_offset,
	         num_clauses,
	         (unsigned)exception_thrown,
	         exception_ptr,
	         exception_selector,
	         caught_exception_ptr,
	         caught_exception_selector,
	         ip);

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
	EH_TRACE("[EH_LPAD] end stored_ptr=%p stored_selector=%d exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\n",
	         exc_ptr,
	         exc_selector,
	         (unsigned)exception_thrown,
	         exception_ptr,
	         exception_selector,
	         caught_exception_ptr,
	         caught_exception_selector,
	         ip);

#ifdef GOVM_CPP_DEBUG
	printf("[LANDINGPAD] Exception caught, ptr=%p, selector=%d\n", exc_ptr, exc_selector);
	fflush(NULL);
#endif
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void resume_handler() {
	// 获取异常聚合值 {ptr, selector}
	uint8_t exc_size = get_byte_code();
	uint8_t exc_type = get_byte_code();
	uint64_t exc_value = 0;
	uint32_t exc_selector_value = exception_selector;

	if (exc_type == 0) {
		// 变量
		uint64_t exc_offset = unpack_code(pointer_size);
		exc_value = unpack_data(exc_offset, exc_size);
		if (exc_size >= pointer_size + 4) {
			exc_selector_value = (uint32_t)unpack_data(exc_offset + pointer_size, 4);
		}
	} else {
		// 常量
		exc_value = unpack_code(exc_size);
		if (exc_size >= pointer_size + 4) {
			exc_selector_value = (uint32_t)unpack_code(4);
		}
	}

	// 重新抛出异常
	exception_thrown = 1;
	exception_ptr = (void*)(uintptr_t)exc_value;
	exception_selector = (int)exc_selector_value;
	EH_TRACE("[EH_RESUME] exc_value=0x%llx exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\n",
	         (unsigned long long)exc_value,
	         (unsigned)exception_thrown,
	         exception_ptr,
	         exception_selector,
	         caught_exception_ptr,
	         caught_exception_selector,
	         ip);

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
	uint32_t num_targets = (uint32_t)unpack_code(4);
	uint64_t matched_target = 0;

	// #region debug-point interpreter-indirectbr
#ifdef GOVM_CPP_DEBUG
	printf("[INDIRECTBR_HANDLER] raw_target=0x%llx num_targets=%u current_code_seg=0x%llx\n",
	       (unsigned long long)target_addr,
	       (unsigned)num_targets,
	       (unsigned long long)code_seg_addr);
	fflush(NULL);
#endif
	// #endregion

	for (uint32_t i = 0; i < num_targets; ++i) {
		uint64_t candidate_addr = get_value();
		uint64_t candidate_target = unpack_code(pointer_size);
		if (target_addr == candidate_addr) {
			matched_target = candidate_target;
		}
	}

	sync_expected_bb_token(matched_target);
	ip = matched_target;
}

#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
void extractelement_handler() {
	vm_debug_log_ip_stage("extractelement-enter", ip);
	uint8_t res_size = get_byte_code();
	vm_debug_log_u32_stage("extractelement-after-res-size", res_size, (uint32_t)ip);
	uint8_t res_type = get_byte_code();
	uint64_t res_offset = unpack_code(pointer_size);

	uint8_t vector_tmp[256];
	uint64_t vector_ptr = get_aggregate_addr(vector_tmp, sizeof(vector_tmp), NULL);
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

	uint8_t vector_tmp[256];
	uint64_t vector_ptr = get_aggregate_addr(vector_tmp, sizeof(vector_tmp), NULL);
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

	uint8_t v1_tmp[256];
	uint8_t v2_tmp[256];
	uint64_t v1_ptr = get_aggregate_addr(v1_tmp, sizeof(v1_tmp), NULL);
	uint64_t v2_ptr = get_aggregate_addr(v2_tmp, sizeof(v2_tmp), NULL);
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
		EH_TRACE("[EH_CSW] no-exception handlers=%u unwind=%llu exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\n",
		         num_handlers,
		         (unsigned long long)unwind_target,
		         exception_ptr,
		         exception_selector,
		         caught_exception_ptr,
		         caught_exception_selector,
		         ip);
		if (unwind_target != 0) {
			sync_expected_bb_token(unwind_target);
			ip = (uint32_t)unwind_target;
		}
		return;
	}

	// 获取异常对象的类型信息
	// 在C++ ABI中，异常对象包含类型信息
	// exception_selector通常包含类型信息的索引或指针
	uint64_t exception_type_info = (uint64_t)exception_selector;
	EH_TRACE("[EH_CSW] begin handlers=%u unwind=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u\n",
	         num_handlers,
	         (unsigned long long)unwind_target,
	         (unsigned)exception_thrown,
	         exception_ptr,
	         exception_selector,
	         caught_exception_ptr,
	         caught_exception_selector,
	         ip);

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

		// 使用完整的 RTTI 类型匹配逻辑
		void *adjusted_ptr = NULL;
		bool type_matches = can_catch_exception(
			(const void *)exception_type_info,
			(const void *)catch_type_info,
			exception_ptr,
			&adjusted_ptr
		);

#ifdef GOVM_CPP_DEBUG
		printf("[CATCHSWITCH]   Type match result: %s, adjusted_ptr=%p\n",
		       type_matches ? "YES" : "NO", adjusted_ptr);
		fflush(NULL);
#endif

		if (type_matches) {
#ifdef GOVM_CPP_DEBUG
			printf("[CATCHSWITCH] Matched! Jumping to handler %lu\n", handler_target);
			fflush(NULL);
#endif

			// 更新异常指针（如果有调整）
			if (adjusted_ptr) {
				exception_ptr = adjusted_ptr;
			}

			// 跳转到匹配的handler
			EH_TRACE("[EH_CSW] matched target=%llu adjusted_ptr=%p exception_ptr=%p selector=%d ip=%u\n",
			         (unsigned long long)handler_target,
			         adjusted_ptr,
			         exception_ptr,
			         exception_selector,
			         ip);
			sync_expected_bb_token(handler_target);
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
		sync_expected_bb_token(unwind_target);
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
}

// call_handler is declared extern in the header and replaced at link time


#ifdef IS_INLINE_FUNC
	__inline__ __attribute__((always_inline))
#endif
/* Get Opcode, Opcode encrypt by xorshift32*/
uint8_t get_opcode() {
	uint8_t cnt = 0;
	uint8_t his[OP_TOTAL + 1];
	uint32_t opcode_ip = (uint32_t)ip;
	uint64_t saved_opcode_state = opcode_xorshift32_state;
	uint64_t saved_vm_state = vm_code_state;
	uint32_t payload_index = opcode_ip - (uint32_t)current_bb_id - VM_BB_HEADER_SIZE;
	if (payload_index == 0) {
		vm_debug_log_u32_stage("get-opcode-enter", opcode_ip, payload_index);
	}
	uint8_t raw_byte = ((uint8_t *)code_seg_addr)[ip++];
	uint8_t curr_byte = raw_byte;
	{
		uint32_t key_words[8];
		uint32_t nonce_words[3];
		uint8_t block[64];
		uint32_t block_index = payload_index / 64U;
		uint32_t block_offset = payload_index % 64U;
		derive_chacha_material(vm_function_key, vm_code_state, vm_block_chain_state,
		                      (uint32_t)current_bb_id, key_words, nonce_words);
		chacha20_block(key_words, block_index, nonce_words, block);
		curr_byte ^= block[block_offset];
		if (payload_index == 0) {
			vm_debug_log_u32_stage("get-opcode-after-chacha", raw_byte, curr_byte);
		}
	}
	if (payload_index == 0) {
		vm_debug_log_u32_stage("get-opcode-after-chacha-confirm", raw_byte, curr_byte);
	}

	for (int i = 0; i < OP_TOTAL + 1; i++) {
		uint8_t tmp = (uint8_t)xorshift32(&opcode_xorshift32_state);
		if (tmp == curr_byte) {
			vm_trace_push(VM_TRACE_KIND_OPCODE, opcode_ip, (uint8_t)(i + 1), 0,
			             raw_byte, saved_opcode_state, saved_vm_state, current_bb_id);
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

	vm_trace_push(VM_TRACE_KIND_OPCODE, opcode_ip, 0xFF, 0,
	             raw_byte, saved_opcode_state, saved_vm_state, current_bb_id);
	return 0xFF;
}


void vm_interpreter() {
	// #region debug-point vm-stdio-entry-call
	vm_debug_log_stdio_entry("vm-entry");
	// #endregion debug-point vm-stdio-entry-call

	// DEBUG: Print entry
	DEBUG(DEBUG_ID_NEW_BB, 999);

	// init pointer size based on architecture
	pointer_size = sizeof(void*);

	// init
	ip = 0;
	expected_bb_token = 0;
	vm_block_chain_state = 0;
	vm_debug_log_ip_stage("vm-init-done", ip);

	// when step into a new basicblock, we need to fetch opcode_seed and vm_code_seed
	uint8_t is_a_new_bb = 1;

	int instruction_count = 0;

	while (1) {
		instruction_count++;

		DEBUG(DEBUG_ID_IP, instruction_count);

		if (is_a_new_bb) {
			vm_debug_log_ip_stage("vm-new-bb-enter", ip);
			current_bb_id = ip;
			uint32_t bb_tag = read_code_u32_raw();
			uint64_t bb_token = derive_bb_token(vm_function_key, (uint32_t)current_bb_id);
			uint32_t expected_tag = derive_bb_tag(vm_function_key, (uint32_t)current_bb_id);
			if (expected_bb_token != 0 && bb_token != expected_bb_token) {
				vm_dump_fault_context("vm-bb-token-mismatch", bb_token, expected_bb_token);
				return;
			}
			if (bb_tag != expected_tag) {
				vm_dump_fault_context("vm-bb-tag-mismatch", bb_tag, expected_tag);
				return;
			}
			opcode_xorshift32_state = derive_opcode_seed(bb_token);
			vm_code_state = derive_vm_seed(vm_function_key, bb_token);
			vm_block_chain_state = derive_chain_seed(vm_function_key, bb_token,
			                                        (uint32_t)current_bb_id);
			expected_bb_token = 0;
			is_a_new_bb = 0;
			vm_debug_log_ip_stage("vm-new-bb-seeds-ready", ip);
			vm_trace_push(VM_TRACE_KIND_BB, (uint32_t)ip, 0, 0,
			             opcode_xorshift32_state, vm_code_state, bb_token,
			             vm_block_chain_state);

			DEBUG(DEBUG_ID_NEW_BB, ip);
		}

		// switch op_code and add ip
		vm_debug_log_ip_stage("vm-before-get-opcode", ip);
		uint8_t opcode = get_opcode();
		vm_debug_log_u32_stage("vm-after-get-opcode", opcode, (uint32_t)ip);
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
				// 保存所有全局状态，因为递归调用会破坏这些状态
				int saved_ip = ip;
				uint64_t saved_opcode_state = opcode_xorshift32_state;
				uint64_t saved_vmcode_state = vm_code_state;
				uint64_t saved_chain_state = vm_block_chain_state;
				uint64_t saved_expected_token = expected_bb_token;
				uintptr_t saved_data_seg_addr = data_seg_addr;
				uintptr_t saved_code_seg_addr = code_seg_addr;
				uint64_t saved_function_key = vm_function_key;
				vm_trace_push(VM_TRACE_KIND_CALL, (uint32_t)saved_ip, 0, (uint8_t)exception_thrown,
				             packed_funcid, (uint64_t)saved_ip, offset, (uint64_t)caught_exception_selector);
				EH_TRACE("[EH_VM_CALL] before funcid=%llu saved_ip=%u offset=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d opcode_state=0x%016llx vm_state=0x%016llx\n",
				         (unsigned long long)packed_funcid,
				         (unsigned)saved_ip,
				         (unsigned long long)offset,
				         (unsigned)exception_thrown,
				         exception_ptr,
				         exception_selector,
				         caught_exception_ptr,
				         caught_exception_selector,
				         (unsigned long long)saved_opcode_state,
				         (unsigned long long)saved_vmcode_state);
				dispatch_code_seg_addr = saved_code_seg_addr;
				// 使用异常捕获包装函数
				call_handler_with_exception_handling(packed_funcid);
				vm_trace_push(VM_TRACE_KIND_CALL, (uint32_t)saved_ip, 1, (uint8_t)exception_thrown,
				             packed_funcid, (uint64_t)saved_ip, offset, (uint64_t)caught_exception_selector);
				EH_TRACE("[EH_VM_CALL] after funcid=%llu saved_ip=%u offset=%llu exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d ip=%u opcode_state=0x%016llx vm_state=0x%016llx\n",
				         (unsigned long long)packed_funcid,
				         (unsigned)saved_ip,
				         (unsigned long long)offset,
				         (unsigned)exception_thrown,
				         exception_ptr,
				         exception_selector,
				         caught_exception_ptr,
				         caught_exception_selector,
				         ip,
				         (unsigned long long)opcode_xorshift32_state,
				         (unsigned long long)vm_code_state);
				// 恢复所有状态，确保返回后能正确解密后续操作码
				ip = saved_ip;
				opcode_xorshift32_state = saved_opcode_state;
				vm_code_state = saved_vmcode_state;
				vm_block_chain_state = saved_chain_state;
				expected_bb_token = saved_expected_token;
				data_seg_addr = saved_data_seg_addr;
				code_seg_addr = saved_code_seg_addr;
				vm_function_key = saved_function_key;
				EH_TRACE("[EH_VM_CALL] restored funcid=%llu restore_ip=%u exception_thrown=%u exception_ptr=%p selector=%d caught_ptr=%p caught_selector=%d opcode_state=0x%016llx vm_state=0x%016llx\n",
				         (unsigned long long)packed_funcid,
				         (unsigned)ip,
				         (unsigned)exception_thrown,
				         exception_ptr,
				         exception_selector,
				         caught_exception_ptr,
				         caught_exception_selector,
				         (unsigned long long)opcode_xorshift32_state,
				         (unsigned long long)vm_code_state);
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

				// 保存所有全局状态，因为递归调用会破坏这些状态
				int saved_ip = ip;
				uint64_t saved_opcode_state = opcode_xorshift32_state;
				uint64_t saved_vmcode_state = vm_code_state;
				uint64_t saved_chain_state = vm_block_chain_state;
				uint64_t saved_expected_token = expected_bb_token;
				uintptr_t saved_data_seg_addr = data_seg_addr;
				uintptr_t saved_code_seg_addr = code_seg_addr;
				uint64_t saved_function_key = vm_function_key;

				// 调用函数（与 Call_OP 相同）
				dispatch_code_seg_addr = saved_code_seg_addr;
				// 使用异常捕获包装函数
				call_handler_with_exception_handling(packed_funcid);

				// 恢复所有状态，确保返回后能正确解密后续操作码
				ip = saved_ip;
				opcode_xorshift32_state = saved_opcode_state;
				vm_code_state = saved_vmcode_state;
				vm_block_chain_state = saved_chain_state;
				expected_bb_token = saved_expected_token;
				data_seg_addr = saved_data_seg_addr;
				code_seg_addr = saved_code_seg_addr;
				vm_function_key = saved_function_key;

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

				// 获取指针 - 修复：使用 unpack_data 从 data_seg 读取指针值
				uint64_t ptr_value = unpack_data(ptr_offset, pointer_size);
				uint8_t *ptr = (uint8_t*)ptr_value;

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
#ifdef GOVM_CPP_DEBUG
				printf("[ATOMIC_RMW] op_val=%u\n", op_val);
				fflush(NULL);
#endif

				// packed_res: 结果值（原始值）
				uint8_t res_type_size = get_byte_code();
				uint8_t res_type_id = get_byte_code();
				uint64_t res_offset = unpack_code(pointer_size);
#ifdef GOVM_CPP_DEBUG
				printf("[ATOMIC_RMW] res_offset=%llu\n", (unsigned long long)res_offset);
				fflush(NULL);
#endif

				// packed_ptr: 指针操作数
				uint8_t ptr_type_size = get_byte_code();
				uint8_t ptr_type_id = get_byte_code();
				uint64_t ptr_offset = unpack_code(pointer_size);
#ifdef GOVM_CPP_DEBUG
				printf("[ATOMIC_RMW] ptr_offset=%llu\n", (unsigned long long)ptr_offset);
				fflush(NULL);
#endif

				// packed_val: 值操作数
				uint8_t val_type_size = get_byte_code();
				uint8_t val_type_id = get_byte_code();
				uint64_t val;
				if (val_type_id == 0) {
					// 变量：读取偏移量
					uint64_t val_offset = unpack_code(pointer_size);
					val = unpack_data(val_offset, val_type_size);
				} else {
					// 常量：直接读取值
					val = unpack_code(val_type_size);
				}
#ifdef GOVM_CPP_DEBUG
				printf("[ATOMIC_RMW] val=%llu\n", (unsigned long long)val);
				fflush(NULL);
#endif

				// 内存序
				uint8_t ordering = get_byte_code();
#ifdef GOVM_CPP_DEBUG
				printf("[ATOMIC_RMW] ordering=%u\n", ordering);
				fflush(NULL);
#endif

				// 值类型大小
				uint32_t val_size = (uint32_t)unpack_code(4);
#ifdef GOVM_CPP_DEBUG
				printf("[ATOMIC_RMW] val_size=%u\n", val_size);
				fflush(NULL);
#endif

				// 获取指针 - 修复：使用 unpack_data 从 data_seg 读取指针值
				uint64_t ptr_value = unpack_data(ptr_offset, pointer_size);
				uint8_t *ptr = (uint8_t*)ptr_value;

#ifdef GOVM_CPP_DEBUG
				printf("[ATOMIC_RMW] ptr_value=0x%llx val=%u\n", (unsigned long long)ptr_value, (uint32_t)val);
				fflush(NULL);
#endif

				// 执行原子读-修改-写操作
				uint8_t old_val[16];

				// 根据操作类型和值大小执行相应的原子操作
				if (val_size == 4) {
					uint32_t operand = (uint32_t)val;
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
					uint64_t operand = val;
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
			{
				uint64_t available = vm_trace_total < VM_CRASH_TRACE_DEPTH ? vm_trace_total : VM_CRASH_TRACE_DEPTH;
				uint64_t start = vm_trace_total > VM_CRASH_TRACE_DEPTH ? vm_trace_next : 0;
				uint32_t fault_ip = ip > 0 ? (uint32_t)(ip - 1) : 0;

				printf("\n[VM_CRASH] reason=unknown_opcode detail0=%llu detail1=%llu ip=%u current_bb=%llu last_br_from=%llu exception=%u selector=%d\n",
				       (unsigned long long)opcode,
				       (unsigned long long)current_bb_id,
				       (unsigned)fault_ip,
				       (unsigned long long)current_bb_id,
				       (unsigned long long)last_br_from_bb_id,
				       (unsigned)exception_thrown,
				       exception_selector);

				printf("[VM_CRASH] opcode_state=0x%08x vm_state=0x%08x code_bytes=",
				       (unsigned)opcode_xorshift32_state,
				       (unsigned)vm_code_state);
				{
					uint32_t raw_start = fault_ip > 8 ? fault_ip - 8 : 0;
					for (uint32_t i = 0; i < 24; ++i) {
						uint32_t pos = raw_start + i;
						printf("%02x", ((uint8_t *)code_seg_addr)[pos]);
						if (i + 1 != 24) printf(" ");
					}
				}
				printf("\n");

				for (uint64_t i = 0; i < available; ++i) {
					VMTraceEntry *entry = &vm_trace_ring[(start + i) % VM_CRASH_TRACE_DEPTH];
					switch (entry->kind) {
						case VM_TRACE_KIND_BB:
							printf("[VM_CRASH][TRACE] bb ip=%u opcode_seed=0x%08llx vm_seed=0x%08llx\n",
							       entry->ip_value,
							       (unsigned long long)entry->a,
							       (unsigned long long)entry->b);
							break;
						case VM_TRACE_KIND_OPCODE:
							printf("[VM_CRASH][TRACE] opcode ip=%u op=%u raw=0x%02llx bb=%llu opcode_state=0x%08llx vm_state=0x%08llx\n",
							       entry->ip_value,
							       (unsigned)entry->tag0,
							       (unsigned long long)entry->a,
							       (unsigned long long)entry->d,
							       (unsigned long long)entry->b,
							       (unsigned long long)entry->c);
							break;
						case VM_TRACE_KIND_BRANCH:
							printf("[VM_CRASH][TRACE] br source=%llu target=%llu type=%u flag=%u aux0=%llu aux1=%llu\n",
							       (unsigned long long)entry->a,
							       (unsigned long long)entry->b,
							       (unsigned)entry->tag0,
							       (unsigned)entry->tag1,
							       (unsigned long long)entry->c,
							       (unsigned long long)entry->d);
							break;
						case VM_TRACE_KIND_CALL:
							printf("[VM_CRASH][TRACE] call stage=%s funcid=%llu saved_ip=%llu res_offset=%llu exc=%u selector=%llu\n",
							       entry->tag0 ? "leave" : "enter",
							       (unsigned long long)entry->a,
							       (unsigned long long)entry->b,
							       (unsigned long long)entry->c,
							       (unsigned)entry->tag1,
							       (unsigned long long)entry->d);
							break;
						default:
							break;
					}
				}

				printf("[VM] Unknown opcode 0x%02x, returning\n", opcode);
				fflush(NULL);
				return;
			}
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
