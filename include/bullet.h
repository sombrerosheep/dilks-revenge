#ifndef DREV_BULLET_H
#define DREV_BULLET_H

#include <vec.h>

#include <SDL.h>

typedef struct drev_bullet Bullet;

struct drev_bullet {
    Vec2  position;
    Vec2  velocity;
    float health;
};

void Bullet_Init(Bullet *bullet, Vec2 pos, Vec2 vel);
void Bullet_Update(Bullet *bullet, float delta);
void Bullet_Draw(const Bullet *bullet, SDL_Renderer *renderer);

#endif // DREV_BULLET_H
