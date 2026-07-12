#ifndef SHOP_H
#define SHOP_H

#include "core/state/items.h"

#define SHOP_MAX_ITEMS_COUNT 64

typedef struct {
    item_t items[SHOP_MAX_ITEMS_COUNT];
    uint16_t gold;
    float sell_multiplier;
    float buy_multiplier;
} shop_t;

void shop_init(shop_t *shop, uint16_t gold, 
                     float sell_multiplier,
                     float buy_multiplier);

item_t *shop_get_item_by_num(shop_t *shop, uint8_t num);

uint16_t shop_get_sell_price(shop_t *shop, item_t *item);

uint16_t shop_get_buy_price(shop_t *shop, item_t *item);

void shop_generate_village_items(shop_t *shop, const item_info_t info[]);

#endif
