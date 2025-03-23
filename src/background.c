#include "background.h"
#include "assets.h"
#include "camera.h"
#include "globals.h"
#include "resources.h"
#include "sprite.h"
#include "texture.h"
#include "types.h"
#include "util.h"
#include <SDL.h>

#define BACKGROUND_SPEED    20.f
#define BACKGROUND_FAR_MOD  1.f
#define BACKGROUND_MID_MOD  1.25f
#define BACKGROUND_NEAR_MOD 1.45f

static void ParallaxLayer_Update(ParallaxLayer *l, Vec2 velocity, f32 delta) {
    SDL_FRect cam_bounds = Camera_GetBounds(Resources_GetMainCamera());
    SDL_FRect rect       = Sprite_GetBounds(&l->sprite);
    Vec2      half       = {.x = rect.w / 2.f, .y = rect.h / 2.f};

    l->sprite.pos.x += velocity.x * l->repeat * delta;
    if (rect.x - half.x > cam_bounds.x + cam_bounds.w) {
        l->sprite.pos.x -= rect.w;
    }

    if (rect.x + half.x < cam_bounds.x) {
        l->sprite.pos.x += rect.w;
    }

    l->sprite.pos.y += velocity.y * l->repeat * delta;
    if (rect.y - half.y > cam_bounds.y + cam_bounds.h) {
        l->sprite.pos.y -= rect.h;
    }

    if (rect.y + half.y < cam_bounds.y) {
        l->sprite.pos.y += rect.h;
    }
}

static void ParallaxLayer_Draw(ParallaxLayer *l, Vec2 velocity) {
    UNUSED(velocity);
    // draw the tracked sprite
    Sprite_Draw(&l->sprite);

    SDL_FRect bounds = Sprite_GetBounds(&l->sprite);

    // todo: this assumes the layer is at least the same size as
    // the camera bounds but in a lazy way
    // this should be updated to "ftile" the images based on
    // the texture, camera bounds and velocity.
    if (velocity.x < 0.f) {
        Vec2 pos = l->sprite.pos;
        pos.x += bounds.w;

        Sprite_DrawAt(&l->sprite, pos);
    } else if (velocity.x > 0.f) {
        Vec2 pos = l->sprite.pos;
        pos.x -= bounds.w;

        Sprite_DrawAt(&l->sprite, pos);
    }

    if (velocity.y < 0.f) {
        Vec2 pos = l->sprite.pos;
        pos.y += bounds.h;

        Sprite_DrawAt(&l->sprite, pos);
    } else if (velocity.y > 0.f) {
        Vec2 pos = l->sprite.pos;
        pos.y -= bounds.h;

        Sprite_DrawAt(&l->sprite, pos);
    }

    // todo: handle multi-axis velocity
    // this only accounts for x or y plane velocity not both
    // if we were moving in a diagonal direction, we'd have an
    // issue.

#if DREV_DRAW_BB
    Camera_DrawRect(Resources_GetMainCamera(), bounds, ColorRed);
#endif
}

void Background_Init(ParallaxBackground *b, Vec2 velocity) {
    b->velocity = velocity;

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
    ParallaxLayer_Update(&b->near, b->velocity, delta);
    ParallaxLayer_Update(&b->mid, b->velocity, delta);
    ParallaxLayer_Update(&b->far, b->velocity, delta);
}

void Background_Draw(ParallaxBackground *b) {
    ParallaxLayer_Draw(&b->near, b->velocity);
    ParallaxLayer_Draw(&b->mid, b->velocity);
    ParallaxLayer_Draw(&b->far, b->velocity);
}
