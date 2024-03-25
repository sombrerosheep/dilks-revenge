#ifndef DREV_PROJECTILE_H
#define DREV_PROJECTILE_H

#include "camera.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_projectile Projectile;

typedef enum {
    ProjectileType_Player = 0,
} ProjectileType;

struct drev_projectile {
    ProjectileType type;
    Vec2           position;
    Vec2           velocity;
    Vec2           size;
    float          speed_m_sec;
};

// todo: factor in m/s with speeds
//       velocity should be normalized
void Projectile_Init(Projectile    *p,
                     ProjectileType type,
                     Vec2           position,
                     Vec2           velocity,
                     float          speed_m_sec);
void Projectile_Update(Projectile *p, float delta);
void Projectile_Draw(const Projectile *p, SDL_Renderer *renderer);

#endif // DREV_PROJECTILE_H
