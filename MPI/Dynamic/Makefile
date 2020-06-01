WARNS=-Wextra -Wall -Wundef  -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
CC=mpicc
NP=1
CFLAGS=$(WARNS)

info:
	@ echo "Info: Dynamic Process"

build: src/manager.c src/worker.c
	$(CC) $(CFLAGS) src/manager.c -o build/manager
	$(CC) $(CFLAGS) src/worker.c -o build/worker

run: build/manager
	mpirun -np $(NP) build/manager

.PHONY: clean
clean:
	@ -rm -f build/*
