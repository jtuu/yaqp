#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

void *array_concat(const void *a, size_t an, const void *b, size_t bn, size_t s) {
    char *p = malloc(s * (an + bn));
    memcpy(p, a, an * s);
    memcpy(p + an * s, b, bn * s);
    return p;
}

void print_byte_arr(int arr_len, uint8_t *arr) {
    printf("[%02hhx", arr[0]);
    for (int i = 1; i < arr_len; i++) {
        printf(", %02hhx", arr[i]);
    }
    printf("]\n");
}

void print_wide_str(FILE *file, uint16_t *str) {
    uint16_t *cur = str;
    do {
        fprintf(file, "%lc", *cur);
    } while (*(++cur) != 0);
}

void print_wide_str_json_escaped(FILE *file, uint16_t *str) {
    uint16_t *cur = str;
    do {
        switch (*cur) {
        case '"':
        case '\\':
            fprintf(file, "\\%lc", *cur);
            break;
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xa:
        case 0xb:
        case 0xc:
        case 0xd:
        case 0xe:
        case 0xf:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1a:
        case 0x1b:
        case 0x1c:
        case 0x1d:
        case 0x1e:
        case 0x1f:
            fprintf(file, "\\u%04x", *cur);
            break;
        default:
            fprintf(file, "%lc", *cur);
        }
    } while (*(++cur) != 0);
}

char* stristr(char *str1, char *str2) {
    do {
        char *s1 = str1;
        char *s2 = str2;

        while (tolower((unsigned char) *s1) == tolower((unsigned char) *s2) && *s2) {
            s1++;
            s2++;
        }

        if (s2 == NULL) {
            return (char *) str1;
        }
    } while (*str1++);
    
    return NULL;
}
