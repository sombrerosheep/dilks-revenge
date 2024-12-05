#ifndef DREV_VEC_H
#define DREV_VEC_H

#include "types.h"

typedef struct drev_vec2  Vec2;
typedef struct drev_vec2i Vec2i;

struct drev_vec2 {
    f32 x, y;
};

struct drev_vec2i {
    i32 x, y;
};

#define Vec2_Zero \
    (Vec2) { 0.f, 0.f }
#define Vec2_Newf(x) \
    (Vec2) { (x), (x) }
#define Vec2_Up \
    (Vec2) { .x = 0.f, .y = -1.f }
#define Vec2_Down \
    (Vec2) { .x = 0.f, .y = 1.f }
#define Vec2_Left \
    (Vec2) { .x = -1.f, .y = 0.f }
#define Vec2_Right \
    (Vec2) { .x = 1.f, .y = 0.f }

float Vec2_Magnitude(Vec2 vec);
Vec2  Vec2_Normalize(Vec2 vec);
Vec2  Vec2_Rotate(Vec2 vec, f32 deg);

///
/// @brief determines if two Vec2 are equal
///
/// @param a
/// @param b
/// @return i8 1 if equal, 0 if not.
///
i8 Vec2_Equal(Vec2 a, Vec2 b);

#endif // DREV_VEC_H
