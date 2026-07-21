#ifndef WORLD_QUERIES_H
#define WORLD_QUERIES_H

#include "core/state/world.h"
#include "core/state/squad.h"

struct cell *world_queries_get_squad_cell(struct squad *squad, struct world *world);

#endif
