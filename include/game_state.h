#ifndef DREV_GAME_MEMORY_H
#define DREV_GAME_MEMORY_H

#include "camera.h"
#include "game_input.h"
#include "player.h"

typedef struct drev_game_state GameState;

struct drev_game_state {
    Camera    main_camera;
    GameInput controller;
    Player    player;
};

#endif // DREV_GAME_MEMORY_H
