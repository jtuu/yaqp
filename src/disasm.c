#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "bin.h"
#include "utils.h"
#include "disasm.h"
#include "list.h"

void dispose_arg(argument_t *arg) {
    switch (arg->kind) {
    case T_STR:
        free(arg->value.as_string.data);
        break;
    case T_SWITCH:
    case T_SWITCH2B:
        free(arg->value.as_switch.functions);
        break;
    default:
        break;
    }
}

void dispose_stack(parser_t *parser) {
    node_t *cursor = parser->stack_head;
    node_t *tmp;

    while (cursor != NULL) {
        argument_t *arg = (argument_t *) cursor->data;
        dispose_arg(arg);

        tmp = cursor->next;
        free(cursor->data);
        free(cursor);
        cursor = tmp;
    }
    
    parser->stack_head = NULL;
}

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

void begin_string_mode(parser_t *parser) {
    parser->parse_mode = PARSE_MODE_STRING;
}

float parse_float(uint8_t *bytes) {
    return *(float *) bytes;
}

uint16_t parse_uint16(uint8_t *bytes) {
    return LE16(bytes);
}

uint32_t parse_uint32(uint8_t *bytes) {
    return LE32(bytes);
}

bool arg_kind_is_flag(arg_kind arg) {
    switch (arg) {
    case T_NONE:
    case T_ARGS:
    case T_IMED:
    case T_VASTART:
    case T_VAEND:
    case T_V2:
    case T_V3:
    case T_V4:
    case T_DC:
        return true;
    default:
        return false;
    }
}

