#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bin.h"
#include "utils.h"
#include "disasm.h"

int find_label(parser_t *parser) {
    // dumb
    for (unsigned j = 0; j < parser->bin->function_offset_table_len; j++) {
        int32_t offset = parser->bin->function_offset_table[j];
        if (offset != -1 && (size_t) offset == parser->obj_code_counter) {
            return (int) j;
        }
    }
    return -1;
}

bool find_instruction(parser_t *parser) {
    // opcode can be one or two bytes long
    // it is two bytes if the first byte is f8 or f9, otherwise one byte
    uint16_t opcode;
    switch (parser->bin->object_code[parser->obj_code_counter]) {
    case 0xf8:
    case 0xf9:
        opcode = (uint16_t) (BE16(&parser->bin->object_code[parser->obj_code_counter]));
        parser->obj_code_counter += 2;
        break;
    default:
        opcode = (uint16_t) parser->bin->object_code[parser->obj_code_counter];
        parser->obj_code_counter += 1;
        break;
    }
    // find instruction data from global table
    for (unsigned int j = 0; j < NUM_INSTRUCTIONS; j++) {
        if (instructions[j].opcode == opcode) {
            parser->cur_instr = &instructions[j];
            return true;
        }
    }
    return false;
}

void begin_code_mode(parser_t *parser) {
    parser->parse_mode = PARSE_MODE_CODE;
}

void begin_data_mode(parser_t *parser) {
    parser->cur_arg = 0;
    parser->parse_mode = PARSE_MODE_DATA;
    parser->stack_mode = STACK_MODE_IMMEDIATE;
}

void begin_raw_mode(parser_t *parser) {
    parser->parse_mode = PARSE_MODE_RAW;
}

float parse_float(uint8_t *bytes) {
    const size_t float_sz = 4;
    uint8_t cp[float_sz];
    for (size_t i = 0; i < float_sz; i++) {
        cp[float_sz - i - 1] = *(bytes + i);
    }
    return *(float *) (&cp);
}

uint16_t parse_uint16(uint8_t *bytes) {
    return LE16(bytes);
}

uint32_t parse_uint32(uint8_t *bytes) {
    return LE32(bytes);
}

bool should_print(parser_t *parser) {
    if (parser->transform_args_to_immediate) {
        for (size_t i = 0; i < INSTRUCTION_MAX_ARITY; i++) {
            if (parser->cur_instr->args[i] == T_PUSH) {
                return false;
            }
        }
    }
    return true;
}

size_t str_len(uint8_t *bytes) {
    size_t len = 0;
    while (*bytes || *(bytes + 1)) {
        bytes += sizeof(uint16_t);
        len++;
    }
    return len + 1;
}

void print_str(FILE *fd, uint8_t *bytes) {
    uint16_t *cur = (uint16_t *) bytes;
    fprintf(fd, " \"");
    do {
        switch (*cur) {
        case 0x0a:
            fprintf(fd, "\\n");
            break;
        default:
            fprintf(fd, "%lc", *cur);
            break;
        }
    } while (*(++cur) != 0);
    fprintf(fd, "\"");
}

int print_arg(FILE *fd, arg_kind arg, uint8_t *bytes) {
    switch (arg) {
    case T_BYTE:
        fprintf(fd, " %02x", *bytes);
        break;
    case T_REG:
    case T_BREG:
        fprintf(fd, " R%d", *bytes);
        break;
    case T_WORD:
    case T_DATA:
    case T_PFLAG:
        fprintf(fd, " %04x", parse_uint16(bytes));
        break;
    case T_FUNC:
    case T_FUNC2:
        fprintf(fd, " F%d", parse_uint16(bytes));
        break;
    case T_DWORD:
        fprintf(fd, " %08x", parse_uint32(bytes));
        break;
    case T_FLOAT:
        fprintf(fd, " %f", parse_float(bytes));
        break;
    case T_STR:
        print_str(fd, bytes);
        break;
    case T_SWITCH:
    case T_SWITCH2B:
        {
            uint8_t *cursor = bytes;
            uint8_t len = *cursor;
            size_t stride = sizeof(uint16_t);
            cursor++;
            uint8_t *end = cursor + len * stride;
            fprintf(fd, " %d", len);
            for (; cursor != end ; cursor += stride) {
                fprintf(fd, ":F%d", parse_uint16(cursor));
            }
        }
        break;
    default:
        fprintf(stderr, "Unhandled argument kind: %d\n", arg);
        return -1;
    }

    return 0;
}

