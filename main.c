#include <stdio.h>
#include <unistd.h>

#include "constants.h"
#include "cpu_tests.h"
#include "benchmark.h"
#include "utils.h"
#include "context_switch.h"

// 10000 iterations empirically adds the test's instruction address in the I-Cache
#if !defined(ITERATIONS)
#define ITERATIONS 100000 //for testing change to 10
#endif
#if !defined(TRIALS)
#define TRIALS 1000
#endif

void runTest(fun_ptr test, const char* name, uint32_t iterations, uint32_t trials);

int main() {
  runTest(testMeasurementOverhead, "Measurement Overhead", ITERATIONS, TRIALS);
}

void runTest(fun_ptr test, const char* name, uint32_t iterations, uint32_t trials) {

  uint64_t trial_results[trials];
  uint64_t iteration_results[iterations];

  // First, warm up I-Cache with 10,000,000 calls.
  // MUST disable gcc optimizations for this to work
  for (int k=0; k < ICACHE_HITS; k++) {
    benchmarkCycles(test);
  }

  // begin benchmark tests
  for (int i=0; i < trials; i++) {
    for (int j=0; j< iterations; j++) {
      iteration_results[j] = benchmarkContextSwitchThread();

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
