#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bin.h"
#include "utils.h"
#include "asm.h"

bin_t* parse_bin(size_t data_len, uint8_t *data) {
    UNUSED(data_len);
    
    unsigned char *cursor = data;
    
    bin_t *bin = malloc(sizeof(bin_t));
    memcpy(bin, cursor, sizeof(bin_t));
    cursor += bin->object_code_offset;

    bin->object_code_len = bin->function_offset_table_offset - bin->object_code_offset;
    bin->object_code = malloc(sizeof(uint8_t) * bin->object_code_len);
    memcpy(bin->object_code, cursor, sizeof(uint8_t) * bin->object_code_len);

    // probably not the correct length but good enough for now?
    bin->function_offset_table_len = (bin->bin_size - bin->function_offset_table_offset) / sizeof(int32_t);
    cursor = data + bin->function_offset_table_offset;
    bin->function_offset_table = malloc(sizeof(int32_t) * bin->function_offset_table_len);
    memcpy(bin->function_offset_table, cursor, sizeof(int32_t) * bin->function_offset_table_len);
    
    return bin;
}

void dispose_bin(bin_t *bin) {
    free(bin->object_code);
    free(bin);
}

void print_pasm(bin_t *bin) {
    bool data_mode = false;
    bool push_mode = false;
    bool pop_mode = false;
    uint8_t stack[64] = {0};
    size_t stack_ptr = 0;
    instruction_t *cur_instr;
    unsigned int cur_arg = 0;
    for (int i = 0; i < (int) bin->object_code_len; i++) {
        if (data_mode) {
            arg_kind arg = cur_instr->args[cur_arg];
            if (arg == T_NONE || cur_arg >= INSTRUCTION_MAX_ARITY) {
                data_mode = false;
                push_mode = false;
                pop_mode = false;
                printf("\n");
            } else if (arg == T_ARGS) {
                pop_mode = true;
            } else if (arg == T_PUSH) {
                push_mode = true;
            } else {
                size_t arg_sz = arg_sizes[arg];
                i += (int) arg_sz;
                switch (arg_sz) {
                case 1:
                    printf(" %x", bin->object_code[i]);
                    break;
                case 2:
                    printf(" %02x", (uint16_t) (BE16(&bin->object_code[i])));
                    break;
                case 4:
                    printf(" %04x", BE32(&bin->object_code[i]));
                    break;
                default:
                    break;
                }
                cur_arg++;
            }
        } else {
            uint16_t opcode;
            switch (bin->object_code[i]) {
            case 0xf8:
            case 0xf9:
                opcode = (uint16_t) (BE16(&bin->object_code[i]));
                break;
            default:
                opcode = (uint16_t) bin->object_code[i];
                break;
            }
            for (unsigned j = 0; j < bin->function_offset_table_len; j++) {
                if (bin->function_offset_table[j] == i) {
                    printf("F%d:\n", j);
                    break;
                }
            }
            for (unsigned int j = 0; j < NUM_INSTRUCTIONS; j++) {
                instruction_t instr = instructions[j];
                if (instr.opcode == opcode) {
                    cur_instr = &instr;
                    cur_arg = 0;
                    data_mode = true;
                    break;
                }
            }
            printf("    %s", cur_instr->name);
        }
    }
}
