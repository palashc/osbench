/*
 * File: benchmark.h
 * Desc: Contains RDTSC cycle-counting benchmarks. All code to be benchmarked is 
 *       inline so that the compiler ensures no instructions other than the ones
 *       we test are benchmarked.
 */
#pragma once

#include "constants.h"
#include "cpu_tests.h"

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

