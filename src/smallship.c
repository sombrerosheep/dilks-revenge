#include "smallship.h"

#include "assets.h"
#include "camera.h"
#include "entities.h"
#include "maths.h"
#include "projectile.h"
#include "random.h"
#include "resources.h"
#include "util.h"
#include "vec.h"

struct drev_smallshipconfig {
    u32       inititial_health;
    float     fire_rate_min;
    float     fire_rate_max;
    float     size;
    float     ship_speed;
    float     projectile_speed;
    u32       projectile_damage;
    float     projectile_size;
    SDL_Color color;
};

static struct drev_smallshipconfig configs[SmallShipType_Count] = {
    [SmallShipType_Light] =
        {
            .inititial_health  = 50u,
            .fire_rate_min     = 1.5f,
            .fire_rate_max     = 5.f,
            .size              = 6.f,
            .ship_speed        = 125.f,
            .projectile_speed  = 150.f,
            .projectile_damage = 5u,
            .projectile_size   = 1.f,
        },
    [SmallShipType_Heavy] =
        {
            .inititial_health  = 75u,
            .fire_rate_min     = 3.f,
            .fire_rate_max     = 6.f,
            .size              = 6.f,
            .ship_speed        = 125.f,
            .projectile_speed  = 75.f,
            .projectile_damage = 20u,
            .projectile_size   = 3.f,
        },
};

#define SmallShipTravelSpeed 125.f
#define SmallShipAimErrorDeg 15.f

i8 SmallShip_Init(SmallShip *ship, SmallShipType type, Vec2 position, Vec2 velocity, f32 rotation) {
    struct drev_smallshipconfig config = configs[type];

    ship->type            = type;
    ship->target_position = position;
    ship->velocity        = Vec2_Normalize(velocity);
    ship->fire_cooldown   = random_getf_between(config.fire_rate_min, config.fire_rate_max);
    ship->size            = Vec2_Newf(config.size);
    ship->health          = config.inititial_health;

    TextureID shipTextureId =
        ship->type == SmallShipType_Light ? TextureIDLightShip : TextureIDHeavyShip;

    Sprite_Init(&ship->sprite, Assets_GetTexture(shipTextureId));
    Sprite_SetPosition(&ship->sprite, position);
    Sprite_SetRotation(&ship->sprite, rotation);

    return 0;
}

void SmallShip_MoveTo(SmallShip *ship, Vec2 new_position) {
    ship->target_position = new_position;
}

static void SmallShip_Shoot(Vec2 pos, Vec2 vel, float speed, u32 strength, float sz) {
    Projectile p;
    Projectile_Init(&p, ProjectileType_Enemy, pos, vel, speed, strength, sz);
    Entities_AddProjectile(p);
}

void SmallShip_Update(SmallShip *ship, f32 delta) {
    struct drev_smallshipconfig config = configs[ship->type];
    if (Vec2_Equal(ship->sprite.pos, ship->target_position) == 0) {
        ship->sprite.pos.x =
            ease(ship->sprite.pos.x, ship->target_position.x, delta * SmallShipTravelSpeed);
        ship->sprite.pos.y =
            ease(ship->sprite.pos.y, ship->target_position.y, delta * SmallShipTravelSpeed);

        const f32 close_enough = 0.01f;
        if (SDL_fabsf(ship->sprite.pos.x - ship->target_position.x) < close_enough &&
            SDL_fabsf(ship->sprite.pos.y - ship->target_position.y) < close_enough) {
            ship->sprite.pos = ship->target_position;
        }
    } else {
        ship->fire_cooldown -= delta;

        if (ship->fire_cooldown < 0.f) {
            Vec2 playerPos = Entities_GetPlayerPosition();
            Vec2 aim       = {
                      .x = playerPos.x - ship->sprite.pos.x,
                      .y = playerPos.y - ship->sprite.pos.y,
            };

            f32 err = random_getf_between(-SmallShipAimErrorDeg, SmallShipAimErrorDeg);
            aim     = Vec2_Normalize(aim);
            aim     = Vec2_Rotate(aim, err);

            SmallShip_Shoot(ship->sprite.pos,
                            aim,
                            config.projectile_speed,
                            config.projectile_damage,
                            config.projectile_size);
            ship->fire_cooldown = random_getf_between(config.fire_rate_min, config.fire_rate_max);
        }
    }

    ship->sprite.pos.x += ship->velocity.x * config.ship_speed * delta;
    ship->sprite.pos.y += ship->velocity.y * config.ship_speed * delta;
}

SDL_FRect SmallShip_GetBounds(const SmallShip *ship) {
    SDL_FRect rect = {
        .x = ship->sprite.pos.x,
        .y = ship->sprite.pos.y,
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
    Camera *camera = Resources_GetMainCamera();

    Sprite_Draw(&ship->sprite);

#ifdef DREV_DRAW_BB
    Camera_DrawRect(camera, SmallShip_GetBounds(ship), ColorRed);
#endif
}
