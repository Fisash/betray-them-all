#include <stdlib.h>

#include "core/battle_system.h"

#ifdef DEBUG
#include <stdio.h>
#endif

static int is_unit_able_to_turn(const battle_unit_t *b)
{
    return (b && b->unit && b->unit->is_alive && b->action_points > 0);
}

static battle_unit_t *find_active_unit_from_side(battle_unit_t side[])
{
    for(int i = 0; i < MAX_UNITS; i++)
        if(is_unit_able_to_turn(&side[i]))
            return &side[i];        
    return NULL;
}

static battle_unit_t *find_active_unit(battle_state_t *state)
{
    battle_unit_t *side, *active;
    int is_player_turn, r, pass;

    r = rand() % 2;
    for(pass = 0; pass < 2; pass++)
    {
        is_player_turn = (pass == r);
        side = is_player_turn ? state->player_units : state->enemy_units;
        active = find_active_unit_from_side(side);
        if(active)
            return active;
    }

    return NULL;
}

static void fill_unit_available_skills(const battle_unit_t *b, 
                                  const skill_t *out_skills[], 
                                   const skill_t all_skills[],
                            const unit_template_t templates[])
{
    skills_mask_t available;
    int is_include_bit, is_enough_ap, turn_skill_count;

    unit_fill_available_skills(available, b->unit, all_skills, templates);
    turn_skill_count = 0;
    for (int i = 0; i < SKILL_COUNT; i++)
    {
        is_include_bit = skills_mask_is_include_bit(available, i);
        is_enough_ap = (b->action_points >= all_skills[i].action_points_cost);

        if(is_include_bit && is_enough_ap)
        {
            out_skills[turn_skill_count++] = &all_skills[i];
            #ifdef DEBUG
            printf("[DEBUG]skill: %s\n", all_skills[i].name);
            #endif
        }
    }
}

static void battle_system_next_round(battle_state_t *state)
{
    battle_unit_t *side;
    int pass, i;

    for(pass = 0; pass < 2; pass++)
    {
        side = (pass == 0) ? state->player_units : state->enemy_units;
        for(i = 0; i < MAX_UNITS; i++)
            side[i].action_points = START_ACTION_POINTS;
    }

}

battle_turn_context_t battle_system_next_turn(battle_state_t *state, 
                                         const skill_t all_skills[],
                                  const unit_template_t templates[])
{
    battle_turn_context_t result = {0};

    battle_unit_t *active = find_active_unit(state);
    if(active == NULL)
    {
        battle_system_next_round(state);
        active = find_active_unit(state);
    }
    result.active_unit = active;
    fill_unit_available_skills(active, result.available_skills, 
                                         all_skills, templates);
    return result;
}

/*
void battle_system_get_skill_context(battle_state_t *state, 
                const unit_t *active, const skill_t *skill)
{

}*/

/*
void battle_system_select_target(battle_state_t *state, skill_t *skill)
{

}*/
