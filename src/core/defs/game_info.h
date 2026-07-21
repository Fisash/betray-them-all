#ifndef GAME_INFO_H
#define GAME_INFO_H

#include "core/defs/cell_info.h"
#include "core/defs/events_info.h"
#include "core/defs/items_info.h"
#include "core/defs/unit_templates.h"
#include "core/defs/skill.h"

struct game_info {
    struct cell_info cells_info[CELL_TYPE_COUNT];
    struct events_info events_info;
    struct item_info items[ALL_ITEMS_COUNT];
    struct unit_template unit_templates[UNIT_TEMP_COUNT];
    struct skill skills[SKILL_COUNT];
};

void game_info_load(struct game_info *game_info);

#endif
