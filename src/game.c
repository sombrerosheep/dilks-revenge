#include "game.h"

#include "camera.h"
#include "clock.h"
#include "entities.h"
#include "game_input.h"
#include "game_state.h"
#include "globals.h"
#include "player.h"
#include "random.h"
#include "resources.h"

#include <stdio.h>

ResourceManager GameResources;

struct drev_game {
    System   *system;
    GameState state;
};

static void Game_Update(Game *game, Frame delta) {
    Controller_Update(&game->state.controller, game->system);
    Player_Update(&game->state.player, delta.sec);
    Camera_Update(&game->state.main_camera, delta.sec);

    EntityManager_Update(delta.sec);
}

static void Game_Draw(Game *game) {
    SDL_SetRenderDrawColor(game->system->renderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderClear(game->system->renderer);

    Player_Draw(&game->state.player, game->system->renderer);

    Camera_Draw(&game->state.main_camera, game->system->renderer);

    EntityManager_Draw(game->system->renderer);

    SDL_RenderPresent(game->system->renderer);
}

Game *Game_Create(System *sys, int game_width, int game_height) {
    random_init(42);
    Game *g = NULL;

    if ((g = malloc(sizeof(Game))) == NULL) {
        printf("ERROR :: Unable to allocate memory for Game\n");
        return NULL;
    }

    if (Controller_Init(&g->state.controller) != 0) {
        printf("ERROR :: Unable to initialize controller\n");
        Game_Destroy(g);
        return NULL;
    }

    g->system = sys;

    Player_Init(&g->state.player);

    Camera_Init(&g->state.main_camera, (Vec2){game_width, game_height});
    Camera_SetCenter(&g->state.main_camera, Vec2_Zero);

    ResourceManager_Init(&GameResources, &g->state.main_camera, &g->state.controller);

    EntityManager_Init(&g->state.projectiles);

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

            if (event.type == SDL_KEYDOWN) {
                if ( //
                    event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_UP) ||
                    event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_DOWN) ||
                    event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_LEFT) ||
                    event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_RIGHT) //
                ) {
                    CameraFocus new_focus = CameraFocusCenter;
                    if (event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_UP)) {
                        new_focus = CameraFocusTop;
                    } else if (event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_DOWN)) {
                        new_focus = CameraFocusBottom;
                    } else if (event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_LEFT)) {
                        new_focus = CameraFocusLeft;
                    } else if (event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_RIGHT)) {
                        new_focus = CameraFocusRight;
                    }

                    if (g->state.main_camera.focus == new_focus) {
                        Camera_SetFocus(&g->state.main_camera, CameraFocusCenter);
                    } else {
                        Camera_SetFocus(&g->state.main_camera, new_focus);
                    }
                }
            }
        }

        // Input & Update
        Frame frame = Clock_Reset(&game_clock);
        Game_Update(g, frame);

        // Draw
        Game_Draw(g);
    }
}

void Game_Destroy(Game *g) {
    free(g);

    return;
}
