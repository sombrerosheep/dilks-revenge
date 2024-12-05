#include "texture.h"
#include "resources.h"
#include "vec.h"
#include <SDL_render.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

bool Texture_InitFromFile(Texture *t, const char *filename, SDL_Renderer *renderer) {
    int      width, height, px_elements;
    stbi_uc *data = stbi_load(filename, &width, &height, &px_elements, STBI_rgb_alpha);

    if (data == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "error reading image: %s\n", SDL_GetError());
        return false;
    }

    int stb_bpp = px_elements * 8;
    int pitch   = width * px_elements;

    int bpp;
    u32 rmask, gmask, bmask, amask;

    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_RGBA32, &bpp, &rmask, &gmask, &bmask, &amask);
    SDL_assert(bpp == stb_bpp);

    SDL_CreateRGBSurfaceWithFormatFrom(data, width, height, stb_bpp, pitch, SDL_PIXELFORMAT_RGBA32);

    SDL_Surface *canvas =
        SDL_CreateRGBSurfaceFrom(data, width, height, bpp, pitch, rmask, gmask, bmask, amask);

    if (canvas == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "canvas is null! %s\n", SDL_GetError());
        return false;
    }

    t->texture = SDL_CreateTextureFromSurface(renderer, canvas);
    SDL_FreeSurface(canvas);

    if (t->texture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "Cannot create texture from canvas: %s\n",
                     SDL_GetError());
        return false;
    }

    return true;
}

Vec2i Texture_GetSize(Texture *t) {
    int w, h;

    SDL_QueryTexture(t->texture, NULL, NULL, &w, &h);

    Vec2i sz = (Vec2i){
        .x = w,
        .y = h,
    };

    return sz;
}
