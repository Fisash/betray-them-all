#ifndef SQUAD_H
#define SQUAD_H

#include <stdint.h>
#include "core/state/unit.h"
#include "core/state/items.h"

#define SQUAD_START_GOLD 100
#define SQUAD_START_PROVISION 75

#define SQUAD_MAX_UNITS 10
#define SQUAD_MAX_ITEMS 64

enum move_order {
    MOVE_NONE,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN
};

struct squad {
    uint16_t pos_x;
    uint16_t pos_y;
    enum move_order move_order;

    uint16_t gold;
    uint16_t provision;
    struct unit units[SQUAD_MAX_UNITS];
    struct item inventory[SQUAD_MAX_ITEMS];

    const struct item_info *items_info;
};

enum squad_unit_equip_status {
    SQUAD_UNIT_EQUIP_OK,
    SQUAD_UNIT_EQUIP_INVALID_UNIT,
    SQUAD_UNIT_EQUIP_INVALID_ITEM
};

enum squad_unit_unequip_status {
    SQUAD_UNIT_UNEQUIP_OK,
    SQUAD_UNIT_UNEQUIP_INVALID_UNIT,
    SQUAD_UNIT_UNEQUIP_INVALID_TYPE,
    SQUAD_UNIT_UNEQUIP_ALREADY_REMOVED
};

void squad_init(struct squad *squad, uint16_t x, uint16_t y, 
                       const struct item_info items_info[]);

int squad_add_item(struct squad *squad, struct item *item);

struct item *squad_get_item_by_num(struct squad *squad, uint8_t num);

struct unit *squad_get_unit_by_num(struct squad *squad, uint8_t num);

int squad_add_unit(struct squad *squad, struct unit *unit);

void squad_consume_day_provision(struct squad *squad);

enum squad_unit_equip_status 
    squad_unit_equip(struct squad *squad, uint8_t unit_num, struct item *item);

enum squad_unit_unequip_status
    squad_unit_unequip(struct squad *squad, uint8_t unit_num, enum item_type type);

#endif
