MAKE = make
CC = gcc 
CFLAGS = -O3 -Wall -march=native -std=gnu99 -fopenmp
TRACK_FLAGS = -DTRACK_RESOURCES -DDEBUG

SOURCE = binpath_accelerated_benchmark.c

EXE = binpath_accelerated_benchmark

all: $(EXE)

binpath_accelerated_benchmark: $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $< -lm 

binpath_accelerated_benchmark-perf: $(SOURCE)
	$(CC) $(CFLAGS) $(TRACK_FLAGS) -o $@ $< -lm

clean:  
	rm -f *.o *.a *~ 
	rm -f $(EXE)