#ifndef CLI_SHOP_H
#define CLI_SHOP_H

#include "core/state/shop.h"
#include "core/state/squad.h"

typedef enum {
    SHOP_CMD_EXIT,
    SHOP_CMD_TRY_BUY,
    SHOP_CMD_TRY_SELL,
} shop_command_type_t;

typedef struct {
    shop_command_type_t type;
    uint8_t slot_index;
} shop_command_t;

void cli_shop_run(shop_t **active_shop, squad_t *squad, 
                             const item_info_t info[]);

#endif
