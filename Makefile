CC=gcc
CFLAGS=-g -Og -Wall -Wextra -ansi -Wpedantic

sh: sh.o
	$(CC) -o sh sh.o
sh.o: sh.c
	$(CC) $(CFLAGS) -c sh.c

format:
	clang-format -i -style=llvm *.c

.PHONY:
	format
