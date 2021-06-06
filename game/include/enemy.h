#ifndef DREV_ENEMY_H
#define DREV_ENEMY_H

#include <vec.h>

#include <SDL.h>

typedef struct _enemy Enemy;

struct _enemy {
  Vec2 position;
  Vec2 velocity;
  unsigned int health;
  double rotation;
  SDL_Texture *texture;
};

int Enemy_Init(Enemy *enemy, Vec2 position, unsigned int health, SDL_Renderer *renderer);
void Enemy_Update(Enemy *enemy, float delta);
void Enemey_Draw(const Enemy *enemy, SDL_Renderer *renderer);

void Enemy_FacePoint(Enemy *enemy, Vec2 point);

void Enemy_Destroy(Enemy *enemy);

#endif // DREV_ENEMY_H