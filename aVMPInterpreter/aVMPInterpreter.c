#include "aVMPInterpreter.h"

// Debug functions - implemented by VMP pass via IR injection
// Controlled by -irobf-debug flag
extern void vmp_debug_id(int id, uint64_t val);

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

#define DEBUG(id, val) vmp_debug_id(id, val)

#define SEG_SIZE 5000

#define IS_INLINE_FUNC

// #define TEST_GOVM_C

uint8_t gv_code_seg[SEG_SIZE] = {
    #ifdef TEST_GOVM_C
    //  0, 8, 0, 12, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 28, 0, 0, 0, 0, 0, 0, 0, 36, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 48, 0, 0, 0, 0, 0, 0, 0, 17, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 12, 0, 0, 0, 0, 0, 0, 0, 17, 4, 0, 8, 0, 0, 0, 0, 0, 0, 0, 8, 0, 28, 0, 0, 0, 0, 0, 0, 0, 17, 4, 11, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 96, 0, 126, 0, 0, 0, 0, 0, 0, 0, 16, 4, 0, 52, 0, 0, 0, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 16, 4, 0, 56, 0, 0, 0, 0, 0, 0, 0, 8, 0, 28, 0, 0, 0, 0, 0, 0, 0, 64, 40, 1, 0, 60, 0, 0, 0, 0, 0, 0, 0, 4, 0, 52, 0, 0, 0, 0, 0, 0, 0, 4, 0, 56, 0, 0, 0, 0, 0, 0, 0, 96, 1, 1, 0, 60, 0, 0, 0, 0, 0, 0, 0, 228, 0, 0, 0, 0, 0, 0, 0, 14, 2, 0, 0, 0, 0, 0, 0, 16, 8, 0, 61, 0, 0, 0, 0, 0, 0, 0, 8, 0, 12, 0, 0, 0, 0, 0, 0, 0, 16, 4, 0, 69, 0, 0, 0, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 80, 8, 0, 73, 0, 0, 0, 0, 0, 0, 0, 4, 0, 69, 0, 0, 0, 0, 0, 0, 0, 48, 1, 11, 8, 0, 81, 0, 0, 0, 0, 0, 0, 0, 8, 0, 61, 0, 0, 0, 0, 0, 0, 0, 8, 0, 73, 0, 0, 0, 0, 0, 0, 0, 16, 1, 0, 89, 0, 0, 0, 0, 0, 0, 0, 8, 0, 81, 0, 0, 0, 0, 0, 0, 0, 80, 4, 0, 90, 0, 0, 0, 0, 0, 0, 0, 1, 0, 89, 0, 0, 0, 0, 0, 0, 0, 32, 29, 4, 0, 94, 0, 0, 0, 0, 0, 0, 0, 4, 0, 90, 0, 0, 0, 0, 0, 0, 0, 4, 11, 58, 0, 0, 0, 80, 1, 0, 98, 0, 0, 0, 0, 0, 0, 0, 4, 0, 94, 0, 0, 0, 0, 0, 0, 0, 17, 1, 0, 98, 0, 0, 0, 0, 0, 0, 0, 8, 0, 81, 0, 0, 0, 0, 0, 0, 0, 96, 0, 190, 1, 0, 0, 0, 0, 0, 0, 16, 4, 0, 99, 0, 0, 0, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 32, 12, 4, 0, 103, 0, 0, 0, 0, 0, 0, 0, 4, 0, 99, 0, 0, 0, 0, 0, 0, 0, 4, 11, 1, 0, 0, 0, 17, 4, 0, 103, 0, 0, 0, 0, 0, 0, 0, 8, 0, 40, 0, 0, 0, 0, 0, 0, 0, 96, 0, 126, 0, 0, 0, 0, 0, 0, 0, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    243, 149, 17, 126, 14, 92, 244, 62, 16, 249, 39, 46, 15, 215, 218, 109, 247, 148, 139, 153, 184, 179, 1, 197, 71, 203, 253, 79, 245, 251, 238, 205, 163, 80, 118, 201, 130, 155, 240, 35, 189, 175, 219, 162, 206, 47, 50, 86, 244, 152, 43, 206, 127, 7, 96, 119, 37, 102, 184, 254, 124, 155, 77, 58, 139, 170, 132, 121, 240, 116, 126, 214, 205, 100, 252, 152, 52, 140, 120, 219, 59, 193, 209, 118, 220, 192, 214, 152, 143, 83, 100, 77, 57, 188, 93, 250, 68, 63, 108, 155, 22, 167, 228, 220, 170, 140, 10, 45, 104, 130, 211, 251, 19, 57, 102, 108, 126, 85, 231, 187, 100, 93, 150, 50, 253, 3, 33, 213, 60, 239, 3, 11, 161, 97, 25, 34, 219, 88, 79, 248, 180, 38, 6, 212, 162,154, 2, 134, 179, 143, 126, 212, 140, 97, 130, 46, 230, 127, 234, 144, 237, 169, 186, 228, 86, 179, 113, 217, 52, 175, 203, 137, 105, 211, 180, 119, 188, 10, 32, 95, 209, 108, 233, 12, 99, 194, 144, 72, 35, 10, 121, 124, 8, 71, 170, 33, 90, 158, 233, 11, 65, 73, 142, 49, 188, 210, 200, 250, 146, 117, 24, 16, 129, 114, 246, 70, 178, 106, 216, 100, 147, 138, 111, 235, 36, 159, 86, 130, 47, 136, 128, 246, 187, 98, 226, 242, 240, 148, 170, 161, 171, 34, 24, 131, 73, 13, 7, 7, 156, 237, 190, 53, 75, 168, 231, 138, 0, 39, 130, 190, 155, 242, 130, 173, 20, 151, 199, 112, 211, 116, 113, 172, 189, 150, 228, 122, 78, 191, 177, 161, 119, 63, 74, 187, 121, 199, 165, 0, 146, 203, 253, 189, 85, 144, 24, 162, 117, 130, 200, 223, 110, 92, 116, 98, 240, 209, 246, 12, 19, 236, 6, 242, 36, 76, 32, 26, 101, 82, 176, 68, 218, 125, 48, 20, 14, 221, 234, 50, 141, 216, 17, 57, 243, 191, 56, 145, 204, 213, 193, 162, 89, 21, 143, 170, 184, 238, 62, 92, 62, 19, 43, 160, 171, 223, 23, 187, 144, 35, 19, 116, 64, 11, 27, 212, 249, 236, 34, 77, 191, 45, 58, 139, 156, 39, 255, 15, 163, 196, 154, 151, 74, 102, 211, 135, 198, 225, 185, 139, 98, 149, 71, 200, 109, 47, 161, 181, 161, 215, 84, 82, 173, 45, 160, 55, 116, 159, 30, 180, 46, 45, 84, 25, 120, 245, 51, 197, 195, 11, 19, 184, 239, 242, 155, 31, 94, 131, 55, 68, 43, 82, 55, 27, 86, 9, 255, 0, 0, 0
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
uint32_t xorshift32(uint32_t *state)
{
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
// unpack data from data_seg
uint64_t unpack_data(uint64_t offset, int size) {
    uint64_t res = 0;
    
    for (int i = 0; i < size; i++) {
        // must add (uint64_t), or overflow int32
        res |= (uint64_t)((uint8_t *)data_seg_addr)[offset++] << (8 * i);
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
        res |= (uint64_t)*ptr << (8 * i);
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

        // get var_offset of data_seg
        uint64_t var_offset = unpack_code(pointer_size);

        // fetch data from data_seg
        res = unpack_data(var_offset, value_size);
    }
    else {
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

        // get var_offset of data_seg
        uint64_t var_offset = unpack_code(pointer_size);

        // fetch data from data_seg
        res = unpack_data(var_offset, value_size);
    }
    else {
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

        // get var_offset of data_seg
        uint64_t var_offset = unpack_code(pointer_size);

        // fetch data from data_seg
        res = unpack_data(var_offset, value_size);
    }
    else {
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
    // set_var(var_offset, pointer_size, data_seg_addr+area_offset);
    pack_store_addr(data_seg_addr+var_offset, data_seg_addr+area_offset, var_size);
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
    pack_store_addr(data_seg_addr+var_offset, load_value, var_size);
}

#ifdef IS_INLINE_FUNC
    __inline__ __attribute__((always_inline))
#endif
void store_handler() {
    uint8_t var_size = get_byte_code();
    uint8_t var_type = get_byte_code();
    uint64_t store_value = get_value_with_size(var_size, var_type);

    uint8_t ptr_size = get_byte_code();
    uint8_t ptr_type = get_byte_code();
    uint64_t ptr_offset = unpack_code(pointer_size);

    uint64_t ptr = unpack_data(ptr_offset, pointer_size);

    // printf("store ptr: %lx, store_value: %lx, var_size: %lx\n", ptr, store_value, var_size);
    pack_store_addr(ptr, store_value, var_size);
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
            res_value = (uint64_t)*(uint32_t*)&fr;
        } else {
            double d1 = *(double*)&op1_value;
            double dr = -d1;
            res_value = *(uint64_t*)&dr;
        }
    } else {
        uint8_t op1_size, op2_size;
        uint64_t op1_value = get_value_ex(&op1_size);
        uint64_t op2_value = get_value_ex(&op2_size);

        switch (op_code)
        {
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
                    res_value = (uint64_t)*(uint32_t*)&fr;
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
                    res_value = (uint64_t)*(uint32_t*)&fr;
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
                    res_value = (uint64_t)*(uint32_t*)&fr;
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
                    res_value = (uint64_t)*(uint32_t*)&fr;
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
                    res_value = (uint64_t)*(uint32_t*)&fr;
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

    pack_store_addr(data_seg_addr+res_offset, res_value, res_size);

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
    } 
    else {
        // struct type - idx_value是成员偏移量（常量）
        res_value = ptr_value + idx_value;
    }

    pack_store_addr(data_seg_addr+res_offset, res_value, res_size);
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
            switch (predicate)
            {
                case FCMP_FALSE: res_value = 0; break;
                case FCMP_OEQ:   res_value = (a == b); break;
                case FCMP_OGT:   res_value = (a > b); break;
                case FCMP_OGE:   res_value = (a >= b); break;
                case FCMP_OLT:   res_value = (a < b); break;
                case FCMP_OLE:   res_value = (a <= b); break;
                case FCMP_ONE:   res_value = (a != b); break;
                case FCMP_ORD:   res_value = !((a != a) || (b != b)); break;
                case FCMP_UNO:   res_value = ((a != a) || (b != b)); break;
                case FCMP_UEQ:   res_value = ((a == b) || (a != a) || (b != b)); break;
                case FCMP_UGT:   res_value = ((a > b) || (a != a) || (b != b)); break;
                case FCMP_UGE:   res_value = ((a >= b) || (a != a) || (b != b)); break;
                case FCMP_ULT:   res_value = ((a < b) || (a != a) || (b != b)); break;
                case FCMP_ULE:   res_value = ((a <= b) || (a != a) || (b != b)); break;
                case FCMP_UNE:   res_value = ((a != b) || (a != a) || (b != b)); break;
                case FCMP_TRUE:  res_value = 1; break;
                default: break;
            }
        } else {
            double a = *(double*)&op1_value;
            double b = *(double*)&op2_value;
            switch (predicate)
            {
                case FCMP_FALSE: res_value = 0; break;
                case FCMP_OEQ:   res_value = (a == b); break;
                case FCMP_OGT:   res_value = (a > b); break;
                case FCMP_OGE:   res_value = (a >= b); break;
                case FCMP_OLT:   res_value = (a < b); break;
                case FCMP_OLE:   res_value = (a <= b); break;
                case FCMP_ONE:   res_value = (a != b); break;
                case FCMP_ORD:   res_value = !((a != a) || (b != b)); break;
                case FCMP_UNO:   res_value = ((a != a) || (b != b)); break;
                case FCMP_UEQ:   res_value = ((a == b) || (a != a) || (b != b)); break;
                case FCMP_UGT:   res_value = ((a > b) || (a != a) || (b != b)); break;
                case FCMP_UGE:   res_value = ((a >= b) || (a != a) || (b != b)); break;
                case FCMP_ULT:   res_value = ((a < b) || (a != a) || (b != b)); break;
                case FCMP_ULE:   res_value = ((a <= b) || (a != a) || (b != b)); break;
                case FCMP_UNE:   res_value = ((a != b) || (a != a) || (b != b)); break;
                case FCMP_TRUE:  res_value = 1; break;
                default: break;
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
        switch (predicate)
        {
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

    pack_store_addr(data_seg_addr+res_offset, res_value, res_size);
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
                res_value = (uint64_t)*(uint32_t*)&fval;
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
                res_value = (uint64_t)*(uint32_t*)&fval;
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
                res_value = (uint64_t)*(uint32_t*)&fval;
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

    pack_store_addr(data_seg_addr+res_offset, res_value, res_size);
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
    }
    else {
        // condition
        uint64_t condition_value = get_value();
        uint64_t true_br = unpack_code(pointer_size);
        uint64_t false_br = unpack_code(pointer_size);

        if (condition_value) {
            target_addr = true_br;
        } 
        else {
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

    pack_store_addr(data_seg_addr + res_offset, (uint64_t)(uintptr_t)exception_ptr, pointer_size);
    pack_store_addr(data_seg_addr + res_offset + pointer_size, (uint64_t)exception_selector, 4);

    exception_thrown = 0;
}

#ifdef IS_INLINE_FUNC
    __inline__ __attribute__((always_inline))
#endif
void resume_handler() {
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
void data_seg_clean(int return_value_off) {
    // clean data seg, from the end of return value
    for (unsigned i=return_value_off; i<SEG_SIZE; i++) {
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
        pack_store_addr((uint64_t)data_seg_addr, ret_value, var_size);
    }
    // we dont know data_seg size, may segmentfault
    // data_seg_clean(var_size);
    // for (unsigned i=var_size; i<SEG_SIZE; i++) {
    //     ((uint8_t *)data_seg_addr)[i] = 0;
    // }
}

// call_handler is declared extern in the header and replaced at link time


#ifdef IS_INLINE_FUNC
    __inline__ __attribute__((always_inline))
#endif
/* Get Opcode, Opcode encrypt by xorshift32*/
uint8_t get_opcode() {
    uint8_t cnt = 0;
    uint8_t his[OP_TOTAL+1];

    uint8_t curr_byte = ((uint8_t *)code_seg_addr)[ip++];
    curr_byte ^= (xorshift32(&vm_code_state) & 0xFF);

    for (int i = 0; i < OP_TOTAL+1; i++) {
        uint8_t tmp = xorshift32(&opcode_xorshift32_state);
        if (tmp == curr_byte) {
            return i+1;
        }
        
        uint8_t flag = 1;
        for (int j=0; j < i; j++) {
            if (his[j] == tmp) {
                flag = 0;
            }
        }

        if (flag == 1) {
            his[i] = tmp;
        }
        else {
            i--;
        }
    }

    return 0xFF;
}


void vm_interpreter() {

    // DEBUG: Print entry
    #ifdef GOVM_CPP_DEBUG
    printf("[VM] vm_interpreter started\n");
    fflush(stdout);
    #endif

    // init pointer size based on architecture
    pointer_size = sizeof(void*);

    // DEBUG: Print pointer size
    #ifdef GOVM_CPP_DEBUG
    printf("[VM] pointer_size = %u\n", pointer_size);
    fflush(stdout);
    #endif

    // init
    ip = 0;

    // DEBUG: Print data_seg_addr and code_seg_addr
    #ifdef GOVM_CPP_DEBUG
    printf("[VM] data_seg_addr = 0x%lx\n", (unsigned long)data_seg_addr);
    printf("[VM] code_seg_addr = 0x%lx\n", (unsigned long)code_seg_addr);
    fflush(stdout);
    #endif

    // when step into a new basicblock, we need to fetch opcode_seed and vm_code_seed
    uint8_t is_a_new_bb = 1;
    
    int instruction_count = 0;
    
    while(1) {
        instruction_count++;
        
        #ifdef GOVM_CPP_DEBUG
        if (instruction_count <= 10 || instruction_count % 100 == 0) {
            printf("[VM] Instruction #%d, ip=%lu\n", instruction_count, (unsigned long)ip);
            fflush(stdout);
        }
        #endif

        if (is_a_new_bb) {
            opcode_xorshift32_state = get_xorshift_seed();
            vm_code_state = get_xorshift_seed();
            is_a_new_bb = 0;
            current_bb_id = ip;
            
            #ifdef GOVM_CPP_DEBUG
            printf("[VM] New BB: ip=%lu, op_seed=%u, vm_seed=%u\n", 
                   (unsigned long)ip, opcode_xorshift32_state, vm_code_state);
            fflush(stdout);
            #endif
        }

        // switch op_code and add ip
        uint8_t opcode = get_opcode();
        #ifdef GOVM_CPP_DEBUG
        if (instruction_count <= 20) {
            printf("[VM]   opcode = 0x%02x\n", opcode);
            fflush(stdout);
        }
        #endif
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
            case Ret_OP:
                #ifdef GOVM_CPP_DEBUG
                printf("[VM] Ret_OP encountered, returning\n");
                fflush(stdout);
                #endif
                return_handler();
                return;
                break;
            case Call_OP:
                call_handler(unpack_code(pointer_size));
                break;
            default:
                #ifdef GOVM_CPP_DEBUG
                printf("[VM] Unknown opcode 0x%02x, returning\n", opcode);
                fflush(stdout);
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
//     ((uintptr_t *)gv_data_seg)[0] = (uintptr_t) test;
//     ((uint32_t *)gv_data_seg)[2] = len;
//     data_seg_addr = (uintptr_t) gv_data_seg;
//     code_seg_addr = (uintptr_t) gv_code_seg;
//     vm_interpreter();
//     for(int i=0; i < len; i++) {
//         printf("%d, ", test[i]);
//     }
//     printf("\n");
// }
