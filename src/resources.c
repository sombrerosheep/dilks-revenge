#include "resources.h"

#include "globals.h"

void ResourceManager_Init(ResourceManager *resources, Camera *camera, GameInput *controller) {
    resources->game_camera = camera;
    resources->controller  = controller;
}

Camera *ResourceManager_GetMainCamera(ResourceManager *manager) {
    return manager->game_camera;
}

const GameInput *ResourceManager_GetController(ResourceManager *manager) {
    return manager->controller;
}

void ResourceManager_Destroy(ResourceManager *manager) {
    UNUSED(manager);
}
