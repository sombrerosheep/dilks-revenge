#ifndef DREV_ENTITIES_H
#define DREV_ENTITIES_H

#include "player.h"
#include "projectile.h"
#include "smallship.h"
#include "util.h"

#include <SDL.h>

#define MaxProjectiles 50
DefineContainedType(Projectile, MaxProjectiles);

#define MaxSmallShips 20
DefineContainedType(SmallShip, MaxSmallShips);

typedef struct drev_entity_manager EntityManager;

struct drev_entity_manager {
    ProjectileContainer *projectiles;
    SmallShipContainer  *smallShips;
    Player              *player;
};

void EntityManager_Init(ProjectileContainer *projectiles,
                        SmallShipContainer  *smallShips,
                        Player              *player);

int  EntityManager_AddProjectile(Projectile p);
int  EntityManager_InsertSmallShip(SmallShip ship);
int  EntityManager_ClearSmallShips(void);
Vec2 EntityManager_GetPlayerPosition(void);
void EntityManager_Update(float delta);
void EntityManager_Draw(SDL_Renderer *renderer);

#endif // DREV_ENTITIES_H
