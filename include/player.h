#ifndef DREV_PLAYER_H
#define DREV_PLAYER_H

#include "camera.h"
#include "game_input.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_player Player;

struct drev_player {
    Vec2 position;
    Vec2 velocity;
    Vec2 size;
    Vec2 aim;
};

int  Player_Init(Player *p);
void Player_Update(Player *p, Camera *cam, GameInput controller, float delta);
void Player_Draw(Player *p, Camera camera);

#endif // DREV_PLAYER_H
