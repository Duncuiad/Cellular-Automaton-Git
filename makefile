# Define required macros here
SHELL = /bin/sh

OBJ = main.o grid.o image.o lodepng.o
CFLAG =
CC = gcc
INCLUDE =
LIB = -lm


all: CellularAutomaton
	rm -f images/*

main.o: lodepng.h grid.h image.h
image.o: lodepng.h grid.h image.h
grid.o: grid.h
lodepng.o: lodepng.h

CellularAutomaton: $(OBJ)
	$(CC) $(CFLAG) $(INCLUDE) -o $@ $(OBJ) $(LIB)

clean:
	-rm -f *.o

run: CellularAutomaton
	gnome-terminal --profile=Dev  -- bash -c "./CellularAutomaton && echo TERMINATED && read line"
