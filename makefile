# Define required macros here
SHELL = /bin/sh

OBJ = grid.o image.o lodepng.o init.o rules.o transform.o debug.o IO.o
CFLAGS = -std=c90 -pedantic
CC = gcc
INCLUDE =
LIB = -lm


all: CellularAutomaton
	rm -f images/*

main.o: grid.h image.h rules.h debug.h init.h
image.o: lodepng.h grid.h image.h debug.h
grid.o: grid.h
init.o: init.h
lodepng.o: lodepng.h
rules.o: rules.h grid.h debug.h
transform.o: grid.h
debug.o: debug.h
IO.o: IO.h debug.h
test.o: grid.h image.h rules.h debug.h init.h IO.h transform.h

test: $(OBJ) test.o
	-rm -f *.gch
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $(OBJ) test.o $(LIB)

runTest: test
	gnome-terminal --profile=Dev  -- bash -c "./test ; echo TERMINATED TEST ; read line"

CellularAutomaton: $(OBJ) main.o
	-rm -f *.gch
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $(OBJ) main.o $(LIB)

clean:
	-rm -f *.o

run: CellularAutomaton
	gnome-terminal --profile=Dev  -- bash -c "./CellularAutomaton ; echo TERMINATED ; read line"
