#include "collisions.h"
#include "bullet.h"

// todo: inline
static float reduce_health(float start, float by) {
    return SDL_max((start - by), 0.f);
}

int is_colliding(const SDL_FRect *a, const SDL_FRect *b) {
    SDL_FRect collision;
    if (SDL_IntersectFRect(a, b, &collision) == SDL_TRUE) {
        return 1;
    }

    return 0;
}

void resolve_collision_bullet_bullet(Bullet *a, Bullet *b) {
    if (a->health > b->health) {
        a->health = reduce_health(a->health, b->health);
        b->health = 0.f;
    } else {
        b->health -= reduce_health(b->health, a->health);
        a->health = 0.f;
    }

    return;
}

void resolve_collision_player_bullet(Player *p, Bullet *b) {
    if (b->type == BulletType_Player) {
        return;
    }

    p->health = reduce_health(p->health, b->health);
    b->health = 0.f;

    return;
}

void resolve_collision_enemy_bullet(Enemy *e, Bullet *b) {
    if (b->type == BulletType_Enemy) {
        return;
    }

    e->health = reduce_health(e->health, b->health);
    b->health = 0.f;
    return;
}

void resolve_collision_shield_bullet(Shield *s, Bullet *b) {
    if (b->type == BulletType_Player) {
        return;
    }

    s->health = reduce_health(s->health, b->health);
    b->health = 0.f;

    return;
}

void resolve_collision_player_enemy(Player *p, Enemy *e) {
    p->health = 0.f;
    e->health = 0.f;
}
