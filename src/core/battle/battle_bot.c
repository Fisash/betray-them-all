#include <stdlib.h>

#include "core/battle/battle_bot.h"

const skill_t *select_random_not_skip_skill(battle_turn_context_t *context)
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
