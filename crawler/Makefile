# Makefile for 'crawler' module
# made for TSE crawler by John Wang April 2023

OBJS = crawler.o 
C = ../common
L = ../libcs50
LLIBS = $C/common.a $L/libcs50.a

# uncomment the following to turn on verbose memory logging
TESTING= #-DMEMTEST

CFLAGS =  -Wall -pedantic -std=c11 -ggdb $(TESTING)
CC = gcc 
MAKE = make

crawler: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

.PHONY: test valgrind clean

# expects `testing.sh`
test: crawler
	bash -v testing.sh &> testing.out

# expects `valgrind.sh`
valgrind: crawler
	bash -v valgrind.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f crawler
	rm -f core
