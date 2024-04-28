#include "collisions.h"

#include "entities.h"
#include "globals.h"

void HandlePlayerProjectileCollision(Player *player, Projectile *projectile) {
    UNUSED(player);
    Entities_DamagePlayer(projectile->strength);
}
