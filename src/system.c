#include <system.h>

#include <stdio.h>

int System_Init(System *sys, char *title, int win_height, int win_width) {
    if ((sys->window =
             SDL_CreateWindow(title, 100, 50, win_height, win_width, SDL_WINDOW_OPENGL)) == NULL) {
        printf("ERROR :: Unable to allocate memory for window. SDL Error: %s\n", SDL_GetError());
        System_Destroy(sys);
        return -1;
    }

    if ((sys->renderer = SDL_CreateRenderer(sys->window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
        printf("ERROR :: Unable to allocate memory for renderer. SDL Error: %s\n", SDL_GetError());
        System_Destroy(sys);
        return -1;
    }

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