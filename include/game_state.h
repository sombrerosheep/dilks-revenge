#ifndef DREV_GAME_MEMORY_H
#define DREV_GAME_MEMORY_H

#include "camera.h"
#include "entities.h"
#include "font.h"
#include "game_input.h"
#include "game_mode.h"
#include "levels.h"
#include "player.h"
#include "system.h"
#include "ui.h"
#include "wave.h"

typedef struct drev_game_state GameState;

struct drev_game_state {
    // Resources
    Camera    main_camera;
    GameInput controller;
    Levels    levels;
    Font      title_font;
    Font      menu_font;
    Font      debug_font;

    // GameMode and UI
    enum GameMode mode;
    UI            main_menu;
    UI            pause_menu;
    UI            game_over_menu;
    Camera        ui_camera;

    // Entities
    Player              player;
    ProjectileContainer projectiles;
    SmallShipContainer  smallShips;
};

#endif // DREV_GAME_MEMORY_H
