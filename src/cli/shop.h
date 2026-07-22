#ifndef CLI_SHOP_H
#define CLI_SHOP_H

#include "core/state/shop.h"
#include "core/state/squad.h"

void cli_shop_run(struct shop **active_shop, struct squad *squad, 
                                   const struct item_info info[]);

#endif
