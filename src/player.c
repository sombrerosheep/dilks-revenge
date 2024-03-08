#include "player.h"

#include "globals.h"
#include "vec.h"

#define PlayerMeterPerSecond      150.f
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
    p->position.x = 200.f;
    p->position.y = 200.f;

    p->velocity.x = 0.f;
    p->velocity.y = 0.f;

    p->size.x = 10.f;
    p->size.y = 10.f;

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
}

void Player_Draw(Player *p, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xAA, 0x11, 0x11, 0xFF);
    SDL_FRect rect = {//
                      .x = p->position.x,
                      .y = p->position.y,
                      .w = p->size.x * PIXELS_PER_METER,
                      .h = p->size.y * PIXELS_PER_METER};

    SDL_RenderFillRectF(renderer, &rect);
}
