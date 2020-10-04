CC=gcc
CFLAGS=-Wall
DFLAGS=-g

build: empty.c part1.c
	$(CC) $(CFLAGS) -o empty.x empty.c
	$(CC) $(CFLAGS) -o part1.x part1.c

.PHONY: clean
clean:
	rm -rf *.o $(OUT)
