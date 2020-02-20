#include <stdio.h>
#include <unistd.h>

#include "constants.h"
#include "cpu_tests.h"
#include "benchmark.h"
#include "utils.h"
#include "context_switch.h"
#include "page_fault.h"
#include "mem_tests.h"

// 10000 iterations empirically adds the test's instruction address in the I-Cache
#if !defined(ITERATIONS)
#define ITERATIONS 100000 //for testing change to 10
#endif

#if !defined(TRIALS)
#define TRIALS 1000
#endif

void runTest(ben_ptr, fun_ptr, const char*, uint32_t, uint32_t);
void runTestQuarantine(ben_ptr, fun_ptr, const char*, uint32_t, uint32_t);

int main() {
  // runTest(benchmarkCycles, testMeasurementOverhead, "Measurement Overhead", ITERATIONS, TRIALS);
  // runTest(benchmarkCycles, testCall0, "Call method with 0 parameters", ITERATIONS, TRIALS);
  // runTest(benchmarkCycles, testCall1, "Call method with 1 parameters", ITERATIONS, TRIALS);
  // runTest(benchmarkCycles, testCall2, "Call method with 2 parameters", ITERATIONS, TRIALS);
  // runTest(benchmarkCycles, testCall3, "Call method with 3 parameters", ITERATIONS, TRIALS);
  // runTest(benchmarkCycles, testCall4, "Call method with 4 parameters", ITERATIONS, TRIALS);
  // runTest(benchmarkCycles, testCall5, "Call method with 5 parameters", ITERATIONS, TRIALS);
  // runTest(benchmarkCycles, testCall6, "Call method with 6 parameters", ITERATIONS, TRIALS);
  // runTest(benchmarkCycles, testCall7, "Call method with 7 parameters", ITERATIONS, TRIALS);
  // runTest(benchmarkCycles, testSystemCall, "System call: clock_gettime", ITERATIONS, TRIALS);
  // runTest(benchmarkThread, NULL, "System call: test kernel thread", ITERATIONS, TRIALS);
  // runTest(benchmarkThread, NULL, "Test kernel thread", ITERATIONS, TRIALS);
  // runTest(benchmarkFork, NULL, "Test fork thread", ITERATIONS, TRIALS);
  // runTest(benchmarkContextSwitchThread, NULL, "Context Switch Thread", ITERATIONS, TRIALS);
  // runTest(benchmarkContextSwitchProcess, NULL, "Context switch Process", ITERATIONS, TRIALS);
   runTest(benchmark_pf, NULL, "Page Fault access time + RAM access time", ITERATIONS, TRIALS); 

  // run_memoryAccessTest(ITERATIONS, TRIALS, 100000, 1024);
  // run_memoryAccessTest(ITERATIONS, TRIALS, 200000, 1024);
  // run_memoryAccessTest(ITERATIONS, TRIALS, 300000, 1024);
  // run_memoryAccessTest(ITERATIONS, TRIALS, 400000, 1024);
  // run_memoryAccessTest(ITERATIONS, TRIALS, 500000, 1024);
  // run_memoryAccessTest(m_benchmarkMeasurementOverhead, ITERATIONS, TRIALS, 600000, 1024);
  // run_memoryAccessTest(m_benchmarkMeasurementOverhead, ITERATIONS, TRIALS, 700000, 1024);
  // run_memoryAccessTest(m_benchmarkMeasurementOverhead, ITERATIONS, TRIALS, 800000, 1024);
  // run_memoryAccessTest(m_benchmarkMeasurementOverhead, ITERATIONS, TRIALS, 900000, 1024);
  // run_memoryAccessTest(m_benchmarkMeasurementOverhead, ITERATIONS, TRIALS, 1000000, 1024);
  // run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, 600000, 1024);
  // run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, 700000, 1024);
  // run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, 800000, 1024);
  // run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, 900000, 1024);
  // run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, 1000000, 1024);
  run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, pow(2,20), 1024);
  run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, pow(2,21), 1024);
  run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, pow(2,22), 1024);
  run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, pow(2,21) + pow(2,22), 1024);
  run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, pow(2,23), 1024);
  //run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, , 1024);
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

      fprintf(stderr, "#%d\tFILE DESCRIPTOR SIZE IS: %d\n", i, getdtablesize());
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
}

void runTestQuarantine(ben_ptr benchmark, fun_ptr test, const char* name, uint32_t iterations, uint32_t trials) {
  fprintf(stderr, "First run\n");
  benchmark(test);

  fprintf(stderr, "Second run\n");
  benchmark(test);

  fprintf(stderr, "tests completed correctly");
}
