#ifndef GAME_INFO_H
#define GAME_INFO_H

#include "core/defs/cell_info.h"
#include "core/defs/events_info.h"
#include "core/defs/items_info.h"
#include "core/defs/unit_templates.h"
#include "core/defs/skill.h"

typedef struct {
    cell_info_t cells_info[CELL_TYPE_COUNT];
    events_info_t events_info;
    item_info_t items[ALL_ITEMS_COUNT];
    unit_template_t unit_templates[UNIT_TEMP_COUNT];
    skill_t skills[SKILL_COUNT];
} game_info_t;

void game_info_load(game_info_t *game_info);

#endif
