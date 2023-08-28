# Makefile for 'common' 
# Made for TSE crawler by John Wang April 2023

L = ../libcs50

LIB = common.a

OBJS = pagedir.o index.o word.o
FLAGS = 
MAKE = make

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L
CC = gcc
MAKE = make

.PHONY: all clean


$(LIB): $(OBJS) 
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h $L/webpage.h $L/file.h
index.o: index.h
word.o: word.h


clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f $(LIB)
