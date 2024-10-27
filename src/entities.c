#include "entities.h"

#include "camera.h"
#include "collisions.h"
#include "player.h"
#include "projectile.h"
#include "resources.h"
#include "smallship.h"

static EntityManager GameEntities;

#define clean_container(c) \
    (c)->size = 0;         \
    memset((c)->items, 0, sizeof((c)->items))

void Entities_Init(ProjectileContainer *projectiles,
                   SmallShipContainer  *small_ships,
                   Player              *player //
) {
    clean_container(projectiles);
    projectiles->capacity = MaxProjectiles;

    clean_container(small_ships);
    small_ships->capacity = MaxSmallShips;

    GameEntities = (EntityManager){
        .projectiles = projectiles,
        .player      = player,
        .small_ships = small_ships,
    };

    return;
}

i8 Entities_AddProjectile(Projectile p) {
    for (u32 i = 0; i < GameEntities.projectiles->capacity; i++) {
        if (GameEntities.projectiles->items[i].in_use == 0) {
            GameEntities.projectiles->items[i].data   = p;
            GameEntities.projectiles->items[i].in_use = 1;

            return 0;
        }
    }

    SDL_LogError(SDL_LOG_PRIORITY_WARN, "Unable to insert projectile. No slots availablen");
    return -1;
}

ContainedSmallShip *Entities_InsertSmallShip(SmallShip ship) {
    for (u32 i = 0; i < GameEntities.small_ships->capacity; i++) {
        if (GameEntities.small_ships->items[i].in_use == 0) {
            GameEntities.small_ships->items[i].data   = ship;
            GameEntities.small_ships->items[i].in_use = 1;

            return &GameEntities.small_ships->items[i];
        }
    }

    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unable to insert small ship. No slots available\n");
    return NULL;
}

i8 Entities_ClearSmallShips(void) {
    clean_container(GameEntities.small_ships);

    return 0;
}

Vec2 Entities_GetPlayerPosition(void) {
    return GameEntities.player->position;
}

void Entities_MovePlayerTo(Vec2 target) {
    Player_MoveTo(GameEntities.player, target);
}

static void Entities_UpdateProjectiles(SDL_FRect bounds, f32 delta) {
    for (u32 i = 0; i < GameEntities.projectiles->capacity; i++) {
        if (GameEntities.projectiles->items[i].in_use == 1) {
            SDL_FRect bb = Projectile_GetBounds(&GameEntities.projectiles->items[i].data);
            if (SDL_HasIntersectionF(&bb, &bounds) == SDL_FALSE) {
                GameEntities.projectiles->items[i].in_use = 0;
            } else {
                Projectile_Update(&GameEntities.projectiles->items[i].data, delta);
            }
        }
    }

    return;
}

static void Entities_UpdateSmallShips(f32 delta) {
    for (u32 i = 0; i < GameEntities.small_ships->capacity; i++) {
        if (GameEntities.small_ships->items[i].in_use == 1) {
            if (GameEntities.small_ships->items[i].data.health < 1) {
                GameEntities.small_ships->items[i].in_use = 0;
                continue;
            }

            SmallShip_Update(&GameEntities.small_ships->items[i].data, delta);
        }
    }

    return;
}

void Entities_Update(f32 delta) {
    Camera   *camera = Resources_GetMainCamera();
    SDL_FRect bounds = Camera_GetBounds(camera);

    Entities_UpdateProjectiles(bounds, delta);
    Entities_UpdateSmallShips(delta);
    Player_Update(GameEntities.player, delta);
}

void Entities_KillProjectile(Projectile *p) {
    for (u32 i = 0; i < GameEntities.projectiles->capacity; i++) {
        if (&GameEntities.projectiles->items[i].data == p) {
            GameEntities.projectiles->items[i].in_use = 0;
            return;
        }
    }

    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "Entities_KillProjectile::Not found (%p)\n",
                (void *)p);
}

static void CheckAndHandlePlayerSmallShipCollision(void) {
    for (u32 i = 0; i < GameEntities.small_ships->capacity; i++) {
        ContainedSmallShip *ship = &GameEntities.small_ships->items[i];
        if (ship->in_use == 0) {
            continue;
        }

        SDL_FRect player_bb = Player_GetBounds(GameEntities.player);
        SDL_FRect ship_bb   = SmallShip_GetBounds(&ship->data);
        if (SDL_HasIntersectionF(&player_bb, &ship_bb)) {
            HandlePlayerSmallShipCollision(GameEntities.player, &ship->data);
        }
    }
}

static void CheckAndHandleProjectileSmallShipCollision(Projectile *p) {
    SDL_FRect projectile_bb = Projectile_GetBounds(p);

    for (u32 i = 0; i < GameEntities.small_ships->capacity; i++) {
        ContainedSmallShip *ship = &GameEntities.small_ships->items[i];
        if (ship->in_use == 1) {
            SDL_FRect ship_bb = SmallShip_GetBounds(&ship->data);
            if (SDL_HasIntersectionF(&projectile_bb, &ship_bb)) {
                HandleSmallShipProjectileCollision(&ship->data, p);
            }
        }
    }
}

static void CheckAndHandleProjectileCollisions(void) {
    for (u32 i = 0; i < GameEntities.projectiles->capacity; i++) {
        ContainedProjectile *p = &GameEntities.projectiles->items[i];
        if (p->in_use == 0) {
            continue;
        }

        // Projectiles + player
        if (Projectile_CanHurtPlayer(&p->data)) {
            SDL_FRect player     = Player_GetBounds(GameEntities.player);
            SDL_FRect projectile = Projectile_GetBounds(&p->data);
            if (SDL_HasIntersectionF(&player, &projectile)) {
                HandlePlayerProjectileCollision(GameEntities.player, &p->data);
            }
        } else if (p->data.type == ProjectileType_Player) {
            // Projectiles + SmallShip
            CheckAndHandleProjectileSmallShipCollision(&p->data);
        }
    }
}

void Entities_CheckAndHandleCollisions(void) {
    // Projectiles + SmallShip | Player
    CheckAndHandleProjectileCollisions();

    // SmallShip + Player
    CheckAndHandlePlayerSmallShipCollision();
}

static void Entities_DrawProjectiles(void) {
    for (u32 i = 0; i < GameEntities.projectiles->capacity; i++) {
        if (GameEntities.projectiles->items[i].in_use == 1) {
            Projectile_Draw(&GameEntities.projectiles->items[i].data);
        }
    }

    return;
}

static void Entities_DrawSmallShips(void) {
    for (u32 i = 0; i < GameEntities.small_ships->capacity; i++) {
        if (GameEntities.small_ships->items[i].in_use == 1) {
            SmallShip_Draw(&GameEntities.small_ships->items[i].data);
        }
    }

    return;
}

void Entities_Draw(void) {
    Entities_DrawProjectiles();
    Entities_DrawSmallShips();
    Player_Draw(GameEntities.player);
}
