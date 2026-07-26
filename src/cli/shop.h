#ifndef CLI_SHOP_H
#define CLI_SHOP_H

#include "cli/command_inputer.h"
#include "core/state/shop.h"
#include "core/state/squad.h"

void cli_shop_run(struct command_inputer *inputer,
                        struct shop **active_shop, 
                             struct squad *squad);

#endif
