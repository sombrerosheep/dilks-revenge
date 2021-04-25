#ifndef DREV_PLAYER_H
#define DREV_PLAYER_H

#include <vec.h>
#include <controller.h>
#include <shield.h>

#include <SDL.h>

typedef struct _player Player;

struct _player {
  Vec2 position;
  Vec2 velocity;
  Shield shield;
};

int Player_Init(Player *player, Vec2 starting_pos);
void Player_Update(Player *player, const Controller *controller, float delta);
void Player_Draw(const Player *player, SDL_Renderer *renderer);

void Player_Destroy(Player *player);

#endif // DREV_PLAYER_H