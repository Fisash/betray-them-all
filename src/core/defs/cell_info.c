#include <string.h>

#include "core/defs/cell_info.h"

static void cell_info_init(cell_info_t *cell_info,
                          const char *title, const char *description)
{
    strcpy(cell_info->title, title);
    strcpy(cell_info->description, description);
}

void cells_info_load(cell_info_t cells[])
{
    /* uses CELLS TYPE COUNT iterations*/
    int i = 0;
    cell_info_init(&cells[i++], "VOID", "void cell description");
    cell_info_init(&cells[i++], "Meadow", "looks empty");
    cell_info_init(&cells[i++], "Forest", "wild and dangerous forest");
    cell_info_init(&cells[i++], "Mountains", "high and majestic cliffs");
}

const cell_info_t *get_cell_info(cell_t *cell, 
               const cell_info_t *info_array)
{
    return &info_array[cell->type_id];
}
