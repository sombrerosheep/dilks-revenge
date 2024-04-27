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

void Entities_Init(ProjectileContainer *projectiles,
                   SmallShipContainer  *smallShips,
                   Player              *player);

int  Entities_AddProjectile(Projectile p);
int  Entities_InsertSmallShip(SmallShip ship);
int  Entities_ClearSmallShips(void);
Vec2 Entities_GetPlayerPosition(void);
void Entities_MovePlayerTo(Vec2 target);
void Entities_Update(float delta);
void Entities_Draw(void);

#endif // DREV_ENTITIES_H
