#ifndef COMMAND_INTERPRETATION_H
#define COMMAND_INTERPRETATION_H

#include "core/time_system.h"
#include "core/exploring_system.h"
#include "core/event_system.h"
#include "core/squad_movement.h"
#include "cli/command_parser.h"

void interpret_command(struct parsed_command *cmd, 
                          struct game_state *game,
                    const struct game_info *info);

#endif
