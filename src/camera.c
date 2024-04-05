#include "camera.h"

#include "globals.h"
#include "resources.h"
#include "util.h"
#include "vec.h"
#include <SDL_render.h>

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

Vec2 Camera_WorldToScreen(const Camera *cam, Vec2 pos) {
    Vec2 screen = Camera_WorldToScreenF(cam, pos.x, pos.y);

    return screen;
}

Vec2 Camera_WorldToScreenF(const Camera *cam, float x, float y) {
    Vec2 screen = (Vec2){
        .x = x - cam->position.x,
        .y = y - cam->position.y,
    };

    return screen;
}

Vec2 Camera_ScreenToWorld(const Camera *cam, Vec2 pos) {
    Vec2 world = Camera_ScreenToWorldF(cam, pos.x, pos.y);

    return world;
}

Vec2 Camera_ScreenToWorldF(const Camera *cam, float x, float y) {
    Vec2 world = (Vec2){
        .x = x + cam->position.x,
        .y = y + cam->position.y,
    };

    return world;
}

SDL_FRect Camera_GetBounds(const Camera *cam) {
    // bounds h/w is in pixels, should it be units?
    SDL_FRect rect = (SDL_FRect){
        .x = cam->position.x,
        .y = cam->position.y,
        .w = cam->half_size.x * 2.f,
        .h = cam->half_size.y * 2.f,
    };

    return rect;
}

void Camera_DrawFillRect(Camera *cam, SDL_FRect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Renderer *renderer = ResourceManager_GetRenderer();

    // Eventually PPM will come from a texture/sprite to be drawn
    float w_pixels  = rect.w * PIXELS_PER_METER;
    float h_pixels  = rect.h * PIXELS_PER_METER;
    float half_w_px = w_pixels / 2.f;
    float half_h_px = h_pixels / 2.f;

    Vec2 screen_pos = Camera_WorldToScreenF(cam, rect.x, rect.y);
    rect.x          = screen_pos.x - half_h_px;
    rect.y          = screen_pos.y - half_w_px;
    rect.w          = w_pixels;
    rect.h          = h_pixels;

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRectF(renderer, &rect);
}

void Camera_DrawRect(Camera *cam, SDL_FRect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Renderer *renderer = ResourceManager_GetRenderer();

    // Eventually PPM will come from a texture/sprite to be drawn
    float w_pixels  = rect.w * PIXELS_PER_METER;
    float h_pixels  = rect.y * PIXELS_PER_METER;
    float half_w_px = w_pixels / 2.f;
    float half_h_px = h_pixels / 2.f;

    Vec2 screen_pos = Camera_WorldToScreenF(cam, rect.x, rect.y);
    rect.x          = screen_pos.x - half_h_px;
    rect.y          = screen_pos.y - half_w_px;
    rect.w          = w_pixels;
    rect.h          = h_pixels;

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawRectF(renderer, &rect);
}

void Camera_DrawLine(Camera *cam,
                     float   x1,
                     float   y1,
                     float   x2,
                     float   y2,
                     Uint8   r,
                     Uint8   g,
                     Uint8   b,
                     Uint8   a //
) {
    SDL_Renderer *renderer = ResourceManager_GetRenderer();

    Vec2 start = Camera_WorldToScreenF(cam, x1, y1);
    Vec2 end   = Camera_WorldToScreenF(cam, x2, y2);

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

void Camera_Draw(const Camera *camera, SDL_Renderer *renderer) {
    UNUSED(camera);
    UNUSED(renderer);

    // tell the renderer to present the screen

#if DREV_DRAW_BB
    // todo: hardcoded bounding box
    // draw center bounds
    SDL_FRect rect = {
        .x = 0.f,
        .y = 0.f,
        .w = camera->half_size.x * 2.f,
        .h = camera->half_size.y * 2.f,
    };
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x0, 0x0, 0xFF);
    SDL_RenderDrawRectF(renderer, &rect);

    // draw current bounds
    Vec2 current_pos = Camera_ScreenToWorldF( //
        camera,
        0.f + camera->half_size.x,
        0.f + camera->half_size.y);

    rect.x = current_pos.x;
    rect.y = current_pos.y;

    SDL_SetRenderDrawColor(renderer, 0x0, 0xFF, 0x0, 0xFF);
    SDL_RenderDrawRectF(renderer, &rect);

    // draw center crosshair
    const float extra = 500.f;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x0, 0x0, 0xFF);
    Vec2 vert_start = (Vec2){
        .x = camera->half_size.x,
        .y = -extra,
    };
    Vec2 vert_end = (Vec2){
        .x = camera->half_size.x,
        .y = camera->half_size.y * 2.f + extra,
    };
    Vec2 hor_start = (Vec2){
        .x = -extra,
        .y = camera->half_size.y,
    };
    Vec2 hor_end = (Vec2){
        .x = camera->half_size.x * 2.f + extra,
        .y = camera->half_size.y,
    };

    SDL_RenderDrawLineF(renderer, vert_start.x, vert_start.y, vert_end.x, vert_end.y);
    SDL_RenderDrawLineF(renderer, hor_start.x, hor_start.y, hor_end.x, hor_end.y);
#endif
}

void Camera_Destroy(Camera *camera) {
    camera->position  = Vec2_Zero;
    camera->half_size = Vec2_Zero;
}
