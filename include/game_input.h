#ifndef DREV_CONTROLLER_H
#define DREV_CONTROLLER_H

#include "system.h"
#include "types.h"

typedef u8                     KeyState;
typedef struct drev_controller GameInput;

struct drev_controller {
    KeyState up;
    KeyState down;
    KeyState left;
    KeyState right;
    KeyState fire;
    KeyState pause;
    u8       mouse_left;
    f32      mouse_screen_x;
    f32      mouse_screen_y;
};

int Controller_Init(GameInput *controller);

bool Controller_Is(KeyState key);
bool Controller_Was(KeyState key);
bool Controller_JustPressed(KeyState key);

void Controller_Update(GameInput *controller, System *sys);

#endif // DREV_CONTROLLER_H
