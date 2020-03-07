/*
 * File: benchmark.h
 * Desc: Contains RDTSC cycle-counting benchmarks. All code to be benchmarked is 
 *       inline so that the compiler ensures no instructions other than the ones
 *       we test are benchmarked.
 */
#pragma once

#include "constants.h"
#include "cpu_tests.h"
#include "utils.h"
#include "limits.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
  uint32_t siz; 

  uint64_t median;
  uint64_t min;
  uint64_t max;

  double mean;
  double stdev;
} benchmark_stats;

extern benchmark_stats fill_stats(uint64_t* arr, uint32_t siz) {
  benchmark_stats stats = {0};

  stats.siz = siz;
  stats.median = median(arr, siz);
  stats.mean = mean(arr,siz);
  stats.stdev = stdev(arr, stats.mean, siz);

  uint64_t min = LONG_MAX;

  // the smallest unsigned number is 0
  uint64_t max = 0;
  for (int i=0; i<siz; i++) {
    if(arr[i] < min){
      min = arr[i];
    }
    if (arr[i] > max){
      max = arr[i];
    }
  }

  stats.min = min;
  stats.max = max;

  return stats;
}

extern uint64_t benchmarkEmpty(fun_ptr test) {
  uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
  asm volatile (
    "CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high0), 
    "=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
  );
  
	asm volatile (
		"RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high1), 
    "=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
  );

  uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
  uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;

  return end-start;
}

extern uint64_t benchmarkCycles(fun_ptr test) {
  uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
  asm volatile (
    "CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high0), 
    "=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
  );
  
  // time function below
  //testMeasurementOverhead();
  test();

	asm volatile (
		"RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high1), 
    "=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
  );

  uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
  uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;

  return end-start;
}


extern uint64_t benchmarkFork(fun_ptr _ignore) {
  uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
  
  pid_t parentId = getpid();

  asm volatile (
    "CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high0), 
    "=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
  );
  
  fork();

	asm volatile (
		"RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high1), 
    "=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
  );

  uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
  uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;

  if (getpid() != parentId) {
    exit(0);
  } 
  else {
    // this ensures that we minimize the total number of logical scheduling units low
    // the more schedulable threads that exist, the larger the time-lapse may be from
    // being context_switched out and being allocated the CPU again.
    wait(NULL);
  }

  return end-start;
}

void* testThreadf(void * arg){ }

extern uint64_t benchmarkThread(fun_ptr _ignore) {
  uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
  
  pthread_t tid;

  asm volatile (
    "CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high0), 
    "=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
  );
  

	pthread_create(&tid, NULL, &testThreadf, NULL);

	asm volatile (
		"RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high1), 
    "=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
  );

  uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
  uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;

  // this ensures that we minimize the total number of logical scheduling units low
  // the more schedulable threads that exist, the larger the time-lapse may be from
  // being context_switched out and being allocated the CPU again.
  pthread_join(tid, NULL);

  return end-start;
}

void runTest(ben_ptr benchmark, fun_ptr test, const char* name, uint32_t iterations, uint32_t trials) {

  uint64_t trial_results[trials];
  uint64_t iteration_results[iterations];

  // First, warm up I-Cache with 10,000,000 calls.
  // MUST disable gcc optimizations for this to work
  for (int i = 0; i < ICACHE_HITS; i++) {
    benchmark(test);
  }

  // begin benchmark tests
  for (int i=0; i < trials; i++) {
    for (int j=0; j< iterations; j++) {
      iteration_results[j] = benchmark(test);
    }

    // select median of all iteration tests per trial
    trial_results[i] = median(iteration_results, iterations);
  }

  benchmark_stats stats = fill_stats(trial_results, trials);
  printf("Testing %s\n", name);
  printf("%d Trials of %d Iterations\n", trials, iterations);
  printf("Mean: %f\n", stats.mean);
  printf("Median: %ld\n", stats.median);
  printf("StDev: %f\n", stats.stdev);

  printf("Min: %ld\n", stats.min);
  printf("Max: %ld\n", stats.max);
  printf("\n");
}

extern void runTestQuarantine(ben_ptr benchmark, fun_ptr test, const char* name, uint32_t iterations, uint32_t trials) {
  fprintf(stderr, "First run\n");
  benchmark(test);

  fprintf(stderr, "Second run\n");
  benchmark(test);

  fprintf(stderr, "tests completed correctly");
}

void runTestMod(ben_ptr benchmark, fun_ptr test, const char* name, uint32_t iterations, uint32_t trials, mod_ptr mod) {

  uint64_t trial_results[trials];
  uint64_t iteration_results[iterations];

  // First, warm up I-Cache with 10,000,000 calls.
  // MUST disable gcc optimizations for this to work
  for (int i = 0; i < ICACHE_HITS; i++) {
    benchmark(test);
  }

  // begin benchmark tests
  for (int i=0; i < trials; i++) {
    for (int j=0; j< iterations; j++) {
      iteration_results[j] = mod(benchmark(test));
    }

    // select median of all iteration tests per trial
    trial_results[i] = median(iteration_results, iterations);
  }

  benchmark_stats stats = fill_stats(trial_results, trials);
  printf("Testing %s\n", name);
  printf("%d Trials of %d Iterations\n", trials, iterations);
  printf("Mean: %f\n", stats.mean);
  printf("Median: %ld\n", stats.median);
  printf("StDev: %f\n", stats.stdev);

  printf("Min: %ld\n", stats.min);
  printf("Max: %ld\n", stats.max);
  printf("\n");
}

void runTestSetup(ben_ptr benchmark, fun_ptr test, const char* name, uint32_t iterations, uint32_t trials, fun_ptr setup) {

  uint64_t trial_results[trials];
  uint64_t iteration_results[iterations];

  // First, warm up I-Cache with 10,000,000 calls.
  // MUST disable gcc optimizations for this to work
  for (int i = 0; i < ICACHE_HITS; i++) {
    setup();
    benchmark(test);
  }

  // begin benchmark tests
  for (int i=0; i < trials; i++) {
    for (int j=0; j< iterations; j++) {
      setup();
      iteration_results[j] = benchmark(test);
    }

    // select median of all iteration tests per trial
    trial_results[i] = median(iteration_results, iterations);
  }

  benchmark_stats stats = fill_stats(trial_results, trials);
  printf("Testing %s\n", name);
  printf("%d Trials of %d Iterations\n", trials, iterations);
  printf("Mean: %f\n", stats.mean);
  printf("Median: %ld\n", stats.median);
  printf("StDev: %f\n", stats.stdev);

  printf("Min: %ld\n", stats.min);
  printf("Max: %ld\n", stats.max);
  printf("\n");
}