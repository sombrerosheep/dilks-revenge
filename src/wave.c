#include "wave.h"

#include "camera.h"
#include "entities.h"
#include "globals.h"
#include "maths.h"
#include "player.h"
#include "random.h"
#include "resources.h"
#include "smallship.h"
#include "util.h"
#include "vec.h"

const u32 grid_rows       = 2;
const u32 grid_columns    = MaxSmallShips / grid_rows;
const f32 row_spacing     = 10.f;
const f32 travel_distance = 65.f;

const f32 grid_w_v = 240.f;
const f32 grid_w_h = 135.f;

const char *WaveStateLabels[WaveStateCount] = {
    [WaveStateIdle]     = "WaveStateIdle",
    [WaveStateInit]     = "WaveStateInit",
    [WaveStateRunning]  = "WaveStateRunning",
    [WaveStateFinished] = "WaveStateFinished",
};

static void Wave_ClearSmallShips(Wave *wave) {
    for (u32 i = 0; i < MaxSmallShips; i++) {
        wave->ships[i] = NULL;
    }
}

struct wave_grid {
    Vec2 start;
    f32  spacing;
    f32  width;
    u32  columns;
    u32  rows;
};

// const u32 max_cols_top_bottom = 8;
// const u32 max_cols_left_right = 5;

// static u32 get_grid_columns(CameraFocus direction) {
//     if (direction == CameraFocusTop || direction == CameraFocusBottom) {
//         return max_cols_top_bottom;
//     }

//     return max_cols_left_right;
// }

// gets the starting point to create the grid from.
// all grid's are built in the Top orientation and then
// have the points rotated from there.
// This means, Top/Bottom have the same starting point
// as does Left/Right.
static Vec2 get_grid_start(CameraFocus dir) {
    switch (dir) {
        case CameraFocusTop:
        case CameraFocusBottom:
            return (Vec2){.x = -(grid_w_v / 2.f), .y = -100.f + row_spacing};
        case CameraFocusRight: // fallthrough
        case CameraFocusLeft:
            return (Vec2){.x = -(grid_w_h / 2.f), .y = -165.f + row_spacing};
        default:
            return Vec2_Zero;
    }
}

static struct wave_grid make_wave_grid(Wave w) {
    struct wave_grid grid;

    switch (w.wave_direction) {
        case CameraFocusTop:
            grid.width   = grid_w_v;
            grid.rows    = grid_rows;
            grid.columns = grid_columns;
            grid.spacing = grid.width / ((f32)grid.columns - 1.f);
            grid.start   = get_grid_start(w.wave_direction);
            break;
        case CameraFocusBottom:
            grid.width   = grid_w_v;
            grid.rows    = grid_rows;
            grid.columns = grid_columns;
            grid.spacing = grid.width / ((f32)grid.columns - 1.f);
            grid.start   = get_grid_start(w.wave_direction);
            break;
        case CameraFocusLeft:
            grid.width   = grid_w_h;
            grid.rows    = grid_rows;
            grid.columns = grid_columns;
            grid.spacing = grid.width / ((f32)grid.columns - 1.f);
            grid.start   = get_grid_start(w.wave_direction);
            break;
        case CameraFocusRight:
            grid.width   = grid_w_h;
            grid.rows    = grid_rows;
            grid.columns = grid_columns;
            grid.spacing = grid.width / ((f32)grid.columns - 1.f);
            grid.start   = get_grid_start(w.wave_direction);
            break;
        default:
            grid.start = Vec2_Zero;
    }

    // printf("making grid: %s...\n\twidth: %.2f\n\trows: %.2d\n\tcolumns: %.2d\n\tspacing: %.2f\n",
    //        CameraFocusLabels[w.wave_direction],
    //        grid.width,
    //        grid.rows,
    //        grid.columns,
    //        grid.spacing);

    return grid;
}

static Vec2 get_position_for_ship(Vec2 origin, f32 col_spacing, f32 row_spacing, i32 col, i32 row) {
    Vec2 pos = (Vec2){
        .x = origin.x + (col_spacing * col),
        .y = origin.y + (row_spacing * row),
    };

    return pos;
}

static void set_grid_ship(Wave *wave, ContainedSmallShip *ship, u32 x, u32 y, u32 stride) {
    u32 index          = x + (y * stride);
    wave->ships[index] = ship;

    return;
}

