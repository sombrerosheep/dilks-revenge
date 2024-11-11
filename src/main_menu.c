#include "main_menu.h"

#include "font.h"
#include "game_input.h"
#include "globals.h"
#include "resources.h"
#include "util.h"

#define num_menu_items 2
const char *options[num_menu_items] = {
    "Play",
    "Quit",
};

i8 MainMenu_Init(MainMenu *menu) {
    menu->selected = 0;

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
}

void MainMenu_Draw(MainMenu *menu) {
    SDL_Color option_color   = ColorWhite;
    SDL_Color selected_color = ColorGreen;
    Font     *title          = Resources_GetTitleFont();
    Font     *menu_font      = Resources_GetMenuFont();

    float items_x = 400.f;
    float items_y = 400.f;

    Font_DrawText(title, "Dilks Revenge", 100.f, 100.f);

    for (int i = 0; i < num_menu_items; i++) {
        if (menu->selected == i) {
            Font_DrawTextC(menu_font, options[i], items_x, items_y, selected_color);
        } else {
            Font_DrawTextC(menu_font, options[i], items_x, items_y, option_color);
        }

        items_y += Font_GetLineAdvance(menu_font);
    }
}
