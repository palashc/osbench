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
#include "file_cache.h"
#include "file_contention.h"
#include "disk_read.h"

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
  // run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, pow(2,20), 1024);
  // run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, pow(2,21), 1024);
  // run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, pow(2,22), 1024);
  // run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, pow(2,21) + pow(2,22), 1024);
  // run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, pow(2,23), 1024);
  //run_memoryAccessTest(m_benchmarkAccessTime, ITERATIONS, TRIALS, , 1024);
  // run_fileCache();
  // runContention(1);
  // runContention(1);
  // runContention(2);
  // runContention(3);
  // runContention(4);
  // runContention(5);
  // runContention(6);
  // runContention(7);
  // runContention(8);
  // runContention(9);
  // runContention(10);
  // runContention(20);
  // runContention(30);
  // runContention(40);
  // runContention(50);
  // runContention(60);
  // readFile("fs/4.txt");
  // readFileRand("fs/4.txt", 1, 1);

  readFileSeq("fs-remote/4.txt", 10, 1);
  readFileSeq("fs-remote/5.txt", 10, 1);
  readFileSeq("fs-remote/6.txt", 10, 1);
  readFileSeq("fs-remote/7.txt", 10, 1);
  readFileSeq("fs-remote/8.txt", 10, 1);
  readFileSeq("fs-remote/9.txt", 10, 1);
  readFileSeq("fs-remote/10.txt", 10, 1);

  readFileRand("fs-remote/4.txt", 10, 1);
  readFileRand("fs-remote/5.txt", 10, 1);
  readFileRand("fs-remote/6.txt", 10, 1);
  readFileRand("fs-remote/7.txt", 10, 1);
  readFileRand("fs-remote/8.txt", 10, 1);
  readFileRand("fs-remote/9.txt", 10, 1);
  readFileRand("fs-remote/10.txt", 10, 1);


  // readFile("fs-remote/4.txt", 100, 1);
  // readFile("fs-remote/5.txt", 80, 1);
  // readFile("fs-remote/6.txt", 70, 1);
  // readFile("fs-remote/7.txt", 60, 1);
  // readFile("fs-remote/8.txt", 30, 1);
  // readFile("fs-remote/9.txt", 20, 1);
  // readFile("fs-remote/10.txt", 10, 1);

  // readFileRand("fs-remote/4.txt", 10, 10);
  // readFileRand("fs-remote/5.txt", 80, 1);
  // readFileRand("fs-remote/6.txt", 70, 1);
  // readFileRand("fs-remote/7.txt", 60, 1);
  // readFileRand("fs-remote/8.txt", 30, 1);
  // readFileRand("fs-remote/9.txt", 20, 1);
  // readFileRand("fs-remote/10.txt", 10, 1);
}