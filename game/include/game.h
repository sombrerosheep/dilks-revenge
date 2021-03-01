#ifndef DREV_GAME_H
#define DREV_GAME_H

#include <player.h>
#include <controller.h>

#include <SDL.h>

typedef struct _game Game;

struct _game {
  SDL_Window *window;
  SDL_Renderer *renderer;
  Player *player;
  Controller controller;
};

Game *Game_Create();
void Game_Run(Game *g);

void Game_Destroy(Game *g);

#endif // DREV_GAME_H
