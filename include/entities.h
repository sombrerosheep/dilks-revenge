#ifndef DREV_ENTITIES_H
#define DREV_ENTITIES_H

#include "projectile.h"
#include "util.h"

#include <SDL.h>

#define MaxProjectiles 50
DefineContainedType(Projectile, MaxProjectiles);

typedef struct drev_entity_manager EntityManager;

struct drev_entity_manager {
    ProjectileContainer *projectiles;
};

void EntityManager_Init(ProjectileContainer *projectiles);

int  EntityManager_AddProjectile(Projectile p);
void EntityManager_Update(float delta);
void EntityManager_Draw(SDL_Renderer *renderer);

#endif // DREV_ENTITIES_H
