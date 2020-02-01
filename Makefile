
default: main.c constants.h benchmark.h cpu_tests.h
	gcc -O0 -o test.out main.c -lm

clean: 
	rm -f *.out
	rm -f *.o
