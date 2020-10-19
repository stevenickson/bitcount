#ifndef POPCOUNT_H
#define POPCOUNT_H

#include <stdint.h>

int popcount64(volatile uint64_t x);
int popcount64i(volatile uint64_t *x, int n);

#endif
