#include "enemy_rail_manager.h"

#include "bullet.h"
#include "enemy.h"
#include "enemy_rail.h"
#include "globals.h"
#include "random.h"

#include <stdio.h>
#include <string.h>

RailConfig RailConfig_NewStatic(unsigned int num_enemies, unsigned int rate_ms) {
    RailConfig rc;

    rc.Type              = RailConfigType_Static;
    rc.num_enemies       = num_enemies;
    rc.static_rc.next    = 0;
    rc.static_rc.rate_ms = rate_ms;

    return rc;
}

RailConfig RailConfig_NewRandom(unsigned int num_enemies, unsigned int min, unsigned int max) {
    RailConfig rc;

    rc.Type               = RailConfigType_Random;
    rc.num_enemies        = num_enemies;
    rc.random_rc.next     = 0;
    rc.random_rc.rand_min = min;
    rc.random_rc.rand_max = max;

    return rc;
}

#define rail_buffer      10.f
#define offscreen_buffer 100.f
#define top_left_x       (0.f)
#define top_left_y       (0.f)
#define top_right_x      ((float)GAME_WIDTH)
#define top_right_y      (0.f)
#define bottom_left_x    (0.f)
#define bottom_left_y    ((float)GAME_HEIGHT)
#define bottom_right_x   ((float)GAME_WIDTH)
#define bottom_right_y   ((float)GAME_HEIGHT)

struct rail_definition {
    Vec2 start;
    Vec2 end;
    Vec2 stop;
    Vec2 velocity;
};

struct rail_definition rail_defs[RailPosition_Count] = { //
    [RailPosition_Top] =
        {
            .start    = {.x = -100.f, .y = 10.f},
            .end      = {.x = 900.f, .y = 10.f},
            .stop     = {.x = -100.f, .y = 100.f},
            .velocity = {.x = 0.f, .y = 5.f},
        },
    [RailPosition_Bottom] =
        {
            .start    = {.x = -100.f, .y = 590.f},
            .end      = {900.f, 590.f},
            .stop     = {-100.f, 500.f},
            .velocity = {.x = 0.f, -5.f},
        },
    [RailPosition_Left] =
        {
            .start    = {.x = 10.f, .y = -100.f},
            .end      = {10.f, 700.f},
            .stop     = {100.f, -100.f},
            .velocity = {.x = 5.f, 0.f},
        },
    [RailPosition_Right] = {
        .start    = {.x = 790.f, .y = -100.f},
        .end      = {790.f, 700.f},
        .stop     = {700.f, -100.f},
        .velocity = {.x = -5.f, 0.f},
    }};

void ManagedEnemyRail_Destroy(ManagedEnemyRail *managed_rail) {
    managed_rail->state = RailState_Idle;
}

int EnemyRailManager_Init(EnemyRailManager *manager) {
    for (unsigned int i = 0; i < RailPosition_Count; i++) {
        manager->rails[i].state = RailState_Idle;
    }

    manager->active_rails = 0;

    return 0;
}

int EnemyRailManager_StartRail(EnemyRailManager *manager, RailPosition pos, RailConfig cfg) {
    if (manager->rails[pos].state != RailState_Idle) {
        return -1;
    }

    manager->rails[pos].state = RailState_Running;

    manager->rails[pos].config = cfg;

    struct rail_definition def = rail_defs[pos];

    EnemyRail_Init(&manager->rails[pos].rail, def.start, def.end, def.velocity, def.stop);

    manager->active_rails++;

    return 0;
}

int EnemyRailManager_KillRail(EnemyRailManager *manager, RailPosition pos) {
    manager->rails[pos].state = RailState_Dieing;

    return 0;
}

void EnemyRailManager_SetFocus(EnemyRailManager *manager, Vec2 point) {
    for (unsigned int i = 0; i < RailPosition_Count; i++) {
        EnemyRail_SetFocus(&manager->rails[i].rail, point);
    }
}

static void EnemyRailManager_UpdateRunningRail(ManagedEnemyRail *rail,
                                               BulletContainer  *c,
                                               Vec2              focus,
                                               float             delta,
                                               SDL_Renderer     *renderer) {
    switch (rail->config.Type) {
        case RailConfigType_Static: {
            rail->config.static_rc.next -= (unsigned int)(delta * 1000.f);

            if (rail->config.num_enemies > 0 && rail->config.static_rc.next <= 0) {
                EnemyRail_Add_Enemy(&rail->rail, renderer);
                rail->config.num_enemies--;

                if (rail->config.num_enemies <= 0) {
                    rail->state = RailState_Dieing;
                    break;
                }

                rail->config.static_rc.next = rail->config.static_rc.rate_ms;
            }

            break;
        }
        case RailConfigType_Random: {
            rail->config.random_rc.next -= (unsigned int)(delta * 1000.f);

            if (rail->config.num_enemies > 0 && rail->config.random_rc.next <= 0) {
                EnemyRail_Add_Enemy(&rail->rail, renderer);
                rail->config.num_enemies--;

                if (rail->config.num_enemies <= 0) {
                    rail->state = RailState_Dieing;
                    break;
                }

                unsigned int next =
                    (unsigned int)random_get_between(rail->config.random_rc.rand_min,
                                                     rail->config.random_rc.rand_max);
                rail->config.random_rc.next = next;
            }
            break;
        }
        default: {
            printf("RailConfigType not implemented: %d\n", rail->config.Type);
            break;
        }
    }

    EnemyRail_SetFocus(&rail->rail, focus);
    EnemyRail_Update(&rail->rail, c, delta);
}

void EnemyRailManager_Update(EnemyRailManager *manager,
                             BulletContainer  *c,
                             Vec2              focus,
                             float             delta,
                             SDL_Renderer     *renderer) {
    for (unsigned int i = 0; i < RailPosition_Count; i++) {
        ManagedEnemyRail *managed_rail = &manager->rails[i];

        switch (managed_rail->state) {
            case RailState_Running: {
                EnemyRailManager_UpdateRunningRail(managed_rail, c, focus, delta, renderer);
                break;
            }
            case RailState_Dieing:
                if (managed_rail->rail.alive_enemies == 0) {
                    managed_rail->state = RailState_Idle;
                    manager->active_rails--;
                }

                EnemyRail_SetFocus(&managed_rail->rail, focus);
                EnemyRail_Update(&managed_rail->rail, c, delta);
                break;
            case RailState_Starting:
            case RailState_Idle:
            default: {
                // skipping
                break;
            }
        }
    }
}

void EnemyRailManager_Draw(EnemyRailManager *manager, SDL_Renderer *renderer) {
    for (unsigned int i = 0; i < RailPosition_Count; i++) {
        if (manager->rails[i].state != RailState_Idle) {
            EnemyRail_Draw(&manager->rails[i].rail, renderer);
        }
    }
}

void EnemyRailManager_Destroy(EnemyRailManager *manager) {
    for (unsigned int i = 0; i < RailPosition_Count; i++) {
        ManagedEnemyRail_Destroy(&manager->rails[i]);
    }

    return;
}
