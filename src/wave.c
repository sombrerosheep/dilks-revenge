#include "wave.h"
#include "camera.h"
#include "entities.h"
#include "globals.h"
#include "resources.h"
#include "smallship.h"
#include "vec.h"

#define DREV_PI 3.14

#define DEG_TO_RAD(a) ((a) * (DREV_PI / 180.f))
#define RAD_TO_DEG(a) ((a) * (180.f / DREV_PI))

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

static unsigned int get_grid_columns(CameraFocus direction) {
    if (direction == CameraFocusTop || direction == CameraFocusBottom) {
        return max_cols_top_bottom;
    }

    return max_cols_left_right;
}

static struct wave_grid make_wave_grid(Wave w) {
    struct wave_grid grid;
    float            padding_percent = 0.1;
    Camera          *camera          = ResourceManager_GetMainCamera();
    SDL_FRect        cam_box         = Camera_GetBounds(camera);

    unsigned int dir_width =
        (w.wave_direction == CameraFocusTop || w.wave_direction == CameraFocusBottom) ? cam_box.w
                                                                                      : cam_box.h;
    float padding = (float)dir_width * padding_percent;

    grid.width   = (float)dir_width - (padding * 2.f);
    grid.rows    = 2;
    grid.columns = get_grid_columns(w.wave_direction);
    grid.spacing = grid.width / (float)(grid.columns - 1);

    switch (w.wave_direction) {
        case CameraFocusTop:
            grid.start   = (Vec2){.x = -(grid.width / 2.f), .y = -(cam_box.h / 2.f - 10.f)};
            grid.row_dir = Vec2_Right;
            grid.col_dir = Vec2_Up;
            break;
        case CameraFocusBottom:
            grid.start   = (Vec2){.x = (grid.width / 2.f), .y = (cam_box.h / 2.f) - 60.f};
            grid.row_dir = Vec2_Right;
            grid.col_dir = Vec2_Down;
            break;
        case CameraFocusLeft:
            grid.start   = (Vec2){.x = -(cam_box.w / 2.f) + 10.f, .y = (grid.width / 2.f)};
            grid.row_dir = Vec2_Up;
            grid.col_dir = Vec2_Left;
            break;
        case CameraFocusRight:
            grid.start   = (Vec2){.x = (cam_box.w / 2.f) - 60.f, .y = (grid.width / 2.f)};
            grid.row_dir = Vec2_Up;
            grid.col_dir = Vec2_Right;
            break;
        default:
            grid.start = Vec2_Zero;
    }

    return grid;
}

Wave Wave_New(CameraFocus direction) {
    Wave w = {
        .state          = WaveStateInit,
        .wave_direction = direction,
        .num_rows       = 2,
    };
    const float row_spacing = 100.f;

    Wave_ClearSmallShips(&w);

    struct wave_grid grid = make_wave_grid(w);

    switch (w.wave_direction) {
        case CameraFocusTop: {
            for (unsigned int row = 0; row < grid.rows; row++) {
                for (unsigned int col = 0; col < grid.columns; col++) {
                    Vec2 pos = (Vec2){
                        .x = grid.start.x + (grid.spacing * col),
                        .y = grid.start.y + (row_spacing * row),
                    };

                    SmallShip ship = SmallShip_Create(pos, Vec2_Zero, 0.f);
                    EntityManager_InsertSmallShip(ship);
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

                    SmallShip ship = SmallShip_Create(pos, Vec2_Zero, 0.f);
                    EntityManager_InsertSmallShip(ship);
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

                    SmallShip ship = SmallShip_Create(pos, Vec2_Zero, 0.f);
                    EntityManager_InsertSmallShip(ship);
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

                    SmallShip ship = SmallShip_Create(pos, Vec2_Zero, 0.f);
                    EntityManager_InsertSmallShip(ship);
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
        printf("can't transition from current state to Running\n");
        return;
    }

    Camera *camera = ResourceManager_GetMainCamera();
    printf("Wave_Start: Setting camera forucs\n");
    Camera_SetFocus(camera, wave->wave_direction);
    wave->state = WaveStateRunning;
}

void Wave_Update(Wave *wave) {
    UNUSED(wave);

    // check each small ships position
    // if they've reached their limit
    // set velocities to Vec2_Zero
}

void Wave_Clean(Wave *wave) {
    wave->wave_direction = CameraFocusCenter;
    wave->state          = WaveStateIdle;
    wave->num_rows       = 0;

    EntityManager_ClearSmallShips();

    for (int i = 0; i < MaxSmallShips; i++) {
        wave->ships[i] = NULL;
    }
}
