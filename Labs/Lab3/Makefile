MAKE = make
CC = gcc 
CFLAGS = -O3 -Wall -march=native -std=gnu99 -fopenmp
TRACK_FLAGS = -DTRACK_RESOURCES -DDEBUG

SOURCE = binpath_accelerated.c

EXE = binpath_accelerated

all: $(EXE)

binpath_accelerated: $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $< -lm 

binpath_accelerated-perf: $(SOURCE)
	$(CC) $(CFLAGS) $(TRACK_FLAGS) -o $@ $< -lm

clean:  
	rm -f *.o *.a *~ 
	rm -f $(EXE)