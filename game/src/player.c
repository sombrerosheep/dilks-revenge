#include <player.h>

#include <stdio.h>

#define PLAYER_SPEED 1.f

int Player_Init(Player *player, Vec2 starting_pos) {
  player->position = starting_pos;
  player->velocity = Vec2_Zero;

  return 0;
}

void Player_Update(Player *player, Controller *controller) {
  player->velocity = Vec2_Zero;

  if (controller->up) {
    player->velocity.y = -1.f;
  }

  if (controller->down) {
    player->velocity.y = 1.f;
  }

  if (controller->left) {
    player->velocity.x = -1.f;
  }

  if (controller->right) {
    player->velocity.x = 1.f;
  }

  player->position.x += player->velocity.x * PLAYER_SPEED;
  player->position.y += player->velocity.y * PLAYER_SPEED;
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
