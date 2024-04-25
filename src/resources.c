#include "resources.h"

#include "system.h"

static ResourceManager GameResources;

void ResourceManager_Init(Camera       *camera,
                          GameInput    *controller,
                          SDL_Renderer *renderer,
                          SysConfig    *sys_config,
                          Font         *debug_font) {
    GameResources.game_camera = camera;
    GameResources.controller  = controller;
    GameResources.renderer    = renderer;
    GameResources.sys_config  = sys_config;
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

const SysConfig *ResourceManager_GetSysConfig(void) {
    return GameResources.sys_config;
}

void ResourceManager_Destroy(void) {
    GameResources.game_camera = NULL;
    GameResources.controller  = NULL;
    GameResources.renderer    = NULL;
    GameResources.sys_config  = NULL;
    GameResources.debug_font  = NULL;
}
