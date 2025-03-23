#ifndef DREV_SPRITE_H
#define DREV_SPRITE_H

#include "texture.h"
#include "vec.h"
#include <SDL.h>

typedef struct drev_sprite Sprite;

struct drev_sprite {
    Vec2     pos;
    Texture *texture;
    f32      rotation;
};

bool Sprite_Init(Sprite *s, Texture *t);

// Returns the sprites bounding box in world position units.
// position is center origin.
SDL_FRect Sprite_GetBounds(Sprite *s);
void      Sprite_SetPosition(Sprite *s, Vec2 p);
void      Sprite_SetPositionF(Sprite *s, f32 x, f32 y);
void      Sprite_SetRotation(Sprite *s, f32 deg);
void      Sprite_Draw(const Sprite *s);

// Draws a sprite at a specific position
void Sprite_DrawAt(const Sprite *s, Vec2 pos);

#endif // DREV_SPRITE_H
