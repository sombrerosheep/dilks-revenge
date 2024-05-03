#include "projectile.h"

#include "camera.h"
#include "globals.h"
#include "resources.h"
#include "util.h"
#include "vec.h"

void Projectile_Init(Projectile    *p,
                     ProjectileType type,
                     Vec2           position,
                     Vec2           velocity,
                     f32            speed_m_sec,
                     u32            strength,
                     float          sz //
) {
    p->type        = type;
    p->position    = position;
    p->velocity    = Vec2_Normalize(velocity);
    p->speed_m_sec = speed_m_sec;
    p->size        = Vec2_Newf(sz);

    // todo: should this be a lookup based on the projectile type?
    p->strength = strength;
}

SDL_FRect Projectile_GetBounds(const Projectile *p) {
    SDL_FRect rect = {
        .x = p->position.x,
        .y = p->position.y,
        .w = p->size.x,
        .h = p->size.y,
    };

    return rect;
}

i8 Projectile_CanHurtPlayer(const Projectile *p) {
    switch (p->type) {
        case ProjectileType_Enemy:
            return 1;
        default:
            return 0;
    }
}

void Projectile_Update(Projectile *p, f32 delta) {
    p->position.x += p->velocity.x * p->speed_m_sec * delta;
    p->position.y += p->velocity.y * p->speed_m_sec * delta;
}

void Projectile_Draw(const Projectile *p) {
    SDL_Color projectile_color = {
        .r = 0xDD,
        .g = 0xDD,
        .b = 0xDD,
        .a = 0xFF,
    };
    Camera *camera = Resources_GetMainCamera();

    SDL_FRect rect = (SDL_FRect){
        .x = p->position.x,
        .y = p->position.y,
        .w = p->size.x,
        .h = p->size.y,
    };

    Camera_DrawFillRect(camera, rect, projectile_color);

#ifdef DREV_DRAW_BB
    Camera_DrawRect(camera, Projectile_GetBounds(p), ColorRed);
#endif
}
