#include <clock.h>

#include <SDL.h>
#include <stdio.h>

struct drev_clock {
    unsigned int start;
};

Clock *Clock_Create() {
    Clock *clock = NULL;

    if ((clock = malloc(sizeof(Clock))) == NULL) {
        printf("ERROR :: error allocating memory for clock\n");
        return NULL;
    }

    clock->start = SDL_GetTicks();

    return clock;
}

Frame Clock_Reset(Clock *clock) {
    Frame        frame;
    unsigned int prev_ticks = clock->start;

    clock->start = SDL_GetTicks();

    frame.milli = clock->start - prev_ticks;
    frame.sec   = frame.milli / 1000.f;

    return frame;
}