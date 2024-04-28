#ifndef DREV_RANDOM_H
#define DREV_RANDOM_H

#include "types.h"

u32 random_get_eskil(u32 seed);
u32 random_get_eskil_between(u32 index, u32 min, u32 max);

void random_init(i32 seed);
i32  random_get(void);
f32  random_getf(void);
i32  random_get_between(i32 min, i32 max);
f32  random_getf_between(f32 min, f32 max);

#endif // DREV_RANDOM_H
