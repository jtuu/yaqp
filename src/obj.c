#include "dat.h"
#include "obj.h"
#include "items.h"
#include "utils.h"

obj_kind get_obj_kind(dat_object_t *obj) {
    switch (obj->object_type) {
    case OBJ_BOX_STANDARD:
    case OBJ_BOX_SPECIAL:
    case OBJ_BOX_STANDARD_RUINS:
    case OBJ_BOX_SPECIAL_RUINS:
    case OBJ_BOX_STANDARD_CCA:
    case OBJ_BOX_SPECIAL_CCA:
    case OBJ_GEE_NEST:
    case OBJ_BOX_SPECIAL_DESERT:
        if (obj->full_random == 0 && obj->random_item & FLOAT_MASK) {
            if (obj->fixed_item & FLOAT_MASK) {
                if (obj->item_param[3] == 0x4) {
                    return OBJ_FIXED_BOX_MESETA;
                } else {
                    if (obj->item_param[3] == 0 && obj->item_param[2] == 0) {
                        return OBJ_RANDOM_BOX_NO_RARE;
                    } else {
                        if (obj->item_param[3] < 0x2) {
                            obj->item_param[2]++;
                        }
                        return OBJ_FIXED_BOX_EXACT + (LE32(obj->item_param));
                    }
                }
            } else {
                switch (obj->item_param[3]) {
                case 0x0:
                    return OBJ_FIXED_BOX_WEAPON;
                case 0x1:
                    return OBJ_FIXED_BOX_ARMOR;
                case 0x2:
                    return OBJ_FIXED_BOX_MAG;
                case 0x3:
                    return obj->item_param[2] == 0x2 ? OBJ_FIXED_BOX_TECH : OBJ_FIXED_BOX_TOOL;
                case 0x4:
                    return OBJ_FIXED_BOX_MESETA;
                default:
                    return OBJ_UNKNOWN;
                }
            }
        }
        return OBJ_RANDOM_BOX;
    default:
        return OBJ_IGNORE;
    }
} 

#define MAX_BOX_NAME_LEN 47

const char* get_obj_name(obj_kind kind) {
    if (kind >= OBJ_FIXED_BOX_EXACT) {
        int item_param = (int) kind - OBJ_FIXED_BOX_EXACT;
        int item_id = item_param & ~0xff;
        int item_grp = (item_param >> 24) & 0xff;
        int item_grp2 = (item_param >> 16) & 0xff;
        int item_arg = item_param & 0xff;
        for (int i = 0; i < NUM_ITEMS; i++) {
            item_t item = items[i];
            if (item.id == item_id) {
                char *str = malloc(MAX_BOX_NAME_LEN);
                if (item_grp == 0x1 && item_grp2 == 0x1) {
                    // armor
                    snprintf(str, MAX_BOX_NAME_LEN, "Box (%s [%ds])", item.name, item_arg);
                } else if (item_grp == 0x3 && item_grp2 == 0x2) {
                    // tech
                    snprintf(str, MAX_BOX_NAME_LEN, "Box (%s Lv.%d)", tech_names[item_arg], item_grp);
                } else {
                    snprintf(str, MAX_BOX_NAME_LEN, "Box (%s)", item.name);
                }
                return str;
            }
        }
        char *str = malloc(MAX_BOX_NAME_LEN);
        snprintf(str, MAX_BOX_NAME_LEN, "Box (%08x)", item_id);
        return str;
    } else {
        switch (kind) {
        case OBJ_RANDOM_BOX:
            return "Box";
        case OBJ_RANDOM_BOX_NO_RARE:
            return "Box (Rareless)";
        case OBJ_FIXED_BOX_WEAPON:
            return "Box (Weapon)";
        case OBJ_FIXED_BOX_ARMOR:
            return "Box (Armor)";
        case OBJ_FIXED_BOX_MAG:
            return "Box (Mag)";
        case OBJ_FIXED_BOX_TOOL:
            return "Box (Tool)";
        case OBJ_FIXED_BOX_TECH:
            return "Box (Tech)";
        case OBJ_FIXED_BOX_MESETA:
            return "Box (Meseta)";
        default:
            return "Unknown";
        }
    }
}
