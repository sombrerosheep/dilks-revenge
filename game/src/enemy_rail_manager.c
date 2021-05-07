#include <enemy_rail_manager.h>

void EnemyRailManager_Init(EnemyRailManager *manager, unsigned int num) {
  manager->rails = NULL;
  manager->num_rails = 0;

  if (num < 1) {
    return;
  }

  manager->rails = malloc(sizeof(EnemyRail*) * num);
  if (manager->rails != NULL) {
    manager->num_rails = num;
  }

  return;
}

void EnemyRailManager_Update(EnemyRailManager *manager, float delta) {
  for (unsigned int i = 0; i < manager->num_rails; i++) {
    EnemyRail_Update(manager->rails[i], delta);
  }
}

void EnemyRailManager_Draw(EnemyRailManager *manager, SDL_Renderer *renderer) {
  for (unsigned int i = 0; i < manager->num_rails; i++) {
    EnemyRail_Draw(manager->rails[i], renderer);
  }
}