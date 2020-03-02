#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "constants.h"
#include "cpu_tests.h"
#include "utils.h"
#include "limits.h"
#define BLOCKSIZE 512
#define FILELOC "/dev/sda1"
uint64_t diskReadSeq(int length){
	uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
	uint64_t arr[length];
	int offset = 0;
	int data;
	int fd = open("/dev/sda1", O_RDONLY | __O_DIRECT);
	printf("%x\n", fd);
	if(fd <= 0){
		printf("File not opened \n");
		return -1;
	}
	void * sector =  malloc(BLOCKSIZE);
	int aligned = posix_memalign(sector, BLOCKSIZE, BLOCKSIZE);
	if(aligned != 0){
		printf("Memeory not aligned\n");
		return -1; 
	}
	lseek(fd, offset, SEEK_SET);

	for(int i = 0; i < length; i++){
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
			printf("data not read, read returned %d at offset = %d at sector = %d with error=%s\n", data, offset, i, strerror(errno));
		uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
  		uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;
  		arr[i] = end - start;
	}
	free(sector);
	close(fd);
	return median(arr, length);
}