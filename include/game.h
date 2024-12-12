#ifndef DREV_GAME_H
#define DREV_GAME_H

#include "args.h"
#include "system.h"

typedef struct drev_game_options GameOptions;

struct drev_game_options {
    char *font_path;
    char *asset_path;
};

void GameOptions_FromArgs(GameOptions *opts, Args *args);

typedef struct drev_game Game;

Game *Game_Create(System *system, GameOptions *opts);
void  Game_Run(Game *g);

void Game_Destroy(Game *g);

#endif // DREV_GAME_H
