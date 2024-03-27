#ifndef DREV_CONTAINERS_H
#define DREV_CONTAINERS_H

#define CONCAT(a, b)     a##b
#define CONCAT3(a, b, c) a##b##c

///
/// @brief Adds prototypes for _Update and _Draw functions
///
#define NewGameObject(T)                                     \
    extern void CONCAT(T, _Update)(T * entity, float delta); \
    extern void CONCAT(T, _Draw)(const T *entity, SDL_Renderer *renderer)

///
/// @brief Implements a fixed size container for type T with capacity S.
///        Implements a Contained`T` struct to be used as container items.
///        Implements the container as a GameObject to call _Update/_Draw
///        on all container items.
///
#define NewGameObjectContainer(T, S)                                            \
    typedef struct CONCAT(drev_contained_, T) CONCAT(Contained, T);             \
    struct CONCAT(drev_contained_, T) {                                         \
        int in_use;                                                             \
        T   data;                                                               \
    };                                                                          \
    typedef struct CONCAT3(drev_, T, _game_container) CONCAT(T, GameContainer); \
    struct CONCAT3(drev_, T, _game_container) {                                 \
        unsigned int capacity;                                                  \
        unsigned int size;                                                      \
        CONCAT(Contained, T) items[S];                                          \
    };                                                                          \
    NewGameObject(CONCAT(T, GameContainer))

///
/// @brief Initializes a GameContainer. Must be called before it is used
///
#define GameContainer_Init(T, c)               \
    memset((c)->items, 0, sizeof((c)->items)); \
    (c)->capacity = sizeof((c)->items) / sizeof(CONCAT(Contained, T))

///
/// @brief Calls T_Update for all items in use
///
#define GameContainer_Update(T, c, d)                     \
    for (unsigned int i = 0; i < (c)->capacity; i++) {    \
        if ((c)->items[i].in_use == 1) {                  \
            CONCAT(T, _Update)(&(c)->items[i].data, (d)); \
        }                                                 \
    }

///
/// @brief Calls T_Draw for all items in use
///
#define GameContainer_Draw(T, c, r)                     \
    for (unsigned int i = 0; i < (c)->capacity; i++) {  \
        if ((c)->items[i].in_use == 1) {                \
            CONCAT(T, _Draw)(&(c)->items[i].data, (r)); \
        }                                               \
    }

///
/// @brief Inserts a T into the next available slot
///
#define GameContainer_Insert(T, c, n)                                       \
    for (unsigned int i = 0; i < (c)->capacity; i++) {                      \
        if ((c)->items[i].in_use == 0) {                                    \
            (c)->items[i] = (CONCAT(Contained, T)){.in_use = 1, .data = n}; \
            (c)->size++;                                                    \
            break;                                                          \
        }                                                                   \
    }

#endif // DREV_CONTAINERS_H
