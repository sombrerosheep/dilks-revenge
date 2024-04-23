#include "debug.h"

#include "camera.h"
#include "entities.h"
#include "font.h"
#include "globals.h"
#include "resources.h"
#include "vec.h"

#include <SDL.h>

#define TEXT_BUFFER_LEN 512
#define UI_LEFT_INDENT  20.f

void Debug_Draw(void) {
    Camera *cam   = ResourceManager_GetMainCamera();
    Font   *debug = ResourceManager_GetDebugFont();
#define next_line() y += debug->ascent + SDL_fabs(debug->descent)

    Vec2      camera_pos    = ResourceManager_GetMainCamera()->position;
    Vec2      camera_screen = Camera_WorldToScreen(cam, camera_pos);
    Vec2      player_pos    = EntityManager_GetPlayerPosition();
    Vec2      player_screen = Camera_WorldToScreen(cam, player_pos);
    SDL_FRect bounds        = Camera_GetBounds(cam);

    float x = UI_LEFT_INDENT;
    float y = 100.f;

    char buffer[TEXT_BUFFER_LEN] = {0};

    snprintf(buffer,
             TEXT_BUFFER_LEN,
             "Camera: %.2f x %.2f (%.2f x %.2f)",
             camera_pos.x,
             camera_pos.y,
             camera_screen.x,
             camera_screen.y);
    Font_DrawText(debug, buffer, x, y);

    next_line();

    snprintf(buffer,
             TEXT_BUFFER_LEN,
             "Player: %.2f x %.2f (%.2f x %.2f)",
             player_pos.x,
             player_pos.y,
             player_screen.x,
             player_screen.y);
    Font_DrawText(debug, buffer, x, y);

    next_line();

    snprintf(buffer,
             TEXT_BUFFER_LEN,
             "CameraUnitBounds: %.2f x %.2f (%.2f x %.2f)\n",
             bounds.x,
             bounds.y,
             bounds.w,
             bounds.h);
    Font_DrawText(debug, buffer, x, y);

    next_line();

    // this should match camera_screen
    Vec2 cam_bounds_to_screen = Camera_WorldToScreenF(cam, bounds.x, bounds.y);
    SDL_assert(cam_bounds_to_screen.x == camera_screen.x);
    SDL_assert(cam_bounds_to_screen.y == camera_screen.y);

    snprintf(buffer,
             TEXT_BUFFER_LEN,
             "CameraPixelBounds: %.2f x %.2f (%.2f x %.2f)\n",
             cam_bounds_to_screen.x,
             cam_bounds_to_screen.y,
             bounds.w * PIXELS_PER_UNIT,
             bounds.h * PIXELS_PER_UNIT);
    Font_DrawText(debug, buffer, x, y);

    return;
}
