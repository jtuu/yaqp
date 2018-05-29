#ifndef YAQP_DAT_H
#define YAQP_DAT_H

#include <stdint.h>

#define FLOAT_MASK 0x800000

#define DAT_HEADER_SZ 16
#define DAT_OBJECT_SZ 68
#define DAT_NPC_SZ 72
#define DAT_NPC_UNKNOWN2_SZ 2
#define DAT_NPC_UNKNOWN3_SZ 4
#define DAT_NPC_UNKNOWN8_SZ 8
#define DAT_WAVEINFO_HEADER_SZ 16
#define DAT_WAVE_SZ 20
#define DAT_TYPE_OBJECT 1
#define DAT_TYPE_NPC 2
#define DAT_TYPE_WAVE 3
#define DAT_TYPE_UNKNOWN1 4
#define DAT_TYPE_UNKNOWN2 5

typedef struct dat_table_header {
    uint32_t type;
    uint32_t table_size;
    uint32_t area;
    uint32_t table_body_size;
} dat_table_header_t;

typedef struct dat_object {
   uint16_t object_type;
   uint8_t unknown1[6];
   uint16_t id;
   uint16_t group;
   uint16_t section;
   uint8_t unknown2[2];
   float x;
   float y;
   float z;
   uint32_t x_rotation;
   uint32_t y_rotation;
   uint32_t z_rotation;
   uint32_t full_random;
   uint32_t random_item;
   uint32_t fixed_item;
   uint8_t item_param[4];
   uint8_t unknown4[16];
} dat_object_t;

typedef struct dat_npc {
   uint16_t npc_type;
   uint16_t unknown1;
   uint8_t unknown2[2];
   uint16_t clone_count;
   uint8_t unknown3[4];
   uint16_t section;
   uint16_t wave_id;
   uint32_t wave_id_again;
   float x;
   float y;
   float z;
   uint32_t x_rotation;
   uint32_t y_rotation;
   uint32_t z_rotation;
   float movement_data;
   uint32_t flags;
   uint8_t unknown8[8];
   uint32_t unknown9;
   uint32_t skin;
   uint32_t unknown10;
} dat_npc_t;

typedef struct dat_waveinfo_header {
    uint32_t wavesize;
    uint32_t x10000000; // unknown, 10 00 00 00
    uint32_t wavecount;
    uint32_t zero; // unknown, 00 00 00 00
} dat_waveinfo_header_t;

typedef struct dat_wave {
    uint32_t wave_id;
    uint32_t x00000100; // unknown, 00 00 01 00
    uint16_t section;
    uint16_t wave;
    uint16_t delay;
    uint16_t unknown; // qedit code does something with this, don't know what it is used for
    uint32_t wave_clear_event_index; // index into wave clear event block
} dat_wave_t;

typedef struct dat_waveinfo {
    dat_waveinfo_header_t *header;
    dat_wave_t **waves;
    uint8_t *wave_clear_events;
} dat_waveinfo_t;

typedef struct dat_table {
    dat_table_header_t *header;
    unsigned int num_items;
    union body {
        dat_object_t **objects;
        dat_npc_t **npcs;
        dat_waveinfo_t *waveinfo;
    } body;
} dat_table_t;

typedef struct dat {
    unsigned int num_tables;
    dat_table_t **entity_tables;
} dat_t;

dat_t* parse_dat(unsigned int data_len, uint8_t *data);

void print_dat(dat_t *dat);

void dispose_dat(dat_t *dat);

#endif
