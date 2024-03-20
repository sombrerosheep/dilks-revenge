#include "camera.h"

#include "util.h"
#include "vec.h"

#define FOCUS_OFFSET 200.f

int Camera_Init(Camera *camera, Vec2 size) {
    camera->half_size.x = size.x / 2.f;
    camera->half_size.y = size.y / 2.f;

    Camera_SetCenter(camera, Vec2_Zero);
    camera->position = camera->target_position;

    return 0;
}

void Camera_Update(Camera *camera, float delta) {
    const float speed  = 250.f;
    camera->position.x = ease(camera->position.x, camera->target_position.x, delta * speed);
    camera->position.y = ease(camera->position.y, camera->target_position.y, delta * speed);
}

void Camera_SetFocus(Camera *camera, CameraFocus focus) {
    Vec2 center   = Vec2_Zero;
    camera->focus = focus;

    if (camera->focus == CameraFocusTop) {
        center.y -= FOCUS_OFFSET;
    } else if (camera->focus == CameraFocusBottom) {
        center.y += FOCUS_OFFSET;

    } else if (camera->focus == CameraFocusLeft) {
        center.x -= FOCUS_OFFSET;

    } else if (camera->focus == CameraFocusRight) {
        center.x += FOCUS_OFFSET;
    }

    Camera_SetCenter(camera, center);
}

void Camera_SetCenter(Camera *camera, Vec2 center) {
    camera->target_position.x = center.x - camera->half_size.x;
    camera->target_position.y = center.y - camera->half_size.y;
}

Vec2 Camera_WorldToScreen(Camera *cam, Vec2 pos) {
    Vec2 screen = Camera_WorldToScreenF(cam, pos.x, pos.y);

    return screen;
}

Vec2 Camera_WorldToScreenF(Camera *cam, float x, float y) {
    Vec2 screen = (Vec2){
        .x = x - cam->position.x,
        .y = y - cam->position.y,
    };

    return screen;
}

Vec2 Camera_ScreenToWorld(Camera *cam, Vec2 pos) {
    Vec2 world = Camera_ScreenToWorldF(cam, pos.x, pos.y);

    return world;
}

Vec2 Camera_ScreenToWorldF(Camera *cam, float x, float y) {
    Vec2 world = (Vec2){
        .x = x + cam->position.x,
        .y = y + cam->position.y,
    };

    return world;
}

SDL_FRect Camera_GetBounds(Camera *cam) {
    SDL_FRect rect = (SDL_FRect){
        .x = cam->position.x,
        .y = cam->position.y,
        .w = cam->half_size.x * 2.f,
        .h = cam->half_size.y * 2.f,
    };

    return rect;
}

void Camera_Destroy(Camera *camera) {
    camera->position  = Vec2_Zero;
    camera->half_size = Vec2_Zero;
}
