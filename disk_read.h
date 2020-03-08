#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "constants.h"
#include "cpu_tests.h"
#include "utils.h"
#include "limits.h"
#define BLOCKSIZE 4096
#define FILELOC "/dev/sda1"
uint64_t diskReadSeq(int iterations, int filesize){
	uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
	uint64_t arr[iterations];
	int offset = 0;
	int data;
	int fd = open(FILELOC, 'r' |'b' | O_RDONLY | __O_DIRECT);
	if(fd <= 0){
		printf("File not opened \n");
		return -1;
	}
	void * sector =  malloc(BLOCKSIZE);
	int aligned = posix_memalign(&sector, BLOCKSIZE, 4096);
	if(aligned != 0){
		printf("Memeory not aligned\n");
		return -1; 
	}
	uint64_t sum;
	for(int j = 0; j < iterations; j++){
		lseek(fd, offset, SEEK_SET);
		sum = 0;
		for(int i = 0; i < filesize; i++){
			asm volatile (
				"CPUID\n\t"
					"RDTSC\n\t"
					"mov %%edx, %0\n\t"
					"mov %%eax, %1\n\t": "=r" (cycles_high0), 
				"=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
			);
			int data = read(fd, sector, BLOCKSIZE);

			asm volatile (
				"RDTSCP\n\t"
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t": "=r" (cycles_high1), 
		    "=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
		    );

			if(data < 0)
				printf("data not read from filepointer %d, read returned %d at offset = %d at sector = %d with error=%s\n", fd, data, offset, i, strerror(errno));
			uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
	  		uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;
	  		sum += end - start;
		}
		arr[j] = sum/filesize;
	}
	free(sector);
	close(fd);
	return median(arr, iterations);
}

void runDiskReadTests(int iterations, int trials, int filesize){
  for(int size = 1; size < filesize; size *=2){
	  uint64_t trial_results[trials];
	  for(int i = 0; i < trials; i++){
	  	trial_results[i] = diskReadSeq(iterations, size*1024);
	  }
	  benchmark_stats stats = fill_stats(trial_results, trials);
	  printf("Testing seq disk read for a %d MB file\n", size*4);
	  printf("%d Trials of %d Iterations\n", trials, iterations);
	  printf("Mean: %f\n", stats.mean);
	  printf("Median: %ld\n", stats.median);
	  printf("StDev: %f\n", stats.stdev);

	  printf("Min: %ld\n", stats.min);
	  printf("Max: %ld\n", stats.max);
	}
}

uint64_t diskReadRand(int iterations, int filesize){
	uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
	uint64_t arr[iterations];
	int offset = 0;
	int data;
	int fd = open(FILELOC, 'r' |'b' | O_RDONLY | __O_DIRECT);
	if(fd <= 0){
		printf("File not opened \n");
		return -1;
	}
	void * sector =  malloc(BLOCKSIZE);
	int aligned = posix_memalign(&sector, BLOCKSIZE, 4096);
	if(aligned != 0){
		printf("Memeory not aligned\n");
		return -1; 
	}
	uint64_t sum;
	for(int j = 0; j < iterations; j++){
		sum = 0;
		for(int i = 0; i < filesize; i++){
					offset = (rand() % 5000) * 1024;

			lseek(fd, offset, SEEK_SET);
			asm volatile (
				"CPUID\n\t"
					"RDTSC\n\t"
					"mov %%edx, %0\n\t"
					"mov %%eax, %1\n\t": "=r" (cycles_high0), 
				"=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
			);
			int data = read(fd, sector, BLOCKSIZE);

			asm volatile (
				"RDTSCP\n\t"
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t": "=r" (cycles_high1), 
		    "=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
		    );

			if(data < 0)
				printf("data not read from filepointer %d, read returned %d at offset = %d at sector = %d with error=%s\n", fd, data, offset, i, strerror(errno));
			uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
	  		uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;
	  		sum += end - start;
		}
		arr[j] = sum/filesize;
	}
	free(sector);
	close(fd);
	return median(arr, iterations);
}

void runDiskReadTestsRand(int iterations, int trials, int filesize){
  for(int size = 1; size < filesize; size*=2){
	  uint64_t trial_results[trials];
	  for(int i = 0; i < trials; i++){
	  	trial_results[i] = diskReadRand(iterations, size*1024);
	  }
	  benchmark_stats stats = fill_stats(trial_results, trials);
	  printf("Testing rand disk read for a %d MB file\n", size*4);
	  printf("%d Trials of %d Iterations\n", trials, iterations);
	  printf("Mean: %f\n", stats.mean);
	  printf("Median: %ld\n", stats.median);
	  printf("StDev: %f\n", stats.stdev);

	  printf("Min: %ld\n", stats.min);
	  printf("Max: %ld\n", stats.max);
	}
}