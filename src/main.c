#ifdef CLI
    #include "cli/cli_main.h"
#elif defined(XLIB)
    #include "gui/gui-frontend/gui_main.h"
    #include "gui/xlib-port/xlib.h"
#else
    #error "Invalid platform"
#endif

#include "core/state/game_state.h"
#include "core/defs/game_info.h"

int main()
{
    game_info_t game_info;
    game_info_load(&game_info);

    game_state_t game_state;
    game_state_init(&game_state, &game_info);

#ifdef CLI
    cli_run(&game_state, &game_info);
#elif defined(XLIB)
    platform_interface_t platform;
    xlib_init_interface(&platform);
    gui_run(&platform);
#endif
    return 0;
}
