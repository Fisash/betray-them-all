#include <string.h>

#include "core/defs/cell_info.h"

static void cell_info_init(cell_info_t *cell_info, const char *title, 
                       const char *description, uint8_t exploring_cd)
{
    strcpy(cell_info->title, title);
    strcpy(cell_info->description, description);
    cell_info->exploring_cd = exploring_cd;
}

void cells_info_load(cell_info_t cells[])
{
    cell_info_init(&cells[CELL_TYPE_NONE], "Void", 
                      "void cell description", 0);
    cell_info_init(&cells[CELL_TYPE_MEADOW], "Meadow", 
                                   "looks empty", 30);
    cell_info_init(&cells[CELL_TYPE_FOREST], "Forest", 
                     "wild and dangerous forest", 30);
    cell_info_init(&cells[CELL_TYPE_MOUNTAIN], "Mountains", 
                           "high and majestic cliffs", 30);
    cell_info_init(&cells[CELL_TYPE_VILLAGE], "Village", 
                            "there are people here", 0);
}
