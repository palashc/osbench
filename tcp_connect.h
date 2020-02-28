#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "constants.h"
#include "utils.h"
#include "benchmark.h"
#include "lib_tcp.h"

#define	GO_AWAY	signal(SIGALRM, exit); alarm(60 * 60);

// #define TCP_CONNECT (u_long)404042
// #define SOCKOPT_REUSE 0x0008
// #define SOCKOPT_NONE 0

typedef struct  _state {
	char * server;
}state_t;

uint64_t doClient(uint64_t iterations, void * cookie);
void serverMain();


void benchmarkConntectionOverhead(){
	state_t state;
	uint64_t trial_results[TRIALS];
	for(int j=0; j < TRIALS; j++){
		for(int i=0; i < ITERATIONS; i++){
			if(fork() == 0){
				serverMain();
				exit(0);
			}
		}

		trial_results[j] = doClient(ITERATIONS, &state);
	}

	benchmark_stats stats = fill_stats(trial_results, TRIALS);
	printf("Testing tcp connection overhead \n");
	printf("%d Trials of %d Iterations\n", TRIALS, ITERATIONS);
	printf("Mean: %f\n", stats.mean);
	printf("Median: %ld\n", stats.median);
	printf("StDev: %f\n", stats.stdev);

	printf("Min: %ld\n", stats.min);
	printf("Max: %ld\n", stats.max);

}

uint64_t doClient(uint64_t iterations, void * cookie){
	uint64_t start, end;
	state_t * state = (state_t * ) cookie;
	register char * server = state->server;
	register int sock;
	uint64_t arr[ITERATIONS];
	uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
	while(iterations-- > 0){
		asm volatile (
			"CPUID\n\t"
				"RDTSC\n\t"
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t": "=r" (cycles_high0), 
			"=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
		);

		sock = tcp_connect(server, TCP_CONNECT, SOCKOPT_REUSE);
		close(sock);

		asm volatile (
			"RDTSCP\n\t"
			"mov %%edx, %0\n\t"
			"mov %%eax, %1\n\t": "=r" (cycles_high1), 
	    "=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
	    );
	    start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
  		end = ((uint64_t)cycles_high1 << 32) | cycles_low1;
  		arr[iterations] = end - start;
	}
	return median(arr, ITERATIONS);
}

void serverMain(){
	int newsock, sock;
	char c ='1';

	GO_AWAY;
	sock = tcp_server(TCP_CONNECT, SOCKOPT_NONE|SOCKOPT_REUSE);
	while(1){
		newsock = tcp_accept(sock, SOCKOPT_NONE);
		if(read(newsock, &c, 1) > 0){
			tcp_done(TCP_CONNECT);
			exit(0);
		}
		close(newsock);
	}
}