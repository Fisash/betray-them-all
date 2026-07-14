#ifndef ITEM_STORAGE_H
#define ITEM_STORAGE_H

#include <stddef.h>

#include "core/state/items.h"

int item_storage_add(item_t storage[], size_t capacity, item_t *item);

int item_storage_add_by_item_id(item_t storage[], size_t capacity, 
                             const item_info_t info[], item_id id);

item_t *item_storage_get_item(item_t storage[], size_t capacity,   
                                           uint16_t real_index);
#endif
