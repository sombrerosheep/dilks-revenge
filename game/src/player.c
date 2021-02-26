#include <player.h>

int Player_Init(Player *player, Vec2 starting_pos) {
  player->position = starting_pos;
  player->velocity = Vec2_Zero;

  return 0;
}

void Player_Update(Player *player) {
  
}

void Player_Draw(const Player *player, SDL_Renderer *renderer) {
  float player_width = 25.f;
  float player_height = 25.f;
  SDL_FRect player_rect = (SDL_FRect){
    player->position.x,
    player->position.y,
    player_width,
    player_height
  };

  SDL_SetRenderDrawColor(renderer, 0xAA, 0x00, 0xAA, 0xFF);
  SDL_RenderFillRectF(renderer, &player_rect);

  return;
}

void Player_Destroy(Player *player) {
  player->position = Vec2_Zero;
  player->velocity = Vec2_Zero;

  return;
}