int finalize_arg(parser_t *parser, arg_kind arg, uint8_t *arg_data) {
    int ret = 0;

    if (arg == T_DATA) {
        parser->label_flags[parse_uint16(arg_data)] |= LABEL_RAW_DATA;
    }

    if (should_print(parser)) {
        ret = print_arg(parser->out_fd, arg, arg_data);
    }

    return ret;
}

int stack_push(parser_t *parser) {
    arg_kind arg = parser->cur_instr->args[parser->cur_arg];
    size_t arg_sz = arg_sizes[arg];

    if (arg_sz == VARIABLE_SIZED) {
        switch (arg) {
        case T_STR:
            arg_sz = str_len(&parser->bin->object_code[parser->obj_code_counter]) * sizeof(uint16_t);
            break;
        default:
            fprintf(stderr, "Unhandled push of variable sized argument: %d\n", arg);
            return -1;
        }
    }

    // check if there's enough space
    if (parser->stack_ptr + arg_sz > STACK_SIZE) {
        fprintf(stderr, "Stack overflow\n");
        return -1;
    }

    // push data from object code to stack
    for (size_t i = parser->obj_code_counter; i < parser->obj_code_counter + arg_sz; i++) {
        parser->stack[parser->stack_ptr++] = parser->bin->object_code[i];
    }

    if (arg == T_STR) {
        // write string size to the end
        parser->stack[parser->stack_ptr + 0] = (arg_sz >> 24) & 0xff;
        parser->stack[parser->stack_ptr + 1] = (arg_sz >> 16) & 0xff;
        parser->stack[parser->stack_ptr + 2] = (arg_sz >> 8) & 0xff;
        parser->stack[parser->stack_ptr + 3] = arg_sz & 0xff;
        parser->stack_ptr += 4;
    }

    finalize_arg(parser, arg, &parser->bin->object_code[parser->obj_code_counter]);

    return 0;
}

// doesn't actually pop in the traditional sense because we need to
// take the args in the insertion order
int stack_pop(parser_t *parser) {
    arg_kind arg = parser->cur_instr->args[parser->cur_arg];
    size_t arg_sz = arg_sizes[arg];
    int ret = 0;

    if (arg_sz == VARIABLE_SIZED) {
        switch (arg) {
        case T_STR:
            arg_sz = str_len(&parser->stack[parser->stack_ptr]) * sizeof(uint16_t) + 4;
            break;
        default:
            fprintf(stderr, "Unhandled variable sized argument: %d\n", arg);
            break;
        }
    }

    ret = finalize_arg(parser, arg, &parser->stack[parser->stack_ptr]);

    // move to next arg
    parser->stack_ptr += arg_sz;
    return ret;
}

int process_arg(parser_t *parser) {
    arg_kind arg = parser->cur_instr->args[parser->cur_arg];
    size_t arg_sz = arg_sizes[arg];
    int ret = 0;
    
    if (parser->transform_args_to_immediate) {
        switch (parser->stack_mode) {
        case STACK_MODE_POP:
            ret = stack_pop(parser);
            break;
        case STACK_MODE_PUSH:
            ret = stack_push(parser);
            break;
        default:
            ret = finalize_arg(parser, arg, &parser->bin->object_code[parser->obj_code_counter]);
            break;
        }
    } else {
        ret = finalize_arg(parser, arg, &parser->bin->object_code[parser->obj_code_counter]);
    }

    if (parser->stack_mode != STACK_MODE_POP) {
        // move forward
        if (arg_sz == VARIABLE_SIZED) {
            switch (arg) {
            case T_SWITCH:
            case T_SWITCH2B:
                // the size of a switch is determined by the first byte
                parser->obj_code_counter += parser->bin->object_code[parser->obj_code_counter] * sizeof(uint16_t) + 1;
                break;
            case T_STR:
                if (parser->transform_args_to_immediate) {
                    // we encoded the length at the end of a string
                    parser->obj_code_counter += BE32(&parser->stack[parser->stack_ptr - 4]);
                } else {
                    parser->obj_code_counter += str_len(&parser->bin->object_code[parser->obj_code_counter]) * sizeof(uint16_t);
                }
                break;
            default:
                fprintf(stderr, "Unhandled variable sized argument: %d\n", arg);
                ret = -1;
                break;
            }
        } else {
            parser->obj_code_counter += arg_sz;
        }
    }

    return ret;
}

