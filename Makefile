# Declare targets that don't produce actual files
.PHONY: all run clean

all:
	gcc main.c -o my_program

run:
	./my_program

clean:
	rm -f my_program

