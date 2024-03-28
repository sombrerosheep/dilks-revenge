#include "projectile.h"

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

void Projectile_Update(Projectile *p, float delta) {
    p->position.x += p->velocity.x * p->speed_m_sec * delta;
    p->position.y += p->velocity.y * p->speed_m_sec * delta;
}

void Projectile_Draw(const Projectile *p, SDL_Renderer *renderer) {
    Camera   *camera     = ResourceManager_GetMainCamera(&GameResources);
    Vec2      screen_pos = Camera_WorldToScreen(camera, p->position);
    SDL_FRect rect       = (SDL_FRect){
              .x = screen_pos.x,
              .y = screen_pos.y,
              .w = p->size.x * PIXELS_PER_METER,
              .h = p->size.y * PIXELS_PER_METER,
    };

    SDL_SetRenderDrawColor(renderer, 0xDD, 0xDD, 0xDD, 0xFF);
    SDL_RenderFillRectF(renderer, &rect);
}
