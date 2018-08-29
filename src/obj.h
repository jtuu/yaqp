#ifndef YAQP_OBJ_H
#define YAQP_OBJ_H

#include "dat.h"

typedef enum {
    OBJ_BOX_STANDARD = 0x88,
    OBJ_BOX_SPECIAL = 0x92,
    OBJ_BOX_STANDARD_RUINS = 0x162,
    OBJ_BOX_SPECIAL_RUINS = 0x161,
    OBJ_BOX_STANDARD_CCA = 0x200,
    OBJ_BOX_SPECIAL_CCA = 0x203,
    OBJ_GEE_NEST = 0x2b0,
    OBJ_BOX_SPECIAL_DESERT = 0x38d
} obj_skin;

typedef enum {
    OBJ_RANDOM_BOX,
    OBJ_RANDOM_BOX_NO_RARE,
    OBJ_FIXED_BOX_WEAPON,
    OBJ_FIXED_BOX_ARMOR,
    OBJ_FIXED_BOX_MAG,
    OBJ_FIXED_BOX_TOOL,
    OBJ_FIXED_BOX_TECH,
    OBJ_FIXED_BOX_MESETA,
    OBJ_UNKNOWN,
    OBJ_IGNORE,
    OBJ_FIXED_BOX_EXACT
} obj_kind;

obj_kind get_obj_kind(dat_object_t*);
const char* get_obj_name(obj_kind);

#endif
