#ifndef DREV_ENEMY_RAIL_MANAGER_H
#define DREV_ENEMY_RAIL_MANAGER_H

#include "enemy.h"
#include "enemy_rail.h"
#include "globals.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_enemy_rail_manager EnemyRailManager;
typedef struct drev_managed_enemy_rail ManagedEnemyRail;
typedef struct drev_static_rail_config StaticRailConfig;
typedef struct drev_random_rail_config RandomRailConfig;
typedef struct drev_rail_config        RailConfig;

typedef enum RailPosition {
    RailPosition_Top = 0,
    RailPosition_Bottom,
    RailPosition_Left,
    RailPosition_Right,

    RailPosition_Count
} RailPosition;

typedef enum RailState {
    RailState_Idle,
    RailState_Starting,
    RailState_Running,
    RailState_Dieing,

    RailState_Count
} RailState;

typedef enum RailConfigType {
    RailConfigType_Static, // new enemy every x ms
    RailConfigType_Random, // spaced out every min/max ms
    // RailConfigType_Pattern, // multiple configs? a rhythm?

    RailConfigType_Count
} RailConfigType;

struct drev_static_rail_config {
    unsigned int rate_ms;
    int          next;
};

struct drev_random_rail_config {
    unsigned int rand_min;
    unsigned int rand_max;
    int          next;
};

struct drev_rail_config {
    enum RailConfigType Type;

    float num_enemies;

    union {
        StaticRailConfig static_rc;
        RandomRailConfig random_rc;
    };
};

struct drev_managed_enemy_rail {
    RailConfig config;
    RailState  state;
    EnemyRail  rail;
};

struct drev_enemy_rail_manager {
    ManagedEnemyRail rails[RailPosition_Count];
};

int EnemyRailManager_Init(EnemyRailManager *manager);

int EnemyRailManager_StartRail(EnemyRailManager *manager, RailPosition pos);
int EnemyRailManager_KillRail(EnemyRailManager *manager, RailPosition pos);

void EnemyRailManager_Update(EnemyRailManager *manager,
                             BulletContainer  *c,
                             Vec2              focus,
                             float             delta,
                             SDL_Renderer     *renderer);
void EnemyRailManager_Draw(EnemyRailManager *manager, SDL_Renderer *renderer);

void EnemyRailManager_Destroy(EnemyRailManager *manager);

#endif
