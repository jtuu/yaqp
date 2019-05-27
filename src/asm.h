#ifndef YAQP_ASM_H
#define YAQP_ASM_H

#include <stdlib.h>
#include <stdint.h>

#define INSTRUCTION_MAX_ARITY 11

typedef enum {
    T_NONE,
    T_ARGS,
    T_DWORD,
    T_WORD,
    T_BYTE,
    T_FLOAT,
    T_REG,
    T_BREG,
    T_DREG,
    T_IMED,
    T_FUNC,
    T_FUNC2,
    T_PUSH,
    T_VASTART,
    T_VAEND,
    T_V2,
    T_V3,
    T_V4,
    T_DC,
    T_STR,
    T_PFLAG,
    T_SWITCH,
    T_SWITCH2B,
    T_DATA,
    T_STRDATA
} arg_kind;

#define VARIABLE_SIZED 0xb00b5

#define NUM_ARG_KINDS 25
const size_t arg_sizes[NUM_ARG_KINDS];

typedef struct instruction {
    const uint16_t opcode;
    const char *name;
    const arg_kind args[INSTRUCTION_MAX_ARITY];
} instruction_t;

#define NUM_INSTRUCTIONS 505
const instruction_t instructions[NUM_INSTRUCTIONS];

#endif
