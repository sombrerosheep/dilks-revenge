#include <enemy_rail_manager.h>

#include <random.h>

#include <string.h>
#include <stdio.h>

typedef struct drev_managed_enemy_rail ManagedEnemyRail; 

#define RAIL_NAME_MAX   16

struct drev_managed_enemy_rail {
  RailManagerPlacementConfig placement;
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
    ManagedEnemyRail *managed_rail = &manager->rails[i];

    if (managed_rail->initialized == 0) {
      managed_rail->initialized = 1;
      managed_rail->rail = rail;
      strncpy(managed_rail->name, rail_name, RAIL_NAME_MAX);

      managed_rail->placement.type = RAIL_MANAGER_PLACEMENT_TYPE_RANDOM;
      managed_rail->placement.config.min = 300;
      managed_rail->placement.config.max = 900;
      managed_rail->placement.config.next = 
        random_get_between(
          managed_rail->placement.config.min,
          managed_rail->placement.config.max
        );
      managed_rail->placement.config.since = 0;

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

void EnemyRailManager_SetPlacementConfig(
  EnemyRailManager *manager,
  char *rail_name,
  RailManagerPlacementConfig config
) {
  ManagedEnemyRail *managed_rail = EnemyRailManager_GetRegisteredRail(manager, rail_name);
  managed_rail->placement = config;
  managed_rail->placement.config.next = random_get_between(
    managed_rail->placement.config.min,
    managed_rail->placement.config.max
  );
  managed_rail->placement.config.since = 0;
}


void EnemyRailManager_Update(EnemyRailManager *manager, float delta, SDL_Renderer *renderer) {
  for (unsigned int i = 0; i < manager->num_rails; i++) {
    ManagedEnemyRail *managed_rail = &manager->rails[i];

    if (managed_rail->initialized == 1) {
        managed_rail->placement.config.since += (unsigned int)(delta * 1000);

      if (managed_rail->placement.config.since > managed_rail->placement.config.next) {
        managed_rail->placement.config.since = 0;
        managed_rail->placement.config.next = random_get_between(
          managed_rail->placement.config.min,
          managed_rail->placement.config.max
        );

        switch (managed_rail->placement.type) {
          case RAIL_MANAGER_PLACEMENT_TYPE_RANDOM: {
            unsigned int rail_index = random_get() % manager->num_rails;
            EnemyRail_Add_Enemy(manager->rails[rail_index].rail, renderer);

            break;
          };
          default: {
            printf("Unsupported placement type: %u\n", managed_rail->placement.type);
            break;
          }
        }
      }

      EnemyRail_Update(managed_rail->rail, delta);
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