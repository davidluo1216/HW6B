CC=gcc
CFLAGS=-g -Wall -std=c99
DFLAGS=

default: bench

debug: DFLAGS += -DDEBUG
debug: bench

bench: bench.o getmem.o freemem.o mem_utils.o
	$(CC) $(CFLAGS) $(DFLAGS) -o bench bench.o getmem.o freemem.o mem_utils.o

bench.o: bench.c mem.h
	$(CC) $(CFLAGS) $(DFLAGS) -c bench.c

getmem.o: getmem.c mem.h mem_impl.h
	$(CC) $(CFLAGS) $(DFLAGS) -c getmem.c

freemem.o: freemem.c mem.h mem_impl.h
	$(CC) $(CFLAGS) $(DFLAGS) -c freemem.c

mem_utils.o: mem_utils.c mem.h mem_impl.h
	$(CC) $(CFLAGS) $(DFLAGS) -c mem_utils.c

test: bench
	./bench

clean:
	rm -f bench *.o
