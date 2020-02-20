#pragma once

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "benchmark.h"

uint64_t m_benchmarkAccessTime(void**, int, int); 

/*
 * Desc: Returns the head of a list-based circular linked list where
 *        every element of the linked list is at least `stride` away 
 *        from the next element. 
 * 
 *        For example if we have an array of size 10 with stride 3, we 
 *        shall have 3 stride runs that are chained in the following manner:
 *        
 *        head (0) -> 3 -> 6 -> 9 ->
 *              1  -> 4 -> 7 ->
 *              2  -> 5 -> 8 -> 0
 *        
 * Param:
 *    stride: the number of elements between every two adjacent nodes in
 *            the linked list
 *    bufsiz: the size of the underlying array datastructure that constitutes 
 *            the linked list
 * 
 * Returns:
 *    head (void**): Returns the head of the circular list. To deallocate the 
 *                   underlying memory of the list the programmer shall `free(head)`:w
 * 
 */
void** createCircularLL(int stride, int bufsiz) {

  // array of pointers
  void** buffer = malloc(bufsiz * sizeof(void*));

  // head of linked list
  void* head = NULL;

  // This is the tail node of the prev stride run
  void** prevTail = &head;

  // initialize buffer
  for (int j = 0; j < stride; j++) {
    *prevTail = &buffer[j];

    // beginning index of a stride run 
    int si;

    for (si = j; si < bufsiz - stride; si += stride) {
      buffer[si] = &(buffer[si+stride]);
    }

    prevTail = &buffer[si];
  }

  // make list circular
  *prevTail = head;

  return head;
}

void run_memoryAccessTest(m_ben_ptr m_benchmark, uint32_t iterations, int trials, int bufsiz, int stride) {  
  uint64_t trial_results[trials];
  uint64_t iteration_results[iterations];

  void** headLL = createCircularLL(stride, bufsiz);

  // Fill the cache with the buffer 
  // loop through the circular linked list
  void** tmp = headLL;
  for (int i = 0; i < bufsiz * ICACHE_HITS; i++) {
    tmp = *tmp;
  }

  // begin benchmark tests
  for (int i=0; i < trials; i++) {
    trial_results[i] = m_benchmark(headLL, bufsiz, iterations);
  }

  benchmark_stats stats = fill_stats(trial_results, trials);
  printf("Testing Memory Access Time on array of size: %d\n", bufsiz);
  printf("%d Trials of %d Iterations\n", trials, iterations);
  printf("Mean: %f\n", stats.mean);
  printf("Median: %ld\n", stats.median);
  printf("StDev: %f\n", stats.stdev);

  printf("Min: %ld\n", stats.min);
  printf("Max: %ld\n", stats.max);

  free(headLL);
}

uint64_t m_benchmarkAccessTime(void** headLL, int bufsiz, int iterations) {

  uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1,
            stridestart, i;
  
  int total_iterations = bufsiz * iterations;

  asm volatile (
    "CPUID\n\t"
      "RDTSC\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t": "=r" (cycles_high0), 
    "=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
  );

  for(i = 0; i < total_iterations; i++) {
    headLL = *headLL;
  }
  
  asm volatile (
    "RDTSCP\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t": "=r" (cycles_high1), 
    "=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
  );

  uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
  uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;

  return (end-start)/total_iterations;
}

uint64_t m_benchmarkMeasurementOverhead(void** headLL, int bufsiz, int iterations) {

  uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1,
            stridestart, i;
  
  int total_iterations = bufsiz * iterations;

  asm volatile (
    "CPUID\n\t"
      "RDTSC\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t": "=r" (cycles_high0), 
    "=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
  );

  for(i = 0; i < total_iterations; i++) {
    // empty loop
  }
  
  asm volatile (
    "RDTSCP\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t": "=r" (cycles_high1), 
    "=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
  );

  uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
  uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;

  return (end-start)/total_iterations;
}