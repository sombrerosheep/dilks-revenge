#ifndef DREV_GAME_UI_H
#define DREV_GAME_UI_H

#include "events.h"
#include "ui.h"

void pause_callback(void) {
    SDL_Event pause = {
        .type = PauseEventId,
    };

    SDL_PushEvent(&pause);
}

static void unpause_callback(void) {
    SDL_Event unpause = {
        .type = UnPauseEventId,
    };

    SDL_PushEvent(&unpause);
}

static void quit_to_menu_callback(void) {
    SDL_Event quit_to_menu = {
        .type = QuitToMenuEventId,
    };

    SDL_PushEvent(&quit_to_menu);
}

static void play_callback(void) {
    SDL_Event play = {
        .type = PlayEventId,
    };

    SDL_PushEvent(&play);
}

static void quit_callback(void) {
    SDL_Event quit = {
        .type = SDL_QUIT,
    };

    SDL_PushEvent(&quit);
}

enum pause_menu_items {
    PauseMenuItemContinue = 0,
    PauseMenuItemQuitToMenu,
    PauseMenuItemQuit,
    PauseMenuItemCount
};

Button pause_menu_buttons[PauseMenuItemCount] = {
    [PauseMenuItemContinue] =
        {
            .text     = "Resume",
            .callback = unpause_callback,
        },
    [PauseMenuItemQuitToMenu] =
        {
            .text     = "Quit to Menu",
            .callback = quit_to_menu_callback,
        },
    [PauseMenuItemQuit] =
        {
            .text     = "Quit",
            .callback = quit_callback,
        },
};

enum main_menu_items {
    MainMenuItemPlay = 0,
    MainMenuItemQuit,
    MainMenuItemCount
};

Button main_menu_buttons[MainMenuItemCount] = {
    [MainMenuItemPlay] =
        {
            .text     = "Play",
            .callback = play_callback,
        },
    [MainMenuItemQuit] =
        {
            .text     = "Quit",
            .callback = quit_callback,
        },
};

enum game_over_items {
    GameOverItemMenu = 0,
    GameOverItemQuit,
    GameOverItemCount
};

Button game_over_buttons[GameOverItemCount] = {
    [GameOverItemMenu] =
        {
            .text     = "Main Menu",
            .callback = quit_to_menu_callback,
        },
    [GameOverItemQuit] =
        {
            .text     = "Quit",
            .callback = quit_callback,
        },
};

#endif // DREV_GAME_UI_H
