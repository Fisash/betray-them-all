#include <string.h>
#include <stdlib.h>

#include "core/state/shop.h"
#include "core/state/item_storage.h"

void shop_init(struct shop *shop, uint16_t gold, 
                          float sell_multiplier,
                           float buy_multiplier)
{
    memset(&shop->items, 0, sizeof(struct item)*SHOP_MAX_ITEMS_COUNT);
    shop->gold = gold;
    shop->sell_multiplier = sell_multiplier;
    shop->buy_multiplier = buy_multiplier;
}

struct item *shop_get_item_by_num(struct shop *shop, uint16_t num)
{
    return item_storage_get_item(shop->items, 
              SHOP_MAX_ITEMS_COUNT, (num-1));
}

uint16_t shop_get_sell_price(struct shop *shop, struct item *item)
{
    return (uint16_t)(item->cost * shop->sell_multiplier);
}

uint16_t shop_get_buy_price(struct shop *shop, struct item *item)
{
    return (uint16_t)(item->cost * shop->buy_multiplier);
}

void shop_generate_village_items(struct shop *shop, const struct item_info info[])
{
    struct item provision;
    enum item_id id;
    int i;

    item_init(&provision, info, ITEM_PROVISION_BAG);
    for(i = 0; i < (5 + rand() % 5); i++)
        item_storage_add(shop->items, SHOP_MAX_ITEMS_COUNT, &provision);

    for(i = 0; i < (2 + rand() % 3); i++)
    {
        id = items_info_get_rand_weapon_id();
        item_storage_add_by_item_id(shop->items, 
                SHOP_MAX_ITEMS_COUNT, info, id);
    }

    for(i = 0; i < (2 + rand() % 3); i++)
    {
        id = items_info_get_rand_armor_id();
        item_storage_add_by_item_id(shop->items, 
                SHOP_MAX_ITEMS_COUNT, info, id);
    }
}
