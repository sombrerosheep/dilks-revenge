#include <controller.h>

#include <SDL.h>

int Controller_Init(Controller *controller) {
  controller->up = 0;
  controller->down = 0;
  controller->left = 0;
  controller->right = 0;

  return 0;
}

void Controller_Update(Controller *controller) {
  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  controller->up = keys[SDL_GetScancodeFromKey(SDLK_w)];
  controller->left = keys[SDL_GetScancodeFromKey(SDLK_a)];
  controller->down = keys[SDL_GetScancodeFromKey(SDLK_s)];
  controller->right = keys[SDL_GetScancodeFromKey(SDLK_d)];

  return;
}