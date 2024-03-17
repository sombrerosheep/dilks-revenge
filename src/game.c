#include "game.h"

#include "camera.h"
#include "clock.h"
#include "game_input.h"
#include "player.h"
#include "random.h"

#include <stdio.h>

#define UNUSED(v) (void)(v)

struct drev_game {
    System   *system;
    Camera    camera;
    GameInput controller;
    Player    player;
};

static void Game_Update(Game *game, System *sys, Frame delta) {
    Controller_Update(&game->controller, sys);
    Player_Update(&game->player, game->controller, delta.sec);

    Camera_SetCenter(&game->camera, game->player.position);
}

static void Game_Draw(Game *game, System *sys) {
    SDL_SetRenderDrawColor(sys->renderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderClear(sys->renderer);

    Player_Draw(&game->player, game->camera);

    SDL_RenderPresent(sys->renderer);
}

Game *Game_Create(System *sys, int game_width, int game_height) {
    random_init(42);
    Game *g = NULL;

    if ((g = malloc(sizeof(Game))) == NULL) {
        printf("ERROR :: Unable to allocate memory for Game\n");
        return NULL;
    }

    if (Controller_Init(&g->controller) != 0) {
        printf("ERROR :: Unable to initialize controller\n");
        Game_Destroy(g);
        return NULL;
    }

    g->system = sys;

    Player_Init(&g->player);
    Camera_Init(&g->camera, sys->renderer, (Vec2){game_width, game_height});

    return g;
}

void Game_Run(Game *g) {
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
        Game_Update(g, g->system, frame);

        // Draw
        Game_Draw(g, g->system);
    }
}

void Game_Destroy(Game *g) {
    free(g);

    return;
}
