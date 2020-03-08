#include <stdio.h>
#include <string.h>
#include "benchmark.h"
#include "constants.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>

#include <errno.h>

#define BLOCK_SIZE 4096

void readFile(const char* filename, int trials, int iterations) {

	char *buffer = (char*) malloc(BLOCK_SIZE);
	posix_memalign(&buffer, BLOCK_SIZE, BLOCK_SIZE);

	int fd = open(filename, O_RDWR | O_SYNC | __O_DIRECT);

	long filesize = lseek(fd, 0, SEEK_END);
	long blockCount = filesize / BLOCK_SIZE;

	lseek(fd, 0, SEEK_SET);

	fprintf(stderr, "file size is %ld\n", filesize);
	fprintf(stderr, "Oh dear, something went wrong with open()! %s\n", strerror(errno));


	// we will only ever access first block of file. We advise that first two blocks don't need to be cached
	int advice = posix_fadvise(fd, 0, filesize, POSIX_FADV_DONTNEED);

	// no setup required
	void setup() {
		lseek(fd, 0, SEEK_SET);
	};

	void test() {
		while(read(fd, buffer, BLOCK_SIZE) == BLOCK_SIZE);
	};

	const char* testname = "Disk Read Time";
	runTestSetup(benchmarkCycles, test, testname, iterations, trials, setup);

	free(buffer);
	close(fd);
}

/* Arrange the N elements of ARRAY in random order.
   Only effective if N is much smaller than RAND_MAX;
   if this may not be the case, use a better random
   number generator. */
void shuffle(long *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

void readFileSeq(const char* filename, int trials, int iterations) {

	char *buffer = (char*) malloc(BLOCK_SIZE);
	posix_memalign(&buffer, BLOCK_SIZE, BLOCK_SIZE);

	int fd = open(filename, O_RDWR | O_SYNC | __O_DIRECT);
	long filesize = lseek(fd, 0, SEEK_END);
	long blockCount = filesize / BLOCK_SIZE;
	long blockLocations[blockCount];

	for (int i=0; i < blockCount; i++ ) {
		blockLocations[i] = i*BLOCK_SIZE;
	}

	lseek(fd, 0, SEEK_SET);
	fprintf(stderr, "file size is %ld\n", filesize);

	// we will only ever access first block of file. We advise that first two blocks don't need to be cached
	int advice = posix_fadvise(fd, 0, filesize, POSIX_FADV_DONTNEED);

	void test() {
		read(fd, buffer, BLOCK_SIZE);
	};

	uint64_t trial_results[trials];
	uint64_t iteration_results[iterations];

	for (int i=0; i < trials; i++) {
		memset(iteration_results, 0, sizeof(iteration_results));

		for (int j=0; j < iterations; j++) {
			for (int k = 0; k < blockCount; k++) {
				lseek(fd, blockLocations[k], SEEK_SET);
				iteration_results[j] += benchmarkCycles(test);
			}
		}
		trial_results[i] = median(iteration_results, iterations);
	}

	benchmark_stats stats = fill_stats(trial_results, trials);
	printf("Testing Seq File Reads\n");
	printf("%d Trials of %d Iterations\n", trials, iterations);
	printf("Mean: %f\n", stats.mean);
	printf("Median: %ld\n", stats.median);
	printf("StDev: %f\n", stats.stdev);

	printf("Min: %ld\n", stats.min);
	printf("Max: %ld\n", stats.max);
	printf("\n");

	free(buffer);
	close(fd);
}
void readFileRand(const char* filename, int trials, int iterations) {

	char *buffer = (char*) malloc(BLOCK_SIZE);
	posix_memalign(&buffer, BLOCK_SIZE, BLOCK_SIZE);

	int fd = open(filename, O_RDWR | O_SYNC | __O_DIRECT);
	long filesize = lseek(fd, 0, SEEK_END);
	long blockCount = filesize / BLOCK_SIZE;
	long blockLocations[blockCount];

	for (int i=0; i < blockCount; i++ ) {
		blockLocations[i] = i*BLOCK_SIZE;
	}

	// shuffle through all block locations
	shuffle(blockLocations, blockCount);

	lseek(fd, 0, SEEK_SET);
	fprintf(stderr, "file size is %ld\n", filesize);

	// we will only ever access first block of file. We advise that first two blocks don't need to be cached
	int advice = posix_fadvise(fd, 0, filesize, POSIX_FADV_DONTNEED);

	void test() {
		read(fd, buffer, BLOCK_SIZE);
	};

	uint64_t trial_results[trials];
	uint64_t iteration_results[iterations];

	for (int i=0; i < trials; i++) {
		memset(iteration_results, 0, sizeof(iteration_results));

		for (int j=0; j < iterations; j++) {
			for (int k = 0; k < blockCount; k++) {
				lseek(fd, blockLocations[k], SEEK_SET);
				iteration_results[j] += benchmarkCycles(test);
			}
		}
		trial_results[i] = median(iteration_results, iterations);
	}

	benchmark_stats stats = fill_stats(trial_results, trials);
	printf("Testing Random File Reads\n");
	printf("%d Trials of %d Iterations\n", trials, iterations);
	printf("Mean: %f\n", stats.mean);
	printf("Median: %ld\n", stats.median);
	printf("StDev: %f\n", stats.stdev);

	printf("Min: %ld\n", stats.min);
	printf("Max: %ld\n", stats.max);
	printf("\n");

	free(buffer);
	close(fd);
}
