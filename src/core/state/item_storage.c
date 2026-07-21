#include "core/state/item_storage.h"

int item_storage_add(struct item storage[], size_t capacity, struct item *item)
{
    if(item->id == ITEM_NONE)
        return 1;

    size_t i;
    for(i = 0; i < capacity; i++)
    {
        if(storage[i].id == ITEM_NONE)
        {
            storage[i] = *item;
            return 0;
        }
    }
    return 1;
}

int item_storage_add_by_item_id(struct item storage[], size_t capacity, 
                             const struct item_info info[], enum item_id id)
{
    struct item item;
    item_init(&item, info, id);
    return item_storage_add(storage, capacity, &item);
}

struct item *item_storage_get_item(struct item storage[], size_t capacity,   
                                            uint16_t real_index)
{
    size_t i;
    uint16_t current;
    for(i = 0, current = 0; i < capacity; i++)
        if(storage[i].id != ITEM_NONE)
            if(current++ == real_index)
                return &storage[i];

    return NULL;
}
