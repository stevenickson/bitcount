#ifndef COUNTER_H
#define COUNTER_H

#include <stdint.h>

// This struct defines a section of memory to weigh
struct workload {
	volatile uint64_t *data;
	int n;
};

// We can only accept a single void * as an arguement to a pthread function,
// so we create this struct to contain data to pass to such functions
struct thread_args {
	struct workload job;
	uint64_t *result;
};

/**
 * This function, given an array of data, the number of elements
 * it contains, creates a given number of workloads. This is represented
 * by an array of workload structs, which is stored in a given pointer.
 *
 * Parameters:
 * 	uint64_t *data - An array of uint64_t's on which to work on
 * 	int n - The number of uint64_t's in `data`
 * 	int div - The number of workloads to create (The "divisor")
 * 	struct **workload result - A pointer to an array of workloads, where the
 * 		result of this function will be stored.
 */
void make_workloads(volatile uint64_t *data, int nmemb, int div,
		struct workload *result);

void *run_memclear64(void *data);
void *run_popcount64(void *data);
void threaded_memclear64(volatile uint64_t *data, int nmemb, int nthreads);
uint64_t threaded_popcount64(volatile uint64_t *data, int nmemb, int nthreads);

#endif
