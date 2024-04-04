#include "smallship.h"

#include "camera.h"
#include "entities.h"
#include "globals.h"
#include "projectile.h"
#include "random.h"
#include "resources.h"
#include "vec.h"
#include <SDL_stdinc.h>

#define FIRE_RATE_MIN    1.f
#define FIRE_RATE_MAX    1.2f
#define SHIP_SIZE_METERS 6.f

#define ENEMY_PROJECTILE_SPEED 450.f
#define SmallShipSpeed         125.f

int SmallShip_Init(SmallShip *ship) {
    ship->position      = Vec2_Zero;
    ship->size          = Vec2_Newf(SHIP_SIZE_METERS);
    ship->velocity      = Vec2_Zero;
    ship->rotation      = 0.f;
    ship->fire_cooldown = random_getf_between(FIRE_RATE_MIN, FIRE_RATE_MAX);

    return 0;
}

SmallShip SmallShip_Create(Vec2 position, Vec2 velocity, float rotation) {
    SmallShip ship;

    SmallShip_Init(&ship);

    ship.position = position;
    ship.velocity = Vec2_Normalize(velocity);
    ship.rotation = rotation;

    return ship;
}

void SmallShip_MoveTo(SmallShip *ship, Vec2 new_position) {
    ship->target_position = new_position;
}

static void SmallShip_Shoot(Vec2 pos, Vec2 vel) {
    Projectile p;
    Projectile_Init(&p, ProjectileType_Enemy, pos, vel, ENEMY_PROJECTILE_SPEED);
    EntityManager_AddProjectile(p);
}

void SmallShip_Update(SmallShip *ship, float delta) {

    if (Vec2_Equal(ship->position, ship->target_position) == 0) {
        const float speed = 185.f;
        ship->position.x  = ease(ship->position.x, ship->target_position.x, delta * speed);
        ship->position.y  = ease(ship->position.y, ship->target_position.y, delta * speed);

        const float close_enough = 0.01f;
        if (SDL_fabsf(ship->position.x - ship->target_position.x) < close_enough &&
            SDL_fabsf(ship->position.y - ship->target_position.y) < close_enough) {
            ship->position = ship->target_position;
        }
    } else {
        ship->fire_cooldown -= delta;
        // only shoot when its not moving?
        if (ship->fire_cooldown < 0.f) {
            Vec2 playerPos = EntityManager_GetPlayerPosition();
            Vec2 aim       = {
                      .x = playerPos.x - ship->position.x,
                      .y = playerPos.y - ship->position.y,
            };
            aim = Vec2_Normalize(aim);

            SmallShip_Shoot(ship->position, aim);
            ship->fire_cooldown = random_getf_between(FIRE_RATE_MIN, FIRE_RATE_MAX);
        }
    }

    ship->position.x += ship->velocity.x * SmallShipSpeed * delta;
    ship->position.y += ship->velocity.y * SmallShipSpeed * delta;
}

void SmallShip_Draw(SmallShip *ship, SDL_Renderer *renderer) {
    Camera *camera     = ResourceManager_GetMainCamera();
    Vec2    screen_pos = Camera_WorldToScreen(camera, ship->position);

    SDL_FRect rect = {
        .x = screen_pos.x,
        .y = screen_pos.y,
        .w = ship->size.x * PIXELS_PER_METER,
        .h = ship->size.y * PIXELS_PER_METER,
    };

    SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0xCC, 0xFF);
    SDL_RenderFillRectF(renderer, &rect);
}
