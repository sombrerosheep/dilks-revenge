#include "sprite.h"

#include "resources.h"
#include "system.h"
#include "texture.h"
#include "vec.h"
#include <SDL_rect.h>

bool Sprite_Init(Sprite *s, Texture *t) {
    s->pos      = Vec2_Zero;
    s->texture  = t;
    s->rotation = 0.f;

    return true;
}

SDL_FRect Sprite_GetBounds(Sprite *s) {
    const SysConfig *config = Resources_GetSysConfig();
    Vec2i            txsz   = Texture_GetSize(s->texture);

    SDL_FRect rect = {
        .x = s->pos.x,
        .y = s->pos.y,
        .w = txsz.x / config->ppu,
        .h = txsz.y / config->ppu,
    };

    return rect;
}

void Sprite_SetPosition(Sprite *s, Vec2 p) {
    s->pos = p;
}

void Sprite_SetPositionF(Sprite *s, f32 x, f32 y) {
    s->pos.x = x;
    s->pos.y = y;
}

void Sprite_SetRotation(Sprite *s, f32 deg) {
    s->rotation = deg;
}

void Sprite_Draw(const Sprite *s) {
    Sprite_DrawAt(s, s->pos);
}

void Sprite_DrawAt(const Sprite *s, Vec2 pos) {
    Camera *cam = Resources_GetMainCamera();

    Camera_DrawTexture(cam, s->texture, pos, s->rotation);
}