int get_arg_size(argument_t *arg) {
    int arg_sz = (int)  arg_sizes[arg->kind];

    if (arg_sz == VARIABLE_SIZED) {
        switch (arg->kind) {
        case T_SWITCH:
        case T_SWITCH2B:
            arg_sz = (int) (arg->value.as_switch.length * sizeof(uint16_t) + 1);
            break;
        case T_STR:
            arg_sz = (int) (arg->value.as_string.length * sizeof(uint16_t));
            break;
        default:
            fprintf(stderr, "Unhandled variable sized argument: %d\n", arg->kind);
            arg_sz = -1;
            break;
        }
    }

    return arg_sz;
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

void print_str(FILE *fd, uint16_t *str_data) {
    uint16_t *cur = str_data;
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

int print_arg(FILE *fd, argument_t *arg) {    
    switch (arg->kind) {
    case T_BYTE:
        fprintf(fd, " %02x", arg->value.as_byte);
        break;
    case T_REG:
    case T_BREG:
        fprintf(fd, " R%d", arg->value.as_byte);
        break;
    case T_WORD:
    case T_PFLAG:
        fprintf(fd, " %04x", arg->value.as_word);
        break;
    case T_DATA:
    case T_STRDATA:
    case T_FUNC:
    case T_FUNC2:
        fprintf(fd, " F%d", arg->value.as_word);
        break;
    case T_DWORD:
        fprintf(fd, " %08x", arg->value.as_dword);
        break;
    case T_FLOAT:
        fprintf(fd, " %f", arg->value.as_float);
        break;
    case T_STR:
        print_str(fd, arg->value.as_string.data);
        break;
    case T_SWITCH:
    case T_SWITCH2B:
        fprintf(fd, " %ld", arg->value.as_switch.length);
        for (size_t i = 0; i < arg->value.as_switch.length; i++) {
            fprintf(fd, ":F%d", arg->value.as_switch.functions[i]);
        }
        break;
    default:
        fprintf(stderr, "Unhandled argument kind: %d\n", arg->kind);
        return -1;
    }

    return 0;
}

int finalize_arg(parser_t *parser, argument_t *arg) {
    int ret = 0;

    switch (parser->cur_instr->args[parser->cur_arg]) {
    case T_DATA:
        parser->label_flags[arg->value.as_word] |= LABEL_RAW_DATA;
        break;
    case T_STRDATA:
        parser->label_flags[arg->value.as_word] |= LABEL_RAW_STRING;
        break;
    default:
        break;
    }

    if (should_print(parser)) {
        ret = print_arg(parser->out_fd, arg);
    }

    return ret;
}

int stack_push(parser_t *parser, argument_t *arg) {
    int key = parser->stack_head == NULL ? 0 : parser->stack_head->key + 1;
    // keep in mind that append_node does a memcpy on the data
    // so the argument object itself can be freed but any other pointers inside should not...
    // kinda bad
    parser->stack_head = append_node(parser->stack_head, key, sizeof(argument_t), arg);

    finalize_arg(parser, arg);

    return 0;
}

// doesn't actually pop in the traditional sense because we need to
// take the args in the insertion order
int stack_pop(parser_t *parser) {
    argument_t *arg = (argument_t *) parser->stack_head->data;
    int ret = 0;

    ret = finalize_arg(parser, arg);

    // move to next arg
    if (parser->stack_head->next != NULL) {
        parser->stack_head = parser->stack_head->next;
    }

    return ret;
}

int process_arg(parser_t *parser) {
    int ret = 0;

    argument_t arg = {
        .kind = parser->cur_instr->args[parser->cur_arg]
    };
    
    // in pop mode we take arguments from the stack and not from the object code
    // so no need to move forward in the object code or init the argument object
    if (parser->stack_mode != STACK_MODE_POP) {
        uint8_t *obj_code_cursor = &parser->bin->object_code[parser->obj_code_counter];

        // init argument object
        switch (arg.kind) {
        case T_BYTE:
        case T_REG:
        case T_BREG:
            arg.value.as_byte = *obj_code_cursor;
            break;
        case T_WORD:
        case T_DATA:
        case T_STRDATA:
        case T_PFLAG:
        case T_FUNC:
        case T_FUNC2:
            arg.value.as_word = parse_uint16(obj_code_cursor);
            break;
        case T_DWORD:
            arg.value.as_dword = parse_uint32(obj_code_cursor);
            break;
        case T_FLOAT:
            arg.value.as_float = parse_float(obj_code_cursor);
            break;
        case T_STR:
            {
                size_t len = str_len(obj_code_cursor);
                size_t sz = len * sizeof(uint16_t);
                arg.value.as_string.length = len;
                arg.value.as_string.data = malloc(sz);
                memcpy(arg.value.as_string.data, obj_code_cursor, sz);
            }
            break;
        case T_SWITCH:
        case T_SWITCH2B:
            {
                size_t len = *obj_code_cursor;
                obj_code_cursor++;
                size_t sz = len * sizeof(uint16_t);
                arg.value.as_switch.length = len;
                arg.value.as_switch.functions = malloc(sz);
                for (size_t i = 0; i < len; i++, obj_code_cursor += sizeof(uint16_t)) {
                    arg.value.as_switch.functions[i] = parse_uint16(obj_code_cursor);
                }
            }
            break;
        default:
            fprintf(stderr, "Unhandled argument kind: %d\n", arg.kind);
            return -1;
        }

        // move forward in the object code
        int arg_sz = get_arg_size(&arg);
        if (arg_sz < 0) {
            ret = arg_sz;
        } else {
            parser->obj_code_counter += (size_t) arg_sz;
        }
    }
    
    // qedit style hidden arg_push* opcodes
    if (parser->transform_args_to_immediate) {
        switch (parser->stack_mode) {
        case STACK_MODE_POP:
            ret = stack_pop(parser);
            break;
        case STACK_MODE_PUSH:
            ret = stack_push(parser, &arg);
            break;
        default:
            ret = finalize_arg(parser, &arg);
            break;
        }
    } else {
        ret = finalize_arg(parser, &arg);
    }

    // don't dispose because we just pushed it to the stack and it will be used later
    if (parser->stack_mode != STACK_MODE_PUSH) {
        dispose_arg(&arg);
    }

    return ret;
}

int rewind_stack(parser_t *parser) {
    node_t *head = parser->stack_head;
    bool skipped_first = false;

    for (size_t i = INSTRUCTION_MAX_ARITY; i--;) {
        arg_kind arg = parser->cur_instr->args[i];
        
        if (!arg_kind_is_flag(arg)) {
            if (skipped_first) {
                if (head->prev == NULL) {
                    fprintf(stderr, "Stack underflow\n");
                    return -1;
                }
                
                head = head->prev;
            } else {
                skipped_first = true;
            }
        }
    }

    parser->stack_head = head;

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
            dispose_stack(parser);
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
        } else if (parser->label_flags[label] & LABEL_RAW_STRING) {
            begin_string_mode(parser);
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

int parse_raw_string(parser_t *parser) {
    uint8_t *obj_code_cursor = &parser->bin->object_code[parser->obj_code_counter];
    fprintf(parser->out_fd, "    STR:");
    print_str(parser->out_fd, (uint16_t *) obj_code_cursor);
    fprintf(parser->out_fd, "\n");

    parser->obj_code_counter += str_len(obj_code_cursor) * sizeof(uint16_t);

    begin_code_mode(parser);

    return 0;
}

int parser_loop(parser_t *parser) {
    int ret = 0;
    while (parser->obj_code_counter < parser->bin->object_code_len) {
        switch (parser->parse_mode) {
        case PARSE_MODE_CODE:
            ret = parse_code(parser);
            break;
        case PARSE_MODE_DATA:
            ret = parse_data(parser);
            break;
        case PARSE_MODE_RAW:
            ret = parse_raw(parser);
            break;
        case PARSE_MODE_STRING:
            ret = parse_raw_string(parser);
            break;
        default:
            break;
        }
        if (ret < 0) {
            fprintf(stderr, "Parsing failed\n");
            break;
        }
    }
    return ret;
}

int disassemble(FILE *out_fd, bin_t *bin) {
    parser_t parser = {0};
    parser.bin = bin;
    parser.label_flags = calloc(bin->function_offset_table_len, sizeof(label_flag));
    parser.transform_args_to_immediate = true;

    // do a dry run to figure out label flags
    parser.out_fd = fopen("/dev/null", "w");
    begin_code_mode(&parser);
    int dry_ret = parser_loop(&parser);
    dispose_stack(&parser);
    fclose(parser.out_fd);

    if (dry_ret < 0) {
        fprintf(stderr, "Disassembler dry run failed\n");
        return dry_ret;
    }

    // real run
    parser.out_fd = out_fd;
    parser.obj_code_counter = 0;
    begin_code_mode(&parser);
    int ret = parser_loop(&parser);
    dispose_stack(&parser);
    free(parser.label_flags);

    return ret;
}
