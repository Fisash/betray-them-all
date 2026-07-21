#include <stdlib.h>

#include "core/battle/battle_bot.h"

static const struct skill *select_random_not_skip_skill(struct battle_turn_context *context)
{
    if(context->skill_count <= 1)
        return context->available_skills[0];

    int index = 1 + rand() % (context->skill_count - 1);
    return context->available_skills[index];
}

const struct skill *battle_bot_select_skill(struct battle_turn_context *context,
                                  enum bot_selecting_skill_tactic tactic)
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

static struct battle_unit 
       *select_random_target(struct battle_skill_use_context *context)
{
    int index = rand() % (context->target_count - 1);
    return context->available_targets.units[index];
}

static struct battle_unit
       *select_less_hp_target(struct battle_skill_use_context *context)
{
    struct battle_unit *result, *current;
    result = context->available_targets.units[0];
    int i;
    for (i = 0; i < context->target_count; i++)
    {
        current = context->available_targets.units[i];
        if(current->unit->hp < result->unit->hp)
            result = current;
    }
    return result;
}

struct battle_unit *battle_bot_select_target(struct battle_skill_use_context *context,
                                      enum bot_selecting_target_tactic tactic)
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
