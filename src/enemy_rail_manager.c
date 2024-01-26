#include "enemy_rail_manager.h"

#include "bullet.h"
#include "enemy.h"
#include "enemy_rail.h"
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

    rc.Type               = RailConfigType_Static;
    rc.num_enemies        = num_enemies;
    rc.random_rc.next     = 0;
    rc.random_rc.rand_min = min;
    rc.random_rc.rand_max = max;

    return rc;
}

/*
 - consistent start/end position? Yes?
 - rate/speed from start to end? Consistent? Stops at end until enemies are 0?
    - rail speed based on difficulty
    - enemy speed based on difficulty
 - do enemies die at the end or "bounce back"?
 - is each level scripted or randomized?
 */

// int killRail(RailPosition pos) {
// }

// after this all compiles..if something doesn't work right,
// its probably these numbers below...
#define rail_buffer      10.f
#define offscreen_buffer 100.f

/*
Vec2 start_positions[RailPosition_Count] = {
    {.x = -offscreen_buffer, .y = offscreen_buffer},
    {.x = -offscreen_buffer, .y = GAME_HEIGHT - rail_buffer},
    {.x = rail_buffer, .y = -offscreen_buffer},
    {.x = GAME_WIDTH - rail_buffer, .y = -offscreen_buffer}};
Vec2 end_positions[RailPosition_Count] = {
    {.x = GAME_WIDTH + offscreen_buffer, .y = rail_buffer},
    {.x = GAME_WIDTH + offscreen_buffer, .y = GAME_HEIGHT - rail_buffer},
    {.x = rail_buffer, .y = GAME_HEIGHT + offscreen_buffer},
    {.x = GAME_WIDTH - rail_buffer, .y = GAME_HEIGHT + offscreen_buffer}};
Vec2 stop_positions[RailPosition_Count] = {
    {.x = -offscreen_buffer, .y = offscreen_buffer},
    {.x = -offscreen_buffer, .y = GAME_HEIGHT - offscreen_buffer},
    {.x = offscreen_buffer, .y = -offscreen_buffer},
    {.x = GAME_WIDTH - offscreen_buffer, .y = -offscreen_buffer}};
*/

const Vec2 start_positions[RailPosition_Count] = {
    {-100.f, 10.f},
    {-100.f, 590.f},
    {10.f, -100.f},
    {790.f, -100.f},
};
const Vec2 end_positions[RailPosition_Count] = {
    {900.f, 10.f},
    {900.f, 590.f},
    {10.f, 700.f},
    {790.f, 700.f},
};
const Vec2 stop_positions[RailPosition_Count] = {{-100.f, 100.f},
                                                 {-100.f, 500.f},
                                                 {100.f, -100.f},
                                                 {700.f, -100.f}};

const Vec2 velocities[RailPosition_Count] = {
    {.x = 0.f, 5.f},
    {.x = 0.f, -5.f},
    {.x = 5.f, 0.f},
    {.x = -5.f, 0.f},
};

void ManagedEnemyRail_Destroy(ManagedEnemyRail *managed_rail) {
    managed_rail->state = RailState_Idle;
}

int EnemyRailManager_Init(EnemyRailManager *manager) {
    for (unsigned int i = 0; i < RailPosition_Count; i++) {
        manager->rails[i].state = RailState_Idle;
    }

    return 0;
}

int EnemyRailManager_StartRail(EnemyRailManager *manager, RailPosition pos, RailConfig cfg) {
    manager->rails[pos].state = RailState_Running;

    manager->rails[pos].config = cfg;

    EnemyRail_Init(&manager->rails[pos].rail,
                   start_positions[pos],
                   end_positions[pos],
                   velocities[pos],
                   stop_positions[pos]);

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
                }

                rail->config.static_rc.next = rail->config.static_rc.rate_ms;
            }

            break;
        }
        case RailConfigType_Random:
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
