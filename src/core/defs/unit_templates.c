#include <string.h>
#include "core/defs/unit_templates.h"

static void set_start_equipment(unit_template_t *t, item_id armor, 
                                                    item_id weapon)
{
    t->armor_item_id = armor;
    t->weapon_item_id = weapon;
}

static void fill_human_learnings(skills_mask_t *mask)
{
    skills_mask_set_bit(*mask, SKILL_SKIP_TURN);
    skills_mask_set_bit(*mask, SKILL_USUAL_SLASH);
    skills_mask_set_bit(*mask, SKILL_USUAL_STABBING);
}


static void fill_animal_learnings(skills_mask_t *mask)
{
    skills_mask_set_bit(*mask, SKILL_SKIP_TURN);
    skills_mask_set_bit(*mask, SKILL_MAUL);
}

void unit_templates_load(unit_template_t templates[])
{
    unit_template_t *t;

    skills_mask_t human_base_learnings = {0};
    fill_human_learnings(&human_base_learnings);

    skills_mask_t animal_base_learnings = {0};
    fill_animal_learnings(&animal_base_learnings);

    t = &templates[WARRIOR];
    strcpy(t->name, "Warrior");
    t->hp_min=10; t->hp_max = 15;
    unit_stats_init(&t->min_stats_value, 10, 8, 10, 10);
    unit_stats_init(&t->max_stats_value, 12, 10, 12, 10);
    t->tags_mask = (uint64_t)1<<UNIT_TAG_HUMAN;
    set_start_equipment(t, ARMOR_FABRIC, WEAPON_SHORT_SWORD);
    memcpy(t->initial_learned, human_base_learnings, sizeof(skills_mask_t));

    t = &templates[ROGUE];
    strcpy(t->name, "Rogue");
    t->hp_min=8; t->hp_max = 12;
    unit_stats_init(&t->min_stats_value, 8, 12, 9, 10);
    unit_stats_init(&t->max_stats_value, 9, 14, 10, 10);
    t->tags_mask = (uint64_t)1<<UNIT_TAG_HUMAN;
    set_start_equipment(t, ARMOR_FABRIC, WEAPON_DAGGER);
    memcpy(t->initial_learned, human_base_learnings, sizeof(skills_mask_t));

    /*enemy templates:*/
    t = &templates[WOLF];
    strcpy(t->name, "Wolf");
    t->hp_min=8; t->hp_max = 10;
    unit_stats_init(&t->min_stats_value, 8, 10, 3, 1);
    unit_stats_init(&t->max_stats_value, 9, 12, 5, 3);
    t->tags_mask = (uint64_t)1<<UNIT_TAG_ANIMAL;
    set_start_equipment(t, ARMOR_NUDE, WEAPON_CLAW);
    memcpy(t->initial_learned, animal_base_learnings, sizeof(skills_mask_t));
}
