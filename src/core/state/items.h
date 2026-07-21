#ifndef ITEMS_H
#define ITEMS_H

#include <stdint.h>
#include "core/defs/items_info.h"

struct weapon_props {
    uint16_t damage;
    uint16_t crit;
};

struct armor_props {
    uint16_t protection;
    uint16_t mobility;
};

struct item {
    enum item_id id;
    uint16_t cost;
    union {
        struct weapon_props weapon;
        struct armor_props armor;
    } props;
};

uint16_t get_between_value(uint16_t min, uint16_t max);

void item_init(struct item *item, const struct item_info info[], enum item_id id);

#endif
