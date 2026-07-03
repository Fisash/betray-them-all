#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

#define WORLD_WIDTH 54
#define WORLD_HEIGHT 23

#define CELL_LIST_SIZE 128

typedef struct {
    uint8_t type_id; 
    uint8_t days_until_update;
} cell_t;

void world_cell_list_add(cell_t** list, cell_t *cell);

typedef struct {
    cell_t cells[WORLD_HEIGHT][WORLD_WIDTH]; 
    cell_t *event_cd_cells[CELL_LIST_SIZE];
} world_t;

void world_generate(world_t *world);
void world_fill_cells_id_buffer(world_t *world, char *buffer);

#endif
