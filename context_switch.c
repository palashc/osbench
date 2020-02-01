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

#define ITERATIONS 100000

struct PipeFD {
	uint32_t readFD;
	uint32_t writeFD;
};

void testContextSwitchTime(struct PipeFD pipefd) {

	char buf[1];

	for (int i = 0; i < ITERATIONS; ++i)
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
int main(int argc, char const *argv[])
{

	uint32_t parent_child_fd[2];
	uint32_t child_parent_fd[2];

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

	clock_t start, end;
	start = clock();
	testContextSwitchTime(parentFD);
	end = clock();
	double time_taken = ((double)(end-start))/CLOCKS_PER_SEC; 

	pthread_join(childThread, NULL);

	printf("Time per context switch %f ns\n", (time_taken * 1000000000)/ (2 * ITERATIONS)); 

	return 0;
}


