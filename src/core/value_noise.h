#ifndef VALUE_NOISE_H
#define VALUE_NOISE_H

#include <stdint.h>


double value_noise_hash(uint32_t x, uint32_t y, uint32_t seed);

double value_noise(uint32_t x, uint32_t y, uint32_t seed);

#endif
