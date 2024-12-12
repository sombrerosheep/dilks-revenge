#ifndef DREV_ASSETS_H
#define DREV_ASSETS_H

#include "texture.h"
#include "types.h"

#include <SDL.h>

typedef enum drev_texture_id {
    TextureIDPlayerHD = 0,
    TextureIDPlayerFHD,
    TextureIDPlayerQHD,
    TextureIDPlayer4K,
    TextureIDPlayer8K,

    TextureIDCount
} TextureID;

extern const char *TextureKey[TextureIDCount];

typedef struct drev_asset_catalog Assets;

extern Assets GameAssets;

void     Assets_Init(const char *base_path);
bool     Assets_LoadAllTextures(SDL_Renderer *r);
Texture *Assets_GetTexture(TextureID key);

#endif // DREV_ASSETS_H
