#ifndef ITEMS_INFO_H
#define ITEMS_INFO_H

#include <stdint.h>
#include "core/defs/unit_stats.h"

#define ITEM_TITLE_BUF 64
#define ITEM_DESCRIPTION_BUF 128

#define PROVISION_INCREASE_VALUE 15

#define MAX_STAT_VALUE 100

enum scale_rank {
    SCALE_N = 0,
    SCALE_E,
    SCALE_D,
    SCALE_C,
    SCALE_B,
    SCALE_A,
    SCALE_S
};

struct scaling_group {
    enum scale_rank strength;
    enum scale_rank agility;
    enum scale_rank will;
    enum scale_rank intelligence;
};

struct param_spec {
    uint16_t min;
    uint16_t max;
    struct scaling_group scalings;
};

struct weapon_info_props {
    struct param_spec damage;
    struct param_spec crit;
};

struct armor_info_props {
    struct param_spec protection;
    struct param_spec mobility;
};

enum item_type {
    ITEM_TYPE_GENERIC,
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOR,
    ITEM_TYPE_PROVISION
};

struct item_info {
    enum item_type type;
    char title[ITEM_TITLE_BUF];
    char description[ITEM_DESCRIPTION_BUF]; 
    uint16_t cost;
    union {
        struct weapon_info_props weapon;
        struct armor_info_props armor;
        uint8_t provision_increase_value;
    } props;
};

enum item_id {
    ITEM_NONE = 0,

    ARMOR_NUDE,    /*nature armor*/

    ARMOR_REAL_FIRST,
    ARMOR_FABRIC = ARMOR_REAL_FIRST,
    ARMOR_LEATHER,
    ARMOR_IRON,
    ARMOR_SHELL,
    ARMOR_REAL_LAST = ARMOR_SHELL,

    WEAPON_FISTS,  /* nature human weapon*/
    WEAPON_CLAW,   /* nature animal weapon*/

    WEAPON_REAL_FIRST,
    WEAPON_SHORT_SWORD = WEAPON_REAL_FIRST,
    WEAPON_SWORD,
    WEAPON_DAGGER,
    WEAPON_MACE,
    WEAPON_HUMMER,
    WEAPON_SPEAR,
    WEAPON_HALBERD,
    WEAPON_REAL_LAST = WEAPON_HALBERD,

    ITEM_PROVISION_BAG,

    ITEM_MALACHITE,
    ITEM_AMETHYST,
    ITEM_RUBY,

    ALL_ITEMS_COUNT

};

int items_info_is_nature_equip(enum item_id id);

char scale_get_rank_view(enum scale_rank rank);

uint16_t scale_param(uint16_t source, struct scaling_group scalings, 
                                            struct unit_stats stats);

enum item_id items_info_get_rand_weapon_id();

enum item_id items_info_get_rand_armor_id();

void items_info_load(struct item_info items[]);

#endif
