#include "SDL_render.h"
#include "SDL_video.h"
#include <system.h>

#include <stdio.h>

int System_Init(
    System *sys,
    char   *title,
    int     win_width,
    int     win_height,
    int     game_width,
    int     game_height
) {
    if ((sys->window = SDL_CreateWindow(
             title,
             100,
             50,
             win_width,
             win_height,
             SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
         )) == NULL) {
        printf("ERROR :: Unable to allocate memory for window. SDL Error: %s\n", SDL_GetError());
        System_Destroy(sys);
        return -1;
    }

    if ((sys->renderer = SDL_CreateRenderer(sys->window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
        printf("ERROR :: Unable to allocate memory for renderer. SDL Error: %s\n", SDL_GetError());
        System_Destroy(sys);
        return -1;
    }

    SDL_RenderSetLogicalSize(sys->renderer, game_width, game_height);

    return 0;
}

void System_Destroy(System *sys) {
    if (sys->renderer != NULL) {
        SDL_DestroyRenderer(sys->renderer);
    }

    if (sys->window != NULL) {
        SDL_DestroyWindow(sys->window);
    }
}
