#include "smallship.h"

#include "camera.h"
#include "entities.h"
#include "projectile.h"
#include "random.h"
#include "resources.h"
#include "util.h"

#define FIRE_RATE_MIN   1.f
#define FIRE_RATE_MAX   3.5f
#define SHIP_SIZE_UNITS 6.f

#define ENEMY_PROJECTILE_SPEED    150.f
#define SmallShipSpeed            125.f
#define SmallShipProjectileDamage 5u
#define SmallShipInitHealth       50u

i8 SmallShip_Init(SmallShip *ship) {
    ship->position        = Vec2_Zero;
    ship->target_position = Vec2_Zero;
    ship->size            = Vec2_Newf(SHIP_SIZE_UNITS);
    ship->velocity        = Vec2_Zero;
    ship->rotation        = 0.f;
    ship->fire_cooldown   = random_getf_between(FIRE_RATE_MIN, FIRE_RATE_MAX);

    return 0;
}

SmallShip SmallShip_Create(Vec2 position, Vec2 velocity, f32 rotation) {
    SmallShip ship;

    SmallShip_Init(&ship);

    ship.position        = position;
    ship.target_position = position;
    ship.velocity        = Vec2_Normalize(velocity);
    ship.rotation        = rotation;
    ship.health          = SmallShipInitHealth;

    return ship;
}

void SmallShip_MoveTo(SmallShip *ship, Vec2 new_position) {
    ship->target_position = new_position;
}

static void SmallShip_Shoot(Vec2 pos, Vec2 vel) {
    Projectile p;
    Projectile_Init(&p,
                    ProjectileType_Enemy,
                    pos,
                    vel,
                    ENEMY_PROJECTILE_SPEED,
                    SmallShipProjectileDamage);
    Entities_AddProjectile(p);
}

void SmallShip_Update(SmallShip *ship, f32 delta) {
    if (Vec2_Equal(ship->position, ship->target_position) == 0) {
        f32 speed        = SmallShipSpeed;
        ship->position.x = ease(ship->position.x, ship->target_position.x, delta * speed);
        ship->position.y = ease(ship->position.y, ship->target_position.y, delta * speed);

        const f32 close_enough = 0.01f;
        if (SDL_fabsf(ship->position.x - ship->target_position.x) < close_enough &&
            SDL_fabsf(ship->position.y - ship->target_position.y) < close_enough) {
            ship->position = ship->target_position;
        }
    } else {
        ship->fire_cooldown -= delta;

        if (ship->fire_cooldown < 0.f) {
            Vec2 playerPos = Entities_GetPlayerPosition();
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

SDL_FRect SmallShip_GetBounds(const SmallShip *ship) {
    SDL_FRect rect = {
        .x = ship->position.x,
        .y = ship->position.y,
        .w = ship->size.x,
        .h = ship->size.y,
    };

    return rect;
}

void SmallShip_Damage(SmallShip *ship, u32 amount) {
    if (ship->health < amount) {
        ship->health = 0;
    } else {
        ship->health -= amount;
    }
}

void SmallShip_Kill(SmallShip *ship) {
    ship->health = 0;
}

void SmallShip_Draw(SmallShip *ship) {
    Camera   *camera           = Resources_GetMainCamera();
    SDL_Color small_ship_color = {.r = 0x11, .g = 0x11, .b = 0xCC, .a = 0xAA};

    SDL_FRect rect = {
        .x = ship->position.x,
        .y = ship->position.y,
        .w = ship->size.x,
        .h = ship->size.y,
    };

    Camera_DrawFillRect(camera, rect, small_ship_color);

#ifdef DREV_DRAW_BB
    Camera_DrawRect(camera, SmallShip_GetBounds(ship), ColorRed);
#endif
}
