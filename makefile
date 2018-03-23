# Define required macros here
SHELL = /bin/sh

OBJ = main.o grid.o image.o lodepng.o rules.o transform.o
CFLAGS = -std=c90 -pedantic
CC = gcc
INCLUDE =
LIB = -lm


all: CellularAutomaton
	rm -f images/*
	rm -f *.gch

main.o: lodepng.h grid.h image.h
image.o: lodepng.h grid.h image.h
grid.o: grid.h
lodepng.o: lodepng.h
rules.o: rules.h grid.h
transform.o: grid.h

CellularAutomaton: $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $(OBJ) $(LIB)

clean:
	-rm -f *.o

run: CellularAutomaton
	gnome-terminal --profile=Dev  -- bash -c "./CellularAutomaton ; echo TERMINATED ; read line"
