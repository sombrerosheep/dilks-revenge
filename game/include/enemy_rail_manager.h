#ifndef DREV_ENEMY_RAIL_MANAGER_H
#define DREV_ENEMY_RAIL_MANAGER_H

#include <enemy_rail.h>
#include <vec.h>
#include <SDL.h>

typedef struct drev_enemy_rail_manager EnemyRailManager;
typedef struct drev_rail_manager_placement_config RailManagerPlacementConfig;

typedef enum drev_rail_manager_placement_type {
  RAIL_MANAGER_PLACEMENT_TYPE_RANDOM, // each rail gets an enemy at random times
  RAIL_MANAGER_PLACEMENT_TYPE_STATIC, // add enemy at set interval

  // putting a different placement type on each rail doesn't make sense with any of
  // the spread things
  // RAIL_MANAGER_PLACEMENT_TYPE_RANDOM_SPREAD, // random rail gets enemy at random timing
  // RAIL_MANAGER_PLACEMENT_TYPE_STATIC_SPREAD // add enemy to random rail at set interval
} RailManagerPlacementType;
typedef struct drev_rail_manager_placement_params RailManagerPlacementParams;

struct drev_rail_manager_placement_params {
  unsigned int min;
  unsigned int max;
  unsigned int next;
  unsigned int since;
};

struct drev_rail_manager_placement_config {
  RailManagerPlacementType type;
  RailManagerPlacementParams config;
};

EnemyRailManager *EnemyRailManager_Create(unsigned int num_rails_capacity);
EnemyRail *EnemyRailManager_AddRail(EnemyRailManager *manager, char *rail_name, Vec2 start, Vec2 end);
EnemyRail *EnemyRailManager_GetRail(EnemyRailManager *manager, char* rail_name);
int EnemyRailManager_RemoveRail(EnemyRailManager *manager, char* rail_name);

void EnemyRailManager_SetPlacementConfig(
  EnemyRailManager *manager,
  char *rail_name,
  RailManagerPlacementConfig config
);

void EnemyRailManager_SetFocus(EnemyRailManager *manager, Vec2 point);

void EnemyRailManager_Update(EnemyRailManager *manager, float delta, SDL_Renderer *renderer);
void EnemyRailManager_Draw(EnemyRailManager *manager, SDL_Renderer *renderer);

void EnemyRailManager_Destroy(EnemyRailManager *manager);

#endif