#ifndef DREV_PLAYER_H
#define DREV_PLAYER_H

#include "bullet.h"
#include "game_input.h"
#include "shield.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_player Player;

struct drev_player {
    float        health;
    Vec2         position;
    Vec2         velocity;
    Vec2         aim;
    Shield       shield;
    unsigned int last_fire;
};

int  Player_Init(Player *player, Vec2 starting_pos);
void Player_Update(Player *player, const GameInput *controller, BulletContainer *c, float delta);
void Player_Draw(const Player *player, SDL_Renderer *renderer);
SDL_FRect Player_BoundingBox(Player *player);
void      Player_Destroy(Player *player);

#endif // DREV_PLAYER_H
