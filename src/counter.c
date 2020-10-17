#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

#include "popcount.h"
#include "counter.h"


void *run_memclear64(void *data) {
	struct workload *work = (struct workload *) data;
	memset(work->data, 0, sizeof(uint64_t) * work->n);
	return 0;
}


void *run_popcount64(void *rawdata) {
	struct thread_args *args = (struct thread_args *) rawdata;
	*(args->result) = popcount64i(args->job.data, args->job.n);
	return 0;
}

void make_workloads(uint64_t *data, int nmemb, int div,
		struct workload *result) {
	int workload_size = nmemb / div; // We find the size of each workload
	int workload_remainder = nmemb % div; // There may be a remainder

	for (int i = 0; i < div; i++) {
		result[i].data = &(data[i * workload_size]);
		// Every workload is of size workload_size, except the
		// last one, which also takes the remainder
		result[i].n = (i != div - 1) ? workload_size
			: workload_size + workload_remainder;
	}
}
/*
void threaded_memclear64(uint64_t *start, int number, int threads) {
	int thread_workload = number / threads;
	// In case the workload does not divide evenly, we add the extra to the last
	// thread.
	int thread_extra_workload = number % threads;

	pthread_t worker_threads[threads];

	// Create n number of threads
	for (int i = 0; i < threads; i++) {
		// pthread functions may only accept void *'s, so we pass a pointer to a
		// struct.
		struct thread_args datastruct;
		datastruct.data = &start[i * thread_workload];

		datastruct.n = (i != threads - 1) ? thread_workload
			: thread_workload + thread_extra_workload;

		// Attempt to create the threads
		if (pthread_create(&worker_threads[i], 0, run_memclear64,
					&datastruct)) {
			printf("Unable to create thread #%d\n", i);
			return;
		}
	}

	// Now we wait for the worker threads to finish
	for (int i = 0; i < threads; i++) {
		pthread_join(worker_threads[i], 0);
	}
}
*/
void threaded_memclear64(uint64_t *data, int nmemb, int nthreads) {
	struct workload *workloads = malloc(sizeof(struct workload) * nthreads);
	pthread_t *worker_threads = malloc(sizeof(pthread_t) * nthreads);

	make_workloads(data, nmemb, nthreads, workloads);

	// Spawn nthreads number of threads, each with its own workload
	for (int i = 0; i < nthreads; i++) {
		int res =
			pthread_create(&worker_threads[i], 0, run_memclear64, &workloads[i]);
		if (res != 0) {
			printf("Failed to create thread #%d.\n", i);
			return;
		}
	}

	// Now we wait for the worker threads to finish
	for (int i = 0; i < nthreads; i++)
		pthread_join(worker_threads[i], 0);

	free(workloads);
	free(worker_threads);
}

uint64_t threaded_popcount64(uint64_t *data, int nmemb, int nthreads) {
	struct workload *workloads = malloc(sizeof(struct workload) * nthreads);
	pthread_t *worker_threads = malloc(sizeof(pthread_t) * nthreads);
	struct thread_args *args = malloc(sizeof(struct thread_args) * nthreads);

	make_workloads(data, nmemb, nthreads, workloads);

	// Spawn nthreads number of threads, each with its own workload
	for (int i = 0; i < nthreads; i++) {
		// Create a thread_args to contain the data we want to pass
		args[i].job = workloads[i];
		args[i].result = malloc(sizeof(int));

		int res =
			pthread_create(&worker_threads[i], 0, run_popcount64, &args[i]);
		if (res != 0) {
			printf("Failed to create thread #%d.\n", i);
			return 0;
		}
	}

	// Now we wait for the worker threads to finish
	for (int i = 0; i < nthreads; i++) {
		pthread_join(worker_threads[i], 0);
	}

	// Finally, process the results and free the memory
	uint64_t sum = 0;
	for (int i = 0; i < nthreads; i++) {
		sum += *(args[i].result);
		free(args[i].result);
	}

	free(workloads);
	free(worker_threads);
	free(args);

	return sum;
}
