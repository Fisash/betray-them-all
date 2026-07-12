#ifndef ITEM_STORAGE_H
#define ITEM_STORAGE_H

#include <stddef.h>

#include "core/state/items.h"

int item_storage_add(item_t storage[], size_t capacity, item_t *item);

#endif
