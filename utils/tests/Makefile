CC = clang
CFLAGS = -pedantic -Werror

all: test1 test2

test1: test1.c
	$(CC)  $(CFLAGS)  -o $@ $<

test2: test2.c
	$(CC)  $(CFLAGS)  -o $@ $<

clean:
	rm test1 test2

.PHONY:all clean

