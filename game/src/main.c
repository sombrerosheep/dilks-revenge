#include <stdio.h>

#include <game.h>

int main() {
  printf("start\n");

  Game *dr_game = Game_Create();

  Game_Run(dr_game);

  Game_Destroy(dr_game);

  return 0;
}