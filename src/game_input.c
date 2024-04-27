#include "game_input.h"

#include "camera.h"
#include "resources.h"
#include "vec.h"

#include <SDL.h>

int Controller_Init(GameInput *controller) {
    controller->up         = 0;
    controller->down       = 0;
    controller->left       = 0;
    controller->right      = 0;
    controller->space      = 0;
    controller->mouse_left = 0;

    return 0;
}

void Controller_Update(GameInput *controller, System *sys) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    int          mouse_x, mouse_y, win_w, win_h, render_w, render_h;
    Uint32       mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_GetWindowSize(sys->window, &win_w, &win_h);
    SDL_RenderGetLogicalSize(sys->renderer, &render_w, &render_h);

    controller->up    = keys[SDL_GetScancodeFromKey(SDLK_w)];
    controller->left  = keys[SDL_GetScancodeFromKey(SDLK_a)];
    controller->down  = keys[SDL_GetScancodeFromKey(SDLK_s)];
    controller->right = keys[SDL_GetScancodeFromKey(SDLK_d)];
    controller->space = keys[SDL_GetScancodeFromKey(SDLK_SPACE)];

    controller->mouse_screen_x = (float)mouse_x / (float)win_w * (float)render_w;
    controller->mouse_screen_y = (float)mouse_y / (float)win_h * (float)render_h;
    Vec2 mouse_world           = Camera_ScreenToWorldF(Resources_GetMainCamera(),
                                             controller->mouse_screen_x,
                                             controller->mouse_screen_y);
    controller->mouse_world_x  = mouse_world.x;
    controller->mouse_world_y  = mouse_world.y;
    controller->mouse_left     = mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT);

    return;
}
