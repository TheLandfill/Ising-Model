#include "util.h"

namespace ising {
// This is 32 bit unsigned xorshift
uint32_t gen_rand_u_int(uint32_t* seed, uint32_t max_element) {
	uint32_t x = *seed;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	*seed = x;
	return ((double)x / (uint32_t)(-1)) * max_element;
}

}
