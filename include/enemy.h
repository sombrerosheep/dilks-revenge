#ifndef DREV_ENEMY_H
#define DREV_ENEMY_H

#include <bullet.h>
#include <vec.h>

#include <SDL.h>

#define ENEMY_MAX_BULLETS 16

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
    EnemyBullet  bullets[ENEMY_MAX_BULLETS];
};

int  Enemy_Init(Enemy *enemy, Vec2 position, unsigned int health, SDL_Renderer *renderer);
void Enemy_Update(Enemy *enemy, float delta);
void Enemey_Draw(const Enemy *enemy, SDL_Renderer *renderer);

void Enemy_FacePoint(Enemy *enemy, Vec2 point);

void Enemy_Destroy(Enemy *enemy);

#endif // DREV_ENEMY_H
