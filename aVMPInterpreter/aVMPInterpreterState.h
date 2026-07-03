#ifndef AVMP_INTERPRETER_STATE_H
#define AVMP_INTERPRETER_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include "aVMPInterpreterConfig.h"

typedef struct vm_call_frame_t {
    int ip;
    uint64_t opcode_xorshift32_state;
    uint64_t vm_code_state;
    uint64_t vm_block_chain_state;
    uint64_t expected_bb_token;
    uintptr_t data_seg_addr;
    uintptr_t code_seg_addr;
    uintptr_t dispatch_code_seg_addr;
    uint64_t vm_function_key;
    uint64_t last_br_from_bb_id;
    uint64_t current_bb_id;
} vm_call_frame_t;

extern uint8_t gv_code_seg[SEG_SIZE];
extern uint8_t gv_data_seg[SEG_SIZE];
extern uintptr_t data_seg_addr;
extern uintptr_t code_seg_addr;
extern uintptr_t dispatch_code_seg_addr;
extern int ip;
extern unsigned pointer_size;
extern uint64_t opcode_xorshift32_state;
extern uint64_t vm_code_state;
extern uint64_t vm_function_key;
extern uint64_t vm_block_chain_state;
extern uint64_t expected_bb_token;
extern uint8_t exception_thrown;
extern void *exception_ptr;
extern int exception_selector;
extern uint64_t last_br_from_bb_id;
extern uint64_t current_bb_id;

#endif
