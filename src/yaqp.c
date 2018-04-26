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

#include "../lib/include/psoarchive-error.h"
#include "qst.h"
#include "bin.h"
#include "dat.h"
#include "utils.h"
#include "list.h"
#include "mon.h"
#include "areas.h"

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

#define NPC_MASK 0x800000

#define BREED_1TO1_MAP_(name) case BREED_##name: return MON_##name
#define BREED_1TO1_MAP(name) BREED_1TO1_MAP_(name)

int get_npc_kind(dat_npc_t *npc, int episode, int area) {
    switch (npc->npc_type) {
    case BREED_BEAR:
        return npc->skin & 0x01 ? MON_HILDEBLUE : MON_HILDEBEAR;
    case BREED_RAPPY:
        return npc->skin & 0x01 ? MON_AL_RAPPY : MON_RAG_RAPPY;
    case BREED_WOLF:
        return npc->flags & NPC_MASK ? MON_BARBAROUS_WOLF : MON_SAVAGE_WOLF;
    case BREED_BOOMA:
        switch (npc->skin) {
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
    case BREED_LILY:
        return (episode == 2 && area > 15) ? MON_DEL_LILY :
            (npc->flags & NPC_MASK ? MON_NAR_LILY : MON_POISON_LILY);
    BREED_1TO1_MAP(NANO_DRAGON);
    case BREED_SHARK:
        switch (npc->skin) {
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
        return npc->flags & NPC_MASK ? MON_POUILLY_SLIME : MON_POFUILLY_SLIME;
    BREED_1TO1_MAP(PAN_ARMS);
    case BREED_CHIC:
        return npc->skin & 0x01 ? MON_GILLCHIC : MON_DUBCHIC;
    BREED_1TO1_MAP(GARANZ);
    case BREED_SINOW_NINJA:
        return npc->flags & NPC_MASK ? MON_SINOW_GOLD : MON_SINOW_BEAT;
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
        switch (npc->skin) {
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
        switch (npc->skin) {
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
        return area > 15 ? MON_EPSILON :
            (npc->skin & 0x01 ? MON_SINOW_ZELE : MON_SINOW_ZOA);
    BREED_1TO1_MAP(ILL_GILL);
    BREED_1TO1_MAP(ASTARK);
    case BREED_LIZARD:
        return npc->flags & NPC_MASK ? MON_YOWIE : MON_SATELLITE_LIZARD;
    case BREED_MERISSA:
        return npc->skin & 0x01 ? MON_MERISSA_AA : MON_MERISSA_A;
    BREED_1TO1_MAP(GIRTABLULU);
    case BREED_ZU:
        return npc->skin & 0x01 ? MON_PAZUZU : MON_ZU;
    case BREED_BOOTA:
        switch (npc->skin) {
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
        switch (npc->skin) {
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
        return npc->flags & NPC_MASK ? MON_KONDRIEU :
            (npc->skin & 0x01 ? MON_SHAMBERTIN : MON_SAINT_MILION);
    default:
        return npc->npc_type;
    }
}

char* get_npc_name(int type) {
    if (type >= 0 && type < NUM_NPCS) {
        return npc_names[type];
    }

    char *str = malloc(16);
    snprintf(str, 16, "Unknown %d", type);
    return str;
}

void print_int_node(node_t *node) {
    printf("%d: %d\n", node->key, *(int *) node->data);
}

void print_npc_node(node_t *node) {
    if (node != NULL) {
        printf("    %s: %d\n", get_npc_name(node->key), *(int *) node->data);
    }
}

void print_npc_nodes(node_t *node) {
    printf("%s\n", ep1_area_names[node->key]);
    if (node != NULL) {
        traverse_nodes(*(node_t **) node->data, print_npc_node);
    }
}

const int one = 1;

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

    uint8_t *dat_data;
    uint8_t *bin_data;
    int dat_sz = qst_extract(qst, &dat_data, DAT);
    int bin_sz = qst_extract(qst, &bin_data, BIN);
    dat_t *dat = parse_dat(dat_sz, dat_data);
    bin_t *bin = parse_bin(bin_sz, bin_data);
    
    print_wide_str(bin->quest_name);
    
    node_t *area_npc_counts = NULL;

    int episode = 1;

    for (int i = 0; i < dat->num_tables; i++) {
        dat_table_t *tbl = dat->entity_tables[i];
        if (tbl->header->type != DAT_TYPE_NPC || tbl->header->area >= NUM_EP1_AREAS || tbl->header->area == 0) {
            continue;
        }

        int area = tbl->header->area;

        node_t *area_node = find_node(area_npc_counts, area);
        node_t *npc_counts = NULL;
        if (area_node != NULL) {
            npc_counts = *(node_t **) area_node->data;
        } else {
            area_npc_counts = prepend_node(area_npc_counts, area, 0, NULL);
            area_node = area_npc_counts;
        }

        for (int j = 0; j < tbl->num_items; j++) {
            dat_npc_t *npc = tbl->body.npcs[j];
            int type = get_npc_kind(npc, episode, area);
            if (type == MON_IGNORE) {
                continue;
            }
            node_t *found = find_node(npc_counts, type);
            if (found != NULL) {
                (*(int *) found->data)++;
            } else {
                npc_counts = prepend_node(npc_counts, type, sizeof(int), &one);
                node_t **ptr = area_node->data;
                *ptr = npc_counts;
            }
        }
    }

    traverse_nodes(area_npc_counts, print_npc_nodes);
}
