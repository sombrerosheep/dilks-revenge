#ifndef DREV_GLOBALS_H
#define DREV_GLOBALS_H

#include "resources.h"

// Game Settings

// todo: move all of this out to a config
#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH  1920
#define GAME_HEIGHT   1080
#define GAME_WIDTH    1920

#define PIXELS_PER_UNIT  6.4f
#define METERS_PER_UNIT  1.f
#define PIXELS_PER_METER ((METERS_PER_UNIT) * (PIXELS_PER_UNIT)) // 6.4

#define UNITS_HEIGHT (WINDOW_HEIGHT / PIXELS_PER_UNIT) // 168.75u
#define UNITS_WIDTH  (WINDOW_WIDTH / PIXELS_PER_UNIT)  // 300.00u

// Helpers

#define UNUSED(v) (void)(v)

// Maths

#define max(x, y) ((x) > (y) ? (x) : (y))

#endif // DREV_GLOBALS_H
