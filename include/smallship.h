#ifndef DREV_SMALL_SHIP_H
#define DREV_SMALL_SHIP_H

#include "sprite.h"
#include "types.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_small_ship SmallShip;

typedef enum {
    SmallShipType_Light = 0,
    SmallShipType_Heavy,

    SmallShipType_Count
} SmallShipType;

struct drev_small_ship {
    SmallShipType type;
    Sprite        sprite;
    Vec2          target_position;
    Vec2          velocity;
    Vec2          size;
    f32           fire_cooldown;
    u32           health;
};

i8 SmallShip_Init(SmallShip *ship, SmallShipType type, Vec2 position, Vec2 velocity, f32 rotation);
void      SmallShip_MoveTo(SmallShip *ship, Vec2 new_position);
void      SmallShip_Update(SmallShip *ship, f32 delta);
SDL_FRect SmallShip_GetBounds(const SmallShip *ship);
void      SmallShip_Damage(SmallShip *ship, u32 amount);
void      SmallShip_Kill(SmallShip *ship);
void      SmallShip_Draw(SmallShip *ship);

#endif // DREV_SMALL_SHIP_H
