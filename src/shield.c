#include "shield.h"

#include <stdio.h>

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
}

// void Shield_Update(Shield *s) {
//   return;
// }

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

void Shield_Draw(const Shield *s, SDL_Renderer *renderer) {

    if (s->target_inside == 1) {
        SDL_SetRenderDrawColor(renderer, 0xAA, 0X10, 0X20, 0XFF);
        draw_circle(renderer, s->position.x, s->position.y, s->radius);
    } else {
        SDL_SetRenderDrawColor(renderer, 0x50, 0X10, 0X20, 0X11);
        draw_circle(renderer, s->position.x, s->position.y, s->radius);
    }
}

void Shield_Destroy(Shield *s) {
    s->position      = (Vec2){0.f, 0.f};
    s->radius        = 0.f;
    s->target_inside = 0;
}
