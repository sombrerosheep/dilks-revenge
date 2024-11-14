#include "events.h"

#include <SDL.h>

u32 PlayEventId;
u32 PauseEventId;
u32 UnPauseEventId;
u32 QuitToMenuEventId;
u32 GameOverEventId;

void RegisterCustomEvents(void) {
    PlayEventId       = SDL_RegisterEvents(1);
    PauseEventId      = SDL_RegisterEvents(1);
    UnPauseEventId    = SDL_RegisterEvents(1);
    QuitToMenuEventId = SDL_RegisterEvents(1);
    GameOverEventId   = SDL_RegisterEvents(1);
}
