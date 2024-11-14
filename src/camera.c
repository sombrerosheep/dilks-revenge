#include "camera.h"

#include "font.h"
#include "globals.h"
#include "resources.h"
#include "system.h"
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

i8 Camera_Init(Camera *camera, f32 unit_height, f32 ratio) {
    camera->half_size.y = unit_height / 2.f;
    camera->half_size.x = unit_height * ratio / 2.f;

    Camera_SetCenter(camera, Vec2_Zero);
    camera->position = camera->target_position;

    return 0;
}

void Camera_Update(Camera *camera, f32 delta) {
    const f32 speed    = 250.f;
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

Vec2 Camera_WorldToScreenF(const Camera *cam, f32 x, f32 y) {
    const SysConfig *config = Resources_GetSysConfig();
    Vec2             screen = (Vec2){
                    .x = (x - cam->position.x) * config->ppu,
                    .y = (y - cam->position.y) * config->ppu,
    };

    return screen;
}

Vec2 Camera_WorldToScreen(const Camera *cam, Vec2 pos) {
    Vec2 screen = Camera_WorldToScreenF(cam, pos.x, pos.y);

    return screen;
}

Vec2 Camera_ScreenToWorldF(const Camera *cam, f32 x, f32 y) {
    const SysConfig *config = Resources_GetSysConfig();
    Vec2             world  = (Vec2){
                     .x = (x / config->ppu) + cam->position.x,
                     .y = (y / config->ppu) + cam->position.y,
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
/// @brief returns a world-unit dimensioned version of the provided rect
///        rect is assumed center-origin
///        x and y will be top-left origin in units
///        width and height will be in units
///
/// @param cam
/// @param rect center-origin rect in pixels
/// @return SDL_FRect
///
SDL_FRect Camera_ScreenToWorldRect(Camera *cam, SDL_FRect rect) {
    const SysConfig *config     = Resources_GetSysConfig();
    Vec2             screen_pos = Camera_WorldToScreenF( //
        cam,
        rect.x - (rect.w / 2.f),
        rect.y - (rect.h / 2.f) //
    );

    rect.x = screen_pos.x;
    rect.y = screen_pos.y;
    rect.w /= config->ppu;
    rect.h /= config->ppu;

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
    const SysConfig *config     = Resources_GetSysConfig();
    Vec2             screen_pos = Camera_WorldToScreenF( //
        cam,
        rect.x - (rect.w / 2.f),
        rect.y - (rect.h / 2.f) //
    );

    // Eventually PPU will come from a texture/sprite to be drawn
    rect.x = screen_pos.x;
    rect.y = screen_pos.y;
    rect.w = rect.w * config->ppu;
    rect.h = rect.h * config->ppu;

    return rect;
}

void Camera_DrawFillRect(const Camera *cam, SDL_FRect rect, SDL_Color color) {
    SDL_Renderer *renderer = Resources_GetRenderer();

    SDL_FRect rrect = Camera_WorldRectToScreen(cam, rect);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(renderer, &rrect);
}

void Camera_DrawRect(const Camera *cam, SDL_FRect rect, SDL_Color color) {
    SDL_Renderer *renderer = Resources_GetRenderer();

    SDL_FRect rrect = Camera_WorldRectToScreen(cam, rect);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRectF(renderer, &rrect);
}

void Camera_DrawLine(const Camera *cam,
                     f32           x1,
                     f32           y1,
                     f32           x2,
                     f32           y2,
                     SDL_Color     color //
) {
    SDL_Renderer *renderer = Resources_GetRenderer();

    Vec2 start = Camera_WorldToScreenF(cam, x1, y1);
    Vec2 end   = Camera_WorldToScreenF(cam, x2, y2);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

void Camera_Draw(const Camera *camera, SDL_Renderer *renderer) {
    UNUSED(camera);
    UNUSED(renderer);

#if DREV_DRAW_BB
    const SysConfig *config = Resources_GetSysConfig();

    // draw center bounds
    SDL_FRect rect = {
        .x = 0.f,
        .y = 0.f,
        .w = camera->half_size.x * 2.f,
        .h = camera->half_size.y * 2.f,
    };
    Camera_DrawRect(camera, rect, ColorRed);

    // draw current camera bounds
    // DrawRectC assumes center origin
    rect.x = camera->position.x + camera->half_size.x;
    rect.y = camera->position.y + camera->half_size.y;

    Camera_DrawRect(camera, rect, ColorGreen);

    // Draw 0x0
    Camera_DrawPlus(camera, Vec2_Zero, ColorCyan);

    // draw center crosshair
    SetRenderDrawColor(renderer, ColorRed);
    draw_plus(renderer,
              (Vec2){
                  .x = camera->half_size.x * config->ppu,
                  .y = camera->half_size.y * config->ppu,
              },
              10.f);
#endif
}

void Camera_DrawPlusF(const Camera *camera, f32 x, f32 y, SDL_Color color) {
    SDL_Renderer *renderer = Resources_GetRenderer();

    Vec2 screen_p = Camera_WorldToScreenF(camera, x, y);

    SetRenderDrawColor(renderer, color);
    draw_plus(renderer,
              (Vec2){
                  .x = screen_p.x,
                  .y = screen_p.y,
              },
              10.f);
}

void Camera_DrawPlus(const Camera *camera, Vec2 p, SDL_Color color) {
    Camera_DrawPlusF(camera, p.x, p.y, color);
}

void Camera_DrawText(const Camera *cam, Font *f, const char *text, f32 x, f32 y, SDL_Color color) {
    Vec2 screen_p = Camera_WorldToScreenF(cam, x, y);

    Font_DrawTextC(f, text, screen_p.x, screen_p.y, color);
}

void Camera_Destroy(Camera *camera) {
    camera->position  = Vec2_Zero;
    camera->half_size = Vec2_Zero;
}
