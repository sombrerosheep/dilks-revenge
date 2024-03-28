#include "entities.h"

static EntityManager GameEntities;

#define clean_container(c) \
    (c)->size = 0;         \
    memset((c)->items, 0, sizeof((c)->items))

void EntityManager_Init(ProjectileContainer *projectiles) {
    clean_container(projectiles);
    projectiles->capacity = MaxProjectiles;

    GameEntities = (EntityManager){
        .projectiles = projectiles,
    };

    return;
}

int EntityManager_AddProjectile(Projectile p) {
    for (unsigned int i = 0; i < GameEntities.projectiles->capacity; i++) {
        if (GameEntities.projectiles->items[i].in_use == 0) {
            GameEntities.projectiles->items[i].data   = p;
            GameEntities.projectiles->items[i].in_use = 1;

            return 0;
        }
    }

    printf("Unable to insert projectile...\n");
    return -1;
}

static void EntityManager_UpdateProjectiles(float delta) {
    for (unsigned int i = 0; i < GameEntities.projectiles->capacity; i++) {
        if (GameEntities.projectiles->items[i].in_use == 1) {
            Projectile_Update(&GameEntities.projectiles->items[i].data, delta);
        }
    }

    return;
}

void EntityManager_Update(float delta) {
    EntityManager_UpdateProjectiles(delta);
}

static void EntityManager_DrawProjectiles(SDL_Renderer *renderer) {
    for (unsigned int i = 0; i < GameEntities.projectiles->capacity; i++) {
        if (GameEntities.projectiles->items[i].in_use == 1) {
            Projectile_Draw(&GameEntities.projectiles->items[i].data, renderer);
        }
    }

    return;
}

void EntityManager_Draw(SDL_Renderer *renderer) {
    EntityManager_DrawProjectiles(renderer);
}
