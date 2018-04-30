#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../lib/include/PRS-decomp.h"
#include "../lib/include/psoarchive-error.h"
#include "utils.h"
#include "qst.h"

qst_t* parse_qst(unsigned int data_len, unsigned char *data) {
    unsigned char *cursor = data;
    qst_t *qst = malloc(sizeof(qst_t));

    for (int i = 0; i < NUM_QST_HEADER; i++) {
        qst->headers[i] = malloc(sizeof(qst_header_t));
        memcpy(qst->headers[i], cursor, sizeof(qst_header_t));
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

int qst_extract(qst_t *qst, uint8_t **result, int format) {
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
        return 0;
    }

    int prs_sz = 0;
    for (unsigned int i = 0; i < NUM_QST_HEADER; i++) {
        qst_header_t *header = qst->headers[i];
        char *dot = strrchr(header->file_name, '.');
        if (dot && !strcmp(dot, search_ext)) {
            prs_sz = header->file_size;
            break;
        }
    }

    uint8_t *prs = malloc(prs_sz);
    uint8_t *cursor = prs;
    int chunk_no = 0;
    for (unsigned int i = 0; i < qst->body.num_msgs; i++) {
        qst_message_t *msg = qst->body.messages[i];
        char *dot = strrchr((char *) msg->file_name, '.');
        if (dot && !strcmp(dot, search_ext)) {
            if (msg->file_chunk_no != chunk_no++) {
                fprintf(stderr, "%s chunks out of order\n", search_ext);
                exit(1);
            }
            memcpy(cursor, msg->file_chunk, msg->size);
            cursor += msg->size;
        }
    }

    int ret = pso_prs_decompress_buf(prs, result, prs_sz);
    free(prs);

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
    }

    return ret;
}

void dispose_qst(qst_t *qst) {
    for (unsigned int i = 0; i < NUM_QST_HEADER; i++) {
        free(qst->headers[i]);
    }
    for (unsigned int i = 0; i < qst->body.num_msgs; i++) {
        free(qst->body.messages[i]->file_chunk);
        free(qst->body.messages[i]);
    }
    free(qst->body.messages);
    free(qst);
}
