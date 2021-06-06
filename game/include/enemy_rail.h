#ifndef DREV_ENEMY_RAIL_H
#define DREV_ENEMY_RAIL_H

#include <vec.h>
#include <enemy.h>
#include <SDL.h>

typedef struct _enemy_rail EnemyRail;

EnemyRail *EnemyRail_Create(Vec2 start, Vec2 end);
int EnemyRail_Add_Enemy(EnemyRail *rail, SDL_Renderer *rendere);
int EnemyRail_Remove_Enemy(EnemyRail *rail, Enemy *enemy);
void EnemyRail_Update(EnemyRail *rail, float delta);
void EnemyRail_Draw(const EnemyRail *rail, SDL_Renderer *renderer);

void EnemyRail_Destroy(EnemyRail *rail);

#endif // DREV_ENEMY_RAIL_H