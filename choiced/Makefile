CC = gcc
CFLAGS = -pedantic -Werror -Wall
CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)

.PHONY:
	all clean

all: choiced

choiced : $(OFILES)
	$(CC) $(CFLAGS) $^ -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf choiced *.o
