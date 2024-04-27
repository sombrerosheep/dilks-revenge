#include "game.h"

#include "camera.h"
#include "clock.h"
#include "debug.h"
#include "entities.h"
#include "font.h"
#include "game_input.h"
#include "game_state.h"
#include "globals.h"
#include "player.h"
#include "random.h"
#include "resources.h"
#include "system.h"
#include "vec.h"
#include "wave.h"

#include <stdio.h>

const char *font_path = "/home/swansong/.local/share/fonts/ProggyVector Regular.ttf";

// const char *font_path = "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf";

struct drev_game {
    System   *system;
    GameState state;
};

static void Game_Update(Game *game, Frame delta) {
    Controller_Update(&game->state.controller, game->system);
    Camera_Update(&game->state.main_camera, delta.sec);

    Wave_Update(&game->state.current_wave);

    Entities_Update(delta.sec);
}

static void Game_Draw(Game *game) {
    SDL_SetRenderDrawColor(game->system->renderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderClear(game->system->renderer);

    Entities_Draw();

    Camera_Draw(&game->state.main_camera, game->system->renderer);

    Debug_Draw();

    SDL_RenderPresent(game->system->renderer);
}

static void Game_InitState(Game *game, System *sys) {
    // Resources
    float ratio      = (float)sys->config.window_width / sys->config.window_height;
    float units_high = (float)sys->config.window_height / sys->config.ppu;
    Camera_Init(&game->state.main_camera, units_high, ratio);
    Camera_SetCenter(&game->state.main_camera, Vec2_Zero);

    Font_Load(game->system->renderer, &game->state.debug_font, font_path, 3.75 * sys->config.ppu);

    Resources_Init(&game->state.main_camera,
                   &game->state.controller,
                   game->system->renderer,
                   &sys->config,
                   &game->state.debug_font);

    // Entities
    Player_Init(&game->state.player);
    Entities_Init(&game->state.projectiles, &game->state.smallShips, &game->state.player);

    game->state.current_wave.state = WaveStateIdle;
}

Game *Game_Create(System *sys) {
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

    Game_InitState(g, sys);

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

            if (event.type == SDL_KEYUP) {
                if ( //
                    event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_UP) ||
                    event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_DOWN) ||
                    event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_LEFT) ||
                    event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_RIGHT) ||
                    event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_c) //
                ) {
                    if (event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_UP)) {
                        Wave_Clean(&g->state.current_wave);
                        g->state.current_wave = Wave_New(CameraFocusTop);
                    } else if (event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_DOWN)) {
                        Wave_Clean(&g->state.current_wave);
                        g->state.current_wave = Wave_New(CameraFocusBottom);
                    } else if (event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_LEFT)) {
                        Wave_Clean(&g->state.current_wave);
                        g->state.current_wave = Wave_New(CameraFocusLeft);
                    } else if (event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_RIGHT)) {
                        Wave_Clean(&g->state.current_wave);
                        g->state.current_wave = Wave_New(CameraFocusRight);
                    } else if (event.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_c)) {
                        Wave_Clean(&g->state.current_wave);
                        Camera_SetFocus(&g->state.main_camera, CameraFocusCenter);
                        Entities_MovePlayerTo(Vec2_Zero);
                    }

                    Wave_Start(&g->state.current_wave);
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
