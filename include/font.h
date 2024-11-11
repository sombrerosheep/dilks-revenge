#ifndef DREV_FONT_H
#define DREV_FONT_H

#include "types.h"
#include "vec.h"

#include <SDL.h>

#define CHAR_START 32
#define CHAR_COUNT 95

typedef struct drev_font  Font;
typedef struct drev_glyph Glyph;

struct drev_glyph {
    u16 atlas_x;  // x0
    u16 atlas_y;  // y0
    u16 width;    // x1 - x0
    u16 height;   // y1 - y0
    f32 offset_x; // xoff
    f32 offset_y; // yoff font->ascent - abs(yoff)
    f32 xadvance;
    i32 lsb; // unused right now...

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
    i32          ascent, descent, linegap;
    f32          scale;
};

i8 Font_Load(SDL_Renderer *renderer, Font *f, const char *fontPath, f32 sz);

u32  Font_GetLineAdvance(const Font *f);
void Font_DrawText(Font *f, const char *text, f32 x, f32 y);
void Font_DrawTextC(Font *f, const char *text, f32 x, f32 y, SDL_Color c);

void Font_Unload(Font *f);

#endif // DREV_FONT_H
