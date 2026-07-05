#include <stdlib.h>

#include "core/battle/battle_bot.h"

static const skill_t *select_random_not_skip_skill(battle_turn_context_t *context)
{
    if(context->skill_count <= 1)
        return context->available_skills[0];

    int index = 1 + rand() % (context->skill_count - 1);
    return context->available_skills[index];
}

const skill_t *battle_bot_select_skill(battle_turn_context_t *context,
                                  bot_selecting_skill_tactic_t tactic)
{
    switch(tactic)
    {
        case SELECTING_RANDOM_NOT_SKIP_SKILL:
            return select_random_not_skip_skill(context);
        default:
            break;
    }    

    return NULL;
}

static battle_unit_t 
       *select_random_target(battle_skill_use_context_t *context)
{
    int index = rand() % (context->target_count - 1);
    return context->available_targets.units[index];
}

static battle_unit_t
       *select_less_hp_target(battle_skill_use_context_t *context)
{
    battle_unit_t *result, *current;
    result = context->available_targets.units[0];
    for (int i = 0; i < context->target_count; i++)
    {
        current = context->available_targets.units[i];
        if(current->unit->hp < result->unit->hp)
            result = current;
    }
    return result;
}

battle_unit_t *battle_bot_select_target(battle_skill_use_context_t *context,
                                      bot_selecting_target_tactic_t tactic)
{
    switch(tactic)
    {
        case SELECTING_RANDOM_TARGET:
            return select_random_target(context);
        case SELECTING_LESS_HP_TARGET:
            return select_less_hp_target(context);
        default:
            break;
    }    

    return NULL;
}
