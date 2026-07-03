#include "terminal-io/cli.h"

#include "core/state/game_state.h"
#include "core/defs/game_info.h"

int main()
{
    game_info_t game_info;
    game_info_load(&game_info);

    game_state_t game_state;
    game_state_init(&game_state, &game_info);

    cli_run(&game_state, &game_info);
    return 0;
}
