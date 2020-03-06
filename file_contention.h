#include <stdio.h>
#include "benchmark.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>

#include <errno.h>

#define BLOCK_SIZE 4096

void runContention(const int PROCESS_COUNT) {

	int* childPIDs = malloc(PROCESS_COUNT * sizeof(int));
	char *buffer = malloc(BLOCK_SIZE);
	posix_memalign(&buffer, BLOCK_SIZE, BLOCK_SIZE);

	char testname[50] = {0};
	char filename[50] = {0};

	// contend for a block. We run a total of i processes
	// including parent process
	for (int i=1; i < PROCESS_COUNT; i++) {
		childPIDs[i] = fork();
		if (childPIDs[i] == 0) {
			// only terminate from a kill signal
			sprintf(filename, "fs/%d.txt", i);
			int fd = open(filename, 'r' | O_RDWR | O_SYNC | __O_DIRECT);

			long filesize = lseek(fd, 0, SEEK_END);
			long blockcount = filesize/BLOCK_SIZE;
			lseek(fd, 0, SEEK_SET);

			// we will only ever access first block of file. We advise that first two blocks don't need to be cached
			int advice = posix_fadvise(fd, 0, filesize, POSIX_FADV_DONTNEED);

			while(1) {
				// read random location
				int setTo = (rand() % (blockcount - 1))*BLOCK_SIZE;
				lseek(fd, setTo, SEEK_SET);
				// read file location
				read(fd, buffer, BLOCK_SIZE);
			}
		}
	}

	int fd = open("fs/49.txt", O_RDWR | O_SYNC | __O_DIRECT);
	long filesize = lseek(fd, 0, SEEK_END);
	long blockCount = filesize / BLOCK_SIZE;

	lseek(fd, 0, SEEK_SET);

	fprintf(stderr, "file size is %ld\n", filesize);

	// we will only ever access first block of file. We advise that first two blocks don't need to be cached
	int advice = posix_fadvise(fd, 0, filesize, POSIX_FADV_DONTNEED);

	void setup() {
		// read random location
		int setTo = (rand() % (blockCount - 1))*BLOCK_SIZE;
		lseek(fd, setTo, SEEK_SET);
	};
	void test() {
		read(fd, buffer, BLOCK_SIZE);
	};

	sprintf(testname, "Block access time contending with %d processes", PROCESS_COUNT);
	sleep(2);
	runTestSetup(benchmarkCycles, test, testname, 1, 10000, setup);

	for (int i=1; i< PROCESS_COUNT; i++) {
		// fprintf(stderr, "Killing %d/%d pid: %d\n", i, PROCESS_COUNT, childPIDs[i]);
		kill(childPIDs[i], SIGKILL);
	}

	close(fd);
}

