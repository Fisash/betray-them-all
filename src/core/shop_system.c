#include "core/shop_system.h"
#include "core/state/item_storage.h"

shop_transaction_status_t shop_system_try_buy_item_by_squad(shop_t *shop, 
                                      uint8_t slot_index, squad_t *squad)
{
    uint16_t price;
    int adding_status;

    item_t *item = &shop->items[slot_index];
    if(!item || item->id == ITEM_NONE)
        return SHOP_TRANSACTION_INCORRECT_ITEM;
     
    price = shop_get_buy_price(shop, item);
    if(squad->gold < price)
        return SHOP_TRANSACTION_INSUFFCIENT_GOLD;

    adding_status = item_storage_add(squad->inventory, MAX_ITEMS, item);
    if(adding_status != 0)
        return SHOP_TRANSACTION_INSUFFCIENT_SPACE;

    squad->gold -= price;
    item->id = ITEM_NONE;
    return SHOP_TRANSACTION_OK;    
}
