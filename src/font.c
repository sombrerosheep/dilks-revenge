#include "font.h"

#include "file.h"
#include "globals.h"
#include "resources.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

#ifdef DREV_SAVE_FONT_BITMAP
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#endif

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#include <SDL.h>

i8 Font_Load(SDL_Renderer *renderer, Font *f, const char *fontPath, f32 font_sz) {
#define NUM_GLYPHS 95

    unsigned char *file_data = ReadEntireFile(fontPath, "rb");

    stbtt_packedchar glyph_metrics[NUM_GLYPHS] = {0};

    // make a bitmap larger than what we need
    // we'll crop it later
    i32            width      = 2048;
    i32            max_height = 2048;
    unsigned char *bitmap     = SDL_malloc(max_height * width);

    // do the packing, based on the ranges specified
    stbtt_pack_context pc;
    stbtt_PackBegin(&pc, bitmap, width, max_height, 0, 1, NULL);
    stbtt_PackSetOversampling(&pc, 1, 1);
    stbtt_PackFontRange(&pc, file_data, 0, font_sz, 32, 95, glyph_metrics);
    stbtt_PackEnd(&pc);

    // get the global metrics for each size/range
    stbtt_fontinfo info;
    stbtt_InitFont(&info, file_data, stbtt_GetFontOffsetForIndex(file_data, 0));

    i32 ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

    // todo: is there a reason to call stbtt_ScaleForPixelHeight vs calculating?
    // f32 scale = stbtt_ScaleForPixelHeight(&info, ranges[i].font_size);
    f32 scale = (f32)font_sz / (f32)(ascent - descent);

    f->scale      = scale;
    f->ascent     = ascent * scale;
    f->descent    = descent * scale;
    f->linegap    = lineGap * scale;
    f->font_px_sz = font_sz;

    for (u32 i = 0; i < NUM_GLYPHS; i++) {
        stbtt_packedchar pc = glyph_metrics[i];

        int lsb;
        // Not using advanceWidth since stbtt_packedchar already has this
        stbtt_GetCodepointHMetrics(&info, i + CHAR_START, NULL, &lsb);

        f->glyphs[i] = (Glyph){
            .atlas_x  = pc.x0,
            .atlas_y  = pc.y0,
            .width    = pc.x1 - pc.x0,
            .height   = pc.y1 - pc.y0,
            .offset_x = pc.xoff,
            .offset_y = pc.yoff,
            .xadvance = pc.xadvance,
            .lsb      = lsb * scale,
        };
    }

    // calculate fill rate and crop the bitmap
    int height = 0;
    for (int i = 0; i < NUM_GLYPHS; i++) {
        if (glyph_metrics[i].y1 > height) {
            height = glyph_metrics[i].y1;
        }
    }

    // When the asset packer is implemented, I may want a separate struct for the packing texture
    // (which contains the raw bitmap) and a game_mode font that references a loaded SDL_Texture.
    f->texture = SDL_CreateTexture(renderer,
                                   // Can i use a different format with the stb buffer?
                                   SDL_PIXELFORMAT_RGBA32,
                                   SDL_TEXTUREACCESS_STATIC,
                                   width,
                                   height);
    SDL_SetTextureBlendMode(f->texture, SDL_BLENDMODE_BLEND);

    u32             *pixels = SDL_malloc(height * width * sizeof(u32));
    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    for (i32 i = 0; i < width * height; i++) {
        pixels[i] = SDL_MapRGBA(format, 0xff, 0xff, 0xff, bitmap[i]);
    }
    SDL_UpdateTexture(f->texture, NULL, pixels, width * sizeof(u32));

// print font and glyph metrics
#ifdef DREV_PRINT_FONT_DATA
    printf("size    %.2f:\n", font_sz);
    printf("ascent  %3d:\n", f->ascent);
    printf("descent %.3d:\n", f->descent);
    printf("linegap %.3d:\n", f->linegap);

    for (int i = 0; i < NUM_GLYPHS; i++) {
        stbtt_packedchar m = glyph_metrics[i];
        printf("    '%c':  (x0,y0) = (%4d,%4d),  (x1,y1) = (%4d,%4d),  (xoff,yoff) = "
               "(%+5.1f,%+5.1f),  (xoff2,yoff2) = (%+5.1f,%+5.1f),  xadvance = %.3f\n",
               32 + i,
               m.x0,
               m.y0,
               m.x1,
               m.y1,
               m.xoff,
               m.yoff,
               m.xoff2,
               m.yoff2,
               m.xadvance);
    }
#endif

// Write atlas to a file
#ifdef DREV_SAVE_FONT_BITMAP
    char *fname = strrchr(fontPath, '/');
    fname++;
    char name[32];
    sprintf(name, "%s_%.0f.png", fname, font_sz);
    stbi_write_png(name, width, height, 1, bitmap, 0);
#endif

    SDL_free(file_data);
    SDL_free(bitmap);

    return 0;
}

