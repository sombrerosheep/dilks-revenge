#ifndef DREV_BACKGROUND_H
#define DREV_BACKGROUND_H

#include "assets.h"
#include "sprite.h"
#include "types.h"
#include "vec.h"
typedef struct drev_parallax_layer ParallaxLayer;
typedef struct drev_background     ParallaxBackground;

struct drev_parallax_layer {
    Sprite sprite;
    f32    repeat;
};

struct drev_background {
    Vec2          direction;
    ParallaxLayer near;
    ParallaxLayer mid;
    ParallaxLayer far;
};

void Background_Init(ParallaxBackground *b, Vec2 direction);
void Background_Update(ParallaxBackground *b, f32 delta);
void Background_Draw(ParallaxBackground *b);

#endif // DREV_BACKGROUND_H
