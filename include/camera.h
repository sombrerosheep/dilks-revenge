#ifndef DREV_CAMERA_H
#define DREV_CAMERA_H

#include "vec.h"

#include <SDL.h>

typedef struct drev_camera Camera;

struct drev_camera {
    Vec2 position;
    Vec2 half_size;
};

int Camera_Init(Camera *camera, Vec2 size);

void Camera_SetCenter(Camera *camera, Vec2 center);
Vec2 Camera_WorldToScreen(Camera *cam, Vec2 pos);
Vec2 Camera_WorldToScreenF(Camera *cam, float x, float y);
Vec2 Camera_ScreenToWorld(Camera *cam, Vec2 pos);
Vec2 Camera_ScreenToWorldF(Camera *cam, float x, float y);

void Camera_Destroy(Camera *camera);

#endif // DREV_CAMERA_H
