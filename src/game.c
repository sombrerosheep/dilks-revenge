#include "game.h"

#include "bullet.h"
#include "clock.h"
#include "enemy.h"
#include "enemy_rail_manager.h"
#include "game_input.h"
#include "player.h"
#include "random.h"

#include <stdio.h>

struct drev_game {
    Player            player;
    BulletContainer   bullets;
    EnemyRailManager *rail_manager;
    GameInput         controller;
};

static void Game_Update(Game *game, System *sys, Frame delta) {
    Controller_Update(&game->controller, sys);

    Player_Update(&game->player, &game->controller, &game->bullets, delta.sec);

    EnemyRailManager_SetFocus(game->rail_manager, game->player.position);
    EnemyRailManager_Update(game->rail_manager, &game->bullets, delta.sec, sys->renderer);

    BulletContainer_Update(&game->bullets, delta.sec);
}

static void Game_Draw(Game *game, System *sys) {
    SDL_SetRenderDrawColor(sys->renderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderClear(sys->renderer);

    BulletContainer_Draw(&game->bullets, sys->renderer);
    Player_Draw(&game->player, sys->renderer);
    EnemyRailManager_Draw(game->rail_manager, sys->renderer);

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
    if ((g->rail_manager = EnemyRailManager_Create(4)) == NULL) {
        printf("ERROR :: Unable to initialize enemy rail manager\n");
        Game_Destroy(g);
        return NULL;
    }

    { // Init rails
        EnemyRailManager_AddRail(
            g->rail_manager,
            "top",
            (Vec2){-100.f, 100.f},
            (Vec2){900.f, 100.f}
        );

        EnemyRailManager_AddRail(
            g->rail_manager,
            "right",
            (Vec2){700.f, -100.f},
            (Vec2){700.f, 700.f}
        );

        EnemyRailManager_AddRail(
            g->rail_manager,
            "bottom",
            (Vec2){-100.f, 500.f},
            (Vec2){900.f, 500.f}
        );

        EnemyRailManager_AddRail(
            g->rail_manager,
            "left",
            (Vec2){100.f, -100.f},
            (Vec2){100.f, 700.f}
        );
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
    int       running    = 1;
    Clock    *game_clock = Clock_Create();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }
        }

        // Input & Update
        Frame frame = Clock_Reset(game_clock);
        Game_Update(g, sys, frame);

        // Draw
        Game_Draw(g, sys);
    }
}

void Game_Destroy(Game *g) {
    if (g->rail_manager != NULL) {
        EnemyRailManager_Destroy(g->rail_manager);
    }

    return;
}
