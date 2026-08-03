#include <stddef.h>

#include "core/core.h"

static struct game_info GLOBAL_game_info;
static struct game_state GLOBAL_game_state;

void core_init(struct core *core)
{
    game_info_load(&GLOBAL_game_info);
    core->info = &GLOBAL_game_info;
    core->state = NULL;
}

void core_new_session(struct core *core)
{
    game_state_init(&GLOBAL_game_state, core->info); 
    core->state = &GLOBAL_game_state;
}
