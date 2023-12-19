#include <stdio.h>

#include <game.h>
#include <system.h>

int main() {
    printf("start\n");

    System sys;

    System_Init(&sys, "Dilks Revenge", 800, 600);
    Game *dr_game = Game_Create();

    Game_Run(dr_game, &sys);

    Game_Destroy(dr_game);
    System_Destroy(&sys);

    return 0;
}
