#include <SDL.h>
#include <stdio.h>

#include "game.h"
#include "globals.h"
#include "system.h"

SDL_version required_sdl = {.major = 2, .minor = 0, .patch = 22};
const char *name         = "Dilk's Revenge!";

int main(void) {
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

    SysConfig sys_config = SysConfig_FHD;
    System_Init(&sys, "Dilks Revenge", sys_config);
    Game *dr_game = Game_Create(&sys, sys_config);

    Game_Run(dr_game);

    Game_Destroy(dr_game);
    System_Destroy(&sys);

    return 0;
}
