#ifdef CLI
#   include "cli/entry.h"
#elif defined(XLIB)
#   include "gui/gui-frontend/gui_main.h"
#   include "gui/xlib-port/xlib.h"
#else
#   error "Invalid platform"
#endif

#include "core/state/game_state.h"
#include "core/defs/game_info.h"

struct game_info game_info;
struct game_state game_state;

int main()
{
    game_info_load(&game_info);
    game_state_init(&game_state, &game_info);

#ifdef CLI
    cli_run(&game_state, &game_info);
#elif defined(XLIB)
    struct platform_interface platform;
    xlib_init_interface(&platform);
    gui_run(&platform, &game_state, &game_info);
#endif
    return 0;
}
