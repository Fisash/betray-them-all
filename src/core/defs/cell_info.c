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
    cell_info_init(&cells[CELL_TYPE_NONE], "Void", 
                         "void cell description");
    cell_info_init(&cells[CELL_TYPE_MEADOW], "Meadow", 
                                       "looks empty");
    cell_info_init(&cells[CELL_TYPE_FOREST], "Forest", 
                         "wild and dangerous forest");
    cell_info_init(&cells[CELL_TYPE_MOUNTAIN], "Mountains", 
                               "high and majestic cliffs");
    cell_info_init(&cells[CELL_TYPE_VILLAGE], "Village", 
                               "there are people here");
}
