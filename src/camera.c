#include "camera.h"
#include "vec.h"

int Camera_Init(Camera *camera, SDL_Renderer *renderer, Vec2 size) {
    camera->renderer    = renderer;
    camera->half_size.x = size.x / 2.f;
    camera->half_size.y = size.y / 2.f;
    camera->position    = Vec2_Zero;

    return 0;
}

void Camera_SetCenter(Camera *camera, Vec2 center) {
    camera->position.x = center.x - camera->half_size.x;
    camera->position.y = center.y - camera->half_size.y;
}

Vec2 Camera_WorldToScreen(Camera *cam, Vec2 pos) {
    Vec2 screen = pos;
    screen.x -= cam->position.x;
    screen.y -= cam->position.y;

    return screen;
}

Vec2 Camera_ScreenToWorld(Camera *cam, Vec2 pos) {
    Vec2 world = pos;
    world.x += cam->position.x;
    world.y += cam->position.y;

    return world;
}

void Camera_Destroy(Camera *camera) {
    camera->renderer  = NULL;
    camera->position  = Vec2_Zero;
    camera->half_size = Vec2_Zero;
}
