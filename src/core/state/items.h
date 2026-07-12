#ifndef ITEMS_H
#define ITEMS_H

#include <stdint.h>
#include "core/defs/items_info.h"

typedef struct {
    uint16_t damage;
    uint16_t crit;
} weapon_props_t;

typedef struct {
    uint16_t protection;
    uint16_t mobility;
} armor_props_t;

typedef struct {
    item_id id;
    uint16_t cost;
    union {
        weapon_props_t weapon;
        armor_props_t armor;
    } props;
} item_t;

uint16_t get_between_value(uint16_t min, uint16_t max);

void item_init(item_t *item, const item_info_t info[], item_id id);

#endif
