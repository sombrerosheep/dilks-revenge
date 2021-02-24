#include <vec.h>

#include <math.h>

float Vec2_Magnitude(Vec2 vec) {
  float x2, y2, mag;

  x2 = vec.x * vec.x;
  y2 = vec.y * vec.y;

  mag = sqrtf(x2 + y2);

  return mag;
}

Vec2 Vec2_Normalize(Vec2 vec) {
  Vec2 norm;
  float mag;

  mag = Vec2_Magnitude(vec);

  norm.x = vec.x / mag;
  norm.y = vec.y / mag;

  return norm;
}