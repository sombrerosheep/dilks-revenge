#ifndef DREV_ENEMY_RAIL_MANAGER_H
#define DREV_ENEMY_RAIL_MANAGER_H

#include <enemy_rail.h>
#include <SDL.h>

typedef struct _enemy_rail_manager EnemyRailManager;

struct _enemy_rail_manager {
  EnemyRail **rails;
  unsigned int num_rails;
};

void EnemyRailManager_Init(EnemyRailManager *manager, unsigned int num);
void EnemyRailManager_Update(EnemyRailManager *manager, float delta);
void EnemyRailManager_Draw(EnemyRailManager *manager, SDL_Renderer *renderer);

#endif