#ifndef DREV_GAME_H
#define DREV_GAME_H

#include <player.h>
#include <controller.h>
#include <enemy_rail.h>

#include <SDL.h>

#define RAIL_COUNT 4

typedef struct _game Game;

struct _game {
  SDL_Window *window;
  SDL_Renderer *renderer;
  Player *player;
  EnemyRail *rails[RAIL_COUNT];
  Controller controller;
};

Game *Game_Create();
void Game_Run(Game *g);

void Game_Destroy(Game *g);

#endif // DREV_GAME_H
