#ifndef BATTLE_BOT_H
#define BATTLE_BOT_H

#include "core/defs/skill.h"
#include "core/state/battle_state.h"

enum bot_selecting_skill_tactic {
    SELECTING_RANDOM_NOT_SKIP_SKILL
};

enum bot_selecting_target_tactic {
    SELECTING_RANDOM_TARGET,
    SELECTING_LESS_HP_TARGET
};

const struct skill *battle_bot_select_skill(struct battle_turn_context *context,
                                            enum bot_selecting_skill_tactic tactic);

struct battle_unit *battle_bot_select_target(struct battle_skill_use_context *context,
                                             enum bot_selecting_target_tactic tactic);

#endif
