# Declare targets that don't produce actual files
.PHONY: all run clean

CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = my_program
SRCS = main.c utils.c parser.c builtin.c ls.c
OBJS = $(SRCS:.c=.o)
HEADERS = utils.h parser.h builtins.h ls.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
