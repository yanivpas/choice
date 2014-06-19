CC = clang
CFLAGS = -pedantic -Werror

all: test1

test1: test1.c
	$(CC)  $(CFLAGS)  -o $@ $<

clean:
	rm test1

.PHONY:all clean

