#include "core/defs/skills_mask.h"

int skills_mask_is_include_bit(const skills_mask_t mask, enum skill_id id)
{
    int word_c = id / BITS_PER_WORD;
    int bit_c = id % BITS_PER_WORD;

    uint64_t word = mask[word_c];
    uint64_t bit_mask = (uint64_t)1 << bit_c;
    return (word & bit_mask) != 0;
}

void skills_mask_set_bit(skills_mask_t mask, enum skill_id id)
{
    int word_c = id / BITS_PER_WORD;
    int bit_c = id % BITS_PER_WORD;

    uint64_t bit_mask = (uint64_t)1 << bit_c;
    
    mask[word_c] |= bit_mask;
}

void skills_mask_clear_bit(skills_mask_t mask, enum skill_id id)
{
    int word_c = id / BITS_PER_WORD;
    int bit_c = id % BITS_PER_WORD;

    uint64_t bit_mask = (uint64_t)1 << bit_c;
    
    mask[word_c] &= ~bit_mask;
}
