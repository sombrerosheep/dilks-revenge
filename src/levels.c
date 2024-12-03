#include "levels.h"
#include "camera.h"
#include "globals.h"
#include "random.h"
#include "wave.h"

const f32 wave_countown_s = 3.f;

const char *LevelStateLabels[LevelStateCount] = {
    [LevelStateIdle]      = "Idle",
    [LevelStateCountdown] = "Countdown",
    [LevelStateBossFight] = "BossFight",
    [LevelStatePlaying]   = "Playing",
};

static void Levels_Start(Levels *l) {
    if (l->wave_num > 0 && l->wave_num % 10 == 0) {
        l->wave_num = 0;
        l->level_num++;
    }

    l->wave_num++;
    printf("Starting level: %d::%d...\n", l->level_num, l->wave_num);
    CameraFocus direction = (CameraFocus)random_get_between(CameraFocusTop, CameraFocusLeft);
    l->wave               = Wave_New(direction);
    printf("Starting wave: %s...\n", CameraFocusLabels[direction]);
    Wave_Start(&l->wave);
}

i32 Levels_Init(Levels *l) {
    Levels_Reset(l);

    return 0;
}

void Levels_Reset(Levels *l) {
    l->level_num = 1;
    l->wave_num  = 0;
    l->state     = LevelStateIdle;
    Wave_Clean(&l->wave);
}

void Levels_Update(Levels *l, f32 delta) {
    switch (l->state) {
        case LevelStateIdle: {
            // printf("wave idling...\n");
            l->state       = LevelStateCountdown;
            l->start_delay = wave_countown_s;
            break;
        }
        case LevelStateCountdown: {
            // printf("wave countdown...%.2f\n", l->start_delay);
            l->start_delay -= delta;
            if (l->start_delay < 0.f) {
                l->state = LevelStatePlaying;
                Levels_Start(l);
            }
            break;
        }
        case LevelStatePlaying: {
            Wave_Update(&l->wave);
            // how to communicate when the wave ends and a new one needs to begin?
            if (l->wave.state == WaveStateFinished) {
                l->state = LevelStateIdle;
            }
            break;
        }
        case LevelStateBossFight:
        default:
            printf("Unsupported Level State: %s\n", LevelStateLabels[l->state]);
            break;
    }
}

void Levels_Draw(Levels *l) {
    Wave_Draw(&l->wave);
}
