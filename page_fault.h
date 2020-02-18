#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t get_file_size(const char *filename) {
	struct stat st;
	stat(filename, &st);
	return st.st_size;
}

uint64_t benchmark_pf()
{

	size_t file_size = get_file_size("page_fault.h");
	int fd = open("page_fault.h", O_RDONLY);

	char* addr = mmap(NULL, file_size, PROT_READ, MAP_FILE|MAP_PRIVATE, fd, 0);

	size_t pagesize = getpagesize();


	uint32_t cycles_high0, cycles_low0, cycles_low1, cycles_high1;
	asm volatile (
	"CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high0), 
	"=r" (cycles_low0)::"rax", "%rbx", "%rcx", "%rdx"
	);

	char first_byte = addr[0];

	asm volatile (
		"RDTSCP\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high1), 
	"=r" (cycles_low1)::"rax", "%rbx", "%rcx", "%rdx"
	);

	uint64_t start = ((uint64_t)cycles_high0 << 32) | cycles_low0;
	uint64_t end = ((uint64_t)cycles_high1 << 32) | cycles_low1;
	uint64_t total = end - start;

	close(fd);

	return total;
}
