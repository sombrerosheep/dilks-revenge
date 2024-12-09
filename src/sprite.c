#include "sprite.h"

#include "resources.h"
#include "vec.h"

bool Sprite_Init(Sprite *s, Texture t) {
    s->pos      = Vec2_Zero;
    s->texture  = t;
    s->rotation = 0.f;

    return true;
}

void Sprite_SetPosition(Sprite *s, Vec2 p) {
    s->pos = p;
}

void Sprite_SetRotation(Sprite *s, f32 deg) {
    s->rotation = deg;
}

void Sprite_Draw(Sprite *s) {
    Camera *cam = Resources_GetMainCamera();

    Camera_DrawTexture(cam, &s->texture, s->pos, s->rotation);
}
