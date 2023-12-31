#include "bullet.h"

#include "collisions.h"
#include "vec.h"
#include <SDL_stdinc.h>

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
}

void Bullet_Update(Bullet *bullet, float delta) {
    if (bullet->health > 0.f) {
        bullet->health -= SDL_max((delta * BULLET_DECAY_RATE), 0.f);

        bullet->position.x += bullet->velocity.x * BULLET_SPEED * delta;
        bullet->position.y += bullet->velocity.y * BULLET_SPEED * delta;
    }
}

void Bullet_Draw(const Bullet *bullet, SDL_Renderer *renderer) {
    SDL_FRect rect = {bullet->position.x, bullet->position.y, BULLET_WIDTH, BULLET_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRectF(renderer, &rect);
}

SDL_FRect Bullet_BoundingBox(Bullet *bullet) {
    SDL_FRect box = (SDL_FRect){.x = bullet->position.x,
                                .y = bullet->position.y,
                                .w = BULLET_HEIGHT,
                                .h = BULLET_HEIGHT};

    return box;
}

///////////////////
// BulletContainer

int BulletContainer_Init(BulletContainer *c) {
    memset(&c->bullets, 0, sizeof(ContainerBullet) * GAME_MAX_BULLETS);

    return 0;
}

int BulletContainer_Add(BulletContainer *c, BulletType type, Vec2 pos, Vec2 vel) {
    for (unsigned int i = 0; i < GAME_MAX_BULLETS; i++) {
        if (c->bullets[i].in_use == 0) {
            if ((pos.x == 0.f && pos.y == 0) || (vel.x == 0.f && vel.y == 0.f)) {
                printf("initializing bad bullet: %u\n", i);
            }

            Bullet_Init(&c->bullets[i].bullet, type, pos, vel);
            c->bullets[i].in_use = 1;

            return 0;
        }
    }

    return -1;
}

void BulletContainer_Update(BulletContainer *c, float delta) {
    for (unsigned int i = 0; i < GAME_MAX_BULLETS; i++) {
        if (c->bullets[i].bullet.health <= 0.f) {
            c->bullets[i].in_use = 0;
            continue;
        }

        if (c->bullets[i].in_use == 1) {
            if (c->bullets[i].bullet.position.x == 0.f && c->bullets[i].bullet.position.y == 0.f) {
                printf("Updating bullet zero: %u\n", i);
            }

            Bullet_Update(&c->bullets[i].bullet, delta);
        }
    }
}

int BulletContainer_GetFirstCollision(BulletContainer *c, SDL_FRect rect, Bullet **bullet) {
    if (bullet == NULL) {
        return 0;
    }

    for (unsigned int i = 0; i < GAME_MAX_BULLETS; i++) {
        ContainerBullet *cb = &c->bullets[i];
        if (cb->in_use == 1) {
            SDL_FRect bullet_box = Bullet_BoundingBox(&cb->bullet);
            if (is_colliding(&bullet_box, &rect) == 1) {
                *bullet = &cb->bullet;
                return 1;
            }
        }
    }

    return 0;
}

int BulletContainer_GetFirstCollisionC(BulletContainer *c,
                                       Vec2             center,
                                       float            radius,
                                       Bullet         **bullet) {
    if (bullet == NULL) {
        return 0;
    }

    for (unsigned int i = 0; i < GAME_MAX_BULLETS; i++) {
        ContainerBullet *cb = &c->bullets[i];
        if (cb->in_use == 1) {
            SDL_FRect bullet_box = Bullet_BoundingBox(&cb->bullet);
            if (is_collidingc(&bullet_box, center, radius) == 1) {
                *bullet = &cb->bullet;
                return 1;
            }
        }
    }

    return 0;
}

void BulletContainer_Draw(BulletContainer *c, SDL_Renderer *r) {
    for (unsigned int i = 0; i < GAME_MAX_BULLETS; i++) {
        if (c->bullets[i].in_use == 1) {
            if (c->bullets[i].bullet.position.x == 0.f || c->bullets[i].bullet.position.y == 0.f) {
                printf("==> Drawing bullet zero: %u\n", i);
            }

            Bullet_Draw(&c->bullets[i].bullet, r);
        }
    }
}

void BulletContainer_Clear(BulletContainer *c) {
    BulletContainer_Init(c);
}
