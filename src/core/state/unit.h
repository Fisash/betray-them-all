#ifndef UNIT_H
#define UNIT_H

#include <stdint.h>
#include "core/state/items.h"
#include "core/defs/unit_stats.h"
#include "core/defs/unit_templates.h"

#define UNIT_NAME_BUF_SIZE 32

struct unit {
    uint8_t is_alive;
    char name[UNIT_NAME_BUF_SIZE];
    enum unit_template_id template_id;

    uint16_t hp, max_hp;
    struct unit_stats stats;
    struct item weapon;
    struct item armor;

    uint16_t exp, exp_for_next_level;
    uint8_t level;
    uint16_t unspent_stat_points;

    uint16_t exp_reward;

    skills_mask_t learned;
};


void unit_init(struct unit *unit, const char *name, enum unit_template_id id, 
                                      const struct unit_template templates[],
                                        const struct item_info items_info[]);

uint16_t unit_get_damage(struct unit *unit, const struct item_info items[]);
uint16_t unit_get_crit(struct unit *unit, const struct item_info items[]);
uint16_t unit_get_protection(struct unit *unit, const struct item_info items[]);
uint16_t unit_get_mobility(struct unit *unit, const struct item_info items[]);

void unit_fill_available_skills(skills_mask_t out, const struct unit *unit,
                                          const struct skill skills[],
                                    const struct unit_template templates[]);

void unit_add_exp(struct unit *unit, uint16_t exp);

int unit_apply_stat_point(struct unit *unit, enum stat_selection s);

#endif
