#define LCG_A 1664525
#define LCG_C 1013904223
#define LCG_M (1U << 31) // 2^31

//rand
void lcg_init(unsigned int seed);
unsigned int lcg_random_between(int m);