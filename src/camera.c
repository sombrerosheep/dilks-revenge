#include "camera.h"

#include "globals.h"
#include "resources.h"
#include "util.h"
#include "vec.h"

#define FOCUS_OFFSET 35.f

const char *CameraFocusLabels[CameraFocusCount] = {
    [CameraFocusCenter] = "CameraFocusCenter",
    [CameraFocusTop]    = "CameraFocusTop",
    [CameraFocusBottom] = "CameraFocusBottom",
    [CameraFocusRight]  = "CameraFocusRight",
    [CameraFocusLeft]   = "CameraFocusLeft",
};

int Camera_Init(Camera *camera, float unit_height, float ratio) {
    camera->half_size.y = unit_height / 2.f;
    camera->half_size.x = unit_height * ratio / 2.f;

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

// camera position should be in top-left origin
void Camera_SetCenter(Camera *camera, Vec2 center) {
    camera->target_position.x = center.x - camera->half_size.x;
    camera->target_position.y = center.y - camera->half_size.y;
}

Vec2 Camera_WorldToScreenF(const Camera *cam, float x, float y) {
    Vec2 screen = (Vec2){
        .x = (x - cam->position.x) * PIXELS_PER_UNIT,
        .y = (y - cam->position.y) * PIXELS_PER_UNIT,
    };

    return screen;
}

Vec2 Camera_WorldToScreen(const Camera *cam, Vec2 pos) {
    Vec2 screen = Camera_WorldToScreenF(cam, pos.x, pos.y);

    return screen;
}

Vec2 Camera_ScreenToWorldF(const Camera *cam, float x, float y) {
    Vec2 world = (Vec2){
        .x = (x / PIXELS_PER_UNIT) + cam->position.x,
        .y = (y / PIXELS_PER_UNIT) + cam->position.y,
    };

    return world;
}

Vec2 Camera_ScreenToWorld(const Camera *cam, Vec2 pos) {
    Vec2 world = Camera_ScreenToWorldF(cam, pos.x, pos.y);

    return world;
}

// Camera bounds in world position
SDL_FRect Camera_GetBounds(const Camera *cam) {
    SDL_FRect rect = (SDL_FRect){
        .x = cam->position.x,
        .y = cam->position.y,
        .w = cam->half_size.x * 2.f,
        .h = cam->half_size.y * 2.f,
    };

    return rect;
}

///
/// @brief returns a screen-pixel dimensioned version of the provided rect
///        rect is assumed center-origin
///        x and y will be top-left origin dimensions
///        width and height will be in pixels
///
/// @param cam
/// @param rect
/// @return SDL_FRect
///
static SDL_FRect Camera_WorldRectToScreen(const Camera *cam, SDL_FRect rect) {
    Vec2 screen_pos = Camera_WorldToScreenF( //
        cam,
        rect.x - (rect.w / 2.f),
        rect.y - (rect.h / 2.f) //
    );

    // Eventually PPU will come from a texture/sprite to be drawn
    rect.x = screen_pos.x;
    rect.y = screen_pos.y;
    rect.w = rect.w * PIXELS_PER_UNIT;
    rect.h = rect.h * PIXELS_PER_UNIT;

    return rect;
}

void Camera_DrawFillRect(const Camera *cam, SDL_FRect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Renderer *renderer = ResourceManager_GetRenderer();

    SDL_FRect rrect = Camera_WorldRectToScreen(cam, rect);

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRectF(renderer, &rrect);
}

void Camera_DrawRectC(const Camera *cam, SDL_FRect rect, SDL_Color color) {
    Camera_DrawRect(cam, rect, color.r, color.g, color.b, color.a);
}

void Camera_DrawRect(const Camera *cam, SDL_FRect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Renderer *renderer = ResourceManager_GetRenderer();

    SDL_FRect rrect = Camera_WorldRectToScreen(cam, rect);

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawRectF(renderer, &rrect);
}

void Camera_DrawLine(const Camera *cam,
                     float         x1,
                     float         y1,
                     float         x2,
                     float         y2,
                     Uint8         r,
                     Uint8         g,
                     Uint8         b,
                     Uint8         a //
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

#if DREV_DRAW_BB
    // draw center bounds
    SDL_FRect rect = {
        .x = 0.f,
        .y = 0.f,
        .w = camera->half_size.x * 2.f,
        .h = camera->half_size.y * 2.f,
    };
    Camera_DrawRectC(camera, rect, ColorRed);

    // draw current camera bounds
    // DrawRectC assumes center origin
    rect.x = camera->position.x + camera->half_size.x;
    rect.y = camera->position.y + camera->half_size.y;

    Camera_DrawRectC(camera, rect, ColorGreen);

    // draw center crosshair
    SetRenderDrawColor(renderer, ColorRed);
    draw_plus(renderer,
              (Vec2){.x = camera->half_size.x * PIXELS_PER_UNIT,
                     .y = camera->half_size.y * PIXELS_PER_UNIT});
#endif
}

void Camera_Destroy(Camera *camera) {
    camera->position  = Vec2_Zero;
    camera->half_size = Vec2_Zero;
}
