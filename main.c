#include <stdio.h>
#include <unistd.h>

#include "constants.h"
#include "cpu_tests.h"
#include "benchmark.h"
#include "utils.h"

// 10000 iterations empirically adds the test's instruction address in the I-Cache
#define ITERATIONS 100000
#define TRIALS 1000


int main() {
  uint64_t trial_results[TRIALS];

  // we perform ITERATIONS iterations of a test, and select the median to put in our
  // trial_results array
  uint64_t iteration_results[ITERATIONS];

  for (int i=0; i < TRIALS; i++) {
    for (int j=0; j< ITERATIONS; j++) {
      iteration_results[j] = benchmarkCycles(testMeasurementOverhead);
    }

    trial_results[i] = median(iteration_results, ITERATIONS);
    // printf("Result #%d is %ld\n", i, trial_results[i]);
  }

  printf("%d Trials of %d Iterations\n", TRIALS, ITERATIONS);
  printf("Mean: %ld\n", mean(trial_results, TRIALS));
  printf("Median: %ld\n", median(trial_results, TRIALS));
  printf("StDev: %f\n", stdev(trial_results, TRIALS));
}

