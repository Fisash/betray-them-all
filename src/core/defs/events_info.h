#ifndef EVENTS_INFO_H
#define EVENTS_INFO_H

#include <stdint.h>
#include "core/defs/event.h"
#include "core/defs/cell_info.h"

/*event tags (number of bit for tags bit mask*/
#define TAG_MEADOW 1
#define TAG_FOREST 2
#define TAG_MOUNTAIN 3

typedef struct {
    event_t events[EVENT_COUNT]; 
    uint16_t cell_type_weight_sums[CELL_TYPE_COUNT];
} events_info_t;

void events_info_load(events_info_t *load_ptr);

#endif
