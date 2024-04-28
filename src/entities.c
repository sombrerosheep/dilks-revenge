#include "entities.h"

#include "camera.h"
#include "player.h"
#include "projectile.h"
#include "resources.h"
#include "smallship.h"
#include <SDL_log.h>
#include <SDL_rect.h>

static EntityManager GameEntities;

#define clean_container(c) \
    (c)->size = 0;         \
    memset((c)->items, 0, sizeof((c)->items))

void Entities_Init(ProjectileContainer *projectiles,
                   SmallShipContainer  *smallShips,
                   Player              *player //
) {
    clean_container(projectiles);
    projectiles->capacity = MaxProjectiles;

    clean_container(smallShips);
    smallShips->capacity = MaxSmallShips;

    GameEntities = (EntityManager){
        .projectiles = projectiles,
        .player      = player,
        .smallShips  = smallShips,
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

    SDL_LogError(SDL_LOG_PRIORITY_WARN, "Unable to insert projectile...\n");
    return -1;
}

i8 Entities_InsertSmallShip(SmallShip ship) {
    for (u32 i = 0; i < GameEntities.smallShips->capacity; i++) {
        if (GameEntities.smallShips->items[i].in_use == 0) {
            GameEntities.smallShips->items[i].data   = ship;
            GameEntities.smallShips->items[i].in_use = 1;

            return 0;
        }
    }

    return -1;
}

i8 Entities_ClearSmallShips(void) {
    clean_container(GameEntities.smallShips);

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
    for (u32 i = 0; i < GameEntities.smallShips->capacity; i++) {
        if (GameEntities.smallShips->items[i].in_use == 1) {
            SmallShip_Update(&GameEntities.smallShips->items[i].data, delta);
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

void Entities_DamagePlayer(u64 amount) {
    Player_Damage(GameEntities.player, amount);
}

void Entities_CheckAndHandleCollisions(void) {

    // Projectiles & Player
    for (u32 i = 0; i < GameEntities.projectiles->capacity; i++) {
        ContainedProjectile *p = &GameEntities.projectiles->items[i];
        if (p->in_use == 1 && Projectile_CanHurtPlayer(&p->data)) {
            i8        Projectile_CanHurtPlayer(const Projectile *p);
            SDL_FRect player     = Player_GetBounds(GameEntities.player);
            SDL_FRect projectile = Projectile_GetBounds(&p->data);
            if (SDL_HasIntersectionF(&player, &projectile)) {
                Player_Damage(GameEntities.player, p->data.strength);
                Entities_KillProjectile(&p->data);
            }
        }
    }
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
    for (u32 i = 0; i < GameEntities.smallShips->capacity; i++) {
        if (GameEntities.smallShips->items[i].in_use == 1) {
            SmallShip_Draw(&GameEntities.smallShips->items[i].data);
        }
    }

    return;
}

void Entities_Draw(void) {
    Entities_DrawProjectiles();
    Entities_DrawSmallShips();
    Player_Draw(GameEntities.player);
}
