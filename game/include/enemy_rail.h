#ifndef DREV_ENEMY_RAIL_H
#define DREV_ENEMY_RAIL_H

#include <vec.h>
#include <enemy.h>
#include <SDL.h>

typedef struct _enemy_rail EnemyRail;

EnemyRail *EnemyRail_Create(Vec2 start, Vec2 end);
void EnemyRail_Update(EnemyRail *rail);
void EnemyRail_Draw(const EnemyRail *rail, SDL_Renderer *renderer);

void EnemyRail_Destroy(EnemyRail *rail);

#endif // DREV_ENEMY_RAIL_H