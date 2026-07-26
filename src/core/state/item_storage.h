#ifndef ITEM_STORAGE_H
#define ITEM_STORAGE_H

#include <stddef.h>

#include "core/state/items.h"

int item_storage_add(struct item storage[], size_t capacity, struct item *item);

int item_storage_add_by_item_id(struct item storage[], size_t capacity, 
                       const struct item_info info[], enum item_id id);

struct item *item_storage_get_item(struct item storage[], size_t capacity,   
                                                     uint16_t real_index);
#endif
