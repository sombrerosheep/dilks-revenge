#include "game.h"

#include "args.h"
#include "assets.h"
#include "background.h"
#include "camera.h"
#include "clock.h"
#include "debug.h"
#include "entities.h"
#include "events.h"
#include "font.h"
#include "game_input.h"
#include "game_mode.h"
#include "game_state.h"
#include "game_ui.h"
#include "globals.h"
#include "levels.h"
#include "player.h"
#include "random.h"
#include "resources.h"
#include "sprite.h"
#include "system.h"
#include "texture.h"
#include "ui.h"
#include "util.h"
#include "vec.h"

static void Game_SetMode(GameState *state, enum GameMode newMode) {
    if (state->mode == newMode) {
        return;
    }

    if (bit_isset(state->modeGraph[state->mode], newMode)) {
        state->mode = newMode;
        return;
    }

    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "Cannot perform transtion from %s to %s\n",
                GameModeLabels[state->mode],
                GameModeLabels[newMode]);
}

static void Game_InitModeGraph(GameState *state, int sz, enum GameMode init_mode) {
    memset(state->modeGraph, 0, sizeof(*state->modeGraph) * sz);

    bit_set(state->modeGraph[GameModeMenu], GameModePlay);
    bit_set(state->modeGraph[GameModePlay], GameModeMenu);
    bit_set(state->modeGraph[GameModePlay], GameModePause);
    bit_set(state->modeGraph[GameModePlay], GameModeGameOver);
    bit_set(state->modeGraph[GameModePause], GameModeMenu);
    bit_set(state->modeGraph[GameModePause], GameModePlay);
    bit_set(state->modeGraph[GameModeGameOver], GameModeMenu);

    state->mode = init_mode;
}

static void Game_LoadAssets(GameOptions *opts, SysConfig *config) {
    Assets_Init(opts->asset_path);
    if (!Assets_LoadAllTextures(config, Resources_GetRenderer())) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "error loading assets1\n");
    }
}

void GameOptions_FromArgs(GameOptions *opts, Args *args) {
    for (u32 i = 0; i < args->count; i++) {
        if (String_EqualCstr(&args->pairs[i].key, "font_path")) {
            opts->font_path = String_Cstr(&args->pairs[i].value);
        }

        if (String_EqualCstr(&args->pairs[i].key, "asset_path")) {
            opts->asset_path = String_Cstr(&args->pairs[i].value);
        }
    }
}

struct drev_game {
    System   *system;
    GameState state;
};

static void reset_game(Game *game) {
    Player_Init(&game->state.player);
    Background_Init(&game->state.background, Vec2_Up);

    Entities_Init(&game->state.projectiles,
                  &game->state.smallShips,
                  &game->state.player,
                  &game->state.background);
    Levels_Reset(&game->state.levels);

    Camera_SetCenter(Resources_GetMainCamera(), Vec2_Zero);
    Camera_SetFocus(Resources_GetMainCamera(), CameraFocusCenter);
}

static void Game_UpdateModePlay(Game *game, Frame delta) {
    if (Controller_JustPressed(game->state.controller.pause)) {
        pause_callback();
    }

    Camera_Update(&game->state.main_camera, delta.sec);

    // Levels_Update(&game->state.levels, delta.sec);

    Entities_Update(delta.sec);

    Entities_CheckAndHandleCollisions();

    if (!Player_Alive(&game->state.player)) {
        SDL_Event event = {
            .type = GameOverEventId,
        };
        SDL_PushEvent(&event);
    }
}

static void Game_Update(Game *game, Frame delta) {
    Controller_Update(&game->state.controller, game->system);

    switch (game->state.mode) {
        case GameModePlay: {
            Game_UpdateModePlay(game, delta);
            break;
        }
        case GameModeMenu: {
            Camera_Update(&game->state.ui_camera, delta.sec);
            UI_Update(&game->state.main_menu);
            break;
        }
        case GameModePause: {
            Camera_Update(&game->state.ui_camera, delta.sec);
            UI_Update(&game->state.pause_menu);
            break;
        }
        case GameModeGameOver: {
            Camera_Update(&game->state.ui_camera, delta.sec);
            Game_UpdateModePlay(game, delta);
            UI_Update(&game->state.game_over_menu);
            break;
        }
        default: {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                        "Update::Non implemented mode: %s\n",
                        GameModeLabels[game->state.mode]);
            break;
        }
    }
}

