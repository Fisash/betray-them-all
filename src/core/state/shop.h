#ifndef SHOP_H
#define SHOP_H

#include "core/state/items.h"

#define SHOP_MAX_ITEMS_COUNT 64

struct shop {
    struct item items[SHOP_MAX_ITEMS_COUNT];
    uint16_t gold;
    float sell_multiplier;
    float buy_multiplier;
};

void shop_init(struct shop *shop, uint16_t gold, float sell_multiplier,
                                             float buy_multiplier);

struct item *shop_get_item_by_num(struct shop *shop, uint8_t num);

uint16_t shop_get_sell_price(struct shop *shop, struct item *item);

uint16_t shop_get_buy_price(struct shop *shop, struct item *item);

void shop_generate_village_items(struct shop *shop, const struct item_info info[]);

#endif
