#ifndef DREV_CLOCK_H
#define DREV_CLOCK_H

typedef struct drev_clock Clock;
typedef struct drev_frame Frame;

struct drev_frame {
  unsigned int milli;
  float sec;
};

Clock *Clock_Create(void);

Frame Clock_Reset(Clock *clock);

#endif // DREV_CLOCK_H
