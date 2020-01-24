## Benchmarking tests
We run the cpu-tests on a single thread in a single process. Thus, they run on a single core.

We wrap the codepaths to be benchmarked inside of a function of type `void (*fun_ptr)()`. We then pass a pointer to that function to our benchmarking code. This adds overhead in the form of performing another function `call` and `ret` on top of the codepath being benchmarked. We found that the first hundred thousand iterations are 3x of the future iterations. This is likely because the compiler is then able to cache the memory address of the function pointer in the I-Cache. For this reason, we perform multiple trials of 100000 iterations.

On 1000 Trials of 100000 iterations, we found the measurement cycle time to take 38 cycles.

