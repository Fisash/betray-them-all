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

shop_transaction_status_t shop_system_try_buy_item_by_squad(shop_t *shop, 
                                     uint8_t slot_index, squad_t *squad);

#endif
