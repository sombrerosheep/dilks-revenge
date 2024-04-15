#include "resources.h"

static ResourceManager GameResources;

void ResourceManager_Init(Camera       *camera,
                          GameInput    *controller,
                          SDL_Renderer *renderer,
                          Font         *debug_font) {
    GameResources.game_camera = camera;
    GameResources.controller  = controller;
    GameResources.renderer    = renderer;
    GameResources.debug_font  = debug_font;
}

Camera *ResourceManager_GetMainCamera(void) {
    return GameResources.game_camera;
}

const GameInput *ResourceManager_GetController(void) {
    return GameResources.controller;
}

SDL_Renderer *ResourceManager_GetRenderer(void) {
    return GameResources.renderer;
}

Font *ResourceManager_GetDebugFont(void) {
    return GameResources.debug_font;
}

void ResourceManager_Destroy(void) {
    GameResources.game_camera = NULL;
    GameResources.controller  = NULL;
}