SDL_FRect Font_MeasureText(Font *f, const char *text) {
    f32 x             = 0;
    f32 y             = 0;
    f32 current_point = x;

    size_t len = SDL_strlen(text);
    for (size_t i = 0; i < len; i++) {
        char  c = text[i];
        Glyph g = f->glyphs[c - CHAR_START];
        if (c == ' ') {
            // no need for a draw command for whitespace
            current_point += g.xadvance;
            continue;
        }

        // if (i != len - 1) {
        current_point += g.xadvance;
        // }
    }

    SDL_FRect text_bb = (SDL_FRect){
        .x = x,
        .y = y,
        .w = current_point,
        .h = SDL_abs(f->ascent) + SDL_abs(f->descent),
    };

    return text_bb;
}

void Font_DrawText(Font *f, const char *text, f32 x, f32 y) {
    Font_DrawTextC(f, text, x, y, ColorWhite);
}

void Font_DrawTextC(Font *f, const char *text, f32 x, f32 y, SDL_Color c) {
    f32 current_point = x;
    // Keeps y as the top-left origin moving the baseline down the ascent
    f32           baseline = y + f->ascent; // so the origin is top-left
    SDL_Renderer *renderer = Resources_GetRenderer();

    SDL_SetTextureColorMod(f->texture, c.r, c.g, c.b);

    size_t len = SDL_strlen(text);
    for (size_t i = 0; i < len; i++) {
        char  c = text[i];
        Glyph g = f->glyphs[c - CHAR_START];
        if (c == ' ') {
            // no need for a draw command for whitespace
            current_point += g.xadvance;
            continue;
        }

        SDL_Rect src_rect = (SDL_Rect){
            .x = g.atlas_x,
            .y = g.atlas_y,
            .w = g.width,
            .h = g.height,
        };
        SDL_FRect dst_rect = (SDL_FRect){
            .x = current_point + g.offset_x,
            .y = baseline + g.offset_y,
            .w = g.width,
            .h = g.height,
        };

#ifdef DREV_DRAW_TEXT_BB
        // Draw each characters bounding box and current_point X baseline
        SetRenderDrawColor(renderer, ColorRed);
        SDL_RenderDrawRectF(renderer, &dst_rect);

        draw_plus(renderer, (Vec2){.x = current_point, .y = baseline}, 10.f);
#endif

        SDL_RenderCopyF(renderer, f->texture, &src_rect, &dst_rect);

        current_point += g.xadvance;
    }

#ifdef DREV_DRAW_TEXT_BB
    // Draw the baseline
    SetRenderDrawColor(renderer, ColorGreen);
    // Why was f->font_px_sz here? to give some extra runway?
    // SDL_RenderDrawLine(renderer, x, baseline, current_point + f->font_px_sz, baseline);
    SDL_RenderDrawLine(renderer, x, baseline, current_point, baseline);

    // bounding box for the entire message
    SetRenderDrawColor(renderer, ColorRed);
    SDL_Rect text_bb = (SDL_Rect){
        .x = x,
        .y = y,
        .w = current_point - x,
        .h = SDL_abs(f->ascent) + SDL_abs(f->descent),
    };
    SDL_RenderDrawRect(renderer, &text_bb);
#endif
}

u32 Font_GetLineAdvance(const Font *f) {
    return f->ascent + SDL_fabs(f->descent);
}

void Font_Unload(Font *f) {
    SDL_DestroyTexture(f->texture);
    f->font_px_sz = 0;
    f->ascent     = 0;
    f->descent    = 0;
    f->linegap    = 0;
    memset(f->glyphs, 0, sizeof(f->glyphs));
}
