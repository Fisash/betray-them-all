#include "core/state/item_storage.h"

int item_storage_add(item_t storage[], size_t capacity, item_t *item)
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
