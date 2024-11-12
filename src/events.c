#include "events.h"

#include <SDL.h>

u32 PlayEventId;

void RegisterCustomEvents(void) {
    PlayEventId = SDL_RegisterEvents(1);
}
