#include <string.h>
#include <stdlib.h>

#include "core/state/unit.h"

uint16_t unit_get_damage(unit_t *unit, const item_info_t items[])
{
   scaling_group_t scalings = 
                   items[unit->weapon.id].props.weapon.damage.scalings;
   return scale_param(unit->weapon.props.weapon.damage, 
                                              scalings,
                                          unit->stats);
}

uint16_t unit_get_crit(unit_t *unit, const item_info_t items[])
{
   scaling_group_t scalings = 
                   items[unit->weapon.id].props.weapon.crit.scalings;
   return scale_param(unit->weapon.props.weapon.crit, 
                                            scalings,
                                        unit->stats);
}

uint16_t unit_get_protection(unit_t *unit, const item_info_t items[])
{

   scaling_group_t scalings = 
                   items[unit->armor.id].props.armor.protection.scalings;
   return scale_param(unit->armor.props.armor.protection, 
                                                scalings,
                                            unit->stats);
}

uint16_t unit_get_mobility(unit_t *unit, const item_info_t items[])
{
   scaling_group_t scalings = 
                   items[unit->armor.id].props.armor.mobility.scalings;
   return scale_param(unit->armor.props.armor.mobility, 
                                              scalings,
                                          unit->stats);
}

int unit_try_set_equip(unit_t *unit, item_t *item, 
                         const item_info_t info[])
{
    int status = 0;
    switch(info[item->id].type)
    {
        case ITEM_TYPE_WEAPON:
            unit->weapon = *item;
            break;
        case ITEM_TYPE_ARMOR:
            unit->armor = *item;
            break;
        default:
            status = 1;
            break;
    }
    return status;
}

void unit_init(unit_t *unit, const char *name, unit_template_id id, 
                                  const unit_template_t templates[],
                                  const item_info_t items_info[])
{
    unit->is_alive = 1;
    unit->template_id = id;
    const unit_template_t *t = &templates[id];

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

    unit->exp = 0;
    unit->exp_for_next_level = 100;
    unit->level = 1;

    memcpy(unit->learned, t->initial_learned, sizeof(skills_mask_t));
}

void unit_fill_available_skills(skills_mask_t out, const unit_t *unit,
                                               const skill_t skills[],
                                    const unit_template_t templates[])
{
    const skill_t *skill;
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
