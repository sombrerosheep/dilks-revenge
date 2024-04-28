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
#define UI_TOP_INDENT   20.f

void Debug_Draw(void) {
    Camera *cam   = Resources_GetMainCamera();
    Font   *debug = Resources_GetDebugFont();

    Vec2      camera_pos    = Resources_GetMainCamera()->position;
    Vec2      camera_screen = Camera_WorldToScreen(cam, camera_pos);
    Vec2      player_pos    = Entities_GetPlayerPosition();
    Vec2      player_screen = Camera_WorldToScreen(cam, player_pos);
    SDL_FRect bounds        = Camera_GetBounds(cam);

    f32 x = UI_LEFT_INDENT;
    f32 y = UI_TOP_INDENT;

    char buffer[TEXT_BUFFER_LEN] = {0};

    SDL_snprintf(buffer,
                 TEXT_BUFFER_LEN,
                 "Camera: %.2f x %.2f (%.2f x %.2f)",
                 camera_pos.x,
                 camera_pos.y,
                 camera_screen.x,
                 camera_screen.y);
    Font_DrawText(debug, buffer, x, y);

    y += Font_GetLineAdvance(debug);

    SDL_snprintf(buffer,
                 TEXT_BUFFER_LEN,
                 "Player: %.2f x %.2f (%.2f x %.2f)",
                 player_pos.x,
                 player_pos.y,
                 player_screen.x,
                 player_screen.y);
    Font_DrawText(debug, buffer, x, y);

    y += Font_GetLineAdvance(debug);

    SDL_snprintf(buffer,
                 TEXT_BUFFER_LEN,
                 "CameraUnitBounds: %.2f x %.2f (%.2f x %.2f)\n",
                 bounds.x,
                 bounds.y,
                 bounds.w,
                 bounds.h);
    Font_DrawText(debug, buffer, x, y);

    y += Font_GetLineAdvance(debug);

    return;
}
