CC=clang
EXENAME=HHArray
CFLAGS=-Wall -Wno-gnu-empty-struct -Ofast -std=gnu99 -Wextra -pedantic -g -march=native -flto -ffast-math
CFILES=main.c utilities.c HHArray.c
COBJFILES=$(CFILES:c=o)
CASSEMBLYFILES=$(CFILES:c=s)
EXECUTABLES=$(EXENAME)

all: $(EXENAME)

$(EXENAME): $(COBJFILES)

clean:
	rm -rf $(COBJFILES)
	rm -rf $(CASSEMBLYFILES)

realclean purge: clean
	rm -rf $(EXECUTABLES)
	rm -rf $(COUTPUT)
