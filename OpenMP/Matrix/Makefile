WARNS=-Wextra -Wall -Wundef  -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
CC=gcc
OMP_NUM_THREADS=12
ROWS=600
COLS=800
CFLAGS=$(WARNS) -fopenmp -O0

info:
	@ echo "Info: Matrix Multiplication"

build: src/main.c
	$(CC) $(CFLAGS) src/main.c -o build/main

run: build/main
	./build/main $(ROWS) $(COLS)

.PHONY: clean
clean:
	@ -rm -f build/*
