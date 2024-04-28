#ifndef DREV_CLOCK_H
#define DREV_CLOCK_H

#include "types.h"

typedef struct drev_clock Clock;
typedef struct drev_frame Frame;

struct drev_frame {
    u32 milli;
    f32 sec;
};

struct drev_clock {
    u32 start;
};

int Clock_Init(Clock *clock);

Frame Clock_Reset(Clock *clock);

#endif // DREV_CLOCK_H
