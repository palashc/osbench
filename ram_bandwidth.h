#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "constants.h"

#define SIZE 64 // size of array in MB
#define FREQ 2.9 //processor frequency in GHz
#define ITER 10000

//returns bandwidth in MBPS
uint64_t benchmarkReadRamBandwidth(fun_ptr _ignore)
{
	long size = (long)(SIZE * 1024 * 1024);
	int temp = 0.0;
	int *arr = (int *) malloc(size);
	uint64_t bw = 0;
	for (int i=0; i<ITER; i++) {
		uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
		asm volatile (
		"CPUID\n\t"
			"RDTSC\n\t"
			"mov %%edx, %0\n\t"
			"mov %%eax, %1\n\t": "=r" (cycles_high0), 
		"=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
		);

		for (int i = 0; i < (size/sizeof(double))-480; i+=480)
		{	

			temp += arr[i];
			temp += arr[i+16];
			temp += arr[i+32];
			temp += arr[i+48];
			temp += arr[i+64];
			temp += arr[i+80];
			temp += arr[i+96];
			temp += arr[i+112];
			temp += arr[i+128];
			temp += arr[i+144];
			temp += arr[i+160];
			temp += arr[i+176];
			temp += arr[i+192];
			temp += arr[i+208];
			temp += arr[i+224];
			temp += arr[i+240];
			temp += arr[i+256];
			temp += arr[i+272];
			temp += arr[i+288];
			temp += arr[i+304];
			temp += arr[i+320];
			temp += arr[i+336];
			temp += arr[i+352];
			temp += arr[i+368];
			temp += arr[i+384];
			temp += arr[i+400];
			temp += arr[i+416];
			temp += arr[i+432];
			temp += arr[i+448];
			temp += arr[i+464];
		
		}
		
		asm volatile (
			"RDTSCP\n\t"
			"mov %%edx, %0\n\t"
			"mov %%eax, %1\n\t": "=r" (cycles_high1), 
		"=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
		);

		long start = ((long)cycles_high0 << 32) | cycles_low0;
		long end = ((long)cycles_high1 << 32) | cycles_low1;
		long total = end - start;
		bw += (uint64_t)((SIZE*FREQ*pow(10, 9))/total);
	}

	free(arr);
	return (uint64_t)bw/ITER;
}

//returns bandwidth in MBPS
uint64_t benchmarkWriteRamBandwidth(fun_ptr _ignore)
{
	long size = (long)(SIZE * 1024 * 1024);
	int temp = 1;
	int *arr = (int *) malloc(size);
	uint64_t bw = 0;
	for (int i=0; i<ITER; i++) {
		uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
		asm volatile (
		"CPUID\n\t"
			"RDTSC\n\t"
			"mov %%edx, %0\n\t"
			"mov %%eax, %1\n\t": "=r" (cycles_high0), 
		"=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
		);

		for (int i = 0; i < (size/sizeof(double))-480; i+=480)
		{	

			arr[i] = temp;
			arr[i+16] = temp;
			arr[i+32] = temp;
			arr[i+48] = temp;
			arr[i+64] = temp;
			arr[i+80] = temp;
			arr[i+96] = temp;
			arr[i+112] = temp;
			arr[i+128] = temp;
			arr[i+144] = temp;
			arr[i+160] = temp;
			arr[i+176] = temp;
			arr[i+192] = temp;
			arr[i+208] = temp;
			arr[i+224] = temp;
			arr[i+240] = temp;
			arr[i+256] = temp;
			arr[i+272] = temp;
			arr[i+288] = temp;
			arr[i+304] = temp;
			arr[i+320] = temp;
			arr[i+336] = temp;
			arr[i+352] = temp;
			arr[i+368] = temp;
			arr[i+384] = temp;
			arr[i+400] = temp;
			arr[i+416] = temp;
			arr[i+432] = temp;
			arr[i+448] = temp;
			arr[i+464] = temp;
		
		}
		
		asm volatile (
			"RDTSCP\n\t"
			"mov %%edx, %0\n\t"
			"mov %%eax, %1\n\t": "=r" (cycles_high1), 
		"=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
		);

		long start = ((long)cycles_high0 << 32) | cycles_low0;
		long end = ((long)cycles_high1 << 32) | cycles_low1;
		long total = end - start;
		bw += (uint64_t)((SIZE*FREQ*pow(10, 9))/total);
	}

	free(arr);
	return (uint64_t)bw/ITER;
}
