#include "player.h"

#include "camera.h"
#include "globals.h"
#include "util.h"
#include "vec.h"

#define PlayerMeterPerSecond      125.f
#define PlayerDecayMeterPerSecond ((PlayerMeterPerSecond) / .5f)

int Player_Init(Player *p) {
    p->position = Vec2_Newf(0.f);

    p->velocity = Vec2_Zero;

    p->size.x = 10.f;
    p->size.y = 10.f;

    p->aim = Vec2_Zero;

    return 0;
}

void Player_Update(Player *p, Camera camera, GameInput controller, float delta) {
    float speed = PlayerMeterPerSecond * PIXELS_PER_METER;
    float decay = PlayerDecayMeterPerSecond * PIXELS_PER_METER;

    p->velocity.x = ease(p->velocity.x, 0.f, decay * delta);
    p->velocity.y = ease(p->velocity.y, 0.f, decay * delta);

    if (controller.down) {
        p->velocity.y += speed;
    }

    if (controller.up) {
        p->velocity.y -= speed;
    }

    if (controller.left) {
        p->velocity.x -= speed;
    }

    if (controller.right) {
        p->velocity.x += speed;
    }

    p->velocity.x = clamp(p->velocity.x, speed, -speed);
    p->velocity.y = clamp(p->velocity.y, speed, -speed);

    float v_mag = Vec2_Magnitude(p->velocity);
    p->velocity = Vec2_Normalize(p->velocity);
    p->velocity.x *= v_mag;
    p->velocity.y *= v_mag;

    p->position.x += p->velocity.x * delta;
    p->position.y += p->velocity.y * delta;

    Vec2 mouse_world = Camera_ScreenToWorldF(&camera, controller.mouse_x, controller.mouse_y);

    p->aim = (Vec2){
        mouse_world.x - p->position.x,
        mouse_world.y - p->position.y,
    };
    p->aim = Vec2_Normalize(p->aim);

    // Constrain player
    // todo: does px/m have any influence here? When checking right/bottom
    //       bounds do I need the pixel values?
    //       If the origin is changed, will I need the pixel values?
    SDL_FRect cam_bounds    = Camera_GetBounds(&camera);
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

void Player_Draw(Player *p, Camera camera, SDL_Renderer *renderer) {
#define AIM_RADIUS 75.f
    SDL_SetRenderDrawColor(renderer, 0xAA, 0x11, 0x11, 0xFF);
    float w_pixels      = p->size.x * PIXELS_PER_METER;
    float half_w_pixels = w_pixels / 2.f;
    float h_pixels      = p->size.y * PIXELS_PER_METER;
    float half_h_pixels = h_pixels / 2.f;

    Vec2 aim_point = (Vec2){
        .x = (p->aim.x * AIM_RADIUS) + p->position.x,
        .y = (p->aim.y * AIM_RADIUS) + p->position.y,
    };

    Vec2 screen     = Camera_WorldToScreen(&camera, p->position);
    Vec2 screen_aim = Camera_WorldToScreen(&camera, aim_point);

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
