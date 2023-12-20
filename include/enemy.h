#ifndef DREV_ENEMY_H
#define DREV_ENEMY_H

#include "bullet.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_enemy        Enemy;
typedef struct drev_enemy_bullet EnemyBullet;

struct drev_enemy_bullet {
    int    in_use;
    Bullet bullet;
};

struct drev_enemy {
    Vec2         position;
    Vec2         velocity;
    unsigned int health;
    double       rotation;
    Vec2         target;
    SDL_Texture *texture;
    unsigned int last_fired;
};

int  Enemy_Init(Enemy *enemy, Vec2 position, unsigned int health, SDL_Renderer *renderer);
void Enemy_Update(Enemy *enemy, BulletContainer *c, float delta);
void Enemey_Draw(const Enemy *enemy, SDL_Renderer *renderer);

void Enemy_FacePoint(Enemy *enemy, Vec2 point);

SDL_FRect Enemy_BoundingBox(Enemy *enemy);

void Enemy_Destroy(Enemy *enemy);

#endif // DREV_ENEMY_H
