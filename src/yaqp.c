/*
yaqp - Yet Another QST Parser
Copyright (C) 2018  Joona Tuukkanen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*
Contains parts of the Sylverant project.
(https://sourceforge.net/p/sylverant/)
(libpsoarchive, quest_enemies)
*/

/*
The Phantasy Star Online Developer's Wiki was used as reference.
(http://sharnoth.com/psodevwiki/format/qst, http://sharnoth.com/psodevwiki/format/dat)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../lib/include/PRS-decomp.h"
#include "../lib/include/psoarchive-error.h"

#define NUM_QST_HEADER 2
#define QST_HEADER_SZ 88
#define QST_HEADER_UNKOWN2_SZ 38
#define QST_FILE_NAME_SZ 16
#define QST_MESSAGE_CHUNK_SZ 1024
#define QST_MESSAGE_SZ 24 + QST_MESSAGE_CHUNK_SZ + 8
#define QST_MESSAGE_UNKNOWN1_SZ 4
#define QST_MESSAGE_UNKNOWN2_SZ 3
#define QST_MESSAGE_UNKNOWN3_SZ 6

#define BIN 0
#define DAT 1

#define LE16(x) *(x) | (uint16_t) *(x + 1) << 8
#define LE32(x) *(x) | (uint32_t) *(x + 1) << 8 | (uint32_t) *(x + 2) << 16 | (uint32_t) *(x + 3) << 24

#define ARRAY_CONCAT(TYPE, A, An, B, Bn) (TYPE *)array_concat((const void *)(A), (An), (const void *)(B), (Bn), sizeof(TYPE));

void *array_concat(const void *a, size_t an, const void *b, size_t bn, size_t s) {
    char *p = malloc(s * (an + bn));
    memcpy(p, a, an * s);
    memcpy(p + an * s, b, bn * s);
    return p;
}

typedef struct qst_header {
    uint16_t header_size; // 88
    uint16_t unknown1; // 0x44
    uint16_t quest_no;
    uint8_t unknown2[QST_HEADER_UNKOWN2_SZ]; // Seems to always be zeroes
    uint8_t file_name[QST_FILE_NAME_SZ]; // Null-terminated ASCII
    uint32_t file_size;
    uint8_t file_name_j[24]; // Null-terminated ASCII
} qst_header_t;

typedef struct qst_message {
    uint8_t unknown1[QST_MESSAGE_UNKNOWN1_SZ];
    uint8_t file_chunk_no; // Starts at 0, each file's chunks are numbered separately
    uint8_t unknown2[QST_MESSAGE_UNKNOWN2_SZ];
    uint8_t file_name[QST_FILE_NAME_SZ]; // Null-terminated ASCII
    uint8_t *file_chunk;
    uint16_t size;
    uint8_t unknown3[QST_MESSAGE_UNKNOWN3_SZ];
} qst_message_t;

typedef struct qst_body {
    unsigned int num_msgs;
    qst_message_t **messages;
} qst_body_t;

typedef struct qst {
    qst_header_t *headers[NUM_QST_HEADER]; // 2 or possibly more?
    qst_body_t body; // Variably sized
} qst_t;

qst_t* parse_qst(unsigned int data_len, unsigned char *data) {
    unsigned char *cursor = data;
    qst_t *qst = malloc(sizeof(qst_t));

    for (int i = 0; i < NUM_QST_HEADER; i++) {
        qst->headers[i] = (qst_header_t *) cursor;
        cursor += QST_HEADER_SZ;
    }

    unsigned int num_msgs = 0;
    qst_message_t **msgs = (qst_message_t **) malloc(num_msgs * sizeof(qst_message_t *));

    while (cursor - data < data_len) {
        num_msgs++;
        msgs = (qst_message_t **) realloc(msgs, num_msgs * sizeof(qst_message_t *));

        qst_message_t *msg = malloc(sizeof(qst_message_t));

        memcpy(msg->unknown1, cursor, QST_MESSAGE_UNKNOWN1_SZ);
        cursor += QST_MESSAGE_UNKNOWN1_SZ * sizeof(msg->unknown1[0]);

        msg->file_chunk_no = *cursor;
        cursor += sizeof(msg->file_chunk_no);

        memcpy(msg->unknown2, cursor, QST_MESSAGE_UNKNOWN2_SZ);
        cursor += QST_MESSAGE_UNKNOWN2_SZ * sizeof(msg->unknown2[0]);

        memcpy(msg->file_name, cursor, QST_FILE_NAME_SZ);
        cursor += QST_FILE_NAME_SZ * sizeof(msg->file_name[0]);

        uint16_t chunk_size = LE16(cursor + QST_MESSAGE_CHUNK_SZ);
        msg->size = chunk_size;
        
        msg->file_chunk = (uint8_t *) malloc(chunk_size * sizeof(uint8_t));
        memcpy(msg->file_chunk, cursor, chunk_size);

        cursor += QST_MESSAGE_CHUNK_SZ;
        cursor += sizeof(msg->size);

        memcpy(msg->unknown3, cursor, QST_MESSAGE_UNKNOWN3_SZ);
        cursor += QST_MESSAGE_UNKNOWN3_SZ * sizeof(msg->unknown3[0]);

        msgs[num_msgs - 1] = msg;
    }

    qst_body_t body = {.num_msgs = num_msgs, .messages = msgs};
    qst->body = body;

    return qst;
}

void print_qst(qst_t *qst) {
    for (int i = 0; i < NUM_QST_HEADER; i++) {
        qst_header_t *h = qst->headers[i];
        printf("qst_header {\n");
        printf("    header_size = %d\n", h->header_size);
        printf("    unknown1 = %d\n", h->unknown1);
        printf("    quest_no = %d\n", h->quest_no);
        printf("    unknown2 = [%d", h->unknown2[0]);
        for (int j = 1; j < QST_HEADER_UNKOWN2_SZ; j++) {
            printf(", %d", h->unknown2[j]);
        }
        printf("]\n");
        printf("    file_name = \"%s\"\n", h->file_name);
        printf("    file_size = %d\n", h->file_size);
        printf("    file_name_j = \"%s\"\n", h->file_name_j);
        printf("}\n");
    }

    for (unsigned int i = 0; i < qst->body.num_msgs; i++) {
        qst_message_t *msg = qst->body.messages[i];
        printf("qst_message {\n");
        printf("    unknown1 = [%d", msg->unknown1[0]);
        for (int j = 1; j < 4; j++) {
            printf(", %d", msg->unknown1[j]);
        }
        printf("]\n");
        printf("    file_chunk_no = %d\n", msg->file_chunk_no);
        printf("    unknown2 = [%d", msg->unknown2[0]);
        for (int j = 1; j < 3; j++) {
            printf(", %d", msg->unknown2[j]);
        }
        printf("]\n");
        printf("    file_name = \"%s\"\n", msg->file_name);
        printf("    file_chunk = [%d", msg->file_chunk[0]);
        for (int j = 1; j < 6; j++) {
            printf(", %d", msg->file_chunk[j]);
        }
        printf(" ...]\n");
        printf("    size = %d\n", msg->size);
        printf("    unknown3 = [%d", msg->unknown3[0]);
        for (int j = 1; j < QST_MESSAGE_UNKNOWN3_SZ; j++) {
            printf(", %d", msg->unknown3[j]);
        }
        printf("]\n");
        printf("}\n");
    }
}

int qst_extract(qst_t *qst, uint8_t *result, int format) {
    char *search_ext;

    switch (format) {
    case BIN:
        search_ext = ".bin";
        break;
    case DAT:
        search_ext = ".dat";
        break;
    default:
        fprintf(stderr, "Unknown format for qst_extract: %d.\n", format);
        return;
    }

    unsigned int prs_sz = 0;
    uint8_t *prs = NULL;

    for(unsigned int i = 0; i < qst->body.num_msgs; i++) {
        qst_message_t *msg = qst->body.messages[i];
        char *dot = strrchr((char *)msg->file_name, '.');
        if (dot && !strcmp(dot, search_ext)) {
            prs = ARRAY_CONCAT(uint8_t, prs, prs_sz, msg->file_chunk, msg->size);
            prs_sz += msg->size;
        }
    }

    int ret = pso_prs_decompress_buf(prs, result, prs_sz);

    if (ret < 0) {
        switch (ret) {
        case PSOARCHIVE_EBADMSG:
            fprintf(stderr, "PSOARCHIVE_EBADMSG\n");
            break;
        case PSOARCHIVE_EINVAL:
            fprintf(stderr, "PSOARCHIVE_EINVAL\n");
            break;
        case PSOARCHIVE_EFAULT:
            fprintf(stderr, "PSOARCHIVE_EFAULT\n");
            break;
        default:
            fprintf(stderr, "Unknown error %d", ret);
            break;
        }
        exit(1);
    }

    return ret;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "No input file specified.\n");
        exit(1);
    }
    char *inp_filename = argv[1];

    unsigned char *file_data = NULL;
    unsigned int file_sz;
    size_t read_sz;
    FILE *file = fopen(inp_filename, "r");

    if (file) {
        fseek(file, 0, SEEK_END);
        file_sz = ftell(file);
        rewind(file);

        file_data = (unsigned char *) malloc(sizeof(unsigned char) * (file_sz + 1));
        read_sz = fread(file_data, sizeof(unsigned char), file_sz, file);
        file_data[file_sz] = '\0';

        if (file_sz != read_sz) {
            free(file_data);
            file_data = NULL;
        }

        fclose(file);
    }

    qst_t *qst = parse_qst(file_sz, file_data);
    free(file_data);

    // print_qst(qst);
    uint8_t *dat_data = NULL;
    int bin_sz = qst_extract(qst, &dat_data, DAT);
    printf("%d\n", bin_sz);
}
