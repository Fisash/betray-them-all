#include <string.h>
#include <stdlib.h>

#include "core/state/shop.h"
#include "core/state/item_storage.h"

void shop_init(shop_t *shop, uint16_t gold, 
                     float sell_multiplier,
                     float buy_multiplier )
{
    memset(&shop->items, 0, sizeof(item_t)*SHOP_MAX_ITEMS_COUNT);
    shop->gold = gold;
    shop->sell_multiplier = sell_multiplier;
    shop->buy_multiplier = buy_multiplier;
}

item_t *shop_get_item_by_num(shop_t *shop, uint8_t num)
{
    return item_storage_get_item(shop->items, SHOP_MAX_ITEMS_COUNT, (num-1));
}

uint16_t shop_get_sell_price(shop_t *shop, item_t *item)
{
    return (uint16_t)(item->cost * shop->sell_multiplier);
}

uint16_t shop_get_buy_price(shop_t *shop, item_t *item)
{
    return (uint16_t)(item->cost * shop->buy_multiplier);
}

void shop_generate_village_items(shop_t *shop, const item_info_t info[])
{
    item_t provision;
    item_init(&provision, info, ITEM_PROVISION_BAG);

    int i;
    for(i = 0; i < (5 + rand() % 5); i++)
        item_storage_add(shop->items, SHOP_MAX_ITEMS_COUNT, &provision);
}
