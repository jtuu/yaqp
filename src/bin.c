#include <stdint.h>

#include "bin.h"

bin_t* parse_bin(unsigned int data_len, uint8_t *data) {
    bin_t *bin = (bin_t *) data;
    bin->object_code = &(data[bin->object_code_offset]);
    return bin;
}
