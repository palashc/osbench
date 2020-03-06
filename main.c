#include <stdio.h>
#include <unistd.h>

#include "constants.h"
#include "cpu_tests.h"
#include "benchmark.h"
#include "utils.h"
#include "context_switch.h"
#include "page_fault.h"
#include "ram_bandwidth.h"
#include "mem_tests.h"
#include "file_contention.h"

// 10000 iterations empirically adds the test's instruction address in the I-Cache
#if !defined(ITERATIONS)
#define ITERATIONS 1 //for testing change to 10
#endif

#if !defined(TRIALS)
#define TRIALS 1000
#endif

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
  // runTest(benchmark_pf, NULL, "Page Fault access time + RAM access time", ITERATIONS, TRIALS); 
  // runTest(benchmarkReadRamBandwidth, NULL, "RAM Bandwidth read", ITERATIONS, TRIALS);
  // runTest(benchmarkWriteRamBandwidth, NULL, "RAM Bandwidth write", ITERATIONS, TRIALS);
  //  runTest(benchmarkContention0, NULL, "Contention with 0 processes", ITERATIONS, TRIALS);
  //  runTest(benchmarkContention1, NULL, "Contention with 1 processes", ITERATIONS, TRIALS);
  //  runTest(benchmarkContention2, NULL, "Contention with 2 processes", ITERATIONS, TRIALS);
  runContention(1);
  runContention(2);
  runContention(3);
  runContention(4);
  runContention(5);
  runContention(6);
  runContention(7);
  runContention(8);
  runContention(9);
  runContention(10);
  runContention(20);
  runContention(30);
  runContention(40);
  runContention(50);
  runContention(60);
  runContention(70);
  runContention(80);
  runContention(90);
  runContention(100);
}