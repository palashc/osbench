#pragma once

#include <stdio.h>
#include <time.h>

#include "benchmark.h"

#define BLOCK_SIZE 4096

const long MB = 1024*1024;
const long GB = 1024*1024*1024;

// const char* FILES[] = {
// 	"fs/64m.o", "fs/128m.o", "fs/512m.o", 
// 	"fs/1g.o", "fs/2g.o", "fs/6g.o",
// 	"fs/8g.o", "fs/12g.o", "fs/16g.o"
// };

const char* FILES[] = {
	"fs/128m.o", "fs/7g.o", "fs/13g.o", "fs/15g.o", 
};
#define FILE_NUM 3

void run_fileCache() {

	const int iterations = 10;
	const int cache_iterations = 10;
	char *blockbuf = malloc(BLOCK_SIZE);
	posix_memalign(&blockbuf, BLOCK_SIZE, BLOCK_SIZE);

	char testname[50] = {0};

	for (int i = 0; i < FILE_NUM; i++) {
		const char* filename = FILES[i];

		FILE* fd = fopen(filename, "r");
		fseek(fd, 0L, SEEK_END);
		const long filesize = ftell(fd);
		const long blockcount = filesize / BLOCK_SIZE;

		void test() {
			// bring file pointer to the top
			fseek(fd, 0, SEEK_SET);
			while (fread(blockbuf, sizeof(char), BLOCK_SIZE, fd) == BLOCK_SIZE);
			printf("not ok");
		};

		// perform operation on the cycles returned
		uint64_t mod(uint64_t benchmark_result) {
			return benchmark_result / blockcount;
		}

		sprintf(testname, "4KB block access time on %ld size file", filesize);

		// run the "mod" operation on the result of every benchmark(test) call
		runTestMod(benchmarkCycles, test, testname, 1, 10, mod);
		
		fclose(fd);
	}
}

