#ifndef SKILL_H
#define SKILL_H

#include <stdint.h>
#include "core/defs/unit_stats.h"
#include "core/defs/items_info.h"

#define SKILL_NAME_BUF_SIZE 32
#define SKILL_DES_BUF_SIZE 64

#define ROOT_SKILL NULL
#define NATURE_WEAPON_GROUP 0

enum skill_target_class {
    SKILL_TARGET_NONE,
    SKILL_TARGET_SELF,
    SKILL_TARGET_ENEMY_UNIT,
    SKILL_TARGET_ALLY_UNIT,
    SKILL_TARGET_ANY_UNIT,
    SKILL_TARGET_ALL_ENEMIES,
    SKILL_TARGET_ALL_ALLIES
};

enum skill_id {
    SKILL_SKIP_TURN,
    SKILL_USUAL_SLASH,
    SKILL_USUAL_STABBING,
    SKILL_MAUL,
    SKILL_COUNT 
};

struct skill {
    char name[SKILL_NAME_BUF_SIZE];
    char description[SKILL_DES_BUF_SIZE];

    enum skill_target_class target_class;

    struct unit_stats req_stats;
    struct skill *req_learned;

    uint16_t action_points_cost;
    uint64_t weapon_group_mask;
    uint64_t unit_template_tags_group_mask;
};


int skill_is_target_class_require_choice(enum skill_target_class t);

enum skill_id skill_get_id(const struct skill *skill,
                        const struct skill *all_skills);

void skills_load(struct skill all_skills[SKILL_COUNT]);

int skill_is_compatible_with_weapon(const struct skill *skill, 
                                    const enum item_id weapon_id);

int skill_is_compatible_with_unit_template(const struct skill *skill,
                                           const uint64_t template_tags); 

#endif
