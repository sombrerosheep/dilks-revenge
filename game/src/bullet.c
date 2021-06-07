#include <bullet.h>

#define BULLET_HEIGHT 5.f
#define BULLET_WIDTH 5.f
#define BULLET_SPEED 200.f

void Bullet_Init(Bullet *bullet, Vec2 pos, Vec2 vel) {
  bullet->position = pos;
  bullet->velocity = vel;
}

void Bullet_Update(Bullet *bullet, float delta) {
  bullet->position.x += bullet->velocity.x * BULLET_SPEED * delta;
  bullet->position.y += bullet->velocity.y * BULLET_SPEED * delta;
}

void Bullet_Draw(const Bullet *bullet, SDL_Renderer *renderer) {
  SDL_FRect rect = {
    bullet->position.x,
    bullet->position.y,
    BULLET_WIDTH,
    BULLET_HEIGHT
  };
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderFillRectF(renderer, &rect);
}
