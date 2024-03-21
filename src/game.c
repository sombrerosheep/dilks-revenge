#include "game.h"

#include "camera.h"
#include "clock.h"
#include "game_input.h"
#include "player.h"
#include "random.h"

#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <stdio.h>

struct drev_game {
    System   *system;
    Camera    camera;
    GameInput controller;
    Player    player;
};

static void Game_Update(Game *game, Frame delta) {
    Controller_Update(&game->controller, game->system);
    Player_Update(&game->player, game->camera, game->controller, delta.sec);
    Camera_Update(&game->camera, delta.sec);
}

static void Game_Draw(Game *game) {
    SDL_SetRenderDrawColor(game->system->renderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderClear(game->system->renderer);

    Player_Draw(&game->player, game->camera, game->system->renderer);

    SDL_RenderPresent(game->system->renderer);
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
    Camera_Init(&g->camera, (Vec2){game_width, game_height});
    Camera_SetCenter(&g->camera, Vec2_Zero);

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
                    // todo: when camera is at a non-center focus and player is at the extent,
                    //       when camera is moved back center, player is not drawn on the new
                    //       extent until input has changed
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

                    if (g->camera.focus == new_focus) {
                        Camera_SetFocus(&g->camera, CameraFocusCenter);
                    } else {
                        Camera_SetFocus(&g->camera, new_focus);
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
