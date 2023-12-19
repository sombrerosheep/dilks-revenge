#include <stdio.h>

#include <game.h>
#include <system.h>

#define WINDOW_HEIGHT 1200
#define WINDOW_WIDTH  1600
#define GAME_HEIGHT   600
#define GAME_WIDTH    800

int main() {
    printf("start\n");

    System sys;

    System_Init(&sys, "Dilks Revenge", WINDOW_WIDTH, WINDOW_HEIGHT, GAME_WIDTH, GAME_HEIGHT);
    Game *dr_game = Game_Create(GAME_WIDTH, GAME_HEIGHT);

    Game_Run(dr_game, &sys);

    Game_Destroy(dr_game);
    System_Destroy(&sys);

    return 0;
}
