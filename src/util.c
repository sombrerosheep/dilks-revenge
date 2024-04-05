#include "util.h"

float clamp(float value, float upper, float lower) {
    float result = value;

    if (result > upper) {
        result = upper;
    }

    if (result < lower) {
        result = lower;
    }

    return result;
}

float ease(float have, float want, float ease) {
    if (have == want) {
        return have;
    }

    float dir = (have > want) ? -1.f : 1.f;
    have += dir * ease;

    float result = clamp( //
        have,
        dir > 0.f ? want : have,
        dir < 0.f ? want : have);

    return result;
}
