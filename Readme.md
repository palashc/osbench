# OSBench
OSBench is a benchmaring framework and test suite designed to benchmark an Ubuntu operating system on a Thinkpad X1-carbon thinkpad. 

The results can be seen in the report [here](https://drive.google.com/file/d/1UagEpavcYhuJLwtviOf8LLfi5PORz7Id/view?usp=sharing).

## Benchmarking framework
We prioritized creating a robust benchmarking framework above creating benchmarks for specific tests. See `benchmark.h`. 

Our library consists of _benchmark_ methods are used by _run_ methods to time the CPU cycles by code segments using raw assembly RDTSC/RDTSCP blocks. 

Currently three benchmark methods in the library exist:
- `benchmarkCycles`: takes an incoming parameter `void (*test)()`, and then measure cycles taken by `test()` between RDTSC/RDTSCP blocks
- `benchmarkFork`: benchmarks the `fork` system call between RDTSC/RDTSCP blocks but makes sure to kill child process
`benchmarkThread`: benchmarks the `pthread_create` call between RDTSC/RDTSCP blocks but makes sure to kill child thread

The _run_ methods report the results of running benchmarks for a given test over many trials, each trial consisting of many iterations. These methods allow for performing transformations on the results of _benchmark_ programs, (e.g. converting cycles to seconds) or for performing a setup operation before every _benchmark_ call (e.g. resetting an existing file descriptor to offset 0 before scanning through a file).

Unless stated otherwise, all machine benchmarks are performed by invoking _run_ methods in `main.c`, compiling with optimizations turned off, and finally running with a niceness of -20. For CPU tests, the executable was additionally pinned to a single core using the `taskset` command.

## Benchmarking tests
Tests are specified in `main.c` in runs.

CPU tests included the following runs:

```c++
runTest(benchmarkCycles, testMeasurementOverhead, "Measurement Overhead", ITERATIONS, TRIALS);
runTest(benchmarkCycles, testCall0, "Call method with 0 parameters", ITERATIONS, TRIALS);
runTest(benchmarkCycles, testCall1, "Call method with 1 parameters", ITERATIONS, TRIALS);
runTest(benchmarkCycles, testCall2, "Call method with 2 parameters", ITERATIONS, TRIALS);
runTest(benchmarkCycles, testCall3, "Call method with 3 parameters", ITERATIONS, TRIALS);
runTest(benchmarkCycles, testCall4, "Call method with 4 parameters", ITERATIONS, TRIALS);
runTest(benchmarkCycles, testCall5, "Call method with 5 parameters", ITERATIONS, TRIALS);
runTest(benchmarkCycles, testCall6, "Call method with 6 parameters", ITERATIONS, TRIALS);
runTest(benchmarkCycles, testCall7, "Call method with 7 parameters", ITERATIONS, TRIALS);
runTest(benchmarkCycles, testSystemCall, "System call: clock_gettime", ITERATIONS, TRIALS);
runTest(benchmarkThread, NULL, "System call: test kernel thread", ITERATIONS, TRIALS);
runTest(benchmarkThread, NULL, "Test kernel thread", ITERATIONS, TRIALS);
runTest(benchmarkFork, NULL, "Test fork thread", ITERATIONS, TRIALS);
runTest(benchmarkContextSwitchThread, NULL, "Context Switch Thread", ITERATIONS, TRIALS);
runTest(benchmarkContextSwitchProcess, NULL, "Context switch Process", ITERATIONS, TRIALS);
```

Memory tests included the following runs:
```c++ 
runTest(benchmark_pf, NULL, "Page Fault access time + RAM access time", ITERATIONS, TRIALS); 
runTest(benchmarkReadRamBandwidth, NULL, "RAM Bandwidth read", ITERATIONS, TRIALS);
runTest(benchmarkWriteRamBandwidth, NULL, "RAM Bandwidth write", ITERATIONS, TRIALS);
run_memoryAccessTest(m_benchmarkMeasurementOverhead, ITERATIONS, TRIALS, 600000, 1024);
```

File system tests tested on files in a local `fs` directory that is not tracked in this git repo. This `fs` directory contained hundreds of files that ranged from 64Mb to 20Gb in size. Also, they relied on an `fs-remote` directory that is a remote AWS micro's directory mounted locally through `ssh-fs`. Neither are the private keys for the Micro nor the mounted files are tracked in this git repo.

File system tests included the following runs:
```c++
run_filecache();
runContention(1);
...
runContention(100);
readFileSeq("fs/4.txt");
readFileRand("fs-remote/4.txt")
```


## Network test
Network tests were written and evaluated in Python. We chose python because the network overhead of measuring bandwidth and RTT is so much larger in magnitude than the overhead of the Python interpretter that results still provide accurate measurements of the system. Also, all members enjoyed increased developer productivity from this decision due to all-around familiarity with and simplicity of Python networking. 

All network tests exist in the `network` directory.
