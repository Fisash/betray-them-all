#include "core/shop_system.h"
#include "core/state/item_storage.h"

shop_transaction_status_t shop_system_try_buy_item(shop_t *shop, 
                                   item_t *item, squad_t *squad)
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

shop_transaction_status_t shop_system_try_sell_item(shop_t *shop, 
                                    item_t *item, squad_t *squad)
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
