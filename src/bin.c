#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bin.h"
#include "utils.h"

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
    free(bin->function_offset_table);
    free(bin);
}
