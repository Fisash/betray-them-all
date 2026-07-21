#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

#include <stdint.h>

#include "core/state/unit.h"
#include "core/state/squad.h"

#define MAX_TURN_SKILL_COUNT 8

#define IS_PLAYER 1
#define IS_ENEMY 0

#define START_ACTION_POINTS 10

#define MAX_UNITS SQUAD_MAX_UNITS

enum battle_status {
    BATTLE_STATUS_LOST,
    BATTLE_STATUS_ACTIVE,
    BATTLE_STATUS_WON
};

struct battle_unit {
    struct unit *unit;
    uint16_t action_points;
    uint8_t side;
};

struct battle_state {
    enum battle_status status;
    uint8_t round_num;

    struct battle_unit player_units[MAX_UNITS];
    uint8_t player_unit_count;

    struct battle_unit enemy_units[MAX_UNITS];
    uint8_t enemy_unit_count;

    struct unit enemy_storage[MAX_UNITS];

    const struct skill *all_skills;
    const struct item_info *all_items;
    const struct unit_template *all_templates;
};

struct battle_turn_context {
    struct battle_unit *active_unit;
    uint8_t skill_count;
    const struct skill *available_skills[MAX_TURN_SKILL_COUNT];
};

struct battle_skill_use_context {
    const struct skill *skill;
    uint8_t target_count;
    union {
        struct battle_unit *units[MAX_UNITS*2];
    } available_targets;
};

enum battle_event_type {
    BATTLE_EVENT_TAKING_DAMAGE,
    BATTLE_EVENT_HEALING,
    BATTLE_EVENT_TARGET_EVASION
};

struct battle_event_report {
    enum battle_event_type type;
    struct battle_unit *target;
    int hp_change;
    uint8_t is_target_died;
};

struct battle_skill_execution_report {
    struct battle_unit *caster;
    const struct skill *skill;
    uint8_t is_crit;
    uint8_t target_count;
    struct battle_event_report events[MAX_UNITS*2];
    uint8_t is_actor_died;
};

int battle_unit_is_alive(struct battle_unit *b);

struct battle_unit *battle_state_get_faction_units(struct battle_state *state, 
                                                                int is_player);

void battle_state_init(struct battle_state *b, struct squad *squad, 
                     const struct skill all_skills[SKILL_COUNT],
                     const struct item_info all_items[ALL_ITEMS_COUNT],
                     const struct unit_template all_templates[UNIT_TEMP_COUNT]);

int battle_state_add_enemy(struct battle_state *b, const struct unit *enemy);

#endif
