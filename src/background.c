#include "background.h"
#include "assets.h"
#include "sprite.h"
#include "texture.h"
#include <SDL.h>

#define BACKGROUND_SPEED    20.f
#define BACKGROUND_FAR_MOD  1.f
#define BACKGROUND_MID_MOD  1.25f
#define BACKGROUND_NEAR_MOD 1.45f

static void ParallaxLayer_Update(ParallaxLayer *l, Vec2 direction, f32 delta) {
    Vec2i sz = Texture_GetSize(l->sprite.texture);

    l->sprite.pos.x += direction.x * l->repeat * delta;
    l->sprite.pos.x = SDL_fmod(l->sprite.pos.x, (f32)sz.x);

    l->sprite.pos.y += direction.y * l->repeat * delta;
    l->sprite.pos.y = SDL_fmod(l->sprite.pos.y, (f32)sz.y);
}

static void ParallaxLayer_Draw(ParallaxLayer *l) {
    Sprite_Draw(&l->sprite);
}

void Background_Init(ParallaxBackground *b, Vec2 direction) {
    b->direction = direction;

    b->near = (ParallaxLayer){
        .repeat = BACKGROUND_SPEED * BACKGROUND_NEAR_MOD,
    };
    Sprite_Init(&b->near.sprite, Assets_GetTexture(TextureIDBackgroundNear));

    b->mid = (ParallaxLayer){
        .repeat = BACKGROUND_SPEED * BACKGROUND_MID_MOD,
    };
    Sprite_Init(&b->mid.sprite, Assets_GetTexture(TextureIDBackgroundMid));

    b->far = (ParallaxLayer){
        .repeat = BACKGROUND_SPEED * BACKGROUND_FAR_MOD,
    };
    Sprite_Init(&b->far.sprite, Assets_GetTexture(TextureIDBackgroundFar));
}

void Background_Update(ParallaxBackground *b, f32 delta) {
    ParallaxLayer_Update(&b->near, b->direction, delta);
    ParallaxLayer_Update(&b->mid, b->direction, delta);
    ParallaxLayer_Update(&b->far, b->direction, delta);
}

void Background_Draw(ParallaxBackground *b) {
    ParallaxLayer_Draw(&b->near);
    ParallaxLayer_Draw(&b->mid);
    ParallaxLayer_Draw(&b->far);
}
