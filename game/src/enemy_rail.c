#include <enemy_rail.h>

#include <stdio.h>

struct _enemy_rail {
  Vec2 start;
  Vec2 end;
  Enemy *enemies;
  unsigned int num_enemies;
};

EnemyRail *EnemyRail_Create(Vec2 start, Vec2 end) {
  EnemyRail *rail = NULL;

  if ((rail = malloc(sizeof(EnemyRail))) == NULL) {
    printf("ERROR :: Unable to allocate memory for rail\n");
    return NULL;
  }

  rail->start = start;
  rail->end = end;
  
  rail->num_enemies = 3;
  if ((rail->enemies = malloc(sizeof(Enemy) * 3)) == NULL) {
    printf("ERROR :: Unable to allocate memory for enemy rail enemies");
    EnemyRail_Destroy(rail);
    free(rail);
    return NULL;
  }

  Enemy_Init(&rail->enemies[0], rail->start, 100);
  Vec2 middle = (Vec2){
    start.x == end.x ? start.x : start.x + ((SDL_fabsf(start.x - end.x) / 2.f)),
    start.y == end.y ? start.y : start.x + ((SDL_fabsf(start.y - end.y) / 2.f))
  };
  Enemy_Init(&rail->enemies[1], middle, 100);
  Enemy_Init(&rail->enemies[2], rail->end, 100);

  return rail;
}

void EnemyRail_Update(EnemyRail *rail) {
  for (unsigned int i = 0; i < rail->num_enemies; i++) {
   Enemy_Update(&rail->enemies[i]);
  }
}

void EnemyRail_Draw(const EnemyRail *rail, SDL_Renderer *renderer) {
  for (unsigned int i = 0; i < rail->num_enemies; i++) {
    Enemey_Draw(&rail->enemies[i], renderer);
  }
}

void EnemyRail_Destroy(EnemyRail *rail) {
  rail->start = Vec2_Zero;
  rail->end = Vec2_Zero;
  rail->num_enemies = 0;
  
  if (rail->enemies != NULL) {
    free(rail->enemies);
    rail->enemies = NULL;
  }
}