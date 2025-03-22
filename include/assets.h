#ifndef DREV_ASSETS_H
#define DREV_ASSETS_H

#include "system.h"
#include "texture.h"
#include "types.h"

#include <SDL.h>

// assume sysconfig is _only_ applied on start and not dynamic
// resolution specific textures are loaded to the individual
// texture id's on start.
// assets stores the textures internally and entities (sprites)
// will request the texture as needed, instead of "owning" them.
typedef enum drev_texture_id {
    TextureIDPlayer = 0,
    TextureIDLightShip,
    TextureIDHeavyShip,

    TextureIDBackgroundNear,
    TextureIDBackgroundMid,
    TextureIDBackgroundFar,

    TextureIDCount
} TextureID;

extern const char *TextureKey[TextureIDCount];

typedef struct drev_asset_catalog Assets;

extern Assets GameAssets;

void     Assets_Init(const char *base_path);
bool     Assets_LoadAllTextures(SysConfig *config, SDL_Renderer *r);
Texture *Assets_GetTexture(TextureID key);

#endif // DREV_ASSETS_H
