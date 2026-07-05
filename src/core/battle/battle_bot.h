#ifndef BATTLE_BOT_H
#define BATTLE_BOT_H

#include "core/defs/skill.h"
#include "core/state/battle_state.h"

typedef enum {
    SELECTING_RANDOM_NOT_SKIP_SKILL
} bot_selecting_skill_tactic_t;

const skill_t *battle_bot_select_skill(battle_turn_context_t *context,
                                  bot_selecting_skill_tactic_t tactic);

#endif
