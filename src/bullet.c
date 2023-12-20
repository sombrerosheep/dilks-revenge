#include "bullet.h"

#include "vec.h"

#define BULLET_HEIGHT      5.f
#define BULLET_WIDTH       5.f
#define BULLET_SPEED       300.f
#define BULLET_INIT_HEALTH 100.f
#define BULLET_DECAY_RATE  (BULLET_INIT_HEALTH) / 3.f

void Bullet_Init(Bullet *bullet, BulletType type, Vec2 pos, Vec2 vel) {
    bullet->type     = type;
    bullet->position = pos;
    bullet->velocity = vel;
    bullet->health   = BULLET_INIT_HEALTH;
    bullet->in_use   = 1;
}

void Bullet_Update(Bullet *bullet, float delta) {
    if (bullet->health > 0.f) {
        bullet->health -= delta * BULLET_DECAY_RATE;

        bullet->position.x += bullet->velocity.x * BULLET_SPEED * delta;
        bullet->position.y += bullet->velocity.y * BULLET_SPEED * delta;
    } else if (bullet->health < 0.f) {
        *bullet = Bullet_Zero;
    }
}

void Bullet_Draw(const Bullet *bullet, SDL_Renderer *renderer) {
    if (bullet->health > 0.f) {
        SDL_FRect rect = {bullet->position.x, bullet->position.y, BULLET_WIDTH, BULLET_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRectF(renderer, &rect);
    }
}

///////////////////
// BulletContainer

int BulletContainer_Init(BulletContainer *c) {
    for (unsigned int i = 0; i < GAME_MAX_BULLETS; i++) {
        c->bullets[i] = Bullet_Zero;
    }

    return 0;
}

int BulletContainer_Add(BulletContainer *c, BulletType type, Vec2 pos, Vec2 vel) {
    for (unsigned int i = 0; i < GAME_MAX_BULLETS; i++) {
        if (c->bullets[i].in_use == 0) {
            Bullet_Init(&c->bullets[i], type, pos, vel);

            return 0;
        }
    }

    return -1;
}

void BulletContainer_Update(BulletContainer *c, float delta) {
    for (unsigned int i = 0; i < GAME_MAX_BULLETS; i++) {
        if (c->bullets[i].in_use == 1) {
            Bullet_Update(&c->bullets[i], delta);
        }
    }
}

void BulletContainer_Draw(BulletContainer *c, SDL_Renderer *r) {
    for (unsigned int i = 0; i < GAME_MAX_BULLETS; i++) {
        if (c->bullets[i].in_use == 1) {
            Bullet_Draw(&c->bullets[i], r);
        }
    }
}

void BulletContainer_Clear(BulletContainer *c) {
    BulletContainer_Init(c);
}
