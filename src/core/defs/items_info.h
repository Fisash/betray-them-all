#ifndef ITEMS_INFO_H
#define ITEMS_INFO_H

#include <stdint.h>
#include "core/defs/unit_stats.h"

#define ITEM_TITLE_BUF 64
#define ITEM_DESCRIPTION_BUF 128

#define MAX_STAT_VALUE 100

typedef enum {
    SCALE_N = 0,
    SCALE_E,
    SCALE_D,
    SCALE_C,
    SCALE_B,
    SCALE_A,
    SCALE_S
} scale_rank_t;

typedef struct {
    scale_rank_t strength;
    scale_rank_t agility;
    scale_rank_t will;
    scale_rank_t intelligence;
} scaling_group_t;

typedef struct {
    uint16_t min;
    uint16_t max;
    scaling_group_t scalings;
} param_spec_t;

typedef struct {
    param_spec_t damage;
    param_spec_t crit;
} weapon_info_props_t;

typedef struct {
    param_spec_t protection;
    param_spec_t mobility;
} armor_info_props_t;

typedef enum {
    ITEM_TYPE_GENERIC,
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOR
} item_type_t;

typedef struct {
    item_type_t type;
    char title[ITEM_TITLE_BUF];
    char description[ITEM_DESCRIPTION_BUF]; 
    union {
        weapon_info_props_t weapon;
        armor_info_props_t armor;
    } props;
} item_info_t;

typedef enum {
    ITEM_NONE = 0,

    ARMOR_NUDE,    /*nature armor*/
    ARMOR_FABRIC,
    ARMOR_LEATHER,
    ARMOR_IRON,
    ARMOR_SHELL,

    WEAPON_FISTS,  /* nature human weapon*/
    WEAPON_CLAW,   /* nature animal weapon*/

    WEAPON_SHORT_SWORD,
    WEAPON_SWORD,
    WEAPON_DAGGER,
    WEAPON_MACE,
    WEAPON_HUMMER,
    WEAPON_SPEAR,
    WEAPON_HALBERD,

    ITEM_MALACHITE,
    ITEM_AMETHYST,
    ITEM_RUBY,

    ALL_ITEMS_COUNT

} item_id;

uint16_t scale_param(uint16_t source, scaling_group_t scalings, 
                                           unit_stats_t stats);

void items_info_load(item_info_t items[]);

#endif
