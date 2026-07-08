#include <string.h>

#include "core/defs/skill.h"
#include "core/defs/unit_templates.h"


int skill_is_target_class_require_choice(skill_target_class_t t)
{
    return (t == SKILL_TARGET_ENEMY_UNIT ||
            t == SKILL_TARGET_ANY_UNIT   ||
            t == SKILL_TARGET_ANY_UNIT);            
}

static void skill_init(skill_t *skill,const char *name, 
        const char *description, skill_target_class_t target_class,
                      unit_stats_t req_stats, skill_t *req_learned, 
           uint16_t action_points_cost, uint64_t weapon_group_mask, 
                            uint64_t unit_template_tags_group_mask)
{
    strcpy(skill->name, name);
    strcpy(skill->description, description);
    skill->target_class = target_class;
    skill->req_stats = req_stats;
    skill->req_learned = req_learned;
    skill->action_points_cost = action_points_cost;
    skill->weapon_group_mask = weapon_group_mask;
    skill->unit_template_tags_group_mask = unit_template_tags_group_mask;
}

int skill_is_compatible_with_weapon(const skill_t *skill, 
                                const item_id weapon_id)
{
    int is_natural = (skill->weapon_group_mask == NATURE_WEAPON_GROUP);
    int is_weapon_in_group = 
        (skill->weapon_group_mask & ((uint64_t)1 << weapon_id));

    return is_natural || is_weapon_in_group;
}

int skill_is_compatible_with_unit_template(const skill_t *skill,
                                   const uint64_t template_tags)
{
    return ((skill->unit_template_tags_group_mask & template_tags) != 0);
}

skill_id_t skill_get_id(const skill_t *skill, const skill_t all_skills[])
{
    skill_id_t i;
    for(i = 0; i < SKILL_COUNT; i++)
        if(all_skills+i == skill)
            return i;
    return SKILL_SKIP_TURN;
}

void skills_load(skill_t skills[])
{
    unit_stats_t req_stats;

    unit_stats_init(&req_stats, 0, 0, 0, 0);
    skill_init(&skills[SKILL_SKIP_TURN], 
               "Skip turn", "Just skip the turn", 
               SKILL_TARGET_SELF, req_stats, ROOT_SKILL, 
               0, NATURE_WEAPON_GROUP,UNIT_ALL_TAGS_MASK);

    unit_stats_init(&req_stats, 10, 0, 0, 0);
    skill_init(&skills[SKILL_USUAL_SLASH], 
               "Usual slash", "Base damage", 
               SKILL_TARGET_ENEMY_UNIT, req_stats, ROOT_SKILL, 
               10, NATURE_WEAPON_GROUP, (uint64_t)1<<UNIT_TAG_HUMAN);

    unit_stats_init(&req_stats, 0, 10, 0, 0);
    skill_init(&skills[SKILL_USUAL_STABBING], 
               "Usual stabbing", "Base damage",
               SKILL_TARGET_ENEMY_UNIT, req_stats, ROOT_SKILL, 
               10, NATURE_WEAPON_GROUP, (uint64_t)1<<UNIT_TAG_HUMAN);

    unit_stats_init(&req_stats, 5, 5, 0, 0);
    skill_init(&skills[SKILL_MAUL], 
               "Maul", "Maul",
               SKILL_TARGET_ENEMY_UNIT, req_stats, ROOT_SKILL, 
               10, NATURE_WEAPON_GROUP, (uint64_t)1<<UNIT_TAG_ANIMAL);
}
