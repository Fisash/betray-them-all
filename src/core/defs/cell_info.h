#ifndef CELL_INFO_H
#define CELL_INFO_H

#include "core/state/world.h"

#define CELLS_TYPE_COUNT 4

#define CELL_INFO_NAME_BUF_SIZE 32
#define CELL_INFO_DESCRIPTION_BUF_SIZE 128

typedef struct {
    char title[CELL_INFO_NAME_BUF_SIZE];
    char description[CELL_INFO_DESCRIPTION_BUF_SIZE];
} cell_info_t;

void cells_info_load(cell_info_t cells[]);

const cell_info_t *get_cell_info(cell_t *cell, 
               const cell_info_t *info_array);

#endif
