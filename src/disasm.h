#ifndef YAQP_DISASM_H
#define YAQP_DISASM_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "bin.h"
#include "asm.h"
#include "list.h"

typedef enum {
    PARSE_MODE_CODE,
    PARSE_MODE_DATA,
    PARSE_MODE_RAW,
    PARSE_MODE_STRING
} parse_mode_kind;

typedef enum {
    STACK_MODE_IMMEDIATE,
    STACK_MODE_PUSH,
    STACK_MODE_POP
} stack_mode_kind;

typedef enum {
    LABEL_RAW_DATA = 1 << 0,
    LABEL_RAW_STRING = 1 << 1
} label_flag;

typedef struct parser {
    FILE *out_fd;
    bin_t *bin;
    size_t obj_code_counter;
    parse_mode_kind parse_mode;
    stack_mode_kind stack_mode;
    node_t *stack_head;
    const instruction_t *cur_instr;
    size_t cur_arg;
    label_flag *label_flags;
    bool transform_args_to_immediate;
} parser_t;

typedef struct argument {
    arg_kind kind;
    union {
        uint8_t as_byte;
        uint16_t as_word;
        uint32_t as_dword;
        float as_float;
        struct {
            size_t length;
            uint16_t *data;
        } as_string;
        struct {
            size_t length;
            uint16_t *functions;
        } as_fn_switch;
        struct {
            size_t length;
            uint8_t *registers;
        } as_reg_switch;
    } value;
} argument_t;

int disassemble(FILE *out_fd, bin_t *bin);

#endif
