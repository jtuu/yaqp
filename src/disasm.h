#ifndef YAQP_DISASM_H
#define YAQP_DISASM_H

#include <stdio.h>
#include <stdint.h>

#include "bin.h"
#include "asm.h"

typedef enum {
    PARSE_MODE_CODE,
    PARSE_MODE_DATA,
    PARSE_MODE_RAW
} parse_mode_kind;

typedef enum {
    STACK_MODE_IMMEDIATE,
    STACK_MODE_PUSH,
    STACK_MODE_POP
} stack_mode_kind;

typedef enum {
    LABEL_RAW_DATA = 1 << 0
} label_flag;

// idk
#define STACK_SIZE 1024

typedef struct parser {
    FILE *out_fd;
    bin_t *bin;
    size_t obj_code_counter;
    parse_mode_kind parse_mode;
    stack_mode_kind stack_mode;
    uint8_t stack[STACK_SIZE];
    size_t stack_ptr;
    const instruction_t *cur_instr;
    size_t cur_arg;
    label_flag *label_flags;
} parser_t;

void disassemble(FILE *out_fd, bin_t *bin);

#endif
