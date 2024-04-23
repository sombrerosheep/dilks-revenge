#include "player.h"

#include "camera.h"
#include "entities.h"
#include "game_input.h"
#include "globals.h"
#include "projectile.h"
#include "resources.h"
#include "util.h"
#include "vec.h"

#define PLAYER_INIT_HEALTH        100.f
#define AIM_RADIUS                15.f
#define PLAYER_PROJECTILE_SPEED   900.f
#define PLAYER_FIRE_RATE          0.5f
#define PlayerMeterPerSecond      125.f
#define PlayerDecayMeterPerSecond ((PlayerMeterPerSecond) / .5f)

int Player_Init(Player *p) {
    p->health          = PLAYER_INIT_HEALTH;
    p->position        = Vec2_Newf(0.f);
    p->target_position = Vec2_Newf(0.f);
    p->velocity        = Vec2_Zero;

    p->size.x = 10.f;
    p->size.y = 10.f;

    p->aim = Vec2_Zero;

    p->being_moved = 0;

    p->fire_cooldown = PLAYER_FIRE_RATE;

    return 0;
}

void Player_MoveTo(Player *p, Vec2 target) {
    p->target_position = target;
    p->being_moved     = 1;
}

static void Player_Shoot(Vec2 pos, Vec2 vel) {
    Projectile p;
    Projectile_Init(&p, ProjectileType_Player, pos, vel, PLAYER_PROJECTILE_SPEED);
    EntityManager_AddProjectile(p);
}

void Player_Update(Player *p, float delta) {
    Camera          *camera     = ResourceManager_GetMainCamera();
    const GameInput *controller = ResourceManager_GetController();
    float            speed      = PlayerMeterPerSecond;
    float            decay      = PlayerDecayMeterPerSecond;

    if (p->being_moved == 1) {
        const float ease_speed = PlayerMeterPerSecond;
        p->position.x          = ease(p->position.x, p->target_position.x, delta * ease_speed);
        p->position.y          = ease(p->position.y, p->target_position.y, delta * ease_speed);

        const float close_enough = 0.01f;
        if (SDL_fabsf(p->position.x - p->target_position.x) < close_enough &&
            SDL_fabsf(p->position.y - p->target_position.y) < close_enough) {
            p->velocity    = Vec2_Zero;
            p->being_moved = 0;
        }
    } else {
        // get velocity
        p->velocity.x = ease(p->velocity.x, 0.f, decay * delta);
        p->velocity.y = ease(p->velocity.y, 0.f, decay * delta);

        if (controller->down) {
            p->velocity.y += speed;
        }

        if (controller->up) {
            p->velocity.y -= speed;
        }

        if (controller->left) {
            p->velocity.x -= speed;
        }

        if (controller->right) {
            p->velocity.x += speed;
        }

        p->velocity.x = clamp(p->velocity.x, speed, -speed);
        p->velocity.y = clamp(p->velocity.y, speed, -speed);

        // move player
        float v_mag = Vec2_Magnitude(p->velocity);
        p->velocity = Vec2_Normalize(p->velocity);
        p->velocity.x *= v_mag;
        p->velocity.y *= v_mag;

        p->position.x += p->velocity.x * delta;
        p->position.y += p->velocity.y * delta;

        Vec2 mouse_world = Camera_ScreenToWorldF(camera, controller->mouse_x, controller->mouse_y);

        p->aim = (Vec2){
            mouse_world.x - p->position.x,
            mouse_world.y - p->position.y,
        };
        p->aim = Vec2_Normalize(p->aim);

        // Shooting
        p->fire_cooldown -= delta;

        if (p->fire_cooldown < 0) {
            if (controller->mouse_left || controller->space) {
                Vec2 aim_point = (Vec2){
                    .x = (p->aim.x * AIM_RADIUS) + p->position.x,
                    .y = (p->aim.y * AIM_RADIUS) + p->position.y,
                };
                Player_Shoot(aim_point, p->aim);
                p->fire_cooldown = PLAYER_FIRE_RATE;
            }
        }
    }

    SDL_FRect cam_bounds = Camera_GetBounds(camera);

    if (p->position.x < cam_bounds.x) {
        p->position.x = cam_bounds.x;
    } else if (p->position.x > cam_bounds.x + cam_bounds.w) {
        p->position.x = cam_bounds.x + cam_bounds.w;
    }
    if (p->position.y < cam_bounds.y) {
        p->position.y = cam_bounds.y;
    } else if (p->position.y > cam_bounds.y + cam_bounds.h) {
        p->position.y = cam_bounds.y + cam_bounds.h;
    }
}

void Player_Draw(const Player *p) {
    Camera   *camera       = ResourceManager_GetMainCamera();
    SDL_Color player_color = {.r = 0xAA, .g = 0x11, .b = 0x11, .a = 0xFF};

    Vec2 aim_point = (Vec2){
        .x = (p->aim.x * AIM_RADIUS) + p->position.x,
        .y = (p->aim.y * AIM_RADIUS) + p->position.y,
    };

    // draw the player
    SDL_FRect rect = {
        .x = p->position.x,
        .y = p->position.y,
        .w = p->size.x,
        .h = p->size.y,
    };

    Camera_DrawFillRect(camera, rect, player_color);

    // draw the crosshair box
    rect.x = aim_point.x;
    rect.y = aim_point.y;
    rect.w = 1.f;
    rect.h = 1.f;
    Camera_DrawFillRect(camera, rect, player_color);
}
