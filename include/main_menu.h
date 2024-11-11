#ifndef DREV_MAIN_MENU_H
#define DREV_MAIN_MENU_H

#include "font.h"
#include "game_input.h"
#include "types.h"

typedef struct drev_main_menu MainMenu;

struct drev_main_menu {
    int selected;
    int items;
};

i8   MainMenu_Init(MainMenu *menu);
void MainMenu_Update(MainMenu *menu);
void MainMenu_Draw(MainMenu *menu);

#endif // DREV_MAIN_MENU_H
