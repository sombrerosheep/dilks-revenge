#ifndef DREV_PLAYER_H
#define DREV_PLAYER_H

#include <bullet.h>
#include <game_input.h>
#include <shield.h>
#include <vec.h>

#include <SDL.h>

#define PLAYER_MAX_BULLETS 128

typedef struct drev_player        Player;
typedef struct drev_player_bullet PlayerBullet;

struct drev_player_bullet {
    int    in_use;
    Bullet bullet;
};

struct drev_player {
    Vec2         position;
    Vec2         velocity;
    Vec2         aim;
    Shield       shield;
    unsigned int last_fire;
    PlayerBullet bullets[PLAYER_MAX_BULLETS];
};

int  Player_Init(Player *player, Vec2 starting_pos);
void Player_Update(Player *player, const GameInput *controller, float delta);
void Player_Draw(const Player *player, SDL_Renderer *renderer);

void Player_Destroy(Player *player);

#endif // DREV_PLAYER_H
