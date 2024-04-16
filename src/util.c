#include "util.h"

float clamp(float value, float upper, float lower) {
    float result = value;

    if (result > upper) {
        result = upper;
    }

    if (result < lower) {
        result = lower;
    }

    return result;
}

float ease(float have, float want, float ease) {
    if (have == want) {
        return have;
    }

    float dir = (have > want) ? -1.f : 1.f;
    have += dir * ease;

    float result = clamp( //
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
