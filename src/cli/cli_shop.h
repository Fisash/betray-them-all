#ifndef CLI_SHOP_H
#define CLI_SHOP_H

#include "core/state/shop.h"
#include "core/state/squad.h"

void cli_shop_run(shop_t **active_shop, squad_t *squad, 
                             const item_info_t info[]);

#endif
