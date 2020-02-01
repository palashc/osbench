MY_OPT = -O0 -o 
LIBS = -lm -pthread
default: main.c constants.h benchmark.h cpu_tests.h context_switch.h
	g++ $(MY_OPT) test.out main.c $(LIBS) $(ARGS) 
clean: 
	rm -f *.out
	rm -f *.o
#for testing I would use below
#make ARGS="-DITERATIONS=100 -DTRIALS=10 -DICACHE_HITS=10"