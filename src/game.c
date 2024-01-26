#include "game.h"

#include "bullet.h"
#include "clock.h"
#include "collisions.h"
#include "enemy.h"
#include "enemy_rail.h"
#include "enemy_rail_manager.h"
#include "game_input.h"
#include "player.h"
#include "random.h"

#include <stdio.h>

struct drev_game {
    Player           player;
    BulletContainer  bullets;
    EnemyRailManager rail_manager;
    GameInput        controller;
};

static void Game_Collisions(Game *game) {
    // collisions
    // player to bullets
    Bullet *colliding_bullet = NULL;
    if (BulletContainer_GetFirstCollision(&game->bullets,
                                          Player_BoundingBox(&game->player),
                                          &colliding_bullet) == 1) {
        // printf("Resolving player bullet collision\n");
        resolve_collision_player_bullet(&game->player, colliding_bullet);
    }
    // enemies to bullets
    colliding_bullet = NULL;
    for (unsigned int i = 0; i < RailPosition_Count; i++) {
        ManagedEnemyRail *rail = &game->rail_manager.rails[i];

        if (rail->state == RailState_Idle) {
            continue;
        }

        for (unsigned int j = 0; j < RAIL_MAX_ENEMIES; j++) {
            RailEnemy *enemy = &rail->rail.enemies[j];

            if (enemy->in_use != 1) {
                continue;
            }

            if (BulletContainer_GetFirstCollision(&game->bullets,
                                                  Enemy_BoundingBox(&enemy->enemy),
                                                  &colliding_bullet) == 1) {
                // printf("Resolving enemy bullet collision\n");
                resolve_collision_enemy_bullet(&enemy->enemy, colliding_bullet);
            }
        }
    }

    // shield to bullets
    colliding_bullet = NULL;
    if (game->player.shield.health > 0.f) {
        if (BulletContainer_GetFirstCollisionC(&game->bullets,
                                               game->player.shield.position,
                                               game->player.shield.radius,
                                               &colliding_bullet) == 1) {
            // printf("Resolving shield bullet collision\n");
            resolve_collision_shield_bullet(&game->player.shield, colliding_bullet);
        }
    }

    // enemies to player
    for (unsigned int i = 0; i < RailPosition_Count; i++) {
        ManagedEnemyRail *rail = &game->rail_manager.rails[i];

        if (rail->state == RailState_Idle) {
            continue;
        }

        for (unsigned int j = 0; j < RAIL_MAX_ENEMIES; j++) {
            RailEnemy *enemy = &rail->rail.enemies[j];

            if (enemy->in_use != 1) {
                continue;
            }

            SDL_FRect enemy_bb  = Enemy_BoundingBox(&enemy->enemy);
            SDL_FRect player_bb = Player_BoundingBox(&game->player);

            if (is_colliding(&player_bb, &enemy_bb) == 1) {
                resolve_collision_player_enemy(&game->player, &enemy->enemy);
            }
        }
    }

    // bullets to bullets
    BulletContainer_HandleBulletBulletCollisions(&game->bullets);
}

static void Game_Update(Game *game, System *sys, Frame delta) {
    if (game->rail_manager.active_rails < 2) {
        int rate        = random_get_between(800, 1500);
        int num_enemies = random_get_between(3, 10);

        int        rail = random_get_between(0, RailPosition_Count - 1);
        RailConfig cfg  = RailConfig_NewStatic(num_enemies, rate);

        while (EnemyRailManager_StartRail(&game->rail_manager, rail, cfg) != 0) {
            rail++;

            if (rail >= RailPosition_Count) {
                rail = 0;
            }
        }
    }

    Controller_Update(&game->controller, sys);

    Player_Update(&game->player, &game->controller, &game->bullets, delta.sec);

    EnemyRailManager_Update(&game->rail_manager,
                            &game->bullets,
                            game->player.position,
                            delta.sec,
                            sys->renderer);

    BulletContainer_Update(&game->bullets, delta.sec);

    Game_Collisions(game);
}

static void Game_Draw(Game *game, System *sys) {
    SDL_SetRenderDrawColor(sys->renderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderClear(sys->renderer);

    BulletContainer_Draw(&game->bullets, sys->renderer);
    Player_Draw(&game->player, sys->renderer);
    EnemyRailManager_Draw(&game->rail_manager, sys->renderer);

    SDL_RenderPresent(sys->renderer);
}

Game *Game_Create(int game_width, int game_height) {
    random_init(42);
    Game *g = NULL;

    if ((g = malloc(sizeof(Game))) == NULL) {
        printf("ERROR :: Unable to allocate memory for Game\n");
        return NULL;
    }

    if (Player_Init(&g->player, (Vec2){(float)game_width / 2.f, (float)game_height / 2.f}) != 0) {
        printf("ERROR :: Unable to initialize player\n");
        Game_Destroy(g);
        return NULL;
    }

    if (BulletContainer_Init(&g->bullets) != 0) {
        printf("ERROR :: Unable to initialize bullet container\n");
        Game_Destroy(g);
        return NULL;
    }

    // Init Rail Manager
    if (EnemyRailManager_Init(&g->rail_manager) != 0) {
        printf("ERROR :: Unable to initialize enemy rail manager\n");
        Game_Destroy(g);
        return NULL;
    }

    if (Controller_Init(&g->controller) != 0) {
        printf("ERROR :: Unable to initialize controller\n");
        Game_Destroy(g);
        return NULL;
    }

    return g;
}

void Game_Run(Game *g, System *sys) {
    SDL_Event event;
    int       running = 1;
    Clock     game_clock;
    Clock_Init(&game_clock);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }
        }

        // Input & Update
        Frame frame = Clock_Reset(&game_clock);
        Game_Update(g, sys, frame);

        // Draw
        Game_Draw(g, sys);
    }
}

void Game_Destroy(Game *g) {
    EnemyRailManager_Destroy(&g->rail_manager);

    free(g);

    return;
}
