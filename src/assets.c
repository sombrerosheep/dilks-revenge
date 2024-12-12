#include "assets.h"

#include "font.h"
#include "texture.h"
#include <SDL_log.h>

#define PATH_BUFFER_SZ 1024

const char *TextureKey[TextureIDCount] = {
    [TextureIDPlayerHD]  = "textures/player/player_44.png",
    [TextureIDPlayerFHD] = "textures/player/player_64.png",
    [TextureIDPlayerQHD] = "textures/player/player_86.png",
    [TextureIDPlayer4K]  = "textures/player/player_128.png",
    [TextureIDPlayer8K]  = "textures/player/player_256.png",
};

struct drev_asset_catalog {
    const char *base_path;
    Texture     textures[TextureIDCount];
};

Assets GameAssets;

void Assets_Init(const char *base_path) {
    GameAssets.base_path = base_path;
}

bool Assets_LoadAllTextures(SDL_Renderer *r) {
    static char path_buffer[PATH_BUFFER_SZ];

    for (u32 i = 0; i < TextureIDCount; i++) {
        snprintf(path_buffer, PATH_BUFFER_SZ, "%s%s", GameAssets.base_path, TextureKey[i]);
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
