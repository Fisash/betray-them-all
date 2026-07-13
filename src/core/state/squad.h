#ifndef SQUAD_H
#define SQUAD_H

#include <stdint.h>
#include "core/state/unit.h"
#include "core/state/items.h"

#define SQUAD_START_GOLD 100
#define SQUAD_START_PROVISION 75

#define SQUAD_MAX_UNITS 10
#define SQUAD_MAX_ITEMS 64

typedef enum {
    MOVE_NONE,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN
} move_order_t;

typedef struct {
    uint16_t pos_x;
    uint16_t pos_y;
    move_order_t move_order;

    uint16_t gold;
    uint16_t provision;
    unit_t units[SQUAD_MAX_UNITS];
    item_t inventory[SQUAD_MAX_ITEMS];

    const item_info_t *items_info;
} squad_t;

void squad_init(squad_t *squad, uint16_t x, uint16_t y, 
                       const item_info_t items_info[]);

int squad_add_item(squad_t *squad, item_t *item);

item_t *squad_get_item_by_num(squad_t *squad, uint8_t num);

unit_t *squad_get_unit_by_num(squad_t *squad, uint8_t num);

int squad_add_unit(squad_t *squad, unit_t *unit);

void squad_consume_day_provision(squad_t *squad);

#endif
