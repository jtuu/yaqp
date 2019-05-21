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

/*
Contains parts of the Tethealla project.
(https://pioneer2.net)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

#include "../lib/include/psoarchive-error.h"
#include "qst.h"
#include "bin.h"
#include "dat.h"
#include "utils.h"
#include "list.h"
#include "mon.h"
#include "areas.h"
#include "bp.h"
#include "obj.h"

typedef enum {
    BREED_BEAR = 64,
    BREED_RAPPY = 65,
    BREED_MONEST = 66,
    BREED_WOLF = 67,
    BREED_BOOMA = 68,

    BREED_GRASS_ASSASSIN = 96,
    BREED_LILY = 97,
    BREED_NANO_DRAGON = 98,
    BREED_SHARK = 99,
    BREED_SLIME = 100,
    BREED_PAN_ARMS = 101,

    BREED_CHIC = 128,
    BREED_GARANZ = 129,
    BREED_SINOW_NINJA = 130,
    BREED_CANADINE = 131,
    BREED_CANANE = 132,
    BREED_DUBWITCH = 133,

    BREED_DELSABER = 160,
    BREED_CHAOS_SORCERER = 161,
    BREED_DARK_GUNNER = 162,
    BREED_DEATH_GUNNER = 163,
    BREED_CHAOS_BRINGER = 164,
    BREED_DARK_BELRA = 165,
    BREED_DIMENIAN = 166,
    BREED_BULCLAW = 167,
    BREED_CLAW = 168,

    BREED_NATIVE_BOSS = 192,
    BREED_DE_ROL_LE = 193,
    BREED_VOL_OPT_ = 194,
    BREED_VOL_OPT = 197,
    BREED_DARK_FALZ = 200,
    BREED_OLGA_FLOW = 202,
    BREED_BARBA_RAY = 203,
    BREED_GOL_DRAGON = 204,

    BREED_SINOW_FAT_1 = 212,
    BREED_MERIL = 213,
    BREED_MERIC = 214,
    BREED_GIBBON = 215,

    BREED_GIBBLES = 216,
    BREED_GEE = 217,
    BREED_GI_GUE = 218,

    BREED_DELDEPTH = 219,
    BREED_DELBITER = 220,
    BREED_DOLM = 221,
    BREED_MORFOS = 222,
    BREED_RECOBOX = 223,
    
    BREED_SINOW_FAT_2 = 224,
    BREED_ILL_GILL = 225,

    BREED_ASTARK = 272,
    BREED_LIZARD = 273,
    BREED_MERISSA = 274,
    BREED_GIRTABLULU = 275,
    BREED_ZU = 276,
    BREED_BOOTA = 277,
    BREED_DORPHON = 278,
    BREED_GORAN = 279,
    BREED_SNAKE = 281
} mon_breed;

#define BREED_1TO1_MAP_(name) case BREED_##name: return MON_##name
#define BREED_1TO1_MAP(name) BREED_1TO1_MAP_(name)

// similar to ParseMapData of Tethealla's ship_server.c
/************************************************************************
	Tethealla Ship Server
	Copyright (C) 2008  Terry Chatman Jr.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License version 3 as
	published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/
int get_npc_kind(dat_npc_t *npc, int episode, int area) {
    switch (npc->npc_type) {
    case BREED_BEAR:
        return npc->skin & 0x01 ? MON_HILDEBLUE : MON_HILDEBEAR;
    case BREED_RAPPY:
        return episode == 4 ? (npc->skin & 0x01 ? MON_DEL_RAPPY : MON_SAND_RAPPY) :
            (npc->skin & 0x01 ? (episode == 2 ? MON_LOVE_RAPPY : MON_AL_RAPPY) : MON_RAG_RAPPY);
    case BREED_WOLF:
        return npc->flags & FLOAT_MASK ? MON_BARBAROUS_WOLF : MON_SAVAGE_WOLF;
    case BREED_BOOMA:
        switch (npc->skin % 3) {
        case 0:
            return MON_BOOMA;
        case 1:
            return MON_GOBOOMA;
        case 2:
            return MON_GIGOBOOMA;
        default:
            return MON_UNKNOWN;
        }
    BREED_1TO1_MAP(MONEST);
    BREED_1TO1_MAP(GRASS_ASSASSIN);
    case BREED_LILY:
        return area == EP2_CONTROL_TOWER ? MON_DEL_LILY :
            (npc->skin & 0x01 ? MON_NAR_LILY : MON_POISON_LILY);
    BREED_1TO1_MAP(NANO_DRAGON);
    case BREED_SHARK:
        switch (npc->skin % 3) {
        case 0:
            return MON_EVIL_SHARK;
        case 1:
            return MON_PAL_SHARK;
        case 2:
            return MON_GUIL_SHARK;
        default:
            return MON_UNKNOWN;
        }
    case BREED_SLIME:
        return npc->flags & FLOAT_MASK ? MON_POUILLY_SLIME : MON_POFUILLY_SLIME;
    BREED_1TO1_MAP(PAN_ARMS);
    case BREED_CHIC:
        return npc->skin & 0x01 ? MON_GILLCHIC : MON_DUBCHIC;
    BREED_1TO1_MAP(GARANZ);
    case BREED_SINOW_NINJA:
        return npc->flags & FLOAT_MASK ? MON_SINOW_GOLD : MON_SINOW_BEAT;
    BREED_1TO1_MAP(CANADINE);
    BREED_1TO1_MAP(CANANE);
    BREED_1TO1_MAP(DUBWITCH);
    BREED_1TO1_MAP(DELSABER);
    BREED_1TO1_MAP(CHAOS_SORCERER);
    BREED_1TO1_MAP(DARK_GUNNER);
    BREED_1TO1_MAP(DEATH_GUNNER);
    BREED_1TO1_MAP(CHAOS_BRINGER);
    BREED_1TO1_MAP(DARK_BELRA);
    case BREED_DIMENIAN:
        switch (npc->skin % 3) {
        case 0:
            return MON_DIMENIAN;
        case 1:
            return MON_LA_DIMENIAN;
        case 2:
            return MON_SO_DIMENIAN;
        default:
            return MON_UNKNOWN;
        }
    BREED_1TO1_MAP(BULCLAW);
    BREED_1TO1_MAP(CLAW);
    case BREED_NATIVE_BOSS:
        return episode == 2 ? MON_GAL_GRYPHON : MON_DRAGON;
    BREED_1TO1_MAP(DE_ROL_LE);
    case BREED_VOL_OPT_:
        return MON_IGNORE;
    BREED_1TO1_MAP(VOL_OPT);
    BREED_1TO1_MAP(DARK_FALZ);
    BREED_1TO1_MAP(OLGA_FLOW);
    BREED_1TO1_MAP(BARBA_RAY);
    BREED_1TO1_MAP(GOL_DRAGON);
    case BREED_SINOW_FAT_1:
        return npc->skin & 0x01 ? MON_SINOW_SPIGELL : MON_SINOW_BERILL;
    case BREED_MERIL:
        return npc->skin & 0x01 ? MON_MERILTAS : MON_MERILLIA;
    case BREED_MERIC:
        switch (npc->skin % 3) {
        case 0:
            return MON_MERICAROL;
        case 1:
            return MON_MERIKLE;
        case 2:
            return MON_MERICUS;
        default:
            return MON_UNKNOWN;
        }
    case BREED_GIBBON:
        return npc->skin & 0x01 ? MON_ZOL_GIBBON : MON_UL_GIBBON;
    BREED_1TO1_MAP(GIBBLES);
    BREED_1TO1_MAP(GEE);
    BREED_1TO1_MAP(GI_GUE);
    BREED_1TO1_MAP(DELDEPTH);
    BREED_1TO1_MAP(DELBITER);
    case BREED_DOLM:
        return npc->skin & 0x01 ? MON_DOLMDARL : MON_DOLMOLM;
    BREED_1TO1_MAP(MORFOS);
    BREED_1TO1_MAP(RECOBOX);
    case BREED_SINOW_FAT_2:
        return area == EP2_CONTROL_TOWER ? MON_EPSILON :
            (npc->skin & 0x01 ? MON_SINOW_ZELE : MON_SINOW_ZOA);
    BREED_1TO1_MAP(ILL_GILL);
    BREED_1TO1_MAP(ASTARK);
    case BREED_LIZARD:
        return npc->flags & FLOAT_MASK ? MON_YOWIE : MON_SATELLITE_LIZARD;
    case BREED_MERISSA:
        return npc->skin & 0x01 ? MON_MERISSA_AA : MON_MERISSA_A;
    BREED_1TO1_MAP(GIRTABLULU);
    case BREED_ZU:
        return npc->skin & 0x01 ? MON_PAZUZU : MON_ZU;
    case BREED_BOOTA:
        switch (npc->skin % 3) {
        case 0:
            return MON_BOOTA;
        case 1:
            return MON_ZE_BOOTA;
        case 2:
            return MON_BA_BOOTA;
        default:
            return MON_UNKNOWN;
        }
    case BREED_DORPHON:
        return npc->skin & 0x01 ? MON_DORPHON_ECLAIR : MON_DORPHON;
    case BREED_GORAN:
        switch (npc->skin % 3) {
        case 0:
            return MON_GORAN;
        case 1:
            return MON_PYRO_GORAN;
        case 2:
            return MON_GORAN_DETONATOR;
        default:
            return MON_UNKNOWN;
        }
    case BREED_SNAKE:
        return npc->flags & FLOAT_MASK ? MON_KONDRIEU :
            (npc->skin & 0x01 ? MON_SHAMBERTIN : MON_SAINT_MILION);
    default:
        return MON_IGNORE;
    }
}

int get_mon_bp_id(mon_kind kind, int episode, int area) {
    mon_kind *ids;
    int ids_len;

    switch (episode) {
    case 1:
        ids = ep1_bp_ids;
        ids_len = NUM_EP1_BP;
        break;
    case 2:
        ids = ep2_bp_ids;
        ids_len = NUM_EP2_BP;
        break;
    case 4:
        ids = ep4_bp_ids;
        ids_len = NUM_EP4_BP;
        break;
    default:
        fprintf(stderr, "Invalid episode %d passed to get_mon_bp_id\n", episode);
        return -1;
    }

    if (episode == 4 && area >= EP4_SUBTERRANEAN_DESERT_1) {
        for (int i = ids_len - 1; i >= 0; i--) {
            if (ids[i] == kind) {
                return i;
            }
        }
    } else {
        for (int i = 0; i < ids_len; i++) {
            if (ids[i] == kind) {
                return i;
            }
        }
    }

    return -1;
}

const char* get_npc_name(int type) {
    if (type >= 0 && type < NUM_NPCS) {
        return npc_names[type];
    }

    char *str = malloc(16);
    snprintf(str, 16, "Unknown %d", type);
    return str;
}

int dat_detect_episode(dat_t *dat) {
    for (unsigned int i = 0; i < dat->num_tables; i++) {
        dat_table_t *tbl = dat->entity_tables[i];
        uint32_t area = tbl->header->area;
        if (tbl->header->type != DAT_TYPE_NPC || area == 0) {
            continue;
        }

        for (unsigned int j = 0; j < tbl->num_items; j++) {
            dat_npc_t *npc = tbl->body.npcs[j];

            switch (npc->npc_type) {
            case BREED_BOOMA:
            case BREED_NANO_DRAGON:
            case BREED_SHARK:
            case BREED_SLIME:
            case BREED_SINOW_NINJA:
            case BREED_CANADINE:
            case BREED_CANANE:
            case BREED_DARK_GUNNER:
            case BREED_DEATH_GUNNER:
            case BREED_CHAOS_BRINGER:
            case BREED_BULCLAW:
            case BREED_CLAW:
            case BREED_DE_ROL_LE:
            case BREED_VOL_OPT:
            case BREED_VOL_OPT_:
            case BREED_DARK_FALZ:
                return 1;
            case BREED_OLGA_FLOW:
            case BREED_BARBA_RAY:
            case BREED_GOL_DRAGON:
            case BREED_SINOW_FAT_1:
            case BREED_MERIL:
            case BREED_MERIC:
            case BREED_GIBBON:
            case BREED_GIBBLES:
            case BREED_GEE:
            case BREED_GI_GUE:
            case BREED_DELDEPTH:
            case BREED_DELBITER:
            case BREED_DOLM:
            case BREED_MORFOS:
            case BREED_RECOBOX:
            case BREED_SINOW_FAT_2:
            case BREED_ILL_GILL:
                return 2;
            case BREED_ASTARK:
            case BREED_LIZARD:
            case BREED_MERISSA:
            case BREED_GIRTABLULU:
            case BREED_ZU:
            case BREED_BOOTA:
            case BREED_DORPHON:
            case BREED_GORAN:
            case BREED_SNAKE:
                return 4;
            default:
                break;
            }
        }
    }
    return -1;
}

#define OPCODE_SET_EPISODE_TABLE 0xf8
#define OPCODE_SET_EPISODE 0xbc

#define OPCODE_BB_MAP_DESIGNATE_TABLE 0xf9
#define OPCODE_BB_MAP_DESIGNATE 0x51

int bin_detect_episode(bin_t *bin) {
    uint32_t obj_code_len = bin->function_offset_table_offset - bin->object_code_offset;
    for (unsigned int i = 0; i < obj_code_len - 2; i++) {
        uint8_t a = bin->object_code[i];
        uint8_t b = bin->object_code[i + 1];
        uint8_t c = bin->object_code[i + 2];
        if (a == OPCODE_SET_EPISODE_TABLE && b == OPCODE_SET_EPISODE) {
            switch (c) {
            case 0:
                return 1;
            case 1:
                return 2;
            case 2:
                return 4;
            default:
                break;
            }
        }
    }
    return -1;
}

#define NUM_AREAS 18
#define NUM_MAPS 45

int* bin_get_map_ids(bin_t *bin) {
    uint32_t obj_code_len = bin->function_offset_table_offset - bin->object_code_offset;
    int *map_ids = malloc(NUM_AREAS * sizeof(int));
    for (int i = 0; i < NUM_AREAS; i++) { map_ids[i] = -1; }
    
    for (unsigned int i = 0; i < obj_code_len - 2; i++) {
        uint8_t a = bin->object_code[i];
        uint8_t b = bin->object_code[i + 1];
        if (a == OPCODE_BB_MAP_DESIGNATE_TABLE && b == OPCODE_BB_MAP_DESIGNATE) {
            uint8_t area_id = bin->object_code[i + 2];
            uint8_t map_id = bin->object_code[i + 3];
            map_ids[area_id] = (int) map_id;
        }
    }
    return map_ids;
}

long null = 0;
int one = 1;
int eight = 8;

#define BOXES -8035

node_t* count_monsters(dat_t *dat, int episode) {
    node_t *area_npc_counts = NULL;

    for (unsigned int i = dat->num_tables - 1; i < dat->num_tables; i--) {
        dat_table_t *tbl = dat->entity_tables[i];
        int area = (int) tbl->header->area;
        if (area == 0) {
            continue;
        }

        switch (tbl->header->type) {
        case DAT_TYPE_NPC:
            {
                node_t *area_node = find_node(area_npc_counts, area);
                node_t *rooms = NULL;
                if (area_node != NULL) {
                    rooms = *(node_t **) area_node->data;
                } else {
                    area_npc_counts = prepend_node(area_npc_counts, area, sizeof(node_t *), &null);
                    area_node = area_npc_counts;
                }

                for (unsigned int j = 0; j < tbl->num_items; j++) {
                    dat_npc_t *npc = tbl->body.npcs[j];
                    int kind = get_npc_kind(npc, episode, area);
                    if (kind == MON_IGNORE) {
                        continue;
                    }

                    node_t *room_node = find_node(rooms, npc->section);
                    node_t *waves = NULL;
                    if (room_node != NULL) {
                        waves = *(node_t **) room_node->data;
                    } else {
                        room_node = prepend_node(rooms, npc->section, sizeof(node_t *), &null);
                        rooms = room_node;
                        node_t **ptr = (node_t **) area_node->data;
                        *ptr = rooms;
                    }

                    node_t *wave_node = find_node(waves, npc->wave_id);
                    node_t *npc_counts = NULL;
                    if (wave_node != NULL) {
                        npc_counts = *(node_t **) wave_node->data;
                    } else {
                        wave_node = prepend_node(waves, npc->wave_id, sizeof(node_t *), &null);
                        waves = wave_node;
                        node_t **ptr = (node_t **) room_node->data;
                        *ptr = waves;
                    }
                    
                    node_t *count_node = find_node(npc_counts, kind);
                    if (count_node != NULL) {
                        (*(int *) count_node->data)++;
                    } else {
                        npc_counts = prepend_node(npc_counts, kind, sizeof(int), &one);
                        node_t **ptr = (node_t **) wave_node->data;
                        *ptr = npc_counts;
                    }
                    
                    switch (kind) {
                    case MON_CANANE:
                        {
                            node_t *canadine_node = find_node(npc_counts, MON_CANADINE);
                            if (canadine_node != NULL) {
                                (*(int *) canadine_node->data) += 8;
                            } else {
                                npc_counts = prepend_node(npc_counts, MON_CANADINE, sizeof(int), &eight);
                                node_t **ptr = (node_t **) wave_node->data;
                                *ptr = npc_counts;
                            }
                        }
                        break;
                    case MON_RECOBOX:
                        {
                            node_t *recon_node = find_node(npc_counts, MON_RECON);
                            int clone_count = npc->clone_count;
                            if (recon_node != NULL) {
                                (*(int *) recon_node->data) += clone_count;
                            } else {
                                npc_counts = prepend_node(npc_counts, MON_RECON, sizeof(int), &clone_count);
                                node_t **ptr = (node_t **) wave_node->data;
                                *ptr = npc_counts;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            break;
        case DAT_TYPE_OBJECT:
            {
                node_t *area_node = find_node(area_npc_counts, area);
                node_t *rooms = NULL;
                if (area_node != NULL) {
                    rooms = *(node_t **) area_node->data;
                } else {
                    area_npc_counts = prepend_node(area_npc_counts, area, sizeof(node_t *), &null);
                    area_node = area_npc_counts;
                }

                for (unsigned int j = 0; j < tbl->num_items; j++) {
                    dat_object_t *obj = tbl->body.objects[j];
                    int kind = get_obj_kind(obj);
                    if (kind == OBJ_IGNORE) {
                        continue;
                    }

                    node_t *room_node = find_node(rooms, obj->section);
                    node_t *waves = NULL;
                    if (room_node != NULL) {
                        waves = *(node_t **) room_node->data;
                    } else {
                        room_node = prepend_node(rooms, obj->section, sizeof(node_t *), &null);
                        rooms = room_node;
                        node_t **ptr = (node_t **) area_node->data;
                        *ptr = rooms;
                    }

                    node_t *wave_node = find_node(waves, BOXES);
                    node_t *box_counts = NULL;
                    if (wave_node != NULL) {
                        box_counts = *(node_t **) wave_node->data;
                    } else {
                        wave_node = prepend_node(waves, BOXES, sizeof(node_t *), &null);
                        waves = wave_node;
                        node_t **ptr = (node_t **) room_node->data;
                        *ptr = waves;
                    }
                    
                    node_t *count_node = find_node(box_counts, kind);
                    if (count_node != NULL) {
                        (*(int *) count_node->data)++;
                    } else {
                        box_counts = prepend_node(box_counts, kind, sizeof(int), &one);
                        node_t **ptr = (node_t **) wave_node->data;
                        *ptr = box_counts;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    return area_npc_counts;
}

void write_monster_counts_as_json(char *dest_file_name, bin_t *bin, node_t *area, int episode) {
    int area_name_offset = 0;
    const char* const *area_names = ep1_area_names;

    switch (episode) {
    default:
    case 1:
        area_names = ep1_area_names;
        area_name_offset = 0;
        break;
    case 2:
        area_names = ep2_area_names;
        area_name_offset = 18;
        break;
    case 4:
        area_names = ep4_area_names;
        area_name_offset = 35;
        break;
    }

    int *map_ids = bin_get_map_ids(bin);
    int cur_area_counts[NUM_MAPS];
    int max_area_counts[NUM_MAPS];
    for (int i = 0; i < NUM_MAPS; i++) {
        cur_area_counts[i] = 1;
        max_area_counts[i] = 0;
    }

    node_t *a = area;
    while (a != NULL) {
        int map_id = map_ids[a->key];
        if (map_id > -1) {
            max_area_counts[map_id]++;
        } else {
            map_ids[a->key] = a->key + area_name_offset;
        }
        a = a->next;
    }

    FILE *file = fopen(dest_file_name, "w");
    if (file) {
        fprintf(file, "{\n  \"quest_name\": \"");
        print_wide_str_json_escaped(file, bin->quest_name);
        fprintf(file, "\",\n  \"episode\": %d,\n", episode);
        fprintf(file, "  \"quest_id\": %d,\n", bin->quest_number);
        fprintf(file, "  \"short_description\": \"");
        print_wide_str_json_escaped(file, bin->short_description);
        fprintf(file, "\", \n  \"long_description\": \"");
        print_wide_str_json_escaped(file, bin->long_description);
        fprintf(file, "\",\n  \"areas\": [\n");

        while (area != NULL) {
            int map_id = map_ids[area->key];
            int cur_area_count = cur_area_counts[map_id];
            int max_area_count = max_area_counts[map_id];
            cur_area_counts[map_id]++;
            map_id -= area_name_offset;

            fprintf(file, "    {\n");
            if (max_area_count > 1) {
                fprintf(file, "      \"area_name\": \"%s (%d/%d)\",\n", area_names[map_id], cur_area_count, max_area_count);
            } else {
                fprintf(file, "      \"area_name\": \"%s\",\n", area_names[map_id]);
            }
            fprintf(file, "      \"rooms\": [\n");

            node_t *room = *(node_t **) area->data;
            node_t *boxes = NULL;
            while (room != NULL) {
                fprintf(file,
                    "        {\n"
                    "          \"room_id\": %d,\n"
                    "          \"waves\": [\n",
                    room->key);
                node_t *wave = sort_nodes(*(node_t **) room->data);
                
                while (wave != NULL) {
                    if (wave->key == BOXES) {
                        boxes = *(node_t **) wave->data;
                        wave = wave->next;
                    } else {
                        fprintf(file,
                            "            {\n"
                            "              \"wave_id\": %d,\n"
                            "              \"monsters\": [\n",
                            wave->key);
                        node_t *mon = *(node_t **) wave->data;

                        while (mon != NULL) {
                            fprintf(file,
                                "                {\n"
                                "                  \"monster_name\": \"%s\",\n"
                                "                  \"bp_id\": %d,\n"
                                "                  \"count\": %d\n"
                                "                }",
                                get_npc_name(mon->key),
                                get_mon_bp_id(mon->key, episode, area->key),
                                *(int *) mon->data);
                            mon = mon->next;

                            if (mon != NULL) {
                                fprintf(file, ",\n");
                            } else {
                                fprintf(file, "\n");
                            }
                        }
                        wave = wave->next;

                        if (wave != NULL) {
                            fprintf(file,
                                "              ]\n"
                                "            },\n");
                        } else {
                            fprintf(file, "              ]\n"
                                "            }\n");
                        }
                    }
                }
                room = room->next;

                fprintf(file,
                        "          ],\n"
                        "          \"boxes\": [\n");

                while (boxes != NULL) {
                    fprintf(file,
                        "            {\n"
                        "              \"box_name\": \"%s\",\n"
                        "              \"count\": %d\n",
                        get_obj_name(boxes->key),
                        *(int *) boxes->data);

                    boxes = boxes->next;

                    if (boxes != NULL) {
                        fprintf(file, 
                            "            },\n");
                    } else {
                        fprintf(file, 
                            "            }\n");
                    }
                }

                if (room != NULL) {
                    fprintf(file,
                                "          ]\n"
                                "        },\n");
                } else {
                    fprintf(file,
                                "          ]\n"
                                "        }\n");
                }
            }
            
            area = area->next;
            
            if (area != NULL) {
                fprintf(file,
                    "      ]\n"
                    "    },\n");
            } else {
                fprintf(file,
                    "      ]\n"
                    "    }\n");
            }
        }

        fprintf(file, "  ]\n}\n");

        fclose(file);
    } else {
        fprintf(stderr, "Failed to open %s\n", dest_file_name);
    }

    free(map_ids);
}

void dispose_monster_counts(node_t *area) {
    node_t *atmp;
    while (area != NULL) {
        node_t *r = *(node_t **) area->data;
        node_t *rtmp;
        while (r != NULL) {
            node_t *w = *(node_t **) r->data;
            node_t *wtmp;
            while (w != NULL) {
                node_t *m = *(node_t **) w->data;
                node_t *mtmp;
                while (m != NULL) {
                    mtmp = m->next;
                    free(m->data);
                    free(m);
                    m = mtmp;
                }
                wtmp = w->next;
                free(w->data);
                free(w);
                w = wtmp;
            }
            rtmp = r->next;
            free(r->data);
            free(r);
            r = rtmp;
        }
        atmp = area->next;
        free(area->data);
        free(area);
        area = atmp;
    }
}

typedef enum {
    VERBOSE,
    WRITE_JSON,
    DUMP_BIN,
    DUMP_DAT,
    DUMP_OBJCODE,
    NUM_OPTS
} yaqp_opt;

const char opt_shorthands[NUM_OPTS] = {
    [VERBOSE] = 'v',
    [WRITE_JSON] = 'j',
    [DUMP_BIN] = 'b',
    [DUMP_DAT] = 'd',
    [DUMP_OBJCODE] = 'o'
};

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "Not enough arguments.\n");
        exit(1);
    }

    bool opts[NUM_OPTS] = {false};
    int num_opts = 0;

    // check args for options
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] == '-') {
            num_opts++;
            char *c = &arg[1];
            // stop checking after "--"
            if (*c == '-') {
                break;
            }
            while (*c) {
                for (unsigned int j = 0; j < NUM_OPTS; j++) {
                    if (*c == opt_shorthands[j]) {
                        opts[j] = true;
                        break;
                    }
                }
                c++;
            }
        }
    }

    if ((argc - num_opts) <= 1) {
        fprintf(stderr, "No input file(s) specified.\n");
        return -1;
    }
    
    setlocale(LC_ALL, "");

    // process files
    bool double_dash = false;
    for (int i = 1; i < argc; i++) {
        char *file_name = argv[i];
        // ignore args starting with a dash unless a double dash was encountered
        if (!double_dash && file_name[0] == '-') {
            if (file_name[1] == '-') {
                double_dash = true;
            }
            continue;
        }

        uint8_t *file_data;
        FILE *file = fopen(file_name, "rb");
        long ftell_result;
        size_t file_sz;
        FILE *out_file;

        if (file) {
            fseek(file, 0, SEEK_END);
            ftell_result = ftell(file);
            if (ftell_result < 0) {
                fprintf(stderr, "ftell failed with error %d\n", errno);
                fclose(file);
                continue;
            }
            rewind(file);

            file_sz = (size_t) ftell_result;

            file_data = malloc(file_sz * sizeof(uint8_t));
            fread(file_data, file_sz, 1, file);

            fclose(file);

            qst_t *qst = parse_qst(file_sz, file_data);

            uint8_t *dat_data;
            uint8_t *bin_data;
            int dat_sz = qst_extract(qst, &dat_data, DAT);
            int bin_sz = qst_extract(qst, &bin_data, BIN);
            if (dat_sz <= 0 || bin_sz <= 0) {
                free(file_data);
                dispose_qst(qst);
                fprintf(stderr, "Failed to parse %s, skipping\n", file_name);
                continue;
            }

            dat_t *dat = parse_dat((size_t) dat_sz, dat_data);
            bin_t *bin = parse_bin((size_t) bin_sz, bin_data);

            if (opts[WRITE_JSON]) {    
                int episode = bin_detect_episode(bin);
                if (episode == -1) {
                    episode = dat_detect_episode(dat);
                    if (episode == -1) {
                        fprintf(stderr, "Failed to detect episode, defaulting to 1\n");
                        episode = 1;
                    }
                }

                node_t *monster_counts = count_monsters(dat, episode);

                char *dest_file_name = change_file_ext(file_name, ".json");
                write_monster_counts_as_json(dest_file_name, bin, monster_counts, episode);

                if (opts[VERBOSE]) {
                    printf("%s\n", dest_file_name);
                }

                free(dest_file_name);
                dispose_monster_counts(monster_counts);
            }

            if (opts[DUMP_BIN]) {
                char *dest_file_name = change_file_ext(file_name, ".bin");
                out_file = fopen(dest_file_name, "wb");

                if (out_file) {
                    fwrite(bin_data, (size_t) bin_sz, 1, out_file);
                    fclose(out_file);

                    if (opts[VERBOSE]) {
                        printf("%s\n", dest_file_name);
                    }
                } else {
                    fprintf(stderr, "Failed to write file %s\n", dest_file_name);
                }

                free(dest_file_name);
            }

            if (opts[DUMP_DAT]) {
                char *dest_file_name = change_file_ext(file_name, ".dat");
                out_file = fopen(dest_file_name, "wb");

                if (out_file) {
                    fwrite(dat_data, (size_t) dat_sz, 1, out_file);
                    fclose(out_file);

                    if (opts[VERBOSE]) {
                        printf("%s\n", dest_file_name);
                    }
                } else {
                    fprintf(stderr, "Failed to write file %s\n", dest_file_name);
                }

                free(dest_file_name);
            }

            if (opts[DUMP_OBJCODE]) {
                char *objcode_dest_file_name = change_file_ext(file_name, ".o");
                out_file = fopen(objcode_dest_file_name, "wb");

                if (out_file) {
                    fwrite(bin->object_code, bin->object_code_len, 1, out_file);
                    fclose(out_file);

                    if (opts[VERBOSE]) {
                        printf("%s\n", objcode_dest_file_name);
                    }
                } else {
                    fprintf(stderr, "Failed to write file %s\n", objcode_dest_file_name);
                }

                free(objcode_dest_file_name);

                char *funcs_dest_file_name = change_file_ext(file_name, ".functions");
                out_file = fopen(funcs_dest_file_name, "wb");

                if (out_file) {
                    fwrite(bin->function_offset_table, bin->function_offset_table_len, 1, out_file);
                    fclose(out_file);

                    if (opts[VERBOSE]) {
                        printf("%s\n", funcs_dest_file_name);
                    }
                } else {
                    fprintf(stderr, "Failed to write file %s\n", funcs_dest_file_name);
                }

                free(funcs_dest_file_name);
            }

            free(dat_data);
            free(bin_data);
            free(file_data);
            dispose_dat(dat);
            dispose_bin(bin);
            dispose_qst(qst);
        } else {
            fprintf(stderr, "Failed to open %s\n", file_name);
        }
    }
}
