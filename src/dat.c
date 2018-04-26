#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dat.h"
#include "utils.h"

dat_t* parse_dat(unsigned int data_len, uint8_t *data) {
    uint8_t *cursor = data;
    dat_t *dat = malloc(sizeof(dat_t));

    unsigned int num_tbls = 0;
    dat_table_t **tbls = (dat_table_t **) malloc(num_tbls * sizeof(dat_table_t *));

    while (cursor - data < data_len) {
        num_tbls++;
        tbls = (dat_table_t **) realloc(tbls, num_tbls * sizeof(dat_table_t *));

        dat_table_t *tbl = malloc(sizeof(dat_table_t));
        tbl->header = (dat_table_header_t *) cursor;
        cursor += DAT_HEADER_SZ;

        unsigned int num_items = 0;

        switch (tbl->header->type) {
        case DAT_TYPE_NPC:
            {
                unsigned int npcs_len = tbl->header->table_body_size / DAT_NPC_SZ;
                tbl->body.npcs = (dat_npc_t **) malloc(npcs_len * sizeof(dat_npc_t *));
                for (; num_items < npcs_len; num_items++) {
                    tbl->body.npcs[num_items] = (dat_npc_t *) cursor;
                    cursor += DAT_NPC_SZ;
                }
            }
            break;
        default:
            cursor += tbl->header->table_body_size;
            break;
        }

        tbl->num_items = num_items;

        tbls[num_tbls - 1] = tbl;
    }

    dat->num_tables = num_tbls;
    dat->entity_tables = tbls;

    return dat;
}

void print_dat(dat_t *dat) {
    for (unsigned int i = 0; i < dat->num_tables; i++) {
        dat_table_t *tbl = dat->entity_tables[i];
        printf("dat_table_header {\n");
        printf("    type = %d\n", tbl->header->type);
        printf("    table_size = %d\n", tbl->header->table_size);
        printf("    area = %d\n", tbl->header->area);
        printf("    table_body_size = %d\n", tbl->header->table_body_size);
        printf("}\n");

        switch (tbl->header->type) {
        case DAT_TYPE_NPC:
            for (unsigned int j = 0; j < tbl->num_items; j++) {
                dat_npc_t *npc = tbl->body.npcs[j];
                printf("dat_npc {\n");
                printf("    npc_type = %d\n", npc->npc_type);
                printf("    unknown1 = %d\n", npc->unknown1);
                printf("    unknown2 = ");
                print_byte_arr(DAT_NPC_UNKNOWN2_SZ, npc->unknown2);
                printf("    clone_count = %d\n", npc->clone_count);
                printf("    unknown3 = ");
                print_byte_arr(DAT_NPC_UNKNOWN3_SZ, npc->unknown3);
                printf("    section = %d\n", npc->section);
                printf("    wave_id = %d\n", npc->wave_id);
                printf("    wave_id_again = %d\n", npc->wave_id_again);
                printf("    x = %f\n", npc->x);
                printf("    y = %f\n", npc->y);
                printf("    z = %f\n", npc->z);
                printf("    x_rotation = %d\n", npc->x_rotation);
                printf("    y_rotation = %d\n", npc->y_rotation);
                printf("    z_rotation = %d\n", npc->z_rotation);
                printf("    movement_data = %f\n", npc->movement_data);
                printf("    flags = %d\n", npc->flags);
                printf("    unknown8 = ");
                print_byte_arr(DAT_NPC_UNKNOWN8_SZ, npc->unknown8);
                printf("    unknown9 = %d\n", npc->unknown9);
                printf("    skin = %d\n", npc->skin);
                printf("    unknown10 = %d\n", npc->unknown10);
                printf("}\n");
            }
            break;
        default:
            break;
        }
    }
}