int rewind_stack(parser_t *parser) {
    size_t args_sz_sum = 0;

    for (size_t i = INSTRUCTION_MAX_ARITY; i--;) {
        arg_kind arg = parser->cur_instr->args[i];
        size_t arg_sz = arg_sizes[arg];

        if (arg_sz == VARIABLE_SIZED) {
            switch (arg) {
            case T_STR:
                // read the size at the end (move -4 bytes)
                // and add the additional 4 bytes to the sum
                args_sz_sum += (BE32(&parser->stack[parser->stack_ptr - args_sz_sum - 4])) + 4;
                break;
            default:
                fprintf(stderr, "Unhandled variable sized argument: %d\n", arg);
                break;
            }
        } else {
            args_sz_sum += arg_sz;
        }
    }

    if (parser->stack_ptr - args_sz_sum > parser->stack_ptr) {
        fprintf(stderr, "Stack underflow\n");
        return -1;
    }

    parser->stack_ptr -= args_sz_sum;

    return 0;
}

void end_data(parser_t *parser) {
    if (should_print(parser)) {
        fprintf(parser->out_fd, "\n");
    }
    begin_code_mode(parser);
}

int parse_data(parser_t *parser) {
    arg_kind arg = parser->cur_instr->args[parser->cur_arg];
    int ret = 0;

    switch (arg) {
    case T_NONE:
        if (parser->stack_mode == STACK_MODE_POP && parser->transform_args_to_immediate) {
            // need to move stack pointer back to the real head after popping
            ret = rewind_stack(parser);
        }
        end_data(parser);
        break;
    case T_ARGS:
        parser->stack_mode = STACK_MODE_POP;
        if (parser->transform_args_to_immediate) {
            // we need to take the args in the insertion order
            // so let's move the stack pointer all the way back
            // to where the arguments for this function start
            ret = rewind_stack(parser);
        } else {
            end_data(parser);
        }
        break;
    case T_PUSH:
        parser->stack_mode = STACK_MODE_PUSH;
        break;
    case T_IMED:
        parser->stack_mode = STACK_MODE_IMMEDIATE;
        break;
    case T_VASTART:
        parser->transform_args_to_immediate = false;
        break;
    case T_VAEND:
        parser->transform_args_to_immediate = true;
        break;
    default:
        ret = process_arg(parser);
        break;
    }

    // move forward
    parser->cur_arg++;

    return ret;
}

int parse_code(parser_t *parser) {
    int ret = 0;
    int label = find_label(parser);

    if (label != -1) {
        fprintf(parser->out_fd, "F%d:\n", label);

        if (parser->label_flags[label] & LABEL_RAW_DATA) {
            begin_raw_mode(parser);
            return ret;
        }
    }
    
    if (find_instruction(parser)) {
        if (should_print(parser)) {
            fprintf(parser->out_fd, "    %s", parser->cur_instr->name);
        }
        begin_data_mode(parser);
    } else {
        fprintf(stderr, "Failed to find next instruction, switching to raw output\n");
        begin_raw_mode(parser);
    }
    return ret;
}

int parse_raw(parser_t *parser) {
    int ret = 0;

    unsigned int num_cols = 8;
    unsigned int i = 0;
    fprintf(parser->out_fd, "    HEX:");
    do {
        if (i != 0 && i % num_cols == 0) {
            fprintf(parser->out_fd, "\n    HEX:");
        }
        fprintf(parser->out_fd, " %02x", parser->bin->object_code[parser->obj_code_counter]);
        i++;
        parser->obj_code_counter++;
    } while (find_label(parser) == -1 && parser->obj_code_counter < parser->bin->object_code_len);
    fprintf(parser->out_fd, "\n");

    begin_code_mode(parser);

    return ret;
}

void disassemble(FILE *out_fd, bin_t *bin) {
    parser_t parser = {0};
    parser.out_fd = out_fd;
    parser.bin = bin;
    parser.label_flags = calloc(bin->function_offset_table_len, sizeof(label_flag));
    parser.transform_args_to_immediate = true;

    begin_code_mode(&parser);
    
    while (parser.obj_code_counter < bin->object_code_len) {
        int ret = 0;
        switch (parser.parse_mode) {
        case PARSE_MODE_CODE:
            ret = parse_code(&parser);
            break;
        case PARSE_MODE_DATA:
            ret = parse_data(&parser);
            break;
        case PARSE_MODE_RAW:
            ret = parse_raw(&parser);
            break;
        default:
            break;
        }
        if (ret < 0) {
            fprintf(stderr, "Parsing failed\n");
            break;
        }
    }

    free(parser.label_flags);
}
