#ifndef DREV_GAME_H
#define DREV_GAME_H

#include "system.h"

typedef struct drev_game Game;

Game *Game_Create(int game_width, int game_height);
void  Game_Run(Game *g, System *sys);

void Game_Destroy(Game *g);

#endif // DREV_GAME_H
