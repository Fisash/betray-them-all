#ifndef SKILL_H
#define SKILL_H

#include <stdint.h>
#include "core/defs/unit_stats.h"
#include "core/defs/items_info.h"

#define SKILL_NAME_BUF_SIZE 32
#define SKILL_DES_BUF_SIZE 64

#define ROOT_SKILL NULL
#define NATURE_WEAPON_GROUP 0

typedef enum {
    SKILL_TARGET_NONE,
    SKILL_TARGET_SELF,
    SKILL_TARGET_ENEMY_UNIT,
    SKILL_TARGET_ALLY_UNIT,
    SKILL_TARGET_ANY_UNIT,
    SKILL_TARGET_ALL_ENEMIES,
    SKILL_TARGET_ALL_ALLIES
} skill_target_class_t;

typedef enum {
    SKILL_SKIP_TURN,
    SKILL_USUAL_SLASH,
    SKILL_USUAL_STABBING,
    SKILL_MAUL,
    SKILL_COUNT 
} skill_id;

typedef struct skill_s skill_t; 
struct skill_s {
    char name[SKILL_NAME_BUF_SIZE];
    char description[SKILL_DES_BUF_SIZE];

    skill_target_class_t target_class;

    unit_stats_t req_stats;
    skill_t *req_learned;

    uint16_t action_points_cost;
    uint64_t weapon_group_mask;
    uint64_t unit_template_tags_group_mask;
};

int skill_is_target_class_require_choice(skill_target_class_t t);

void skills_load(skill_t skills[]);

int skill_is_compatible_with_weapon(const skill_t *skill, 
                                const item_id weapon_id);

int skill_is_compatible_with_unit_template(const skill_t *skill,
                                   const uint64_t template_tags); 

#endif
