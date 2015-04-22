CC=clang
CFLAGS= -Wall -Wno-gnu-empty-struct -Wextra -Werror -pedantic -O2 -pipe -march=native
DEBUG_CFLAGS= -Wall -Wextra -Werror -pedantic -O0 -g -pipe -DDEBUG_MSG_ENABLE
INCLUDE= -I../include
LFLAGS = -L../ -lhharray

.PHONY: all
all:
	$(CC) $(INCLUDE) -o test $(CFLAGS) main.c $(LFLAGS)
	./test

clean:
	rm test
	rm *.o
