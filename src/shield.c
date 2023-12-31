#include "shield.h"

#include <SDL_render.h>
#include <stdio.h>

#define SHIELD_RECHARGE_DELAY_SEC 3.f
#define SHIELD_INIT_HEALTH        200.f

void draw_circle(SDL_Renderer *renderer, float n_cx, float n_cy, float radius) {
    float error = (float)-radius;
    float x     = radius - 0.5f;
    float y     = 0.5;
    float cx    = n_cx - 0.5;
    float cy    = n_cy - 0.5;

    while (x >= y) {
        SDL_RenderDrawPointF(renderer, (cx + x), (cy + y));
        SDL_RenderDrawPointF(renderer, (cx + y), (cy + x));

        if (x != 0) {
            SDL_RenderDrawPointF(renderer, (cx - x), (cy + y));
            SDL_RenderDrawPointF(renderer, (cx + y), (cy - x));
        }

        if (y != 0) {
            SDL_RenderDrawPointF(renderer, (cx + x), (cy - y));
            SDL_RenderDrawPointF(renderer, (cx - y), (cy + x));
        }

        if (x != 0 && y != 0) {
            SDL_RenderDrawPointF(renderer, (cx - x), (cy - y));
            SDL_RenderDrawPointF(renderer, (cx - y), (cy - x));
        }

        error += y;
        ++y;
        error += y;

        if (error >= 0) {
            --x;
            error -= x;
            error -= x;
        }
    }
}

void Shield_Init(Shield *s, float x, float y) {
    s->position      = (Vec2){x, y};
    s->radius        = 125.f;
    s->target_inside = 0;
    s->recharge      = 0.f;
    s->health        = SHIELD_INIT_HEALTH;
}

// todo: does shield recharge? when? after Xs?
//       is recharge rate based on difficulty?
//       is shield health based on difficulty?
void Shield_Update(Shield *s, float delta) {
    if (s->health <= 0.f) {
        s->recharge += delta;
    }

    if (s->recharge >= SHIELD_RECHARGE_DELAY_SEC) {
        s->health   = SHIELD_INIT_HEALTH;
        s->recharge = 0.f;
    }

    return;
}

static int Shield_HasPoint(const Shield *s, float x, float y) {
    int has_point = 0;

    Vec2  diff     = (Vec2){s->position.x - x, s->position.y - y};
    float distance = Vec2_Magnitude(diff);

    if (distance < s->radius) {
        has_point = 1;
    }

    return has_point;
}

int Shield_EvalTarget(Shield *s, float x, float y, float w, float h) {
    if (Shield_HasPoint(s, x, y) == 1 && Shield_HasPoint(s, x + w, y) == 1 &&
        Shield_HasPoint(s, x + w, y + h) == 1 && Shield_HasPoint(s, x, y + h) == 1) {
        s->target_inside = 1;
    } else {
        s->target_inside = 0;
    }

    return s->target_inside;
}

// todo: shield opacity based on health. Lower health -> more dim shield?
void Shield_Draw(const Shield *s, SDL_Renderer *renderer) {
    if (s->health <= 0.f) {
        SDL_SetRenderDrawColor(renderer, 0xEE, 0xEE, 0xEE, 0xFF);
    } else if (s->target_inside == 1) {
        SDL_SetRenderDrawColor(renderer, 0xAA, 0X10, 0X20, 0XFF);
    } else {
        SDL_SetRenderDrawColor(renderer, 0x50, 0X10, 0X20, 0X11);
    }

    draw_circle(renderer, s->position.x, s->position.y, s->radius);
}

void Shield_Destroy(Shield *s) {
    s->position      = (Vec2){0.f, 0.f};
    s->radius        = 0.f;
    s->target_inside = 0;
}
