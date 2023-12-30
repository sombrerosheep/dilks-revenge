#include <SDL_version.h>
#include <stdio.h>

#include "game.h"
#include "system.h"

#define WINDOW_HEIGHT 1200
#define WINDOW_WIDTH  1600
#define GAME_HEIGHT   600
#define GAME_WIDTH    800

SDL_version required_sdl = {.major = 2, .minor = 0, .patch = 22};
const char *name         = "Dilk's Revenge!";

int main() {
    printf("%s\n", name);

    System sys;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_version vers;
    SDL_GetVersion(&vers);
    printf("Using SDL Version: %d.%d.%d\n", vers.major, vers.minor, vers.patch);

    if (!SDL_VERSION_ATLEAST(required_sdl.major, required_sdl.minor, required_sdl.patch)) {
        printf("SDL minimum version not met!\n");
        printf("\twant: %d.%d.%d\n", required_sdl.major, required_sdl.minor, required_sdl.patch);
        printf("\thave: %d.%d.%d\n", vers.major, vers.minor, vers.patch);
        return -1;
    }

    System_Init(&sys, "Dilks Revenge", WINDOW_WIDTH, WINDOW_HEIGHT, GAME_WIDTH, GAME_HEIGHT);
    Game *dr_game = Game_Create(GAME_WIDTH, GAME_HEIGHT);

    Game_Run(dr_game, &sys);

    Game_Destroy(dr_game);
    System_Destroy(&sys);

    return 0;
}
