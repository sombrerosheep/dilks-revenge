#ifndef DREV_RESOURCES_H
#define DREV_RESOURCES_H

#include "camera.h"

#include <SDL.h>

typedef struct drev_resources ResourceManager;

struct drev_resources {
    Camera game_camera;
};

void ResourceManager_Init(ResourceManager *resources);

Camera *ResourceManager_GetMainCamera(ResourceManager *resources);
void    ResourceManager_SetMainCamera(ResourceManager *resources, Camera camera);

void ResourceManager_Destroy(ResourceManager *resources);

#endif // DREV_RESOURCES_H
