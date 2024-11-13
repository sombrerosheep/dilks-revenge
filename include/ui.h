#ifndef DREV_UI_H
#define DREV_UI_H

#include "types.h"

#define MAX_BUTTONS 5

typedef struct ui  UI;
typedef struct btn Button;
typedef void (*CallbackFunc)(void);

struct btn {
    const char  *text;
    SDL_FRect    hit_box;
    CallbackFunc callback;
};

struct ui {
    i8          selected;
    const char *title;
    u8          num_buttons;
    Button      buttons[MAX_BUTTONS];
};

i8   UI_Init(UI *ui, const char *title, u8 num_buttons, Button *buttons);
void UI_Update(UI *ui);
void UI_Draw(UI *ui);

#endif // DREV_UI_H
