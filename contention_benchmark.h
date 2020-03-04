#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "constants.h"
#include "contention_helper.h"

#define ITERS 1000
#define BLOCKSIZE 4096

uint64_t rdtsc() {
	uint32_t cycles_high, cycles_low;

	asm volatile (
	"CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high), 
	"=r" (cycles_low)::"rax", "%rbx", "%rcx", "%rdx"
	);

	return ((uint64_t)cycles_high << 32) | cycles_low;
}


uint64_t benchmarkContention(int num_processes)
{
	const char* filename = "contention_test.txt";
	size_t filesize = _get_file_size(filename);
	int fd = open(filename, O_RDONLY);
	int advice = posix_fadvise(fd, 0, filesize, POSIX_FADV_DONTNEED);

	char *readBuf = (char *)malloc(BLOCKSIZE);
	uint64_t total_cycles = 0;
	uint64_t start_t, end_t;

	startBackgroundJobs(num_processes);

	for (int i = 0; i < ITERS; ++i)
	{	
		start_t = rdtsc();
		read(fd, readBuf, BLOCKSIZE);
		end_t = rdtsc();
		total_cycles += (end_t - start_t);
		lseek(fd, 0, SEEK_SET);
	}

	killJobs();
	
	return (uint64_t) total_cycles / ITER;
}
uint64_t benchmarkContention0() { benchmarkContention(0); }
uint64_t benchmarkContention1() { benchmarkContention(1); }
uint64_t benchmarkContention2() { benchmarkContention(2); }
uint64_t benchmarkContention3() { benchmarkContention(3); }
uint64_t benchmarkContention4() { benchmarkContention(4); }
uint64_t benchmarkContention5() { benchmarkContention(5); }
uint64_t benchmarkContention6() { benchmarkContention(6); }
uint64_t benchmarkContention7() { benchmarkContention(7); }
uint64_t benchmarkContention8() { benchmarkContention(8); }
uint64_t benchmarkContention9() { benchmarkContention(9); }
uint64_t benchmarkContention10() { benchmarkContention(10); }
