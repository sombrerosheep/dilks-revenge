#include "resources.h"

#include "globals.h"

void ResourceManager_Init(ResourceManager *manager) {
    UNUSED(manager);
}

Camera *ResourceManager_GetMainCamera(ResourceManager *manager) {
    return &manager->game_camera;
}

void ResourceManager_SetMainCamera(ResourceManager *manager, Camera camera) {
    manager->game_camera = camera;
}

void ResourceManager_Destroy(ResourceManager *manager) {
    UNUSED(manager);
}
