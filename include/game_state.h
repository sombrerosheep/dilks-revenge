#ifndef DREV_GAME_MEMORY_H
#define DREV_GAME_MEMORY_H

#include "camera.h"
#include "entities.h"
#include "game_input.h"
#include "player.h"
#include "wave.h"

typedef struct drev_game_state GameState;

struct drev_game_state {
    // Resources
    Camera    main_camera;
    GameInput controller;
    Wave      current_wave;

    // Entities
    Player              player;
    ProjectileContainer projectiles;
    SmallShipContainer  smallShips;
};

#endif // DREV_GAME_MEMORY_H
