#ifndef UNIT_H
#define UNIT_H

#include <stdint.h>
#include "core/state/items.h"
#include "core/defs/unit_stats.h"
#include "core/defs/unit_templates.h"

#define UNIT_NAME_BUF_SIZE 32

typedef struct {
    uint8_t is_alive;
    char name[UNIT_NAME_BUF_SIZE];
    unit_template_id template_id;

    uint16_t hp, max_hp;
    unit_stats_t stats;
    item_t weapon;
    item_t armor;

    uint16_t exp, exp_for_next_level;
    uint8_t level;

    skills_mask_t learned;
} unit_t;

void unit_init(unit_t *unit, const char *name, unit_template_id id, 
                                  const unit_template_t templates[],
                                  const item_info_t items_info[]);

void unit_fill_available_skills(skills_mask_t out, const unit_t *unit,
                                               const skill_t skills[],
                                    const unit_template_t templates[]);

#endif
