#include "game.h"

#include "clock.h"
#include "game_input.h"
#include "player.h"
#include "random.h"

#include <stdio.h>

#define UNUSED(v) (void)(v)

struct drev_game {
    GameInput controller;
    Player    player;
};

static void Game_Update(Game *game, System *sys, Frame delta) {
    Controller_Update(&game->controller, sys);
    Player_Update(&game->player, game->controller, delta.sec);
}

static void Game_Draw(Game *game, System *sys) {
    SDL_SetRenderDrawColor(sys->renderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderClear(sys->renderer);

    Player_Draw(&game->player, sys->renderer);

    SDL_RenderPresent(sys->renderer);
}

Game *Game_Create(int game_width, int game_height) {
    UNUSED(game_width);
    UNUSED(game_height);
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

    Player_Init(&g->player);

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
    free(g);

    return;
}
