#include <random.h>
#include <stdlib.h>

unsigned int random_get_eskil(unsigned int index) {
    index = (index << 13) ^ index;
    return ((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

unsigned int random_get_eskil_between(unsigned int index, unsigned int min, unsigned int max) {
    unsigned int rand;

    rand = ((random_get_eskil(index) % (max - min + 1)) + min);

    return rand;
}

void random_init(int seed) {
    srand(seed);
}

int random_get() {
    int num;

    num = rand();

    return num;
}

float random_getf() {
    int   num;
    float res;

    num = rand();
    res = (float)num / 1000.f;

    return res;
}

int random_get_between(int min, int max) {
    int num;

    num = (random_get() % (max - min + 1)) + min;

    return num;
}

float random_getf_between(float min, float max) {
    float scale, num;

    scale = rand() / (float)RAND_MAX;
    num   = min + scale * (max - min);

    return num;
}
