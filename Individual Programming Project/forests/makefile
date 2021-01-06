CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

.PHONY = all clean

all: forests

forests: BinarySearchTree.o operationsParser.o inputParser.o main.o
	@$(CC) -o $@ $^

BinarySearchTree.o: BinarySearchTree.c BinarySearchTree.h
	@$(CC) $(CFLAGS) -c $<

operationsParser.o: operationsParser.c operationsParser.h
	@$(CC) $(CFLAGS) -c $<

inputParser.o: inputParser.c inputParser.h operationsParser.h
	@$(CC) $(CFLAGS) -c $<

main.o: main.c BinarySearchTree.h inputParser.h
	@$(CC) $(CFLAGS) -c $<

clean:
	@rm -f *.o forests
