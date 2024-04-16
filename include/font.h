#ifndef DREV_FONT_H
#define DREV_FONT_H

#include "vec.h"

#include <SDL.h>

#define CHAR_START 32
#define CHAR_COUNT 95

typedef struct drev_font  Font;
typedef struct drev_glyph Glyph;

struct drev_glyph {
    unsigned short atlas_x;  // x0
    unsigned short atlas_y;  // y0
    unsigned short width;    // x1 - x0
    unsigned short height;   // y1 - y0
    float          offset_x; // xoff
    float          offset_y; // yoff font->ascent - abs(yoff)
    float          xadvance;
    int            lsb;

    // original fields from stbtt_packedchar
    // unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
    // float          xadvance;
    // float xoff, yoff;   // bb_x1 && bb_y1 from GetCodepointBitmapBox.
    //                     // offsets from the baseline to render a glyph
    // float xoff2, yoff2; // bb_x2 && bb_y2 from GetCodepointBitmapBox
    //                     // not sure what these are for
};

struct drev_font {
    SDL_Texture *texture;
    Glyph        glyphs[CHAR_COUNT];
    size_t       font_px_sz;
    int          ascent, descent, linegap;
    float        scale;
};

int Font_Load(SDL_Renderer *renderer, Font *f, const char *fontPath, size_t font_sz);

void Font_DrawText(Font *f, const char *text, float x, float y);

void Font_Unload(Font *f);

#endif // DREV_FONT_H
