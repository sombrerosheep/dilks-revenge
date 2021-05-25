#include <enemy_rail_manager.h>

#include <string.h>
#include <stdio.h>

typedef struct drev_managed_enemy_rail ManagedEnemyRail; 

#define RAIL_NAME_MAX   16

struct drev_managed_enemy_rail {
  EnemyRail *rail;
  char name[RAIL_NAME_MAX];
  unsigned int last_added;
  int initialized;
};

struct drev_enemy_rail_manager {
  ManagedEnemyRail *rails;
  unsigned int num_rails;
  unsigned int cap_rails;
};

static ManagedEnemyRail *EnemyRailManager_GetRegisteredRail(EnemyRailManager *manager, char *rail_name) {
  ManagedEnemyRail *managed_rail = NULL;

  for (unsigned int i = 0; i < manager->num_rails; i++) {
    if (strncmp(manager->rails[i].name, rail_name, RAIL_NAME_MAX) == 0) {
      managed_rail = &manager->rails[i];
    }
  }

  if (managed_rail == NULL) {
    printf("DEBUG :: registered rail (%s) not found\n", rail_name);
  }

  return managed_rail;
}

void ManagedEnemyRail_Destroy(ManagedEnemyRail *managed_rail) {
  managed_rail->name[0] = '\0';
  managed_rail->initialized = 0;
  managed_rail->rail = NULL;
  managed_rail->last_added = 0;
}

EnemyRailManager *EnemyRailManager_Create(unsigned int num_rails_capacity) {
  EnemyRailManager *manager = NULL;

  if ((manager = malloc(sizeof(EnemyRailManager))) == NULL) {
    return NULL;
  }

  manager->rails = NULL;
  manager->cap_rails = num_rails_capacity;
  manager->num_rails = 0;

  if (manager->cap_rails < 1) {
    return manager;
  }

  if ((manager->rails = malloc(sizeof(ManagedEnemyRail) * num_rails_capacity)) == NULL) {
    printf("ERROR :: unable to allocate memory for enemy rails\n");
    EnemyRailManager_Destroy(manager);
    return NULL;
  }

  for (unsigned int i = 0; i < manager->cap_rails; i++) {
    ManagedEnemyRail_Destroy(&manager->rails[i]);
  }

  return manager;
}

EnemyRail *EnemyRailManager_AddRail(EnemyRailManager *manager, char *rail_name, Vec2 start, Vec2 end) {
  EnemyRail *rail = NULL;

  if (manager->num_rails >= manager->cap_rails) {
    printf("ERROR :: rail manager is full!\n");
    return NULL;
  }

  rail = EnemyRailManager_GetRail(manager, rail_name);
  if (rail != NULL) {
    printf("ERROR :: a rail with name '%s' is already registered\n", rail_name);
    return NULL;
  }

  if ((rail = EnemyRail_Create(start, end)) == NULL) {
    printf("ERROR :: unable to add rail to rail manager\n");
    return NULL;
  }

  for (unsigned int i = 0; i < manager->cap_rails; i++) {
    if (manager->rails[i].initialized == 0) {
      manager->rails[i].initialized = 1;
      manager->rails[i].rail = rail;
      strncpy(manager->rails[i].name, rail_name, RAIL_NAME_MAX);

      break;
    }
  }

  manager->num_rails++;
  return rail;
}

EnemyRail *EnemyRailManager_GetRail(EnemyRailManager *manager, char* rail_name) {
  ManagedEnemyRail *managed_rail = NULL;

  managed_rail = EnemyRailManager_GetRegisteredRail(manager, rail_name);

  if (managed_rail == NULL) {
    printf("DEBUG :: rail (%s) not found\n", rail_name);
    return NULL;
  }

  return managed_rail->rail;
}

int EnemyRailManager_RemoveRail(EnemyRailManager *manager, char* rail_name) {
  ManagedEnemyRail *managed_rail = NULL;

  managed_rail = EnemyRailManager_GetRegisteredRail(manager, rail_name);

  if (managed_rail == NULL) {
    printf("DEBUG :: rail (%s) not found\n", rail_name);
    return -1;
  }

  EnemyRail_Destroy(managed_rail->rail);
  ManagedEnemyRail_Destroy(managed_rail);

  return 0;
}

void EnemyRailManager_Update(EnemyRailManager *manager, float delta) {
  for (unsigned int i = 0; i < manager->num_rails; i++) {
    if (manager->rails[i].initialized == 1) {
      EnemyRail_Update(manager->rails[i].rail, delta);
      manager->rails[i].last_added += (unsigned int)(delta * 1000.f);

      if (manager->rails[i].last_added > 3000 + 250 * i) {
       EnemyRail_Add_Enemy(manager->rails[i].rail);
       manager->rails[i].last_added = 0;
      }
    }
  }
}

void EnemyRailManager_Draw(EnemyRailManager *manager, SDL_Renderer *renderer) {
  for (unsigned int i = 0; i < manager->num_rails; i++) {
    if (manager->rails[i].initialized == 1) {
      EnemyRail_Draw(manager->rails[i].rail, renderer);
    }
  }
}

void EnemyRailManager_Destroy(EnemyRailManager *manager) {
  if (manager->rails != NULL) {
    for (unsigned int i = 0; i < manager->num_rails; i++) {
      ManagedEnemyRail_Destroy(&manager->rails[i]);
    }

    free(manager->rails);
  }

  manager->num_rails = 0;

  return;
}