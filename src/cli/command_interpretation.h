#ifndef COMMAND_INTERPRETATION_H
#define COMMAND_INTERPRETATION_H
#include "core/time_system.h"
#include "core/exploring_system.h"
#include "core/event_system.h"
#include "core/squad_movement.h"
#include "cli/base.h"

/* dependencies are not clear! */
/* where the fckn game_info and game_state?? */

void interpret_command(struct command *cmd, struct game_state *state,
                       const struct game_info *info, int *need_redraw);

#endif
