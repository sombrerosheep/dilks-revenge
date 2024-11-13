#include "ui.h"
#include "resources.h"
#include "system.h"
#include "util.h"

i8 UI_Init(UI *ui, const char *title, u8 num_buttons, Button *buttons) {
    ui->selected    = 0;
    ui->title       = title;
    ui->num_buttons = num_buttons;
    memcpy(&ui->buttons, buttons, (sizeof *buttons) * num_buttons);

    const SysConfig *cfg       = Resources_GetSysConfig();
    Font            *menu_font = Resources_GetMenuFont();
    Camera          *ui_cam    = Resources_GetUICamera();

    float items_y = 20.f;

    for (i8 i = 0; i < num_buttons; i++) {
        Button   *btn     = &ui->buttons[i];
        SDL_FRect text_bb = Font_MeasureText(menu_font, btn->text);
        text_bb           = Camera_ScreenToWorldRect(ui_cam, text_bb);
        text_bb.x         = -(text_bb.w / 2.f);
        text_bb.y         = items_y;

        btn->hit_box = text_bb;

        // todo: pretty sure this will return px and I'll need to conver that to units.
        items_y += Font_GetLineAdvance(menu_font) / cfg->ppu;
    }

    return 0;
}

void UI_Update(UI *ui) {
    const GameInput *input     = Resources_GetController();
    Vec2             mouse_pos = Camera_ScreenToWorldF(Resources_GetUICamera(),
                                           input->mouse_screen_x,
                                           input->mouse_screen_y);
    SDL_FPoint       p         = {.x = mouse_pos.x, .y = mouse_pos.y};

    for (i8 i = 0; i < ui->num_buttons; i++) {
        if (SDL_PointInFRect(&p, &ui->buttons[i].hit_box)) {
            ui->selected = i;

            if (input->mouse_left) {
                ui->buttons[i].callback();
            }
            return;
        }
    }

    // if (input->up && !prev.up) {
    if (Controller_JustPressed(input->up)) {
        ui->selected--;
    }

    // if (input->down && !prev.down) {
    if (Controller_JustPressed(input->down)) {
        ui->selected++;
    }

    if (ui->selected < 0) {
        ui->selected = ui->num_buttons - 1;
    }

    if (ui->selected >= ui->num_buttons) {
        ui->selected = 0;
    }

    if (Controller_Is(input->space)) {
        ui->buttons[ui->selected].callback();
    }
}

void UI_Draw(UI *ui) {
    Camera   *camera         = Resources_GetUICamera();
    SDL_Color option_color   = ColorWhite;
    SDL_Color selected_color = ColorGreen;
    Font     *title          = Resources_GetTitleFont();
    Font     *ui_font        = Resources_GetMenuFont();

    // does this give units?
    SDL_FRect rect = Font_MeasureText(title, ui->title);
    rect           = Camera_ScreenToWorldRect(camera, rect);

    Camera_DrawText(camera, title, ui->title, -(rect.w / 2.f), -40.f, ColorCyan);

    for (int i = 0; i < ui->num_buttons; i++) {
        SDL_Color color = ui->selected == i ? selected_color : option_color;
        Camera_DrawText(camera,
                        ui_font,
                        ui->buttons[i].text,
                        ui->buttons[i].hit_box.x,
                        ui->buttons[i].hit_box.y,
                        color);
    }

    const GameInput *input = Resources_GetController();
    Vec2 mouse_world = Camera_ScreenToWorldF(camera, input->mouse_screen_x, input->mouse_screen_y);
    Camera_DrawPlus(camera, mouse_world, ColorRed);
}
