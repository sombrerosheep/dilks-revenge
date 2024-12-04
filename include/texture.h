#ifndef DREV_TEXTURE_H
#define DREV_TEXTURE_H

#include "types.h"
#include <SDL.h>

typedef struct drev_texture Texture;

struct drev_texture {
    SDL_Texture *texture;
};

bool Texture_InitFromFile(Texture *t, const char *filename, SDL_Renderer *renderer);

#endif // DREV_TEXTURE_H
