#ifndef AVMP_INTERPRETER_OPCODE_H
#define AVMP_INTERPRETER_OPCODE_H

#include <stdint.h>

uint8_t get_opcode(void);
uint64_t unpack_code(int size);
uint64_t unpack_data(uint64_t offset, int size);
uint64_t unpack_addr(uint64_t address, int size);
uint64_t get_value_with_size(uint8_t value_size, uint8_t value_type);
uint64_t get_value(void);
void pack_store_addr(uint64_t address, uint64_t value, int size);

#endif
