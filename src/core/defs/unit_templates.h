#ifndef UNIT_TEMPLATES_H
#define UNIT_TEMPLATES_H

#include <stdint.h>
#include "core/defs/items_info.h"
#include "core/defs/skills_mask.h"
#include "core/defs/unit_stats.h"

#define UNIT_TEMP_BUF_SIZE 32

#define UNIT_TAG_HUMAN 1
#define UNIT_TAG_ANIMAL 2
#define UNIT_ALL_TAGS_MASK UINT64_MAX

typedef enum {
    WARRIOR,
    ROGUE,
 
    WOLF,
    UNIT_TEMP_COUNT
} unit_template_id;

typedef struct {
    char name[UNIT_TEMP_BUF_SIZE];
    uint16_t hp_min, hp_max;

    unit_stats_t min_stats_value;
    unit_stats_t max_stats_value;

    uint64_t tags_mask;

    item_id armor_item_id;
    item_id weapon_item_id;

    skills_mask_t initial_learned;
} unit_template_t;

void unit_templates_load(unit_template_t templates[]);

#endif
