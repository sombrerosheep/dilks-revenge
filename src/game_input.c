#include "game_input.h"

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
    int          mouse_x, mouse_y, ww, wh, rw, rh;
    Uint32       mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_GetWindowSize(sys->window, &ww, &wh);
    SDL_RenderGetLogicalSize(sys->renderer, &rw, &rh);

    controller->up    = keys[SDL_GetScancodeFromKey(SDLK_w)];
    controller->left  = keys[SDL_GetScancodeFromKey(SDLK_a)];
    controller->down  = keys[SDL_GetScancodeFromKey(SDLK_s)];
    controller->right = keys[SDL_GetScancodeFromKey(SDLK_d)];
    controller->space = keys[SDL_GetScancodeFromKey(SDLK_SPACE)];

    controller->mouse_x    = (float)mouse_x / (float)ww * (float)rw;
    controller->mouse_y    = (float)mouse_y / (float)wh * (float)rh;
    controller->mouse_left = mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT);

    return;
}
