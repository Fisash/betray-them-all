#ifndef LEARNED_SKILLS_H
#define LEARNED_SKILLS_H

#include "core/defs/skill.h"

#define BITS_PER_WORD 64
#define LEARNED_SKILL_WORDS ((SKILL_COUNT + BITS_PER_WORD - 1) / BITS_PER_WORD)

typedef uint64_t skills_mask_t[LEARNED_SKILL_WORDS];

int skills_mask_is_include_bit(const skills_mask_t mask, enum skill_id id);

void skills_mask_set_bit(skills_mask_t mask, enum skill_id id);

void skills_mask_clear_bit(skills_mask_t mask, enum skill_id id);

#endif
