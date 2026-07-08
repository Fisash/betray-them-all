#include <stdlib.h>

#include "core/state/items.h"

uint16_t get_between_value(uint16_t min, uint16_t max)
{
    return min + rand() % (max - min + 1);
}

void item_init(item_t *item, const item_info_t info[], item_id id)
{
    const item_info_t *item_info = &info[id];
    item->id = id;

    switch(item_info->type)
    {
        case ITEM_TYPE_WEAPON:
            item->props.weapon.damage = 
                get_between_value(item_info->props.weapon.damage.min, 
                                  item_info->props.weapon.damage.max);
            item->props.weapon.crit = 
                get_between_value(item_info->props.weapon.crit.min, 
                                  item_info->props.weapon.crit.max);
            break;
        case ITEM_TYPE_ARMOR:
            item->props.armor.protection = 
                get_between_value(item_info->props.armor.protection.min,
                                  item_info->props.armor.protection.max);
            item->props.armor.mobility = 
                get_between_value(item_info->props.armor.mobility.min,
                                  item_info->props.armor.mobility.max);
            break;
        case ITEM_TYPE_GENERIC:
        default:
            break;
    }
}
