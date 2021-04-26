#ifndef DREV_SHIELD_H
#define DREV_SHIELD_H

#include <vec.h>
#include <SDL.h>

typedef struct shield Shield;

struct shield {
  Vec2 position;
  float radius;
  int target_inside;
};

void Shield_Init(Shield *s, float x, float y);
// void Shield_Update(Shield *s);
int Shield_EvalTarget(Shield *s, float x, float y, float w, float h);

void Shield_Draw(const Shield *s, SDL_Renderer *renderer);

void Shield_Destroy(Shield *s);

#endif // DREV_SHIELD_H