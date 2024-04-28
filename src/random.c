#include "random.h"

#include <stdlib.h>

u32 random_get_eskil(u32 index) {
    index = (index << 13) ^ index;
    return ((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

u32 random_get_eskil_between(u32 index, u32 min, u32 max) {
    u32 rand;

    rand = ((random_get_eskil(index) % (max - min + 1)) + min);

    return rand;
}

void random_init(i32 seed) {
    srand(seed);
}

i32 random_get(void) {
    i32 num;

    num = rand();

    return num;
}

f32 random_getf(void) {
    i32 num;
    f32 res;

    num = rand();
    res = (f32)num / 1000.f;

    return res;
}

i32 random_get_between(i32 min, i32 max) {
    i32 num;

    num = (random_get() % (max - min + 1)) + min;

    return num;
}

f32 random_getf_between(f32 min, f32 max) {
    f32 scale, num;

    scale = rand() / (f32)RAND_MAX;
    num   = min + scale * (max - min);

    return num;
}
