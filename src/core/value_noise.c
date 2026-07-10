#include "core/value_noise.h"

double value_noise_hash(uint32_t x, uint32_t y, uint32_t seed)
{
    uint32_t r1 = seed*(x*34825763 + y*24582791);
    uint32_t r2 = seed*(x*49480787 + y*12458033);
    uint32_t h = r1 ^ r2;
    h = h ^ (h>>13) * 38452943;
    h = h ^ 5892347345;

    return (double)h/((double)UINT32_MAX + 1.0);
}

static double linear_inter(double a, double b, double t)
{
    return a*(1-t)+b*t;
}

double value_noise_octave(uint32_t x, uint32_t y, 
                uint32_t node_size, uint32_t seed)
{
    uint32_t nx = x + (node_size - x % node_size);
    uint32_t ny = y + (node_size - y % node_size);
    uint32_t px = x - (x % node_size);
    uint32_t py = y - (y % node_size);

    double right_up = value_noise_hash(nx, ny, seed);
    double right_down = value_noise_hash(nx, py, seed);
    double left_up = value_noise_hash(px, ny, seed);
    double left_down = value_noise_hash(px, py, seed);

    double tx = (x - px)/node_size;
    double ty = (y - py)/node_size;

    double down = linear_inter(left_down, right_down, tx);
    double up = linear_inter(left_up, right_up, tx);

    return linear_inter(down, up, ty);
}

double value_noise(uint32_t x, uint32_t y, uint32_t seed)
{
    double o5 = value_noise_octave(x, y, 5, seed);
    double o4 = value_noise_octave(x, y, 4, seed);
    double o3 = value_noise_octave(x, y, 3, seed);
    double o2 = value_noise_octave(x, y, 2, seed);

    return (o5+o4+o3+o2)/4;
}
