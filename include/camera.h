#ifndef DREV_CAMERA_H
#define DREV_CAMERA_H

#include "font.h"
#include "game_input.h"
#include "texture.h"
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

i8        Camera_Init(Camera *camera, f32 unit_height, f32 ratio);
void      Camera_Update(Camera *camera, f32 delta);
void      Camera_SetFocus(Camera *camera, CameraFocus focus);
void      Camera_SetCenter(Camera *camera, Vec2 center);
Vec2      Camera_WorldToScreen(const Camera *cam, Vec2 pos);
Vec2      Camera_WorldToScreenF(const Camera *cam, f32 x, f32 y);
Vec2      Camera_ScreenToWorld(const Camera *cam, Vec2 pos);
Vec2      Camera_ScreenToWorldF(const Camera *cam, f32 x, f32 y);
SDL_FRect Camera_ScreenToWorldRect(Camera *cam, SDL_FRect rect);
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
