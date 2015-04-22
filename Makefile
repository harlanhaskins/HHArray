CC=clang
EXENAME=HHArray
CFLAGS=-Wall -Wno-pointer-arith -Wno-gnu-empty-struct -Ofast -std=gnu99 -Wextra -pedantic -ggdb -march=native -flto -ffast-math
INCLUDE= -I./include
EXECUTABLES=$(EXENAME)
AR=ar
ARFLAGS=rvs

all: libhharray.a test

libhharray.a: HHArray.o utilities.o
	$(AR) $(ARFLAGS) libhharray.a HHArray.o utilities.o

HHArray.o: src/HHArray.c
	$(CC) -c $(CFLAGS) $(INCLUDE) src/HHArray.c

utilities.o: src/utilities.c
	$(CC) -c $(CFLAGS) $(INCLUDE) src/utilities.c

main.o: tests/main.c
	$(CC) -c $(CFLAGS) $(INCLUDE) tests/main.c

.PHONY: test
test: libhharray.a
	make -C tests

clean:
	rm *.o
	rm *.a

realclean purge: clean
	rm -rf $(EXENAME)
