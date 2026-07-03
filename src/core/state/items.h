#ifndef ITEMS_H
#define ITEMS_H

#include <stdint.h>
#include "core/defs/items_info.h"

typedef struct {
    uint16_t damage;
} weapon_props_t;

typedef struct {
    uint16_t protection;
} armor_props_t;

typedef struct {
    item_id id;
    union {
        weapon_props_t weapon;
        armor_props_t armor;
    } props;
} item_t;

void item_init(item_t *item, const item_info_t info[], item_id id);

#endif
