#ifndef SHOP_SYSTEM_H
#define SHOP_SYSTEM_H

#include "core/state/shop.h"
#include "core/state/squad.h"

typedef enum {
    SHOP_TRANSACTION_OK,
    SHOP_TRANSACTION_INCORRECT_ITEM,
    SHOP_TRANSACTION_INSUFFCIENT_GOLD,
    SHOP_TRANSACTION_INSUFFCIENT_SPACE
} shop_transaction_status_t;

shop_transaction_status_t shop_system_try_buy_item(shop_t *shop, 
                                   item_t *item, squad_t *squad);

shop_transaction_status_t shop_system_try_sell_item(shop_t *shop, 
                                    item_t *item, squad_t *squad);

#endif
