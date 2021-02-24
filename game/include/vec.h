#ifndef DREV_VEC_H
#define DREV_VEC_H

typedef struct _vec2 Vec2;

struct _vec2 {
  float x, y;
};

float Vec2_Magnitude(Vec2 vec);
Vec2 Vec2_Normalize(Vec2 vec);

#endif // DREV_VEC_H