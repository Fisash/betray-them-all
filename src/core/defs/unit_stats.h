#ifndef UNIT_PROPS_H
#define UNIT_PROPS_H

#include <stdint.h>

typedef struct {
    uint16_t strength;
    uint16_t agility;
    uint16_t will;
    uint16_t intelligence;
} unit_stats_t;

typedef enum {
    STRENGTH,
    AGILITY,
    WILL,
    INTELLIGENCE
} stat_selection_t;

void unit_stats_init(unit_stats_t *p, uint16_t strength, uint16_t agility, 
                                    uint16_t will, uint16_t intelligence);

uint16_t *unit_stats_get_stat(unit_stats_t *stats, stat_selection_t s);

int unit_stats_is_eligible(const unit_stats_t *unit, 
                           const unit_stats_t *req);

#endif
