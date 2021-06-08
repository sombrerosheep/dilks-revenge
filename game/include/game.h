#ifndef DREV_GAME_H
#define DREV_GAME_H

#include <player.h>
#include <controller.h>
#include <enemy_rail_manager.h>

#include <SDL.h>

typedef struct drev_game Game;

struct drev_game {
  SDL_Window *window;
  SDL_Renderer *renderer;
  Player *player;
  EnemyRailManager *rail_manager;
  Controller controller;
};

Game *Game_Create();
void Game_Run(Game *g);

void Game_Destroy(Game *g);

#endif // DREV_GAME_H
