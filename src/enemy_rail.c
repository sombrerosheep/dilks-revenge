#include "enemy_rail.h"

#include <stdio.h>

static void EnemyRail_InitEnemies(EnemyRail *rail) {
    memset(&rail->enemies, 0, sizeof(RailEnemy) * RAIL_MAX_ENEMIES);

    return;
}

int EnemyRail_Init(EnemyRail *rail, Vec2 start, Vec2 end, Vec2 velocity, Vec2 stop) {
    rail->start         = start;
    rail->end           = end;
    rail->slope         = Vec2_Normalize((Vec2){end.x - start.x, end.y - start.y});
    rail->velocity      = velocity;
    rail->stop          = stop;
    rail->alive_enemies = 0;

    EnemyRail_InitEnemies(rail);

    return 0;
}

int EnemyRail_Add_Enemy(EnemyRail *rail, SDL_Renderer *renderer) {
    for (unsigned int i = 0; i < RAIL_MAX_ENEMIES; i++) {
        if (rail->enemies[i].in_use == 1) {
            continue;
        }

        Enemy_Init(&rail->enemies[i].enemy, rail->start, 100, renderer);
        rail->enemies[i].enemy.velocity = rail->slope;

        rail->enemies[i].in_use = 1;
        rail->alive_enemies++;

        return 0;
    }

    return -1;
}

int EnemyRail_Remove_Enemy(EnemyRail *rail, Enemy *enemy) {
    for (unsigned int i = 0; i < RAIL_MAX_ENEMIES; i++) {
        if (rail->enemies[i].in_use == 1 && &rail->enemies[i].enemy == enemy) {
            rail->enemies[i].in_use = 0;

            if (rail->alive_enemies > 0) {
                rail->alive_enemies--;
            }

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
    Vec2 base_transform = (Vec2){.x = rail->velocity.x * delta, .y = rail->velocity.y * delta};

    rail->start.x += base_transform.x;
    rail->start.y += base_transform.y;
    rail->end.x += base_transform.x;
    rail->end.y += base_transform.y;

    if ((rail->velocity.x > 0.f && rail->start.x > rail->stop.x) ||
        (rail->velocity.x < 0.f && rail->start.x < rail->stop.x)) {
        rail->start.x    = rail->stop.x;
        rail->velocity.x = 0.f;
    }
    if ((rail->velocity.y > 0.f && rail->start.y > rail->stop.y) ||
        (rail->velocity.y < 0.f && rail->start.y < rail->stop.y)) {
        rail->start.x    = rail->stop.x;
        rail->velocity.y = 0.f;
    }

    for (unsigned int i = 0; i < RAIL_MAX_ENEMIES; i++) {
        RailEnemy *enemy = &rail->enemies[i];

        if (enemy->in_use == 1 && enemy->enemy.health <= 0.f) {
            enemy->in_use = 0;
            rail->alive_enemies--;
            continue;
        }

        if (enemy->in_use == 1) {
            { // check if off the rail's end
                // todo: this will be different for rail modes
                if (rail->slope.x > 0.f && rail->slope.y == 0.f) {
                    // L->R
                    if (enemy->enemy.position.x > rail->end.x) {
                        EnemyRail_Remove_Enemy(rail, &enemy->enemy);
                        continue;
                    }
                } else if (rail->slope.x < 0.f && rail->slope.y == 0.f) {
                    // R->L
                    if (enemy->enemy.position.x < rail->end.x) {
                        EnemyRail_Remove_Enemy(rail, &enemy->enemy);
                        continue;
                    }
                } else if (rail->slope.x == 0.f && rail->slope.y > 0.f) {
                    // U->D
                    if (enemy->enemy.position.y > rail->end.y) {
                        EnemyRail_Remove_Enemy(rail, &enemy->enemy);
                        continue;
                    }
                } else if (rail->slope.x == 0.f && rail->slope.y < 0.f) {
                    // D->U
                    if (enemy->enemy.position.y < rail->end.y) {
                        EnemyRail_Remove_Enemy(rail, &enemy->enemy);
                        continue;
                    }
                }

                Enemy_Update(&enemy->enemy, c, delta, &base_transform);
            }
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
