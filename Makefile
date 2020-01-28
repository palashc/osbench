
default: main.c constants.h benchmark.h cpu_tests.h
	g++ -o test.out main.c 

clean: 
	rm -f *.out
	rm -f *.o
