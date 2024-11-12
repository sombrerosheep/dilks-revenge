#include "resources.h"

#include "system.h"

static ResourceManager GameResources;

void Resources_Init(Camera       *camera,
                    Camera       *ui_camera,
                    GameInput    *controller,
                    SDL_Renderer *renderer,
                    SysConfig    *sys_config,
                    Font         *title_font,
                    Font         *menu_font,
                    Font         *debug_font) {
    GameResources.game_camera = camera;
    GameResources.ui_camera   = ui_camera;
    GameResources.controller  = controller;
    GameResources.renderer    = renderer;
    GameResources.sys_config  = sys_config;
    GameResources.title_font  = title_font;
    GameResources.menu_font   = menu_font;
    GameResources.debug_font  = debug_font;
}

Camera *Resources_GetMainCamera(void) {
    return GameResources.game_camera;
}

Camera *Resources_GetUICamera(void) {
    return GameResources.ui_camera;
}

const GameInput *Resources_GetController(void) {
    return GameResources.controller;
}

SDL_Renderer *Resources_GetRenderer(void) {
    return GameResources.renderer;
}

Font *Resources_GetTitleFont(void) {
    return GameResources.title_font;
}

Font *Resources_GetMenuFont(void) {
    return GameResources.menu_font;
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
    GameResources.title_font  = NULL;
    GameResources.menu_font   = NULL;
    GameResources.debug_font  = NULL;
}
