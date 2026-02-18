CC=gcc
CFLAGS=-std=c99 -O2
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: raytracer

raytracer: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) raytracer output.ppm
