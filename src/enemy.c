#include "enemy.h"

#include "bullet.h"
#include "vec.h"

#include <SDL.h>
#include <stdio.h>

#define ENEMY_HEIGHT       10.f
#define ENEMY_HALF_HEIGHT  (ENEMY_HEIGHT / 2.f)
#define ENEMY_WIDTH        20.f
#define ENEMY_HALF_WIDTH   (ENEMY_WIDTH / 2.f)
#define ENEMY_SPEED        100.f
#define ENEMY_FIRE_RATE_MS 2000

static void Enemy_SetTexture(Enemy *enemy, SDL_Renderer *renderer) {
    SDL_FRect src_rect = {0.f, 0.f, ENEMY_WIDTH, ENEMY_HEIGHT};

    enemy->texture = SDL_CreateTexture(renderer,
                                       SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_TARGET,
                                       ENEMY_WIDTH,
                                       ENEMY_HEIGHT);
    SDL_SetRenderTarget(renderer, enemy->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0xA0, 0xDD, 0xFF);
    SDL_RenderFillRectF(renderer, &src_rect);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderDrawLineF(renderer, ENEMY_HALF_WIDTH, ENEMY_HALF_HEIGHT, ENEMY_HALF_WIDTH, 0.f);

    SDL_SetRenderTarget(renderer, NULL);
}

static void Enemy_Fire(Enemy *enemy, BulletContainer *c) {
    Vec2 vel = (Vec2){enemy->target.x - enemy->position.x, enemy->target.y - enemy->position.y};
    vel      = Vec2_Normalize(vel);
    BulletContainer_Add(c, BulletType_Enemy, enemy->position, vel);
}

int Enemy_Init(Enemy *enemy, Vec2 position, unsigned int health, SDL_Renderer *renderer) {
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    if (!(info.flags & SDL_RENDERER_TARGETTEXTURE)) {
        printf("ERROR :: rendering to texture not supported. cannot render enemy\n");
    }

    enemy->position   = position;
    enemy->velocity   = Vec2_Zero;
    enemy->health     = health;
    enemy->rotation   = 0.f;
    enemy->last_fired = 0;

    Enemy_SetTexture(enemy, renderer);

    return 0;
}

void Enemy_Update(Enemy *enemy, BulletContainer *c, float delta) {
    enemy->position.x += enemy->velocity.x * ENEMY_SPEED * delta;
    enemy->position.y += enemy->velocity.y * ENEMY_SPEED * delta;

    enemy->last_fired += (unsigned int)(delta * 1000.f);
    if (enemy->last_fired >= ENEMY_FIRE_RATE_MS) {
        enemy->last_fired = 0;
        Enemy_Fire(enemy, c);
    }
}

void Enemey_Draw(const Enemy *enemy, SDL_Renderer *renderer) {
    SDL_FRect dst_rect = {.x = enemy->position.x - ENEMY_HALF_WIDTH,
                          .y = enemy->position.y - ENEMY_HALF_HEIGHT,
                          .w = ENEMY_WIDTH,
                          .h = ENEMY_HEIGHT};

    SDL_RenderCopyExF(renderer,
                      enemy->texture,
                      NULL,
                      &dst_rect,
                      enemy->rotation * (180.f / M_PI),
                      NULL,
                      SDL_FLIP_NONE);

    SDL_FRect bb = Enemy_BoundingBox(enemy);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x12, 0x12, 0xFF);
    SDL_RenderDrawRectF(renderer, &bb);
}

void Enemy_FacePoint(Enemy *enemy, Vec2 point) {
    Vec2 diff = (Vec2){point.x - enemy->position.x, enemy->position.y - point.y};

    enemy->rotation = SDL_atan2f(diff.x, diff.y);
    enemy->target   = point;

    return;
}

static float get_min(float x, float y, float z, float a) {
    float min = x;

    min = SDL_min(min, y);
    min = SDL_min(min, z);
    min = SDL_min(min, a);

    return min;
}

static float get_max(float x, float y, float z, float a) {
    float min = x;

    min = SDL_max(min, y);
    min = SDL_max(min, z);
    min = SDL_max(min, a);

    return min;
}

SDL_FRect Enemy_BoundingBox(const Enemy *enemy) {
    float x = -ENEMY_HALF_WIDTH;
    float y = -ENEMY_HALF_HEIGHT;

    Vec2 ul = (Vec2){.x = x, .y = y};
    Vec2 ur = (Vec2){.x = ENEMY_HALF_WIDTH, .y = y};
    Vec2 ll = (Vec2){.x = x, .y = ENEMY_HALF_HEIGHT};
    Vec2 lr = (Vec2){.x = ENEMY_HALF_WIDTH, .y = ENEMY_HALF_HEIGHT};

    Vec2 rul = Vec2_Rotate(ul, enemy->rotation);
    Vec2 rur = Vec2_Rotate(ur, enemy->rotation);
    Vec2 rll = Vec2_Rotate(ll, enemy->rotation);
    Vec2 rlr = Vec2_Rotate(lr, enemy->rotation);

    float min_x, min_y, max_x, max_y;
    min_x = get_min(rul.x, rur.x, rll.x, rlr.x);
    min_y = get_min(rul.y, rur.y, rll.y, rlr.y);
    max_x = get_max(rul.x, rur.x, rll.x, rlr.x);
    max_y = get_max(rul.y, rur.y, rll.y, rlr.y);

    SDL_FRect bounding_box = (SDL_FRect){.x = min_x + enemy->position.x,
                                         .y = min_y + enemy->position.y,
                                         .w = max_x - min_x,
                                         .h = max_y - min_y};

    return bounding_box;
}

void Enemy_Destroy(Enemy *enemy) {
    enemy->position = (Vec2){0.f, 0.f};
    enemy->velocity = (Vec2){0.f, 0.f};
    enemy->rotation = 0.f;
    enemy->health   = 0;

    SDL_DestroyTexture(enemy->texture);
}
