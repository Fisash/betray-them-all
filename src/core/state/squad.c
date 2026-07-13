#include <stdlib.h>

#include "core/state/squad.h"
#include "core/state/item_storage.h"

#include <string.h>

void squad_init(squad_t *squad, uint16_t x, uint16_t y, 
                        const item_info_t items_info[])
{
    squad->pos_x = x;
    squad->pos_y = y;
    squad->gold = SQUAD_START_GOLD;
    squad->provision = SQUAD_START_PROVISION;
    squad->move_order = MOVE_NONE;

    memset(&squad->units, 0, sizeof(unit_t)*SQUAD_MAX_UNITS);
    memset(&squad->inventory, 0, sizeof(item_t)*SQUAD_MAX_ITEMS);

    squad->items_info = items_info;
}

int squad_add_item(squad_t *squad, item_t *item)
{
    const item_info_t *info = &(squad->items_info[item->id]);
    if(info->type == ITEM_TYPE_PROVISION)
    {
        squad->provision += info->props.provision_increase_value;
        return 0;
    }

    return item_storage_add(squad->inventory, SQUAD_MAX_ITEMS, item);
}

item_t *squad_get_item_by_num(squad_t *squad, uint8_t num)
{
    return item_storage_get_item(squad->inventory, SQUAD_MAX_ITEMS, (num-1));
}

unit_t *squad_get_unit_by_num(squad_t *squad, uint8_t num)
{
    unit_t *unit;
    uint8_t i, current;
    for(i = 0, current = 0; i < SQUAD_MAX_UNITS; i++)
    {
        unit = &squad->units[i];
        if(!unit->is_alive)
            continue;
        if(++current == num)
            return unit;
    }

    return NULL;
}

int squad_add_unit(squad_t *squad, unit_t *unit)
{
    if(!unit->is_alive)
        return 1;

    int i;
    for(i = 0; i < SQUAD_MAX_UNITS; i++)
    {
        if(!squad->units[i].is_alive)
        {
            squad->units[i] = *unit;
            return 0;
        }
    }
    return 1;
}

void squad_consume_day_provision(squad_t *squad)
{
    unit_t *unit;
    int i;
    for(i = 0; i < SQUAD_MAX_UNITS; i++)
    {
        if(squad->provision <= 0)
            break;

        unit = &squad->units[i];
        if(!unit->is_alive)
            continue;

        squad->provision--;
        if(unit->hp < unit->max_hp)
            unit->hp++;
    }
}
