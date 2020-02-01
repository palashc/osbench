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

struct Pipes {
	struct PipeFD parentFD;
	struct PipeFD childFD;
};

struct Pipes getPipes() {

	int parent_child_fd[2];
	int child_parent_fd[2];

	pipe(parent_child_fd);
	pipe(child_parent_fd);

	struct PipeFD parentfd = {
		.readFD = child_parent_fd[0],
		.writeFD = parent_child_fd[1]
	};

	struct PipeFD childfd = {
		.readFD = parent_child_fd[0],
		.writeFD = child_parent_fd[1]
	};


	struct Pipes pipes = {
		.parentFD = parentfd,
		.childFD = childfd
	};

	return pipes;
}

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

uint64_t benchmarkHelper(struct PipeFD pipefd, pthread_t* childThread) {

	char buf[2] = {'x'};
	write(pipefd.writeFD, buf, 1);

	uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
	asm volatile (
	"CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high0), 
	"=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
	);

	testContextSwitchTime(pipefd);

	asm volatile (
		"RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high1), 
	"=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
	);

	uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
	uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;

	if (childThread != NULL) {
		pthread_join(*childThread, NULL);
	}
	
	uint64_t total = end - start;
	total = total/2000;
	return total;

}
//TODO: check return values of system calls
uint64_t benchmarkContextSwitchThread()
{

	struct Pipes pipes = getPipes();

	pthread_t childThread;
	pthread_create(&childThread, NULL, threadStart, (void *)&(pipes.childFD));

	return benchmarkHelper(pipes.parentFD, &childThread);
}

uint64_t benchmarkContextSwitchProcess() {
	struct Pipes pipes = getPipes();

	if (fork() == 0) {
		testContextSwitchTime(pipes.childFD);
		return 0;
	}
	else {
		return benchmarkHelper(pipes.parentFD, NULL);
	}	

}


