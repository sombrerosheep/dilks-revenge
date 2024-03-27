#ifndef DREV_RESOURCES_H
#define DREV_RESOURCES_H

#include "camera.h"
#include "game_input.h"
#include "projectile.h"

#include <SDL.h>

typedef struct drev_resources ResourceManager;

struct drev_resources {
    Camera                  *game_camera;
    GameInput               *controller;
    ProjectileGameContainer *projectiles;
};

void ResourceManager_Init(ResourceManager         *manager,
                          Camera                  *camera,
                          GameInput               *controller,
                          ProjectileGameContainer *projectiles);

Camera                  *ResourceManager_GetMainCamera(ResourceManager *manager);
const GameInput         *ResourceManager_GetController(ResourceManager *manager);
ProjectileGameContainer *ResourceManager_GetProjectileContainer(ResourceManager *manager);

void ResourceManager_Destroy(ResourceManager *resources);

#endif // DREV_RESOURCES_H
