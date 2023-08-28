# Makefile for querier
# John Wang, CS50, May 2023

OBJS = querier.o fuzzquery.o
C = ../common
L = ../libcs50
LLIBS = $C/common.a $L/libcs50.a

# uncomment the following to turn on verbose memory logging
TESTING= #-DMEMTEST

CFLAGS =  -Wall -pedantic -std=c11 -ggdb $(TESTING)
CC = gcc 
MAKE = make

.PHONY: all test valgrind clean

all: querier fuzzquery

querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

fuzzquery: fuzzquery.o $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

# expects `testing.sh`
test: querier fuzzquery
	bash -v testing.sh &> testing.out


clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f querier fuzzquery
	rm -f core