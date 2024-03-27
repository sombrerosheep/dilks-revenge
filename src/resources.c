#include "resources.h"

#include "globals.h"
#include "projectile.h"

void ResourceManager_Init(ResourceManager         *resources,
                          Camera                  *camera,
                          GameInput               *controller,
                          ProjectileGameContainer *projectiles) {
    resources->game_camera = camera;
    resources->controller  = controller;
    resources->projectiles = projectiles;
}

Camera *ResourceManager_GetMainCamera(ResourceManager *manager) {
    return manager->game_camera;
}

const GameInput *ResourceManager_GetController(ResourceManager *manager) {
    return manager->controller;
}

ProjectileGameContainer *ResourceManager_GetProjectileContainer(ResourceManager *manager) {
    return manager->projectiles;
}

void ResourceManager_Destroy(ResourceManager *manager) {
    UNUSED(manager);
}
