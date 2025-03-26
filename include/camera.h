#ifndef DREV_CAMERA_H
#define DREV_CAMERA_H

#include "font.h"
#include "game_input.h"
#include "texture.h"
#include "vec.h"

#include <SDL.h>

typedef struct drev_camera Camera;

typedef enum {
    CameraFocusBottom = 0,
    CameraFocusLeft,
    CameraFocusTop,
    CameraFocusRight,
    CameraFocusCenter,

    CameraFocusCount
} CameraFocus;

extern const char *CameraFocusLabels[CameraFocusCount];

struct drev_camera {
    /// Camera position in top-left origin
    Vec2 position;
    /// position for camera to move to
    Vec2 target_position;
    /// Half the camera width and height in units
    Vec2        half_size;
    CameraFocus focus;
};

i8          Camera_Init(Camera *camera, f32 unit_height, f32 ratio);
void        Camera_Update(Camera *camera, f32 delta);
CameraFocus Camera_GetFocus(Camera *camera);
void        Camera_SetFocus(Camera *camera, CameraFocus focus);
void        Camera_SetCenter(Camera *camera, Vec2 center);
void        Camera_MoveCenter(Camera *camera, Vec2 center);
Vec2        Camera_WorldToScreen(const Camera *cam, Vec2 pos);
Vec2        Camera_WorldToScreenF(const Camera *cam, f32 x, f32 y);
Vec2        Camera_ScreenToWorld(const Camera *cam, Vec2 pos);
Vec2        Camera_ScreenToWorldF(const Camera *cam, f32 x, f32 y);
SDL_FRect   Camera_ScreenToWorldRect(Camera *cam, SDL_FRect rect);

///
/// @brief Returns the camera's rect in a top-left origin
///
/// @param cam camera to get the bounds for
///
SDL_FRect Camera_GetBounds(const Camera *cam);

// all coordinates for draw functions should be in world coordinates (x/y) and units.
void Camera_DrawFillRect(const Camera *cam, SDL_FRect rect, SDL_Color color);
void Camera_DrawRect(const Camera *cam, SDL_FRect rect, SDL_Color color);
void Camera_DrawPlusF(const Camera *camera, f32 x, f32 y, SDL_Color color);
void Camera_DrawPlus(const Camera *camera, Vec2 p, SDL_Color color);
void Camera_DrawLine(const Camera *cam, f32 x1, f32 y1, f32 x2, f32 y2, SDL_Color color);
void Camera_DrawText(const Camera *cam, Font *f, const char *text, f32 x, f32 y, SDL_Color color);
void Camera_DrawTexture(const Camera *cam, Texture *t, Vec2 p, f32 rot);

void Camera_Draw(const Camera *camera, SDL_Renderer *renderer);
void Camera_Destroy(Camera *camera);

#endif // DREV_CAMERA_H
