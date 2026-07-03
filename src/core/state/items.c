#include "core/state/items.h"

void item_init(item_t *item, const item_info_t info[], item_id id)
{
    const item_info_t *item_info = &info[id];
    item->id = id;

    switch(item_info->type)
    {
        case ITEM_TYPE_WEAPON:
            item->props.weapon.damage = 
                item_info->props.weapon.damage;
            break;
        case ITEM_TYPE_ARMOR:
            item->props.armor.protection = 
                item_info->props.armor.protection;
            break;
        case ITEM_TYPE_GENERIC:
        default:
            break;
    }
}
