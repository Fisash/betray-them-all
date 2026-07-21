#include "core/shop_system.h"
#include "core/state/item_storage.h"

enum shop_transaction_status
shop_system_try_buy_item(struct shop *shop, struct item *item, struct squad *squad)
{
    uint16_t price;
    int adding_status;

    if(!item || item->id == ITEM_NONE)
        return SHOP_TRANSACTION_INCORRECT_ITEM;
     
    price = shop_get_sell_price(shop, item);
    if(squad->gold < price)
        return SHOP_TRANSACTION_INSUFFCIENT_GOLD;

    adding_status = squad_add_item(squad, item);
    if(adding_status != 0)
        return SHOP_TRANSACTION_INSUFFCIENT_SPACE;

    squad->gold -= price;
    shop->gold += price;
    item->id = ITEM_NONE;
    return SHOP_TRANSACTION_OK;    
}

enum shop_transaction_status
shop_system_try_sell_item(struct shop *shop, struct item *item, struct squad *squad)
{
    uint16_t price;
    int adding_status;

    if(!item || item->id == ITEM_NONE)
        return SHOP_TRANSACTION_INCORRECT_ITEM;
     
    price = shop_get_buy_price(shop, item);
    if(shop->gold < price)
        return SHOP_TRANSACTION_INSUFFCIENT_GOLD;

    adding_status = item_storage_add(shop->items, 
                     SHOP_MAX_ITEMS_COUNT, item); 
    if(adding_status != 0)
        return SHOP_TRANSACTION_INSUFFCIENT_SPACE;

    squad->gold += price;
    shop->gold -= price;
    item->id = ITEM_NONE;
    return SHOP_TRANSACTION_OK;    
}
