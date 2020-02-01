#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "constants.h"

#define SWITCHS 1000

struct PipeFD {
	int readFD;
	int writeFD;
};

void testContextSwitchTime(struct PipeFD pipefd) {

	char buf[1];

	for (int i = 0; i < SWITCHS; ++i)
	{
		read(pipefd.readFD, buf, 1);
		write(pipefd.writeFD, buf, 1);
	}
}

void* threadStart(void *arg) {
	struct PipeFD * pipefd = (struct PipeFD *)arg;
	testContextSwitchTime(*pipefd);
	return NULL;
}

//TODO: check return values of system calls
uint64_t benchmarkContextSwitch()
{

	int parent_child_fd[2];
	int child_parent_fd[2];

	pipe(parent_child_fd);
	pipe(child_parent_fd);

	struct PipeFD parentFD = {
		.readFD = child_parent_fd[0],
		.writeFD = parent_child_fd[1]
	};

	struct PipeFD childFD = {
		.readFD = parent_child_fd[0],
		.writeFD = child_parent_fd[1]
	};

	pthread_t childThread;
	pthread_create(&childThread, NULL, threadStart, (void *)&childFD);

	char buf[2] = {'x'};
	write(parentFD.writeFD, buf, 1);

  uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
  asm volatile (
    "CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high0), 
    "=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
  );
  
	testContextSwitchTime(parentFD);

	asm volatile (
		"RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high1), 
    "=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
  );

  uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
  uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;
	pthread_join(childThread, NULL);
uint64_t total = end - start;
total = total/2000;
return total;
}


