#ifndef ITEMS_INFO_H
#define ITEMS_INFO_H

#include <stdint.h>

#define ITEM_TITLE_BUF 64
#define ITEM_DESCRIPTION_BUF 128

typedef enum {
    ITEM_TYPE_GENERIC,
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOR
} item_type_t;

typedef struct {
    uint16_t damage;
} weapon_info_props_t;

typedef struct {
    uint16_t protection;
} armor_info_props_t;

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

    ARMOR_FABRIC,
    ARMOR_LEATHER,
    ARMOR_IRON,
    ARMOR_SHELL,

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

void items_info_load(item_info_t items[]);

#endif
