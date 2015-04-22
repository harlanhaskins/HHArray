CC=clang
CFLAGS= -Wall -Wno-gnu-empty-struct -Wextra -Werror -pedantic -Ofast -ggdb -pipe -march=native
INCLUDE= -I../include
LFLAGS = -L../ -lhharray

.PHONY: all
all:
	$(CC) $(INCLUDE) -o test $(CFLAGS) main.c $(LFLAGS)
	./test

clean:
	rm test
	rm *.o
