#include "game_input.h"

#include "camera.h"
#include "resources.h"
#include "vec.h"

#include <SDL.h>

static u8 is  = 0x01;
static u8 was = 0x10;

int Controller_Init(GameInput *controller) {
    controller->up             = 0;
    controller->down           = 0;
    controller->left           = 0;
    controller->right          = 0;
    controller->space          = 0;
    controller->mouse_left     = 0;
    controller->mouse_screen_x = 0.f;
    controller->mouse_screen_y = 0.f;
    controller->mouse_world_x  = 0.f;
    controller->mouse_screen_y = 0.f;

    return 0;
}

bool Controller_Is(KeyState key) {
    return key & is;
}

bool Controller_Was(KeyState key) {
    return key & was;
}

bool Controller_JustPressed(KeyState key) {
    return Controller_Is(key) && !Controller_Was(key);
}

void Controller_Update(GameInput *controller, System *sys) {
    const u8 *keys = SDL_GetKeyboardState(NULL);
    i32       mouse_x, mouse_y, win_w, win_h, render_w, render_h;
    u32       mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_GetWindowSize(sys->window, &win_w, &win_h);
    SDL_RenderGetLogicalSize(sys->renderer, &render_w, &render_h);

    // shift the previous state to the was bits
    controller->up <<= 4;
    controller->down <<= 4;
    controller->left <<= 4;
    controller->right <<= 4;
    controller->space <<= 4;

    // set the is bits if pressed 
    if (keys[SDL_GetScancodeFromKey(SDLK_w)]) {
        controller->up |= is;
    }

    if (keys[SDL_GetScancodeFromKey(SDLK_a)]) {
        controller->left |= is;
    }

    if (keys[SDL_GetScancodeFromKey(SDLK_s)]) {
        controller->down |= is;
    }

    if (keys[SDL_GetScancodeFromKey(SDLK_d)]) {
        controller->right |= is;
    }

    if (keys[SDL_GetScancodeFromKey(SDLK_SPACE)]) {
        controller->space |= is;
    }

    controller->mouse_screen_x = (f32)mouse_x / (f32)win_w * (f32)render_w;
    controller->mouse_screen_y = (f32)mouse_y / (f32)win_h * (f32)render_h;
    Vec2 mouse_world           = Camera_ScreenToWorldF(Resources_GetMainCamera(),
                                             controller->mouse_screen_x,
                                             controller->mouse_screen_y);
    controller->mouse_world_x  = mouse_world.x;
    controller->mouse_world_y  = mouse_world.y;
    controller->mouse_left     = mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT);

    return;
}
