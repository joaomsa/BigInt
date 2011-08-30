CC=gcc
CFLAGS=-Wall -Wextra -ansi -g -O3
EXECUTABLE=tp0
all: $(EXECUTABLE)

$(EXECUTABLE): main.o bignum.o list.o
	$(CC) -o $@ main.o list.o bignum.o

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ main.c

bignum.o: bignum.c
	$(CC) $(CFLAGS) -c -o $@ bignum.c

list.o: list.c
	$(CC) $(CFLAGS) -c -o $@ list.c

clean:
	rm -f main.o bignum.o list.o $(EXECUTABLE)

.PHONY: clean
