#ifndef DREV_COLLISIONS_H
#define DREV_COLLISIONS_H

#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "shield.h"
#include <SDL.h>

typedef enum {
    Collider_Bullet = 0,
    Collider_Player,
    Collider_Enemy,

    Collider_Count
} Collider;

int is_colliding(const SDL_FRect *a, const SDL_FRect *b);
int is_collidingc(const SDL_FRect *a, Vec2 center, float radius);

void resolve_collision_bullet_bullet(Bullet *a, Bullet *b);
void resolve_collision_player_bullet(Player *p, Bullet *b);
void resolve_collision_enemy_bullet(Enemy *e, Bullet *b);
void resolve_collision_shield_bullet(Shield *s, Bullet *b);
void resolve_collision_player_enemy(Player *p, Enemy *e);

#endif // DREV_COLLISIONS_H
