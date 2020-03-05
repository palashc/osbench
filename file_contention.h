#include <stdio.h>
#include "benchmark.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define BLOCK_SIZE 4096

void runContention(const int PROCESS_COUNT) {
	int fd = open("fs/64m.o", 'r');

	long filesize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	// we will only ever access first block of file. We advise that first two blocks don't need to be cached
	int advice = posix_fadvise(fd, 0, filesize, POSIX_FADV_DONTNEED);

	int* childPIDs = malloc(PROCESS_COUNT * sizeof(int));
	char buffer[BLOCK_SIZE] = {0};
	char testname[50] = {0};

	// contend for a block. We run a total of i processes
	// including parent process
	for (int i=1; i < PROCESS_COUNT; i++) {
		childPIDs[i] = fork();
		if (childPIDs[i] == 0) {
			// only terminate from a kill signal
			while(1) {
				lseek(fd, 0, SEEK_SET);
				read(fd, buffer, BLOCK_SIZE);
			}
		}
	}

	void setup() {
		lseek(fd, 0, SEEK_SET);
	};
	void test() {
		read(fd, buffer, BLOCK_SIZE);
	};

	sprintf(testname, "Block access time contending with %d processes", PROCESS_COUNT);

	runTestSetup(benchmarkCycles, test, testname, 1, 1000000, setup);

	for (int i=1; i< PROCESS_COUNT; i++) {
		fprintf(stderr, "Killing %d/%d pid: %d\n", i, PROCESS_COUNT, childPIDs[i]);
		kill(childPIDs[i], SIGKILL);
	}

	close(fd);
}

