#ifndef DREV_SMALL_SHIP_H
#define DREV_SMALL_SHIP_H

#include "vec.h"

#include <SDL.h>

typedef struct drev_small_ship SmallShip;

struct drev_small_ship {
    Vec2  position;
    Vec2  target_position;
    Vec2  velocity;
    Vec2  size;
    float rotation;
    float fire_cooldown;
};

int       SmallShip_Init(SmallShip *ship);
SmallShip SmallShip_Create(Vec2 position, Vec2 velocity, float rotation);
void      SmallShip_MoveTo(SmallShip *ship, Vec2 new_position);
void      SmallShip_Update(SmallShip *ship, float delta);
SDL_FRect SmallShip_GetBounds(const SmallShip *ship);
void      SmallShip_Draw(SmallShip *ship);

#endif // DREV_SMALL_SHIP_H
