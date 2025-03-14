#ifndef DREV_UTIL_H
#define DREV_UTIL_H

#include "vec.h"

#include <SDL.h>

#define bit_set(t, slot)                \
    SDL_assert(slot < (sizeof(t) * 8)); \
    (t) |= (1 << slot)

#define bit_unset(t, slot)              \
    SDL_assert(slot < (sizeof(t) * 8)); \
    (t) ^= (1 << slot)

#define bit_isset(t, slot) (t) & (1 << slot)

// Maths

f32 clamp(f32 value, f32 upper, f32 lower);

f32 ease(f32 have, f32 want, f32 ease);

f32 slope_to_deg(Vec2 vec);

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

extern SDL_Color ColorWhite;
extern SDL_Color ColorRed;
extern SDL_Color ColorGreen;
extern SDL_Color ColorBlue;
extern SDL_Color ColorCyan;
extern SDL_Color ColorBlack;
extern SDL_Color ColorGrey;
extern SDL_Color ColorLightGrey;

void SetTextureColorMod(SDL_Texture *texture, SDL_Color color);
void SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color color);
void draw_plus(SDL_Renderer *r, Vec2 p, f32 len);

#endif // DREV_UTIL_H
