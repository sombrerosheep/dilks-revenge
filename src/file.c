#include "file.h"

#include <SDL.h>

unsigned char *ReadEntireFile(const char *filePath, const char *mode) {
    SDL_RWops *io = SDL_RWFromFile(filePath, mode);

    if (io == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Unable to open file from SD_RWFromFile: %s\n",
                     SDL_GetError());
        return NULL;
    }

    SDL_RWseek(io, 0, RW_SEEK_END);
    Sint64 sz = SDL_RWtell(io);
    SDL_RWseek(io, 0L, SEEK_SET);

    unsigned char *buffer = SDL_malloc(sizeof(unsigned char) * sz);
    if (buffer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Unable to allocate memory for file: %s\n",
                     filePath);
        return NULL;
    }

    SDL_RWread(io, buffer, sizeof(unsigned char), sz);

    SDL_RWclose(io);

    return buffer;
}
