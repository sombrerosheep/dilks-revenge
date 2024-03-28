#include "resources.h"

#include "globals.h"

static ResourceManager GameResources;

void ResourceManager_Init(Camera *camera, GameInput *controller) {
    GameResources.game_camera = camera;
    GameResources.controller  = controller;
}

Camera *ResourceManager_GetMainCamera(void) {
    return GameResources.game_camera;
}

const GameInput *ResourceManager_GetController(void) {
    return GameResources.controller;
}

void ResourceManager_Destroy(void) {
    GameResources.game_camera = NULL;
    GameResources.controller  = NULL;
}
