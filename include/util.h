#ifndef DREV_UTIL_H
#define DREV_UTIL_H

#include "vec.h"

// Maths

float clamp(float value, float upper, float lower);

float ease(float have, float want, float ease);

// Containers

#define CONCAT(a, b)     a##b
#define CONCAT3(a, b, c) a##b##c

#define DefineContainedType(T, S)                                      \
    typedef struct CONCAT(drev_contained_, T) CONCAT(Contained, T);    \
    struct CONCAT(drev_contained_, T) {                                \
        int in_use;                                                    \
        T   data;                                                      \
    };                                                                 \
    typedef struct CONCAT3(drev_, T, _container) CONCAT(T, Container); \
    struct CONCAT3(drev_, T, _container) {                             \
        unsigned int capacity;                                         \
        unsigned int size;                                             \
        CONCAT(Contained, T) items[S];                                 \
    }

// SDL helpers

#include <SDL.h>

extern SDL_Color ColorWhite;
extern SDL_Color ColorRed;
extern SDL_Color ColorGreen;
extern SDL_Color ColorBlue;
extern SDL_Color ColorCyan;

void SetTextureColorMod(SDL_Texture *texture, SDL_Color color);
void SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color color);
void draw_plus(SDL_Renderer *r, Vec2 p);

#endif // DREV_UTIL_H
