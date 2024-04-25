#ifndef DREV_RESOURCES_H
#define DREV_RESOURCES_H

#include "camera.h"
#include "font.h"
#include "game_input.h"
#include "system.h"

#include <SDL.h>

typedef struct drev_resources ResourceManager;

struct drev_resources {
    Camera       *game_camera;
    GameInput    *controller;
    SDL_Renderer *renderer;
    Font         *debug_font;
    SysConfig    *sys_config;
};

void ResourceManager_Init(Camera       *camera,
                          GameInput    *controller,
                          SDL_Renderer *renderer,
                          SysConfig    *sys_config,
                          Font         *debug_font);

Camera          *ResourceManager_GetMainCamera(void);
const GameInput *ResourceManager_GetController(void);
SDL_Renderer    *ResourceManager_GetRenderer(void);
Font            *ResourceManager_GetDebugFont(void);
const SysConfig *ResourceManager_GetSysConfig(void);

void ResourceManager_Destroy(void);

#endif // DREV_RESOURCES_H
