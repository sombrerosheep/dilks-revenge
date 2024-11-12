#include "main_menu.h"

#include "camera.h"
#include "font.h"
#include "game_input.h"
#include "globals.h"
#include "resources.h"
#include "system.h"
#include "util.h"

#define num_menu_items 2
const char *options[num_menu_items] = {
    "Play",
    "Quit",
};

i8 MainMenu_Init(MainMenu *menu, CallbackFunc play, CallbackFunc quit) {
    menu->selected  = 0;
    menu->play_func = play;
    menu->quit_func = quit;

    return 0;
}

void MainMenu_Update(MainMenu *menu) {
    // static GameInput prev  = {0};
    const GameInput *input = Resources_GetController();

    // if (input->up && !prev.up) {
    if (Controller_JustPressed(input->up)) {
        menu->selected--;
    }

    // if (input->down && !prev.down) {
    if (Controller_JustPressed(input->down)) {
        menu->selected++;
    }

    if (menu->selected < 0) {
        menu->selected = num_menu_items - 1;
    }

    if (menu->selected >= num_menu_items) {
        menu->selected = 0;
    }

    if (Controller_Is(input->space)) {
        if (menu->selected == 0) {
            menu->play_func();
        }

        if (menu->selected == 1) {
            menu->quit_func();
        }
    }
}

void MainMenu_Draw(MainMenu *menu) {
    const char      *title_text     = "Dilks Revenge";
    const SysConfig *cfg            = Resources_GetSysConfig();
    Camera          *ui             = Resources_GetUICamera();
    SDL_Color        option_color   = ColorWhite;
    SDL_Color        selected_color = ColorGreen;
    Font            *title          = Resources_GetTitleFont();
    Font            *menu_font      = Resources_GetMenuFont();

    // does this give units?
    SDL_FRect rect = Font_MeasureText(title, title_text);
    rect.w /= cfg->ppu;

    // float items_x = 400.f;
    // float items_y = 400.f;

    // Font_DrawText(title, "Dilks Revenge", 100.f, 100.f);
    Camera_DrawText(ui, title, "Dilks Revenge", -(rect.w / 2.f), -40.f, ColorCyan);

    float items_y = 20.f;
    for (int i = 0; i < num_menu_items; i++) {
        rect = Font_MeasureText(menu_font, options[i]);
        rect.w /= cfg->ppu;
        if (menu->selected == i) {
            // Font_DrawTextC(menu_font, options[i], items_x, items_y, selected_color);
            Camera_DrawText(ui, menu_font, options[i], -(rect.w / 2.f), items_y, selected_color);
        } else {
            // Font_DrawTextC(menu_font, options[i], items_x, items_y, option_color);
            Camera_DrawText(ui, menu_font, options[i], -(rect.w / 2.f), items_y, option_color);
        }

        // todo: pretty sure this will return px and I'll need to conver that to units.
        items_y += Font_GetLineAdvance(menu_font) / cfg->ppu;
    }
}
