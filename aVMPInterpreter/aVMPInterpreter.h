typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned long long uintptr_t;

// Opcode
#define NOP_OP              0x00
#define ALLOCA_OP           0x01
#define LOAD_OP             0x02
#define STORE_OP            0x03
#define BinaryOperator_OP   0x04
#define GEP_OP              0x05
#define CMP_OP              0x06
#define CAST_OP             0x07
#define BR_OP               0x08
#define Call_OP             0x09
#define Ret_OP              0x0A
#define SWITCH_OP           0x0B
#define INSERTVALUE_OP      0x0C
#define EXTRACTVALUE_OP     0x0D
#define PHI_OP              0x0E
#define SELECT_OP           0x0F
#define LANDINGPAD_OP       0x10
#define RESUME_OP           0x11
#define INDIRECTBR_OP       0x12
#define EXTRACTELEMENT_OP   0x13
#define INSERTELEMENT_OP    0x14
#define SHUFFLEVECTOR_OP    0x15
#define FREEZE_OP           0x16

#define OP_TOTAL            0x16

extern unsigned pointer_size;

// BinaryOperator codes
#define BINOP_ADD       13
#define BINOP_FADD      14
#define BINOP_SUB       15
#define BINOP_FSUB      16
#define BINOP_MUL       17
#define BINOP_FMUL      18
#define BINOP_UDIV      19
#define BINOP_SDIV      20
#define BINOP_FDIV      21
#define BINOP_UREM      22
#define BINOP_SREM      23
#define BINOP_FREM      24
#define BINOP_SHL       25
#define BINOP_LSHR      26
#define BINOP_ASHR      27
#define BINOP_AND       28
#define BINOP_OR        29
#define BINOP_XOR       30
#define BINOP_FNEG      31

// Cast opcodes (from LLVM Instruction.def)
#define CAST_TRUNC      0
#define CAST_ZEXT       1
#define CAST_SEXT       2
#define CAST_FPTRUNC    3
#define CAST_FPEXT      4
#define CAST_FPTOUI     5
#define CAST_FPTOSI     6
#define CAST_UITOFP     7
#define CAST_SITOFP     8
#define CAST_PTRTOINT   9
#define CAST_INTTOPTR   10
#define CAST_BITCAST    11
#define CAST_ADDRSPACECAST 12

// ICMP predicates (from LLVM Instruction.def)
#define ICMP_EQ     32
#define ICMP_NE     33
#define ICMP_UGT    34
#define ICMP_UGE    35
#define ICMP_ULT    36
#define ICMP_ULE    37
#define ICMP_SGT    38
#define ICMP_SGE    39
#define ICMP_SLT    40
#define ICMP_SLE    41

// FCMP predicates (from LLVM Instruction.def)
#define FCMP_FALSE  42
#define FCMP_OEQ    43
#define FCMP_OGT    44
#define FCMP_OGE    45
#define FCMP_OLT    46
#define FCMP_OLE    47
#define FCMP_ONE    48
#define FCMP_ORD    49
#define FCMP_UNO    50
#define FCMP_UEQ    51
#define FCMP_UGT    52
#define FCMP_UGE    53
#define FCMP_ULT    54
#define FCMP_ULE    55
#define FCMP_UNE    56
#define FCMP_TRUE   57

// Exception globals
extern uint8_t exception_thrown;
extern void *exception_ptr;
extern int exception_selector;

// PHI tracking
extern uint64_t last_br_from_bb_id;
extern uint64_t current_bb_id;

// functions
uint64_t unpack_code(int size);
uint64_t unpack_data(uint64_t offset, int size);
uint64_t unpack_addr(uint64_t address, int size);
void pack_store_addr(uint64_t address, uint64_t value, int size);
uint64_t get_value_with_size(uint8_t value_size, uint8_t value_type);
uint64_t get_value();
void alloca_handler();
void load_handler();
void store_handler();
void binaryOperator_handler();
void gep_handler();
void cmp_handler();
void cast_handler();
void br_handler();
void return_handler();
void switch_handler();
void phi_handler();
void select_handler();
void landingpad_handler();
void resume_handler();
void indirectbr_handler();
void extractelement_handler();
void insertelement_handler();
void shufflevector_handler();
void freeze_handler();
extern void call_handler(uint64_t targetfunc_id);
void vm_interpreter();
void data_seg_clear();
