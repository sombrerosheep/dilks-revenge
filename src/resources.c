#include "resources.h"

#include "system.h"

static ResourceManager GameResources;

void Resources_Init(Camera       *camera,
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

Camera *Resources_GetMainCamera(void) {
    return GameResources.game_camera;
}

const GameInput *Resources_GetController(void) {
    return GameResources.controller;
}

SDL_Renderer *Resources_GetRenderer(void) {
    return GameResources.renderer;
}

Font *Resources_GetDebugFont(void) {
    return GameResources.debug_font;
}

const SysConfig *Resources_GetSysConfig(void) {
    return GameResources.sys_config;
}

void Resources_Destroy(void) {
    GameResources.game_camera = NULL;
    GameResources.controller  = NULL;
    GameResources.renderer    = NULL;
    GameResources.sys_config  = NULL;
    GameResources.debug_font  = NULL;
}
