#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "benchmark.h"

// Cache associativity are 8-way for L1, 4-way for L2, 16-way for L3
// 16 was chosen arbitrarily; 
#define SETUP_ITERATIONS 16

uint64_t m_benchmarkAccessTime(uint8_t* buffer, uint32_t siz, uint32_t setup_iterations, uint8_t* forgotme) {

  uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;

  char ignore;
  // loop over array first to push char to higher level of memory access
  for (int i=0; i < setup_iterations; i++) {
    for (int j = 0; j < siz; j++) {
      ignore = buffer[j];
    }
  }

  asm volatile (
    "CPUID\n\t"
      "RDTSC\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t": "=r" (cycles_high0), 
    "=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
  );

  // assume dereference brigns forgotme into L1 cache
  *forgotme = ignore;
  
  asm volatile (
    "RDTSCP\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t": "=r" (cycles_high1), 
    "=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
  );

  uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
  uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;

  return (end-start);
}

void run_memoryAccessTest(uint32_t iterations, uint32_t trials, int32_t siz) {
  uint64_t trial_results[trials];
  uint64_t iteration_results[iterations];

  // we measure time to retrieve memory address for this char
  uint8_t *dontforgetme = malloc(sizeof(uint8_t));
  uint8_t *buffer = malloc(siz * sizeof(uint8_t));

  // derefernce buffer and bring it into L1 cache
  int _ = *dontforgetme;

  // Fill the cache with the buffer 
  // MUST disable gcc optimizations for this to work
  for (int i = 0; i < ICACHE_HITS; i++) {
    for (int j = 0; j < siz; j++) {
      int b = buffer[j];
    }
  }

  // begin benchmark tests
  for (int i=0; i < trials; i++) {
    for (int j=0; j< iterations; j++) {
      iteration_results[j] = m_benchmarkAccessTime(buffer, siz, SETUP_ITERATIONS, dontforgetme);
    }

    // select median of all iteration tests per trial
    trial_results[i] = median(iteration_results, iterations);
  }

  benchmark_stats stats = fill_stats(trial_results, trials);
  printf("Testing Memory Access Time on array of size: %d\n", siz);
  printf("%d Trials of %d Iterations\n", trials, iterations);
  printf("Mean: %f\n", stats.mean);
  printf("Median: %ld\n", stats.median);
  printf("StDev: %f\n", stats.stdev);

  printf("Min: %ld\n", stats.min);
  printf("Max: %ld\n", stats.max);

  free(dontforgetme);
  free(buffer);
}