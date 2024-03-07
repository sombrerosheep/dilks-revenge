#include "player.h"

#include "globals.h"
#include "vec.h"

#define PlayerMeterPerSecond 150.f

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
    p->velocity.x = 0.f;
    p->velocity.y = 0.f;

    if (controller.down) {
        p->velocity.y = 1.f;
    }

    if (controller.up) {
        p->velocity.y = -1.f;
    }

    if (controller.left) {
        p->velocity.x = -1.f;
    }

    if (controller.right) {
        p->velocity.x = 1.f;
    }

    p->velocity = Vec2_Normalize(p->velocity);

    float speed = PlayerMeterPerSecond * PIXELS_PER_METER;
    p->position.x += p->velocity.x * speed * delta;
    p->position.y += p->velocity.y * speed * delta;
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
