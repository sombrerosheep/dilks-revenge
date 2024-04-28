#ifndef DREV_ENTITIES_H
#define DREV_ENTITIES_H

#include "player.h"
#include "projectile.h"
#include "smallship.h"
#include "types.h"
#include "util.h"

#include <SDL.h>

#define MaxProjectiles 50
DefineContainedType(Projectile, MaxProjectiles);

#define MaxSmallShips 20
DefineContainedType(SmallShip, MaxSmallShips);

typedef struct drev_entity_manager EntityManager;

struct drev_entity_manager {
    ProjectileContainer *projectiles;
    SmallShipContainer  *small_ships;
    Player              *player;
};

void Entities_Init(ProjectileContainer *projectiles,
                   SmallShipContainer  *small_ships,
                   Player              *player);

i8   Entities_AddProjectile(Projectile p);
i8   Entities_InsertSmallShip(SmallShip ship);
i8   Entities_ClearSmallShips(void);
Vec2 Entities_GetPlayerPosition(void);
void Entities_MovePlayerTo(Vec2 target);
void Entities_KillProjectile(Projectile *p);

void Entities_CheckAndHandleCollisions(void);

void Entities_Update(f32 delta);
void Entities_Draw(void);

#endif // DREV_ENTITIES_H
