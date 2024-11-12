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
    Camera       *ui_camera;
    GameInput    *controller;
    SDL_Renderer *renderer;
    Font         *title_font;
    Font         *menu_font;
    Font         *debug_font;
    SysConfig    *sys_config;
};

void Resources_Init(Camera       *camera,
                    Camera       *ui_camera,
                    GameInput    *controller,
                    SDL_Renderer *renderer,
                    SysConfig    *sys_config,
                    Font         *title_font,
                    Font         *menu_font,
                    Font         *debug_font);

Camera          *Resources_GetMainCamera(void);
Camera          *Resources_GetUICamera(void);
const GameInput *Resources_GetController(void);
SDL_Renderer    *Resources_GetRenderer(void);
Font            *Resources_GetTitleFont(void);
Font            *Resources_GetMenuFont(void);
Font            *Resources_GetDebugFont(void);
const SysConfig *Resources_GetSysConfig(void);

void Resources_Destroy(void);

#endif // DREV_RESOURCES_H
