#ifndef YAQP_UTILS_H
#define YAQP_UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define LE16(x) *(x) | (uint16_t) *(x + 1) << 8
#define LE32(x) *(x) | (uint32_t) *(x + 1) << 8 | (uint32_t) *(x + 2) << 16 | (uint32_t) *(x + 3) << 24
#define BE16(x) *(x + 1) | (uint16_t) *(x) << 8
#define BE32(x) (uint16_t) (*(x + 3) | (uint16_t) *(x + 2) << 8) | (uint32_t) *(x + 1) << 16 | (uint32_t) *(x) << 24

#define SWAP_BYTES(x) (x & 0xff) << 8 | (x & 0xff00) >> 8

void *array_concat(const void *a, size_t an, const void *b, size_t bn, size_t s);

#define ARRAY_CONCAT(TYPE, A, An, B, Bn) (TYPE *)array_concat((const void *)(A), (An), (const void *)(B), (Bn), sizeof(TYPE));

void print_byte_arr(int arr_len, uint8_t *arr);

void print_wide_str(FILE *file, uint16_t *str);
void print_wide_str_json_escaped(FILE *file, uint16_t *str);

char* stristr(const char *str1, const char *str2);

char *change_file_ext(char *file_path, const char *new_ext);

#define UNUSED(x) (void)(x)

#endif
