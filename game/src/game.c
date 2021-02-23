#include <game.h>

#include <stdio.h>

static const char *title = "dilks revenge";

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
    SDL_SetRenderDrawColor(g->renderer, 0x0, 0x0, 0x0, 0xFF);
    SDL_RenderClear(g->renderer);

    SDL_RenderPresent(g->renderer);
  }
}

void Game_Destroy(Game *g) {
  if (g->renderer != NULL) {
    SDL_DestroyRenderer(g->renderer);
  }

  if (g->window != NULL) {
    SDL_DestroyWindow(g->window);
  }

  return;
}
