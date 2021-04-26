#include <enemy.h>

#define ENEMY_HEIGHT 10.f
#define ENEMY_WIDTH 20.f
#define ENEMY_SPEED 150.f

int Enemy_Init(Enemy *enemy, Vec2 position, unsigned int health) {
  enemy->position = position;
  enemy->velocity = Vec2_Zero;
  enemy->health = health;

  return 0;
}

void Enemy_Update(Enemy *enemy, float delta) {
  enemy->position.x += enemy->velocity.x * ENEMY_SPEED * delta;
  enemy->position.y += enemy->velocity.y * ENEMY_SPEED * delta;
}

void Enemey_Draw(const Enemy *enemy, SDL_Renderer *renderer) {
  SDL_FRect rect = {
    enemy->position.x - ENEMY_WIDTH / 2.f,
    enemy->position.y - ENEMY_HEIGHT / 2.f,
    ENEMY_WIDTH,
    ENEMY_HEIGHT
  };

  SDL_SetRenderDrawColor(renderer, 0x00, 0xA0, 0xDD, 0xFF);
  SDL_RenderFillRectF(renderer, &rect);
}
