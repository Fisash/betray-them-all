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

typedef enum {
    BATTLE_STATUS_LOST,
    BATTLE_STATUS_ACTIVE,
    BATTLE_STATUS_WON
} battle_status_t;

typedef struct {
    unit_t *unit;
    uint16_t action_points;
    uint8_t side;
} battle_unit_t;

typedef struct {
    battle_status_t status;
    uint8_t round_num;

    battle_unit_t player_units[MAX_UNITS];
    uint8_t player_unit_count;

    battle_unit_t enemy_units[MAX_UNITS];
    uint8_t enemy_unit_count;

    unit_t enemy_storage[MAX_UNITS];

    const skill_t *all_skills;
    const item_info_t *all_items;
    const unit_template_t *all_templates;
} battle_state_t;

typedef struct {
    battle_unit_t *active_unit;
    uint8_t skill_count;
    const skill_t *available_skills[MAX_TURN_SKILL_COUNT];
} battle_turn_context_t;

typedef struct {
    const skill_t *skill;
    uint8_t target_count;
    union {
        battle_unit_t *units[MAX_UNITS*2];
    } available_targets;
} battle_skill_use_context_t;

typedef enum {
    BATTLE_EVENT_TAKING_DAMAGE,
    BATTLE_EVENT_HEALING,
    BATTLE_EVENT_TARGET_EVASION
} battle_event_type_t;

typedef struct {
    battle_event_type_t type;
    battle_unit_t *target;
    int hp_change;
    uint8_t is_target_died;
} battle_event_report_t;

typedef struct {
    battle_unit_t *caster;
    const skill_t *skill;
    uint8_t is_crit;
    uint8_t target_count;
    battle_event_report_t events[MAX_UNITS*2];
    uint8_t is_actor_died;
} battle_skill_execution_report_t;

int battle_unit_is_alive(battle_unit_t *b);

battle_unit_t *battle_state_get_faction_units(battle_state_t *state, 
                                                      int is_player);

void battle_state_init(battle_state_t *b, squad_t *squad, 
                   const skill_t all_skills[SKILL_COUNT],
                   const item_info_t all_items[ALL_ITEMS_COUNT],
                   const unit_template_t all_templates[UNIT_TEMP_COUNT]);

int battle_state_add_enemy(battle_state_t *b, const unit_t *enemy);

#endif
