#include <stdlib.h>

#include "core/state/squad.h"
#include "core/state/item_storage.h"

#include <string.h>

void squad_init(squad_t *squad, uint16_t x, uint16_t y, uint16_t gold)
{
    squad->pos_x = x;
    squad->pos_y = y;
    squad->gold = gold;
    squad->move_order = MOVE_NONE;

    memset(&squad->units, 0, sizeof(unit_t)*MAX_UNITS);
    memset(&squad->inventory, 0, sizeof(item_t)*MAX_ITEMS);
}

int squad_add_item(squad_t *squad, item_t *item)
{
    return item_storage_add(squad->inventory, MAX_ITEMS, item);
}

int squad_add_unit(squad_t *squad, unit_t *unit)
{
    if(!unit->is_alive)
        return 1;

    int i;
    for(i = 0; i < MAX_UNITS; i++)
    {
        if(!squad->units[i].is_alive)
        {
            squad->units[i] = *unit;
            return 0;
        }
    }
    return 1;
}

