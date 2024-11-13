#ifndef DREV_EVENTS_H
#define DREV_EVENTS_H

#include "types.h"

extern u32 PlayEventId;
extern u32 PauseEventId;
extern u32 UnPauseEventId;
extern u32 QuitToMenuEventId;

void RegisterCustomEvents(void);

#endif // DREV_EVENTS_H
