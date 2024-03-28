#ifndef DREV_PLAYER_H
#define DREV_PLAYER_H

#include "camera.h"
#include "game_input.h"
#include "projectile.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_player Player;

typedef struct player_projectile PlayerProjectile;

struct drev_player {
    Vec2  position;
    Vec2  velocity;
    Vec2  size;
    Vec2  aim;
    float fire_cooldown;
    float health;
};

int  Player_Init(Player *p);
void Player_Update(Player *p, float delta);
void Player_Draw(const Player *p, SDL_Renderer *renderer);

#endif // DREV_PLAYER_H
