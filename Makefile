# Declare targets that don't produce actual files
.PHONY: all run clean

CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: my_program

my_program: main.c utils.c utils.h parser.c parser.h
	$(CC) $(CFLAGS) main.c utils.c parser.c -o my_program

run: my_program
	./my_program

clean:
	rm -f my_program
