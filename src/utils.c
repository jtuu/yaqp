#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void *array_concat(const void *a, size_t an, const void *b, size_t bn, size_t s) {
    char *p = malloc(s * (an + bn));
    memcpy(p, a, an * s);
    memcpy(p + an * s, b, bn * s);
    return p;
}

void print_byte_arr(int arr_len, uint8_t *arr) {
    printf("[%d", arr[0]);
    for (int i = 1; i < arr_len; i++) {
        printf(", %d", arr[i]);
    }
    printf("]\n");
}

void print_wide_str(uint16_t *str) {
    uint16_t *cur = str;
    do {
        printf("%lc", *cur);
    } while(*(cur++) != 0);
    printf("\n");
}
