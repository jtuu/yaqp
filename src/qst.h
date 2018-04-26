#ifndef YAQP_QST_H
#define YAQP_QST_H

#include <stdint.h>

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

qst_t* parse_qst(unsigned int data_len, unsigned char *data);

void print_qst(qst_t *qst);

int qst_extract(qst_t *qst, uint8_t **result, int format);

#endif
