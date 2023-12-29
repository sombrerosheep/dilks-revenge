#include "enemy_rail_manager.h"

#include "bullet.h"
#include "random.h"

#include <stdio.h>
#include <string.h>

typedef struct drev_managed_enemy_rail ManagedEnemyRail;

/*
typedef enum RailPosition {
    RailTop = 0,
    RailBottom,
    RailLeft,
    RailRight,

    RailIndex_Count
} RailIndex;
*/

#define RAIL_MANAGER_MAX_RAILS 4

struct drev_managed_enemy_rail {
    RailManagerPlacementConfig placement;
    EnemyRail                  rail;
    unsigned int               last_added;
    int                        in_use;
};

struct drev_enemy_rail_manager {
    ManagedEnemyRail rails[RAIL_MANAGER_MAX_RAILS];
};

void ManagedEnemyRail_Destroy(ManagedEnemyRail *managed_rail) {
    managed_rail->in_use     = 0;
    managed_rail->last_added = 0;
}

EnemyRailManager *EnemyRailManager_Create() {
    EnemyRailManager *manager = NULL;

    if ((manager = malloc(sizeof(EnemyRailManager))) == NULL) {
        return NULL;
    }

    for (unsigned int i = 0; i < RAIL_MANAGER_MAX_RAILS; i++) {
        manager->rails[i].in_use = 0;
    }

    return manager;
}

int EnemyRailManager_AddRail(EnemyRailManager *manager, Vec2 start, Vec2 end) {
    for (unsigned int i = 0; i < RAIL_MANAGER_MAX_RAILS; i++) {
        if (manager->rails[i].in_use == 0) {
            ManagedEnemyRail *managed_rail = &manager->rails[i];

            EnemyRail_Init(&managed_rail->rail, start, end);
            managed_rail->in_use = 1;

            managed_rail->placement.type       = RAIL_MANAGER_PLACEMENT_TYPE_RANDOM;
            managed_rail->placement.config.min = 2000;
            managed_rail->placement.config.max = 12000;
            managed_rail->placement.config.next =
                random_get_between(managed_rail->placement.config.min,
                                   managed_rail->placement.config.max);
            managed_rail->placement.config.since = 0;

            return 0;
        }
    }

    return -1;
}

void EnemyRailManager_SetFocus(EnemyRailManager *manager, Vec2 point) {
    for (unsigned int i = 0; i < RAIL_MANAGER_MAX_RAILS; i++) {
        EnemyRail_SetFocus(&manager->rails[i].rail, point);
    }
}

void EnemyRailManager_Update(EnemyRailManager *manager,
                             BulletContainer  *c,
                             float             delta,
                             SDL_Renderer     *renderer) {
    for (unsigned int i = 0; i < RAIL_MANAGER_MAX_RAILS; i++) {
        ManagedEnemyRail *managed_rail = &manager->rails[i];

        if (managed_rail->in_use == 1) {
            managed_rail->placement.config.since += (unsigned int)(delta * 1000);

            if (managed_rail->placement.config.since > managed_rail->placement.config.next) {
                managed_rail->placement.config.since = 0;
                managed_rail->placement.config.next =
                    random_get_between(managed_rail->placement.config.min,
                                       managed_rail->placement.config.max);

                switch (managed_rail->placement.type) {
                    case RAIL_MANAGER_PLACEMENT_TYPE_RANDOM: {
                        unsigned int rail_index = random_get() % RAIL_MANAGER_MAX_RAILS;
                        EnemyRail_Add_Enemy(&manager->rails[rail_index].rail, renderer);

                        break;
                    };
                    default: {
                        printf("Unsupported placement type: %u\n", managed_rail->placement.type);
                        break;
                    }
                }
            }

            EnemyRail_Update(&managed_rail->rail, c, delta);
        }
    }
}

void EnemyRailManager_Draw(EnemyRailManager *manager, SDL_Renderer *renderer) {
    for (unsigned int i = 0; i < RAIL_MANAGER_MAX_RAILS; i++) {
        if (manager->rails[i].in_use == 1) {
            EnemyRail_Draw(&manager->rails[i].rail, renderer);
        }
    }
}

void EnemyRailManager_Destroy(EnemyRailManager *manager) {
    for (unsigned int i = 0; i < RAIL_MANAGER_MAX_RAILS; i++) {
        ManagedEnemyRail_Destroy(&manager->rails[i]);
    }

    return;
}
