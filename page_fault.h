#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

size_t get_file_size(const char *filename) {
	struct stat st;
	stat(filename, &st);
	return st.st_size;
}

uint64_t run_pf(uint32_t iterations, uint32_t trials)
{
	const char* FILENAME = "fs/7g.o";

	size_t file_size = get_file_size(FILENAME);
	int fd = open(FILENAME, O_RDONLY);

	int advice = posix_fadvise(fd, 0, file_size, __POSIX_FADV_DONTNEED);
	char* addr = mmap(NULL, file_size, PROT_READ|PROT_WRITE, MAP_FILE|MAP_PRIVATE, fd, 0);
	
	srand(time(NULL));
	long n;
	
	void setup() {
		// rand() only outputs integers up until 65311. Multiply by large prime
		n = (rand() * 999331)  % file_size;
	}; 

	void test() {
		char nth_byte = addr[n];
	};
	
	runTestSetup(benchmarkCycles, test, "page fault time", iterations, trials, setup);
	int unmap_result = munmap(addr, file_size);
	close(fd);
}
