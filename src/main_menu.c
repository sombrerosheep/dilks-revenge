#include "main_menu.h"

#include "camera.h"
#include "font.h"
#include "game_input.h"
#include "globals.h"
#include "resources.h"
#include "system.h"
#include "util.h"
#include <SDL_rect.h>

const char *menu_text[MenuItemCount] = {
    [MenuItemPlay] = "Play",
    [MenuItemQuit] = "Quit",
};

i8 MainMenu_Init(MainMenu *menu, CallbackFunc play, CallbackFunc quit) {
    const SysConfig *cfg = Resources_GetSysConfig();
    menu->selected       = 0;

    Font   *menu_font = Resources_GetMenuFont();
    Camera *ui        = Resources_GetUICamera();

    float     items_y = 20.f;
    SDL_FRect text_bb = Font_MeasureText(menu_font, menu_text[MenuItemPlay]);
    text_bb           = Camera_ScreenToWorldRect(ui, text_bb);
    text_bb.x         = -(text_bb.w / 2.f);
    text_bb.y         = items_y;

    // todo: pretty sure this will return px and I'll need to conver that to units.
    items_y += Font_GetLineAdvance(menu_font) / cfg->ppu;

    menu->options[MenuItemPlay] = (Option){
        .text     = menu_text[MenuItemPlay],
        .callback = play,
        .box      = text_bb,
    };

    text_bb                     = Font_MeasureText(menu_font, menu_text[MenuItemQuit]);
    text_bb                     = Camera_ScreenToWorldRect(ui, text_bb);
    text_bb.x                   = -(text_bb.w / 2.f);
    text_bb.y                   = items_y;
    menu->options[MenuItemQuit] = (Option){
        .text     = menu_text[MenuItemQuit],
        .callback = quit,
        .box      = text_bb,
    };

    return 0;
}

void MainMenu_Update(MainMenu *menu) {
    // static GameInput prev  = {0};
    const GameInput *input = Resources_GetController();

    for (i8 i = 0; i < MenuItemCount; i++) {
        Vec2       mouse_pos = Camera_ScreenToWorldF(Resources_GetUICamera(),
                                               input->mouse_screen_x,
                                               input->mouse_screen_y);
        SDL_FPoint p         = {.x = mouse_pos.x, .y = mouse_pos.y};

        if (SDL_PointInFRect(&p, &menu->options[i].box)) {
            menu->selected = i;

            if (input->mouse_left) {
                menu->options[i].callback();
            }
            break;
        }
    }

    // if (input->up && !prev.up) {
    if (Controller_JustPressed(input->up)) {
        menu->selected--;
    }

    // if (input->down && !prev.down) {
    if (Controller_JustPressed(input->down)) {
        menu->selected++;
    }

    if (menu->selected < 0) {
        menu->selected = MenuItemCount - 1;
    }

    if (menu->selected >= MenuItemCount) {
        menu->selected = 0;
    }

    if (Controller_Is(input->space)) {
        menu->options[menu->selected].callback();
    }
}

void MainMenu_Draw(MainMenu *menu) {
    const char *title_text     = "Dilks Revenge";
    Camera     *ui             = Resources_GetUICamera();
    SDL_Color   option_color   = ColorWhite;
    SDL_Color   selected_color = ColorGreen;
    Font       *title          = Resources_GetTitleFont();
    Font       *menu_font      = Resources_GetMenuFont();

    // does this give units?
    SDL_FRect rect = Font_MeasureText(title, title_text);
    rect           = Camera_ScreenToWorldRect(ui, rect);

    Camera_DrawText(ui, title, "Dilks Revenge", -(rect.w / 2.f), -40.f, ColorCyan);

    for (int i = 0; i < MenuItemCount; i++) {
        SDL_Color color = menu->selected == i ? selected_color : option_color;
        Camera_DrawText(ui,
                        menu_font,
                        menu_text[i],
                        menu->options[i].box.x,
                        menu->options[i].box.y,
                        color);
    }

    const GameInput *input = Resources_GetController();
    Vec2 mouse_world = Camera_ScreenToWorldF(ui, input->mouse_screen_x, input->mouse_screen_y);
    Camera_DrawPlus(ui, mouse_world, ColorRed);
}
