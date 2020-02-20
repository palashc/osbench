#pragma once 

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

typedef struct {
	int readFD;
	int writeFD;
} PipeFd;

typedef struct {
	PipeFd parentFD;
	PipeFd childFD;
} Pipes;

Pipes getPipes() {

	int pipe1_fd[2];
	int pipe2_fd[2];

	pipe(pipe1_fd);
	pipe(pipe2_fd);

	// parent writes to where child reads
	// child writes to where parent reads
	PipeFd parentfd = {
		.readFD = pipe1_fd[0],
		.writeFD = pipe2_fd[1]
	};

	PipeFd childfd = {
		.readFD = pipe2_fd[0],
		.writeFD = pipe1_fd[1]
	};

	Pipes cs_pipes = {
		.parentFD = parentfd,
		.childFD = childfd
	};

	return cs_pipes;
}

void testContextSwitchTime(PipeFd pipefd) {

	char buf[1];

	for (int i = 0; i < SWITCHS; ++i)
	{
		read(pipefd.readFD, buf, 1);
		write(pipefd.writeFD, buf, 1);
		// fprintf(stderr, "process id: %d, finishing switch: %d\n", getpid(), i);
	}
}

void* threadStart(void *arg) {
	PipeFd * pipefd = (PipeFd *)arg;
	testContextSwitchTime(*pipefd);
		close(pipefd->readFD);
		close(pipefd->writeFD);
	return NULL;
}

uint64_t cs_benchmarkHelper(PipeFd pipefd, pthread_t* childThread) {

	char buf[1] = {'8'};
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
	total = total/(2*SWITCHS);
	return total;

}
//TODO: check return values of system calls
uint64_t benchmarkContextSwitchThread(fun_ptr _ignore)
{
	Pipes pipes = getPipes();

	pthread_t childThread;
	pthread_create(&childThread, NULL, threadStart, (void *)&(pipes.childFD));

	uint64_t cycles = cs_benchmarkHelper(pipes.parentFD, &childThread);
		close(pipes.parentFD.readFD);
		close(pipes.childFD.writeFD);
	return cycles;
}

uint64_t benchmarkContextSwitchProcess(fun_ptr _ignore) {
	Pipes pipes = getPipes();

	if (fork() == 0) {
		testContextSwitchTime(pipes.childFD);
			close(pipes.childFD.readFD);
			close(pipes.childFD.writeFD);
		exit(0);
	}
	else {
		uint64_t cycles = cs_benchmarkHelper(pipes.parentFD, NULL);
			close(pipes.parentFD.readFD);
			close(pipes.childFD.writeFD);

		return cycles;
	}	
}
