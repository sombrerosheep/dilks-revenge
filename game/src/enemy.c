#include <enemy.h>

#include <stdio.h>

#define ENEMY_HEIGHT 10.f
#define ENEMY_WIDTH 20.f
#define ENEMY_SPEED 150.f

int Enemy_Init(Enemy *enemy, Vec2 position, unsigned int health, SDL_Renderer *renderer) {
  SDL_RendererInfo info;
  SDL_GetRendererInfo(renderer, &info);
  if (!(info.flags & SDL_RENDERER_TARGETTEXTURE)) {
    printf("ERROR :: rendering to texture not supported. cannot render enemy\n");
  }

  enemy->position = position;
  enemy->velocity = Vec2_Zero;
  enemy->health = health;
  enemy->rotation = 0.f;

  SDL_FRect src_rect = {
    0.f,
    0.f,
    ENEMY_WIDTH,
    ENEMY_HEIGHT
  };

  enemy->texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_TARGET,
    ENEMY_WIDTH,
    ENEMY_HEIGHT
  );
  SDL_SetRenderTarget(renderer, enemy->texture);
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0x00, 0xA0, 0xDD, 0xFF);
  SDL_RenderFillRectF(renderer, &src_rect);

  SDL_SetRenderTarget(renderer, NULL);

  return 0;
}

void Enemy_Update(Enemy *enemy, float delta) {
  enemy->position.x += enemy->velocity.x * ENEMY_SPEED * delta;
  enemy->position.y += enemy->velocity.y * ENEMY_SPEED * delta;
}

void Enemey_Draw(const Enemy *enemy, SDL_Renderer *renderer) {
  SDL_FRect dst_rect = {
    enemy->position.x - ENEMY_WIDTH / 2.f,
    enemy->position.y - ENEMY_HEIGHT / 2.f,
    ENEMY_WIDTH,
    ENEMY_HEIGHT
  };

  SDL_RenderCopyExF(
    renderer,
    enemy->texture,
    NULL,
    &dst_rect,
    enemy->rotation,
    NULL,
    SDL_FLIP_NONE
  );
}

void Enemy_Destroy(Enemy *enemy) {
  enemy->position = (Vec2){ 0.f, 0.f };
  enemy->velocity = (Vec2){ 0.f, 0.f };
  enemy->rotation = 0.f;
  enemy->health = 0;

  SDL_DestroyTexture(enemy->texture);
}
