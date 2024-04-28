#ifndef DREV_COLLISIONS_H
#define DREV_COLLISIONS_H

#include "player.h"
#include "projectile.h"
#include "smallship.h"

void HandlePlayerProjectileCollision(Player *player, Projectile *projectile);
void HandlePlayerSmallShipCollision(Player *player, SmallShip *ship);
void HandleSmallShipProjectileCollision(SmallShip *ship, Projectile *projectile);

#endif // DREV_COLLISIONS_H
