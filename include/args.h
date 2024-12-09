#ifndef DREV_ARGS_H
#define DREV_ARGS_H

#include "types.h"

typedef struct slice Slice;

struct slice {
    const char *buffer;
    u32         len;
};

void Slice_Put(Slice *s, FILE *dst);

// #define MAX_PATH_NAME  32
// #define MAX_PATH_VALUE 256

typedef struct path_pair Pair;

struct path_pair {
    Slice key;
    Slice value;
};

typedef struct drev_args Args;

struct drev_args {
    Pair *pairs;
    u32   count;
};

Args ParseArgs(u32 count, const char **args);

#endif // DREV_ARGS_H
