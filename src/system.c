#include "system.h"

SysConfig SysConfig_HD = {
    .window_height = 720,
    .window_width  = 1280,
    .render_height = 720,
    .render_width  = 1280,
    .ppu           = 4.27, // hmm
};
SysConfig SysConfig_FHD = {
    .window_height = 1080,
    .window_width  = 1920,
    .render_height = 1080,
    .render_width  = 1920,
    .ppu           = 6.4f,
};
SysConfig SysConfig_QHD = {
    .window_height = 1440,
    .window_width  = 2560,
    .render_height = 1440,
    .render_width  = 2560,
    .ppu           = 8.53, // hmm
};
SysConfig SysConfig_4K = {
    .window_height = 2160,
    .window_width  = 3840,
    .render_height = 2160,
    .render_width  = 3840,
    .ppu           = 12.8,
};
SysConfig SysConfig_8K = {
    .window_height = 4320,
    .window_width  = 7680,
    .render_height = 4320,
    .render_width  = 7680,
    .ppu           = 25.6,
};

i8 System_Init(System *sys, const char *title, SysConfig config) {
    if ((sys->window = SDL_CreateWindow(title,
                                        100,
                                        50,
                                        config.window_width,
                                        config.window_height,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI)) == NULL) {
        SDL_Log("ERROR :: Unable to allocate memory for window. SDL Error: %s\n", SDL_GetError());
        System_Destroy(sys);
        return -1;
    }

    if ((sys->renderer = SDL_CreateRenderer(sys->window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
        SDL_Log("ERROR :: Unable to allocate memory for renderer. SDL Error: %s\n", SDL_GetError());
        System_Destroy(sys);
        return -1;
    }

    SDL_SetRenderDrawBlendMode(sys->renderer, SDL_BLENDMODE_BLEND);

    sys->config = config;

    SDL_RenderSetLogicalSize(sys->renderer, config.render_width, config.render_height);

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
