WARNS=-Wextra -Wall -Wundef  -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
CC=gcc
OMP_NUM_THREADS=12
CFLAGS=$(WARNS)

info:
	@ echo "Info: Task Distribution"

build: src/main_a.c src/main_b.c
	$(CC) $(CFLAGS) src/main_a.c -o build/main_a -fopenmp
	$(CC) $(CFLAGS) src/main_b.c -o build/main_b -fopenmp

run-a: build/main_a
	./build/main_a

run-b: build/main_b
	./build/main_b

.PHONY: clean
clean:
	@ -rm -f build/*
