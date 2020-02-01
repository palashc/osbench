
default: main.c context_switch.c constants.h benchmark.h cpu_tests.h
	gcc -O0 -o test.out main.c -lm
	gcc -pthread -o cswitch.out context_switch.c

clean: 
	rm -f *.out
	rm -f *.o
