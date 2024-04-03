#ifndef DREV_WAVE_H
#define DREV_WAVE_H

#include "camera.h"
#include "entities.h"
#include "smallship.h"

typedef struct drev_wave Wave;

typedef enum {
    WaveStateIdle = 0,
    WaveStateInit,
    WaveStateRunning,
    WaveStateFinished,

    WaveStateCount
} WaveState;

struct drev_wave {
    WaveState    state;
    CameraFocus  wave_direction;
    unsigned int num_rows;
    SmallShip   *ships[MaxSmallShips];
};

Wave Wave_New(CameraFocus direction);
void Wave_Start(Wave *wave);
void Wave_Update(Wave *wave);

void Wave_Clean(Wave *wave);

#endif // DREV_WAVE_H
