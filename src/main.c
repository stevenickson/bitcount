#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <unistd.h>
#include <sys/time.h>

#include "popcount.h"
#include "counter.h"

#define SIZE 512000000 // 512 mb
// #define SIZE 2048000000
#define NUM_THREADS 4

long start, end;
struct timeval timecheck;

void start_timer() {
	gettimeofday(&timecheck, 0);
	start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
}

void end_timer() {
	gettimeofday(&timecheck, 0);
	end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
	printf("%ld\n\n", (end - start));
}

int main(int argc, char **argv) {
	// We first determine the number of uint64_t's needed to fill the
	// prescribed amount of memory, and retrieve it from the OS
	int num64 = SIZE / sizeof(uint64_t);

	start_timer();
	uint64_t *datafield = malloc(num64 * sizeof(uint64_t));
	printf("Malloced.\n");
	end_timer();

	for (int i = 0; i < num64; i++) {
		if (datafield[i] != 0)
			printf("Non-zero.");
	}

	start_timer();
	threaded_memclear64(datafield, num64, NUM_THREADS);
	printf("Cleared.\n");
	end_timer();
	datafield[1235] = 1;

	start_timer();
	uint64_t c = threaded_popcount64(datafield, num64, NUM_THREADS);
	printf("Popcount (threaded): %ld\n", c);
	end_timer();

	start_timer();
	threaded_memclear64(datafield, num64, NUM_THREADS);
	printf("Cleared.\n");
	end_timer();

	start_timer();
	c = threaded_popcount64(datafield, num64, NUM_THREADS);
	printf("Popcount (threaded): %ld\n", c);
	end_timer();

	start_timer();
	c = popcount64i(datafield, num64);
	printf("Popcount (unthreaded): %ld\n", c);
	end_timer();

	free(datafield);
}
