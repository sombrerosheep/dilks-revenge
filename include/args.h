#ifndef DREV_ARGS_H
#define DREV_ARGS_H

#include "types.h"

typedef struct string String;

struct string {
    const char *buffer;
    u32         len;
};

void  String_Put(String *s, FILE *dst);
bool  String_Equal(String *a, String *b);
bool  String_EqualCstr(String *s, const char *cstr);
char *String_Cstr(String *s);

// #define MAX_PATH_NAME  32
// #define MAX_PATH_VALUE 256

typedef struct path_pair Pair;

struct path_pair {
    String key;
    String value;
};

typedef struct drev_args Args;

struct drev_args {
    Pair *pairs;
    u32   count;
};

Args Args_ParseArgv(u32 count, const char **args);
void Args_Put(Args *args, FILE *f);

#endif // DREV_ARGS_H
