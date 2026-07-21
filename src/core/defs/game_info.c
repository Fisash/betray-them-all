#include "core/defs/game_info.h"

void game_info_load(struct game_info *game_info)
{
    events_info_load(&game_info->events_info);
    cells_info_load(game_info->cells_info); 
    items_info_load(game_info->items);
    unit_templates_load(game_info->unit_templates);
    skills_load(game_info->skills);
}
