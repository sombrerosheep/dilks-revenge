#ifndef DREV_ENEMY_RAIL_H
#define DREV_ENEMY_RAIL_H

#include "bullet.h"

#include "enemy.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_enemy_rail       EnemyRail;
typedef struct drev_enemy_rail_enemy RailEnemy;

struct drev_enemy_rail_enemy {
    int   in_use;
    Enemy enemy;
};

#define RAIL_MAX_ENEMIES 16

struct drev_enemy_rail {
    Vec2      start;
    Vec2      end;
    Vec2      slope;
    Vec2      velocity;
    Vec2      stop;
    RailEnemy enemies[RAIL_MAX_ENEMIES];
};

int  EnemyRail_Init(EnemyRail *rail, Vec2 start, Vec2 end, Vec2 velocity, Vec2 stop);
int  EnemyRail_Add_Enemy(EnemyRail *rail, SDL_Renderer *rendere);
int  EnemyRail_Remove_Enemy(EnemyRail *rail, Enemy *enemy);
void EnemyRail_SetFocus(EnemyRail *rail, Vec2 point);
void EnemyRail_Update(EnemyRail *rail, BulletContainer *c, float delta);
void EnemyRail_Draw(const EnemyRail *rail, SDL_Renderer *renderer);

void EnemyRail_Destroy(EnemyRail *rail);

#endif // DREV_ENEMY_RAIL_H
