#include "args.h"
#include <SDL_stdinc.h>
#include <string.h>

void String_Put(String *s, FILE *dst) {
    for (u32 i = 0; i < s->len; i++) {
        putc(s->buffer[i], dst);
    }
}

bool String_Equal(String *a, String *b) {
    if (a->len != b->len) {
        return false;
    }

    for (u32 i = 0; i < a->len; i++) {
        if (a->buffer[i] != b->buffer[i]) {
            return false;
        }
    }

    return true;
}

bool String_EqualCstr(String *s, const char *cstr) {
    u32 cstrlen = SDL_strlen(cstr);

    if (s->len != cstrlen) {
        return false;
    }

    for (u32 i = 0; i < s->len; i++) {
        if (s->buffer[i] != cstr[i]) {
            return false;
        }
    }

    return true;
}

char *String_Cstr(String *s) {
    char *cstr = NULL;

    if ((cstr = SDL_malloc(sizeof(*cstr) * s->len + 1)) == NULL) {
        return NULL;
    }

    memcpy(cstr, s->buffer, s->len);

    cstr[s->len] = '\0';

    return cstr;
}

static Pair parse(const char *buf) {
    Pair  p;
    int   buf_len     = strlen(buf);
    char *delim_index = strchr(buf, '=');

    p.key.buffer = buf;
    p.key.len    = delim_index - buf;

    p.value.buffer = delim_index + 1;
    p.value.len    = buf_len - p.key.len - 1;

    return p;
}

Args Args_ParseArgv(u32 argc, const char **argv) {
    Args args = {
        .count = argc - 1,
        .pairs = NULL,
    };

    if ((args.pairs = malloc(sizeof(*args.pairs) * args.count)) == NULL) {
        printf("ParseArgs::malloc failed to allocate\n");
        return args;
    }

    for (u32 pair_i = 0, argc_i = 1;           //
         pair_i < args.count && argc_i < argc; //
         pair_i++, argc_i++                    //
    ) {
        Pair p = parse(argv[argc_i]);
        if (p.key.len < 1 || p.value.len < 1) {
            args.count--;
            continue;
        }

        args.pairs[pair_i] = p;
    }

    return args;
}

void Args_Put(Args *args, FILE *f) {
    for (u32 i = 0; i < args->count; i++) {
        printf("Arg[%u]: ", i);
        String_Put(&args->pairs[i].key, f);
        putc('=', f);
        String_Put(&args->pairs[i].value, f);
        putc('\n', f);
    }
}
