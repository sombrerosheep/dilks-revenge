#ifndef DREV_BULLET_H
#define DREV_BULLET_H

#include "vec.h"

#include <SDL.h>

#define PLAYER_MAX_BULLETS 128
#define ENEMY_MAX_BULLETS  (16 * 50)
#define GAME_MAX_BULLETS   ((PLAYER_MAX_BULLETS) + (ENEMY_MAX_BULLETS))

typedef struct drev_bullet Bullet;

typedef enum {
    BulletType_Player = 0,
    BulletType_Enemy,

    BulletType_Count
} BulletType;

struct drev_bullet {
    BulletType type;
    int        in_use;
    Vec2       position;
    Vec2       velocity;
    float      health;
};

#define Bullet_Zero                                                             \
    (Bullet) {                                                                  \
        .health = 0, .in_use = 0, .position = Vec2_Zero, .velocity = Vec2_Zero, \
        .type = BulletType_Count                                                \
    }

void Bullet_Init(Bullet *bullet, BulletType type, Vec2 pos, Vec2 vel);
void Bullet_Update(Bullet *bullet, float delta);
void Bullet_Draw(const Bullet *bullet, SDL_Renderer *renderer);

// todo: inline?
SDL_FRect Bullet_BoundingBox(Bullet *bullet);

typedef struct drev_bullet_container BulletContainer;

struct drev_bullet_container {
    Bullet bullets[GAME_MAX_BULLETS];
};

int  BulletContainer_Init(BulletContainer *c);
int  BulletContainer_Add(BulletContainer *c, BulletType type, Vec2 pos, Vec2 vel);
void BulletContainer_Update(BulletContainer *c, float delta);
void BulletContainer_Draw(BulletContainer *c, SDL_Renderer *r);
void BulletContainer_Clear(BulletContainer *c);

#endif // DREV_BULLET_H
