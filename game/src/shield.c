#include <shield.h>

void draw_circle(SDL_Renderer *renderer, float n_cx, float n_cy, float radius) {
  SDL_SetRenderDrawColor(renderer, 0xAA, 0X10, 0X20, 0XFF);

  float error = (float)-radius;	
	float x = radius - 0.5f;
	float y = 0.5;
	float cx = n_cx - 0.5;
	float cy = n_cy - 0.5;

	while (x >= y) {
		SDL_RenderDrawPointF(renderer, (cx + x), (cy + y));
		SDL_RenderDrawPointF(renderer, (cx + y), (cy + x));

		if (x != 0) {
			SDL_RenderDrawPointF(renderer, (cx - x), (cy + y));
			SDL_RenderDrawPointF(renderer, (cx + y), (cy - x));
		}

		if (y != 0) {
			SDL_RenderDrawPointF(renderer, (cx + x), (cy - y));
			SDL_RenderDrawPointF(renderer, (cx - y), (cy + x));
		}

		if (x != 0 && y != 0) {
			SDL_RenderDrawPointF(renderer, (cx - x), (cy - y));
			SDL_RenderDrawPointF(renderer, (cx - y), (cy - x));
		}

    error += y;
		++y;
		error += y;

		if (error >= 0) {
			--x;
			error -= x;
			error -= x;
		}
	}
}

void Shield_Init(Shield *s, float x, float y) {
  s->position = (Vec2){ x, y };
  s->radius = 200.f;
}

// void Shield_Update(Shield *s) {
//   return;
// }

void Shield_Draw(const Shield *s, SDL_Renderer *renderer) {
  draw_circle(renderer, s->position.x, s->position.y, s->radius);
}


void Shield_Destroy(Shield *s) {
  s->position = (Vec2){ 0.f, 0.f };
  s->radius = 0.f;
}
