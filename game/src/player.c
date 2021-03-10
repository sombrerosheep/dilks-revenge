#include <player.h>

#include <stdio.h>

#define PLAYER_SPEED 250.f
#define PLAYER_HEIGHT 25.f
#define PLAYER_WIDTH 25.f

int Player_Init(Player *player, Vec2 starting_pos) {
  player->position.x = starting_pos.x - (PLAYER_HEIGHT / 2.f);
  player->position.y = starting_pos.y - (PLAYER_WIDTH / 2.f);
  player->velocity = Vec2_Zero;

  return 0;
}

void Player_Update(Player *player, const Controller *controller, float delta) {
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

  player->velocity = Vec2_Normalize(player->velocity);

  player->position.x += player->velocity.x * PLAYER_SPEED * delta;
  player->position.y += player->velocity.y * PLAYER_SPEED * delta;
}

void Player_Draw(const Player *player, SDL_Renderer *renderer) {
  SDL_FRect player_rect = (SDL_FRect){
    player->position.x - PLAYER_WIDTH / 2.f,
    player->position.y - PLAYER_HEIGHT / 2.f,
    PLAYER_WIDTH,
    PLAYER_HEIGHT
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
