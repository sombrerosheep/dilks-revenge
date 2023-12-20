#ifndef DREV_SYSTEM
#define DREV_SYSTEM

#include <SDL.h>

typedef struct drev_system System;

struct drev_system {
    SDL_Window   *window;
    SDL_Renderer *renderer;
};

int  System_Init(System *sys,
                 char   *title,
                 int     win_width,
                 int     win_height,
                 int     game_width,
                 int     game_height);
void System_Destroy(System *sys);

#endif // DREV_SYSTEM
