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

typedef struct {
    shop_t shop;
} village_t;

typedef struct {
    cell_type_id_t type_id; 
    uint8_t days_until_update;
    uint8_t data_index;
} cell_t;

typedef struct {
    cell_t cells[WORLD_HEIGHT][WORLD_WIDTH]; 
    cell_t *event_cd_cells[CELL_LIST_SIZE];

    village_t villages[VILLAGE_MAX_COUNT];
    uint8_t village_count;
} world_t;

void world_cell_list_add(cell_t** list, cell_t *cell);

void world_generate(world_t *world, const item_info_t items[]);

void world_fill_cells_id_buffer(world_t *world, char *buffer);

#endif
