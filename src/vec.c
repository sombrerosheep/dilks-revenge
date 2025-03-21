#include "vec.h"
#include "maths.h"

#include <SDL.h>

f32 Vec2_Magnitude(Vec2 vec) {
    f32 x2, y2, mag;

    x2 = vec.x * vec.x;
    y2 = vec.y * vec.y;

    mag = SDL_sqrtf(x2 + y2);

    return mag;
}

Vec2 Vec2_Normalize(Vec2 vec) {
    Vec2 norm;
    f32  mag;

    norm = Vec2_Zero;
    mag  = Vec2_Magnitude(vec);

    if (mag > 0.f) {
        norm.x = vec.x / mag;
        norm.y = vec.y / mag;
    }

    return norm;
}

Vec2 Vec2_Rotate(Vec2 vec, f32 deg) {
    // todo: this accepted the angel in degress but really wanted it
    // in radians. Since i'm only using this with deg currently, moving
    // that conversion inside the function.
    f32 rad = DegToRad(deg);
    Vec2 rot   = Vec2_Zero;
    f32  a_sin = SDL_sinf(rad);
    f32  a_cos = SDL_cosf(rad);

    rot.x = vec.x * a_cos - vec.y * a_sin;
    rot.y = vec.y * a_cos + vec.x * a_sin;

    return rot;
}

i8 Vec2_Equal(Vec2 a, Vec2 b) {
    if (a.x == b.x && a.y == b.y) {
        return 1;
    }

    return 0;
}
