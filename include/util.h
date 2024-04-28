#ifndef DREV_UTIL_H
#define DREV_UTIL_H

#include "vec.h"

// Maths

f32 clamp(f32 value, f32 upper, f32 lower);

f32 ease(f32 have, f32 want, f32 ease);

// Containers

#define CONCAT(a, b)     a##b
#define CONCAT3(a, b, c) a##b##c

#define DefineContainedType(T, S)                                      \
    typedef struct CONCAT(drev_contained_, T) CONCAT(Contained, T);    \
    struct CONCAT(drev_contained_, T) {                                \
        i8 in_use;                                                     \
        T  data;                                                       \
    };                                                                 \
    typedef struct CONCAT3(drev_, T, _container) CONCAT(T, Container); \
    struct CONCAT3(drev_, T, _container) {                             \
        u32 capacity;                                                  \
        u32 size;                                                      \
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
