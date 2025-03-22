#include "assets.h"

#include "font.h"
#include "texture.h"
#include <SDL_log.h>

#define PATH_BUFFER_SZ 1024

const char *TextureKey[TextureIDCount] = {
    //
    [TextureIDPlayer]    = "textures/player/player",
    [TextureIDLightShip] = "textures/ship/ship-light",
    [TextureIDHeavyShip] = "textures/ship/ship-heavy",

    [TextureIDBackgroundNear] = "textures/background/near",
    [TextureIDBackgroundMid]  = "textures/background/mid",
    [TextureIDBackgroundFar]  = "textures/background/far",
};

struct drev_asset_catalog {
    const char *base_path;
    Texture     textures[TextureIDCount];
};

Assets GameAssets;

void Assets_Init(const char *base_path) {
    GameAssets.base_path = base_path;
}

bool Assets_LoadAllTextures(SysConfig *config, SDL_Renderer *r) {
    static char path_buffer[PATH_BUFFER_SZ];

    for (u32 i = 0; i < TextureIDCount; i++) {
        snprintf(path_buffer,
                 PATH_BUFFER_SZ,
                 "%s%s.%s.png",
                 GameAssets.base_path,
                 TextureKey[i],
                 config->label);
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "loading: %s\n", path_buffer);
        if (!Texture_InitFromFile(&GameAssets.textures[i], path_buffer, r)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                         "failed to load texture asset: %s\n",
                         TextureKey[i]);
            return false;
        }
    }

    return true;
}

Texture *Assets_GetTexture(TextureID key) {
    return &GameAssets.textures[key];
}
