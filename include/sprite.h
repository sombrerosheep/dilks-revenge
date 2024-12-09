#ifndef DREV_SPRITE_H
#define DREV_SPRITE_H

#include "texture.h"
#include "vec.h"

typedef struct drev_sprite Sprite;

struct drev_sprite {
    Vec2    pos;
    Texture texture;
    f32     rotation;
};

bool Sprite_Init(Sprite *s, Texture t);
void Sprite_SetPosition(Sprite *s, Vec2 p);
void Sprite_SetRotation(Sprite *s, f32 deg);
void Sprite_Draw(Sprite *s);

#endif // DREV_SPRITE_H
