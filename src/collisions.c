#include "collisions.h"

#include "entities.h"
#include "globals.h"
#include "smallship.h"

void HandlePlayerProjectileCollision(Player *player, Projectile *projectile) {
    UNUSED(player);
    Entities_DamagePlayer(projectile->strength);
}

void HandleSmallShipProjectileCollision(SmallShip *ship, Projectile *projectile) {
    SDL_Log("Damaging Small ship!");
    SmallShip_Damage(ship, projectile->strength);
}
