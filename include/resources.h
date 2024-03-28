#ifndef DREV_RESOURCES_H
#define DREV_RESOURCES_H

#include "camera.h"
#include "game_input.h"

#include <SDL.h>

typedef struct drev_resources ResourceManager;

struct drev_resources {
    Camera    *game_camera;
    GameInput *controller;
};

void ResourceManager_Init(Camera *camera, GameInput *controller);

Camera          *ResourceManager_GetMainCamera(void);
const GameInput *ResourceManager_GetController(void);

void ResourceManager_Destroy(void);

#endif // DREV_RESOURCES_H
