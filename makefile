# Define required macros here
SHELL = /bin/sh

OBJ = grid.o image.o lodepng.o init.o rules.o transform.o
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
init.o: init.h
lodepng.o: lodepng.h
rules.o: rules.h grid.h
transform.o: grid.h

boolTest.o: lodepng.h grid.h image.h

CellularAutomaton: main.o $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ main.o $(OBJ) $(LIB)

boolTest.x: boolTest.o $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ boolTest.o $(OBJ) $(LIB)

clean:
	-rm -f *.o

run: all
	gnome-terminal --profile=Dev  -- bash -c "./CellularAutomaton ; echo TERMINATED ; read line"
