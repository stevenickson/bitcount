#include "popcount.h"

// Constants needed for popcount algorithm
const uint64_t m1  = 0x5555555555555555;
const uint64_t m2  = 0x3333333333333333;
const uint64_t m4  = 0x0f0f0f0f0f0f0f0f;
const uint64_t m8  = 0x00ff00ff00ff00ff;
const uint64_t m16 = 0x0000ffff0000ffff;
const uint64_t m32 = 0x00000000ffffffff;
const uint64_t h01 = 0x0101010101010101;

// This function returns the number of bits set in a given uint64_t. It is
// the fastest known algorithm for doing this on a machine with fast
// multiplication.
inline int popcount64(volatile uint64_t x) {
	x -= (x >> 1) & m1;
	x = (x & m2) + ((x >> 2) & m2);
	x = (x + (x >> 4)) & m4;
	return (x * h01) >> 56;
}

// This function performs the popcount operation on a given array of uint64_t's
// to a given value (exlusive).
int popcount64i(volatile uint64_t *x, int n) {
	int sum = 0;
	for (int i = 0; i < n; i++)
		sum += popcount64(x[i]);
	return sum;
}

