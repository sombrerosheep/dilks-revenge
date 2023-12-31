#ifndef DREV_BULLET_H
#define DREV_BULLET_H

#include "vec.h"

#include <SDL.h>

#define PLAYER_MAX_BULLETS 128
#define ENEMY_MAX_BULLETS  (16 * 50)
#define GAME_MAX_BULLETS   ((PLAYER_MAX_BULLETS) + (ENEMY_MAX_BULLETS))

typedef struct drev_bullet      Bullet;
typedef struct container_bullet ContainerBullet;

typedef enum {
    BulletType_Player = 0,
    BulletType_Enemy,

    BulletType_Count
} BulletType;

struct drev_bullet {
    BulletType type;
    Vec2       position;
    Vec2       velocity;
    float      health;
};

struct container_bullet {
    int    in_use;
    Bullet bullet;
};

#define Bullet_Zero                                                                           \
    (Bullet) {                                                                                \
        .health = 0.f, .position = Vec2_Zero, .velocity = Vec2_Zero, .type = BulletType_Count \
    }

void Bullet_Init(Bullet *bullet, BulletType type, Vec2 pos, Vec2 vel);
void Bullet_Update(Bullet *bullet, float delta);
void Bullet_Draw(const Bullet *bullet, SDL_Renderer *renderer);

// todo: inline?
SDL_FRect Bullet_BoundingBox(Bullet *bullet);

typedef struct drev_bullet_container BulletContainer;

struct drev_bullet_container {
    ContainerBullet bullets[GAME_MAX_BULLETS];
};

int  BulletContainer_Init(BulletContainer *c);
int  BulletContainer_Add(BulletContainer *c, BulletType type, Vec2 pos, Vec2 vel);
void BulletContainer_Update(BulletContainer *c, float delta);

///
/// @brief determines if any bullets are colliding with the provided rect
///
/// @param c
/// @param rect rect to check collisions against
/// @param bullet pointer to a bullet that will be assigned to a colliding bullet
/// @return int 1 if a collision was found and bullet points to the colliding bullet
/// @return int 0 if no collisions occured and bullet is NULL
///
int BulletContainer_GetFirstCollision(BulletContainer *c, SDL_FRect rect, Bullet **bullet);

int  BulletContainer_GetFirstCollisionC(BulletContainer *c,
                                        Vec2             center,
                                        float            radius,
                                        Bullet         **bullet);
void BulletContainer_Draw(BulletContainer *c, SDL_Renderer *r);
void BulletContainer_Clear(BulletContainer *c);

#endif // DREV_BULLET_H
