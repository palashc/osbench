#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "constants.h"

#define SIZE 0.5 // size of array in GB
#define FREQ 2.4 //processor frequency in GHz

//returns bandwidth in MBPS
uint64_t benchmarkReadRamBandwidth(fun_ptr _ignore)
{
	long size = (long)(SIZE * 1024 * 1024 * 1024);
	double temp;
	double *arr = (double *) malloc(size);

	uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
	asm volatile (
	"CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high0), 
	"=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
	);

	for (int i = 0; i < (size/sizeof(double))-16; i+=16)
	{
		temp = arr[i];
		temp = arr[i+1];
		temp = arr[i+2];
		temp = arr[i+3];
		temp = arr[i+4];
		temp = arr[i+5];
		temp = arr[i+6];
		temp = arr[i+7];
		temp = arr[i+8];
		temp = arr[i+9];
		temp = arr[i+10];
		temp = arr[i+11];
		temp = arr[i+12];
		temp = arr[i+13];
		temp = arr[i+14];
		temp = arr[i+15];
	}
	
	asm volatile (
		"RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high1), 
	"=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
	);

	uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
	uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;
	uint64_t total = end - start;

	free(arr);
	// time in seconds = cycles / frequency in Hz
	// bandwidth = size / time
	return (uint64_t) ((SIZE*1024*FREQ*pow(10, 9))/(double) total);
}

//returns bandwidth in MBPS
uint64_t benchmarkWriteRamBandwidth(fun_ptr _ignore)
{
	long size = (long)(SIZE * 1024 * 1024 * 1024);
	double temp = 0.0;
	double *arr = (double *) malloc(size);

	uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
	asm volatile (
	"CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high0), 
	"=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
	);

	for (int i = 0; i < (size/sizeof(double))-16; i+=16)
	{
		arr[i] = temp;
		arr[i+1] = temp;
		arr[i+2] = temp;
		arr[i+3] = temp;
		arr[i+4] = temp;
		arr[i+5] = temp;
		arr[i+6] = temp;
		arr[i+7] = temp;
		arr[i+8] = temp;
		arr[i+9] = temp;
		arr[i+10] = temp;
		arr[i+11] = temp;
		arr[i+12] = temp;
		arr[i+13] = temp;
		arr[i+14] = temp;
		arr[i+15] = temp;
	}
	
	asm volatile (
		"RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high1), 
	"=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
	);

	uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
	uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;
	uint64_t total = end - start; 

	free(arr);
	// time in seconds = cycles / frequency in Hz
	// bandwidth = size / time	
	return (uint64_t) ((SIZE*1024*FREQ*pow(10, 9))/(double) total);
}
