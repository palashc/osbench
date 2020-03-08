MY_OPT = -g -O0  
LIBS = -lm -pthread
default: main.c constants.h benchmark.h cpu_tests.h context_switch.h mem_tests.h
	gcc $(MY_OPT) -o test.out main.c $(LIBS) $(ARGS) 
clean: 
	rm -f *.out
	rm -f *.o

# for creating threads/processes
create:
	make ARGS="-DITERATIONS=10000 -DTRIALS=1000 -DICACHE_HITS=10000"

cs:
	make ARGS="-DITERATIONS=100 -DTRIALS=1000 -DICACHE_HITS=100"  

#for testing I would use below

test:
	make ARGS="-DITERATIONS=10 -DTRIALS=10 -DICACHE_HITS=10"

mem:
	make ARGS="-DITERATIONS=10 -DTRIALS=100 -DICACHE_HITS=100"
