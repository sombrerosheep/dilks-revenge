#include "collisions.h"

#include "entities.h"
#include "globals.h"
#include "smallship.h"

void HandlePlayerProjectileCollision(Player *player, Projectile *projectile) {
    Player_Damage(player, projectile->strength);
    Entities_KillProjectile(projectile);
}

void HandlePlayerSmallShipCollision(Player *player, SmallShip *ship) {
    Player_Kill(player);
    SmallShip_Kill(ship);
}

void HandleSmallShipProjectileCollision(SmallShip *ship, Projectile *projectile) {
    SmallShip_Damage(ship, projectile->strength);
    Entities_KillProjectile(projectile);
}
