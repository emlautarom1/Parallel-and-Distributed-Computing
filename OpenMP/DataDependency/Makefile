WARNS=-Wextra -Wall -Wundef  -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
CC=gcc
OMP_NUM_THREADS=12
CFLAGS=$(WARNS)

info:
	@ echo "Info: Data Dependency"

build: src/main.c
	$(CC) $(CFLAGS) src/main.c -o build/main -fopenmp

run: build/main
	./build/main

.PHONY: clean
clean:
	@ -rm -f build/*
