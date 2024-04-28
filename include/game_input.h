#ifndef DREV_CONTROLLER_H
#define DREV_CONTROLLER_H

#include "system.h"
#include "types.h"

typedef struct drev_controller GameInput;

struct drev_controller {
    u32 up;
    u32 down;
    u32 left;
    u32 right;
    u32 space;
    u32 mouse_left;
    f32 mouse_world_x;
    f32 mouse_world_y;
    f32 mouse_screen_x;
    f32 mouse_screen_y;
};

int Controller_Init(GameInput *controller);

void Controller_Update(GameInput *controller, System *sys);

#endif // DREV_CONTROLLER_H
