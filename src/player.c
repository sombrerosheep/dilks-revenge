#include "player.h"

#include "camera.h"
#include "globals.h"
#include "vec.h"

#define PlayerMeterPerSecond      125.f
#define PlayerDecayMeterPerSecond ((PlayerMeterPerSecond) / .5f)

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

static float move_to_target(float value, float target, float rate) {
    if (value > target) {
        return max(value - rate, target);
    } else {
        return min(value + rate, target);
    }
}

static float clamp(float value, float upper, float lower) {
    float result = value;

    if (result > upper) {
        result = upper;
    }

    if (result < lower) {
        result = lower;
    }

    return result;
}

int Player_Init(Player *p) {
    p->position = Vec2_Newf(200.f);

    p->velocity = Vec2_Zero;

    p->size.x = 10.f;
    p->size.y = 10.f;

    p->aim = Vec2_Zero;

    return 0;
}

void Player_Update(Player *p, GameInput controller, float delta) {
    float speed = PlayerMeterPerSecond * PIXELS_PER_METER;
    float decay = PlayerDecayMeterPerSecond * PIXELS_PER_METER;

    p->velocity.x = move_to_target(p->velocity.x, 0.f, decay * delta);
    p->velocity.y = move_to_target(p->velocity.y, 0.f, decay * delta);

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

    p->aim = (Vec2){(float)controller.mouse_x - p->position.x,
                    (float)controller.mouse_y - p->position.y};
    p->aim = Vec2_Normalize(p->aim);
}

void Player_Draw(Player *p, Camera camera) {
#define AIM_RADIUS 75.f
    SDL_SetRenderDrawColor(camera.renderer, 0xAA, 0x11, 0x11, 0xFF);
    float w_pixels = p->size.x * PIXELS_PER_METER;
    float h_pixels = p->size.y * PIXELS_PER_METER;

    Vec2 screen     = Camera_WorldToScreen(&camera, p->position);
    Vec2 screen_aim = Camera_WorldToScreen(&camera, p->aim);

    SDL_FRect rect = {//
                      .x = screen.x,
                      .y = screen.y,
                      .w = w_pixels,
                      .h = h_pixels};

    SDL_RenderFillRectF(camera.renderer, &rect);

    rect.x = screen_aim.x * AIM_RADIUS + rect.x + (w_pixels / 2.f);
    rect.y = screen_aim.y * AIM_RADIUS + rect.y + (h_pixels / 2.f);
    rect.w = 10.f;
    rect.h = 10.f;

    SDL_RenderFillRectF(camera.renderer, &rect);
}
