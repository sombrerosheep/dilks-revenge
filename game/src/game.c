#include <game.h>

#include <stdio.h>

static const char *title = "dilks revenge";

static void Game_Draw(Game *game) {
  SDL_SetRenderDrawColor(game->renderer, 0x0, 0x0, 0x0, 0xFF);
  SDL_RenderClear(game->renderer);

  Player_Draw(game->player, game->renderer);

  SDL_RenderPresent(game->renderer);
}

Game *Game_Create() {
  Game *g = NULL;

  if ((g = malloc(sizeof(Game))) == NULL) {
    printf("ERROR :: Unable to allocate memory for Game\n");
    return NULL;
  }

  if ((g->window = SDL_CreateWindow(title, 100, 50, 800, 600, SDL_WINDOW_OPENGL)) == NULL) {
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

  return g;
}

void Game_Run(Game *g) {
  SDL_Event event;
  int running = 1;

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
