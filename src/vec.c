#include "vec.h"

#include <SDL.h>
#include <math.h>

float Vec2_Magnitude(Vec2 vec) {
    float x2, y2, mag;

    x2 = vec.x * vec.x;
    y2 = vec.y * vec.y;

    mag = sqrtf(x2 + y2);

    return mag;
}

Vec2 Vec2_Normalize(Vec2 vec) {
    Vec2  norm;
    float mag;

    norm = Vec2_Zero;
    mag  = Vec2_Magnitude(vec);

    if (mag > 0.f) {
        norm.x = vec.x / mag;
        norm.y = vec.y / mag;
    }

    return norm;
}

Vec2 Vec2_Rotate(Vec2 vec, float deg) {
    Vec2  rot   = Vec2_Zero;
    float a_sin = SDL_sinf(deg);
    float a_cos = SDL_cosf(deg);

    rot.x = vec.x * a_cos - vec.y * a_sin;
    rot.y = vec.y * a_cos + vec.x * a_sin;

    return rot;
}

int Vec2_Equal(Vec2 a, Vec2 b) {
    if (a.x == b.x && a.y == b.y) {
        return 1;
    }

    return 0;
}
