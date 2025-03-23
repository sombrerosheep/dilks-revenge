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
#include <SDL_rect.h>

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

static void ParallaxLayer_Draw(ParallaxLayer *l) {
    SDL_FRect cam_bounds = Camera_GetBounds(Resources_GetMainCamera());
    SDL_FRect bounds     = Sprite_GetBounds(&l->sprite);
    Vec2      half       = {.x = bounds.w / 2.f, .y = bounds.h / 2.f};

    // find min and max for texture that goes off screen
    // draw X copies of the sprite.
    f32 min_x = bounds.x - half.x;
    f32 min_y = bounds.y - half.y;
    f32 max_x = bounds.x + half.x;
    f32 max_y = bounds.y + half.y;
    u32 crows = 1;
    u32 ccols = 1;

    while (min_x > cam_bounds.x) {
        min_x -= bounds.w;
        ccols++;
    }

    while (min_y > cam_bounds.y) {
        min_y -= bounds.h;
        crows++;
    }

    while (max_x < cam_bounds.x + bounds.w) {
        max_x += bounds.w;
        ccols++;
    }

    while (max_y < cam_bounds.y + bounds.h) {
        max_y += bounds.h;
        crows++;
    }

    for (u32 r = 0; r < crows; r++) {
        for (u32 c = 0; c < ccols; c++) {
            Vec2 pos = {
                .x = min_x + half.x + (bounds.w * c),
                .y = min_y + half.y + (bounds.h * r),
            };
            Sprite_DrawAt(&l->sprite, pos);
#if DREV_DRAW_PARALLAX_BB
            SDL_FRect bb_bounds = bounds;
            bb_bounds.x         = pos.x;
            bb_bounds.y         = pos.y;
            Camera_DrawRect(Resources_GetMainCamera(), bb_bounds, ColorRed);
#endif
        }
    }
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
    ParallaxLayer_Draw(&b->near);
    ParallaxLayer_Draw(&b->mid);
    ParallaxLayer_Draw(&b->far);
}