// rotate a given point around the origin, based on the wave direction
// since this works in 90 deg angles only, cheating by transposing the coordinates
// instead of doing an expenstive rotation.
// assumes you are already in "top" positioning.
static Vec2 transpose_for_direction(Vec2 v, CameraFocus dir) {
    switch (dir) {
        case CameraFocusBottom: {
            Vec2 rotated = (Vec2){
                .x = -v.x,
                .y = -v.y,
            };
            return rotated;
        }
        case CameraFocusRight: {
            Vec2 rotated = (Vec2){
                .x = -v.y,
                .y = v.x,
            };
            return rotated;
        }
        case CameraFocusLeft: {
            Vec2 rotated = (Vec2){
                .x = v.y,
                .y = -v.x,
            };
            return rotated;
        }
        case CameraFocusTop:    // fallthrough
        case CameraFocusCenter: // fallthrough
        default:
            return v;
            break;
    }
}

// @brief returns the players starting position for the wave.
// centered and set back from the incoming wave
static Vec2 get_player_wave_start_pos(CameraFocus dir) {
    const f32 player_offset_h = 60.f;
    const f32 player_offset_v = 32.f;

    switch (dir) {
        case CameraFocusTop: {
            return (Vec2){
                .x = 0.f,
                .y = player_offset_v,
            };
        }
        case CameraFocusBottom: {
            return (Vec2){
                .x = 0.f,
                .y = -player_offset_v,
            };
        }
        case CameraFocusRight: {
            return (Vec2){
                .x = -player_offset_h,
                .y = 0.f,
            };
        }

        case CameraFocusLeft: {
            return (Vec2){
                .x = player_offset_h,
                .y = 0.f,
            };
        }
        default:
            return Vec2_Zero;
    }
}

Wave Wave_New(CameraFocus direction) {
    Wave w = {
        .state          = WaveStateInit,
        .wave_direction = direction,
    };

    Wave_ClearSmallShips(&w);

    struct wave_grid grid = make_wave_grid(w);
    // printf("grid for wave %s\n", CameraFocusLabels[w.wave_direction]);
    // printf("\twidth: %.2f\n", grid.width);
    // printf("\tstart: %.2f x %.2f\n", grid.start.x, grid.start.y);
    // printf("\tspacing: %.2f\n", grid.spacing);

    for (u32 row = 0; row < grid.rows; row++) {
        for (u32 col = 0; col < grid.columns; col++) {
            Vec2 pos      = get_position_for_ship(grid.start, grid.spacing, -row_spacing, col, row);
            Vec2 start_at = {.x = pos.x, .y = pos.y - travel_distance};

            pos      = transpose_for_direction(pos, w.wave_direction);
            start_at = transpose_for_direction(start_at, w.wave_direction);

            SmallShipType type = random_get_between(SmallShipType_Light, SmallShipType_Heavy);
            SmallShip     ship;

            SmallShip_Init(&ship, type, start_at, Vec2_Zero, 0.f);
            SmallShip_MoveTo(&ship, pos);
            ContainedSmallShip *con_ship = Entities_InsertSmallShip(ship);
            set_grid_ship(&w, con_ship, col, row, grid.columns);
        }
    }

    Vec2 player_pos = get_player_wave_start_pos(w.wave_direction);
    Entities_MovePlayerTo(player_pos);

    return w;
}

void Wave_Start(Wave *wave) {
    // can only transition to start from init
    if (wave->state != WaveStateInit) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "Can't transition wave from %s to %s\n",
                    WaveStateLabels[wave->state],
                    WaveStateLabels[WaveStateRunning]);
        return;
    }

    Camera *camera = Resources_GetMainCamera();
    Camera_SetFocus(camera, wave->wave_direction);
    wave->state = WaveStateRunning;
}

void Wave_Update(Wave *wave) {
    UNUSED(wave);
}

void Wave_Draw(Wave *wave) {
    Camera *cam = Resources_GetMainCamera();
    Vec2    pos = get_grid_start(wave->wave_direction);
    pos         = transpose_for_direction(pos, wave->wave_direction);
    Camera_DrawPlus(cam, pos, ColorCyan);
}

void Wave_Clean(Wave *wave) {
    wave->wave_direction = CameraFocusCenter;
    wave->state          = WaveStateIdle;

    Entities_ClearSmallShips();

    // Does this need to track pointers or do we expose the container
    // from the enity manager?
    for (u32 i = 0; i < MaxSmallShips; i++) {
        wave->ships[i] = NULL;
    }
}
