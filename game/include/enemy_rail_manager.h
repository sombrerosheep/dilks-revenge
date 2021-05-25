#ifndef DREV_ENEMY_RAIL_MANAGER_H
#define DREV_ENEMY_RAIL_MANAGER_H

#include <enemy_rail.h>
#include <vec.h>
#include <SDL.h>

typedef struct drev_enemy_rail_manager EnemyRailManager;

EnemyRailManager *EnemyRailManager_Create(unsigned int num_rails_capacity);
EnemyRail *EnemyRailManager_AddRail(EnemyRailManager *manager, char *rail_name, Vec2 start, Vec2 end);
EnemyRail *EnemyRailManager_GetRail(EnemyRailManager *manager, char* rail_name);
int EnemyRailManager_RemoveRail(EnemyRailManager *manager, char* rail_name);

void EnemyRailManager_Update(EnemyRailManager *manager, float delta);
void EnemyRailManager_Draw(EnemyRailManager *manager, SDL_Renderer *renderer);

void EnemyRailManager_Destroy(EnemyRailManager *manager);

#endif