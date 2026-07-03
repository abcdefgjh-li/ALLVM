#ifndef AVMP_INTERPRETER_TRACE_H
#define AVMP_INTERPRETER_TRACE_H

#include <stdint.h>

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

void vm_trace_push(uint8_t kind, uint32_t ip_value, uint8_t tag0, uint8_t tag1,
                   uint64_t a, uint64_t b, uint64_t c, uint64_t d);
void vm_dump_fault_context(const char *reason, uint64_t detail0, uint64_t detail1);

#endif
