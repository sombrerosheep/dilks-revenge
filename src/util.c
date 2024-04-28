#include "util.h"

f32 clamp(f32 value, f32 upper, f32 lower) {
    f32 result = value;

    if (result > upper) {
        result = upper;
    }

    if (result < lower) {
        result = lower;
    }

    return result;
}

f32 ease(f32 have, f32 want, f32 ease) {
    if (have == want) {
        return have;
    }

    f32 dir = (have > want) ? -1.f : 1.f;
    have += dir * ease;

    f32 result = clamp( //
        have,
        dir > 0.f ? want : have,
        dir < 0.f ? want : have);

    return result;
}

SDL_Color ColorWhite = {0xFF, 0xFF, 0xFF, 0xFF};
SDL_Color ColorRed   = {0xFF, 0x0, 0x0, 0xFF};
SDL_Color ColorGreen = {0x0, 0xFF, 0x0, 0xFF};
SDL_Color ColorBlue  = {0x0, 0x0, 0xFF, 0xFF};
SDL_Color ColorCyan  = {0x0, 0xFF, 0xFF, 0xFF};

void SetTextureColorMod(SDL_Texture *texture, SDL_Color color) {
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
}

void SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void draw_plus(SDL_Renderer *r, Vec2 p) {
    Vec2 v1, v2;
    Vec2 h1, h2;

    v1.x = p.x;
    v1.y = p.y - 10.f;
    v2.x = p.x;
    v2.y = p.y + 10.f;

    h1.x = p.x - 10.f;
    h1.y = p.y;
    h2.x = p.x + 10.f;
    h2.y = p.y;

    SetRenderDrawColor(r, ColorCyan);
    SDL_RenderDrawLineF(r, v1.x, v1.y, v2.x, v2.y);
    SDL_RenderDrawLineF(r, h1.x, h1.y, h2.x, h2.y);

    return;
}
