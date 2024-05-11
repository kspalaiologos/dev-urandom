// xorshiro-256 seeded with splitmix64 to generate random numbers that follow
// a normal distribution using the Box-Muller transform.

#include "common.h"
#include <math.h>

u64 rotl(u64 x, i32 k)_((x << k) | (x >> (64 - k)));
u64 s[4];
u64 rand_xor(void) {
	u64 result = rotl(s[0] + s[3], 23) + s[0];
	u64 t = s[1] << 17;
	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];
	s[2] ^= t;
	s[3] = rotl(s[3], 45);
	return result;
}

u64 srand_xor(u64 seed) {
	Fi(4, seed += 0x9e3779b97f4a7c15;
		  u64 z = seed;
		  z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		  z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
		  s[i] = z ^ (z >> 31))
}

typedef struct { double z0, z1; } d2;
d2 rand_normal(double mu, double sigma) {
	const double TWO_PI = 6.283185307179586;
	double u1, u2;
	do {
		u1 = rand_xor() / (double)UINT64_MAX;
		u2 = rand_xor() / (double)UINT64_MAX;
	} while (u1 <= 2.2204460492503131e-16);
	const double mag = sqrt(-2 * log(u1)) * sigma;
	const double z0 = mag * cos(TWO_PI * u2) + mu;
	const double z1 = mag * sin(TWO_PI * u2) + mu;
	return (d2) { z0, z1 };
}

#ifndef AS_LIB
int main(int argc, char * argv[]) {
	ASSERT(argc == 4, "Usage: %s <seed> <mu> <sigma>\n", argv[0]);
	u64 seed = strtoull(argv[1], NULL, 10);
	double mu = strtod(argv[2], NULL);
	double sigma = strtod(argv[3], NULL);
	srand_xor(seed);
	for (;;) { d2 r = rand_normal(mu, sigma); printf("%.17g\n%.17g\n", r.z0, r.z1); }
}
#endif