static void Game_Draw(Game *game) {
    SDL_SetRenderDrawColor(game->system->renderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderClear(game->system->renderer);

    switch (game->state.mode) {
        case GameModePlay: {
            Entities_Draw();

            Camera_Draw(&game->state.main_camera, game->system->renderer);
            Levels_Draw(&game->state.levels);
            break;
        }
        case GameModeMenu: {
            Camera_Draw(&game->state.ui_camera, game->system->renderer);
            UI_Draw(&game->state.main_menu);
            break;
        }
        case GameModePause: {
            Entities_Draw();
            Camera_Draw(&game->state.main_camera, game->system->renderer);
            Levels_Draw(&game->state.levels);

            SDL_Color overlay = ColorBlack;
            overlay.a >>= 1;
            SDL_FRect rect = (SDL_FRect){
                .x = 0.f,
                .y = 0.f,
                .w = game->state.ui_camera.half_size.x * 2.f,
                .h = game->state.ui_camera.half_size.y * 2.f,
            };
            Camera_DrawFillRect(&game->state.ui_camera, rect, overlay);

            Camera_Draw(&game->state.ui_camera, game->system->renderer);
            UI_Draw(&game->state.pause_menu);
            break;
        }
        case GameModeGameOver: {
            Entities_Draw();
            Camera_Draw(&game->state.main_camera, game->system->renderer);
            Levels_Draw(&game->state.levels);

            SDL_Color overlay = ColorBlack;
            overlay.a >>= 1;
            SDL_FRect rect = (SDL_FRect){
                .x = 0.f,
                .y = 0.f,
                .w = game->state.ui_camera.half_size.x * 2.f,
                .h = game->state.ui_camera.half_size.y * 2.f,
            };
            Camera_DrawFillRect(&game->state.ui_camera, rect, overlay);

            Camera_Draw(&game->state.ui_camera, game->system->renderer);
            UI_Draw(&game->state.game_over_menu);
            break;
        }
        default:
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                        "Draw::Non implemented mode: %s\n",
                        GameModeLabels[game->state.mode]);
            break;
    }

    Debug_Draw();

    SDL_RenderPresent(game->system->renderer);
}

static void Game_InitState(Game *game, System *sys, GameOptions *opts) {
    // Resources
    f32 ratio      = (f32)sys->config.window_width / sys->config.window_height;
    f32 units_high = (f32)sys->config.window_height / sys->config.ppu;
    Camera_Init(&game->state.main_camera, units_high, ratio);
    Camera_MoveCenter(&game->state.main_camera, Vec2_Zero);

    Font_Load(game->system->renderer,
              &game->state.title_font,
              opts->font_path,
              32 * sys->config.ppu);
    Font_Load(game->system->renderer,
              &game->state.menu_font,
              opts->font_path,
              16 * sys->config.ppu);
    Font_Load(game->system->renderer,
              &game->state.debug_font,
              opts->font_path,
              3.75 * sys->config.ppu);

    Resources_Init(&game->state.main_camera,
                   &game->state.ui_camera,
                   &game->state.controller,
                   game->system->renderer,
                   &sys->config,
                   &game->state.title_font,
                   &game->state.menu_font,
                   &game->state.debug_font);

    // Entities
    reset_game(game);

    UI_Init(&game->state.main_menu,
            "Dilks Revenge",
            MainMenuItemCount,
            (Button *)main_menu_buttons);
    UI_Init(&game->state.pause_menu, "paused", PauseMenuItemCount, (Button *)pause_menu_buttons);
    UI_Init(&game->state.game_over_menu,
            "Game Over",
            GameOverItemCount,
            (Button *)game_over_buttons);

    Game_InitModeGraph(&game->state, GameModeCount, GameModeMenu);

    Camera_Init(&game->state.ui_camera, units_high, ratio);
    Camera_MoveCenter(&game->state.ui_camera, Vec2_Zero);

    Levels_Init(&game->state.levels);
}

Game *Game_Create(System *sys, GameOptions *opts) {
    random_init(42);
    Game *g = NULL;

    RegisterCustomEvents();

    if ((g = malloc(sizeof(Game))) == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ERROR :: Unable to allocate memory for Game\n");
        return NULL;
    }

    if (Controller_Init(&g->state.controller) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ERROR :: Unable to initialize controller\n");
        Game_Destroy(g);
        return NULL;
    }

    g->system = sys;

    Game_InitState(g, sys, opts);
    Game_LoadAssets(opts, &sys->config);

    return g;
}

void Game_Run(Game *g) {
    SDL_Event event;
    i8        running = 1;
    Clock     game_clock;
    Clock_Init(&game_clock);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            if (event.type == PlayEventId) {
                // g->state.mode = GameModePlay;
                Game_SetMode(&g->state, GameModePlay);
            }

            if (event.type == PauseEventId) {
                // g->state.mode = GameModePause;
                Game_SetMode(&g->state, GameModePause);
            }

            if (event.type == UnPauseEventId) {
                // g->state.mode = GameModePlay;
                Game_SetMode(&g->state, GameModePlay);
            }

            if (event.type == GameOverEventId) {
                // g->state.mode = GameModeGameOver;
                Game_SetMode(&g->state, GameModeGameOver);
            }

            if (event.type == QuitToMenuEventId) {
                reset_game(g);
                // g->state.mode = GameModeMenu;
                Game_SetMode(&g->state, GameModeMenu);
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
