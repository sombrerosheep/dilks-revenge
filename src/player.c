#include "player.h"

#include "assets.h"
#include "camera.h"
#include "entities.h"
#include "events.h"
#include "game_input.h"
#include "globals.h"
#include "projectile.h"
#include "resources.h"
#include "sprite.h"
#include "util.h"
#include "vec.h"

#define PLAYER_INIT_HEALTH        100.f
#define AIM_RADIUS                15.f
#define PLAYER_PROJECTILE_SPEED   400.f
#define PLAYER_FIRE_RATE          0.5f
#define PlayerMeterPerSecond      125.f
#define PlayerDecayMeterPerSecond ((PlayerMeterPerSecond) / .5f)
#define PlayerProjectileDamage    200u
#define PlayerProjectileSize      1.f

i8 Player_Init(Player *p) {
    p->health = PLAYER_INIT_HEALTH;
    Sprite_Init(&p->sprite, Assets_GetTexture(TextureIDPlayer));

    p->target_position = Vec2_Newf(0.f);
    p->velocity        = Vec2_Zero;

    p->size.x = 10.f;
    p->size.y = 10.f;

    p->aim = Vec2_Zero;

    p->being_moved = 0;

    p->fire_cooldown = PLAYER_FIRE_RATE;

    return 0;
}

Vec2 Player_GetPosition(const Player *p) {
    return p->sprite.pos;
}

void Player_SetPosition(Player *p, Vec2 pos) {
    p->sprite.pos = pos;
}

void Player_MoveTo(Player *p, Vec2 target) {
    p->target_position = target;
    p->being_moved     = 1;
}

void Player_Damage(Player *p, u64 amount) {
    if (amount > p->health) {
        p->health = 0;
    } else {
        p->health -= amount;
    }
}

bool Player_Alive(const Player *p) {
    return p->health > 0;
}

void Player_Kill(Player *p) {
    p->health = 0;
}

static void Player_Shoot(Vec2 pos, Vec2 vel) {
    Projectile p;
    Projectile_Init(&p,
                    ProjectileType_Player,
                    pos,
                    vel,
                    PLAYER_PROJECTILE_SPEED,
                    PlayerProjectileDamage,
                    PlayerProjectileSize);
    Entities_AddProjectile(p);
}

void Player_Update(Player *p, f32 delta) {
    Camera          *camera     = Resources_GetMainCamera();
    const GameInput *controller = Resources_GetController();
    f32              speed      = PlayerMeterPerSecond;
    f32              decay      = PlayerDecayMeterPerSecond;

    if (p->health == 0) {
        p->sprite.pos.x += p->velocity.x * (delta * .25f);
        p->sprite.pos.y += p->velocity.y * (delta * .25f);

        return;
    }

    if (p->being_moved == 1) {
        const f32 ease_speed = PlayerMeterPerSecond;
        p->sprite.pos.x      = ease(p->sprite.pos.x, p->target_position.x, delta * ease_speed);
        p->sprite.pos.y      = ease(p->sprite.pos.y, p->target_position.y, delta * ease_speed);

        const f32 close_enough = 0.01f;
        if (SDL_fabsf(p->sprite.pos.x - p->target_position.x) < close_enough &&
            SDL_fabsf(p->sprite.pos.y - p->target_position.y) < close_enough) {
            p->velocity    = Vec2_Zero;
            p->being_moved = 0;
        }
    } else {
        // get velocity
        p->velocity.x = ease(p->velocity.x, 0.f, decay * delta);
        p->velocity.y = ease(p->velocity.y, 0.f, decay * delta);

        if (Controller_Is(controller->down)) {
            p->velocity.y += speed;
        }

        if (Controller_Is(controller->up)) {
            p->velocity.y -= speed;
        }

        if (Controller_Is(controller->left)) {
            p->velocity.x -= speed;
        }

        if (Controller_Is(controller->right)) {
            p->velocity.x += speed;
        }

        p->velocity.x = clamp(p->velocity.x, speed, -speed);
        p->velocity.y = clamp(p->velocity.y, speed, -speed);

        // move player
        f32 v_mag   = Vec2_Magnitude(p->velocity);
        p->velocity = Vec2_Normalize(p->velocity);
        p->velocity.x *= v_mag;
        p->velocity.y *= v_mag;

        p->sprite.pos.x += p->velocity.x * delta;
        p->sprite.pos.y += p->velocity.y * delta;

        p->aim = Camera_ScreenToWorldF(Resources_GetMainCamera(),
                                       controller->mouse_screen_x,
                                       controller->mouse_screen_y);
        p->aim.x -= p->sprite.pos.x;
        p->aim.y -= p->sprite.pos.y;

        p->aim = Vec2_Normalize(p->aim);
        Sprite_SetRotation(&p->sprite, slope_to_deg(p->aim) + 90.f);

        // Shooting
        p->fire_cooldown -= delta;

        if (p->fire_cooldown < 0) {
            if (controller->mouse_left || Controller_Is(controller->fire)) {
                Vec2 aim_point = (Vec2){
                    // .x = (p->aim.x * AIM_RADIUS) + p->sprite.pos.x,
                    // .y = (p->aim.y * AIM_RADIUS) + p->sprite.pos.y,
                    .x = (p->aim.x * (p->size.x / 2.f)) + p->sprite.pos.x,
                    .y = (p->aim.y * (p->size.y / 2.f)) + p->sprite.pos.y,
                };
                Player_Shoot(aim_point, p->aim);
                p->fire_cooldown = PLAYER_FIRE_RATE;
            }
        }
    }

    SDL_FRect cam_bounds = Camera_GetBounds(camera);

    if (p->sprite.pos.x < cam_bounds.x) {
        p->sprite.pos.x = cam_bounds.x;
    } else if (p->sprite.pos.x > cam_bounds.x + cam_bounds.w) {
        p->sprite.pos.x = cam_bounds.x + cam_bounds.w;
    }
    if (p->sprite.pos.y < cam_bounds.y) {
        p->sprite.pos.y = cam_bounds.y;
    } else if (p->sprite.pos.y > cam_bounds.y + cam_bounds.h) {
        p->sprite.pos.y = cam_bounds.y + cam_bounds.h;
    }
}

SDL_FRect Player_GetBounds(const Player *p) {
    SDL_FRect rect = {
        .x = p->sprite.pos.x,
        .y = p->sprite.pos.y,
        .w = p->size.x,
        .h = p->size.y,
    };

    return rect;
}

static void draw_health(const Player *p) {
    f32       inset = 0.5;
    Camera   *cam   = Resources_GetMainCamera();
    SDL_FRect rect  = (SDL_FRect){
         .x = p->sprite.pos.x,
         .y = p->sprite.pos.y + (p->size.y / 2.f) + 2.f,
         .w = p->size.x,
         .h = 2.f,
    };

    Camera_DrawFillRect(cam, rect, ColorGrey);

    rect.w -= (inset * 2.f);

    f32 health_pcnt = (f32)(p->health / PLAYER_INIT_HEALTH);
    f32 new_width   = rect.w * health_pcnt;
    rect.x -= (rect.w - new_width) / 2.f;
    rect.w = new_width;
    rect.h -= (inset * 2.f);

    Camera_DrawFillRect(cam, rect, ColorRed);
}

void Player_Draw(const Player *p) {
    Camera *camera = Resources_GetMainCamera();

    draw_health(p);

    Sprite_Draw(&p->sprite);

#ifdef DREV_DRAW_BB
    Camera_DrawRect(camera, Player_GetBounds(p), ColorRed);
#endif
}
