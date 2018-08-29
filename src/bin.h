#ifndef YAQP_BIN_H
#define YAQP_BIN_H

#include <stdint.h>
#include <stdlib.h>

typedef struct bin {
    uint32_t object_code_offset;
    uint32_t function_offset_table_offset;
    uint32_t bin_size;
    uint32_t xffffffff;
    uint32_t quest_number;
    uint32_t language;
    uint16_t quest_name[32];
    uint16_t short_description[128];
    uint16_t long_description[288];
    uint32_t padding;
    uint32_t items_list[932];
    uint8_t *object_code;
} bin_t;

bin_t* parse_bin(size_t data_len, uint8_t *data);

void dispose_bin(bin_t *bin);

#endif
