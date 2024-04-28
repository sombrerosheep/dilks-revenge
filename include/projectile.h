#ifndef DREV_PROJECTILE_H
#define DREV_PROJECTILE_H

#include "camera.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_projectile Projectile;

typedef enum {
    ProjectileType_Player = 0,
    ProjectileType_Enemy,

    ProjectileType_Count
} ProjectileType;

struct drev_projectile {
    ProjectileType type;
    Vec2           position;
    Vec2           velocity;
    Vec2           size;
    f32            speed_m_sec;
    u32            strength;
};

// todo: factor in m/s with speeds
void      Projectile_Init(Projectile    *p,
                          ProjectileType type,
                          Vec2           position,
                          Vec2           velocity,
                          f32            speed_m_sec,
                          u32            strength);
SDL_FRect Projectile_GetBounds(const Projectile *p);
i8 Projectile_CanHurtPlayer(const Projectile *p);
void      Projectile_Update(Projectile *p, f32 delta);
void      Projectile_Draw(const Projectile *p);

#endif // DREV_PROJECTILE_H
