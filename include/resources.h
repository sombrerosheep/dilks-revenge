#ifndef DREV_RESOURCES_H
#define DREV_RESOURCES_H

#include "camera.h"
#include "game_input.h"

#include <SDL.h>

typedef struct drev_resources ResourceManager;

struct drev_resources {
    Camera       *game_camera;
    GameInput    *controller;
    SDL_Renderer *renderer;
};

void ResourceManager_Init(Camera *camera, GameInput *controller, SDL_Renderer *renderer);

Camera          *ResourceManager_GetMainCamera(void);
const GameInput *ResourceManager_GetController(void);
SDL_Renderer    *ResourceManager_GetRenderer(void);

void ResourceManager_Destroy(void);

#endif // DREV_RESOURCES_H
