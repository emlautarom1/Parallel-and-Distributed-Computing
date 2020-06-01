WARNS=-Wextra -Wall -Wundef  -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
CC=mpicc
NP=3
ROWS=4
COLS=6
CFLAGS=$(WARNS)

info:
	@ echo "Info: Matrix Multiplication"

build: src/main.c
	$(CC) $(CFLAGS) src/main.c -o build/main

run: build/main
	mpirun -np $(NP) build/main $(ROWS) $(COLS)

.PHONY: clean
clean:
	@ -rm -f build/*
