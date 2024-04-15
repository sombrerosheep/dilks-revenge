#include "font.h"

#include "resources.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#include <SDL.h>

#define DREV_PRINT_FONT_DATA  0
#define DREV_SAVE_FONT_BITMAP 0

static SDL_Color ColorWhite = {0xFF, 0xFF, 0xFF, 0xFF};
static SDL_Color ColorRed   = {0xFF, 0x0, 0x0, 0xFF};
static SDL_Color ColorGreen = {0x0, 0xFF, 0x0, 0xFF};
static SDL_Color ColorBlue  = {0x0, 0x0, 0xFF, 0xFF};
static SDL_Color ColorCyan  = {0x0, 0xFF, 0xFF, 0xFF};

static void SDLext_SetTextureColorMod(SDL_Texture *texture, SDL_Color color) {
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
}

static void SDLext_SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

static void draw_plus(SDL_Renderer *r, Vec2 p) {
    Vec2 v1, v2;
    Vec2 h1, h2;

    v1.x = p.x;
    v1.y = p.y - 10.f;
    v2.x = p.x;
    v2.y = p.y + 10.f;

    h1.x = p.x - 10.f;
    h1.y = p.y;
    h2.x = p.x + 10.f;
    h2.y = p.y;

    SDLext_SetRenderDrawColor(r, ColorCyan);
    SDL_RenderDrawLineF(r, v1.x, v1.y, v2.x, v2.y);
    SDL_RenderDrawLineF(r, h1.x, h1.y, h2.x, h2.y);

    return;
}

static unsigned char *read_file_contents(const char *filePath) {
    SDL_RWops *io = SDL_RWFromFile(filePath, "rb");

    if (io == NULL) {
        printf("Unable to open file from SD_RWFromFile: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_RWseek(io, 0, RW_SEEK_END);
    Sint64 sz = SDL_RWtell(io);
    SDL_RWseek(io, 0L, SEEK_SET);

    unsigned char *buffer = SDL_malloc(sizeof(unsigned char) * sz);
    if (buffer == NULL) {
        printf("Unable to allocate memory for file: %s\n", filePath);
        return NULL;
    }

    SDL_RWread(io, buffer, sizeof(unsigned char), sz);

    SDL_RWclose(io);

    return buffer;
}

int Font_Load(SDL_Renderer *renderer, Font *f, const char *fontPath, size_t font_sz) {
#define NUM_GLYPHS 95

    unsigned char *file_data = read_file_contents(fontPath);

    stbtt_packedchar glyph_metrics[NUM_GLYPHS] = {0};

    // make a bitmap larger than what we need
    // we'll crop it later
    int            width      = 1024;
    int            max_height = 1024;
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

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

    // todo: is there a reason to call stbtt_ScaleForPixelHeight vs calculating?
    // float scale = stbtt_ScaleForPixelHeight(&info, ranges[i].font_size);
    float scale = (float)font_sz / (float)(ascent - descent);

    f->scale      = scale;
    f->ascent     = ascent * scale;
    f->descent    = descent * scale;
    f->linegap    = lineGap * scale;
    f->font_px_sz = font_sz;

    for (unsigned int i = 0; i < NUM_GLYPHS; i++) {
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

    Uint32          *pixels = SDL_malloc(height * width * sizeof(Uint32));
    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    for (int i = 0; i < width * height; i++) {
        pixels[i] = SDL_MapRGBA(format, 0xff, 0xff, 0xff, bitmap[i]);
    }
    SDL_UpdateTexture(f->texture, NULL, pixels, width * sizeof(Uint32));

    // print font and glyph metrics
    if (DREV_PRINT_FONT_DATA) {
        printf("size    %lu:\n", font_sz);
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
    }

    // Write atlas to a file
    if (DREV_SAVE_FONT_BITMAP) {
        stbi_write_png("x1.png", width, height, 1, bitmap, 0);
    }

    SDL_free(file_data);
    SDL_free(bitmap);

    return 0;
}

void Font_DrawText(Font *f, const char *text, float x, float y) {
    float current_point = x;
    // Keeps y as the top-left origin moving the baseline down the ascent
    float         baseline = y + f->ascent; // so the origin is top-left
    SDL_Renderer *renderer = ResourceManager_GetRenderer();

    size_t len = SDL_strlen(text);
    for (size_t i = 0; i < len; i++) {
        int color = i % 3;
        if (color == 0) {
            SDLext_SetTextureColorMod(f->texture, ColorRed);

        } else if (color == 1) {
            SDLext_SetTextureColorMod(f->texture, ColorGreen);

        } else {
            SDLext_SetTextureColorMod(f->texture, ColorBlue);
        }

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

        SDLext_SetRenderDrawColor(renderer, ColorRed);
        SDL_RenderDrawRectF(renderer, &dst_rect);

        SDL_RenderCopyF(renderer, f->texture, &src_rect, &dst_rect);
        draw_plus(renderer, (Vec2){.x = current_point, .y = baseline});

        if (i != len - 1) {
            current_point += g.xadvance;
        }
    }

    SDLext_SetTextureColorMod(f->texture, ColorWhite);

#ifdef DREV_DRAW_BB
    // Draw the baseline
    SDLext_SetRenderDrawColor(renderer, ColorGreen);
    SDL_RenderDrawLine(renderer, x, baseline, current_point + f->font_px_sz, baseline);

    // bounding box for the entire message
    SDLext_SetRenderDrawColor(renderer, ColorRed);
    SDL_Rect text_bb = (SDL_Rect){
        .x = x,
        .y = y,
        .w = current_point,
        .h = SDL_abs(f->ascent) + SDL_abs(f->descent),
    };
    SDL_RenderDrawRect(renderer, &text_bb);
#endif
}

void Font_Unload(Font *f) {
    SDL_DestroyTexture(f->texture);
    f->font_px_sz = 0;
    f->ascent     = 0;
    f->descent    = 0;
    f->linegap    = 0;
    memset(f->glyphs, 0, sizeof(f->glyphs));
}
