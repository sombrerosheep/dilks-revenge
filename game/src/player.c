#include <player.h>

#include <stdio.h>

#define PLAYER_SPEED 250.f
#define PLAYER_HEIGHT 25.f
#define PLAYER_WIDTH 25.f
#define PLAYER_AIM_ARM_LEN 35.f
#define PLAYER_FIRE_COOLDOWN 600

static void Player_Fire(Player *player) {
  for (unsigned int i = 0; i < PLAYER_MAX_BULLETS; i++) {
    if (player->bullets[i].in_use == 0) {
      player->bullets[i].bullet.position = (Vec2){
        player->aim.x * PLAYER_AIM_ARM_LEN + player->position.x,
        player->aim.y * PLAYER_AIM_ARM_LEN + player->position.y
      };
      player->bullets[i].bullet.velocity = Vec2_Normalize(player->aim);
      player->bullets[i].in_use = 1;

      return;
    }
  }
}

int Player_Init(Player *player, Vec2 starting_pos) {
  player->position.x = starting_pos.x - (PLAYER_HEIGHT / 2.f);
  player->position.y = starting_pos.y - (PLAYER_WIDTH / 2.f);
  player->velocity = Vec2_Zero;
  player->last_fire = 0;

  Shield_Init(&player->shield, player->position.x, player->position.y);

  for (unsigned int i = 0; i < PLAYER_MAX_BULLETS; i++) {
    player->bullets[i].in_use = 0;
  }

  return 0;
}

void Player_Update(Player *player, const Controller *controller, float delta) {
  player->last_fire += delta * 1000.f;

  player->aim = (Vec2){
    (float)controller->mouse_x - player->position.x,
    (float)controller->mouse_y - player->position.y
  };
  player->aim = Vec2_Normalize(player->aim);

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

  if (
    (controller->mouse_left || controller->space) &&
    player->last_fire >= PLAYER_FIRE_COOLDOWN
  ) {
    Player_Fire(player);
    player->last_fire = 0;
  }

  player->velocity = Vec2_Normalize(player->velocity);

  player->position.x += player->velocity.x * PLAYER_SPEED * delta;
  player->position.y += player->velocity.y * PLAYER_SPEED * delta;

  Shield_EvalTarget(&player->shield, player->position.x, player->position.y, PLAYER_WIDTH, PLAYER_HEIGHT);

  for (unsigned int i = 0; i < PLAYER_MAX_BULLETS; i++) {
    if (player->bullets[i].in_use == 1) {
      Bullet_Update(&player->bullets[i].bullet, delta);
    }
  }
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

  Shield_Draw(&player->shield, renderer);

  SDL_FRect aim_rect = {
    player->aim.x * PLAYER_AIM_ARM_LEN + player->position.x,
    player->aim.y * PLAYER_AIM_ARM_LEN + player->position.y,
    5.f,
    5.f
  };
  SDL_RenderFillRectF(renderer, &aim_rect);

  for (unsigned int i = 0; i < PLAYER_MAX_BULLETS; i++) {
    if (player->bullets[i].in_use == 1) {
      Bullet_Draw(&player->bullets[i].bullet, renderer);
    }
  }

  return;
}

void Player_Destroy(Player *player) {
  player->position = Vec2_Zero;
  player->velocity = Vec2_Zero;

  return;
}
