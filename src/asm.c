#include <stdint.h>

#include "asm.h"

const size_t arg_sizes[NUM_ARG_KINDS] = {
#include "arg_sizes.txt"
};

const instruction_t instructions[NUM_INSTRUCTIONS] = {
#include "instructions.txt"
};
