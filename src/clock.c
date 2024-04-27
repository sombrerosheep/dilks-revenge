#include "clock.h"

#include <SDL.h>

int Clock_Init(Clock *clock) {
    clock->start = SDL_GetTicks();

    return 0;
}

Frame Clock_Reset(Clock *clock) {
    Frame        frame;
    unsigned int prev_ticks = clock->start;

    clock->start = SDL_GetTicks();

    frame.milli = clock->start - prev_ticks;
    frame.sec   = frame.milli / 1000.f;

    return frame;
}
