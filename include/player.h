#ifndef DREV_PLAYER_H
#define DREV_PLAYER_H

#include "game_input.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_player Player;

struct drev_player {
    Vec2 position;
    Vec2 velocity;
    Vec2 size;
};

int  Player_Init(Player *p);
void Player_Update(Player *p, GameInput controller, float delta);
void Player_Draw(Player *p, SDL_Renderer *renderer);

#endif // DREV_PLAYER_H
