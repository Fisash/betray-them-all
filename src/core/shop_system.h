#ifndef SHOP_SYSTEM_H
#define SHOP_SYSTEM_H

#include "core/state/shop.h"
#include "core/state/squad.h"

enum shop_transaction_status {
    SHOP_TRANSACTION_OK,
    SHOP_TRANSACTION_INCORRECT_ITEM,
    SHOP_TRANSACTION_INSUFFCIENT_GOLD,
    SHOP_TRANSACTION_INSUFFCIENT_SPACE
};

enum shop_transaction_status
shop_system_try_buy_item(struct shop *shop, struct item *item, struct squad *squad);

enum shop_transaction_status
shop_system_try_sell_item(struct shop *shop, struct item *item, struct squad *squad);

#endif
