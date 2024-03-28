#ifndef DREV_UTIL_H
#define DREV_UTIL_H

// Maths

float clamp(float value, float upper, float lower);

float ease(float have, float want, float ease);

// Containers

#define CONCAT(a, b)     a##b
#define CONCAT3(a, b, c) a##b##c

#define DefineContainedType(T, S)                                      \
    typedef struct CONCAT(drev_contained_, T) CONCAT(Contained, T);    \
    struct CONCAT(drev_contained_, T) {                                \
        int in_use;                                                    \
        T   data;                                                      \
    };                                                                 \
    typedef struct CONCAT3(drev_, T, _container) CONCAT(T, Container); \
    struct CONCAT3(drev_, T, _container) {                             \
        unsigned int capacity;                                         \
        unsigned int size;                                             \
        CONCAT(Contained, T) items[S];                                 \
    }

#endif // DREV_UTIL_H
