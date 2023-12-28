#include "enemy_rail.h"

#include <stdio.h>

typedef struct drev_enemy_rail_enemy RailEnemy;

struct drev_enemy_rail_enemy {
    int   in_use;
    Enemy enemy;
};

#define RAIL_MAX_ENEMIES 16

struct drev_enemy_rail {
    Vec2      start;
    Vec2      end;
    Vec2      slope;
    RailEnemy enemies[RAIL_MAX_ENEMIES];
};

static void EnemyRail_InitEnemies(EnemyRail *rail) {
    for (unsigned int i = 0; i < RAIL_MAX_ENEMIES; i++) {
        rail->enemies[i].in_use = 0;
    }

    return;
}

EnemyRail *EnemyRail_Create(Vec2 start, Vec2 end) {
    EnemyRail *rail = NULL;

    if ((rail = malloc(sizeof(EnemyRail))) == NULL) {
        printf("ERROR :: Unable to allocate memory for rail\n");
        return NULL;
    }

    rail->start = start;
    rail->end   = end;
    rail->slope = (Vec2){end.x - start.x, end.y - start.y};

    EnemyRail_InitEnemies(rail);

    return rail;
}

int EnemyRail_Add_Enemy(EnemyRail *rail, SDL_Renderer *renderer) {
    for (unsigned int i = 0; i < RAIL_MAX_ENEMIES; i++) {
        if (rail->enemies[i].in_use == 1) {
            continue;
        }

        Enemy_Init(&rail->enemies[i].enemy, rail->start, 100, renderer);
        Vec2 slope_norm                 = Vec2_Normalize(rail->slope);
        rail->enemies[i].enemy.velocity = slope_norm;

        rail->enemies[i].in_use = 1;

        return 0;
    }

    return -1;
}

int EnemyRail_Remove_Enemy(EnemyRail *rail, Enemy *enemy) {
    for (unsigned int i = 0; i < RAIL_MAX_ENEMIES; i++) {
        if (rail->enemies[i].in_use == 1 && &rail->enemies[i].enemy == enemy) {
            rail->enemies[i].in_use = 0;
            return 0;
        }
    }

    printf("attempted to remove rail enemy but failed\n");
    return -1;
}

void EnemyRail_SetFocus(EnemyRail *rail, Vec2 point) {
    for (unsigned int i = 0; i < RAIL_MAX_ENEMIES; i++) {
        if (rail->enemies[i].in_use == 1) {
            Enemy_FacePoint(&rail->enemies[i].enemy, point);
        }
    }
}

void EnemyRail_Update(EnemyRail *rail, BulletContainer *c, float delta) {
    for (unsigned int i = 0; i < RAIL_MAX_ENEMIES; i++) {
        if (rail->enemies[i].in_use == 1) {
            if (rail->slope.x > 0.f && rail->slope.y == 0.f) {
                // L->R
                if (rail->enemies[i].enemy.position.x > rail->end.x) {
                    EnemyRail_Remove_Enemy(rail, &rail->enemies[i].enemy);
                    continue;
                }
            } else if (rail->slope.x < 0.f && rail->slope.y == 0.f) {
                // R->L
                if (rail->enemies[i].enemy.position.x < rail->end.x) {
                    EnemyRail_Remove_Enemy(rail, &rail->enemies[i].enemy);
                    continue;
                }
            } else if (rail->slope.x == 0.f && rail->slope.y > 0.f) {
                // U->D
                if (rail->enemies[i].enemy.position.y > rail->end.y) {
                    EnemyRail_Remove_Enemy(rail, &rail->enemies[i].enemy);
                    continue;
                }
            } else if (rail->slope.x == 0.f && rail->slope.y < 0.f) {
                // D->U
                if (rail->enemies[i].enemy.position.y < rail->end.y) {
                    EnemyRail_Remove_Enemy(rail, &rail->enemies[i].enemy);
                    continue;
                }
            }

            Enemy_Update(&rail->enemies[i].enemy, c, delta);
        }
    }
}

void EnemyRail_Draw(const EnemyRail *rail, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, 0xFF);
    SDL_RenderDrawLineF(renderer, rail->start.x, rail->start.y, rail->end.x, rail->end.y);

    for (unsigned int i = 0; i < RAIL_MAX_ENEMIES; i++) {
        if (rail->enemies[i].in_use == 1) {
            Enemey_Draw(&rail->enemies[i].enemy, renderer);
        }
    }
}

void EnemyRail_Destroy(EnemyRail *rail) {
    rail->start = Vec2_Zero;
    rail->end   = Vec2_Zero;

    EnemyRail_InitEnemies(rail);
}
