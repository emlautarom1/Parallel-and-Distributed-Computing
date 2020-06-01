WARNS=-Wextra -Wall -Wundef  -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
CC=gcc
OMP_NUM_THREADS=12
CFLAGS=$(WARNS) -fopenmp

info:
	@ echo "Info: Loops"

build: src/main-a.c src/main-b.c
	$(CC) $(CFLAGS) src/main-a.c -o build/main-a
	$(CC) $(CFLAGS) src/main-b.c -o build/main-b

run-a: build/main-a
	./build/main-a

run-b: build/main-b
	./build/main-b

.PHONY: clean
clean:
	@ -rm -f build/*
