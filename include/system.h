#ifndef DREV_SYSTEM
#define DREV_SYSTEM

#include "types.h"

#include <SDL.h>

typedef struct drev_system    System;
typedef struct drev_sysconfig SysConfig;

struct drev_sysconfig {
    unsigned int window_height;
    unsigned int window_width;
    unsigned int render_height;
    unsigned int render_width;
    f32          ppu;
};

extern SysConfig SysConfig_HD;
extern SysConfig SysConfig_FHD;
extern SysConfig SysConfig_QHD;
extern SysConfig SysConfig_4K;
extern SysConfig SysConfig_8k;

struct drev_system {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SysConfig     config;
};

i8   System_Init(System *sys, char *title, SysConfig config);
void System_Destroy(System *sys);

#endif // DREV_SYSTEM
