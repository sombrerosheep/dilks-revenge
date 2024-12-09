#include <SDL.h>

#include "args.h"
#include "game.h"
#include "globals.h"
#include "system.h"

SDL_version required_sdl   = {.major = 2, .minor = 0, .patch = 22};
const char *app_title      = "Dilk's Revenge!";
const char *app_path_title = "Dilks Revenge";

int main(int argc, const char **argv) {
    SDL_Log("%s\n", app_title);

    System sys;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_version vers;
    SDL_GetVersion(&vers);
    SDL_Log("Using SDL Version: %d.%d.%d\n", vers.major, vers.minor, vers.patch);

    if (!SDL_VERSION_ATLEAST(required_sdl.major, required_sdl.minor, required_sdl.patch)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "SDL minimum version not met!\n"
                     "\twant: %d.%d.%d\n"
                     "\thave: %d.%d.%d\n",
                     required_sdl.major,
                     required_sdl.minor,
                     required_sdl.patch,
                     vers.major,
                     vers.minor,
                     vers.patch);
        return -1;
    }

    // const char *base_path = SDL_GetBasePath();
    // const char *pref_path = SDL_GetPrefPath("", app_path_title);
    // printf("base path: %s\npref path: %s\n", base_path, pref_path);

    Args appArgs = Args_ParseArgv(argc, argv);
    Args_Put(&appArgs, stdout);

    GameOptions opts;
    GameOptions_FromArgs(&opts, &appArgs);

    SysConfig sys_config = SysConfig_FHD;
    System_Init(&sys, app_title, sys_config);
    Game *dr_game = Game_Create(&sys, &opts);

    Game_Run(dr_game);

    Game_Destroy(dr_game);
    System_Destroy(&sys);

    return 0;
}
