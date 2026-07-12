#include <string.h>
#include <stdlib.h>

#include "core/state/shop.h"
#include "core/state/item_storage.h"

void shop_init(shop_t *shop, uint16_t gold, 
            float selling_price_multiplier,
            float buying_price_multiplier)
{
    memset(&shop->items, 0, sizeof(item_t)*SHOP_MAX_ITEMS_COUNT);
    shop->gold = gold;
    shop->selling_price_multiplier = selling_price_multiplier;
    shop->buying_price_multiplier = buying_price_multiplier;
}

uint16_t shop_get_buy_price(shop_t *shop, item_t *item)
{
    return (uint16_t)(item->cost * shop->selling_price_multiplier);
}

void shop_generate_village_items(shop_t *shop, const item_info_t info[])
{
    item_t provision;
    item_init(&provision, info, ITEM_PROVISION_BAG);

    int i;
    for(i = 0; i < (5 + rand() % 5); i++)
        item_storage_add(shop->items, SHOP_MAX_ITEMS_COUNT, &provision);
}
