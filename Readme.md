Run with `taskset -c 1 nice -20 ./test.out`

## Benchmarking tests
We run the cpu-tests on a single thread in a single process. Thus, they run on a single core.

We wrap the codepaths to be benchmarked inside of a function of type `void (*fun_ptr)()`. We then pass a pointer to that function to our benchmarking code. This adds overhead in the form of performing another function `call` and `ret` on top of the codepath being benchmarked. We found that the first hundred thousand iterations are 3x of the future iterations. This is likely because the compiler is then able to cache the memory address of the function pointer in the I-Cache. 

For this reason, we run 10,000,000 cycles of a test before benchmarking the test, to push function call addresses into the I-Cache of our machine.

---

### CPU Performance benchmarking
For all CPU benchmarks we performed 1000 trials that consist of the median of 100,000 iterations of benchmarking a test. All benchmarks measure the cycles taken for a program.

1. **Measurement Overhead**:

    1000 Trials of 100000 Iterations\
    Mean: 40.000000\
    Median: 40\
    StDev: 1.732051\
    Min: 38\
    Max: 60

2. **Testing Call method with 0 parameters**:

    1000 Trials of 100000 Iterations\
    Mean: 43.000000\
    Median: 42\
    StDev: 3.316625\
    Min: 40\
    Max: 66

3. **Testing Call method with 1 parameters**
    1000 Trials of 100000 Iterations\
    Mean: 41.000000\
    Median: 42\
    StDev: 1.414214\
    Min: 40\
    Max: 51

4.  **Testing Call method with 2 parameters**
    1000 Trials of 100000 Iterations\
    Mean: 43.000000\
    Median: 43\
    StDev: 1.414214\
    Min: 40\
    Max: 61

5. **Testing Call method with 2 parameters**
    1000 Trials of 100000 Iterations\
    Mean: 44.000000\
    Median: 43\
    StDev: 3.464102\
    Min: 40\
    Max: 67

6. **Testing Call method with 3 parameters**
    1000 Trials of 100000 Iterations\
    Mean: 43.000000\
    Median: 44\
    StDev: 2.236068\
    Min: 41\
    Max: 57

7. **Testing Call method with 4 parameters**
    1000 Trials of 100000 Iterations\
    Mean: 45.000000\
    Median: 44\
    StDev: 2.828427\
    Min: 42\
    Max: 67

8. **Testing Call method with 5 parameters**
    1000 Trials of 100000 Iterations\
    Mean: 45.000000\
    Median: 44\
    StDev: 2.828427\
    Min: 42\
    Max: 67

9. **Testing Call method with 6 parameters**
    1000 Trials of 100000 Iterations\
    Mean: 45.000000\
    Median: 46\
    StDev: 1.000000\
    Min: 42\
    Max: 58

10. **Testing Call method with 7 parameters**
    1000 Trials of 100000 Iterations\
    Mean: 48.000000\
    Median: 46\
    StDev: 4.898979\
    Min: 44\
    Max: 75

11. **Testing System call: clock_gettime**
    1000 Trials of 100000 Iterations\
    Mean: 106.000000\
    Median: 105\
    StDev: 4.690416\
    Min: 103\
    Max: 175

12. **Testing Kernel Thread Creation**
    1000 Trials of 10000 Iterations\
    Mean: 19895.000000\
    Median: 19945\
    StDev: 737.404231\
    Min: 17620\
    Max: 23297

13. **Testing Fork Creation**
    1000 Trials of 10000 Iterations\
    Mean: 129011.000000\
    Median: 129206\
    StDev: 1812.436482\
    Min: 116493\
    Max: 162737