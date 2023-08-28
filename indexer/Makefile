# Makefile for indexer
# John Wang, CS50, May 2023

OBJS = indexer.o 
C = ../common
L = ../libcs50
LLIBS = $C/common.a $L/libcs50.a

# uncomment the following to turn on verbose memory logging
TESTING= #-DMEMTEST

CFLAGS =  -Wall -pedantic -std=c11 -ggdb $(TESTING)
CC = gcc 
MAKE = make

.PHONY: test valgrind clean

all: indexer indextest

indexer: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

# required dependencies
indexer.o: $L/webpage.h $L/mem.h $L/hashtable.h $L/file.h $L/counters.h
indexer.o: $C/index.h $C/pagedir.h $C/word.h
indextest.o: $C/index.h $C/pagedir.h $C/word.h


# expects `testing.sh`
test: indexer
	bash -v testing.sh &> testing.out


clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f indexer indextest
	rm -f core