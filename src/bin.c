#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "bin.h"
#include "utils.h"

bin_t* parse_bin(size_t data_len, uint8_t *data) {
    UNUSED(data_len);
    
    unsigned char *cursor = data;
    
    bin_t *bin = malloc(sizeof(bin_t));
    memcpy(bin, cursor, sizeof(bin_t));
    cursor += sizeof(bin_t);

    uint32_t obj_code_len = bin->function_offset_table_offset - bin->object_code_offset;
    bin->object_code = malloc(sizeof(uint8_t) * obj_code_len);
    memcpy(bin->object_code, cursor, sizeof(uint8_t) * obj_code_len);
    return bin;
}

void dispose_bin(bin_t *bin) {
    free(bin->object_code);
    free(bin);
}
