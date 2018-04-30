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

void print_wide_str(FILE *file, uint16_t *str) {
    uint16_t *cur = str;
    do {
        fprintf(file, "%lc", *cur);
    } while(*(++cur) != 0);
}

char* stristr(const char *str1, const char *str2) {
    do {
        const char *s1 = str1;
        const char *s2 = str2;

        while (tolower((unsigned char) *s1) == tolower((unsigned char) *s2) && *s2) {
            s1++;
            s2++;
        }

        if (*s2 == NULL) {
            return (char *) str1;
        }
    } while (*str1++);
    
    return NULL;
}
