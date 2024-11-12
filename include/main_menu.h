#ifndef DREV_MAIN_MENU_H
#define DREV_MAIN_MENU_H

#include "font.h"
#include "game_input.h"
#include "types.h"

typedef struct drev_main_menu MainMenu;
typedef struct option         Option;
typedef void (*CallbackFunc)(void);

enum menu_items {
    MenuItemPlay = 0,
    MenuItemQuit,
    MenuItemCount
};

struct option {
    const char  *text;
    SDL_FRect    box;
    CallbackFunc callback;
};

struct drev_main_menu {
    i8     selected;
    u8     num_options;
    Option options[MenuItemCount];
};

i8   MainMenu_Init(MainMenu *menu, CallbackFunc play, CallbackFunc quit);
void MainMenu_Update(MainMenu *menu);
void MainMenu_Draw(MainMenu *menu);

#endif // DREV_MAIN_MENU_H
