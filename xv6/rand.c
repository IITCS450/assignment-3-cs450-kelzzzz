#include "rand.h"

static unsigned int lcg_state;

void lcg_init(unsigned int seed) {
    lcg_state = seed;
}

unsigned int lcg_random_between(int m) {
    if (m <= 0) return 1;
    lcg_state = (LCG_A * lcg_state + LCG_C) % LCG_M;
    return (lcg_state % m) + 1;
}