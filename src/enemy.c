#include <enemy.h>

#include <stdio.h>

#define ENEMY_HEIGHT       10.f
#define ENEMY_WIDTH        20.f
#define ENEMY_SPEED        150.f
#define ENEMY_FIRE_RATE_MS 2000

static void Enemy_SetTexture(Enemy *enemy, SDL_Renderer *renderer) {
    SDL_FRect src_rect = {0.f, 0.f, ENEMY_WIDTH, ENEMY_HEIGHT};

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

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderDrawLineF(renderer, ENEMY_WIDTH / 2.f, ENEMY_HEIGHT / 2.f, ENEMY_WIDTH / 2.f, 0.f);

    SDL_SetRenderTarget(renderer, NULL);
}

static void Enemy_Fire(Enemy *enemy) {
    for (unsigned int i = 0; i < ENEMY_MAX_BULLETS; i++) {
        if (enemy->bullets[i].in_use == 0) {
            Vec2 vel = Vec2_Normalize((Vec2
            ){enemy->target.x - enemy->position.x, enemy->target.y - enemy->position.y});
            Bullet_Init(&enemy->bullets[i].bullet, BulletType_Enemy, enemy->position, vel);

            enemy->bullets[i].in_use = 1;

            return;
        }
    }
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

    for (unsigned int i = 0; i < ENEMY_MAX_BULLETS; i++) {
        enemy->bullets[i].in_use = 0;
    }

    return 0;
}

void Enemy_Update(Enemy *enemy, float delta) {
    enemy->position.x += enemy->velocity.x * ENEMY_SPEED * delta;
    enemy->position.y += enemy->velocity.y * ENEMY_SPEED * delta;

    enemy->last_fired += (unsigned int)(delta * 1000.f);
    if (enemy->last_fired >= ENEMY_FIRE_RATE_MS) {
        enemy->last_fired = 0;
        Enemy_Fire(enemy);
    }

    for (unsigned int i = 0; i < ENEMY_MAX_BULLETS; i++) {
        if (enemy->bullets[i].in_use == 1) {
            if (enemy->bullets[i].bullet.health <= 0.f) {
                enemy->bullets[i].in_use = 0;
            } else {
                Bullet_Update(&enemy->bullets[i].bullet, delta);
            }
        }
    }
}

void Enemey_Draw(const Enemy *enemy, SDL_Renderer *renderer) {
    SDL_FRect dst_rect = {
        enemy->position.x - ENEMY_WIDTH / 2.f,
        enemy->position.y - ENEMY_HEIGHT / 2.f,
        ENEMY_WIDTH,
        ENEMY_HEIGHT};

    // SDL_SetRenderDrawColor(renderer, 0xFF, 0x0A, 0x0A, 0xFF);
    // SDL_RenderDrawLineF(
    //   renderer,
    //   enemy->position.x,
    //   enemy->position.y,
    //   enemy->target.x,
    //   enemy->target.y
    // );

    SDL_RenderCopyExF(
        renderer,
        enemy->texture,
        NULL,
        &dst_rect,
        enemy->rotation * (180.f / M_PI),
        NULL,
        SDL_FLIP_NONE
    );

    for (unsigned int i = 0; i < ENEMY_MAX_BULLETS; i++) {
        if (enemy->bullets[i].in_use == 1) {
            Bullet_Draw(&enemy->bullets[i].bullet, renderer);
        }
    }
}

void Enemy_FacePoint(Enemy *enemy, Vec2 point) {
    Vec2 diff = (Vec2){point.x - enemy->position.x, enemy->position.y - point.y};

    enemy->rotation = SDL_atan2f(diff.x, diff.y);
    enemy->target   = point;

    return;
}

void Enemy_Destroy(Enemy *enemy) {
    enemy->position = (Vec2){0.f, 0.f};
    enemy->velocity = (Vec2){0.f, 0.f};
    enemy->rotation = 0.f;
    enemy->health   = 0;

    SDL_DestroyTexture(enemy->texture);
}
