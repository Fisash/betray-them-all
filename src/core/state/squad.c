#include <stdlib.h>

#include "core/state/squad.h"
#include "core/state/item_storage.h"

#include <string.h>

void squad_init(struct squad *squad, uint16_t x, uint16_t y, 
                        const struct item_info items_info[])
{
    squad->pos_x = x;
    squad->pos_y = y;
    squad->gold = SQUAD_START_GOLD;
    squad->provision = SQUAD_START_PROVISION;
    squad->move_order = MOVE_NONE;

    memset(&squad->units, 0, sizeof(struct unit)*SQUAD_MAX_UNITS);
    memset(&squad->inventory, 0, sizeof(struct item)*SQUAD_MAX_ITEMS);

    squad->items_info = items_info;
}

int squad_add_item(struct squad *squad, struct item *item)
{
    const struct item_info *info = &(squad->items_info[item->id]);
    if(info->type == ITEM_TYPE_PROVISION)
    {
        squad->provision += info->props.provision_increase_value;
        return 0;
    }

    return item_storage_add(squad->inventory, SQUAD_MAX_ITEMS, item);
}

struct item *squad_get_item_by_num(struct squad *squad, uint16_t num)
{
    return item_storage_get_item(squad->inventory, SQUAD_MAX_ITEMS, (num-1));
}

struct unit *squad_get_unit_by_num(struct squad *squad, uint16_t num)
{
    struct unit *unit;
    uint16_t i, current;
    for(i = 0, current = 0; i < SQUAD_MAX_UNITS; i++)
    {
        unit = &squad->units[i];
        if(!unit->is_alive)
            continue;

        current++;
        if(current == num)
            return unit;
    }

    return NULL;
}

int squad_add_unit(struct squad *squad, struct unit *unit)
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

void squad_consume_day_provision(struct squad *squad)
{
    struct unit *unit;
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

enum squad_unit_equip_status 
squad_unit_equip(struct squad *squad, uint16_t unit_num, 
                                      struct item *item)
{
    struct unit *unit = squad_get_unit_by_num(squad, unit_num);
    if(!unit)
        return SQUAD_UNIT_EQUIP_INVALID_UNIT;

    struct item *equipment;
    switch(squad->items_info[item->id].type)
    {
        case ITEM_TYPE_WEAPON:
            equipment = &unit->weapon;
            break;
        case ITEM_TYPE_ARMOR:
            equipment = &unit->armor;
            break;
        default:
            return SQUAD_UNIT_EQUIP_INVALID_ITEM;
    }

    if(!items_info_is_nature_equip(equipment->id))
        squad_add_item(squad, equipment);
    *equipment = *item;
    item->id = ITEM_NONE;
    return SQUAD_UNIT_EQUIP_OK;
}

enum squad_unit_unequip_status
squad_unit_unequip(struct squad *squad, uint16_t unit_num, 
                                      enum item_type type)
{
    struct unit *unit = squad_get_unit_by_num(squad, unit_num);
    if(!unit)
        return SQUAD_UNIT_UNEQUIP_INVALID_UNIT;

    enum item_id nature_replacement;
    struct item *equipment;
    switch(type)
    {
        case ITEM_TYPE_ARMOR:
            equipment = &unit->armor;
            nature_replacement = ARMOR_NUDE;
            break;
        case ITEM_TYPE_WEAPON:
            equipment = &unit->weapon;
            nature_replacement = WEAPON_FISTS;
            break;
        default:
            return SQUAD_UNIT_UNEQUIP_INVALID_TYPE;
    }

    if(items_info_is_nature_equip(equipment->id))
        return SQUAD_UNIT_UNEQUIP_ALREADY_REMOVED;

    squad_add_item(squad, equipment);
    item_init(equipment, squad->items_info, nature_replacement);
    return SQUAD_UNIT_UNEQUIP_OK;
}
