#ifndef SQUAD_H
#define SQUAD_H

#include <stdint.h>
#include "core/state/unit.h"
#include "core/state/items.h"

#define MAX_UNITS 10
#define MAX_ITEMS 64

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
    unit_t units[MAX_UNITS];
    item_t inventory[MAX_ITEMS];

} squad_t;

void squad_init(squad_t *squad, uint16_t x, uint16_t y, uint16_t gold);

int squad_add_item(squad_t *sqiad, item_t *item);
item_t *squad_get_item_by_num(squad_t *squad, uint8_t num);

int squad_add_unit(squad_t *squad, unit_t *unit);

#endif
