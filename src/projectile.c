#include "projectile.h"

#include "camera.h"
#include "globals.h"
#include "resources.h"
#include "vec.h"

void Projectile_Init(Projectile    *p,
                     ProjectileType type,
                     Vec2           position,
                     Vec2           velocity,
                     float          speed_m_sec //
) {
    p->type        = type;
    p->position    = position;
    p->velocity    = velocity;
    p->speed_m_sec = speed_m_sec;
    p->size        = Vec2_Newf(1.);
}

SDL_FRect Projectile_GetBounds(Projectile *p) {
    SDL_FRect rect = {
        .x = p->position.x,
        .y = p->position.y,
        .w = p->size.x,
        .h = p->size.y,
    };

    return rect;
}

void Projectile_Update(Projectile *p, float delta) {
    p->position.x += p->velocity.x * p->speed_m_sec * delta;
    p->position.y += p->velocity.y * p->speed_m_sec * delta;
}

void Projectile_Draw(const Projectile *p) {
    Camera *camera = ResourceManager_GetMainCamera();

    SDL_FRect rect = (SDL_FRect){
        .x = p->position.x,
        .y = p->position.y,
        .w = p->size.x,
        .h = p->size.y,
    };

    Camera_DrawFillRect(camera, rect, 0xDD, 0xDD, 0xDD, 0xFF);
}
