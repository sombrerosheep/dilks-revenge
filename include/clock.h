#ifndef DREV_CLOCK_H
#define DREV_CLOCK_H

typedef struct drev_clock Clock;
typedef struct drev_frame Frame;

struct drev_frame {
    unsigned int milli;
    float        sec;
};

struct drev_clock {
    unsigned int start;
};

int Clock_Init(Clock *clock);

Frame Clock_Reset(Clock *clock);

#endif // DREV_CLOCK_H
