#include <game.h>

#include <clock.h>
#include <enemy.h>

#include <stdio.h>

static const char *title = "dilks revenge";

Enemy e;

static void Game_Update(Game *game, Frame delta) {
  Controller_Update(&game->controller);

  Player_Update(game->player, &game->controller, delta.sec);
}

static void Game_Draw(Game *game) {
  SDL_SetRenderDrawColor(game->renderer, 0x0, 0x0, 0x0, 0xFF);
  SDL_RenderClear(game->renderer);

  Player_Draw(game->player, game->renderer);

  Enemey_Draw(&e, game->renderer);

  SDL_RenderPresent(game->renderer);
}

Game *Game_Create() {
  #define WINDOW_HEIGHT 600
  #define WINDOW_WIDTH 800

  Game *g = NULL;

  if ((g = malloc(sizeof(Game))) == NULL) {
    printf("ERROR :: Unable to allocate memory for Game\n");
    return NULL;
  }

  if ((g->window = SDL_CreateWindow(title, 100, 50, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL)) == NULL) {
    printf("ERROR :: Unable to allocate memory for window. SDL Error: %s\n", SDL_GetError());
    Game_Destroy(g);
    return NULL;
  }

  if ((g->renderer = SDL_CreateRenderer(g->window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
    printf("ERROR :: Unable to allocate memory for renderer. SDL Error: %s\n", SDL_GetError());
    Game_Destroy(g);
    return NULL;
  }

  if ((g->player = malloc(sizeof(Player))) == NULL) {
    printf("ERROR :: Unable to allocate memory for player\n");
    Game_Destroy(g);
    return NULL;
  }
  if (Player_Init(g->player, (Vec2){ (float)WINDOW_WIDTH / 2.f, (float)WINDOW_HEIGHT / 2.f }) != 0) {
    printf("ERROR :: Unable to initialize player\n");
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

void Game_Run(Game *g) {
  SDL_Event event;
  int running = 1;
  Clock *game_clock = Clock_Create();

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (
          event.type == SDL_KEYDOWN &&
          event.key.keysym.sym == SDLK_ESCAPE
        ) {
        running = 0;
      }
    }

    // Input & Update
    Frame frame = Clock_Reset(game_clock);
    Game_Update(g, frame);

    Enemy_Init(&e, (Vec2){ 10.f, 10.f}, 100);

    // Draw
    Game_Draw(g);
  }
}

void Game_Destroy(Game *g) {
  if (g->renderer != NULL) {
    SDL_DestroyRenderer(g->renderer);
  }

  if (g->window != NULL) {
    SDL_DestroyWindow(g->window);
  }

  if (g->player != NULL) {
    free(g->player);
  }

  return;
}
