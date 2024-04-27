#include "wave.h"

#include "globals.h"
#include "resources.h"

#define DREV_PI 3.14

#define DEG_TO_RAD(a) ((a) * (DREV_PI / 180.f))
#define RAD_TO_DEG(a) ((a) * (180.f / DREV_PI))

const char *WaveStateLabels[WaveStateCount] = {
    [WaveStateIdle]     = "WaveStateIdle",
    [WaveStateInit]     = "WaveStateInit",
    [WaveStateRunning]  = "WaveStateRunning",
    [WaveStateFinished] = "WaveStateFinished",
};

static void Wave_ClearSmallShips(Wave *wave) {
    for (int i = 0; i < MaxSmallShips; i++) {
        wave->ships[i] = NULL;
    }
}

struct wave_grid {
    Vec2         start;
    Vec2         row_dir;
    Vec2         col_dir;
    float        spacing;
    float        width;
    unsigned int columns;
    unsigned int rows;
};

const unsigned int max_cols_top_bottom = 8;
const unsigned int max_cols_left_right = 5;

// static unsigned int get_grid_columns(CameraFocus direction) {
//     if (direction == CameraFocusTop || direction == CameraFocusBottom) {
//         return max_cols_top_bottom;
//     }

//     return max_cols_left_right;
// }

static struct wave_grid make_wave_grid(Wave w) {
    struct wave_grid grid;

    switch (w.wave_direction) {
        case CameraFocusTop:
            grid.width   = 240.f;
            grid.rows    = 2;
            grid.columns = 10;
            grid.spacing = grid.width / ((float)grid.columns - 1.f);
            grid.start   = (Vec2){.x = -120.f, .y = -100.f};
            grid.col_dir = Vec2_Up;
            grid.row_dir = Vec2_Right;
            break;
        case CameraFocusBottom:
            grid.width   = 240.f;
            grid.rows    = 2;
            grid.columns = 10;
            grid.spacing = grid.width / ((float)grid.columns - 1.f);
            grid.start   = (Vec2){.x = 120.f, .y = 100.f};
            grid.col_dir = Vec2_Down;
            grid.row_dir = Vec2_Right;
            break;
        case CameraFocusLeft:
            grid.width   = 135.f;
            grid.rows    = 2;
            grid.columns = 10;
            grid.spacing = grid.width / ((float)grid.columns - 1.f);
            grid.start   = (Vec2){.x = -165.f, .y = 65.f};
            grid.col_dir = Vec2_Left;
            grid.row_dir = Vec2_Up;
            break;
        case CameraFocusRight:
            grid.width   = 135.f;
            grid.rows    = 2;
            grid.columns = 10;
            grid.spacing = grid.width / ((float)grid.columns - 1.f);
            grid.start   = (Vec2){.x = 165.f, .y = 65.f};
            grid.col_dir = Vec2_Right;
            grid.row_dir = Vec2_Up;
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

Wave Wave_New(CameraFocus direction) {
    Wave w = {
        .state          = WaveStateInit,
        .wave_direction = direction,
        .num_rows       = 2,
    };
    const float row_spacing     = 10.f;
    const float travel_distance = 65.f;

    Wave_ClearSmallShips(&w);

    struct wave_grid grid = make_wave_grid(w);
    // printf("grid for wave %s\n", CameraFocusLabels[w.wave_direction]);
    // printf("\twidth: %.2f\n", grid.width);
    // printf("\tstart: %.2f x %.2f\n", grid.start.x, grid.start.y);
    // printf("\tspacing: %.2f\n", grid.spacing);

    switch (w.wave_direction) {
        case CameraFocusTop: {
            for (unsigned int row = 0; row < grid.rows; row++) {
                for (unsigned int col = 0; col < grid.columns; col++) {
                    Vec2 pos = (Vec2){
                        .x = grid.start.x + (grid.spacing * col),
                        .y = grid.start.y + (row_spacing * row),
                    };
                    Vec2 player_desired_pos = {.x = 0.f, .y = pos.y * -1.f - 60.f};
                    Vec2 start_at           = {.x = pos.x, .y = pos.y - travel_distance};

                    SmallShip ship = SmallShip_Create(start_at, Vec2_Zero, 0.f);
                    SmallShip_MoveTo(&ship, pos);
                    Entities_InsertSmallShip(ship);
                    Entities_MovePlayerTo(player_desired_pos);
                }
            }
            break;
        };
        case CameraFocusBottom: {
            for (unsigned int row = 0; row < grid.rows; row++) {
                for (unsigned int col = 0; col < grid.columns; col++) {
                    Vec2 pos = (Vec2){
                        .x = grid.start.x - (grid.spacing * col),
                        .y = grid.start.y - (row_spacing * row),
                    };
                    Vec2 player_desired_pos = {.x = 0.f, .y = pos.y * -1.f + 60.f};
                    Vec2 start_at           = {.x = pos.x, .y = pos.y + travel_distance};

                    SmallShip ship = SmallShip_Create(start_at, Vec2_Zero, 0.f);
                    SmallShip_MoveTo(&ship, pos);
                    Entities_InsertSmallShip(ship);
                    Entities_MovePlayerTo(player_desired_pos);
                }
            }
            break;
        };
        case CameraFocusLeft: {
            for (unsigned int row = 0; row < grid.rows; row++) {
                for (unsigned int col = 0; col < grid.columns; col++) {
                    Vec2 pos = (Vec2){
                        .x = grid.start.x + (row_spacing * row),
                        .y = grid.start.y - (grid.spacing * col),
                    };
                    Vec2 player_desired_pos = {.x = pos.x * -1.f - 60.f, .y = 0.f};
                    Vec2 start_at           = {.x = pos.x - travel_distance, .y = pos.y};

                    SmallShip ship = SmallShip_Create(start_at, Vec2_Zero, 0.f);
                    SmallShip_MoveTo(&ship, pos);
                    Entities_InsertSmallShip(ship);
                    Entities_MovePlayerTo(player_desired_pos);
                }
            }
            break;
        };
        case CameraFocusRight: {
            for (unsigned int row = 0; row < grid.rows; row++) {
                for (unsigned int col = 0; col < grid.columns; col++) {
                    Vec2 pos = (Vec2){
                        .x = grid.start.x - (row_spacing * row),
                        .y = grid.start.y - (grid.spacing * col),
                    };
                    Vec2 player_desired_pos = {.x = pos.x * -1.f + 60.f, .y = 0.f};
                    Vec2 start_at           = {.x = pos.x + travel_distance, .y = pos.y};

                    SmallShip ship = SmallShip_Create(start_at, Vec2_Zero, 0.f);
                    SmallShip_MoveTo(&ship, pos);
                    Entities_InsertSmallShip(ship);
                    Entities_MovePlayerTo(player_desired_pos);
                }
            }
            break;
        };
        default:
            printf("Wave direction not yet implemented\n");
    }

    return w;
}

void Wave_Start(Wave *wave) {
    // can only transition to start from init
    if (wave->state != WaveStateInit) {
        printf("Can't transition wave from %s to %s\n",
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

void Wave_Clean(Wave *wave) {
    wave->wave_direction = CameraFocusCenter;
    wave->state          = WaveStateIdle;
    wave->num_rows       = 0;

    Entities_ClearSmallShips();

    // Does this need to track pointers or do we expose the container
    // from the enity manager?
    for (int i = 0; i < MaxSmallShips; i++) {
        wave->ships[i] = NULL;
    }
}
