#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "dat.h"
#include "utils.h"

dat_t* parse_dat(size_t data_len, uint8_t *data) {
    uint8_t *cursor = data;
    dat_t *dat = malloc(sizeof(dat_t));

    unsigned int num_tbls = 0;
    dat_table_t **tbls = (dat_table_t **) malloc(num_tbls * sizeof(dat_table_t *));

    while ((unsigned int) (cursor - data) < data_len) {
        num_tbls++;
        tbls = (dat_table_t **) realloc(tbls, num_tbls * sizeof(dat_table_t *));

        dat_table_t *tbl = malloc(sizeof(dat_table_t));
        tbl->header = malloc(sizeof(dat_table_header_t));
        memcpy(tbl->header, cursor, sizeof(dat_table_header_t));
        cursor += DAT_HEADER_SZ;

        unsigned int num_items = 0;

        switch (tbl->header->type) {
        case DAT_TYPE_OBJECT:
            {
                unsigned int objs_len = tbl->header->table_body_size / DAT_OBJECT_SZ;
                tbl->body.objects = (dat_object_t **) malloc(objs_len * sizeof(dat_object_t *));
                for (; num_items < objs_len; num_items++) {
                    tbl->body.objects[num_items] = malloc(DAT_OBJECT_SZ);
                    memcpy(tbl->body.objects[num_items], cursor, DAT_OBJECT_SZ);
                    cursor += DAT_OBJECT_SZ;
                }
            }
            break;
        case DAT_TYPE_NPC:
            {
                unsigned int npcs_len = tbl->header->table_body_size / DAT_NPC_SZ;
                tbl->body.npcs = (dat_npc_t **) malloc(npcs_len * sizeof(dat_npc_t *));
                for (; num_items < npcs_len; num_items++) {
                    tbl->body.npcs[num_items] = malloc(DAT_NPC_SZ);
                    memcpy(tbl->body.npcs[num_items], cursor, DAT_NPC_SZ);
                    cursor += DAT_NPC_SZ;
                }
            }
            break;
        case DAT_TYPE_WAVE:
            {
                num_items = 1;
                tbl->body.waveinfo = malloc(sizeof(dat_waveinfo_t));
                tbl->body.waveinfo->header = malloc(sizeof(dat_waveinfo_header_t));
                memcpy(tbl->body.waveinfo->header, cursor, sizeof(dat_waveinfo_header_t));
                cursor += DAT_WAVEINFO_HEADER_SZ;

                uint32_t waves_sz = tbl->body.waveinfo->header->wavecount * DAT_WAVE_SZ;
                tbl->body.waveinfo->waves = (dat_wave_t **) malloc(tbl->body.waveinfo->header->wavecount * sizeof(dat_wave_t *));
                for (unsigned int i = 0; i < tbl->body.waveinfo->header->wavecount; i++) {
                    tbl->body.waveinfo->waves[i] = malloc(DAT_WAVE_SZ);
                    memcpy(tbl->body.waveinfo->waves[i], cursor, DAT_WAVE_SZ);
                    cursor += DAT_WAVE_SZ;
                }

                unsigned int events_sz = tbl->header->table_body_size - waves_sz - DAT_WAVEINFO_HEADER_SZ;
                tbl->body.waveinfo->wave_clear_events = malloc(events_sz);
                memcpy(tbl->body.waveinfo->wave_clear_events, cursor, events_sz);
                cursor += events_sz;
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

void dispose_dat(dat_t *dat) {
    for (unsigned int i = 0; i < dat->num_tables; i++) {
        dat_table_t *tbl = dat->entity_tables[i];
        switch (tbl->header->type) {
        case DAT_TYPE_OBJECT:
            for (unsigned int j = 0; j < tbl->num_items; j++) {
                free(tbl->body.objects[j]);
            }
            free(tbl->body.objects);
            break;
        case DAT_TYPE_NPC:
            for (unsigned int j = 0; j < tbl->num_items; j++) {
                free(tbl->body.npcs[j]);
            }
            free(tbl->body.npcs);
            break;
        case DAT_TYPE_WAVE:
            for (unsigned int j = 0; j < tbl->body.waveinfo->header->wavecount; j++) {
                free(tbl->body.waveinfo->waves[j]);
            }
            free(tbl->body.waveinfo->waves);
            free(tbl->body.waveinfo->header);
            free(tbl->body.waveinfo->wave_clear_events);
            free(tbl->body.waveinfo);
            break;
        default:
            break;
        }
        free(tbl->header);
        free(tbl);
    }
    free(dat->entity_tables);
    free(dat);
}
