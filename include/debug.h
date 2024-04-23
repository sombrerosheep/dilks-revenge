#ifndef DREV_DEBUG_H
#define DREV_DEBUG_H

#include "vec.h"

#include <SDL.h>

struct debug_info {
    Vec2 camera_pos;
    Vec2 camera_screen_pos;

    Vec2 player_pos;
    Vec2 player_screen_pos;

    SDL_FRect cam_unit_bounds;
};

// void Debug_Draw(struct debug_info *info);
void Debug_Draw(void);

#endif // DREV_DEBUG_H
