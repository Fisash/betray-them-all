#include "core/defs/unit_stats.h"

void unit_stats_init(unit_stats_t *p, uint16_t strength, uint16_t agility, 
                                    uint16_t will, uint16_t intelligence)
{
    p->strength = strength;
    p->agility = agility;
    p->will = will;
    p->intelligence = intelligence;
}

int unit_stats_is_eligible(const unit_stats_t *unit, 
                           const unit_stats_t *req)
{
    return ((unit->strength >= req->strength)       &&
            (unit->agility >= req->agility)         &&
            (unit->will >= req->will)               &&
            (unit->intelligence >= req->intelligence));
}
