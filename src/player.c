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
#define AIM_RADIUS                75.f
#define PLAYER_PROJECTILE_SPEED   450.f
#define PlayerMeterPerSecond      125.f
#define PlayerDecayMeterPerSecond ((PlayerMeterPerSecond) / .5f)

int Player_Init(Player *p) {
    p->health   = PLAYER_INIT_HEALTH;
    p->position = Vec2_Newf(0.f);
    p->velocity = Vec2_Zero;

    p->size.x = 10.f;
    p->size.y = 10.f;

    p->aim = Vec2_Zero;

    p->fire_cooldown = 0.f;

    return 0;
}

void Player_Shoot(Vec2 pos, Vec2 vel) {
    Projectile p;
    Projectile_Init(&p, ProjectileType_Player, pos, vel, PLAYER_PROJECTILE_SPEED);
    EntityManager_AddProjectile(p);
}

void Player_Update(Player *p, float delta) {
    Camera          *camera     = ResourceManager_GetMainCamera();
    const GameInput *controller = ResourceManager_GetController();
    float            speed      = PlayerMeterPerSecond * PIXELS_PER_METER;
    float            decay      = PlayerDecayMeterPerSecond * PIXELS_PER_METER;

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
    p->fire_cooldown += delta;

    if (p->fire_cooldown > 1.5f) {
        if (controller->mouse_left || controller->space) {
            Vec2 aim_point = (Vec2){
                .x = (p->aim.x * AIM_RADIUS) + p->position.x,
                .y = (p->aim.y * AIM_RADIUS) + p->position.y,
            };
            Player_Shoot(aim_point, p->aim);
            p->fire_cooldown = 0.f;
        }
    }

    // Constrain player
    SDL_FRect cam_bounds    = Camera_GetBounds(camera);
    float     w_pixels      = p->size.x * PIXELS_PER_METER;
    float     half_w_pixels = w_pixels / 2.f;
    float     h_pixels      = p->size.y * PIXELS_PER_METER;
    float     half_h_pixels = h_pixels / 2.f;

    if (p->position.x - half_w_pixels < cam_bounds.x) {
        p->position.x = cam_bounds.x + half_w_pixels;
    } else if (p->position.x + half_w_pixels > cam_bounds.x + cam_bounds.w) {
        p->position.x = cam_bounds.x + cam_bounds.w - half_w_pixels;
    }
    if (p->position.y - half_h_pixels < cam_bounds.y) {
        p->position.y = cam_bounds.y + half_w_pixels;
    } else if (p->position.y + half_h_pixels > cam_bounds.y + cam_bounds.h) {
        p->position.y = cam_bounds.y + cam_bounds.h - half_h_pixels;
    }
}

void Player_Draw(const Player *p, SDL_Renderer *renderer) {
    Camera *camera = ResourceManager_GetMainCamera();

    SDL_SetRenderDrawColor(renderer, 0xAA, 0x11, 0x11, 0xFF);
    float w_pixels      = p->size.x * PIXELS_PER_METER;
    float half_w_pixels = w_pixels / 2.f;
    float h_pixels      = p->size.y * PIXELS_PER_METER;
    float half_h_pixels = h_pixels / 2.f;

    Vec2 aim_point = (Vec2){
        .x = (p->aim.x * AIM_RADIUS) + p->position.x,
        .y = (p->aim.y * AIM_RADIUS) + p->position.y,
    };

    Vec2 screen     = Camera_WorldToScreen(camera, p->position);
    Vec2 screen_aim = Camera_WorldToScreen(camera, aim_point);

    SDL_FRect rect = {
        .x = screen.x - half_w_pixels,
        .y = screen.y - half_h_pixels,
        .w = w_pixels,
        .h = h_pixels,
    };

    SDL_RenderFillRectF(renderer, &rect);

    rect.x = screen_aim.x;
    rect.y = screen_aim.y;
    rect.w = 10.f;
    rect.h = 10.f;

    SDL_RenderFillRectF(renderer, &rect);
}
