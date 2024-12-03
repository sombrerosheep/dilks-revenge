#ifndef DREV_LEVELS_H
#define DREV_LEVELS_H

#include "types.h"
#include "wave.h"

typedef struct levels Levels;

enum level_state {
    LevelStateIdle = 0,
    LevelStateCountdown,
    LevelStateBossFight,
    LevelStatePlaying,
    LevelStateCount
};

extern const char *LevelStateLabels[LevelStateCount];

struct levels {
    u16              level_num;
    u16              wave_num;
    f32              start_delay;
    Wave             wave;
    enum level_state state;
};

i32  Levels_Init(Levels *l);
void Levels_Reset(Levels *l);
void Levels_Update(Levels *l, f32 delta);
void Levels_Draw(Levels *l);

#endif // DREV_LEVELS_H
