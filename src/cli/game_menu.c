#include <stdio.h>
#include "game_menu.h"
#include "cli/game_session.h"

#define MSG_HELP_GAME_MENU \
    "[\033[2;3mnewgame\033[0m] [loadgame] [\033[2;3msettings\033[0m] [exit]"

void game_menu(struct game *game)
{
    puts(MSG_HELP_GAME_MENU);
    while(game->is_running)
    {
        command_inputer_input(game->inputer);

            /* something better than game->inputer->cmd.type, please! */

        switch(game->inputer->cmd.type)
        {
            case CMD_NEWGAME:
            case CMD_SETTINGS:
                puts("Not implemented.");
                break;
            case CMD_LOADGAME:
                game_session_load(game);
                puts("Session Loaded.");     /* implement saves  */
                game_session_run(game);      /* differenet sessions */
                break;
            case CMD_HELP:
                puts(MSG_HELP_GAME_MENU);
                break;
            case CMD_EXIT:
                puts("Goodbye.");       /* game_exit() */
                game->is_running = 0;
                break;
            default:
                break;
        }
    }
}
