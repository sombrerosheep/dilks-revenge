#include "args.h"

void Slice_Put(Slice *s, FILE *dst) {
    for (u32 i = 0; i < s->len; i++) {
        putc(s->buffer[i], dst);
    }
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

Args ParseArgs(u32 argc, const char **argv) {
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
