#ifndef DREV_CLOCK_H
#define DREV_CLOCK_H

typedef struct _clock Clock;
typedef struct _frame Frame;

struct _frame {
  unsigned int milli;
  float sec;
};

Clock *Clock_Create();

Frame Clock_Reset(Clock *clock);

#endif // DREV_CLOCK_H