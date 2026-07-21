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

enum unit_template_id {
    WARRIOR,
    ROGUE,
 
    WOLF,
    UNIT_TEMP_COUNT
};

struct unit_template {
    char name[UNIT_TEMP_BUF_SIZE];
    uint16_t hp_min, hp_max;

    struct unit_stats min_stats_value;
    struct unit_stats max_stats_value;

    uint64_t tags_mask;

    enum item_id armor_item_id;
    enum item_id weapon_item_id;

    uint16_t base_exp_reward;

    skills_mask_t initial_learned;
};

void unit_templates_load(struct unit_template templates[]);

#endif
