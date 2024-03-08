#ifndef DREV_VEC_H
#define DREV_VEC_H

typedef struct drev_vec2 Vec2;

struct drev_vec2 {
    float x, y;
};

#define Vec2_Zero \
    (Vec2) { 0.f, 0.f }
#define Vec2_Newf(x) \
    (Vec2) { (x), (x) }
float Vec2_Magnitude(Vec2 vec);
Vec2  Vec2_Normalize(Vec2 vec);
Vec2  Vec2_Rotate(Vec2 vec, float deg);

#endif // DREV_VEC_H
