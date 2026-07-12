#ifndef EVENTS_INFO_H
#define EVENTS_INFO_H

#include <stdint.h>
#include "core/defs/event.h"
#include "core/defs/cell_info.h"

/*event tags (number of bit for tags bit mask*/
#define TAG_MEADOW CELL_TYPE_MEADOW
#define TAG_FOREST CELL_TYPE_FOREST
#define TAG_MOUNTAIN CELL_TYPE_MOUNTAIN
#define TAG_VILLAGE CELL_TYPE_VILLAGE

typedef struct {
    event_t events[EVENT_COUNT]; 
    uint16_t cell_type_weight_sums[CELL_TYPE_COUNT];
} events_info_t;

void events_info_load(events_info_t *load_ptr);

#endif
