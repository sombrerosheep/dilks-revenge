#ifndef DREV_CAMERA_H
#define DREV_CAMERA_H

#include "game_input.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_camera Camera;

typedef enum {
    CameraFocusCenter = 0,
    CameraFocusTop,
    CameraFocusBottom,
    CameraFocusRight,
    CameraFocusLeft,

    CameraFocusCount
} CameraFocus;

extern const char *CameraFocusLabels[CameraFocusCount];

struct drev_camera {
    Vec2        position;
    Vec2        target_position;
    Vec2        half_size;
    CameraFocus focus;
    CameraFocus next_focus;
    // for multiple cameras, this may need another Vec2 screen_pos;
};

int       Camera_Init(Camera *camera, float unit_height, float ratio);
void      Camera_Update(Camera *camera, float delta);
void      Camera_SetFocus(Camera *camera, CameraFocus focus);
void      Camera_SetCenter(Camera *camera, Vec2 center);
Vec2      Camera_WorldToScreen(const Camera *cam, Vec2 pos);
Vec2      Camera_WorldToScreenF(const Camera *cam, float x, float y);
Vec2      Camera_ScreenToWorld(const Camera *cam, Vec2 pos);
Vec2      Camera_ScreenToWorldF(const Camera *cam, float x, float y);
SDL_FRect Camera_GetBounds(const Camera *cam);

// all coordinates for draw functions should be in world coordinates.
void Camera_DrawFillRect(const Camera *cam, SDL_FRect rect, SDL_Color color);
void Camera_DrawRect(const Camera *cam, SDL_FRect rect, SDL_Color color);
void Camera_DrawLine(const Camera *cam, float x1, float y1, float x2, float y2, SDL_Color color);

void Camera_Draw(const Camera *camera, SDL_Renderer *renderer);
void Camera_Destroy(Camera *camera);

#endif // DREV_CAMERA_H
