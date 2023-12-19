#ifndef DREV_RANDOM_H
#define DREV_RANDOM_H

unsigned int random_get_eskil(unsigned int seed);
unsigned int random_get_eskil_between(unsigned int index, unsigned int min, unsigned int max);

void  random_init(int seed);
int   random_get();
float random_getf();
int   random_get_between(int min, int max);
float random_getf_between(float min, float max);

#endif // DREV_RANDOM_H
