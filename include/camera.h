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
} CameraFocus;

struct drev_camera {
    Vec2        position;
    Vec2        target_position;
    Vec2        half_size;
    CameraFocus focus;
    CameraFocus next_focus;
};

int  Camera_Init(Camera *camera, Vec2 size);
void Camera_Update(Camera *camera, float delta);
void Camera_SetFocus(Camera *camera, CameraFocus focus);
void Camera_SetCenter(Camera *camera, Vec2 center);
Vec2 Camera_WorldToScreen(Camera *cam, Vec2 pos);
Vec2 Camera_WorldToScreenF(Camera *cam, float x, float y);
Vec2 Camera_ScreenToWorld(Camera *cam, Vec2 pos);
Vec2 Camera_ScreenToWorldF(Camera *cam, float x, float y);

SDL_FRect Camera_GetBounds(Camera *cam);
void      Camera_Draw(Camera *camera, SDL_Renderer *renderer);
void      Camera_Destroy(Camera *camera);

#endif // DREV_CAMERA_H
