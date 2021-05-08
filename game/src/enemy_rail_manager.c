#include <enemy_rail_manager.h>

int EnemyRailManager_Init(EnemyRailManager *manager, unsigned int num) {
  manager->rails = NULL;
  manager->last_add = NULL;
  manager->num_rails = 0;
  manager->num_rails = num;

  if (num < 1) {
    return 0;
  }

  manager->rails = malloc(sizeof(EnemyRail*) * num);
  if (manager->rails == NULL) {
    EnemyRailManager_Destroy(manager);
    return -1;
  }

  manager->last_add = malloc(sizeof(unsigned int) * num);
  if (manager->last_add == NULL) {
    EnemyRailManager_Destroy(manager);
    return -1;
  }
  
  for (unsigned int i = 0; i < num; i++) {
    manager->rails[i] = NULL;
    manager->last_add[i] = 0;
  }

  return 0;
}

void EnemyRailManager_Update(EnemyRailManager *manager, float delta) {
  for (unsigned int i = 0; i < manager->num_rails; i++) {
    EnemyRail_Update(manager->rails[i], delta);
    manager->last_add[i] += (unsigned int)(delta * 1000.f);

    if (manager->last_add[i] > 3000 + 250 * i) {
      EnemyRail_Add_Enemy(manager->rails[i]);
      manager->last_add[i] = 0;
    }
  }
}

void EnemyRailManager_Draw(EnemyRailManager *manager, SDL_Renderer *renderer) {
  for (unsigned int i = 0; i < manager->num_rails; i++) {
    EnemyRail_Draw(manager->rails[i], renderer);
  }
}

void EnemyRailManager_Destroy(EnemyRailManager *manager) {
  if (manager->last_add != NULL) {
    free(manager->last_add);
  }

  if (manager->rails != NULL) {
    free(manager->rails);
  }

  manager->num_rails = 0;

  return;
}