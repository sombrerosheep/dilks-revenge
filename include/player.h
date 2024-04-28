#ifndef DREV_PLAYER_H
#define DREV_PLAYER_H

#include "camera.h"
#include "game_input.h"
#include "projectile.h"
#include "types.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_player       Player;
typedef struct player_projectile PlayerProjectile;

struct drev_player {
    Vec2 position;
    Vec2 target_position;
    Vec2 velocity;
    Vec2 size;
    Vec2 aim;
    i8   being_moved;
    f32  fire_cooldown;
    u64  health;
};

i8        Player_Init(Player *p);
void      Player_MoveTo(Player *p, Vec2 target);
void      Player_Damage(Player *p, u64 amount);
void      Player_Kill(Player *p);
void      Player_Update(Player *p, f32 delta);
SDL_FRect Player_GetBounds(const Player *p);
void      Player_Draw(const Player *p);

#endif // DREV_PLAYER_H
