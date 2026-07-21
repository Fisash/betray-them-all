#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>
#include "core/defs/cell_info.h"
#include "core/state/items.h"
#include "core/state/shop.h"

#define WORLD_WIDTH 54
#define WORLD_HEIGHT 23

#define CELL_LIST_SIZE 128

#define VILLAGE_MAX_COUNT 8

struct village {
    struct shop shop;
};

struct cell {
    enum cell_type_id type_id; 
    uint8_t days_until_update;
    uint8_t data_index;
};

struct world {
    struct cell cells[WORLD_HEIGHT][WORLD_WIDTH]; 
    struct cell *event_cd_cells[CELL_LIST_SIZE];

    struct village villages[VILLAGE_MAX_COUNT];
    uint8_t village_count;
};

void world_cell_list_add(struct cell** list, struct cell *cell);

void world_generate(struct world *world, const struct item_info items[]);

void world_fill_cells_id_buffer(struct world *world, char *buffer);

#endif
