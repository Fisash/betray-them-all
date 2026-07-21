#include <string.h>
#include <stdlib.h>

#include "core/state/unit.h"

#define LEVEL_UP_EXP_MULTIPLIER 1.3f

uint16_t unit_get_damage(struct unit *unit, const struct item_info items[])
{
   struct scaling_group scalings = 
                   items[unit->weapon.id].props.weapon.damage.scalings;
   return scale_param(unit->weapon.props.weapon.damage, 
                                              scalings,
                                          unit->stats);
}

uint16_t unit_get_crit(struct unit *unit, const struct item_info items[])
{
   struct scaling_group scalings = 
                   items[unit->weapon.id].props.weapon.crit.scalings;
   return scale_param(unit->weapon.props.weapon.crit, 
                                            scalings,
                                        unit->stats);
}

uint16_t unit_get_protection(struct unit *unit, const struct item_info items[])
{

   struct scaling_group scalings = 
                   items[unit->armor.id].props.armor.protection.scalings;
   return scale_param(unit->armor.props.armor.protection, 
                                                scalings,
                                            unit->stats);
}

uint16_t unit_get_mobility(struct unit *unit, const struct item_info items[])
{
   struct scaling_group scalings = 
                   items[unit->armor.id].props.armor.mobility.scalings;
   return scale_param(unit->armor.props.armor.mobility, 
                                              scalings,
                                          unit->stats);
}

void unit_init(struct unit *unit, const char *name, enum unit_template_id id, 
                                      const struct unit_template templates[],
                                         const struct item_info items_info[])
{
    unit->is_alive = 1;
    unit->template_id = id;
    const struct unit_template *t = &templates[id];

    memset(unit->name, ' ', UNIT_NAME_BUF_SIZE);
    unit->name[UNIT_NAME_BUF_SIZE-1] = '\0';
    if (strlen(name) < UNIT_NAME_BUF_SIZE)
        strcpy(unit->name, name);

    unit->max_hp = get_between_value(t->hp_min, t->hp_max);
    unit->hp = unit->max_hp;

    unit->stats.strength = get_between_value(t->min_stats_value.strength, 
                                             t->max_stats_value.strength);
    unit->stats.agility = get_between_value(t->min_stats_value.agility, 
                                            t->max_stats_value.agility);
    unit->stats.will = get_between_value(t->min_stats_value.will, 
                                         t->max_stats_value.will);
    unit->stats.intelligence = 
                get_between_value(t->min_stats_value.intelligence, 
                                 t->max_stats_value.intelligence);

    item_init(&unit->weapon, items_info, t->weapon_item_id);
    item_init(&unit->armor, items_info, t->armor_item_id);

    unit->exp_reward = t->base_exp_reward;
    unit->unspent_stat_points = 0;
    unit->exp = 0;
    unit->exp_for_next_level = 100;
    unit->level = 1;

    memcpy(unit->learned, t->initial_learned, sizeof(skills_mask_t));
}

void unit_fill_available_skills(skills_mask_t out, const struct unit *unit,
                                               const struct skill skills[],
                                    const struct unit_template templates[])
{
    const struct skill *skill;
    int is_ok_weapon, is_ok_stats, is_ok_tags, i;

    uint64_t unit_tags = templates[unit->template_id].tags_mask;

    memcpy(out, unit->learned, sizeof(skills_mask_t));
    for(i = 0; i < SKILL_COUNT; i++)
    {
        skill = &skills[i];

        is_ok_weapon = skill_is_compatible_with_weapon(skill, 
                                            unit->weapon.id);
        is_ok_stats = unit_stats_is_eligible(&unit->stats, 
                                    &skills[i].req_stats);
        is_ok_tags = skill_is_compatible_with_unit_template(skill,
                                                       unit_tags);

        if(!(is_ok_weapon && is_ok_stats && is_ok_tags))
            skills_mask_clear_bit(out, i);
    }
}

void unit_add_exp(struct unit *unit, uint16_t exp)
{
    unit->exp += exp;
    while(unit->exp >= unit->exp_for_next_level)
    {
        unit->level++;
        unit->unspent_stat_points++;
        unit->exp -= unit->exp_for_next_level;
        unit->exp_for_next_level *= LEVEL_UP_EXP_MULTIPLIER;
    }
}

int unit_apply_stat_point(struct unit *unit, enum stat_selection s)
{
    uint16_t *stat;
    uint16_t hp_increase;

    if(unit->unspent_stat_points <= 0)
        return 1;

    stat = unit_stats_get_stat(&unit->stats, s);
    if(!stat)
        return 1;

    (*stat)++;
    hp_increase = (s == STRENGTH || s == WILL) ? 2 : 1;
    unit->max_hp += hp_increase;
    unit->unspent_stat_points--;
    return 0;
}
