#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1

uint64_t benchmarkReadRamBandwidth(fun_ptr _ignore)
{
	int size = SIZE * 1024 * 1024 * 1024;
	char temp;
	char *arr = (char *) malloc(size);

	clock_t start, end;
	start = clock();

	for (int i = 0; i < size-20; i+=20)
	{
		temp = arr[i];
		temp = arr[i+1];
		temp = arr[i+2];
		temp = arr[i+3];
		temp = arr[i+4];
		temp = arr[i+5];
		temp = arr[i+6];
		temp = arr[i+7];
		temp = arr[i+8];
		temp = arr[i+9];
		temp = arr[i+10];
		temp = arr[i+11];
		temp = arr[i+12];
		temp = arr[i+13];
		temp = arr[i+14];
		temp = arr[i+15];
		temp = arr[i+16];
		temp = arr[i+17];
		temp = arr[i+18];
		temp = arr[i+19];
	}
	
	end = clock();
	double time_taken = ((double)(end-start))/CLOCKS_PER_SEC; 

	free(arr);
	return (uint64_t) (SIZE*1024)/time_taken;
}


uint64_t benchmarkWriteRamBandwidth(fun_ptr _ignore)
{
	int size = SIZE * 1024 * 1024 * 1024;
	char temp = 'x';
	char *arr = (char *) malloc(size);

	clock_t start, end;
	start = clock();

	for (int i = 0; i < size-20; i+=20)
	{
		arr[i] = temp;
		arr[i+1] = temp;
		arr[i+2] = temp;
		arr[i+3] = temp;
		arr[i+4] = temp;
		arr[i+5] = temp;
		arr[i+6] = temp;
		arr[i+7] = temp;
		arr[i+8] = temp;
		arr[i+9] = temp;
		arr[i+10] = temp;
		arr[i+11] = temp;
		arr[i+12] = temp;
		arr[i+13] = temp;
		arr[i+14] = temp;
		arr[i+15] = temp;
		arr[i+16] = temp;
		arr[i+17] = temp;
		arr[i+18] = temp;
		arr[i+19] = temp;
	}
	
	end = clock();
	double time_taken = ((double)(end-start))/CLOCKS_PER_SEC; 

	free(arr);
	return (uint64_t) (SIZE*1024)/time_taken;
}
